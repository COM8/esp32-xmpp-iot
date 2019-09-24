#pragma once

#include <string>

//---------------------------------------------------------------------------
namespace espiot::esp::bt {
//---------------------------------------------------------------------------
class BLEUnlockHelper {
    private:
    bool locked;

    public:
    bool isLocked();

    bool onChallengeAnswer(std::string& answer);
    std::string getChallenge();
};

//---------------------------------------------------------------------------
} // namespace espiot::esp::bt
//---------------------------------------------------------------------------