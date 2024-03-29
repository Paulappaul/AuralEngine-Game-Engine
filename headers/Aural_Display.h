#pragma once
#include "Directory.h"
#include "DesignerGUI.h"

//controls which dialogue is written to the screen, passed to dialogueMap
int dialogueEventLocator = 0;
int dialogueGraphicLocator = 0;
int dialogueGraphicLocator2 = 0;

int depth[120];      //hold wall line depth to compare for sprite depth

typedef struct
{
    int moveForward, turnLeft, turnRight, moveBackward;                     //button state on off
}ButtonKeys; ButtonKeys Keys;

float frame1, frame2, fps;

int Counter = 0;

int const mapX = 24;
int const  mapY = 24;
int const mapSquareSize = 64;

float playerX = 768;  // 84.0;
float playerY = 768; //84.0;

const int screenHeight = 1280;
const int screenWidth = 1920;

float playerDeltaX, playerDeltaY, playerAngle;

float z = 1.0; // for Spatial Audio

float degToRad(float a) { return a * PI / 180.0; }
float fixAngle(float a) { if (a > 359) { a -= 360; } if (a < 0) { a += 360; } return a; }
float distance(float ax, float ay, float bx, float by, float ang) { return cos(degToRad(ang)) * (bx - ax) - sin(degToRad(ang)) * (by - ay); }

//to return, remove /2

int YSize = screenHeight / mapY;
int XSize = screenWidth / mapX;

bool keyPickup = false;

void drawSprite()
{
    int x, y, s;


    if (keyPickup == false)
    {
        if (playerX < thisSession.worldMaps.at(thisSession.currentMap)->spriteData[0].x + 30 &&
            playerX > thisSession.worldMaps.at(thisSession.currentMap)->spriteData[0].x - 30 &&
            playerY < thisSession.worldMaps.at(thisSession.currentMap)->spriteData[0].y + 30 &&
            playerY > thisSession.worldMaps.at(thisSession.currentMap)->spriteData[0].y - 30)
        {

            thisSession.worldMaps.at(thisSession.currentMap)->spriteData[0].state = 0; // picked up a key
            AK::SoundEngine::PostEvent(AK::EVENTS::KEY_PICKUP, GAME_OBJECT_ID_THEME8);
            std::cout << "key pikup" << std::endl;
            AudioLoop();
            keyPickup = true;
        }

    }

    for (s = 0; s < thisSession.worldMaps.at(thisSession.currentMap)->spriteData.size(); s++)
    {

        float sx = thisSession.worldMaps.at(thisSession.currentMap)->spriteData[s].x - playerX;
        float sy = thisSession.worldMaps.at(thisSession.currentMap)->spriteData[s].y - playerY;
        float sz = thisSession.worldMaps.at(thisSession.currentMap)->spriteData[s].z;

        float CS = cos(degToRad(playerAngle));
        float SN = sin(degToRad(playerAngle));
        float a = sy * CS + sx * SN;
        float b = sx * CS - sy * SN;
        sx = a;
        sy = b;

        sx = (sx * 108.0 / sy) + (120 / 2); // convert to screen x,y
        sy = (sz * 108.0 / sy) + (80 / 2);

        int scale = 32 * 80 / b;   // scale sprite based on distance, sprite size
        if (scale < 0) { scale = 0; } if (scale > 120) { scale = 120; }

        // texture
        float t_x = 0, t_y = 31, t_x_step = 31.5 / (float)scale, t_y_step = 32.0 / (float)scale;

        for (x = sx - scale / 2; x < sx + scale / 2; x++) // draws the sprite size as 32x32
        {
            t_y = 31;
            for (y = 0; y < scale; y++)
            {
                if (thisSession.worldMaps.at(thisSession.currentMap)->spriteData[s].state == 1 &&
                    x > 0 && x < 120 && b < depth[x])
                {
                    int pixel = ((int)t_y * 32 + (int)t_x) * 3 +
                        (thisSession.worldMaps.at(thisSession.currentMap)->spriteData[s].map * 32 * 32 * 3);
                    int red = sprites[pixel + 0];
                    int green = sprites[pixel + 1];
                    int blue = sprites[pixel + 2];
                    if (red != 255 && green != 0 && blue != 255) // dont draw if purple
                    {
                        glPointSize(16);
                        glColor3ub(red, green, blue);
                        glBegin(GL_POINTS);
                        glVertex2i(x * 16, sy * 16 - y * 16);
                        glEnd(); // draw point 
                    }
                    t_y -= t_y_step;
                    if (t_y < 0) { t_y = 0; }
                }
            }
            t_x += t_x_step;
        }
    }
}


void loadDialogueFromFile(const std::string& filename, std::map<int, std::string>& dialogueMap)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Find the position of the colon
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            std::cerr << "Invalid format in line: " << line << std::endl;
            continue;
        }

        // Extract the identifier and dialogue text
        std::string identifierStr = line.substr(0, colonPos);
        std::string dialogueText = line.substr(colonPos + 1);

        // Convert the identifier to an integer
        int identifier = std::stoi(identifierStr);

        // Add the identifier and dialogue text to the map
        dialogueMap[identifier] = dialogueText;
    }

    file.close();
}

void drawSquare(int leftLength, int rightLength, int topLength, int bottomLength)
{
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(leftLength, bottomLength);  // Bottom-left vertex (moved to 1/4 across the screen)
    glVertex2f(rightLength, bottomLength);  // Bottom-right vertex (moved to 3/4 across the screen)
    glVertex2f(rightLength, topLength);  // Top-right vertex (moved to 3/4 across the screen)
    glVertex2f(leftLength, topLength);  // Top-left vertex (moved to 1/4 across the screen)
    glEnd();
}

int counter = 0;

void text()
{
    GLfloat x = 350;
    GLfloat y = 850;
    GLfloat lineHeight = 50; // Adjust this value to set the vertical distance between lines

    // Set the color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Set the raster position (screen coordinates) where the text will start
    glRasterPos2f(x, y);

    // The font to be used for rendering the text
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;

    //  void* font = GLUT_BITMAP_TIMES_ROMAN_10;


    std::string currentDialogue = dialogueMap[dialogueEventLocator];

    // Render a subset of the text based on the numCharsRendered value
    for (int i = 0; i < numCharsRendered; ++i)
    {
        if (i > 0 && i % 91 == 0) // Move to a new line after 91 characters
        {
            y += lineHeight; // Move down to the next line
            glRasterPos2f(x, y); // Set the new raster position
        }
        glutBitmapCharacter(font, currentDialogue[i]); // Convert char to glutBitmapCharacter

    }



    
        AK::SoundEngine::PostEvent(AK::EVENTS::DIALOGUE, GAME_OBJECT_ID_THEME2);
        AudioLoop();
        counter = 0;
    

    // Check if all characters have been rendered, so we Freeze the text to the string 
    if (numCharsRendered >= currentDialogue.size())
    {
        textLengthReach = true;
    }

    glutPostRedisplay();
}

void drawPlayer2D()
{
    glColor3f(1, 1, 0); // sets the player color on the 2D map


    // Calculate the player's position in the scaled coordinates
    int scaledPlayerX = playerX * XSize / mapSquareSize;
    int scaledPlayerY = playerY * YSize / mapSquareSize;

    // Set the point size based on the scaling factor
    float pointSize = XSize / 8.0f;
    glPointSize(pointSize);

    // Draw the player as a point
    glBegin(GL_POINTS);
    glVertex2i(scaledPlayerX, scaledPlayerY);
    glEnd();

    // Calculate the end point of the rotation line
    int rotatedPlayerX = (int)(playerX + playerDeltaX * 20) * XSize / mapSquareSize;
    int rotatedPlayerY = (int)(playerY + playerDeltaY * 20) * YSize / mapSquareSize;

    // Draw the rotation line
    glLineWidth(4);
    glBegin(GL_LINES);
    glVertex2i(scaledPlayerX, scaledPlayerY);
    glVertex2i(rotatedPlayerX, rotatedPlayerY);
    glEnd();
}




void drawMap2D()
{
    int x, y, xo, yo;


    for (y = 0; y < mapY; y++)
    {				// for each Y value, We count 8 in the X axis then reset till all 24 rows are accounted for.
        for (x = 0; x < mapX; x++)
        {
            if (Map[y * mapX + x] == 1)
            {
                glColor3f(0, 0, 0);				// Walls are drawn Red. These are 0 - 4 values...
            }
            else if (Map[y * mapX + x] == 2)
            {
                glColor3f(0.3, 0.4, 0.8);				// Walls are drawn Red. These are 0 - 1 values...
            }
            else if (Map[y * mapX + x] == 3)
            {
                glColor3f(0, 0.8, 0.2);				// Walls are drawn Red. These are 0 - 1 values...
            }
            else if (Map[y * mapX + x] == 4)
            {
                glColor3f(0, 1, 0);				// Walls are drawn Red. These are 0 - 1 values...
            }
            else
            {
                glColor3f(1, 1, 1);				//Empty Space is rendered Blue
            }

            xo = x * XSize; 			//remember, each square is 64 units
            yo = y * YSize;

            glBegin(GL_QUADS); 					// we are drawing squares to the screen, the vertices of which are defined here:
            // + 1 and -1 are the lines that cut the screen into a grid
            glVertex2i(xo + 1, yo + 1);

            glVertex2i(xo + 1, YSize + yo);

            glVertex2i(XSize + xo - 1, YSize + yo - 1);

            glVertex2i(XSize + xo - 1, yo + 1);

            glEnd();
        }
    }
}

//LEGACY CODE 
/*
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        std::cout << screenHeight << std::endl;

            // Convert mouse coordinates to map coordinates
            int mouseX = x / XSize;
            int mouseY = y / ((screenHeight -240)/24) ;


            // Flip the value in the map at the clicked position
            Map[mouseY * mapX + mouseX] = (Map[mouseY * mapX + mouseX] == 0) ? 1 : 0;

            std::cout << Map[mouseY * mapX + mouseX] << std::endl;

    }
}
*/
void mouseOFF(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
       
    }
}

void init()
{
    loadDialogueFromFile("dialogue/dialogue_data.txt", dialogueMap);
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, screenWidth, screenHeight, 0);
    playerDeltaX = cos(degToRad(playerAngle));
    playerDeltaY = -sin(degToRad(playerAngle));

    std::string ppmFilenames[4] = 
    {
        "images/image1.ppm",
        "images/image2.ppm",
        "images/image3.ppm",
        "images/image4.ppm"
    };

    for (const std::string& filename : ppmFilenames)
    {
        std::vector<int> ppmImageData = loadPPM(filename);

        thisSession.worldMaps[thisSession.currentMap]->DialogueGraphic.insert(
            thisSession.worldMaps[thisSession.currentMap]->DialogueGraphic.end(),
            ppmImageData.begin(), ppmImageData.end());
    }

    for (const std::string& filename : ppmFilenames)
    {
        std::vector<int> ppmImageData = loadPPM(filename);

        DialogueGraphic.insert(
            DialogueGraphic.end(),
            ppmImageData.begin(), ppmImageData.end());
    }


    // Initialize the spriteData vector in the current MapData instance
   thisSession.worldMaps.at(thisSession.currentMap)->spriteData =
    {
        {1, 1, 0, 10 *  64, 10  * 64, 20, 0, 0}, // key
        {2, 1, 1, 1.5 * 64, 4.5 * 64, 20, 1, 1}, // light 1
        {2, 1, 1, 3.5 * 64, 4.5 * 64, 20, 2, 2}, // light 2
        {3, 1, 2, 2.5 * 64, 2   * 64, 20, 3 ,3} // enemy
    };
    
}


void resize(int w, int h)
{

    glutReshapeWindow(screenWidth, screenHeight);				 //Forces Display to 1024, 512;

}

void drawFloorCeilingSolidColor()
{

    glColor3f(ceilingRed, ceilingGreen, ceilingBlue);
    glBegin(GL_QUADS);
    glVertex2i(0, screenHeight / 2);
    glVertex2i(screenWidth, screenHeight / 2);
    glVertex2i(screenWidth, 0);
    glVertex2i(0, 0);
    glEnd();

    //floor
    glColor3f(groundRed, groundBlue, groundGreen);
    glBegin(GL_QUADS);
    glVertex2i(0, screenHeight);
    glVertex2i(screenWidth, screenHeight);
    glVertex2i(screenWidth, screenHeight / 2);
    glVertex2i(0, screenHeight / 2);
    glEnd();

}

void drawRays2D()
{
    //First we draw our backdrop, if we want a color we raise the flag
    if (drawSolidColorTextures == true)
    {
        drawFloorCeilingSolidColor();

    }

    int ray, mx, my, mapPosition, dof, side;
    float verticalX, verticalY, rayX, rayY, rayAngle, xOffset, yOffset, disV, disH;

    rayAngle = fixAngle(playerAngle + 30);                                          // ray set back 30 degrees

    for (ray = 0; ray < 120; ray++)
    {
        int vmt = 0, hmt = 0;                                                       // vertical and horizontal map texture number

        //-------------------------------Vertical Check-----------------------------//
        dof = 0;
        side = 0;
        disV = 100000;
        float Tan = tan(degToRad(rayAngle));

        if (cos(degToRad(rayAngle)) > 0.001)                                        // if the angle is greater than PI
        {
            rayX = (((int)playerX >> 6) << 6) + 64;
            rayY = (playerX - rayX) * Tan + playerY;
            xOffset = 64;
            yOffset = -xOffset * Tan;
        }
        else if (cos(degToRad(rayAngle)) < -0.001)                                  // if the angle is less than PI
        {
            rayX = (((int)playerX >> 6) << 6) - 0.0001;                             // round to the nearest 64th value i.e. the mapSquareValue, we are bit shifting
            rayY = (playerX - rayX) * Tan + playerY;
            xOffset = -64;
            yOffset = -xOffset * Tan;
        }
        else                                                                        // If we are looking perfectly up or down, then there will be no vertical Intersections.
        {
            rayX = playerX;
            rayY = playerY;
            dof = 24;

        }

        while (dof < 24)
        {
            mx = (int)(rayX) >> 6;
            my = (int)(rayY) >> 6;
            mapPosition = my * mapX + mx;

            //Check For Wall Collision
            if (mapPosition > 0 && mapPosition < mapX * mapY && thisSession.worldMaps[thisSession.currentMap]->newMap[mapPosition] > 0)                          //first two conditions check to see if are our coordinates are within the map array i.e. 0 -> mapX * mapY
            {                                                                                                  //if so, we check to see if the mapPositionn corrosponds to a wall posistion (1)
                vmt = thisSession.worldMaps[thisSession.currentMap]->newMap[mapPosition] - 1;
                dof = 24;
                disV = cos(degToRad(rayAngle)) * (rayX - playerX) - sin(degToRad(rayAngle)) * (rayY - playerY); // hit
                //Ends the Loop condition

            }
            else                                                                                                // check the next vertical grid line. This is where the DDA is most useful.
            {
                rayX += xOffset;
                rayY += yOffset;
                dof += 1;
            }
        }
        verticalX = rayX;
        verticalY = rayY;

        //---------------------------------------------Horizontal Check----------------------------------------//
        dof = 0;
        disH = 100000;
        Tan = 1.0 / Tan;
        if (sin(degToRad(rayAngle)) > 0.001)
        {
            rayY = (((int)playerY >> 6) << 6) - 0.0001;
            rayX = (playerY - rayY) * Tan + playerX;
            yOffset = -64;
            xOffset = -yOffset * Tan;
        }
        else if (sin(degToRad(rayAngle)) < -0.001)
        {
            rayY = (((int)playerY >> 6) << 6) + 64;
            rayX = (playerY - rayY) * Tan + playerX;
            yOffset = 64;
            xOffset = -yOffset * Tan;
        }
        else                                                    // if we are looking straight left or right there will be no Horizontal Intersections
        {
            rayX = playerX;
            rayY = playerY;
            dof = 24;
        }

        while (dof < 24)
        {
            mx = (int)(rayX) >> 6;
            my = (int)(rayY) >> 6;
            mapPosition = my * mapX + mx;
            if (mapPosition > 0 && mapPosition < mapX * mapY && thisSession.worldMaps[thisSession.currentMap]->newMap[mapPosition] > 0)
            {
                hmt = thisSession.worldMaps[thisSession.currentMap]->newMap[mapPosition] - 1;
                dof = 24;
                disH = cos(degToRad(rayAngle)) * (rayX - playerX) - sin(degToRad(rayAngle)) * (rayY - playerY); // hit

            }
            else                                                                                                // check next horizontal
            {
                rayX += xOffset;
                rayY += yOffset;
                dof += 1;
            }
        }

        float shade = 0.5;
        glColor3f(wallRed, wallGreen, wallBlue);                       // color

        if (disV < disH)                                               // horizontal hit first check this
        {
            hmt = vmt;
            shade = 0.25;
            rayX = verticalX;
            rayY = verticalY;
            disH = disV;
            glColor3f(wallRed - 0.2, wallGreen - 0.2, wallBlue - 0.2); // shade color
        }

        ///*******************2D Map Rav Visualizer*******************///
        /*
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2i(playerX, playerY);
        glVertex2i(rayX, rayY);
        glEnd(); // draw 2D ray
        */
        ///***********************************************************///

        int ca = fixAngle(playerAngle - rayAngle);
        disH = disH * cos(degToRad(ca));                             // Fix fisheye
        int lineH = (mapSquareSize * screenHeight) / disH;
        float ty_step = 64.0 / static_cast<float>(lineH);
        float ty_off = 0;

        if (lineH > screenHeight)                                    // Line height and limit
        {
            ty_off = (lineH - screenHeight) / 2.0;
            lineH = screenHeight;
        }

        int lineOff = screenHeight / 2 - (lineH >> 1);               // Line offset, controls the Height of the Camera too. 

        depth[ray] = disH;                                           //save line depth for Sprites

        // --------------------------------------------------------------------------------------Draw walls--------------------------------------------------------------------------------------------------------------- //

        //Setup
        int y;
        float ty = ty_off * ty_step;
        float tx = 0;
        int textureSize = 0;
        int textureLimiter = 0;

        if (prototypeTexture == false) { textureSize = 64; textureLimiter = 63; }
        else { textureSize = 32; textureLimiter = 31; }


        if (prototypeTexture == true)
        {
            ty = ty_off * ty_step + hmt * 32;
        }

        if (shade == 0.5)
        {
            tx = static_cast<int>(rayX) % textureSize;
            if (rayAngle > 180)
            {
                tx = textureLimiter - tx;
            }
        }
        else
        {
            tx = static_cast<int>(rayY) % textureSize;
            if (rayAngle > 90 && rayAngle < 270)
            {
                tx = textureLimiter - tx;
            }
        }



        // ---------------------------------------------------------------- Three Modes: Textured, ProtoType Texture, Solid Colors -------------------------------------------------------------------------------------- //


        //ProtoType Textures Draw
        for (y = 0; y < lineH; y++)
        {
            if (prototypeTexture == true)
            {


                float c = All_Textures[static_cast<int>(ty) * 32 + static_cast<int>(tx)] * shade;
                if (hmt == 0) { glColor3f(c, c / 2.0, c / 2.0); }                                // Checkerboard red
                if (hmt == 1) { glColor3f(c, c, c / 2.0); }                                      // Brick yellow
                if (hmt == 2) { glColor3f(c / 2.0, c / 2.0, c); }                                // Window blue
                if (hmt == 3) { glColor3f(c / 2.0, c, c / 2.0); }                                // Door green

                glBegin(GL_POINTS);
                glVertex2i(ray * 16, y + lineOff);
                glEnd();
                ty += ty_step;
            }

            //Solid Color Draw
            else if (drawSolidColorTextures == true)
            {


                glLineWidth(16);
                glBegin(GL_LINES);
                glVertex2i(ray * 16, lineOff);
                glVertex2i(ray * 16, lineOff + lineH);
                glEnd();

            }
            //Draw 64X64 textures
            else
            {

                ty = (ty_off + y) * ty_step + hmt * 64;

                // Code for Textured surfaces with 64x64 textures
                int pixel = ((static_cast<int>(ty) & 63) * 64 + (static_cast<int>(tx) & 63)) * 3 + hmt * 64 * 64 * 3;

                int red =   wallTextures[pixel + 0] * shade;
                int green = wallTextures[pixel + 1] * shade;
                int blue =  wallTextures[pixel + 2] * shade;

                glPointSize(16);
                glColor3b(red, green, blue);

                glBegin(GL_POINTS);
                glVertex2i(ray * 16, y + lineOff);
                glEnd();

                // Increment texture coordinate for the next row using 64x64 texture size
                ty += ty_step;
            }
        }


        /**************************Draw Floor and Ceiling*********************************/
        for (y = lineOff + lineH; y < screenHeight; y++)
        {
            float dy = y - (screenHeight / 2.0);
            float deg = degToRad(rayAngle);
            float raFix = cos(degToRad(fixAngle(playerAngle - rayAngle)));
            tx = playerX / 2 + cos(deg) * 158 * 2 * 64 / dy / raFix; // Use 64 for texture size
            ty = playerY / 2 - sin(deg) * 158 * 2 * 64 / dy / raFix; // Use 64 for texture size


            // ------------------------Draw floors---------------------------//

            if (prototypeTexture == true)
            {
                // Ensure ty and tx are within the valid range of texture indices (0 to 255)
                int tyIndex = static_cast<int>(ty / 96.0);
                int txIndex = static_cast<int>(tx / 96.0);

                // Clamp ty and tx to valid range
                if (tyIndex < 0) tyIndex = 0;
                if (tyIndex >= 256) tyIndex = 255;
                if (txIndex < 0) txIndex = 0;
                if (txIndex >= 256) txIndex = 255;

                int mp = mapFloor[tyIndex * mapX + txIndex] * 32 * 32;
                float c = All_Textures[((int)(ty) & 31) * 32 + ((int)(tx) & 31) + mp] * 0.7;

                glColor3f(c / 1.3, c / 1.3, c);
                glPointSize(24);
                glBegin(GL_POINTS);
                glVertex2i(ray * 24, y);
                glEnd();

                mp = mapCeiling[tyIndex * mapX + txIndex] * 32 * 32;
                c = All_Textures[(static_cast<int>(ty) & 31) * 32 + (static_cast<int>(tx) & 31) + mp] * 0.7;
                glColor3f(c / 2.0, c / 1.2, c / 2.0);
                glPointSize(24);
                glBegin(GL_POINTS);
                glVertex2i(ray * 24, screenHeight - y);
                glEnd();

            }
            else if (drawSolidColorTextures == true)
            {




            }
            else
            {
                int mp = thisSession.worldMaps[thisSession.currentMap]->floorMap[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32;
                float c = All_Textures[(static_cast<int>(ty) & 31) * 32 + (static_cast<int>(tx) & 31) + mp] * 0.7;                          // moving 32 spaces moves to the next tile set

                int pixel = (((int)(ty) & 31) * 32 + ((int)(tx) & 31)) * 3 + mp * 3;                                                        // 32 32 *3 represents number of pictures in each texture
                int red =   floorTextures[pixel + 0] * 0.25;
                int green = floorTextures[pixel + 1] * 0.25;
                int blue =  floorTextures[pixel + 2] * 0.25;
                glPointSize(16);
                glColor3b(red, green, blue);
                glBegin(GL_POINTS);
                glVertex2i(ray * 16, y);
                glEnd();

                // -----------------------------------------------------Draw ceiling-------------------------------------------------------- //
                int mpCeiling = thisSession.worldMaps[thisSession.currentMap]->ceilingMap[(int)(ty / 32.0) * mapX + (int)(tx / 32.0)] * 32 * 32;
                pixel = (((int)(ty) & 31) * 32 + ((int)(tx) & 31)) * 3 + mpCeiling * 3;                                                     // 32 32 *3 represents number of pictures in each texture
                red =   ceilingTextures[pixel + 0] * 0.25;
                green = ceilingTextures[pixel + 1] * 0.25;
                blue =  ceilingTextures[pixel + 2] * 0.25;
                if (mpCeiling > 0) // else we Drawing the Sky.
                {
                    glPointSize(16);
                    glColor3b(red, green, blue);
                    glBegin(GL_POINTS);
                    glVertex2i(ray * 16, screenHeight - y);
                    glEnd();
                }
            
            }

        }


        rayAngle = fixAngle(rayAngle - 0.5);                                                                                                  // Go to next ray, incrementing by 0.5. There are 120 Rays total covering a 60 degree POV
    }
}



void drawSky()     //draw sky and rotate based on player rotation
{
    if (thisSession.worldMaps[thisSession.currentMap]->ppmData.empty() || thisSession.currentMap == 0)
    {
        int x, y;
        for (y = 0;y < 80;y++)
        {
            for (x = 0;x < 256; x++)
            {
                int xo = (int)playerAngle * 2 - x; if (xo < 0) { xo += 256; } xo = xo % 256; //return 0-120 based on player angle
                int pixel = (y * 256 + xo) * 3;
                int red = sky[pixel + 0];
                int green = sky[pixel + 1];
                int blue = sky[pixel + 2];
                glPointSize(8);
                glColor3ub(red, green, blue);
                glBegin(GL_POINTS);
                glVertex2i(x * 8, y * 8);
                glEnd();
            }
        }
    }
    else
    {
        int x, y;
        for (y = 0;y < 80;y++)
        {
            for (x = 0;x < 256; x++)
            {
                int xo = (int)playerAngle * 2 - x; if (xo < 0) { xo += 256; } xo = xo % 256; //return 0-120 based on player angle
                int pixel = (y * 256 + xo) * 3;
                int red = thisSession.worldMaps[thisSession.currentMap]->ppmData[pixel + 0] * 0.6;
                int green = thisSession.worldMaps[thisSession.currentMap]->ppmData[pixel + 1] * 0.6;
                int blue = thisSession.worldMaps[thisSession.currentMap]->ppmData[pixel + 2] * 0.6;
                glPointSize(8);
                glColor3ub(red, green, blue);
                glBegin(GL_POINTS);
                glVertex2i(x * 8, y * 8);
                glEnd();
            }
        }

    }

}
void drawDialogueImage()
{
    std::cout << "curentmap " << thisSession.currentMap << std::endl;

    if (thisSession.currentMap == 0)
    {
        if (dialogueGraphicLocator < 0 || dialogueGraphicLocator >= thisSession.worldMaps[thisSession.currentMap]->DialogueGraphic.size())
        {
            // Invalid index, return or handle the error
            return;
        }

        const std::vector<int>& dialogueGraphic = thisSession.worldMaps[thisSession.currentMap]->DialogueGraphic;

        // Calculate the starting index of the selected image in the dialogueGraphic vector
        int startingIndex = dialogueGraphicLocator * (32 * 32 * 3);

        for (int A = 0; A < 32; A++)
        {
            for (int B = 0; B < 32; B++)
            {
                glPointSize(24);

                // Rotate the image counterclockwise by 270 degrees, flip it horizontally, and flip it vertically
                int pixel = startingIndex + (A * 32 + (31 - B)) * 3;  // Find where we are in the image ppm
                int red = dialogueGraphic[pixel + 0];    // Copy the color value
                int green = dialogueGraphic[pixel + 1];
                int blue = dialogueGraphic[pixel + 2];
               
                    glColor3ub(red, green, blue);
                    glBegin(GL_POINTS);
                    glVertex2f(B * 8 + 1550, A * 8 + 875);
                    glEnd();
                
            }
        }
    }
    else
    {
        if (dialogueEventLocator < 0 || dialogueEventLocator >= 4)
        {
            std::cout << "Error" << std::endl;// Invalid index, return or handle the error
            return;
        }

        const std::vector<int>& dialogueGraphic =DialogueGraphic;

        // Calculate the starting index of the selected image in the dialogueGraphic vector
        int startingIndex = dialogueEventLocator * (32 * 32 * 3);

        for (int A = 0; A < 32; A++)
        {
            for (int B = 0; B < 32; B++)
            {
                glPointSize(24);

                // Rotate the image counterclockwise by 270 degrees, flip it horizontally, and flip it vertically
                int pixel = startingIndex + (A * 32 + (31 - B)) * 3;  // Find where we are in the image ppm
                int red = dialogueGraphic[pixel + 0];    // Copy the color value
                int green = dialogueGraphic[pixel + 1];
                int blue = dialogueGraphic[pixel + 2];

                glColor3ub(red, green, blue);
                glBegin(GL_POINTS);
                glVertex2f(B * 8 + 1550, A * 8 + 875);
                glEnd();
            }
        }
    }
}
















void dialogue(float* fps)
{
    if (gameState == dialogueEvent)
    {
        // Check if one second has passed
        if (timeCheck > 50 && !textLengthReach) // the greater than value determines the speed of the typing 
        {
            // Increment the character counter
            numCharsRendered++;

            // Reset the time check
            timeCheck = 0;
        }
        else
        {
            // Update the time check
            timeCheck += *fps;
        }

        if (!textLengthReach)           //add a timer or button to control next
        {
            drawSquare(287, 1629, 807, 1200);

            drawDialogueImage();
            //then we draw the Square and then the text; 

            text();
        }

    }
}



void loadScreenTimerCallback(int value)
{
    // The timer callback function will be called after the specified delay
    // Proceed with the next operation after the load screen

    std::cout << "Load screen finished. Proceeding to the next operation." << std::endl;

    // Set the loadScreenState variable to false to indicate the load screen is finished
    gameState = threeDimensionalDisplay;

    // Set loadScreenTimerSet to false so that the timer can be set up again
    loadScreenTimerSet = false;

    // Redraw the display to update the screen after the load screen
    glutPostRedisplay();
}

void loadBlackScreen()
{
    drawSquare(0, screenWidth, 0, screenHeight);

    // Display the load screen content
    drawSquare(0, screenWidth, 0, screenHeight);

    // Set the timer only if it hasn't been set already
    if (!loadScreenTimerSet)
    {
        glutTimerFunc(1000, loadScreenTimerCallback, 0);
        loadScreenTimerSet = true;
    }

}

void movementLogic()
{

    //Collision Variables
    int xOffset = 0;
    int yOffset = 0;

    //The Offset values determines how a Collision will alter the Player Posistion. 

    if (playerDeltaX < 0)
    {
        xOffset = -20;
    }
    else
    {
        xOffset = 20;
    }

    if (playerDeltaY < 0)
    {
        yOffset = -20;
    }
    else
    {
        yOffset = 20;
    }

    //We Check if the Map Tile in Front or Behind the Player is Empty, is so we move them forward. 

    int currentTileX = playerX / 64.0;
    int nextTileXWithXOffset = (playerX + xOffset) / 64.0;
    int prevTileXWithXOffset = (playerX - xOffset) / 64.0;

    int currentTileY = playerY / 64.0;
    int nextTileYWithYOffset = (playerY + yOffset) / 64.0;
    int prevTileYWithYOffset = (playerY - yOffset) / 64.0;



    // Forward Movement and Forward Wall Collision
    if (Keys.moveForward == 1)
    {
        // Check if the player is moving into any of the portals
        for (const auto& position : thisSession.worldMaps[thisSession.currentMap]->positions)
        {
            int X = position[0];
            int Y = position[1];

            if (currentTileY * mapX + nextTileXWithXOffset == Y * mapX + X)                     // if there is a collision, we check it flags for the kind of Event Collision 
            {
                if (position[5] == Portal)
                {
                    gameState = FREEZE;
                    playerX = position[2] * 64;
                    playerY = position[3] * 64;
                    thisSession.currentMap = position[4]; // Update the current map index to the destination map index
                    thisSession.mapLabel->copy_label(strdup(thisSession.mapNames.at(thisSession.currentMap).c_str()));
                    drawWorldMap();
                    gameState = threeDimensionalDisplay;
                    return; // Exit the function after teleporting to prevent further movement checks
                }
                else if (position[5] == Dialogue)
                {
                    gameState = dialogueEvent;
                    std::cout << "dialogue Event2 Triggered!" << std::endl;
                    textLengthReach = false;
                    numCharsRendered = 0;
                    dialogueEventLocator = position[5];
                    return;

                }
            }
        }

        // If not teleported, perform regular movement
        if (thisSession.worldMaps[thisSession.currentMap]->newMap[currentTileY * mapX + nextTileXWithXOffset] == 0)
        {
            playerX += playerDeltaX * 0.25 * fps; // Move the player forward
           
        }

        if (thisSession.worldMaps[thisSession.currentMap]->newMap[nextTileYWithYOffset * mapX + currentTileX] == 0)
        {
            playerY += playerDeltaY * 0.25 * fps;
        }



        // If not teleported, perform regular movement
        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + nextTileXWithXOffset] == 2 || thisSession.worldMaps[thisSession.currentMap]->floorMap[nextTileYWithYOffset * mapX + currentTileX] == 2)
        {
            FootSteps(gravel);

        }

        // If not teleported, perform regular movement
        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + nextTileXWithXOffset] == 3 || thisSession.worldMaps[thisSession.currentMap]->floorMap[nextTileYWithYOffset * mapX + currentTileX] == 3)
        {
            FootSteps(water);

        }

        // If not teleported, perform regular movement
        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + nextTileXWithXOffset] == 5 || thisSession.worldMaps[thisSession.currentMap]->floorMap[nextTileYWithYOffset * mapX + currentTileX] == 5)
        {
            FootSteps(stone);

        }

        // If not teleported, perform regular movement
        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + nextTileXWithXOffset] == 6 || thisSession.worldMaps[thisSession.currentMap]->floorMap[nextTileYWithYOffset * mapX + currentTileX] == 6)
        {
            FootSteps(lava);

        }
        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + nextTileXWithXOffset] == 0 || thisSession.worldMaps[thisSession.currentMap]->floorMap[nextTileYWithYOffset * mapX + currentTileX] == 0)
        {
            FootSteps(dirt);

        }



    }

    // Backward Movement and BackWard Wall Detection
    if (Keys.moveBackward == 1)
    {
        // Check if the player is moving into any of the portals
        for (const auto& position : thisSession.worldMaps[thisSession.currentMap]->positions)
        {
            int X = position[0];
            int Y = position[1];

            if (currentTileY * mapX + prevTileXWithXOffset == Y * mapX + X)
            {
                // Teleport
                playerX = static_cast<float>(position[2]); // Convert to float and set player's X position to the portal's X position
                playerY = static_cast<float>(position[3]); // Convert to float and set player's Y position to the portal's Y position
                thisSession.currentMap = position[4]; // Update the current map index to the destination map index
                return; // Exit the function after teleporting to prevent further movement checks
            }
        }

        // If not teleported, perform regular movement
        if (thisSession.worldMaps[thisSession.currentMap]->newMap[currentTileY * mapX + prevTileXWithXOffset] == 0)
        {
            playerX -= playerDeltaX * 0.20 * fps; // Move the player backward
            std::cout << "currentTileY * mapX + prevTileXWithXOffset = " << currentTileY * mapX + prevTileXWithXOffset << std::endl;
        }

        if (thisSession.worldMaps[thisSession.currentMap]->newMap[prevTileYWithYOffset * mapX + currentTileX] == 0)
        {
            playerY -= playerDeltaY * 0.20 * fps;
            std::cout << "prevTileYWithYOffset * mapX + currentTileX = " << prevTileYWithYOffset * mapX + currentTileX << std::endl;
        }


        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + prevTileXWithXOffset] == 2 || thisSession.worldMaps[thisSession.currentMap]->floorMap[prevTileYWithYOffset * mapX + currentTileX] == 2)
        {
            FootSteps(gravel);
        }

        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + prevTileXWithXOffset] == 3 || thisSession.worldMaps[thisSession.currentMap]->floorMap[prevTileYWithYOffset * mapX + currentTileX] == 3)
        {
            FootSteps(water);
        }

        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + prevTileXWithXOffset] == 5 || thisSession.worldMaps[thisSession.currentMap]->floorMap[prevTileYWithYOffset * mapX + currentTileX] == 5)
        {
            FootSteps(stone);
        } 

        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + prevTileXWithXOffset] == 6 || thisSession.worldMaps[thisSession.currentMap]->floorMap[prevTileYWithYOffset * mapX + currentTileX] == 6)
        {
            FootSteps(lava);
        }

        if (thisSession.worldMaps[thisSession.currentMap]->floorMap[currentTileY * mapX + prevTileXWithXOffset] == 0 || thisSession.worldMaps[thisSession.currentMap]->floorMap[prevTileYWithYOffset * mapX + currentTileX] == 0)
        {
            FootSteps(dirt);
        }


    }
}


void display()
{

    if (gameState != FREEZE)
    {
        
        // Calculate frames per second
        frame2 = glutGet(GLUT_ELAPSED_TIME);
        fps = (frame2 - frame1);
        frame1 = glutGet(GLUT_ELAPSED_TIME);



        //Player Rotation 
        if (Keys.turnLeft == 1)
        {
            playerAngle += 0.08 * fps;
            playerAngle = fixAngle(playerAngle);
            playerDeltaX = cos(degToRad(playerAngle));
            playerDeltaY = -sin(degToRad(playerAngle));

            // Debugging: Print player movement info
         
        }

        if (Keys.turnRight == 1)
        {
            playerAngle -= 0.08 * fps;
            playerAngle = fixAngle(playerAngle);
            playerDeltaX = cos(degToRad(playerAngle));
            playerDeltaY = -sin(degToRad(playerAngle));

        
        }


        movementLogic();


        glutPostRedisplay();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (gameState == loadScreen)
        {

            loadBlackScreen();

        }
        else if (gameState == twoDimensionalDisplay)
        {
            // glutMouseFunc(mouseClick);
            drawMap2D();
            drawPlayer2D();

        }
        else
        {
            if (drawSolidColorTextures == false)
            {
                drawSky();                              //glitch caused by this. 
            }
            drawRays2D();
            glutMouseFunc(mouseOFF);
            if (!thisSession.worldMaps.at(thisSession.currentMap)->spriteData.empty())
            {
                drawSprite();
            }
            if (gameState == dialogueEvent)
            {
                dialogue(&fps);
            }
        }

        glutSwapBuffers();
      //  AudioLoop();
    }
}



void ButtonDown(unsigned char key, int x, int y)
{
    // Changes to Between Map and POV perspective
    if (key == 'q')
    {
        if (gameState != twoDimensionalDisplay)
        {
            gameState = twoDimensionalDisplay;
        }
        else
        {
            gameState = threeDimensionalDisplay;
        }
    }

    if (key == 'z')
    {
        if (gameState != dialogueEvent)
        {
            // Collision Variables
            int xOffset = 0;
            int yOffset = 0;

            // The Offset values determine how a Collision will alter the Player Position.
            if (playerDeltaX < 0)
            {
                xOffset = -20;
            }
            else
            {
                xOffset = 20;
            }

            if (playerDeltaY < 0)
            {
                yOffset = -20;
            }
            else
            {
                yOffset = 20;
            }

            // We Check if the Map Tile in Front or Behind the Player is Empty, if so we move them forward.
            int currentTileX = playerX / 64.0;
            int nextTileXWithXOffset = (playerX + xOffset) / 64.0;
            int currentTileY = playerY / 64.0;
            int nextTileYWithYOffset = (playerY + yOffset) / 64.0;

            for (int i = 0; i < thisSession.worldMaps.at(thisSession.currentMap)->spriteData.size(); i++)
            {
                int Y = thisSession.worldMaps.at(thisSession.currentMap)->spriteData[i].y / 64;
                int X = thisSession.worldMaps.at(thisSession.currentMap)->spriteData[i].x / 64;
                if ((currentTileY * mapX + nextTileXWithXOffset) == Y * mapX + X)
                {
                    std::cout << "dialogue Event Triggered!" << std::endl;
                    textLengthReach = false;
                    numCharsRendered = 0;

                    dialogueEventLocator = thisSession.worldMaps.at(thisSession.currentMap)->spriteData[i].dialogueMarker;
                    dialogueGraphicLocator = thisSession.worldMaps.at(thisSession.currentMap)->spriteData[i].dialogueGraphic;
                    if (thisSession.currentMap > 0)
                    {
                        dialogueGraphicLocator2 = 1;

                    }
                    gameState = dialogueEvent;
                    return; // Exit the function after triggering the dialogue event
                }
            }
        }
        else
        {
            // If we are already in dialogueEvent, pressing 'z' again will end the dialogue.
            gameState = threeDimensionalDisplay;
            // You can add any additional code you need to end the dialogue here.
            // For example, clearing the text, resetting counters, etc.
        }
    }

    if (key == 'a')
    {
        Keys.turnLeft = 1;
      
        
    }
    if (key == 'd')
    {
        Keys.turnRight = 1;

    }
    if (key == 'w')
    {
        Keys.moveForward = 1;

    }
    if (key == 's')
    {
        Keys.moveBackward = 1;

    }
    if (key == 'e') // && sp[0].state==0 will allow for key
    {
        // Open doors
        int xOffset = 0;
        if (playerDeltaX < 0)
        {
            xOffset = -25;
        }
        else
        {
            xOffset = 25;
        }
        int yOffset = 0;
        if (playerDeltaY < 0)
        {
            yOffset = -25;
        }
        else
        {
            yOffset = 25;
        }

        int currentTileX = playerX / 64.0;
        int currentTileY = playerY / 64.0;
        int nextTileXWithXOffset = (playerX + xOffset) / 64.0;
        int nextTileYWithYOffset = (playerY + yOffset) / 64.0;

        if (thisSession.worldMaps[thisSession.currentMap]->newMap[nextTileYWithYOffset * mapX + nextTileXWithXOffset] == 7)
        {
            thisSession.worldMaps[thisSession.currentMap]->newMap[nextTileYWithYOffset * mapX + nextTileXWithXOffset] = 0;
            AK::SoundEngine::PostEvent(AK::EVENTS::DOOROPEN, GAME_OBJECT_ID_THEME3);
            AudioLoop();

        }
    }

    glutPostRedisplay();
}

void ButtonUp(unsigned char key, int x, int y)
{
    if (key == 'a')
    {
        Keys.turnLeft = 0;
    }
    if (key == 'd')
    {
        Keys.turnRight = 0;
    }
    if (key == 'w')
    {
        Keys.moveForward = 0;
    }
    if (key == 's')
    {
        Keys.moveBackward = 0;
    }
    glutPostRedisplay();
}



