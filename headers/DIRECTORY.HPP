

#include <stdafx.h>
#include <AkSoundEngineDLL.h>
#include <IAkStreamMgr.h>
#include <AK/Tools/Common/AkAssert.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
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
#include <AK/SpatialAudio/Common/AkSpatialAudio.h> //
#include<assert.h>

#include "Wwise_IDs.h"

#include <AkFileLocationBase.h>
#include <AkPlatformProfilerHooks.h>
#include <AkJobWorkerMgr.h>
#include <AkTypes.h>

#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

//Wwise defines
#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_RAYCASTER L"RayCast.bnk"

//Visual Defines
#define PI 3.1415926532
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533 // ONE DEGREE RADIANS

//Wwise objects
extern const AkGameObjectID MY_DEFAULT_LISTENER;
extern const AkGameObjectID GAME_OBJECT_ID_THEME;
extern const AkGameObjectID GAME_OBJECT_ID_THEME2;
extern const AkGameObjectID GAME_OBJECT_ID_THEME3;
extern CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

//Graphic Variables
extern int mapX;
extern int mapY;
extern int mapSquareSize;
extern float playerX, playerY;
extern float playerDeltaX, playerDeltaY, playerAngle;
extern int map[];



/* this function draws the character and the rotation arrow */
void drawPlayer2D();

/*This Function iterates through the map array using two nested for loops;
  where one loop represents Y (rows) and the second represents X (columns). */
void drawMap2D();

/* This function handles the controls of game */
void buttons(unsigned char key, int x, int y);

/*This is the initalization function and sets the characters X, Y posistion, draws the background color*/
void init();

/*Forces a Fixed Screen Size */
void resize(int w, int h);

/*Scales the perspective */
float distance(float ax, float ay, float bx, float by, float ang);

/* Draws the Right Side of the Screen */
void drawRays3D();


/*Game main Loop*/
void display();


bool InitSoundEngine();

void AudioLoop();

void display();

void Setup();