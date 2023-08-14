#pragma once
//REMOVED ALGORITHM, Assert

//Wwise Headers
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
#include <AK/SpatialAudio/Common/AkSpatialAudio.h> 
#include <AkFileLocationBase.h>
#include <AkPlatformProfilerHooks.h>
#include <AkJobWorkerMgr.h>
#include <AkTypes.h>
#include "Wwise_IDs.h"

//Windows Stuff
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

//Standard Library stuff
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include<vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <thread>

//FLTK
#include "FL/Fl.H"
#include "FL//Fl_Window.H"
#include "FL/Fl_Button.H"
#include <FL/Fl_Dial.H>
#include "FL/Fl_Box.H"
#include "FL/Fl_Choice.H"
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_File_Chooser.H>

//Graphics
#include "Metal.ppm"
#include "All_Textures1.ppm"
#include "sky.ppm"
#include "sprites.ppm"

/*******************************************************************************************GLOBAL VARIABLES*****************************************************************************************************/

//surface colors
float groundRed, groundBlue, groundGreen;
float wallRed, wallBlue, wallGreen;
float ceilingRed, ceilingBlue, ceilingGreen;


//CMATH
#define _USE_MATH_DEFINES

//Wwise defines
#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_RAYCASTER L"RayCast.bnk"

bool prototypeTexture = false;
bool drawSolidColorTextures = false;

//Visual Defines
#define PI 3.1415926532
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533 // ONE DEGREE RADIANS

int numCharsRendered = 0;
std::map<int, std::string> dialogueMap;
bool textLengthReach = false;
int timeCheck = 0;

//Wwise objects
extern const AkGameObjectID MY_DEFAULT_LISTENER;
extern const AkGameObjectID GAME_OBJECT_ID_THEME;
extern const AkGameObjectID GAME_OBJECT_ID_THEME2;
extern const AkGameObjectID GAME_OBJECT_ID_THEME3;
extern CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

extern float z; // Variable for Spatial Audio

// Graphic Variables
extern const int mapX;
extern const int mapY;
extern const int mapSquareSize;
extern float z;                                         // Variable for Spatial Audio
extern float playerX, playerY;
extern float playerDeltaX, playerDeltaY, playerAngle;

bool loadScreenTimerSet = false;

bool skynew = false;


extern const int screenHeight;
extern const int screenWidth;

extern float degToRad(float a);
extern float fixAngle(float a);


/**********************************************************************************************HEADER FUNCTIONS AND STRUCTURES************************************************************************************/


/* Aural_Display */
void loadDialogueFromFile(const std::string& filename, std::map<int, std::string>& dialogueMap);
extern void drawPlayer2D();
extern void drawMap2D();
extern void mouseOFF(int button, int state, int x, int y);
extern void init();
extern void resize(int w, int h);
extern void drawRays2D();
extern void display();
extern void ButtonDown(unsigned char key, int x, int y);
extern void ButtonUp(unsigned char key, int x, int y);
extern void drawSky();

//extern void mouseClick(int button, int state, int x, int y); LEGACY CODE: Allows you to modify map from Q map 

/* Aural_Audio */
extern bool InitSoundEngine();
extern void Setup();
extern void FootSteps();
extern void listernerCall();
extern void AudioLoop();
extern void skyChange();

/* Aural_Textures */
extern const int mapFloor[];
extern const int mapCeiling[];
extern const int All_Textures[];

/* Aural_Maps */
extern const int Map[];

//DesignerGui

extern struct sessionData;
extern struct MapData;
extern struct PositionWindowData;
extern struct SpriteWindowData;
extern struct DialogueWindowData;

std::vector<std::vector<Fl_Button*>> gridButtons(mapX, std::vector<Fl_Button*>(mapY));
std::vector<int> loadPPM(const std::string& filename);

extern void intializeGUI();
extern void designerGui();
extern void drawWorldMap();
extern void checkButtonCallback(Fl_Widget* widget, void* data);
extern void submitCallback(Fl_Widget* widget, void* data);
extern void worldMapCallback(Fl_Widget* widget, void* data);
extern void wallTextureCallback(Fl_Widget* widget, void* data);
extern void floorTextureCallback(Fl_Widget* widget, void* data);
extern void ceilingTextureCallback(Fl_Widget* widget, void* data);
extern void textureButtons(int initialX, int initialY, int buttonAmount, int flag);
extern void newWorldMap(Fl_Widget* widget, void* data);
extern void positionInputCallback(Fl_Widget* widget, void* data);
extern void selectButtonCallback(Fl_Widget* widget, void* data);
extern void portalMovement(Fl_Widget* widget, void* data);
extern void mapBackCallback(Fl_Widget* widget, void* data);
extern void mapForwardCallback(Fl_Widget* widget, void* data);
extern void confirmSpriteCallback(Fl_Widget* widget, void* data);
extern void addSpriteCallback(Fl_Widget* widget, void* data);
extern void confirmDialogueEventCallback(Fl_Widget* widget, void* data);
extern void dialogueEventCallback(Fl_Widget* widget, void* data);
extern void setPlayerLocation(Fl_Widget* widget, void* data);
extern void dialogueRefreshCallback(Fl_Widget* widget, void* data);
extern void newSkyCallback(Fl_Widget* widget, void* data);
extern void protoTexCallback(Fl_Widget* widget, void* data);
extern void solidColorCallback(Fl_Widget* widget, void* data);
extern void redColorValueCallback(Fl_Widget* widget, void* data);
extern void greenColorValueCallback(Fl_Widget* widget, void* data);
extern void blueColorValueCallback(Fl_Widget* widget, void* data);
extern void saveData(const std::string& filename);
extern void saveData(const std::string& filename);
extern void loadData(const std::string& filename);
extern void loadButtonCallback(Fl_Widget* widget, void* data);

enum GameState
{
    loadScreen,
    threeDimensionalDisplay,
    twoDimensionalDisplay,
    dialogueEvent,
    FREEZE

}; GameState gameState = threeDimensionalDisplay;

enum textures
{
    Wall,
    Floor,
    Ceiling,

};

typedef struct          //All veriables per sprite
{
    int type;           //static, key, enemy
    int state;          //on off
    int map;            //texture to show
    float x, y, z;      //position
    int dialogueMarker;
}sprite;

enum EVENTS
{
    Portal,
    Dialogue

};