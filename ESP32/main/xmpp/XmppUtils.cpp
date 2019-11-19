#include "XmppUtils.hpp"
#include <codecvt>
#include <locale>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
std::wstring wstring_convert_from_char(const char* str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.from_bytes(str);
}

std::string string_convert_from_wchar(const wchar_t* str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(str);
}

std::wstring wstring_convert_from_bytes(const std::vector<uint8_t>& v) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    const std::vector<char> tmp(v.begin(), v.end());
    return wstring_convert_from_bytes(tmp);
}

std::wstring wstring_convert_from_bytes(const std::vector<char>& v) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.from_bytes(v.data(), v.data() + v.size());
}

std::vector<uint8_t> wstring_convert_to_bytes(const wchar_t* str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string string = converter.to_bytes(str);
    return std::vector<uint8_t>(string.begin(), string.end());
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------