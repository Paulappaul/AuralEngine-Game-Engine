#pragma once
#include "Directory.h"

AkTransform listenerTransform;
AkVector listenerPosition = { playerX, playerY, z };

const AkGameObjectID MY_DEFAULT_LISTENER = 0;
const AkGameObjectID MY_EMITTER = 1;


const AkGameObjectID GAME_OBJECT_ID_THEME   =  100;
const AkGameObjectID GAME_OBJECT_ID_THEME2  =  200;
const AkGameObjectID GAME_OBJECT_ID_THEME3  =  300;
const AkGameObjectID GAME_OBJECT_ID_THEME4  =  400;
const AkGameObjectID GAME_OBJECT_ID_THEME5  =  500;
const AkGameObjectID GAME_OBJECT_ID_THEME6  =  600;
const AkGameObjectID GAME_OBJECT_ID_THEME7  =  700;
const AkGameObjectID GAME_OBJECT_ID_THEME8  =  800;
const AkGameObjectID GAME_OBJECT_ID_THEME9  =  900;
const AkGameObjectID GAME_OBJECT_ID_THEME10 = 1000;


CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

// listern variable
float emitterX = 12*64 / 32.0f;
float emitterY = 2*64 / 32.0f;

bool InitSoundEngine()
{

    AkMemSettings memSettings;
    AK::MemoryMgr::GetDefaultSettings(memSettings);

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        std::cout << (!"Could not create the memory manager.") << std::endl;
        return false;
    }
    else { std::cout << "Memory Manager Created" << std::endl; }



    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    // Customize the Stream Manager settings here.

    if (!AK::StreamMgr::Create(stmSettings))
    {
        std::cout << "Could not create the Streaming Manager" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Streaming Manager Created" << std::endl;
    }

    //
    // Create a streaming device with blocking low-level I/O handshaking.
    // Note that you can override the default low-level I/O module with your own. 
    //
    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    // Customize the streaming device settings here.

    // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
    // in the Stream Manager, and registers itself as the File Location Resolver.
    if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
    {
        std::cout << "Could not create the streaming device and Low-Level I/O system" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Device initalized" << std::endl;
    }



    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) == AK_Success)
    {
        std::cout << "Sound Engine Intialized" << std::endl;
    }
    else { std::cout << "Sound engine fail" << std::endl; }

    g_lowLevelIO.SetBasePath(AKTEXT("C:\\Users\\alcin\\Documents\\WwiseProjects\\AuralEngine\\GeneratedSoundBanks\\Windows"));
    AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

    AkBankID bankID;


    if (AK::SoundEngine::LoadBank(BANKNAME_INIT, bankID) != AK_Success)
    {

        std::cout << "ERROR: Could not load Bank";

    }
    else
    {
        std::cout << "init bank loaded" << std::endl;
    }

    if (AK::SoundEngine::LoadBank(BANKNAME_AURALENGINE, bankID) != AK_Success)
    {

        std::cout << "ERROR: COULD NOT LOAD BANK";
    }
    else
    {
        std::cout << "RayCast bank loaded" << std::endl;
    }

#ifndef AK_OPTIMIZED
    //
    // Initialize communications (not in release build!)
    //
    AkCommSettings commSettings;
    AK::Comm::GetDefaultInitSettings(commSettings);
    if (AK::Comm::Init(commSettings) != AK_Success)
    {
        std::cout << "Could not initialize communication." << std::endl;
        return false;
    }
    else
    {
        std::cout << "communication intialized" << std::endl;
    }
#endif // AK_OPTIMIZED




    //  
    //
    //
    // spatial audio 
    AkSpatialAudioInitSettings SpacSettings;
    if (AK::SpatialAudio::Init(SpacSettings) != AK_Success)
    {
        std::cout << "Could not intialize Spatial Audio" << std::endl;
    }
    else
    {
        std::cout << "Spatial Audio Intialzied" << std::endl;
    }
    //end Spatial Audio 
    // 
    // 
    // 




    return true;


}


void Setup()
{

    if (InitSoundEngine() == true)
    {
        std::cout << " intialization complete" << std::endl;
    }
    else { std::cout << "intialization failed" << std::endl; }


    //listern implementation:
    // 
    // Register the main listener.
    AK::SoundEngine::RegisterGameObj(MY_DEFAULT_LISTENER, "My Default Listener");

    // Set one listener as the default.
    AK::SoundEngine::SetDefaultListeners(&MY_DEFAULT_LISTENER, 1);

    std::cout << "Player position: " << playerX << ", " << playerY << ", " << z << std::endl;

    //listener position

    listenerTransform.SetPosition(listenerPosition);

    //listener orientation
    AkVector front = { 0.0f, 0.0f, -1.0f }; // Set front vector to point towards negative z-axis
    AkVector up = { 0.0f, 1.0f, 0.0f }; // Set up vector to point towards positive y-axis
    listenerTransform.SetOrientation(front, up);

    //listerner error checking
    if (AK::SoundEngine::SetPosition(MY_DEFAULT_LISTENER, listenerTransform) == AK_Success)
    {
        std::cout << "listener Position Initialized" << std::endl;
    }
    else
    {
        std::cout << "listener Position Listener failed" << std::endl;
    }




    AK::SoundEngine::RegisterGameObj(MY_EMITTER, "My Emitter");
    // At this point "My Emitter" has 1 listener, "My Default Listener", because we designated it as the default listener.

    AkTransform EmitterTransform;

    AkVector EmitterPosition = { emitterX, emitterY, z };
    EmitterTransform.SetPosition(EmitterPosition);

    AkVector Efront = { 0.0f, 0.0f, -1.0f };
    AkVector Eup = { 0.0f, 1.0f, 0.0f }; // Set up vector to point towards positive y-axis
    EmitterTransform.SetOrientation(Efront, Eup);

    //Emitter error checking
    if (AK::SoundEngine::SetPosition(MY_EMITTER, EmitterTransform) == AK_Success)
    {
        std::cout << "Emitter Position Initialized" << std::endl;
    }
    else
    {
        std::cout << "Emitter Position Listener failed" << std::endl;
    }


    //end listener implentation



    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME, "AMBIENCE_PLAY") == AK_Success)
    {

        std::cout << "game object sucessfully registered" << std::endl;

    }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME2, "DIALOGUESOUND") == AK_Success)
    {

        std::cout << "game object2 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 2 failed" << std::endl; }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME3, "DOOROPEN") == AK_Success)
    {

        std::cout << "game object3 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 3 failed" << std::endl; }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME4, "FOOTSTEPS_GRAVEL") == AK_Success)
    {

        std::cout << "game object4 sucessfully registered" << std::endl;

    }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME5, "FOOTSTEPS_LAVA") == AK_Success)
    {

        std::cout << "game object5 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 5 failed" << std::endl; }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME6, "FOOTSTEPS_STONE") == AK_Success)
    {

        std::cout << "game object6 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 6 failed" << std::endl; }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME7, "FOOTSTEPS_WATER") == AK_Success)
    {

        std::cout << "game object7 sucessfully registered" << std::endl;

    }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME8, "ITEMPICKUP") == AK_Success)
    {

        std::cout << "game object8 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 8 failed" << std::endl; }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME9, "PORTAL") == AK_Success)
    {

        std::cout << "game object9 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 9 failed" << std::endl; }


    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME10, "dirt") == AK_Success)
    {

        std::cout << "game object1 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 10 failed" << std::endl; }



   AK::SoundEngine::PostEvent(AK::EVENTS::PORTAL, MY_EMITTER);

}

enum footstepsType
{
    gravel, 
    water,
    lava,
    stone,
    dirt,

};


bool walk = true;
int counter2 = 0;
void FootSteps(int type)
{
    std::cout << emitterX << std::endl << "emitter Y" << emitterY << "playerX" << playerX / 7.8 << std::endl << "playerY" << playerY / 7.8 << std::endl;

    if (walk == true)
    {
        if (type == gravel)
        {
            AK::SoundEngine::PostEvent(AK::EVENTS::FOOTSTEPS_GRAVEL, GAME_OBJECT_ID_THEME3);
            walk = false;
        }

        else if (type == water)
        {
            AK::SoundEngine::PostEvent(AK::EVENTS::FOOTSTEPS_WATER, GAME_OBJECT_ID_THEME3);
            walk = false;

        }
        else if (type == lava)
        {
            AK::SoundEngine::PostEvent(AK::EVENTS::FOOTSTEPS_LAVA, GAME_OBJECT_ID_THEME3);
            walk = false;

        }
        else if (type == stone)
        {
            AK::SoundEngine::PostEvent(AK::EVENTS::FOOTSTEPS_STONE, GAME_OBJECT_ID_THEME3);
            walk = false;

        }
        else if (type == dirt)
        {
            AK::SoundEngine::PostEvent(AK::EVENTS::FOOTSTEPS_DIRT, GAME_OBJECT_ID_THEME3);
            walk = false;

        }


    }
    else
    {
        if (counter2 < 3)
        {

            counter2++;

        }
        else
        {
            walk = true;
            counter2 = 0;
        }
    }

    //listener position
    listenerPosition = { playerX / 32.0f, playerY / 32.0f , z };
    listenerTransform.SetPosition(listenerPosition);


    if (AK::SoundEngine::SetPosition(MY_DEFAULT_LISTENER, listenerTransform) == AK_Success)
    {
        std::cout << "Position updated" << std::endl;
    }
    else
    {
        std::cout << "Position failed to update" << std::endl;
    }

    AudioLoop();


}

void listernerCall()
{

}

void AudioLoop()
{

    AK::SoundEngine::RenderAudio();
}

