#include "Directory.h"
#include "Aural_Audio.h"
#include "Aural_Display.h"
#include "Aural_Maps.h"
#include "Aural_Textures.h"
#include "DesignerGUI.h"

void cleanup()
{
    thisSession.deleteWorldMaps();
    
}


int main(int argc, char* argv[])
{
    //audio intialize
    //Setup();

    //AudioLoop();
    std::thread fltkThread(designerGui);

    //graphic intialize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenWidth, screenHeight) ;
    glutInitWindowPosition(0, 0);
    glutCreateWindow("AuralEngine");

    init();
    glutDisplayFunc(display);

    glutKeyboardFunc(ButtonDown);
    glutKeyboardUpFunc(ButtonUp);

    //guiInit();

    glutMainLoop();

    Fl::lock(); // Ensure that FLTK resources are locked before termination
    fltkThread.join();
    Fl::unlock();
/*
    AK::SoundEngine::Term();

    // Terminate the streaming device and streaming manager
    // CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
    // that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
    g_lowLevelIO.Term();

    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();

    // Terminate the Memory Manager
    AK::MemoryMgr::Term();

    */
    atexit(cleanup);

    return 0;
}
