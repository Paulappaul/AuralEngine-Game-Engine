/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/
#pragma once
#
#include <stdafx.h>
#include <AkSoundEngineDLL.h>
#include <IAkStreamMgr.h>
#include <AK/Tools/Common/AkAssert.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Comm/AkCommunication.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AkFilePackageLowLevelIOBlocking.h>                    // Sample low-level I/O implementation
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AkDefaultIOHookBlocking.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AkMultipleFileLocation.h>
#include <AkFilePackageLUT.h>
#include <AkFilePackage.h>
#include <AkGeneratedSoundBanksResolver.h>
#include<assert.h>
#include "Wwise_IDs.h"
#include <iostream>

#include <chrono>
#include <thread>

#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_RAYCASTER L"RayCast.bnk"


const AkGameObjectID GAME_OBJECT_ID_THEME = 100;

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;



bool InitSoundEngine()
{


    AkMemSettings memSettings;
    AK::MemoryMgr::GetDefaultSettings(memSettings);

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        std::cout << (!"Could not create the memory manager.") << std::endl;
        return false;
    }
    else { std::cout << "Memory Manager Intialized"; }


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
        std::cout << "true" << std::endl;
    }


    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

  
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
    else { std::cout << "sound engine fail" << std::endl; }

    g_lowLevelIO.SetBasePath(AKTEXT("C:\\Users\\alcin\\Documents\\WwiseProjects\\MyGame\\GeneratedSoundBanks\\Windows"));
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

    if (AK::SoundEngine::LoadBank(BANKNAME_RAYCASTER, bankID) != AK_Success)
    {

        std::cout << "ERROR: COULD NOT LOAD BANK";
    }
    else
    {
        std::cout << "game bank loaded" << std::endl;
    }

#ifndef AK_OPTIMIZED
    //
    // Initialize communications (not in release build!)
    //
    AkCommSettings commSettings;
    AK::Comm::GetDefaultInitSettings(commSettings);
    if (AK::Comm::Init(commSettings) != AK_Success)
    {
        assert(!"Could not initialize communication.");
        return false;
    }
    else
    {
        std::cout << "communication intialized" << std::endl;
    }
#endif // AK_OPTIMIZED

    return true;
    // Post the event to play the theme song



}
	



int main()
{
    AK::Monitor::SetLocalOutput;

   
    char x;

        if (InitSoundEngine() == true)
        {
            std::cout << " intialization complete" << std::endl;
        }

        else { std::cout << "intialization failed" << std::endl; }

        if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME, "Theme") == AK_Success) {

            std::cout << "game object sucessfully registered" << std::endl;
        }

        if (AK::SoundEngine::PostEvent(AK::EVENTS::THEMESONG, GAME_OBJECT_ID_THEME) == AK_Success)
        {
            std::cout << "Posted" << std::endl;
        }
        else (std::cout << "Post Failed" << std::endl);


        //std::cout << (AK::SoundEngine::PostEvent(AK::EVENTS::THEMESONG, GAME_OBJECT_ID_THEME)) << std::endl;

        std::cin >> x;



        int i = 0;
        while (true)
        {
            std::cout << i << std::endl;
            i++;
            AK::SoundEngine::RenderAudio();
            std::this_thread::sleep_for(std::chrono::milliseconds(22));
        }
   
       

        return 0;
   
}