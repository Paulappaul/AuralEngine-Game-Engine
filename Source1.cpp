#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <iostream>

#define AK_BANK_LOAD_BLOCKING (1 << 0)

int main()
{
    // Initialize the audio engine
    AkMemSettings memSettings;
    AK::MemoryMgr::Init(&memSettings);

    AkInitSettings initSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);

    AkPlatformInitSettings platSettings;
    AK::SoundEngine::GetDefaultPlatformInitSettings(platSettings);

    AK::SoundEngine::Init(&initSettings, &platSettings);

    // Load the SoundBank
    AkBankID bankID;
    AKRESULT result = AK::SoundEngine::LoadBank("RayCaster.bnk", bankID);
    if (result != AK_Success) {
        // handle error
        std::cout << "work" << std::endl;
        return 1;
    }

    std::string x;
    std::cin >> x;
    std::cout << x;

    // Post the event "Themesong"
    AK::SoundEngine::PostEvent("ThemeSong", AK_INVALID_GAME_OBJECT);

    // Do other things in your application...

    std::cout << "work2" << std::endl;


    // Terminate the audio engine
    AK::SoundEngine::Term();
    AK::MemoryMgr::Term();

    return 0;
}