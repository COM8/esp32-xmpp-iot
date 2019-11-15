#pragma once

#include "XmppPacket.hpp"
#include "smooth/core/network/IPacketAssembly.h"

//---------------------------------------------------------------------------
namespace espiot::xmpp::tcp {
//---------------------------------------------------------------------------

class XmppProtocol
    : public smooth::core::network::IPacketAssembly<XmppProtocol, XmppPacket> {
    public:
    using packet_type = XmppPacket;

    XmppProtocol() = default;

    int get_wanted_amount(XmppPacket& packet) override;

    void data_received(XmppPacket& packet, int length) override;

    uint8_t* get_write_pos(XmppPacket& packet) override;

    bool is_complete(XmppPacket& packet) const override;

    bool is_error() override;

    void packet_consumed() override;

    void reset() override;
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------
