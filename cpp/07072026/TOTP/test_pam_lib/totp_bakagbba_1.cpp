// totp_bakagbba.cpp
//
// Standalone TOTP generator that reproduces exactly what
// pam_google_authenticator.so computes for secret = "BAKAGBBAFAMAGANB".
//
// Implements SHA-1 (RFC 3174), HMAC-SHA1 (RFC 2104), base32 decode,
// and HOTP/TOTP (RFC 4226 / RFC 6238) from scratch. No external deps.
//
// Compile:
//   g++ -std=c++17 -O2 -Wall -o totp_bakagbba totp_bakagbba.cpp
//
// Run:
//   ./totp_bakagbba                # current 30s window
//   ./totp_bakagbba 3              # current + next 3 windows
//   ./totp_bakagbba --at 1700000000   # specific unix time
//
// The output format matches google-authenticator's: %06d.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// SHA-1 (RFC 3174)
// ---------------------------------------------------------------------------
namespace sha1 {

struct Ctx {
    uint32_t h[5];
    uint64_t len;              // total message length in bytes
    uint8_t  buf[64];
    size_t   buflen;
};

static inline uint32_t rol(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

static void init(Ctx& c) {
    c.h[0] = 0x67452301u;
    c.h[1] = 0xEFCDAB89u;
    c.h[2] = 0x98BADCFEu;
    c.h[3] = 0x10325476u;
    c.h[4] = 0xC3D2E1F0u;
    c.len = 0;
    c.buflen = 0;
}

static void processBlock(Ctx& c, const uint8_t* p) {
    uint32_t w[80];
    for (int i = 0; i < 16; ++i) {
        w[i] = (uint32_t(p[i*4]) << 24) |
               (uint32_t(p[i*4+1]) << 16) |
               (uint32_t(p[i*4+2]) << 8)  |
               (uint32_t(p[i*4+3]));
    }
    for (int i = 16; i < 80; ++i) {
        w[i] = rol(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
    }

    uint32_t a = c.h[0], b = c.h[1], cc = c.h[2], d = c.h[3], e = c.h[4];
    for (int i = 0; i < 80; ++i) {
        uint32_t f, k;
        if (i < 20)      { f = (b & cc) | ((~b) & d);      k = 0x5A827999u; }
        else if (i < 40) { f = b ^ cc ^ d;                 k = 0x6ED9EBA1u; }
        else if (i < 60) { f = (b & cc) | (b & d) | (cc & d); k = 0x8F1BBCDCu; }
        else             { f = b ^ cc ^ d;                 k = 0xCA62C1D6u; }

        uint32_t tmp = rol(a, 5) + f + e + k + w[i];
        e = d;
        d = cc;
        cc = rol(b, 30);
        b = a;
        a = tmp;
    }
    c.h[0] += a; c.h[1] += b; c.h[2] += cc; c.h[3] += d; c.h[4] += e;
}

static void update(Ctx& c, const uint8_t* data, size_t n) {
    c.len += n;
    while (n > 0) {
        size_t take = 64 - c.buflen;
        if (take > n) take = n;
        std::memcpy(c.buf + c.buflen, data, take);
        c.buflen += take;
        data += take;
        n    -= take;
        if (c.buflen == 64) {
            processBlock(c, c.buf);
            c.buflen = 0;
        }
    }
}

static void final(Ctx& c, uint8_t out[20]) {
    uint64_t bitlen = c.len * 8;

    // Append 0x80
    uint8_t pad = 0x80;
    update(c, &pad, 1);

    // Pad with zeros until length ≡ 56 (mod 64)
    uint8_t zero = 0;
    while (c.buflen != 56) {
        update(c, &zero, 1);
    }

    // Append 64-bit big-endian bit length
    uint8_t lenbuf[8];
    for (int i = 0; i < 8; ++i) {
        lenbuf[7 - i] = uint8_t(bitlen >> (i * 8));
    }
    update(c, lenbuf, 8);

    // Output h[0..4] big-endian
    for (int i = 0; i < 5; ++i) {
        out[i*4]   = uint8_t(c.h[i] >> 24);
        out[i*4+1] = uint8_t(c.h[i] >> 16);
        out[i*4+2] = uint8_t(c.h[i] >> 8);
        out[i*4+3] = uint8_t(c.h[i]);
    }
}

static void hash(const uint8_t* data, size_t n, uint8_t out[20]) {
    Ctx c;
    init(c);
    update(c, data, n);
    final(c, out);
}

} // namespace sha1

// ---------------------------------------------------------------------------
// HMAC-SHA1 (RFC 2104)
// ---------------------------------------------------------------------------
static void hmac_sha1(const uint8_t* key, size_t keylen,
                      const uint8_t* msg, size_t msglen,
                      uint8_t out[20])
{
    const size_t BLOCK = 64;

    uint8_t k[BLOCK];
    std::memset(k, 0, BLOCK);

    if (keylen > BLOCK) {
        // Hash the key first if it's too long
        sha1::hash(key, keylen, k);
    } else {
        std::memcpy(k, key, keylen);
    }

    uint8_t ipad[BLOCK], opad[BLOCK];
    for (size_t i = 0; i < BLOCK; ++i) {
        ipad[i] = k[i] ^ 0x36;
        opad[i] = k[i] ^ 0x5C;
    }

    // inner = SHA1(ipad || msg)
    uint8_t inner[20];
    {
        sha1::Ctx c;
        sha1::init(c);
        sha1::update(c, ipad, BLOCK);
        sha1::update(c, msg, msglen);
        sha1::final(c, inner);
    }

    // outer = SHA1(opad || inner)
    {
        sha1::Ctx c;
        sha1::init(c);
        sha1::update(c, opad, BLOCK);
        sha1::update(c, inner, 20);
        sha1::final(c, out);
    }
}

// ---------------------------------------------------------------------------
// Base32 decoder (RFC 4648, uppercase A-Z and 2-7, no padding required)
// ---------------------------------------------------------------------------
static bool base32_decode(const std::string& in, std::vector<uint8_t>& out)
{
    static const char* ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    uint32_t buffer = 0;
    int bits = 0;
    out.clear();

    for (char ch : in) {
        if (ch == '=' || ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t') {
            continue;             // ignore padding / whitespace
        }
        if (ch >= 'a' && ch <= 'z') ch = char(ch - 'a' + 'A');  // normalize

        const char* p = std::strchr(ALPHABET, ch);
        if (!p) return false;
        int val = int(p - ALPHABET);

        buffer = (buffer << 5) | uint32_t(val);
        bits += 5;

        if (bits >= 8) {
            bits -= 8;
            out.push_back(uint8_t((buffer >> bits) & 0xFF));
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// RFC 4226 / 6238 — HOTP / TOTP
// ---------------------------------------------------------------------------

// Exact port of google-authenticator's generateCode().
static int generateCode(const std::vector<uint8_t>& secret,
                        unsigned long long tm)
{
    if (secret.empty()) return -1;

    uint8_t challenge[8];
    for (int i = 8; i--; tm >>= 8) {
        challenge[i] = uint8_t(tm);
    }

    uint8_t hash[20];
    hmac_sha1(secret.data(), secret.size(),
              challenge, 8,
              hash);

    const int offset = hash[19] & 0xF;

    uint32_t truncatedHash = 0;
    for (int i = 0; i < 4; ++i) {
        truncatedHash <<= 8;
        truncatedHash  |= hash[offset + i];
    }

    truncatedHash &= 0x7FFFFFFF;
    truncatedHash %= 1000000;      // VERIFICATION_CODE_MODULUS

    return int(truncatedHash);
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
static void usage(const char* argv0) {
    std::fprintf(stderr,
        "Usage: %s [--at <unix-time>] [<extra-windows>]\n"
        "  --at <t>          Use unix time <t> instead of now.\n"
        "  <extra-windows>   Also print codes for the next N 30s windows.\n"
        "\n"
        "Secret is hardcoded: BAKAGBBAFAMAGANB\n",
        argv0);
}

int main(int argc, char** argv)
{
    const char* SECRET_B32 = "BAKAGBBAFAMAGANB";

/* // not work this logic
//Extending to any secret

//If you want to use this with a secret passed on the command line instead of the hardcoded one, change main():	
	
	const char* SECRET_B32 = (argc > 1 && argv[1][0] != '-') ? argv[1] : "BAKAGBBAFAMAGANB";
*/

    const int   STEP       = 30;          // default TOTP step size

    long long atTime      = -1;
    int       extraWins   = 0;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--at") == 0 && i + 1 < argc) {
            atTime = std::strtoll(argv[++i], nullptr, 10);
        } else if (std::strcmp(argv[i], "-h") == 0 ||
                   std::strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            extraWins = std::atoi(argv[i]);
            if (extraWins < 0) extraWins = 0;
        }
    }

    // Decode the base32 secret to raw bytes — this is the HMAC key.
    std::vector<uint8_t> key;
    if (!base32_decode(SECRET_B32, key) || key.empty()) {
        std::fprintf(stderr, "Failed to base32-decode secret\n");
        return 1;
    }

    std::fprintf(stderr, "Secret (base32)  : %s\n", SECRET_B32);
    std::fprintf(stderr, "Secret (hex)     : ");
    for (uint8_t b : key) std::fprintf(stderr, "%02x", b);
    std::fprintf(stderr, "\nSecret (bytes)   : %zu\n", key.size());
    std::fprintf(stderr, "Step size        : %d\n", STEP);

    long long now = (atTime >= 0) ? atTime : (long long)std::time(nullptr);
    unsigned long long counter = (unsigned long long)now / STEP;

    for (int w = 0; w <= extraWins; ++w) {
        unsigned long long c = counter + (unsigned long long)w;
        int code = generateCode(key, c);
        std::printf("%06d    (counter=%llu, t=%lld, window=%d)\n",
                    code, c, (long long)c * STEP, w);
    }

    return 0;
}
