/*********************************************************************************************************************************************************************************************************************************/
/*

                   

                                   
                                                                                                                     *********
                                                            *       *         *  * * *  *         *       *          *          *       *    * * *    * * * * *  *       *   * * * *    
                                                          *   *     *         * *        *      *   *     *          * * *      * *     *  *              *      * *     *  *       *   
                                                        *       *   * *     * * *     * *     *       *   *          *          *   *   *  *    *  *      *      *   *   *  *    * *    
                                                      *           *    * * *    *      *  * *           * * * * * *  * * * * *  *     * *  * * * * *  * * * * *  *     * *   * * * *    
                                                                                                                                                                                     
                                                    ********************************************************************************************************************************
                                        

                                                     AuralEngine is 3D first person game engine designed with Sound Designers in mind to test their ideas in a virtual playground. 

                                                                                                                       Overview
   The gateway point for our application is in the main function. First we initialize the Wwise audio engine with the Setup and AudioLoop functions. We then split our application along two threads: in one direction the FLTK GUI
   is initialized through the designerGui function; This thread allows users to build their levels, place dialogue events and sprites. In the other direction, we unroll a thread which opens an OpenGl window and uses a raycast
   algorithm to draw a 3D image to the screen based upon ray intersections on a 2D grid.


                                                                                                                    The Designer GUI
   The designerGui is our initialization function.  It creates all the initial FLTK widgets and links their callback functions. The sessionData structure is the master data structure which is used to communicate between both 
   threads in the application. Beneath it in hierarchy is the MapData structure which holds all the information relating to particular Maps; subservient to that is the Sprite structure which holds data particular to specific
   sprites on specific maps.

   worldMaps is a vector of MapData structs, we navigate through it with the currentMap variable. Member function deleteWorldMaps, performs our heap cleanup. mapNames is a vector of Strings which contains the name of each Map:
   it is passed to the mapLabel FL_Box.

   MapData contains three arrays, which are the Wall, Ceiling and Floor map layouts. Default maps are empty besides boundary walls. Default Maps are copied from the initial array into each new Map. The three sub data structures
   are the positions Vector, which manages Portals and dialogue Events,  the spriteData vector, which is a vector of Sprite objects and by default we have a sky.ppm array which draws our sky but we can have the user load in 
   their own sky as well and store it in the ppmData vector.

   The Designer Gui is refreshed with the drawWorldMaps function and should be called whenever something is updated or loaded from disk.

                                                                                                                     Aural Display

   The Init  function is the gateway into the OpenGL side of the application. We establish the main character's perspective, load game Dialogue and draw any initial sprites or events. The main loop is functionally the display 
   function and its controlled by an ENUM called GameStates; it outlines all the modes the graphics engine can control or if the engine is active or frozen. Its default state is threeDimensionalDisplay.  The game clock is 
   collected from the GLUT_ELAPSED_TIME, swapping between two buffers; this mainly controls how fast the dialogue is written, but also how fast the character turns. Two Dimensional Display reveals the map from the 2D grid 
   perspective. DialogueEvent triggers the game engine's dialogue system, which is modeled after classic RPGs. LoadScreen triggers a Black Screen.



***********************************************************************************************************************************************************************************************************************************/

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
