#pragma once

#include <string>

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
struct Jid {
    const std::string userPart;
    const std::string domainPart;
    const std::string resourcePart;

    Jid(const std::string&& userPart, const std::string&& domainPart, const std::string&& resourcePart) : userPart(userPart),
                                                                                                          domainPart(domainPart),
                                                                                                          resourcePart(resourcePart) {}
    Jid(const std::string&& jid) {
        // TODO split up JID
    }
    std::string getBare() {
        return userPart + "@" + domainPart;
    }

    std::string getFull() {
        return getBare() + "/" + resourcePart;
    }
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------
