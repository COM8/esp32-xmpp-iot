#pragma once

#include <string>
#include <vector>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
/**
 * Based on: https://stackoverflow.com/questions/34113871/c-convert-between-byte-vector-and-wstring
 **/
std::wstring wstring_convert_from_char(const char* str);

/**
 * Based on: https://stackoverflow.com/questions/34113871/c-convert-between-byte-vector-and-wstring
 **/
std::string string_convert_from_wchar(const wchar_t* str);

/**
 * Based on: https://stackoverflow.com/questions/34113871/c-convert-between-byte-vector-and-wstring
 **/
std::wstring wstring_convert_from_bytes(const std::vector<uint8_t>& v);

/**
 * Based on: https://stackoverflow.com/questions/34113871/c-convert-between-byte-vector-and-wstring
 **/
std::wstring wstring_convert_from_bytes(const std::vector<char>& v);

/**
 * Based on: https://stackoverflow.com/questions/34113871/c-convert-between-byte-vector-and-wstring
 **/
std::vector<uint8_t> wstring_convert_to_bytes(const wchar_t* str);
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------