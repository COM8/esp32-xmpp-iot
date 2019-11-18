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
    std::cout << "CALL get_write_pos" << std::endl;
    return packet.get_data_vec().data();
}

bool XmppProtocol::is_complete(XmppPacket& packet) const {
    std::cout << "CALL is_complete" << std::endl;
    return true;
}

bool XmppProtocol::is_error() {
    std::cout << "CALL is_error" << std::endl;
    return false;
}

void XmppProtocol::packet_consumed() {
    std::cout << "CALL packet_consumed" << std::endl;
}

void XmppProtocol::reset() {
    std::cout << "CALL reset" << std::endl;
}
//---------------------------------------------------------------------------
} // namespace espiot::xmpp::tcp
//---------------------------------------------------------------------------
