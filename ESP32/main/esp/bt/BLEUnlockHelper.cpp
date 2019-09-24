#include "BLEUnlockHelper.hpp"

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
bool BLEUnlockHelper::onChallengeAnswer(std::string& answer) {
    return (locked = answer == "ready");
}

std::string BLEUnlockHelper::getChallenge() {
    return "challenge";
}

bool BLEUnlockHelper::isLocked() {
    return locked;
}
//---------------------------------------------------------------------------
} // namespace espiot::esp::bt
//---------------------------------------------------------------------------