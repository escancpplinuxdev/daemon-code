// totp_bakagbba.cpp
//
// Standalone TOTP generator that reproduces what pam_google_authenticator.so
// computes for a given secret. Supports both hex and base32 input.
//
// Implements SHA-1 (RFC 3174), HMAC-SHA1 (RFC 2104), base32 decode (RFC 4648),
// hex decode, and HOTP/TOTP (RFC 4226 / 6238) from scratch. No external deps.
//
// Compile:
//   g++ -std=c++17 -O2 -Wall -o totp_bakagbba totp_bakagbba.cpp
//
// Run:
//   ./totp_bakagbba                                   # default secret, now
//   ./totp_bakagbba <secret>                          # auto-detect hex/base32
//   ./totp_bakagbba <secret> --hex                    # force hex
//   ./totp_bakagbba <secret> --base32                 # force base32
//   ./totp_bakagbba <secret> --at 1700000000          # specific unix time
//   ./totp_bakagbba <secret> 5                        # current + next 5 windows
//   ./totp_bakagbba <secret> --step 60                # custom step size

/*
 *
 # Default secret, current window
./totp_bakagbba

# Vendor secret, auto-detect encoding
./totp_bakagbba 6BFF6CF64E2AC2EC

# Force hex (what you probably want)
./totp_bakagbba 6BFF6CF64E2AC2EC --hex

# Force base32
./totp_bakagbba BAKAGBBAFAMAGANB --base32

# Next 5 windows
./totp_bakagbba 6BFF6CF64E2AC2EC --hex 5

# Specific time, custom step size
./totp_bakagbba 6BFF6CF64E2AC2EC --hex --step 60 --at 1791200070


./totp_bakagbba 6BFF6CF64E2AC2EC --hex
# Secret (hex): 6bff6cf64e2ac2ec      ← starts with 6b now
# Secret (bytes): 8

./totp_bakagbba 5BFF6CF64E2AC2EC --hex
# Secret (hex): 5bff6cf64e2ac2ec      ← starts with 5b now
# Secret (bytes): 8
 
 *
 * */


#include <cctype>
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
        w[i] = (uint32_t(p[i*4])     << 24) |
               (uint32_t(p[i*4 + 1]) << 16) |
               (uint32_t(p[i*4 + 2]) <<  8) |
               (uint32_t(p[i*4 + 3]));
    }
    for (int i = 16; i < 80; ++i) {
        w[i] = rol(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
    }

    uint32_t a = c.h[0], b = c.h[1], cc = c.h[2], d = c.h[3], e = c.h[4];
    for (int i = 0; i < 80; ++i) {
        uint32_t f, k;
        if (i < 20)      { f = (b & cc) | ((~b) & d);         k = 0x5A827999u; }
        else if (i < 40) { f = b ^ cc ^ d;                    k = 0x6ED9EBA1u; }
        else if (i < 60) { f = (b & cc) | (b & d) | (cc & d); k = 0x8F1BBCDCu; }
        else             { f = b ^ cc ^ d;                    k = 0xCA62C1D6u; }

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
        out[i*4]     = uint8_t(c.h[i] >> 24);
        out[i*4 + 1] = uint8_t(c.h[i] >> 16);
        out[i*4 + 2] = uint8_t(c.h[i] >>  8);
        out[i*4 + 3] = uint8_t(c.h[i]);
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
        sha1::hash(key, keylen, k);
    } else {
        std::memcpy(k, key, keylen);
    }

    uint8_t ipad[BLOCK], opad[BLOCK];
    for (size_t i = 0; i < BLOCK; ++i) {
        ipad[i] = k[i] ^ 0x36;
        opad[i] = k[i] ^ 0x5C;
    }

    uint8_t inner[20];
    {
        sha1::Ctx c;
        sha1::init(c);
        sha1::update(c, ipad, BLOCK);
        sha1::update(c, msg, msglen);
        sha1::final(c, inner);
    }

    {
        sha1::Ctx c;
        sha1::init(c);
        sha1::update(c, opad, BLOCK);
        sha1::update(c, inner, 20);
        sha1::final(c, out);
    }
}

// ---------------------------------------------------------------------------
// Base32 decoder (RFC 4648: A-Z, 2-7; case-insensitive; ignores '=' and spaces)
// ---------------------------------------------------------------------------
static bool base32_decode(const std::string& in, std::vector<uint8_t>& out)
{
    static const char* ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    uint32_t buffer = 0;
    int bits = 0;
    out.clear();

    for (char ch : in) {
        if (ch == '=' || ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t') {
            continue;
        }
        if (ch >= 'a' && ch <= 'z') ch = char(ch - 'a' + 'A');

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
// Hex decoder. Ignores whitespace, colons, dashes, optional 0x prefix.
// ---------------------------------------------------------------------------
static bool hex_decode(const std::string& in, std::vector<uint8_t>& out)
{
    out.clear();
    std::string s;
    for (char ch : in) {
        if (std::isxdigit((unsigned char)ch)) s.push_back(ch);
    }
    if (s.empty() || s.size() % 2 != 0) return false;

    auto nib = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };

    for (size_t i = 0; i < s.size(); i += 2) {
        int hi = nib(s[i]);
        int lo = nib(s[i + 1]);
        if (hi < 0 || lo < 0) return false;
        out.push_back(uint8_t((hi << 4) | lo));
    }
    return true;
}

// Heuristic: does this string look like pure hex?
// Rules: even length, ≥ 8 chars, only hex digits.
// Note "BAKAGBBAFAMAGANB" contains 'K','G','N', so it's not hex → base32.
static bool looks_like_hex(const std::string& s) {
    if (s.size() < 8 || s.size() % 2 != 0) return false;
    for (char c : s) {
        if (!std::isxdigit((unsigned char)c)) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// RFC 4226 / 6238 — HOTP / TOTP
// Exact port of google-authenticator's generateCode().
// ---------------------------------------------------------------------------
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
        "Usage: %s [<secret>] [options] [extra-windows]\n"
        "\n"
        "  <secret>          Secret in hex or base32 (default: BAKAGBBAFAMAGANB).\n"
        "                    Auto-detected unless --hex or --base32 is given.\n"
        "  --hex             Force hex decoding of <secret>.\n"
        "  --base32          Force base32 decoding of <secret>.\n"
        "  --at <unix-time>  Use unix time <t> instead of now.\n"
        "  --step <seconds>  TOTP step size (default 30).\n"
        "  <extra-windows>   Also print codes for the next N windows.\n"
        "  -h, --help        Show this message.\n"
        "\n"
        "Examples:\n"
        "  %s 6BFF6CF64E2AC2EC --hex\n"
        "  %s BAKAGBBAFAMAGANB --base32\n"
        "  %s 5BFF6CF64E2AC2EC --at 1700000000\n"
        "  %s 6BFF6CF64E2AC2EC 5\n",
        argv0, argv0, argv0, argv0, argv0);
}

int main(int argc, char** argv)
{
    const char* argSecret = nullptr;
    bool forceHex         = false;
    bool forceBase32      = false;
    long long atTime      = -1;
    int  extraWins        = 0;
    int  stepSize         = 30;

    // First non-flag argument is the secret.
    int i = 1;
    if (i < argc && argv[i][0] != '-') {
        argSecret = argv[i++];
    }

    for (; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 ||
            std::strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else if (std::strcmp(argv[i], "--hex") == 0) {
            forceHex = true;
        } else if (std::strcmp(argv[i], "--base32") == 0) {
            forceBase32 = true;
        } else if (std::strcmp(argv[i], "--at") == 0 && i + 1 < argc) {
            atTime = std::strtoll(argv[++i], nullptr, 10);
        } else if (std::strcmp(argv[i], "--step") == 0 && i + 1 < argc) {
            stepSize = std::atoi(argv[++i]);
            if (stepSize <= 0) stepSize = 30;
        } else {
            extraWins = std::atoi(argv[i]);
            if (extraWins < 0) extraWins = 0;
        }
    }

    if (forceHex && forceBase32) {
        std::fprintf(stderr, "Error: --hex and --base32 are mutually exclusive.\n");
        return 1;
    }

    std::string input = argSecret ? argSecret : "BAKAGBBAFAMAGANB";

    // Decode according to mode / heuristic.
    std::vector<uint8_t> key;
    std::string encoding;

    if (forceHex) {
        if (!hex_decode(input, key) || key.empty()) {
            std::fprintf(stderr, "Error: invalid hex secret \"%s\"\n", input.c_str());
            return 1;
        }
        encoding = "hex (forced)";
    } else if (forceBase32) {
        if (!base32_decode(input, key) || key.empty()) {
            std::fprintf(stderr, "Error: invalid base32 secret \"%s\"\n", input.c_str());
            return 1;
        }
        encoding = "base32 (forced)";
    } else if (looks_like_hex(input)) {
        if (!hex_decode(input, key) || key.empty()) {
            std::fprintf(stderr, "Error: invalid hex secret \"%s\"\n", input.c_str());
            return 1;
        }
        encoding = "hex (auto)";
    } else {
        if (!base32_decode(input, key) || key.empty()) {
            std::fprintf(stderr, "Error: invalid base32 secret \"%s\"\n", input.c_str());
            return 1;
        }
        encoding = "base32 (auto)";
    }

    std::fprintf(stderr, "Secret (input)   : %s\n", input.c_str());
    std::fprintf(stderr, "Encoding         : %s\n", encoding.c_str());
    std::fprintf(stderr, "Secret (hex)     : ");
    for (uint8_t b : key) std::fprintf(stderr, "%02x", b);
    std::fprintf(stderr, "\nSecret (bytes)   : %zu\n", key.size());
    std::fprintf(stderr, "Step size        : %d\n", stepSize);

    long long now = (atTime >= 0) ? atTime : (long long)std::time(nullptr);
    unsigned long long counter = (unsigned long long)now / (unsigned long long)stepSize;

    for (int w = 0; w <= extraWins; ++w) {
        unsigned long long c = counter + (unsigned long long)w;
        int code = generateCode(key, c);
        std::printf("%06d    (counter=%llu, t=%lld, window=%d)\n",
                    code, c, (long long)c * stepSize, w);
    }
    return 0;
}
