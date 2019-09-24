#pragma once

#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
class BluetoothServerCallback {
    public:
    virtual ~BluetoothServerCallback() = default;

    virtual void onConfigurationDone(std::string& wifiSsid, std::string& wifiPassword, std::string& jid, std::string& jidPassword) = 0;
};
//---------------------------------------------------------------------------
} // namespace espiot::esp::bt
//---------------------------------------------------------------------------