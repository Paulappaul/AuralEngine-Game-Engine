#include "directory.hpp"
#include "directory.cpp"


int main(int argc, char* argv[])
{
    //audio intialize
    Setup();
    AK::SoundEngine::PostEvent(AK::EVENTS::EMITTERE, MY_EMITTER);
    AudioLoop();


    //graphic intialize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("RayCaster Engine");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(buttons);
    glutMainLoop();

    AK::SoundEngine::Term();

    // Terminate the streaming device and streaming manager
    // CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
    // that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
    g_lowLevelIO.Term();

    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();

    // Terminate the Memory Manager
    AK::MemoryMgr::Term();


    return 0;
}