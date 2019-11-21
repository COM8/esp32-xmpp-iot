#pragma once

//---------------------------------------------------------------------------
namespace espiot::xmpp {
//---------------------------------------------------------------------------
template <typename EventType>
class INonConstEventListener {
    public:
    virtual ~INonConstEventListener() = default;

    virtual void event(EventType& event) = 0;
};
//---------------------------------------------------------------------------
} // namespace espiot::xmpp
//---------------------------------------------------------------------------
