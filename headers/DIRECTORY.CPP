#include "directory.hpp"



int SkyR = 0;
int SkyG = 0;
int SkyB = 0;
int Counter = 0;

int mapX = 8;
int mapY = 8;
int mapSquareSize = 64;
float playerX = 200.0;
float playerY = 300.0;
float playerDeltaX, playerDeltaY, playerAngle;

// listern variable
float emitterX = 100;
float emitterY = 114;

float z = 1.0;
AkTransform listenerTransform;
AkVector listenerPosition = { playerX, playerY, z };

const AkGameObjectID MY_DEFAULT_LISTENER = 0;
const AkGameObjectID MY_EMITTER = 1;
const AkGameObjectID GAME_OBJECT_ID_THEME = 100;
const AkGameObjectID GAME_OBJECT_ID_THEME2 = 200;
const AkGameObjectID GAME_OBJECT_ID_THEME3 = 300;

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

int map[] =
{

    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,1,0,1,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

void drawPlayer2D()
{
    glColor3f(1, 1, 0); 														//sets the player color on the 2D map
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(playerX, playerY);
    glVertex2i(emitterX, emitterY); // emitter
    glEnd();

    //draws the Rotation Line
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(playerX, playerY);
    glVertex2i(playerX + playerDeltaX * 5, playerY + playerDeltaY * 5);
    glEnd();
}


void drawMap2D()
{
    int x, y, xo, yo;

    for (y = 0; y < mapY; y++) {				// for each Y value, We count 8 in the X axis then reset till all 8 rows are accounted for.
        for (x = 0; x < mapX; x++) {
            if (map[y * mapX + x] == 1) {
                glColor3f(1, 0, 0);				// Walls are drawn Red. These are 0 - 1 values...
            }
            else {
                glColor3f(0, 0, 1);				//Empty Space is rendered Blue
            }

            xo = x * mapSquareSize; 			//remember, each square is 64 units
            yo = y * mapSquareSize;

            glBegin(GL_QUADS); 					// we are drawing squares to the screen, the vertices of which are defined here:
            // + 1 and -1 are the lines that cut the screen into a grid
            glVertex2i(0 + xo + 1, 0 + yo + 1);
            glVertex2i(0 + xo + 1, mapSquareSize + yo - 1);
            glVertex2i(mapSquareSize + xo - 1, mapSquareSize + yo - 1);
            glVertex2i(mapSquareSize + xo - 1, 0 + yo + 1);

            glEnd();
        }
    }
}

void mouseClick(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        // convert mouse coordinates to map coordinates
        int mouseX = x / mapSquareSize;
        int mouseY = y / mapSquareSize;

        // flip value in the map at the clicked position
        map[mouseY * mapX + mouseX] = (map[mouseY * mapX + mouseX] == 0) ? 1 : 0;


    }

}

void skyChange()
{
    AK::SoundEngine::PostEvent(AK::EVENTS::FOOTSTEPS, GAME_OBJECT_ID_THEME2);

}

void FootSteps()
{

    AK::SoundEngine::PostEvent(AK::EVENTS::STEP, GAME_OBJECT_ID_THEME3);

    //listener position
    listenerPosition = { playerX, playerY, z };
    listenerTransform.SetPosition(listenerPosition);

    std::cout << listenerTransform.Position().X << std::endl;
    std::cout << listenerTransform.Position().Y << std::endl;
    std::cout << listenerTransform.Position().Z << std::endl;


    if (AK::SoundEngine::SetPosition(MY_DEFAULT_LISTENER, listenerTransform) == AK_Success)
    {
        std::cout << "Position updated" << std::endl;
    }
    else
    {
        std::cout << "Position failed to update" << std::endl;
    }

    
}

void buttons(unsigned char key, int x, int y)
{
    if (key == 'r')
    {
        skyChange();
        if (Counter == 0)
        {
            SkyR = 1;
            SkyG = 0;
            SkyB = 0;
            Counter++;
        }
        else if (Counter == 1)
        {
            SkyR = 0;
            SkyG = 1;
            SkyB = 0;
            Counter++;
        }
        else
        {
            SkyR = 0;
            SkyG = 0;
            SkyB = 1;
            Counter = 0;
        }

    }

    if (key == 'a') 								//Left Movement
    {
        playerAngle -= 0.1;

        if (playerAngle < 0)
        {
            playerAngle += 2 * PI;
        }

        playerDeltaX = cos(playerAngle) * 5;
        playerDeltaY = sin(playerAngle) * 5;
    }

    if (key == 'd')                             //Right Movement
    {
        playerAngle += 0.1;
        if (playerAngle > 2 * PI) {
            playerAngle -= 2 * PI;
        }
        playerDeltaX = cos(playerAngle) * 5;
        playerDeltaY = sin(playerAngle) * 5;
    }

    if (key == 'w') 							   //UP Movement
    {
        FootSteps();
        playerX += playerDeltaX;
        playerY += playerDeltaY;
    }
    if (key == 's')
    {
        //Down Movement
        FootSteps();
        playerX -= playerDeltaX;
        playerY -= playerDeltaY;

    }

    glutPostRedisplay();					   // calls the Screen to be Redrawn ASAP

}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0);				 // set the background window color
    gluOrtho2D(0, 1024, 512, 0);					 // sets Window Size
    playerX = 200; playerY = 300;					 // sets player X and Y posistions
    playerDeltaX = cos(playerAngle) * 5;			 //Controls Rotation Representation Display
    playerDeltaY = sin(playerAngle) * 5;
}

void resize(int w, int h)
{

    glutReshapeWindow(1024, 512);				 //Forces Display to 1024, 512;

}

float distance(float ax, float ay, float bx, float by, float ang) { 				// scales the distance, realizing the depth of field

    return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}



void drawRays3D()
{

    glColor3f(SkyR, SkyG, SkyB); glBegin(GL_QUADS); glVertex2i(526, 0); glVertex2i(1006, 0); glVertex2i(1006, 160); glVertex2i(526, 160); glEnd();	//Floor
    glColor3f(0, 0, 0); glBegin(GL_QUADS); glVertex2i(526, 160); glVertex2i(1006, 160); glVertex2i(1006, 320); glVertex2i(526, 320); glEnd();	//Ceiling

    int ray, mx, my, mp, dof, side;
    float vx, vy, rayX, rayY, rayAngle, xOffset, yOffset, disV, disH, disT;

    rayAngle = playerAngle - DR * 30;
    if (rayAngle < 0) { rayAngle += 2 * PI; }
    if (rayAngle > 2 * PI) { rayAngle - 2 * PI; };

    for (ray = 0;ray < 60;ray++)
    {

        // --Check Horizontal Lines--
        dof = 0;
        disH = 1000000;
        float hx = playerX, hy = playerY;
        float aTan = -1 / tan(rayAngle);
        if (rayAngle > PI) { rayY = (((int)playerY >> 6) << 6) - 0.0001; rayX = (playerY - rayY) * aTan + playerX; yOffset = -64; xOffset = -yOffset * aTan; }
        if (rayAngle < PI) { rayY = (((int)playerY >> 6) << 6) + 64;     rayX = (playerY - rayY) * aTan + playerX; yOffset = 64; xOffset = -yOffset * aTan; }
        if (rayAngle == 0 || rayAngle == PI) { rayX = playerX; rayY = playerY; dof = 8; }
        while (dof < 8) {

            mx = (int)(rayX) >> 6; my = (int)(rayY) >> 6; mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) { hx = rayX; hy = rayY; disH = distance(playerX, playerY, hx, hy, rayAngle);  dof = 8; }
            else { rayX += xOffset; rayY += yOffset; dof += 1; }
        }

        // --Check Vertical Lines--
        dof = 0;
        disV = 1000000, vx = playerX, vy = playerY;
        float nTan = -tan(rayAngle);
        if (rayAngle > P2 && rayAngle < P3) { rayX = (((int)playerX >> 6) << 6) - 0.0001; rayY = (playerX - rayX) * nTan + playerY; xOffset = -64; yOffset = -xOffset * nTan; }
        if (rayAngle<P2 || rayAngle>P3) { rayX = (((int)playerX >> 6) << 6) + 64;     rayY = (playerX - rayX) * nTan + playerY; xOffset = 64; yOffset = -xOffset * nTan; }
        if (rayAngle == 0 || rayAngle == PI) { rayX = playerX; rayY = playerY; dof = 8; }

        while (dof < 8) {

            mx = (int)(rayX) >> 6; my = (int)(rayY) >> 6; mp = my * mapX + mx;

            if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
            {
                vx = rayX; vy = rayY;
                disV = distance(playerX, playerY, vx, vy, rayAngle);  dof = 8;
            }
            else { rayX += xOffset; rayY += yOffset; dof += 1; }

        }

        if (disV < disH) { rayX = vx; rayY = vy; disT = disV;glColor3f(0, 0.7, 0.7); } // horitzontal hit
        if (disH < disV) { rayX = hx; rayY = hy; disT = disH;glColor3f(0, 0.9, 0.9); } // vertical hit

        glLineWidth(3); glBegin(GL_LINES); glVertex2i(playerX, playerY); glVertex2i(rayX, rayY); glEnd();
        //3D

        float ca = playerAngle - rayAngle;

        if (ca < 0) { ca += 2 * PI; }
        if (ca > 2 * PI) { ca -= 2 * PI; } disT = disT * cos(ca); //fix fisheye

        float lineH = (mapSquareSize * 320) / disT; if (lineH > 320) { lineH = 100; }  //constrains height of Walls, Scales their size relative to the Distance from Player

        float line0 = 160 - lineH / 2;

        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(ray * 8 + 530, line0);glVertex2i(ray * 8 + 530, lineH + line0); glEnd(); // casts to the right window

        rayAngle += DR;
        if (rayAngle < 0) { rayAngle += 2 * PI; }
        if (rayAngle > 2 * PI) { rayAngle -= 2 * PI; }

    }
}

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
        assert(!"Could not initialize communication.");
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
    if(AK::SpatialAudio::Init(SpacSettings) !=AK_Success)
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

void listernerCall()
{

}

void AudioLoop()
{
    
    AK::SoundEngine::RenderAudio();
}


void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutMouseFunc(mouseClick);
    drawMap2D();
    drawRays3D();
    drawPlayer2D();
    glutSwapBuffers();
    listernerCall();
    AudioLoop();
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



    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME, "Theme") == AK_Success)
    {

        std::cout << "game object sucessfully registered" << std::endl;

    }


    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME2, "FootSteps") == AK_Success)
    {

        std::cout << "game object2 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 2 failed" << std::endl; }

    if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_THEME3, "STEPS") == AK_Success)
    {

        std::cout << "game object3 sucessfully registered" << std::endl;
    }
    else { std::cout << "game object 3 failed" << std::endl; }

}