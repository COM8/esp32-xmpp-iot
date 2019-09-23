#pragma once

#include "nvs.h"
#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp {
//---------------------------------------------------------------------------
class Storage {
    public:
    static const std::string INITIALIZED;
    static const std::string WIFI_SSID;
    static const std::string WIFI_SSID_PASSWORD;
    static const std::string JID;
    static const std::string JID_PASSWORD;

    Storage() = default;
    void init();

    bool readBool(std::string& key);
    uint16_t readUInt16(std::string& key);
    uint8_t readUInt8(std::string& key);
    std::string readString(std::string& key);

    void writeBool(std::string& key, bool value);
    void writeUInt16(std::string& key, uint16_t value);
    void writeUInt8(std::string& key, uint8_t value);
    void writeString(std::string& key, std::string& value);

    private:
    bool ready;
    nvs_handle_t handle;
};

//---------------------------------------------------------------------------
} // namespace espiot::esp
//---------------------------------------------------------------------------