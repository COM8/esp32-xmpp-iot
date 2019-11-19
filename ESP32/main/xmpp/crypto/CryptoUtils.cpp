#include "CryptoUtils.hpp"
#include "mbedtls/base64.h"

//---------------------------------------------------------------------------
namespace espiot::xmpp::crypto {
//---------------------------------------------------------------------------
std::string toBase64(const std::string& in) {
    std::size_t olen = 0;
    const unsigned char* inStr = reinterpret_cast<const unsigned char*>(in.c_str());
    // Get the size of the output stored in olen:
    mbedtls_base64_encode(nullptr, 0, &olen, inStr, in.length());

    // Do the actual conversion:
    unsigned char* buffer = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * olen));
    mbedtls_base64_encode(buffer, olen, &olen, inStr, in.length());

    std::string out(reinterpret_cast<char*>(buffer));
    free(buffer);
    return out;
}

std::string fromBase64(const std::string& in) {
    return "";
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::crypto
//---------------------------------------------------------------------------