#include "XmppProtocol.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::xmpp::tcp {
//---------------------------------------------------------------------------
int XmppProtocol::get_wanted_amount(XmppPacket& packet) {
    return static_cast<int>(packet.size());
}

void XmppProtocol::data_received(XmppPacket& packet, int length) {
    std::cout << "Data received: " << packet.to_string() << std::endl;
}

uint8_t* XmppProtocol::get_write_pos(XmppPacket& packet) {
    return packet.get_data_vec().data();
}

bool XmppProtocol::is_complete(XmppPacket& packet) const {
    return true;
}

bool XmppProtocol::is_error() {
    return false;
}

void XmppProtocol::packet_consumed() {
}

void XmppProtocol::reset() {
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------
