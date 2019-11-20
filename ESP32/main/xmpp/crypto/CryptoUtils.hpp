#pragma once

#include <string>

//---------------------------------------------------------------------------
namespace espiot::xmpp::crypto {
//---------------------------------------------------------------------------
std::string toBase64(const std::string& in);

std::string fromBase64(const std::string& in);

void testBase64();
void testBase64(std::string refIn, std::string refOut);
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::crypto
//---------------------------------------------------------------------------