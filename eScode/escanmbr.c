/*
 * Developed By the Developer Ketan Mhaske
 * mbr_crypto_rc4.c - MBR/VBR Encryption with RC4
 * 
 * ENCRYPTION:
 * - Normal MBR: Encrypts bytes 0-509 (510 bytes) + markers at 510-511
 * - Superfloppy: Encrypts bytes 0-509 (510 bytes) + markers at 510-511
 * 
 * RC4 IMPLEMENTATION:
 * - Stream cipher (no padding needed)
 * - Key: SHA-256 hash of master key (32 bytes)
 * - Encryption markers at bytes 510-511 for detection
 * 
 * KERNEL COMPATIBILITY: 5.10 through 6.12+
 * RHEL/CentOS: 8, 9, Stream compatible
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/blkdev.h>
#include <linux/jiffies.h>
#include <linux/time64.h>
#include <linux/rtc.h>
#include <linux/file.h>
/* Handle genhd.h across different kernel versions and distributions */
/* RHEL 9 (5.14.0-617+) backported genhd.h removal despite version < 5.18 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,18,0)
/* Upstream 5.18+: genhd.h merged into blkdev.h */
#else
/* Check if we're on RHEL 9+ which backported the change */
#if defined(RHEL_MAJOR)
#if RHEL_MAJOR >= 9
/* RHEL 9+: genhd.h merged into blkdev.h (backported) */
#else
/* RHEL 8: Still needs genhd.h */
#include <linux/genhd.h>
#endif
#else
/* Non-RHEL kernels < 5.18: Need genhd.h */
#include <linux/genhd.h>
#endif
#endif

#include <linux/bio.h>
#include <linux/kprobes.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/usb/storage.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/buffer_head.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <crypto/hash.h>
#include <generated/utsrelease.h>

/* Block device mode compatibility across kernel versions */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,5,0)
#define COMPAT_MODE_TYPE blk_mode_t
#define COMPAT_MODE_READ BLK_OPEN_READ
#define COMPAT_MODE_WRITE BLK_OPEN_WRITE
#define USE_NEW_BDEV_API
#elif defined(RHEL_MAJOR) && RHEL_MAJOR >= 9
#define COMPAT_MODE_TYPE fmode_t
#define COMPAT_MODE_READ FMODE_READ
#define COMPAT_MODE_WRITE FMODE_WRITE
#define USE_NEW_BDEV_API
#else
#define COMPAT_MODE_TYPE fmode_t
#define COMPAT_MODE_READ FMODE_READ
#define COMPAT_MODE_WRITE FMODE_WRITE
#endif

#define MODULE_NAME "mbr_crypto"
#define MBR_SIZE 512
#define MBR_SECTOR 0

/* CD-ROM specific sectors and sizes */
#define CDROM_SECTOR_SIZE 2048
#define PVD_SECTOR 16
#define UDF_ANCHOR_SECTOR 256

/* RC4 encryption parameters */
#define RC4_SBLOCK_SIZE 256
#define ENCRYPT_DATA_SIZE 510
#define MARKER_OFFSET 510
#define PARTITION_TABLE_OFFSET 446
#define WINDOWS_KEY_LEN 79
#define RC4_KEY_SIZE 96
#define MAX_KEY_LENGTH 256

#ifndef USB_SC_SCSI
#define USB_SC_SCSI 0x06
#endif

#ifndef USB_PR_BULK
#define USB_PR_BULK 0x50
#endif

/* ============================================
 * LOGGING SYSTEM
 * ============================================ */

/* Log level control */
//static int log_level = 1;
static int log_level = 4; // change by rushi for get all log 290426
module_param(log_level, int, 0644);
MODULE_PARM_DESC(log_level, "Log verbosity (1=CRITICAL, 2=INFO, 3=DEBUG, 4=VERBOSE)");

#define LOG_FILE_PATH "/var/MicroWorld/var/log/rc4.log"

#define LOG_CRITICAL  1
#define LOG_INFO      2
#define LOG_DEBUG     3
#define LOG_VERBOSE   4

/* Forward declaration */
static void write_to_log_file(int level, const char *fmt, ...);

#define RC4_LOG(level, fmt, ...) \
	do { \
		if (log_level >= level) { \
			write_to_log_file(level, fmt, ##__VA_ARGS__); \
			pr_info("%s: [L%d] " fmt, MODULE_NAME, level, ##__VA_ARGS__); \
		} \
	} while (0)

#define RC4_LOG_CRITICAL(fmt, ...) RC4_LOG(LOG_CRITICAL, fmt, ##__VA_ARGS__)
#define RC4_LOG_INFO(fmt, ...)     RC4_LOG(LOG_INFO, fmt, ##__VA_ARGS__)
#define RC4_LOG_DEBUG(fmt, ...)    RC4_LOG(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define RC4_LOG_VERBOSE(fmt, ...)  RC4_LOG(LOG_VERBOSE, fmt, ##__VA_ARGS__)

static int debug = 1;
module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "Enable debug output");

static char *corpId = NULL;
module_param(corpId, charp, 0644);
MODULE_PARM_DESC(corpId, "Corporate ID in HEXADECIMAL format");

unsigned int corp_id = 0;
static int status = 1;
module_param(status, uint, 0644);
MODULE_PARM_DESC(status, "Module status (1 or 2 = enabled)");

static int decryptValue = 0;
module_param(decryptValue, int, 0644);
MODULE_PARM_DESC(decryptValue, "Enable decryption (1 = decrypt, 0 = no)");

static int encryptValue = 0;
module_param(encryptValue, int, 0644);
MODULE_PARM_DESC(encryptValue, "Enable encryption (1 = encrypt, 0 = no)");

static u8 cdrom_encryption_marker[2] = {0};
static bool cdrom_marker_initialized = false;

static unsigned char base_key_last2chars[RC4_KEY_SIZE] = "&*&^%^&%asflk;;aasf/.zxdv;'asd&&$%&&()_$%%^k9034509k9*())_KKmnscmsf}{miun$$IOPc";
static unsigned char storage_key[RC4_KEY_SIZE] = "&*&^%^&%asflk;;aasf/.zxdv;'asd&&$%&&()_$%%^k9034509k9*())_KKmnscmsf}{miun$$IOPc";
static u8 sblock_last2chars[RC4_SBLOCK_SIZE] = {0};
static u8 sblock_storage[RC4_SBLOCK_SIZE] = {0};
static bool sblock_initialized = false;

static u8 encryption_marker[2] = {0};
static bool key_loaded = false;

enum disk_format_type {
	FORMAT_UNKNOWN = 0,
	FORMAT_NORMAL_MBR = 1,
	FORMAT_SUPERFLOPPY = 2,
	FORMAT_UNFORMATTED = 3
};

struct mbr_crypto_device {
	struct list_head list;
	dev_t devt;
	char name[32];
	bool enabled;
	bool is_encrypted;
	bool checked;
	bool data_write_detected;
	enum disk_format_type format_type;
	struct mutex encrypt_lock;
	unsigned long format_timestamp;
	bool formatting_in_progress;
	bool is_cdrom;
	bool pvd_encrypted;
	bool udf_encrypted;
};

static LIST_HEAD(device_list);
static DEFINE_SPINLOCK(device_list_lock);

struct crypto_ctx {
	struct work_struct work;
	struct bio *bio;
	bool decrypt;
	bio_end_io_t *original_endio;
	void *original_private;
	sector_t start_sector;
};

static atomic_t read_intercepts = ATOMIC_INIT(0);
static atomic_t write_intercepts = ATOMIC_INIT(0);
static atomic_t decrypt_ops = ATOMIC_INIT(0);
static atomic_t encrypt_ops = ATOMIC_INIT(0);
static atomic_t passthrough_ops = ATOMIC_INIT(0);
static atomic_t data_write_triggers = ATOMIC_INIT(0);
static atomic_t decrypt_skipped = ATOMIC_INIT(0);

struct mbr_encrypt_work {
	struct work_struct work;
	dev_t devt;
	char name[32];
};

static DEFINE_PER_CPU(bool, in_worker_context);

/* Forward declarations */
static struct mbr_crypto_device *find_device(dev_t devt);
static void schedule_mbr_encryption(struct mbr_crypto_device *mcd);

/* ============================================
 * LOGGING FILE OPERATIONS
 * ============================================ */

// get_timestamp date time UTC time to IST time written by rushi 290426
static void get_timestamp(char *buffer, size_t size)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct timespec64 ts;
	struct tm tm_time;
	const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	ktime_get_real_ts64(&ts);     // returns UTC time in seconds since epoch.
     
      // Convert UTC to IST (UTC+5:30 = 19800 seconds)
     ts.tv_sec += 19800;           // Adding 19800 seconds (5 hours × 3600 + 30 minutes × 60) shifts the timestamp to Indian Standard Time.

	time64_to_tm(ts.tv_sec, 0, &tm_time); // automatically handles day, month, and year rollovers (e.g., if the offset pushes past midnight).

	snprintf(buffer, size, "[%02d-%s-%04ld@%02d:%02d:%02d]",
	         tm_time.tm_mday,
	         months[tm_time.tm_mon],
	         tm_time.tm_year + 1900,
	         tm_time.tm_hour,
	         tm_time.tm_min,
	         tm_time.tm_sec);

     RC4_LOG_CRITICAL("End ->                 ");

}

static void write_to_log_file(int level, const char *fmt, ...)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct file *filp;
	char *log_buffer;
	char *full_log;
	char timestamp[32];
	va_list args;
	loff_t pos = 0;

	log_buffer = kmalloc(512, GFP_KERNEL);
	if (!log_buffer)
     {
     RC4_LOG_CRITICAL("End ->                 ");

          return;
     }

	full_log = kmalloc(600, GFP_KERNEL);
	if (!full_log) {
		kfree(log_buffer);
     RC4_LOG_CRITICAL("End ->                 ");

		return;
	}

	get_timestamp(timestamp, sizeof(timestamp));

	va_start(args, fmt);
	vsnprintf(log_buffer, 512, fmt, args);
	va_end(args);

	snprintf(full_log, 600, "%s [L%d] %s\n", timestamp, level, log_buffer);

	filp = filp_open(LOG_FILE_PATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (!IS_ERR(filp)) {
		pos = vfs_llseek(filp, 0, SEEK_END);
		kernel_write(filp, full_log, strlen(full_log), &pos);
		filp_close(filp, NULL);
	}

	kfree(full_log);
	kfree(log_buffer);

     RC4_LOG_CRITICAL("End ->                 ");

}

/* ============================================
 * RC4 IMPLEMENTATION
 * ============================================ */

static void rc4_init(u8 *s, const u8 *key, size_t key_len)
{
     RC4_LOG_CRITICAL("start ->                 ");
	int i, j = 0;
	u8 k[RC4_SBLOCK_SIZE];
	u8 tmp;

	for (i = 0; i < RC4_SBLOCK_SIZE; i++) {
		s[i] = (u8)i;
		k[i] = key[i % key_len];
	}

	for (i = 0; i < RC4_SBLOCK_SIZE; i++) {
		j = (j + s[i] + k[i]) % RC4_SBLOCK_SIZE;
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}

     RC4_LOG_CRITICAL("End ->                 ");

}

static void rc4_crypt(u8 *s, u8 *data, size_t len)
{
     RC4_LOG_CRITICAL("start ->                 ");
	int i = 0, j = 0, t;
	size_t k;
	u8 tmp;

	for (k = 0; k < len; k++) {
		i = (i + 1) % RC4_SBLOCK_SIZE;
		j = (j + s[i]) % RC4_SBLOCK_SIZE;

		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;

		t = (s[i] + s[j]) % RC4_SBLOCK_SIZE;
		data[k] ^= s[t];
	}

     RC4_LOG_CRITICAL("End ->                 ");

}

static int rc4_encrypt_data(u8 *data, size_t len)
{
     RC4_LOG_CRITICAL("start ->                 ");
	u8 local_sblock[RC4_SBLOCK_SIZE];

	if (!sblock_initialized) {
		RC4_LOG_CRITICAL("S-block not initialized");

     RC4_LOG_CRITICAL("End ->                 ");

		return -EINVAL;
	}

	memcpy(local_sblock, sblock_storage, RC4_SBLOCK_SIZE);
	rc4_crypt(local_sblock, data, len);

	RC4_LOG_VERBOSE("RC4 encryption completed: %zu bytes", len);
	atomic_inc(&encrypt_ops);
     RC4_LOG_CRITICAL("End ->                 ");

	return 0;
}

static int rc4_decrypt_data(u8 *data, size_t len)
{
     RC4_LOG_CRITICAL("start ->                 ");
	u8 local_sblock[RC4_SBLOCK_SIZE];

	if (!sblock_initialized) {
		RC4_LOG_CRITICAL("S-block not initialized");
     RC4_LOG_CRITICAL("End ->                 ");

		return -EINVAL;
	}

	memcpy(local_sblock, sblock_storage, RC4_SBLOCK_SIZE);
	rc4_crypt(local_sblock, data, len);

	RC4_LOG_VERBOSE("RC4 decryption completed: %zu bytes", len);
	atomic_inc(&decrypt_ops);
     RC4_LOG_CRITICAL("End ->                 ");

	return 0;
}

/* ============================================
 * BLOCK DEVICE COMPATIBILITY
 * ============================================ */

#ifdef USE_NEW_BDEV_API
struct compat_bdev_handle {
	struct file *bdev_file;
	struct block_device *bdev;
};

static inline void *compat_bdev_open(dev_t devt, blk_mode_t mode)
{
     RC4_LOG_CRITICAL("start ->                 ");
    struct compat_bdev_handle *compat_handle;
    
    compat_handle = kmalloc(sizeof(*compat_handle), GFP_KERNEL);
    if (!compat_handle)
    {
      RC4_LOG_CRITICAL("End ->                 ");

         return ERR_PTR(-ENOMEM);
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
    // Kernel 6.6+ uses bdev_handle
    struct bdev_handle *bhandle = bdev_open_by_dev(devt, mode, NULL, NULL);
    if (IS_ERR(bhandle)) {
        kfree(compat_handle);
     RC4_LOG_CRITICAL("End ->                 ");

        return bhandle;
    }
    compat_handle->bdev_file = (struct file *)bhandle;
    compat_handle->bdev = bhandle->bdev;
    
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
    // Kernel 6.5 still uses OLD blkdev_get_by_dev
    struct block_device *bdev = blkdev_get_by_dev(devt, mode, NULL,NULL);
    if (IS_ERR(bdev)) {
        kfree(compat_handle);
     RC4_LOG_CRITICAL("End ->                 ");

        return bdev;
    }
    compat_handle->bdev_file = NULL;
    compat_handle->bdev = bdev;
    
#else
    // Kernel < 6.5 uses even older API
    struct block_device *bdev = blkdev_get_by_dev(devt, mode, NULL);
    if (IS_ERR(bdev)) {
        kfree(compat_handle);
     RC4_LOG_CRITICAL("End ->                 ");

        return bdev;
    }
    compat_handle->bdev_file = NULL;
    compat_handle->bdev = bdev;
#endif
     RC4_LOG_CRITICAL("End ->                 ");

    return compat_handle;
}

static inline void compat_bdev_close(struct compat_bdev_handle *compat_handle)
{
     RC4_LOG_CRITICAL("start ->                 ");
    if (!compat_handle || IS_ERR(compat_handle))
    {
      RC4_LOG_CRITICAL("End ->                 ");

         return;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
    if (compat_handle->bdev_file) {
        struct bdev_handle *bhandle = (struct bdev_handle *)compat_handle->bdev_file;
        bdev_release(bhandle);
    }
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
    if (compat_handle->bdev)
        blkdev_put(compat_handle->bdev, NULL);
#else
    if (compat_handle->bdev)
        blkdev_put(compat_handle->bdev, mode); // Use proper mode
#endif
     RC4_LOG_CRITICAL("End ->                 ");

    kfree(compat_handle);
     RC4_LOG_CRITICAL("End ->                 ");

}

#else
struct compat_bdev_handle {
	struct block_device *bdev;
	COMPAT_MODE_TYPE mode;
};

static struct compat_bdev_handle *compat_bdev_open(dev_t devt, COMPAT_MODE_TYPE mode)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct compat_bdev_handle *compat_handle;
	struct block_device *bdev;

	compat_handle = kmalloc(sizeof(*compat_handle), GFP_KERNEL);
	if (!compat_handle)
     {
     RC4_LOG_CRITICAL("End ->                 ");

          return ERR_PTR(-ENOMEM);
     }

	bdev = blkdev_get_by_dev(devt, mode, NULL);
	if (IS_ERR(bdev)) {
		kfree(compat_handle);
     RC4_LOG_CRITICAL("End ->                 ");

		return ERR_CAST(bdev);
	}

	compat_handle->bdev = bdev;
	compat_handle->mode = mode;
     RC4_LOG_CRITICAL("End ->                 ");

	return compat_handle;
}

static void compat_bdev_close(struct compat_bdev_handle *compat_handle)
{
     RC4_LOG_CRITICAL("start ->                 ");
	if (compat_handle) {
		blkdev_put(compat_handle->bdev, compat_handle->mode);
		kfree(compat_handle);
	}
     RC4_LOG_CRITICAL("End ->                 ");

}
#endif

/* ============================================
 * DISK FORMAT DETECTION
 * ============================================ */

static enum disk_format_type detect_disk_format(const u8 *data)
{
     RC4_LOG_CRITICAL("start ->                 ");
	bool has_boot_sig;
	bool has_partition_table;
	bool has_filesystem_sig;
	int i;

	has_boot_sig = (data[510] == 0x55 && data[511] == 0xAA);

	has_partition_table = false;
	for (i = 0; i < 4; i++) {
		const u8 *entry = data + PARTITION_TABLE_OFFSET + (i * 16);
		u8 status = entry[0];
		u8 type = entry[4];

		if ((status == 0x00 || status == 0x80) && type != 0x00) {
			has_partition_table = true;
			break;
		}
	}

	has_filesystem_sig = false;
	if (memcmp(data + 82, "FAT32   ", 8) == 0) {
		has_filesystem_sig = true;
		RC4_LOG_DEBUG("Detected FAT32 signature");
	}
	else if (memcmp(data + 54, "FAT12   ", 8) == 0 || 
	         memcmp(data + 54, "FAT16   ", 8) == 0) {
		has_filesystem_sig = true;
		RC4_LOG_DEBUG("Detected FAT12/16 signature");
	}
	else if (memcmp(data + 3, "NTFS    ", 8) == 0) {
		has_filesystem_sig = true;
		RC4_LOG_DEBUG("Detected NTFS signature");
	}
	else if (memcmp(data + 3, "EXFAT   ", 8) == 0) {
		has_filesystem_sig = true;
		RC4_LOG_DEBUG("Detected exFAT signature");

	}

	if (!has_partition_table && has_filesystem_sig) {
		RC4_LOG_INFO("Format detected: SUPERFLOPPY");
     RC4_LOG_CRITICAL("End ->                 ");

		return FORMAT_SUPERFLOPPY;
	}

	if (has_boot_sig && has_partition_table) {
		RC4_LOG_INFO("Format detected: NORMAL MBR");
     RC4_LOG_CRITICAL("End ->                 ");

		return FORMAT_NORMAL_MBR;
	}

	if (!has_boot_sig && !has_partition_table && !has_filesystem_sig) {
		RC4_LOG_INFO("Format detected: UNFORMATTED");
     RC4_LOG_CRITICAL("End ->                 ");

		return FORMAT_UNFORMATTED;
	}

	RC4_LOG_INFO("Format detected: UNKNOWN");
     RC4_LOG_CRITICAL("End ->                 ");

	return FORMAT_UNKNOWN;
}

static const char *format_type_name(enum disk_format_type type)
{
     RC4_LOG_CRITICAL("start ->                 ");
	switch (type) {
		case FORMAT_NORMAL_MBR:
     RC4_LOG_CRITICAL("End ->                 ");

			return "Normal MBR";
		case FORMAT_SUPERFLOPPY:
     RC4_LOG_CRITICAL("End ->                 ");

			return "Superfloppy";
		case FORMAT_UNFORMATTED:
     RC4_LOG_CRITICAL("End ->                 ");

			return "Unformatted";
		default:
     RC4_LOG_CRITICAL("End ->                 ");

			return "Unknown";
	}
}

/* ============================================
 * ENCRYPTION MARKER GENERATION
 * ============================================ */

static void generate_encryption_markers(void)
{
     RC4_LOG_CRITICAL("start ->                 ");
	u8 local_sblock[RC4_SBLOCK_SIZE];
	u8 cx[4] = {0};
	char corp_id_str[32];
	unsigned int slen;

	if (sblock_initialized) {
		RC4_LOG_INFO("S-blocks already initialized");
     RC4_LOG_CRITICAL("End ->                 ");

		return;
	}

	if (corp_id == 0) {
		RC4_LOG_CRITICAL("========================================");
		RC4_LOG_CRITICAL("ERROR: corp_id not set!");
		RC4_LOG_CRITICAL("Cross-platform encryption REQUIRES corp_id");
		RC4_LOG_CRITICAL("Load module with: insmod mbr_crypto.ko corp_id=XXXXX");
		RC4_LOG_CRITICAL("Corp ID must match Windows driver!");
		RC4_LOG_CRITICAL("========================================");
		encryption_marker[0] = 0x0B;
		encryption_marker[1] = 0x10;
     RC4_LOG_CRITICAL("End ->                 ");


		return;
	}

	snprintf(corp_id_str, sizeof(corp_id_str), "%u", corp_id);
	slen = strlen(corp_id_str);

	RC4_LOG_INFO("Using Corp ID configured (cross-platform mode)");

	cx[0] = (u8)corp_id_str[0];
	cx[1] = (u8)corp_id_str[slen-1];
	cx[2] = 0;

	rc4_init(sblock_last2chars, base_key_last2chars, WINDOWS_KEY_LEN);

	memcpy(local_sblock, sblock_last2chars, RC4_SBLOCK_SIZE);
	rc4_crypt(local_sblock, cx, 2);

	encryption_marker[0] = cx[0];
	encryption_marker[1] = cx[1];

	RC4_LOG_CRITICAL("========================================");
	RC4_LOG_CRITICAL("Encryption markers generated successfully");
	RC4_LOG_CRITICAL("Cross-platform mode enabled");
	RC4_LOG_CRITICAL("========================================");

	storage_key[10] = encryption_marker[0];
	storage_key[20] = encryption_marker[1];

	rc4_init(sblock_storage, storage_key, WINDOWS_KEY_LEN);

	sblock_initialized = true;

	RC4_LOG_INFO("Storage key modified successfully");
	RC4_LOG_INFO("S-blocks initialized successfully");
     RC4_LOG_CRITICAL("End ->                 ");

}

/* ============================================
 * CD-ROM SUPPORT
 * ============================================ */

static bool is_cdrom_device(struct gendisk *disk)
{
     RC4_LOG_CRITICAL("start ->                 ");
	if (!disk)
     {	
          RC4_LOG_CRITICAL("End ->                 ");

     
          return false;}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,1,0)
	/* Kernel 6.1+: GENHD_FL_CD removed, check major number */
	if (disk->major == 11) { /* CD-ROM major number */
		RC4_LOG_DEBUG("CD-ROM device detected: %s", disk->disk_name);
     RC4_LOG_CRITICAL("End ->                 ");

		return true;
	}
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0)
	/* Kernel 5.0-6.0: Use GENHD_FL_CD flag */
	if (disk->flags & GENHD_FL_CD) {
		RC4_LOG_DEBUG("CD-ROM device detected: %s", disk->disk_name);
     RC4_LOG_CRITICAL("End ->                 ");

		return true;
	}
#else
	/* Kernel < 5.0: Check device type differently */
	if (disk->flags & GENHD_FL_REMOVABLE) {
		struct device *dev = disk_to_dev(disk);
		if (dev && dev->type && dev->type->name &&
		    strcmp(dev->type->name, "cd") == 0) {
			RC4_LOG_DEBUG("CD-ROM device detected: %s", disk->disk_name);
     RC4_LOG_CRITICAL("End ->                 ");

			return true;
		}
	}
#endif
	     RC4_LOG_CRITICAL("End ->                 ");

	return false;
}
static void generate_cdrom_markers(void)
{
     RC4_LOG_CRITICAL("start ->                 ");
	u8 local_sblock[RC4_SBLOCK_SIZE];
	u8 cx[4] = {0};
	char corp_id_str[32];
	unsigned int slen;
	
	if (cdrom_marker_initialized) {
		RC4_LOG_INFO("CD-ROM markers already initialized");
     RC4_LOG_CRITICAL("End ->                 ");

		return;
	}
	
	if (corp_id == 0) {
		RC4_LOG_INFO("Corp ID not set for CD-ROM markers, using defaults");
		cdrom_encryption_marker[0] = 0x0C;
		cdrom_encryption_marker[1] = 0x0D;
		cdrom_marker_initialized = true;

     RC4_LOG_CRITICAL("End ->                 ");

		return;
	}
	
	snprintf(corp_id_str, sizeof(corp_id_str), "%u", corp_id);
	slen = strlen(corp_id_str);
	
	cx[0] = (u8)corp_id_str[slen-1];
	cx[1] = (u8)corp_id_str[0];
	cx[2] = 0;
	
	memcpy(local_sblock, sblock_last2chars, RC4_SBLOCK_SIZE);
	rc4_crypt(local_sblock, cx, 2);
	
	cdrom_encryption_marker[0] = cx[0];
	cdrom_encryption_marker[1] = cx[1];
	
	cdrom_marker_initialized = true;
	
	RC4_LOG_CRITICAL("========================================");
	RC4_LOG_CRITICAL("CD-ROM markers generated");
	RC4_LOG_CRITICAL("CD-ROM encryption support enabled");
	RC4_LOG_CRITICAL("========================================");
}

static bool check_pvd_encrypted(const u8 *data)
{
     RC4_LOG_CRITICAL("start ->                 ");
	if (data[510] == cdrom_encryption_marker[0] && 
	    data[511] == cdrom_encryption_marker[1]) {
		RC4_LOG_DEBUG("PVD is encrypted (markers found)");
     RC4_LOG_CRITICAL("End ->                 ");

		return true;
	}
	
	if (data[1] == 'C' && data[2] == 'D' && 
	    data[3] == '0' && data[4] == '0' && data[5] == '1') {
		RC4_LOG_DEBUG("PVD is unencrypted (CD001 signature found)");
     RC4_LOG_CRITICAL("End ->                 ");

		return false;
	}
	
	     RC4_LOG_CRITICAL("End ->                 ");
RC4_LOG_DEBUG("PVD status unknown");
	return false;
}

static int encrypt_cdrom_pvd(u8 *data)
{
     RC4_LOG_CRITICAL("start ->                 ");
	u8 *encrypt_buffer;
	int ret;
	
	if (!sblock_initialized) {
		RC4_LOG_CRITICAL("S-block not initialized for CD-ROM encryption");
     RC4_LOG_CRITICAL("End ->                 ");

		return -EINVAL;
	}
	
	if (!cdrom_marker_initialized) {
		RC4_LOG_CRITICAL("CD-ROM markers not initialized");
     RC4_LOG_CRITICAL("End ->                 ");

		return -EINVAL;
	}
	
	encrypt_buffer = kmalloc(ENCRYPT_DATA_SIZE, GFP_ATOMIC);
	if (!encrypt_buffer)
     {
     RC4_LOG_CRITICAL("End ->                 ");

          return -ENOMEM;
     }
	
	memcpy(encrypt_buffer, data, ENCRYPT_DATA_SIZE);
	ret = rc4_encrypt_data(encrypt_buffer, ENCRYPT_DATA_SIZE);
	
	if (ret == 0) {
		memcpy(data, encrypt_buffer, ENCRYPT_DATA_SIZE);
		data[510] = cdrom_encryption_marker[0];
		data[511] = cdrom_encryption_marker[1];
		RC4_LOG_CRITICAL("CD-ROM PVD encrypted (sector 16), markers set");
		atomic_inc(&encrypt_ops);
	}
	
	kfree(encrypt_buffer);
     RC4_LOG_CRITICAL("End ->                 ");

	return ret;
}

static int decrypt_cdrom_pvd(u8 *data)
{
     RC4_LOG_CRITICAL("start ->                 ");
	u8 *decrypt_buffer;
	int ret;
	
	if (!sblock_initialized) {
		RC4_LOG_CRITICAL("S-block not initialized");
     RC4_LOG_CRITICAL("End ->                 ");

		return -EINVAL;
	}
	
	decrypt_buffer = kmalloc(ENCRYPT_DATA_SIZE, GFP_ATOMIC);
	if (!decrypt_buffer)
     {     RC4_LOG_CRITICAL("End ->                 ");


          return -ENOMEM;
     }
	
	memcpy(decrypt_buffer, data, ENCRYPT_DATA_SIZE);
	ret = rc4_decrypt_data(decrypt_buffer, ENCRYPT_DATA_SIZE);
	
	if (ret == 0) {
		memcpy(data, decrypt_buffer, ENCRYPT_DATA_SIZE);
		RC4_LOG_CRITICAL("CD-ROM PVD decrypted (sector 16)");
		atomic_inc(&decrypt_ops);
	}
	
	kfree(decrypt_buffer);
     RC4_LOG_CRITICAL("End ->                 ");

	return ret;
}

/* ============================================
 * KEY LOADING
 * ============================================ */

static int load_encryption_key(void)
{
     RC4_LOG_CRITICAL("start ->                 ");
	generate_encryption_markers();

	if (!sblock_initialized) {
		RC4_LOG_CRITICAL("Failed to initialize S-blocks");
     RC4_LOG_CRITICAL("End ->                 ");

		return -EINVAL;
	}

	key_loaded = true;
	RC4_LOG_CRITICAL("RC4 encryption key loaded successfully");

	if (debug) {
		RC4_LOG_DEBUG("Base key length: %d bytes", WINDOWS_KEY_LEN);
		RC4_LOG_DEBUG("Storage key initialized");
	}
     RC4_LOG_CRITICAL("End ->                 ");

	return 0;
}

/* ============================================
 * MBR ENCRYPTION CHECK
 * ============================================ */

static bool check_mbr_encrypted(const u8 *data, enum disk_format_type *detected_format)
{
     RC4_LOG_CRITICAL("start ->                 ");
	enum disk_format_type format;

	RC4_LOG_DEBUG("=== Starting encryption detection ===");

	format = detect_disk_format(data);
	if (detected_format)
		*detected_format = format;

	RC4_LOG_INFO("Detected format: %s", format_type_name(format));

	if (data[510] == 0x55 && data[511] == 0xAA) {
		RC4_LOG_INFO("[RESULT] Valid boot signature (0x55AA) - NOT ENCRYPTED");
     RC4_LOG_CRITICAL("End ->                 ");

		return false;
	}

	if (data[510] == encryption_marker[0] && data[511] == encryption_marker[1]) {
		RC4_LOG_CRITICAL("[RESULT] *** ENCRYPTION MARKERS MATCH *** - IS ENCRYPTED");
		RC4_LOG_CRITICAL("[RESULT] Device encrypted by our key - WILL DECRYPT");
     RC4_LOG_CRITICAL("End ->                 ");

		return true;
	}

	RC4_LOG_INFO("========================================");
	RC4_LOG_INFO("[RESULT] MARKER MISMATCH - DIFFERENT KEY!");
	RC4_LOG_INFO("This device is encrypted with a DIFFERENT key!");
	RC4_LOG_INFO("Cannot decrypt - will PASSTHROUGH");
	RC4_LOG_INFO("========================================");
     RC4_LOG_CRITICAL("End ->                 ");

	return false;
}

/* ============================================
 * BIO PROCESSING
 * ============================================ */

static int process_bio_decrypt(struct bio *bio, sector_t start_sector)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct bio_vec *bvec;
	void *kaddr;
	u8 *decrypt_buffer;
	int ret = 0;
	unsigned int i;
	sector_t sectors_processed = 0;
	sector_t current_sector;

	if (decryptValue != 1) {
		RC4_LOG_INFO("Decryption DISABLED (decryptValue=%d) - PASSTHROUGH encrypted data", decryptValue);
		atomic_inc(&decrypt_skipped);
     RC4_LOG_CRITICAL("End ->                 ");

		return 0;
	}

	for (i = 0; i < bio->bi_vcnt; i++) {
		bvec = &bio->bi_io_vec[i];
		current_sector = start_sector + sectors_processed;

		if (current_sector == MBR_SECTOR) {
			kaddr = kmap_atomic(bvec->bv_page);
			u8 *sector_data = (u8 *)(kaddr + bvec->bv_offset);

			RC4_LOG_DEBUG("Checking sector %llu for encryption markers", (unsigned long long)current_sector);

			if (sector_data[510] != encryption_marker[0] || 
			    sector_data[511] != encryption_marker[1]) {
				RC4_LOG_DEBUG("Sector not encrypted - skipping decryption");
				kunmap_atomic(kaddr);
				break;
			}

			decrypt_buffer = kmalloc(ENCRYPT_DATA_SIZE, GFP_ATOMIC);
			if (!decrypt_buffer) {
				kunmap_atomic(kaddr);
     RC4_LOG_CRITICAL("End ->                 ");

				return -ENOMEM;
			}

			memcpy(decrypt_buffer, sector_data, ENCRYPT_DATA_SIZE);

			ret = rc4_decrypt_data(decrypt_buffer, ENCRYPT_DATA_SIZE);

			if (ret == 0) {
				memcpy(sector_data, decrypt_buffer, ENCRYPT_DATA_SIZE);
				sector_data[510] = 0x55;
				sector_data[511] = 0xAA;
				RC4_LOG_CRITICAL("Decrypted 510 bytes, restored boot signature");
				ClearPageDirty(bvec->bv_page);
			} else {
				RC4_LOG_CRITICAL("Decryption failed: %d", ret);
			}

			kfree(decrypt_buffer);
			kunmap_atomic(kaddr);
			break;
		}
		sectors_processed += bvec->bv_len >> 9;
	}
     RC4_LOG_CRITICAL("End ->                 ");

	return ret;
}

static int process_bio_encrypt(struct bio *bio, sector_t start_sector)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct bio_vec bvec;
	struct bvec_iter iter;
	void *kaddr;
	u8 *encrypt_buffer;
	int ret = 0;

	bio_for_each_segment(bvec, bio, iter) {
		if (iter.bi_sector == MBR_SECTOR) {
			kaddr = kmap_atomic(bvec.bv_page);
			u8 *sector_data = (u8 *)(kaddr + bvec.bv_offset);

			if (sector_data[510] != 0x55 || sector_data[511] != 0xAA) {
				RC4_LOG_DEBUG("Sector already encrypted - skipping");
				kunmap_atomic(kaddr);
				break;
			}

			encrypt_buffer = kmalloc(ENCRYPT_DATA_SIZE, GFP_ATOMIC);
			if (!encrypt_buffer) {
				kunmap_atomic(kaddr);
     RC4_LOG_CRITICAL("End ->                 ");

				return -ENOMEM;
			}

			memcpy(encrypt_buffer, sector_data, ENCRYPT_DATA_SIZE);

			ret = rc4_encrypt_data(encrypt_buffer, ENCRYPT_DATA_SIZE);

			if (ret == 0) {
				memcpy(sector_data, encrypt_buffer, ENCRYPT_DATA_SIZE);
				sector_data[510] = encryption_marker[0];
				sector_data[511] = encryption_marker[1];
				RC4_LOG_CRITICAL("Encrypted 510 bytes, set markers");
			} else {
				RC4_LOG_CRITICAL("Encryption failed: %d", ret);
			}

			kfree(encrypt_buffer);
			kunmap_atomic(kaddr);
			break;
		}
	}
     RC4_LOG_CRITICAL("End ->                 ");

	return ret;
}

/* ============================================
 * BACKGROUND ENCRYPTION WORKER
 * ============================================ */

static void encrypt_mbr_work_fn(struct work_struct *work)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct mbr_encrypt_work *ctx = container_of(work, struct mbr_encrypt_work, work);
	struct compat_bdev_handle *handle;
	struct block_device *bdev;
	struct buffer_head *bh;
	u8 *mbr_data;
	int ret;
	struct mbr_crypto_device *mcd;
	u8 *encrypt_buffer;
	enum disk_format_type format;

	RC4_LOG_INFO("[BACKGROUND] Starting encryption for %s", ctx->name);

	mcd = find_device(ctx->devt);
	if (!mcd) {
		RC4_LOG_CRITICAL("Device not found");
		goto cleanup;
	}

	if (mcd->is_encrypted) {
		RC4_LOG_DEBUG("Already encrypted, skipping");
		goto cleanup;
	}

	mutex_lock(&mcd->encrypt_lock);

	if (mcd->is_encrypted) {
		mutex_unlock(&mcd->encrypt_lock);
		goto cleanup;
	}

	handle = compat_bdev_open(ctx->devt, COMPAT_MODE_READ | COMPAT_MODE_WRITE);
	if (IS_ERR(handle)) {
		RC4_LOG_CRITICAL("Failed to open device: %ld", PTR_ERR(handle));
		mutex_unlock(&mcd->encrypt_lock);
		goto cleanup;
	}

	bdev = handle->bdev;

	bh = __bread(bdev, 0, 512);
	if (!bh) {
		RC4_LOG_CRITICAL("Failed to read MBR");
		compat_bdev_close(handle);
		mutex_unlock(&mcd->encrypt_lock);
		goto cleanup;
	}

	mbr_data = (u8 *)bh->b_data;

	if (check_mbr_encrypted(mbr_data, &format)) {
		RC4_LOG_DEBUG("Already encrypted");
		mcd->is_encrypted = true;
		mcd->checked = true;
		mcd->format_type = format;
		brelse(bh);
		compat_bdev_close(handle);
		mutex_unlock(&mcd->encrypt_lock);
		goto cleanup;
	}

	mcd->format_type = format;

	RC4_LOG_INFO("========================================");
	RC4_LOG_INFO("Encrypting %s format", format_type_name(format));
	RC4_LOG_INFO("RC4 encrypting 510 bytes (0-509)");
	RC4_LOG_INFO("Setting markers at [510-511]");
	RC4_LOG_INFO("========================================");

	encrypt_buffer = kmalloc(ENCRYPT_DATA_SIZE, GFP_KERNEL);
	if (!encrypt_buffer) {
		RC4_LOG_CRITICAL("Failed to allocate encryption buffer");
		brelse(bh);
		compat_bdev_close(handle);
		mutex_unlock(&mcd->encrypt_lock);
		goto cleanup;
	}

	memcpy(encrypt_buffer, mbr_data, ENCRYPT_DATA_SIZE);

	ret = rc4_encrypt_data(encrypt_buffer, ENCRYPT_DATA_SIZE);
	if (ret) {
		RC4_LOG_CRITICAL("Encryption failed: %d", ret);
		kfree(encrypt_buffer);
		brelse(bh);
		compat_bdev_close(handle);
		mutex_unlock(&mcd->encrypt_lock);
		goto cleanup;
	}

	memcpy(mbr_data, encrypt_buffer, ENCRYPT_DATA_SIZE);
	mbr_data[510] = encryption_marker[0];
	mbr_data[511] = encryption_marker[1];

	kfree(encrypt_buffer);

	mcd->is_encrypted = true;
	mcd->checked = true;

	RC4_LOG_INFO("[WORKER] Marked device as encrypted, preparing to write");

	__this_cpu_write(in_worker_context, true);

	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);

	__this_cpu_write(in_worker_context, false);

	if (buffer_req(bh) && !buffer_uptodate(bh)) {
		RC4_LOG_CRITICAL("Failed to write encrypted MBR");
		mcd->is_encrypted = false;
		mcd->checked = false;
	} else {
		RC4_LOG_CRITICAL("========================================");
		RC4_LOG_CRITICAL("*** RC4 ENCRYPTION COMPLETED ***");
		RC4_LOG_CRITICAL("*** Device %s encrypted successfully ***", ctx->name);
		RC4_LOG_CRITICAL("*** 510 bytes encrypted, markers set ***");
		RC4_LOG_CRITICAL("========================================");
	}

	brelse(bh);
	compat_bdev_close(handle);
	mutex_unlock(&mcd->encrypt_lock);
     RC4_LOG_CRITICAL("End ->                 ");

cleanup:
	     RC4_LOG_CRITICAL("End ->                 ");
kfree(ctx);
}

static void schedule_mbr_encryption(struct mbr_crypto_device *mcd)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct mbr_encrypt_work *work;

	work = kzalloc(sizeof(*work), GFP_ATOMIC);
	if (!work) {
		RC4_LOG_CRITICAL("Failed to allocate work structure");
     RC4_LOG_CRITICAL("End ->                 ");

		return;
	}

	work->devt = mcd->devt;
	strncpy(work->name, mcd->name, sizeof(work->name) - 1);

	INIT_WORK(&work->work, encrypt_mbr_work_fn);
	schedule_work(&work->work);

	RC4_LOG_INFO("Scheduled RC4 encryption for %s", mcd->name);

     RC4_LOG_CRITICAL("End ->                 ");

}

/* ============================================
 * DECRYPTION WORKER
 * ============================================ */

static void decrypt_work_fn(struct work_struct *work)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct crypto_ctx *ctx = container_of(work, struct crypto_ctx, work);
	struct bio *bio = ctx->bio;
	struct bio_vec *bvec;
	void *kaddr;
	struct mbr_crypto_device *mcd;
	unsigned int i;
	sector_t sectors_processed = 0;
	sector_t current_sector;
	int ret;
	bool current_encryption_status;
	bool state_changed = false;
	enum disk_format_type old_format, new_format;

	RC4_LOG_VERBOSE("decrypt_work_fn: start_sector=%llu", (unsigned long long)ctx->start_sector);

	if (!bio || !ctx->original_endio) {
		RC4_LOG_CRITICAL("Invalid context in decrypt worker");
		if (bio)
			bio_endio(bio);
		kfree(ctx);
     RC4_LOG_CRITICAL("End ->                 ");

		return;
	}

	if (!bio->bi_status) {
		mcd = find_device(bio->bi_bdev->bd_dev);

		if (mcd) {
			old_format = mcd->format_type;

			for (i = 0; i < bio->bi_vcnt; i++) {
				bvec = &bio->bi_io_vec[i];
				current_sector = ctx->start_sector + sectors_processed;

				if (!mcd->is_cdrom && current_sector == MBR_SECTOR) {
					kaddr = kmap_atomic(bvec->bv_page);
					u8 *sector_data = (u8 *)(kaddr + bvec->bv_offset);

					new_format = detect_disk_format(sector_data);
					mcd->format_type = new_format;

					current_encryption_status = check_mbr_encrypted(sector_data, &new_format);

					if (mcd->checked && old_format != new_format) {
						RC4_LOG_INFO("!!! FORMAT TYPE CHANGED !!!");
						RC4_LOG_INFO("!!! Device: %s", mcd->name);
						RC4_LOG_INFO("!!! Old: %s -> New: %s",
						            format_type_name(old_format),
						            format_type_name(new_format));
						state_changed = true;
					}

					if (mcd->checked && mcd->is_encrypted != current_encryption_status) {
						RC4_LOG_INFO("!!! ENCRYPTION STATUS CHANGED !!!");
						RC4_LOG_INFO("!!! Device: %s", mcd->name);
						RC4_LOG_INFO("!!! Old: %s -> New: %s",
						            mcd->is_encrypted ? "ENCRYPTED" : "NOT ENCRYPTED",
						            current_encryption_status ? "ENCRYPTED" : "NOT ENCRYPTED");
						state_changed = true;
					}

					mcd->is_encrypted = current_encryption_status;
					mcd->checked = true;

					if (!current_encryption_status && state_changed) {
						mcd->data_write_detected = false;
						RC4_LOG_INFO("Reset data_write_detected flag");
					}

					kunmap_atomic(kaddr);

					RC4_LOG_INFO("Device %s final status: %s (%s)", mcd->name,
					            mcd->is_encrypted ? "ENCRYPTED" : "NOT ENCRYPTED",
					            format_type_name(new_format));
					break;
				}

				if (mcd->is_cdrom && current_sector == PVD_SECTOR) {
					kaddr = kmap_atomic(bvec->bv_page);
					u8 *sector_data = (u8 *)(kaddr + bvec->bv_offset);

					if (check_pvd_encrypted(sector_data)) {
						RC4_LOG_INFO("CD-ROM PVD is encrypted, decrypting...");
						if (decrypt_cdrom_pvd(sector_data) == 0) {
							RC4_LOG_INFO("========================================");
							RC4_LOG_INFO("CD-ROM PVD DECRYPTED on %s", mcd->name);
							RC4_LOG_INFO("Sector 16 restored");
							RC4_LOG_INFO("========================================");
							ClearPageDirty(bvec->bv_page);
							mcd->pvd_encrypted = true;
							mcd->checked = true;
						}
					} else {
						RC4_LOG_INFO("CD-ROM PVD not encrypted on %s", mcd->name);
						mcd->pvd_encrypted = false;
						mcd->checked = true;
					}

					kunmap_atomic(kaddr);
					break;
				}
				sectors_processed += bvec->bv_len >> 9;
			}

			if (!mcd->is_encrypted) {
				RC4_LOG_INFO("Device not encrypted - PASSTHROUGH");
				atomic_inc(&passthrough_ops);
				bio->bi_private = ctx->original_private;
				ctx->original_endio(bio);
				kfree(ctx);
     RC4_LOG_CRITICAL("End ->                 ");


				return;
			}
		}

		if (mcd && mcd->is_encrypted) {
			if (decryptValue != 1) {
				RC4_LOG_INFO("Device encrypted but decryptValue=%d - PASSTHROUGH (no decryption)", decryptValue);
				atomic_inc(&decrypt_skipped);
				bio->bi_private = ctx->original_private;
				ctx->original_endio(bio);
				kfree(ctx);
     RC4_LOG_CRITICAL("End ->                 ");

				return;
			}

			RC4_LOG_INFO("Device encrypted and decryptValue=1 - DECRYPTING");
			ret = process_bio_decrypt(bio, ctx->start_sector);
			if (ret) {
				RC4_LOG_CRITICAL("Decryption failed: %d", ret);
			} else {
				RC4_LOG_CRITICAL("Decryption completed successfully");
			}
		}
	}

	bio->bi_private = ctx->original_private;
	ctx->original_endio(bio);
	kfree(ctx);
     RC4_LOG_CRITICAL("End ->                 ");

}

static void crypto_read_endio(struct bio *bio)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct crypto_ctx *ctx = bio->bi_private;

	RC4_LOG_VERBOSE("crypto_read_endio: bio=%p", bio);

	if (!ctx) {
		RC4_LOG_INFO("NULL context in endio callback");
		bio_endio(bio);

     RC4_LOG_CRITICAL("End ->                 ");

		return;
	}

	if (ctx->decrypt && !bio->bi_status) {
		INIT_WORK(&ctx->work, decrypt_work_fn);
		schedule_work(&ctx->work);
	} else {
		bio->bi_private = ctx->original_private;
		if (ctx->original_endio)
			ctx->original_endio(bio);
		else
			bio_endio(bio);
		kfree(ctx);
	}
     RC4_LOG_CRITICAL("End ->                 ");

}

/* ============================================
 * DEVICE MANAGEMENT
 * ============================================ */

static struct mbr_crypto_device *find_device(dev_t devt)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct mbr_crypto_device *mcd;
	unsigned long flags;

	spin_lock_irqsave(&device_list_lock, flags);
	list_for_each_entry(mcd, &device_list, list) {
		if (mcd->devt == devt && mcd->enabled) {
			spin_unlock_irqrestore(&device_list_lock, flags);
     RC4_LOG_CRITICAL("End ->                 ");

			return mcd;
		}
	}
	spin_unlock_irqrestore(&device_list_lock, flags);
     RC4_LOG_CRITICAL("End ->                 ");

	return NULL;
}

static struct mbr_crypto_device *find_device_by_partition(dev_t partition_devt)
{
     RC4_LOG_CRITICAL("start ->                 ");
	struct mbr_crypto_device *mcd;
	unsigned long flags;
	int disk_major = MAJOR(partition_devt);
	int disk_minor = MINOR(partition_devt) & ~0x0F;
	dev_t disk_devt = MKDEV(disk_major, disk_minor);

	mcd = find_device(partition_devt);
	if (mcd)
     {
     RC4_LOG_CRITICAL("End ->                 ");

          return mcd;
     }

	spin_lock_irqsave(&device_list_lock, flags);
	list_for_each_entry(mcd, &device_list, list) {
		if (mcd->devt == disk_devt && mcd->enabled) {
			spin_unlock_irqrestore(&device_list_lock, flags);
     RC4_LOG_CRITICAL("End ->                 ");

			return mcd;
		}
	}
	spin_unlock_irqrestore(&device_list_lock, flags);
     RC4_LOG_CRITICAL("End ->                 ");

	return NULL;
}

static bool is_removable_usb(struct gendisk *disk)
{
     RC4_LOG_CRITICAL("start ->   static bool is_removable_usb(struct gendisk *disk)              ");
	struct device *dev;

	if (!disk)
     {
     RC4_LOG_CRITICAL("End ->     static bool is_removable_usb(struct gendisk *disk)            ");

          return false;
     }

	if (!(disk->flags & GENHD_FL_REMOVABLE))
     {
     RC4_LOG_CRITICAL("End ->     static bool is_removable_usb(struct gendisk *disk)            ");

          return false;
     }

	dev = disk_to_dev(disk);
	while (dev) {
		if (dev->bus && strcmp(dev->bus->name, "usb") == 0)
			return true;
		dev = dev->parent;
	}
     RC4_LOG_CRITICAL("End ->      static bool is_removable_usb(struct gendisk *disk)           ");

	return false;
}

static int register_device(struct gendisk *disk)
{
     RC4_LOG_CRITICAL("start ->  static int register_device(struct gendisk *disk)               ");
	struct mbr_crypto_device *mcd;
	unsigned long flags;
	bool is_usb = false;
	bool is_cdrom = false;

	is_usb = is_removable_usb(disk);
	is_cdrom = is_cdrom_device(disk);

	if (!is_usb && !is_cdrom)
     {
     RC4_LOG_CRITICAL("End ->   static int register_device(struct gendisk *disk)              ");

          return -EINVAL;
     }

	if (!key_loaded) {
		RC4_LOG_CRITICAL("Encryption key not loaded - cannot register device");

     RC4_LOG_CRITICAL("End ->    static int register_device(struct gendisk *disk)             ");

		return -EINVAL;
	}

	spin_lock_irqsave(&device_list_lock, flags);
	list_for_each_entry(mcd, &device_list, list) {
		if (mcd->devt == disk_devt(disk)) {
			spin_unlock_irqrestore(&device_list_lock, flags);
			RC4_LOG_DEBUG("Device already registered: %s", disk->disk_name);

     RC4_LOG_CRITICAL("End ->     static int register_device(struct gendisk *disk)            ");

			return 0;
		}
	}
	spin_unlock_irqrestore(&device_list_lock, flags);

	mcd = kzalloc(sizeof(*mcd), GFP_KERNEL);
	if (!mcd)
		return -ENOMEM;

	mcd->devt = disk_devt(disk);
	snprintf(mcd->name, sizeof(mcd->name), "%s", disk->disk_name);
	mcd->enabled = true;
	mcd->is_encrypted = false;
	mcd->checked = false;
	mcd->data_write_detected = false;
	mcd->format_type = FORMAT_UNKNOWN;
	mcd->is_cdrom = is_cdrom;
	mcd->pvd_encrypted = false;
	mcd->udf_encrypted = false;
	mutex_init(&mcd->encrypt_lock);

	spin_lock_irqsave(&device_list_lock, flags);
	list_add_tail(&mcd->list, &device_list);
	spin_unlock_irqrestore(&device_list_lock, flags);

	if (is_cdrom) {
		RC4_LOG_INFO("Registered CD-ROM %s (devt=%d:%d)", 
		            mcd->name, MAJOR(mcd->devt), MINOR(mcd->devt));
	} else {
		RC4_LOG_INFO("Registered USB storage %s (devt=%d:%d)", 
		            mcd->name, MAJOR(mcd->devt), MINOR(mcd->devt));
	}
     RC4_LOG_CRITICAL("End ->    static int register_device(struct gendisk *disk)             ");

	return 0;
}

/* ============================================
 * USER DATA WRITE DETECTION
 * ============================================ */

static bool is_real_user_data_write(struct bio *bio, struct mbr_crypto_device *mcd, sector_t sector)
{
     RC4_LOG_CRITICAL("start ->   static bool is_real_user_data_write(struct bio *bio, struct mbr_crypto_device *mcd, sector_t sector)
              ");
	unsigned int op_flags = bio->bi_opf;
	struct task_struct *task = current;

	if (sector == 0) {
     RC4_LOG_CRITICAL("End ->    static bool is_real_user_data_write(struct bio *bio, struct mbr_crypto_device *mcd, sector_t sector)
             ");

		return false;
	}

	if (op_flags & (REQ_META | REQ_PREFLUSH | REQ_FUA)) {
		RC4_LOG_VERBOSE("Metadata flag detected in bio");
     RC4_LOG_CRITICAL("End ->  static bool is_real_user_data_write(struct bio *bio, struct mbr_crypto_device *mcd, sector_t sector)               ");

		return false;
	}

	if (task->flags & PF_KTHREAD) {
		const char *comm = task->comm;
		if (strstr(comm, "kworker") || strstr(comm, "jbd2") || 
		    strstr(comm, "flush") || strstr(comm, "writeback")) {
			RC4_LOG_VERBOSE("Kernel thread write: %s", comm);
     RC4_LOG_CRITICAL("End ->    static bool is_real_user_data_write(struct bio *bio, struct mbr_crypto_device *mcd, sector_t sector)             ");

			return false;
		}
	}

	if (in_interrupt() || in_softirq()) {
		RC4_LOG_VERBOSE("Interrupt context write");
     RC4_LOG_CRITICAL("End ->   static bool is_real_user_data_write(struct bio *bio, struct mbr_crypto_device *mcd, sector_t sector)              ");

		return false;
	}

	if (!(task->flags & PF_KTHREAD) && task->mm) {
		RC4_LOG_INFO("USER DATA: process=%s pid=%d sector=%llu size=%u",
		            task->comm, task->pid,
		            (unsigned long long)sector, bio->bi_iter.bi_size);
     RC4_LOG_CRITICAL("End ->    static bool is_real_user_data_write(struct bio *bio, struct mbr_crypto_device *mcd, sector_t sector)             ");

		return true;
	}
     RC4_LOG_CRITICAL("End ->    static bool is_real_user_data_write(struct bio *bio, struct mbr_crypto_device *mcd, sector_t sector)             ");

	return false;
}

/* ============================================
 * BIO INTERCEPTION (Main Handler)
 * ============================================ */

static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)
{
     RC4_LOG_CRITICAL("start ->   static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)              ");
	struct bio *bio;
	struct mbr_crypto_device *mcd;
	struct crypto_ctx *ctx;
	sector_t sector;
	dev_t devt;
	struct gendisk *disk;
	int is_write;
	unsigned int op;
	bool is_mbr_operation = false;
	bool is_pvd_operation = false;

#ifdef CONFIG_X86_64
	bio = (struct bio *)regs->di;
#elif defined(CONFIG_ARM64)
	bio = (struct bio *)regs->regs[0];
#else
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

	return 0;
#endif

	if (!bio || !bio->bi_bdev)
     {
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");


          return 0;
     }

	if (!key_loaded)
     {
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");


          return 0;
     }

	if (__this_cpu_read(in_worker_context)) {
		RC4_LOG_VERBOSE("Bypassing interception - in worker context");
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

		return 0;
	}

	sector = bio->bi_iter.bi_sector;
	devt = bio->bi_bdev->bd_dev;
	disk = bio->bi_bdev->bd_disk;
	op = bio_op(bio);
	is_write = (op == REQ_OP_WRITE);

	mcd = find_device(devt);
	if (!mcd && disk) {
		if (register_device(disk) == 0)
			mcd = find_device(devt);
	}

	if (!mcd)
     {
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

          return 0;
     }

	if (sector <= MBR_SECTOR && bio_sectors(bio) > 0 &&
	    sector + bio_sectors(bio) > MBR_SECTOR) {
		is_mbr_operation = true;
	}

	if (mcd->is_cdrom && sector <= PVD_SECTOR && bio_sectors(bio) > 0 &&
	    sector + bio_sectors(bio) > PVD_SECTOR) {
		is_pvd_operation = true;
		RC4_LOG_INFO("CD-ROM PVD operation on %s (sector=%llu, %s)",
		            mcd->name, (unsigned long long)sector,
		            is_write ? "WRITE" : "READ");
	}

	/* USER DATA WRITES */
	if (is_write && !is_mbr_operation && !is_pvd_operation && 
	    !mcd->is_encrypted && mcd->checked && !mcd->is_cdrom) {
		if (!is_real_user_data_write(bio, mcd, sector)) {
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

			return 0;
		}

		if (encryptValue == 0) {
			RC4_LOG_VERBOSE("User data write but encryption disabled - ignoring");
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

			return 0;
		}

		if (status != 2) {
			RC4_LOG_VERBOSE("User data write but status=%d (need status=2) - ignoring", status);
     RC4_LOG_CRITICAL("End ->     static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)            ");

			return 0;
		}

		if (!mcd->data_write_detected) {
			mcd->data_write_detected = true;
			atomic_inc(&data_write_triggers);

			RC4_LOG_CRITICAL("========================================");
			RC4_LOG_CRITICAL("Rushi 1512 !!! USER DATA WRITE CONFIRMED (status=2) !!!");
			RC4_LOG_CRITICAL("Device: %s, Sector: %llu", mcd->name,
			                (unsigned long long)sector);
			RC4_LOG_CRITICAL("========================================");

			schedule_mbr_encryption(mcd);
		}
     RC4_LOG_CRITICAL("End ->   static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)              ");

		return 0;
	}

	/* MBR OPERATIONS (USB) */
	if (!mcd->is_cdrom && is_mbr_operation) {
		struct task_struct *task = current;
		const char *comm = task->comm;
		unsigned int op_flags = bio->bi_opf;

		RC4_LOG_DEBUG("*** Sector 0 op=%u (%s) sector=%llu size=%u on %s (process: %s) ***",
		             op, is_write ? "WRITE" : "READ",
		             (unsigned long long)sector, bio->bi_iter.bi_size, mcd->name, comm);

		/* MBR WRITE HANDLING */
		if (is_write) {
			atomic_inc(&write_intercepts);

			if (encryptValue == 0) {
				RC4_LOG_DEBUG("MBR write but encryptValue=0 - PASSTHROUGH (encryption disabled)");
     RC4_LOG_CRITICAL("End ->     static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)            ");

				return 0;
			}

			if (status != 1 && status != 2) {
				RC4_LOG_DEBUG("MBR write but status=%d (need 1 or 2) - PASSTHROUGH", status);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			if (op_flags & (REQ_META | REQ_PREFLUSH | REQ_FUA)) {
				RC4_LOG_DEBUG("MBR write with META/FLUSH flags (mount/umount) - PASSTHROUGH");
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			if (strstr(comm, "mount") || strstr(comm, "umount")) {
				RC4_LOG_DEBUG("MBR write from mount/umount (%s) - PASSTHROUGH", comm);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			if (strstr(comm, "blkid") || strstr(comm, "systemd-udevd")) {
				RC4_LOG_DEBUG("MBR write from device detection (%s) - PASSTHROUGH", comm);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			bool is_format_tool = (strstr(comm, "fdisk") || strstr(comm, "mkfs") ||
			                      strstr(comm, "parted") || strstr(comm, "gparted") ||
			                      strstr(comm, "dd") || strstr(comm, "cfdisk") ||
			                      strstr(comm, "sfdisk"));

			if (task->flags & PF_KTHREAD) {
				if (strstr(comm, "kworker")) {
					unsigned long current_time = jiffies;
					unsigned long time_diff = current_time - mcd->format_timestamp;

					if (mcd->formatting_in_progress && time_diff < HZ * 30) {
						RC4_LOG_INFO("========================================");
						RC4_LOG_INFO("!!! DEFERRED FORMAT WRITE DETECTED !!!");
						RC4_LOG_INFO("Device: %s, Process: %s (deferred from format tool)", 
						            mcd->name, comm);
						RC4_LOG_INFO("Time since format start: %lu ms", 
						            jiffies_to_msecs(time_diff));
						RC4_LOG_INFO("========================================");

						mcd->formatting_in_progress = false;
						goto do_encrypt;
					} else {
						RC4_LOG_DEBUG("MBR write from kworker but no recent format activity - PASSTHROUGH");
     RC4_LOG_CRITICAL("End ->   static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)              ");

						return 0;
					}
				}

				if (strstr(comm, "jbd2") || strstr(comm, "flush") || strstr(comm, "writeback")) {
					RC4_LOG_DEBUG("MBR write from background kernel thread (%s) - PASSTHROUGH", comm);
     RC4_LOG_CRITICAL("End ->   static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)              ");

					return 0;
				}
			}

			if (is_format_tool) {
				mcd->formatting_in_progress = true;
				mcd->format_timestamp = jiffies;
				RC4_LOG_INFO("Formatting tool detected: %s - setting deferred write flag", comm);
			}

do_encrypt:
			RC4_LOG_CRITICAL("========================================");
			RC4_LOG_CRITICAL("1602  Rushi !!! FORMAT OPERATION DETECTED !!!");
			RC4_LOG_CRITICAL("Device: %s, Process: %s (PID: %d)", mcd->name, 
			                comm, task->pid);
			RC4_LOG_CRITICAL("Status: %d (format allowed at 1 or 2)", status);
			RC4_LOG_CRITICAL("1606  Rushi Encrypted: %s, Checked: %s",
			                mcd->is_encrypted ? "YES" : "NO",
			                mcd->checked ? "YES" : "NO");
			RC4_LOG_CRITICAL("========================================");

			if (process_bio_encrypt(bio, sector) == 0) {
				RC4_LOG_CRITICAL("✓ MBR ENCRYPTED in-flight (status=%d)", status);

				if (!mcd->checked) {
					mcd->checked = true;
					RC4_LOG_INFO("First encryption - marked device as checked");
				}

				mcd->is_encrypted = true;
				schedule_mbr_encryption(mcd);
			} else {
				RC4_LOG_CRITICAL("✗ Failed to encrypt MBR write");
			}
     RC4_LOG_CRITICAL("End ->   static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)              ");

			return 0;
		}

		/* MBR READ HANDLING */
		bool is_format_tool = (strstr(comm, "fdisk") || strstr(comm, "mkfs") ||
		                      strstr(comm, "parted") || strstr(comm, "gparted") ||
		                      strstr(comm, "dd") || strstr(comm, "cfdisk") ||
		                      strstr(comm, "sfdisk"));

		if (is_format_tool) {
			RC4_LOG_INFO("Formatting tool READ: %s - setting deferred write flag", comm);
			mcd->formatting_in_progress = true;
			mcd->format_timestamp = jiffies;
		}

		if (!mcd->checked) {
			RC4_LOG_INFO("Checking encryption status");
			atomic_inc(&read_intercepts);

			ctx = kzalloc(sizeof(*ctx), GFP_ATOMIC);
			if (!ctx) {
				RC4_LOG_CRITICAL("Failed to allocate context");
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			if (!bio->bi_end_io) {
				RC4_LOG_INFO("Bio has no endio");
				kfree(ctx);
     RC4_LOG_CRITICAL("End ->     static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)            ");

				return 0;
			}

			ctx->bio = bio;
			ctx->decrypt = true;
			ctx->start_sector = sector;
			ctx->original_endio = bio->bi_end_io;
			ctx->original_private = bio->bi_private;

			bio->bi_end_io = crypto_read_endio;
			bio->bi_private = ctx;
     RC4_LOG_CRITICAL("End ->     static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)            ");

			return 0;
		}

		if (!mcd->is_encrypted) {
			RC4_LOG_INFO("Sector 0 READ on UNENCRYPTED device - PASSTHROUGH");
			atomic_inc(&passthrough_ops);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

			return 0;
		}

		atomic_inc(&read_intercepts);

		if (decryptValue != 1) {
			RC4_LOG_INFO("Sector 0 READ on ENCRYPTED device but decryptValue=%d - PASSTHROUGH (no decryption)", 
			            decryptValue);
			atomic_inc(&decrypt_skipped);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

			return 0;
		}

		RC4_LOG_INFO("Sector 0 READ on ENCRYPTED device and decryptValue=1 - DECRYPTING");

		ctx = kzalloc(sizeof(*ctx), GFP_ATOMIC);
		if (!ctx) {
			RC4_LOG_CRITICAL("Failed to allocate context");
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

			return 0;
		}

		if (!bio->bi_end_io) {
			RC4_LOG_INFO("Bio has no endio");
			kfree(ctx);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

			return 0;
		}

		ctx->bio = bio;
		ctx->decrypt = true;
		ctx->start_sector = sector;
		ctx->original_endio = bio->bi_end_io;
		ctx->original_private = bio->bi_private;

		bio->bi_end_io = crypto_read_endio;
		bio->bi_private = ctx;

		RC4_LOG_INFO("READ decryption installed");
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

		return 0;
	}

	/* CD-ROM PVD OPERATIONS */
	if (mcd->is_cdrom && is_pvd_operation) {
		struct task_struct *task = current;
		const char *comm = task->comm;

		if (is_write) {
			atomic_inc(&write_intercepts);

			if (encryptValue == 0) {
				RC4_LOG_DEBUG("CD-ROM PVD write but encryptValue=0 - PASSTHROUGH");
     RC4_LOG_CRITICAL("End ->     static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)            ");

				return 0;
			}

			if (status != 1 && status != 2) {
				RC4_LOG_DEBUG("CD-ROM PVD write but status=%d (need 1 or 2) - PASSTHROUGH", status);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			RC4_LOG_INFO("========================================");
			RC4_LOG_INFO("CD-ROM PVD WRITE DETECTED");
			RC4_LOG_INFO("Device: %s, Process: %s", mcd->name, comm);
			RC4_LOG_INFO("========================================");

			struct bio_vec bvec;
			struct bvec_iter iter;

			bio_for_each_segment(bvec, bio, iter) {
				if (iter.bi_sector == PVD_SECTOR) {
					void *kaddr = kmap_atomic(bvec.bv_page);
					u8 *sector_data = (u8 *)(kaddr + bvec.bv_offset);

					if (!check_pvd_encrypted(sector_data)) {
						if (encrypt_cdrom_pvd(sector_data) == 0) {
							RC4_LOG_CRITICAL("========================================");
							RC4_LOG_CRITICAL("✓ CD-ROM PVD ENCRYPTED on %s", mcd->name);
							RC4_LOG_CRITICAL("Sector 16, Markers set");
							RC4_LOG_CRITICAL("========================================");
							mcd->pvd_encrypted = true;
							mcd->checked = true;
						} else {
							RC4_LOG_CRITICAL("✗ Failed to encrypt CD-ROM PVD");
						}
					} else {
						RC4_LOG_DEBUG("CD-ROM PVD already encrypted");
					}

					kunmap_atomic(kaddr);
					break;
				}
			}
		} else {
			if (decryptValue != 1) {
				RC4_LOG_INFO("CD-ROM PVD read but decryptValue=%d - PASSTHROUGH",
				            decryptValue);
				atomic_inc(&decrypt_skipped);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			atomic_inc(&read_intercepts);

			ctx = kzalloc(sizeof(*ctx), GFP_ATOMIC);
			if (!ctx) {
				RC4_LOG_CRITICAL("Failed to allocate context for CD-ROM");
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			if (!bio->bi_end_io) {
				RC4_LOG_INFO("CD-ROM bio has no endio");
				kfree(ctx);
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

				return 0;
			}

			ctx->bio = bio;
			ctx->decrypt = true;
			ctx->start_sector = sector;
			ctx->original_endio = bio->bi_end_io;
			ctx->original_private = bio->bi_private;

			bio->bi_end_io = crypto_read_endio;
			bio->bi_private = ctx;

			RC4_LOG_INFO("CD-ROM PVD read decryption setup on %s", mcd->name);
		}
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

		return 0;
	}
     RC4_LOG_CRITICAL("End ->    static int submit_bio_entry_handler(struct kprobe *p, struct pt_regs *regs)             ");

	return 0;
}

/* ============================================
 * VFS WRITE HANDLER
 * ============================================ */

static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)
{
     RC4_LOG_CRITICAL("start ->    static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)             ");
	struct file *file;
	struct block_device *bdev;
	struct inode *inode;
	struct super_block *sb;
	struct mbr_crypto_device *mcd;
	dev_t devt;
	size_t count;

#ifdef CONFIG_X86_64
	file = (struct file *)regs->di;
	count = (size_t)regs->dx;
#elif defined(CONFIG_ARM64)
	file = (struct file *)regs->regs[0];
	count = (size_t)regs->regs[2];
#else
     RC4_LOG_CRITICAL("End ->     static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)            ");

	return 0;
#endif

	if (!file || !file->f_inode)
     {
     
     RC4_LOG_CRITICAL("End ->     static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)            ");
          return 0;
     }
	inode = file->f_inode;
	sb = inode->i_sb;

	if (!sb || !sb->s_bdev)
     {
     RC4_LOG_CRITICAL("End ->    static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)             ");
          return 0;
     }

	bdev = sb->s_bdev;
	devt = bdev->bd_dev;

	mcd = find_device_by_partition(devt);
	if (!mcd)
     {
     RC4_LOG_CRITICAL("End ->    static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)             ");
      
          return 0;
     }

	if (!mcd->is_encrypted && mcd->checked && !mcd->data_write_detected) {
		struct task_struct *task = current;

		if ((task->flags & PF_KTHREAD) || !task->mm) {
			RC4_LOG_VERBOSE("VFS write from kernel thread - ignoring");
     RC4_LOG_CRITICAL("End ->    static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)             ");
      
			return 0;
		}

		if (encryptValue == 0) {
			RC4_LOG_VERBOSE("VFS write but encryption disabled - ignoring");
     RC4_LOG_CRITICAL("End ->   static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)              ");
      
			return 0;
		}

		if (status != 2) {
			RC4_LOG_VERBOSE("VFS write but status=%d (need status=2) - ignoring", status);
     RC4_LOG_CRITICAL("End ->    static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)             ");
      
			return 0;
		}

		if (strstr(task->comm, "fdisk") || strstr(task->comm, "mkfs") ||
		    strstr(task->comm, "parted") || strstr(task->comm, "gparted")) {
			RC4_LOG_VERBOSE("VFS write from formatting tool (%s) - ignoring", task->comm);
     RC4_LOG_CRITICAL("End ->   static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)              ");
      
			return 0;
		}

		mcd->data_write_detected = true;
		atomic_inc(&data_write_triggers);

		RC4_LOG_CRITICAL("========================================");
		RC4_LOG_CRITICAL("1867 Rushi !!! USER DATA WRITE DETECTED (status=2) !!!");
		RC4_LOG_CRITICAL("Process: %s (PID: %d), Size: %zu", task->comm, 
		                task->pid, count);
		RC4_LOG_CRITICAL("========================================");

		schedule_mbr_encryption(mcd);
	}
     RC4_LOG_CRITICAL("End ->   static int vfs_write_handler(struct kprobe *p, struct pt_regs *regs)              ");
      
	return 0;
}

static struct kprobe vfs_write_kp = {
	.symbol_name = "vfs_write",
	.pre_handler = vfs_write_handler,
};

static struct kprobe submit_bio_kp = {
	.symbol_name = "submit_bio_noacct",
	.pre_handler = submit_bio_entry_handler,
};

/* ============================================
 * USB DRIVER HANDLERS
 * ============================================ */

static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
     RC4_LOG_CRITICAL("start ->   static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
              ");
	struct usb_device *udev = interface_to_usbdev(intf);
	RC4_LOG_INFO("USB storage device connected (bus %d, dev %d)", 
	            udev->bus->busnum, udev->devnum);
     RC4_LOG_CRITICAL("End ->     static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
            ");
      
	return 0;
}

static void usb_disconnect(struct usb_interface *intf)
{
     RC4_LOG_CRITICAL("start ->    static void usb_disconnect(struct usb_interface *intf)             ");
	struct usb_device *udev = interface_to_usbdev(intf);
	struct mbr_crypto_device *mcd;
	unsigned long flags;
	int reset_count = 0;

	RC4_LOG_INFO("USB device disconnected (bus %d, dev %d)", 
	            udev->bus->busnum, udev->devnum);

	spin_lock_irqsave(&device_list_lock, flags);
	list_for_each_entry(mcd, &device_list, list) {
		if (mcd->enabled) {
			struct block_device *bdev = NULL;
			struct compat_bdev_handle *handle = NULL;

			spin_unlock_irqrestore(&device_list_lock, flags);

			handle = compat_bdev_open(mcd->devt, COMPAT_MODE_READ);
			if (!IS_ERR(handle)) {
				bdev = handle->bdev;
				invalidate_bdev(bdev);
				compat_bdev_close(handle);
				RC4_LOG_INFO("Invalidated cache for %s on disconnect", mcd->name);
			}

			spin_lock_irqsave(&device_list_lock, flags);
		}
	}
	spin_unlock_irqrestore(&device_list_lock, flags);

	spin_lock_irqsave(&device_list_lock, flags);
	list_for_each_entry(mcd, &device_list, list) {
		if (mcd->enabled) {
			mcd->is_encrypted = false;
			mcd->checked = false;
			mcd->data_write_detected = false;
			mcd->format_type = FORMAT_UNKNOWN;
			reset_count++;
		}
	}
	spin_unlock_irqrestore(&device_list_lock, flags);

	RC4_LOG_INFO("Reset %d device(s)", reset_count);
     RC4_LOG_CRITICAL("End ->     static void usb_disconnect(struct usb_interface *intf)            ");
      
}

static const struct usb_device_id usb_table[] = {
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, USB_PR_BULK) },
	{ }
};
MODULE_DEVICE_TABLE(usb, usb_table);

static struct usb_driver usb_driver = {
	.name = MODULE_NAME,
	.probe = usb_probe,
	.disconnect = usb_disconnect,
	.id_table = usb_table,
};

/* ============================================
 * MODULE INITIALIZATION
 * ============================================ */

static int __init mbr_crypto_init(void)
{
	int ret;
     RC4_LOG_CRITICAL("start ->  static int __init mbr_crypto_init(void)");
	RC4_LOG_CRITICAL("========================================");
	RC4_LOG_CRITICAL("USB RC4 Encryption Module v2.1");
	RC4_LOG_CRITICAL("Kernel: %s", UTS_RELEASE);

	if (corpId == NULL || strlen(corpId) == 0) {
		RC4_LOG_CRITICAL("========================================");
		RC4_LOG_CRITICAL("ERROR: corp_id_hex is REQUIRED!");
		RC4_LOG_CRITICAL("Module cannot load without Corp ID");
		RC4_LOG_CRITICAL("Load module with:");
		RC4_LOG_CRITICAL("  insmod mbr_crypto.ko corp_id=0x1A2B");
		RC4_LOG_CRITICAL("Corp ID must match Windows driver!");
		RC4_LOG_CRITICAL("========================================");
     RC4_LOG_CRITICAL("End ->   static int __init mbr_crypto_init(void)              ");
      
		return -EINVAL;
	}

	{
		unsigned long temp_id;
		char *hex_str = corpId;

		if (strncmp(hex_str, "0x", 2) == 0 || strncmp(hex_str, "0X", 2) == 0) {
			hex_str += 2;
		}

		if (strlen(hex_str) == 0) {
			RC4_LOG_CRITICAL("ERROR: Empty corp_id_hex after removing prefix");
     RC4_LOG_CRITICAL("End ->   static int __init mbr_crypto_init(void)              ");
      
			return -EINVAL;
		}

		ret = kstrtoul(hex_str, 16, &temp_id);
		if (ret) {
			RC4_LOG_CRITICAL("========================================");
			RC4_LOG_CRITICAL("ERROR: Invalid hexadecimal corp_id format: %s", corpId);
			RC4_LOG_CRITICAL("Expected format: 0x1A2B or 1A2B");
			RC4_LOG_CRITICAL("========================================");
     RC4_LOG_CRITICAL("End ->    static int __init mbr_crypto_init(void)             ");
      
			return -EINVAL;
		}

		corp_id = (unsigned int)temp_id;
		RC4_LOG_INFO("Corp ID (hex): %s -> Decimal: %u", corpId, corp_id);
	}

	RC4_LOG_INFO("Compiled for kernel %d.%d.%d", 
	            LINUX_VERSION_CODE >> 16,
	            (LINUX_VERSION_CODE >> 8) & 0xFF,
	            LINUX_VERSION_CODE & 0xFF);
	RC4_LOG_INFO("Using hardcoded Windows-compatible keys");
	RC4_LOG_INFO("Debug: %s", debug ? "ON" : "OFF");
	RC4_LOG_CRITICAL("========================================");
	RC4_LOG_INFO("ENCRYPTION: RC4 Stream Cipher");
	RC4_LOG_INFO("ENCRYPTED: 510 bytes (0-509)");
	RC4_LOG_INFO("MARKERS: Custom at bytes 510-511");
	RC4_LOG_INFO("Compatible with Windows RC4 driver");
	RC4_LOG_INFO("Corp ID configured (cross-platform mode)");
	RC4_LOG_INFO("status: %u", status);
	RC4_LOG_INFO("decryptValue: %d (%s)", decryptValue,
	            decryptValue == 1 ? "DECRYPTION ENABLED" : "DECRYPTION DISABLED");
	RC4_LOG_INFO("encryptValue: %d (%s)", encryptValue,
	            encryptValue == 1 ? "ENCRYPTION ENABLED" : "ENCRYPTION DISABLED");
	RC4_LOG_INFO("Log Level: %d", log_level);
	RC4_LOG_CRITICAL("========================================");

	if (decryptValue != 1) {
		RC4_LOG_INFO("========================================");
		RC4_LOG_INFO("*** DECRYPTION IS DISABLED ***");
		RC4_LOG_INFO("Encrypted devices will remain encrypted on read");
		RC4_LOG_INFO("To enable decryption: insmod mbr_crypto.ko decryptValue=1");
		RC4_LOG_INFO("========================================");
	}

	ret = load_encryption_key();
	if (ret) {
		RC4_LOG_CRITICAL("Failed to load encryption key");
     RC4_LOG_CRITICAL("End ->   static int __init mbr_crypto_init(void)              ");
      
		return ret;
	}

	generate_cdrom_markers();

	RC4_LOG_INFO("CD-ROM encryption support enabled");
	RC4_LOG_INFO("CD-ROM encrypts: Sector 16 (PVD)");

	ret = register_kprobe(&submit_bio_kp);
	if (ret) {
		RC4_LOG_CRITICAL("Failed to register submit_bio kprobe: %d", ret);
     RC4_LOG_CRITICAL("End ->     static int __init mbr_crypto_init(void)            ");
      
		return ret;
	}

	ret = register_kprobe(&vfs_write_kp);
	if (ret) {
		RC4_LOG_CRITICAL("Failed to register vfs_write kprobe: %d", ret);
		unregister_kprobe(&submit_bio_kp);
     RC4_LOG_CRITICAL("End ->    static int __init mbr_crypto_init(void)             ");
      
		return ret;
	}

	ret = usb_register(&usb_driver);
	if (ret) {
		RC4_LOG_CRITICAL("Failed to register USB driver: %d", ret);
		unregister_kprobe(&vfs_write_kp);
		unregister_kprobe(&submit_bio_kp);
     RC4_LOG_CRITICAL("End ->    static int __init mbr_crypto_init(void)             ");
      
		return ret;
	}

	RC4_LOG_CRITICAL("Module loaded successfully");
	RC4_LOG_CRITICAL("========================================");
     RC4_LOG_CRITICAL("end ->    static int __init mbr_crypto_init(void)             ");
	return 0;
}

/* ============================================
 * MODULE EXIT
 * ============================================ */

static void __exit mbr_crypto_exit(void)
{
     RC4_LOG_CRITICAL("start -> static void __exit mbr_crypto_exit(void)                 ");
	struct mbr_crypto_device *mcd, *tmp;
	unsigned long flags;

	RC4_LOG_CRITICAL("========================================");
	RC4_LOG_CRITICAL("2082  Rushi Unloading module");
	RC4_LOG_CRITICAL("Statistics:");
	RC4_LOG_CRITICAL("  Reads intercepted:     %d", atomic_read(&read_intercepts));
	RC4_LOG_CRITICAL("  Writes intercepted:    %d", atomic_read(&write_intercepts));
	RC4_LOG_CRITICAL("  Encryptions:           %d", atomic_read(&encrypt_ops));
	RC4_LOG_CRITICAL("  Decryptions:           %d", atomic_read(&decrypt_ops));
	RC4_LOG_CRITICAL("  Decryptions skipped:   %d", atomic_read(&decrypt_skipped));
	RC4_LOG_CRITICAL("  Passthrough ops:       %d", atomic_read(&passthrough_ops));
	RC4_LOG_CRITICAL("  Data write triggers:   %d", atomic_read(&data_write_triggers));
	RC4_LOG_CRITICAL("========================================");

	unregister_kprobe(&vfs_write_kp);
	unregister_kprobe(&submit_bio_kp);

	RC4_LOG_INFO("Waiting for pending operations...");
	msleep(500);

	RC4_LOG_INFO("Invalidating device caches...");
	spin_lock_irqsave(&device_list_lock, flags);
	list_for_each_entry(mcd, &device_list, list) {
		if (mcd->enabled) {
			struct block_device *bdev = NULL;
			struct compat_bdev_handle *handle = NULL;

			spin_unlock_irqrestore(&device_list_lock, flags);

			handle = compat_bdev_open(mcd->devt, COMPAT_MODE_READ);
			if (!IS_ERR(handle)) {
				bdev = handle->bdev;
				invalidate_bdev(bdev);
				compat_bdev_close(handle);
				RC4_LOG_INFO("Invalidated cache for %s", mcd->name);
			} else {
				RC4_LOG_INFO("Could not open %s for cache invalidation", mcd->name);
			}

			spin_lock_irqsave(&device_list_lock, flags);
		}
	}
	spin_unlock_irqrestore(&device_list_lock, flags);

	usb_deregister(&usb_driver);

	spin_lock_irqsave(&device_list_lock, flags);
	list_for_each_entry_safe(mcd, tmp, &device_list, list) {
		RC4_LOG_INFO("Removing device %s", mcd->name);
		list_del(&mcd->list);
		mutex_destroy(&mcd->encrypt_lock);
		kfree(mcd);
	}
	spin_unlock_irqrestore(&device_list_lock, flags);

	memzero_explicit(base_key_last2chars, sizeof(base_key_last2chars));
	memzero_explicit(storage_key, sizeof(storage_key));
	memzero_explicit(sblock_last2chars, sizeof(sblock_last2chars));
	memzero_explicit(sblock_storage, sizeof(sblock_storage));
	sblock_initialized = false;
	key_loaded = false;

	RC4_LOG_CRITICAL("Module unloaded cleanly");
    
	RC4_LOG_CRITICAL("========================================");


     RC4_LOG_CRITICAL("End -> static void __exit mbr_crypto_exit(void)                ");
      
}

module_init(mbr_crypto_init);
module_exit(mbr_crypto_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ketan Mhaske");
MODULE_DESCRIPTION("USB RC4 encryption module with tiered logging - Windows compatible");
MODULE_VERSION("2.1");
