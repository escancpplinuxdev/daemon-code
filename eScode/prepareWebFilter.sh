#!/bin/bash
ECHO=`which echo`
GCC=`which gcc`
MAKE=`which make`
CP=`which cp`
MV=`which mv`
SETIMMUTABLE=0
RM=`which rm`
KERNELVER=`uname -r`

function checkFileExist()
{
	ECHO=`which echo`
	FILENAME=$1
	if [ ! -f $FILENAME ]; then
		$ECHO "File does not exist : $FILENAME"
		$MV /opt/MicroWorld/netfilter_kmodule /opt/MicroWorld/.netfilter_kmodule
		exit 1;
	else
		$ECHO "$FILENAME...ok" 
	fi	
}
function commentLimitsHeader()
{
	KVERSION=`uname -r`
	FILENAME=$1
	FILENAME2=$2
	ECHO=`which echo`
	CAT=`which cat`
	GREP=`which grep`
	CP=`which cp`

	#if [ ! -f $FILENAME ] && [ ! -f $FILENAME2 ]; then 
	#	return 0;
	#fi

	if [ ! -f $FILENAME ]; then 
		$ECHO "File does not exist : $FILENAME"
		#return 1;
	fi

	$CP $FILENAME ${FILENAME}_MW_ORIG
	if [ $? -ne 0 ]; then
		$ECHO "Failed to take backup : $FILENAME"
		return 1;
	fi
	if [ -f $FILENAME2 ]; then
		$CP $FILENAME2 ${FILENAME2}_MW_ORIG
	fi
	$CAT ${FILENAME}_MW_ORIG | $GREP -v "limits.h" > $FILENAME
	if [ $? -ne 0 ]; then
                $ECHO "Failed to remove entry from file : $FILENAME"
                return 1;
        fi
	
	if [ -f $FILENAME2 ]; then
		$CAT ${FILENAME2}_MW_ORIG | $GREP -v "limits.h" > $FILENAME2
	fi
}

function restoreLimitsHeader()
{
	KVERSION=`uname -r`
	FILENAME=$1
	FILENAME2=$2
	ECHO=`which echo`
	MV=`which mv`
	#FILENAME="/lib/modules/${KVERSION}/build/include/linux/netfilter_ipv4.h"
	#FILENAME2="/lib/modules/${KVERSION}/build/include/uapi/linux/netfilter_ipv4.h"
	if [ -f $FILENAME2 ]; then
		$MV ${FILENAME2}_MW_ORIG $FILENAME2
	fi
	if [ ! -f ${FILENAME}_MW_ORIG ]; then
		$ECHO "File does not exist : ${FILENAME}_MW_ORIG"
		return 1;
	fi
	$MV ${FILENAME}_MW_ORIG $FILENAME
	if [ $? -ne 0 ]; then
                $ECHO "Failed to restore backup : $FILENAME"
                return 1;
        fi
}

function checkKernelVersion()
{
	MAJOR=`uname -r | cut -d '-' -f1 | cut -d '.' -f1`
	MIDDLE=`uname -r | cut -d '-' -f1 | cut -d '.' -f2`
	LAST=`uname -r | cut -d '-' -f1 | cut -d '.' -f3`

	if [ ${MAJOR} -eq 2 ]; then
        	if [ ${MIDDLE} -ge 6 ]; then
                	if [ ${LAST} -ge 32 ]; then
                        	return 1;
                	else
                        	return 0;
                	fi
        	else
                	return 0;
        	fi

	elif [ ${MAJOR} -gt 2 ]; then
        	return 1;
	fi
	return 0
}

#Created by Anup Agrawal 15 Dec 2020
function checkOtherKernalHeader()
{
	KVER=`uname -r`

	apt-cache search linux-headers | grep $KVER
	if [ $? -eq 0 ]; then
		return 0;
	else
		KARCH=`uname -r | cut -d '-' -f3`
		KVER=`uname -r | cut --complement -d '-' -f 3`
		#Regex_Kver='^[0-9]+\.[0-9]+\.[0-9]+-[0-9]+$'


		LinuxHeadersPkgs=`apt-cache search linux-headers | grep $KARCH | sed -n -r "s/.*([0-9]+\.[0-9]+\.[0-9]+-[0-9]+)[ -]+.*/\1/p" | sort -V`
		readarray -t LinuxHeadersPkgList <<<"$LinuxHeadersPkgs"
		for index in "${LinuxHeadersPkgList[@]}"
		do
			min=$(echo -e $KVER"\n"$index|sort -V|head -n 1)
			if [ "$min" == "$KVER" ];then
				READLINK=`which readlink`
				KERNELHBPATH=`${READLINK} /lib/modules/$index-$KARCH/build`
				if [ $? -eq 0 ]; then
					if [ -d ${KERNELHDRPATH} ]; then
						if [ "$(ls -A $KERNELHDRPATH )" ]; then
								NKVER="$index"
								return 0;
						fi
					fi
				fi
				NKVER="$index"
				return 1;
			fi
		done
		return 1;
	fi
}

function checkKernalHeader()
{
	READLINK=`which readlink`
	KERNELHDRPATH=`${READLINK} /lib/modules/$(uname -r)/build`
	if [ $? -eq 0 ]; then
		if [ -d ${KERNELHDRPATH} ]; then
			if [ "$(ls -A $KERNELHDRPATH )" ]; then
				return 0;
			else
				return 1;
			fi
		else
			return 1;
		fi
	else
		return 1;
	fi
}

function checkKernalHeaderAlt()
{

	READLINK=`which readlink`
	#KERNELHDRPATH=`${READLINK} /lib/modules/$(uname -r)/source`
	KERNELHDRPATH=`${READLINK} /lib/modules/${KERNELVER}/source`
	if [ $? -eq 0 ]; then
		if [ -d ${KERNELHDRPATH} ]; then
			if [ "$(ls -A $KERNELHDRPATH )" ]; then
				return 0;
			else
				return 1;
			fi
		else
			return 1;
		fi
	else
		return 1;
	fi
}

function prepareToRedownloadPolicy()
{
	CP=`which cp`
	MV=`which mv`
	CAT=`which cat`
	GREP=`which grep`
	RM=`which rm`

	if [ -f /opt/MicroWorld/var/avsupd/dirlist.txt ]; then
		$CAT /opt/MicroWorld/var/avsupd/dirlist.txt | $GREP -v "l0Policy.dlz\|L0Policy.dlz" > /opt/MicroWorld/var/avsupd/dirlist.txt_tmp
		if [ $? -eq 0 ]; then
			$MV /opt/MicroWorld/var/avsupd/dirlist.txt_tmp /opt/MicroWorld/var/avsupd/dirlist.txt
		fi
	fi
	if [ -f /opt/MicroWorld/var/backavsupd/dirlist.txt ]; then
		$CAT /opt/MicroWorld/var/backavsupd/dirlist.txt | $GREP -v "l0Policy.dlz\|L0Policy.dlz" > /opt/MicroWorld/var/backavsupd/dirlist.txt_tmp
		if [ $? -eq 0 ]; then
			$MV /opt/MicroWorld/var/backavsupd/dirlist.txt_tmp /opt/MicroWorld/var/backavsupd/dirlist.txt
		fi
	fi

	if [ -f /opt/MicroWorld/var/avsupd/l0Policy.dlz ]; then
                ${RM} -f /opt/MicroWorld/var/avsupd/l0Policy.dlz
        elif [ -f /opt/MicroWorld/var/avsupd/L0Policy.dlz ]; then
                ${RM} -f /opt/MicroWorld/var/avsupd/L0Policy.dlz
        fi

        if [ -f /opt/MicroWorld/var/backavsupd/l0Policy.dlz ]; then
                ${RM} -f /opt/MicroWorld/var/backavsupd/l0Policy.dlz
        elif [ -f /opt/MicroWorld/var/backavsupd/L0Policy.dlz ]; then
                ${RM} -f /opt/MicroWorld/var/backavsupd/L0Policy.dlz
        fi

	 ${RM} -rf /var/MicroWorld/var/updinfo/*
}

function stopService()
{
	KILLALL=`which killall`
	SERVICENAME=$1
	ISSUCCESS=1

	if [ "x" = "x$SERVICENAME" ]; then
		return 1
	fi
	if [ -f /bin/systemctl ]; then
		/bin/systemctl stop ${SERVICENAME}.service >/dev/null 2>&1
		ISSUCCESS=$?
	elif [ -d /etc/init/ ] && [ $ISSUCCESS -ne 0 ] && [ -f /etc/init/${SERVICENAME} ]; then
		if [ -f /sbin/initctl ]; then
			/sbin/initctl stop $SERVICENAME >/dev/null 2>&1
			ISSUCCESS=$?
		fi
	elif [ -f /usr/sbin/invoke-rc.d ] && [ $ISSUCCESS -ne 0 ] && [ -f /etc/init.d/${SERVICENAME} ]; then
		invoke-rc.d $SERVICENAME  stop >/dev/null 2>&1
		ISSUCCESS=$?
	elif [ $ISSUCCESS -ne 0 ] && [ -f /etc/init.d/${SERVICENAME} ]; then
		/etc/init.d/${SERVICENAME} stop >/dev/null 2>&1
	fi
	$KILLALL $SERVICENAME >/dev/null 2>&1
	$KILLALL $SERVICENAME >/dev/null 2>&1
}

function startService()
{
	SERVICENAME=$1
	ISSUCCESS=1

	if [ "x" = "x$SERVICENAME" ]; then
		return 1
	fi
	if [ -f /bin/systemctl ]; then
		/bin/systemctl start ${SERVICENAME}.service >/dev/null 2>&1
		ISSUCCESS=$?

	elif [ -d /etc/init/ ] && [ $ISSUCCESS -ne 0 ] && [ -f /etc/init/${SERVICENAME} ]; then
		if [ -f /sbin/initctl ]; then
			/sbin/initctl start $SERVICENAME >/dev/null 2>&1
			ISSUCCESS=$?
		fi
	elif [ -f /usr/sbin/invoke-rc.d ] && [ $ISSUCCESS -ne 0 ] && [ -f /etc/init.d/${SERVICENAME} ]; then
		invoke-rc.d $SERVICENAME  start >/dev/null 2>&1
		ISSUCCESS=$?
	elif [ $ISSUCCESS -ne 0 ] && [ -f /etc/init.d/${SERVICENAME} ]; then
		/etc/init.d/${SERVICENAME} start >/dev/null 2>&1
	fi
}

$ECHO
checkKernelVersion
if [ $? -eq 0 ]; then 

	${ECHO} "Checking Kernel version....Not supported (Supported Kernel version >= 2.6.32)."
	$ECHO
	if [ -d /opt/MicroWorld/netfilter_kmodule ]; then 
		$MV /opt/MicroWorld/netfilter_kmodule /opt/MicroWorld/.netfilter_kmodule
	fi
	exit 2;
else
	${ECHO} "Checking Kernel version...ok"
fi
RECOMPILE=0
PACKAGES_INSTALLED=0
NFDAEMON_WAS_RUNNING=0

if command -v systemctl >/dev/null 2>&1; then
    systemctl is-active --quiet nfdaemon
    if [ $? -eq 0 ]; then
        NFDAEMON_WAS_RUNNING=1
    fi
fi
$ECHO "Checking required files..."
if [ -d /opt/MicroWorld/.netfilter_kmodule ]; then
	$MV /opt/MicroWorld/.netfilter_kmodule /opt/MicroWorld/netfilter_kmodule
	RECOMPILE=1
fi
checkFileExist /opt/MicroWorld/netfilter_kmodule/Makefile
checkFileExist /opt/MicroWorld/netfilter_kmodule/source/escanpfmodule.c

ISGCC=1
ISMAKE=1
ISKERNELHDR=1
ISOTHERKERNELHDR=1
COMPILATIONDEP=1
if [ "x$GCC" = "x" ]; then
	${ECHO} "Checking gcc....Not Installed."
	ISGCC=0
	COMPILATIONDEP=0
else
	if [ -f ${GCC} ]; then
		${ECHO} "Checking gcc....Installed."
	else
		${ECHO} "Checking gcc....Not Installed."
		ISGCC=0
		COMPILATIONDEP=0
	fi
fi
#checkFileExist $GCC

if [ "x$MAKE" = "x" ]; then
	${ECHO} "Checking make....Not Installed."
	ISMAKE=0
	COMPILATIONDEP=0
else
	if [ -f ${MAKE} ]; then
		${ECHO} "Checking make....Installed."
	else
		${ECHO} "Checking make....Not Installed."
		ISMAKE=0
		COMPILATIONDEP=0
	fi
fi
#checkFileExist $MAKE

checkKernalHeader
if [ $? -ne 0 ]; then
	if [ -f /usr/bin/dpkg ] || [ -f /bin/dpkg ]; then
		checkOtherKernalHeader
		if [ $? -ne 0 ]; then
			${ECHO} "Checking kernel headers....Not Installed."
			ISKERNELHDR=0
			COMPILATIONDEP=0
		else
			${ECHO} "Checking Other kernel headers....Installed."
		fi
	else
		${ECHO} "Checking kernel headers....Not Installed."
		ISOTHERKERNELHDR=0
                ISKERNELHDR=0
                COMPILATIONDEP=0
	fi
else
	ISOTHERKERNELHDR=0
	${ECHO} "Checking kernel headers....Installed."
	dpkg --configure -a
	gcc_major_version=$(cat /proc/version | grep -oP 'gcc.*?\(.*?\) \K\d+')
	echo "Kernel was built with GCC version: $gcc_major_version"
	# Step 2: Check if gcc-$gcc_major_version is installed
	if command -v gcc-"$gcc_major_version" >/dev/null 2>&1; then
    echo "gcc-$gcc_major_version is already installed."
	else
		echo "gcc-$gcc_major_version is NOT installed. Installing..."
    # Step 3: Install gcc-$gcc_major_version
    sudo apt update -y 
    sudo apt install -y gcc-"$gcc_major_version"
		if command -v gcc-"$gcc_major_version" >/dev/null 2>&1; then
        echo "gcc-$gcc_major_version successfully installed."
        PACKAGES_INSTALLED=1
		else
        echo " Failed to install gcc-$gcc_major_version."
		fi
	fi
fi

if [ ${ISOTHERKERNELHDR} -eq 0 ]; then
	KERNELVER=`uname -r`
else
	KERNELVER="$NKVER-$KARCH"
fi


if [ ${COMPILATIONDEP} -eq 0 ]; then
      #${ECHO} "Run following to install compilation tools :"
          #sudo apt-get update -y >/dev/null 2>&1
	
          if [ ${ISGCC} -eq 0 ]; then
               if [ -f /usr/bin/dpkg ] || [ -f /bin/dpkg ]; then
                    #${ECHO}  "  sudo apt-get install build-essential"
                    ${ECHO}   "  build-essential is installing..."
                    sudo apt-get install -y build-essential 
                    if [ $? -eq 0 ]; then
                        PACKAGES_INSTALLED=1
                    fi
               fi
               if [ -f /usr/bin/rpm ] || [ -f /bin/rpm ]; then
                    ${ECHO} "  build-essential gcc is installing..."
                    if command -v dnf >/dev/null 2>&1; then
                         sudo dnf install -y gcc make
                    else
                         sudo yum install -y gcc make
                    fi
                    if [ $? -eq 0 ]; then
                         PACKAGES_INSTALLED=1
                    fi
               fi
          fi
          if [ ${ISMAKE} -eq 0 ]; then
               #if [ -f /usr/bin/dpkg ] || [ -f /bin/dpkg ]; then
                                #${ECHO} "  sudo apt-get install build-essential"
                        #fi
                        if [ -f /usr/bin/rpm ] || [ -f /bin/rpm ]; then
                                ${ECHO} "  automake make is installing..."
                                if command -v dnf >/dev/null 2>&1; then
                                     sudo dnf install -y make automake
                                else
                                     sudo yum install -y make automake
                                fi
                                if [ $? -eq 0 ]; then
                                     PACKAGES_INSTALLED=1
                                fi
                        fi
          fi

          if [ ${ISKERNELHDR} -eq 0  ]; then
                        if [ -f /usr/bin/dpkg ] || [ -f /bin/dpkg ]; then
                                $ECHO "  sudo apt-get -y install linux-headers-${KERNELVER}"
                                #sudo apt-get install linux-headers-${KERNELVER}
                        fi
                        if [ -f /usr/bin/rpm ] || [ -f /bin/rpm ]; then
                                $ECHO "  kernel-devel is installing for ${KERNELVER}..."
                                if command -v dnf >/dev/null 2>&1; then
                                     sudo dnf install -y kernel-devel-${KERNELVER}
                                     if [ $? -ne 0 ]; then
                                          sudo dnf install -y kernel-devel
                                     fi
                                else
                                     sudo yum install -y kernel-devel-${KERNELVER}
                                     if [ $? -ne 0 ]; then
                                          sudo yum install -y kernel-devel
                                     fi
                                fi
                                if [ $? -eq 0 ]; then
                                     PACKAGES_INSTALLED=1
                                fi
                        fi
	fi
	$MV /opt/MicroWorld/netfilter_kmodule /opt/MicroWorld/.netfilter_kmodule

	echo "Compilation dependencies were installed."
	if [ $PACKAGES_INSTALLED -eq 1 ] && [ $NFDAEMON_WAS_RUNNING -eq 1 ]; then
		echo "Restarting nfdaemon because dependencies were installed."
		systemctl restart nfdaemon
	fi
	exit 0;
fi

$ECHO "Preparing for compilation..."





FILENAME="/lib/modules/${KERNELVER}/build/include/linux/netfilter_ipv4.h"
FILENAME2="/lib/modules/${KERNELVER}/build/include/uapi/linux/netfilter_ipv4.h"
if [ -f $FILENAME ] || [ -f $FILENAME2 ]; then
	commentLimitsHeader $FILENAME $FILENAME2
fi





checkKernalHeaderAlt
FILENAME="/lib/modules/${KERNELVER}/source/include/linux/netfilter_ipv4.h"
FILENAME2="/lib/modules/${KERNELVER}/source/include/uapi/linux/netfilter_ipv4.h"
if [ -f $FILENAME ] || [ -f $FILENAME2 ]; then
	commentLimitsHeader $FILENAME $FILENAME2
fi

$ECHO "Starting compilation... "
$MAKE -C /opt/MicroWorld/netfilter_kmodule/ clean
$MAKE -C /opt/MicroWorld/netfilter_kmodule/ NKERVER="$KERNELVER">>/var/MicroWorld/var/log/kmod.log 2>&1
if [ $? -ne 0 ]; then
	$ECHO "Please wait ....  "
	apt-get -y install --reinstall linux-headers-$(uname -r)
	$MAKE -C /opt/MicroWorld/netfilter_kmodule/ clean
	$MAKE -C /opt/MicroWorld/netfilter_kmodule/ NKERVER="$KERNELVER">>/var/MicroWorld/var/log/kmod.log 2>&1
	if [ $? -ne 0 ]; then
		$ECHO ""
		$ECHO "Failed to compile webfilter kmodule."
		${ECHO} "Make sure that Kernel-headers for ${KERNELVER}"
		echo "Compilation failed. nfdaemon will not be restarted."
		exit 5;
	fi
fi

checkFileExist /opt/MicroWorld/netfilter_kmodule/escanpfmodule.ko
$ECHO "Copying compiled files..."

if [ -f /usr/bin/chattr ];then
	CHATTR="/usr/bin/chattr"	
elif [ -f /bin/chattr ];then
	CHATTR="/bin/chattr"	
elif [ -f /usr/local/bin/chattr ];then
	CHATTR="/usr/local/bin/chattr"
fi	

if [ ! -w /opt/MicroWorld/sbin/escanpfmodule.ko ];then
 	$CHATTR -i /opt/MicroWorld/sbin/escanpfmodule.ko
	SETIMMUTABLE=1
fi
$CP -f /opt/MicroWorld/netfilter_kmodule/escanpfmodule.ko /opt/MicroWorld/sbin/escanpfmodule.ko
if [ $? -ne 0 ]; then
	$ECHO ""
	$ECHO "Failed to copy file : escanpfmodule.ko"
	$ECHO ""
	$MV /opt/MicroWorld/netfilter_kmodule /opt/MicroWorld/.netfilter_kmodule
	exit 6;
fi

if [ $SETIMMUTABLE -eq 1 ];then
	$CHATTR +i /opt/MicroWorld/sbin/escanpfmodule.ko
fi

$ECHO "Cleaning up unwanted stuff..."

FILENAME="/lib/modules/${KERNELVER}/build/include/linux/netfilter_ipv4.h"
FILENAME2="/lib/modules/${KERNELVER}/build/include/uapi/linux/netfilter_ipv4.h"
if [ -f $FILENAME ] || [ -f $FILENAME2 ]; then
	restoreLimitsHeader $FILENAME $FILENAME2
fi

checkKernalHeaderAlt
if [ $? -eq 0 ]; then
	FILENAME="/lib/modules/${KERNELVER}/source/include/linux/netfilter_ipv4.h"
	FILENAME2="/lib/modules/${KERNELVER}/source/include/uapi/linux/netfilter_ipv4.h"
	restoreLimitsHeader $FILENAME $FILENAME2
fi

if [ -d /opt/MicroWorld/netfilter_kmodule ]; then
	$MV /opt/MicroWorld/netfilter_kmodule /opt/MicroWorld/.netfilter_kmodule
fi

prepareToRedownloadPolicy

if [ ${RECOMPILE} -eq 1 ] ; then
	${ECHO} "Restart winclient..."
     systemctl restart winclient

	if [ $NFDAEMON_WAS_RUNNING -eq 1 ]; then
		${ECHO} "Restarting nfdaemon..."
		systemctl restart nfdaemon
	else
		${ECHO} "nfdaemon was not running before execution. No restart required."
	fi
fi

$ECHO "Kernel module is compiled successfully"
exit 0
