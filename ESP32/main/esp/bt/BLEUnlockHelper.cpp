#include "BLEUnlockHelper.hpp"
#include <iostream>

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
bool BLEUnlockHelper::onChallengeAnswer(std::string& answer) {
    std::cout << "Received challenge response: " << answer << "\n";
    return !(locked = answer != "ready");
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