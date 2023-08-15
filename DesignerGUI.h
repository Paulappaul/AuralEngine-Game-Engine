#pragma once
#include "Directory.h"
#include "Aural_Maps.h"
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>

struct MapData
{
    int mapPlayerLocationX = playerX;
    int mapPlayerLocationY = playerY;

    int newMap[576] = { 0 };        // Initialize all elements to 0
    int ceilingMap[576] = { 0 };    // Initialize all elements to 0
    int floorMap[576] = { 0 };      // Initialize all elements to 0
    std::vector<std::vector<int>> positions; // Vector to store X, Y, PlayerX, PlayerY, and destination map index
    std::vector<sprite>spriteData;
    std::vector<int> ppmData;
    std::vector<int> DialogueGraphic;
    // Function to add a new position to the positions vector

    void addPosition(int x, int y, int playerX, int playerY, int destinationMapIndex, int eventFLAG)
    {
        positions.push_back({ x, y, playerX, playerY, destinationMapIndex, eventFLAG });
    }

};

struct sessionData
{

    int currentMap = 0; 

    std::vector<MapData*> worldMaps;
    std::vector <std::string> mapNames;

    textures current_Texture = Wall;
    int modifyingValue = 0;
    int modifyingIndex = 0;
    Fl_Box* mapLabel;

    void deleteWorldMaps()
    {
        for (MapData* map : worldMaps)
        {
            delete map;
        }
        worldMaps.clear();
    }

}; sessionData thisSession;


// Callback function for the Check Button
void checkButtonCallback(Fl_Widget* widget, void* data)
{
    Fl_Check_Button* checkButton = static_cast<Fl_Check_Button*>(widget);
    bool isChecked = checkButton->value();

    if (isChecked) {
        std::cout << "Check Button is checked!" << std::endl;
        // Do something when the Check Button is checked
    }
    else {
        std::cout << "Check Button is unchecked!" << std::endl;
        // Do something when the Check Button is unchecked
    }
}

// Callback function for the "Submit" button
void submitCallback(Fl_Widget* widget, void* data) 
{
    Fl_Input* inputField = static_cast<Fl_Input*>(data);
    const char* inputValue = inputField->value(); // Get the user-entered value

    // Check if the input value is a valid integer
    try {
        int intValue = std::stoi(inputValue);
        // Do something with the integer value (e.g., save it to a variable)
        std::cout << "Entered integer value: " << intValue << std::endl;
    }
    catch (const std::invalid_argument& e) {
        // Display an error message using an FLTK message box
        fl_message("Invalid input! Please enter an integer.");
    }
}



void worldMapCallback(Fl_Widget* widget, void* data)
{

    int Index = reinterpret_cast<int>(data);
    if (thisSession.current_Texture == Wall)
    {
        thisSession.worldMaps[thisSession.currentMap]->newMap[Index] = thisSession.modifyingValue;

        // Calculate the X and Y coordinates from the Index
        int X = Index % mapX;
        int Y = Index / mapX;

        // Get the button at the specified X and Y coordinates
        Fl_Button* gridBlock = gridButtons[X][Y];

        // Update the label on the button
        int mapValue = thisSession.worldMaps[thisSession.currentMap]->newMap[Index];
        std::string label = std::to_string(mapValue);
        gridBlock->copy_label(label.c_str());

        // Redraw only the specific button widget
        gridBlock->redraw();
    }
    else if (thisSession.current_Texture == Floor)
    {
        thisSession.worldMaps[thisSession.currentMap]->floorMap[Index] = thisSession.modifyingValue;

        // Calculate the X and Y coordinates from the Index
        int X = Index % mapX;
        int Y = Index / mapX;

        // Get the button at the specified X and Y coordinates
        Fl_Button* gridBlock = gridButtons[X][Y];

        // Update the label on the button
        int mapValue = thisSession.worldMaps[thisSession.currentMap]->floorMap[Index];
        std::string label = std::to_string(mapValue);
        gridBlock->copy_label(label.c_str());

        // Redraw only the specific button widget
        gridBlock->redraw();
    }
    else if (thisSession.current_Texture == Ceiling)
    {
        thisSession.worldMaps[thisSession.currentMap]->ceilingMap[Index] = thisSession.modifyingValue;

        // Calculate the X and Y coordinates from the Index
        int X = Index % mapX;
        int Y = Index / mapX;

        // Get the button at the specified X and Y coordinates
        Fl_Button* gridBlock = gridButtons[X][Y];

        // Update the label on the button
        int mapValue = thisSession.worldMaps[thisSession.currentMap]->ceilingMap[Index];
        std::string label = std::to_string(mapValue);
        gridBlock->copy_label(label.c_str());

        // Redraw only the specific button widget
        gridBlock->redraw();
    }

}

void drawWorldMap()
{
    for (int X = 0; X < mapX; X++)
    {
        for (int Y = 0; Y < mapY; Y++)
        {
            if (!gridButtons[X][Y]) // If the button doesn't exist, create it.
            {
                if (thisSession.current_Texture == Wall)
                {
                    int index = Y * mapX + X;
                    int mapValue = thisSession.worldMaps[thisSession.currentMap]->newMap[index];
                    std::string label = std::to_string(mapValue);

                    // Create the button with the appropriate label
                    Fl_Button* gridBlock = new Fl_Button(X * 24 + 24, Y * 24 + 24, 24, 24, strdup(label.c_str()));
                    gridBlock->callback(worldMapCallback, reinterpret_cast<void*>(index));

                    gridButtons[X][Y] = gridBlock;
                }
               
            }
            else
            {
                // Update the label based on the current texture
                if (thisSession.current_Texture == Wall)
                {
                    int index = Y * mapX + X;
                    int mapValue = thisSession.worldMaps[thisSession.currentMap]->newMap[index];
                    std::string label = std::to_string(mapValue);
                    gridButtons[X][Y]->copy_label(label.c_str());
                }
                else if (thisSession.current_Texture == Floor)
                {
                    int index = Y * mapX + X;
                    int mapValue = thisSession.worldMaps[thisSession.currentMap]->floorMap[index];
                    std::string label = std::to_string(mapValue);
                    gridButtons[X][Y]->copy_label(label.c_str());
                }
                else if (thisSession.current_Texture == Ceiling)
                {
                    int index = Y * mapX + X;
                    int mapValue = thisSession.worldMaps[thisSession.currentMap]->ceilingMap[index];
                    std::string label = std::to_string(mapValue);
                    gridButtons[X][Y]->copy_label(label.c_str());
                }
            }
        }
    }
}




void wallTextureCallback(Fl_Widget* widget, void* data)
{
    int texture_Index = reinterpret_cast<int>(data);
    thisSession.current_Texture = Wall;
    thisSession.modifyingValue = texture_Index;
    drawWorldMap();

    

}

void floorTextureCallback(Fl_Widget* widget, void* data)
{
    int texture_Index = reinterpret_cast<int>(data);
    thisSession.current_Texture = Floor;
    thisSession.modifyingValue = texture_Index;
    drawWorldMap();


}

void ceilingTextureCallback(Fl_Widget* widget, void* data)
{
    int texture_Index = reinterpret_cast<int>(data);
    thisSession.current_Texture = Ceiling;
    thisSession.modifyingValue = texture_Index;
    drawWorldMap();    

}



void textureButtons(int initialX, int initialY, int buttonAmount, int flag)
{

    if (flag == Wall)
    {
        for (int i = 0; i < buttonAmount; i++)
        {
            int yOffset = 30;
            std::string label = "Wall Texture " + std::to_string(i);
            Fl_Button* textureButton = new Fl_Button(initialX, initialY, 120, yOffset, strdup(label.c_str()));
            initialY += yOffset;
            textureButton->callback(wallTextureCallback, reinterpret_cast<void*>(i));
        }
    }

    if (flag == Ceiling)
    {
        for (int i = 0; i < buttonAmount; i++)
        {
            int yOffset = 30;
            std::string label = "Ceiling Texture " + std::to_string(i);
            Fl_Button* textureButton = new Fl_Button(initialX, initialY, 120, yOffset, strdup(label.c_str()));
            initialY += yOffset;
            textureButton->callback(ceilingTextureCallback, reinterpret_cast<void*>(i));
        }
    }

    if (flag == Floor)
    {
        for (int i = 0; i < buttonAmount; i++)
        {
            int yOffset = 30;
            std::string label = "Floor Texture " + std::to_string(i);
            Fl_Button* textureButton = new Fl_Button(initialX, initialY, 120, yOffset, strdup(label.c_str()));
            initialY += yOffset;
            textureButton->callback(floorTextureCallback, reinterpret_cast<void*>(i));
        }
    }

}

void intializeGUI()
{
    MapData* protoType_Map = new MapData;


    std::memcpy(protoType_Map->newMap, Map, sizeof(protoType_Map->newMap));


    std::memcpy(protoType_Map->floorMap, mapFloor, sizeof(protoType_Map->floorMap));


    std::memcpy(protoType_Map->ceilingMap, mapCeiling, sizeof(protoType_Map->ceilingMap));


    thisSession.worldMaps.push_back(protoType_Map);
}


void newWorldMap(Fl_Widget* widget, void* data)
{
    gameState = FREEZE;
    const char* newName = fl_input("Enter a new map name:", "New WorldMap");

    if (newName) // Check if the user clicked 'OK' and provided a name
    {
   
        
        std::string newMapName = newName;

        MapData* protoType_Map = new MapData;

        
        std::memcpy(protoType_Map->newMap, Map, sizeof(protoType_Map->newMap));


        std::memcpy(protoType_Map->floorMap, mapFloor, sizeof(protoType_Map->floorMap));


        std::memcpy(protoType_Map->ceilingMap, mapCeiling, sizeof(protoType_Map->ceilingMap));

        thisSession.worldMaps.push_back(protoType_Map);

        // Set the new map label and update the current map index
        thisSession.mapLabel->copy_label(newMapName.c_str());
        thisSession.currentMap = thisSession.worldMaps.size() - 1; // Set the index to the new map
        thisSession.mapNames.push_back(newMapName);

        skynew = false; // ??

        playerX = thisSession.worldMaps[thisSession.currentMap]->mapPlayerLocationX;
        playerY = thisSession.worldMaps[thisSession.currentMap]->mapPlayerLocationY;
        
        drawWorldMap();
       
      gameState = threeDimensionalDisplay;

    }
    else
    {
        // The user clicked 'Cancel' or didn't provide a valid name, handle the case accordingly
        std::cout << "New map creation canceled or no name provided." << std::endl;
    }
}

//annoying struct to pass through all our objects
struct PositionWindowData
{
    int selectedIndex; // The selected destination map index
    Fl_Input* xInput; // Pointer to the X input widget
    Fl_Input* yInput; // Pointer to the Y input widget
    Fl_Input* playerXInput; // Pointer to the PlayerX input widget
    Fl_Input* playerYInput; // Pointer to the PlayerY input widget
};


void positionInputCallback(Fl_Widget* widget, void* data)
{
    PositionWindowData* positionData = static_cast<PositionWindowData*>(data);

    // Get the current X, Y, and PlayerX, PlayerY positions
    int x = std::atoi(positionData->xInput->value());
    int y = std::atoi(positionData->yInput->value());
    int playerX = std::atoi(positionData->playerXInput->value());
    int playerY = std::atoi(positionData->playerYInput->value());

    // Get the destination map index from the custom data structure
    int destinationMapIndex = positionData->selectedIndex;

    // Pass all values to the positions vector via the addPosition function.
    std::cout << "X: " << x << ", Y: " << y << ", PlayerX: " << playerX << ", PlayerY: " << playerY << ", Destination Value: " << destinationMapIndex << std::endl;

    thisSession.worldMaps.at(thisSession.currentMap)->addPosition(x, y, playerX, playerY, destinationMapIndex, Portal);

    // Close the popup window
    Fl_Window* popUpWindow = static_cast<Fl_Window*>(widget->parent());
    popUpWindow->hide();

    // Free the memory used for the custom data structure
    delete positionData;
}


void selectButtonCallback(Fl_Widget* widget, void* data)
{
    Fl_Choice* dropdown = static_cast<Fl_Choice*>(data);
    int selectedIndex = dropdown->value();
    std::cout << "selected Choice: " << selectedIndex << std::endl;

    // Close the current popup window
    Fl_Window* popUpWindow = static_cast<Fl_Window*>(widget->parent());
    popUpWindow->hide();

    // Create the custom data structure to store the position window information
    PositionWindowData* positionData = new PositionWindowData;
    positionData->selectedIndex = selectedIndex;

    // Open a new popup window to ask for X and Y position
    Fl_Window* positionWindow = new Fl_Window(500, 500, "Portal Data Request");
    positionData->xInput = new Fl_Input(150, 50, 100, 30, "Portal X Postion:");
    positionData->yInput = new Fl_Input(150, 100, 100, 30, "Portal Y Postion:");
    positionData->playerXInput = new Fl_Input(150, 150, 100, 30, "Player X Postion:");
    positionData->playerYInput = new Fl_Input(150, 200, 100, 30, "Player Y Postion:");


    Fl_Button* positionButton = new Fl_Button(275, 100, 100, 100, "Confirm");
    positionButton->callback(positionInputCallback, positionData);
    positionData->xInput->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);
    positionData->yInput->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);
    positionData->playerXInput->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);
    positionData->playerYInput->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);
    positionWindow->end();
    positionWindow->show();
}


void portalMovement(Fl_Widget* widget, void* data)
{
    Fl_Window* popUpWindow = new Fl_Window(500, 100, "Portal");
    Fl_Choice* dropdown = new Fl_Choice(150, 35, 200, 30, "Select destination:");
    Fl_Button* selectButton = new Fl_Button(375, 35, 100, 30, "confirm");
    for (int i = 0; i < thisSession.mapNames.size(); i++)
    {
        dropdown->add(thisSession.mapNames[i].c_str());
    }

    selectButton->callback(selectButtonCallback, dropdown);

    popUpWindow->end();
    popUpWindow->show();

}


void mapBackCallback(Fl_Widget* widget, void* data)
{
    if (thisSession.currentMap - 1 >= 0) // Check if the decrement would cause a negative index
    {
        thisSession.currentMap--;

        thisSession.mapLabel->copy_label(strdup(thisSession.mapNames.at(thisSession.currentMap).c_str()));
        playerX = thisSession.worldMaps[thisSession.currentMap]->mapPlayerLocationX;
        playerY = thisSession.worldMaps[thisSession.currentMap]->mapPlayerLocationY;
        std::cout << playerX << playerY << std::endl;
        drawWorldMap();
    }
    else
    {
        std::cout << "Cannot move back. Current Request is already at the first map." << std::endl;
    }
}


void mapForwardCallback(Fl_Widget* widget, void* data)
{
    if (thisSession.currentMap + 1 < thisSession.worldMaps.size())
    {
        thisSession.currentMap++;

        // Fetch the label from the mapNames vector at the current index and update the button label
        thisSession.mapLabel->copy_label(thisSession.mapNames.at(thisSession.currentMap).c_str());

        playerX = thisSession.worldMaps[thisSession.currentMap]->mapPlayerLocationX;
        playerY = thisSession.worldMaps[thisSession.currentMap]->mapPlayerLocationY;
        std::cout << playerX << playerY << std::endl;
        drawWorldMap();
    }
    else
    {
        std::cout << "Cannot move forward. Current Request exceeds total number of Maps" << std::endl;
    }
}

//another annoying struct to pass our objects through to another function 
struct SpriteWindowData
{
    Fl_Input* typeInput; // Pointer to the Type input widget
    Fl_Input* stateInput; // Pointer to the State input widget
    Fl_Input* mapInput; // Pointer to the Map input widget
    Fl_Input* xInput; // Pointer to the X input widget
    Fl_Input* yInput; // Pointer to the Y input widget
    Fl_Input* zInput; // Pointer to the Z input widget
    Fl_Input* dialogueMarkerInput; // Pointer to the Dialogue Marker input widget
};

void confirmSpriteCallback(Fl_Widget* widget, void* data)
{

    SpriteWindowData* spriteData = static_cast<SpriteWindowData*>(data);

    // Get the values for each member variable from the input widget
    int type = std::atoi(spriteData->typeInput->value());
    int state = std::atoi(spriteData->stateInput->value());
    int map = std::atoi(spriteData->mapInput->value());
    float x = static_cast<float>(std::atof(spriteData->xInput->value()));
    float y = static_cast<float>(std::atof(spriteData->yInput->value()));
    float z = static_cast<float>(std::atof(spriteData->zInput->value()));
    int dialogueMarker = std::atoi(spriteData->dialogueMarkerInput->value());

    // Close the popup window
    Fl_Window* popUpWindow = static_cast<Fl_Window*>(widget->parent());
    popUpWindow->hide();


    // Create a new sprite and add it to the spriteData vector of the current map
    sprite newSprite;
    newSprite.type = type;
    newSprite.state = state;
    newSprite.map = map;
    newSprite.x = x *64;
    newSprite.y = y *64;
    newSprite.z = z;
    newSprite.dialogueMarker = dialogueMarker;
    
    thisSession.worldMaps.at(thisSession.currentMap)->spriteData.push_back(newSprite);

    //Cleanup!
    delete spriteData;
}


void addSpriteCallback(Fl_Widget* widget, void* data)
{
    Fl_Window* popUpWindow = new Fl_Window(500, 500, "Add Sprite to Map");
    SpriteWindowData* spriteData = new SpriteWindowData;

    // Create input widgets for each member variable of the sprite struct
    spriteData->typeInput =           new Fl_Input(150, 10, 100, 30, "Type:");
    spriteData->stateInput =          new Fl_Input(150, 50, 100, 30, "State:");
    spriteData->mapInput =            new Fl_Input(150, 90, 100, 30, "Map:");
    spriteData->xInput =              new Fl_Input(150, 130, 100, 30, "X:");
    spriteData->yInput =              new Fl_Input(150, 170, 100, 30, "Y:");
    spriteData->zInput =              new Fl_Input(150, 210, 100, 30, "Z:");
    spriteData->dialogueMarkerInput = new Fl_Input(150, 250, 100, 30, "Dialogue Marker:");

    // Create a button to confirm the sprite input
    Fl_Button* confirmButton = new Fl_Button(250, 290, 100, 30, "Confirm");
    confirmButton->callback(confirmSpriteCallback, spriteData);

    popUpWindow->end();
    popUpWindow->show();
}


//another annoying struct to smuggle our objects
struct DialogueWindowData
{
    Fl_Choice* dialogueChoice; // Pointer to the dropdown menu
    Fl_Input* xInput; // Pointer to the X input widget
    Fl_Input* yInput; // Pointer to the Y input widget
};



void confirmDialogueEventCallback(Fl_Widget* widget, void* data)
{
    DialogueWindowData* dialogueData = static_cast<DialogueWindowData*>(data);

    // Get the selected dialogue value from the dropdown menu
    int selectedValue = std::atoi(dialogueData->dialogueChoice->text());

    // Get the X and Y values from the input widgets
    int x = std::atoi(dialogueData->xInput->value());
    int y = std::atoi(dialogueData->yInput->value());

    // Close the popup window
    Fl_Window* popUpWindow = static_cast<Fl_Window*>(widget->parent());
    popUpWindow->hide();

    // Add the X and Y values to the positions vector of the current map
   thisSession.worldMaps.at(thisSession.currentMap)->addPosition(x, y, 0, 0, selectedValue, Dialogue);

    std::cout << "x: " << x << "y: " << y << std::endl;
    std::cout << "selected value: " << selectedValue << std::endl;

    delete dialogueData;
}


void dialogueEventCallback(Fl_Widget* widget, void* data)
{
    Fl_Window* popUpWindow = new Fl_Window(500, 300, "New Dialogue Event");
    DialogueWindowData* dialogueData = new DialogueWindowData;

    // Create the dropdown menu with the dialogueMap values
    dialogueData->dialogueChoice = new Fl_Choice(150, 10, 200, 30, "Select dialogue:");
    for (const auto& entry : dialogueMap)
    {
        dialogueData->dialogueChoice->add(std::to_string(entry.first).c_str());
    }

    // Create input widgets for X and Y values
    dialogueData->xInput = new Fl_Input(150, 50, 100, 30, "X:");
    dialogueData->yInput = new Fl_Input(150, 90, 100, 30, "Y:");

    // Create a button to confirm the dialogue event
    Fl_Button* confirmButton = new Fl_Button(250, 130, 100, 30, "Confirm");
    confirmButton->callback(confirmDialogueEventCallback, dialogueData);

    popUpWindow->end();
    popUpWindow->show();
}

struct smugglePlayerLocation
{
    Fl_Input* xInput;
    Fl_Input* yInput;

};

void playerLocationCallback(Fl_Widget* widget, void* data)
{
    smugglePlayerLocation* passedSmuggle = static_cast<smugglePlayerLocation*>(data);

    int x = std::atoi(passedSmuggle->xInput->value());
    int y = std::atoi(passedSmuggle->yInput->value());

    std::cout << "x: " << x << " y: " << y << std::endl;

    thisSession.worldMaps[thisSession.currentMap]->mapPlayerLocationX = x;
    thisSession.worldMaps[thisSession.currentMap]->mapPlayerLocationY = y;

    // Close the popup window
    Fl_Window* popUpWindow = static_cast<Fl_Window*>(widget->parent());
    popUpWindow->hide();

    delete passedSmuggle;
}

void setPlayerLocation(Fl_Widget* widget, void* data)
{
    smugglePlayerLocation* smuggle = new smugglePlayerLocation;

    Fl_Window* popUpWindow = new Fl_Window(500, 300, "set Player Location");

    smuggle->xInput = new Fl_Input(150, 50, 100, 30, "X:");
    smuggle->yInput = new Fl_Input(150, 90, 100, 30, "Y:");

    Fl_Button* confirmButton = new Fl_Button(250, 130, 100, 30, "Confirm");
    confirmButton->callback(playerLocationCallback, smuggle);

    popUpWindow->end();
    popUpWindow->show();
}



void dialogueRefreshCallback(Fl_Widget* widget, void* data)
{
    dialogueMap.clear();
    loadDialogueFromFile("dialogue/saveFile.txt", dialogueMap);
}

std::vector<int> loadPPM(const std::string& filename)
{
    std::vector<int> pixelValues;

    std::ifstream file(filename);
    if (!file) 
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return pixelValues;
    }

    // Skip until we find the starting curly brace
    std::string line;
    while (std::getline(file, line) && line.find("{") == std::string::npos);

    // Read the array values
    int value;
    char comma;
    while (file >> value)
    {
        pixelValues.push_back(value);
        file >> comma; // Read and discard the comma
    }

    // Skip until we find the ending curly brace
    while (std::getline(file, line) && line.find("};") == std::string::npos);

    return pixelValues;
}

void newSkyCallback(Fl_Widget* widget, void* data)
{
    // Create a file chooser
    Fl_File_Chooser fileChooser(".", "*.ppm", Fl_File_Chooser::SINGLE, "Select a PPM File");
    fileChooser.show();

    // Block program flow until the file chooser is closed
    while (fileChooser.shown()) {
        Fl::wait();
    }

    if (fileChooser.value() == nullptr) {
        // User canceled the file selection
        std::cout << "Error: File selection canceled" << std::endl;
        return;
    }

    // Get the chosen file path
    const char* filePath = fileChooser.value();
    std::cout << "Selected file: " << filePath << std::endl;

    // Load the PPM data from the chosen file
    thisSession.worldMaps[thisSession.currentMap]->ppmData = loadPPM(filePath);
    skynew = true;


}

void protoTexCallback(Fl_Widget* widget, void* data)
{
    if (!prototypeTexture)
    {
        prototypeTexture = true;
    }
    else
    {
        prototypeTexture = false;

    }
}

void solidColorCallback(Fl_Widget* widget, void* data)
{
    if (!drawSolidColorTextures)
    {
        prototypeTexture = false;
        drawSolidColorTextures = true;
    }
    else
    {
        drawSolidColorTextures = false;

    }
}

void redColorValueCallback(Fl_Widget* widget, void* data)
{
    int state = reinterpret_cast<int>(data);
    Fl_Value_Input* Widget = dynamic_cast<Fl_Value_Input*>(widget);

    if (state == Wall)
    {
       wallRed = Widget->value();

    }
    else if (state == Floor)
    {
        groundRed = Widget->value();

    }
    else if (state == Ceiling)
    {
        ceilingRed =  Widget->value();

    }

}

void greenColorValueCallback(Fl_Widget* widget, void* data)
{
    int state = reinterpret_cast<int>(data);
    Fl_Value_Input* Widget = dynamic_cast<Fl_Value_Input*>(widget);

    if (state == Wall)
    {
        wallGreen = Widget->value();

    }
    else if (state == Floor)
    {
        groundGreen = Widget->value();

    }
    else if (state == Ceiling)
    {
        ceilingGreen = Widget->value();

    }

}

void blueColorValueCallback(Fl_Widget* widget, void* data)
{
    int state = reinterpret_cast<int>(data);
    Fl_Value_Input* Widget = dynamic_cast<Fl_Value_Input*>(widget);

    if (state == Wall)
    {
        wallBlue = Widget->value();

    }
    else if (state == Floor)
    {
        groundBlue= Widget->value();

    }
    else if (state == Ceiling)
    {
        ceilingBlue= Widget->value();

    }

}

void saveData(const std::string& filename)
{
    std::ofstream outFile(filename);

    if (!outFile)
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    // Write the size of worldMaps vector
    size_t numWorldMaps = thisSession.worldMaps.size();
    outFile << numWorldMaps << std::endl;

    // Iterate through worldMaps vector
    for (size_t mapIndex = 0; mapIndex < numWorldMaps; ++mapIndex)
    {
        // Write mapNames corresponding to the current map
        outFile << thisSession.mapNames.at(mapIndex) << std::endl;

        // Write MapData struct
        MapData* mapData = thisSession.worldMaps.at(mapIndex);
        outFile.write(reinterpret_cast<const char*>(mapData), sizeof(MapData));
        outFile << std::endl;
    }

    outFile.close();
}


void saveButtonCallback(Fl_Widget* widget, void* Data)
{

    const std::string fileName =  "savefiles/savefile.txt";
    saveData(fileName);

}

void loadData(const std::string& filename)
{
    std::ifstream inFile(filename);

    if (!inFile)
    {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }

    thisSession.worldMaps.clear();
    thisSession.mapNames.clear();

    size_t numWorldMaps;
    inFile >> numWorldMaps;
    inFile.ignore(); // Ignore the newline character after the number

    for (size_t i = 0; i < numWorldMaps; ++i)
    {
        std::string mapName;
        std::getline(inFile, mapName);

        MapData* mapData = new MapData();
        inFile.read(reinterpret_cast<char*>(mapData), sizeof(MapData));
        inFile.ignore(); // Ignore the newline character after the struct data

        thisSession.worldMaps.push_back(mapData);
        thisSession.mapNames.push_back(mapName);
    }

    inFile.close();
    drawWorldMap();
}


void loadButtonCallback(Fl_Widget* widget, void* data)
{
    std::string fileName = "savefiles/savefile.txt";
    loadData(fileName);
}

void designerGui()
{
    const std::string boxLabel = "World Map";

    intializeGUI();
    Fl_Window* fltkWindow = new Fl_Window(1100, 800, "AuralEngine Editor");
    Fl_Box* MapLabel = new Fl_Box(24, 600, 576, 75, "World Map");
    MapLabel->box(FL_THIN_UP_BOX);

    thisSession.mapLabel = MapLabel;

    thisSession.mapNames.push_back(boxLabel);

    drawWorldMap();

    textureButtons(600, 24, 9, Wall);
    textureButtons(720, 24, 9, Ceiling);
    textureButtons(840, 24, 9, Floor);

    Fl_Button* newMap = new Fl_Button(216, 675, 192, 75, "New Map");
    newMap->callback(newWorldMap);

    Fl_Button* mapBack = new Fl_Button(24, 675, 192, 75, "@<-");
    mapBack->callback(mapBackCallback);
    Fl_Button* mapForward = new Fl_Button(408, 675, 192, 75, "@->");
    mapForward->callback(mapForwardCallback);

    Fl_Box* AuralEngineLogo = new Fl_Box(625, 675, 451, 75, "AURAL ENGINE");
    AuralEngineLogo->labelsize(36);
    AuralEngineLogo->box(FL_DOWN_FRAME);
    AuralEngineLogo->labelfont(FL_TIMES_BOLD);
        
    Fl_Button* add_Portal = new Fl_Button(960, 24, 120, 30, "Add Portal");
    add_Portal->callback(portalMovement);

    Fl_Button* playerLocation = new Fl_Button(960, 54, 120, 30, "Player Location");
    playerLocation->callback(setPlayerLocation);

    Fl_Button* newText = new Fl_Button(960, 84, 120, 30, "Dialogue Event");
    newText->callback(dialogueEventCallback);

    Fl_Button* addSprite = new Fl_Button(960, 114, 120, 30, "Add Sprite");
    addSprite->callback(addSpriteCallback);

    Fl_Button* dialogueRefresh = new Fl_Button(960,144, 120,30, "Dialogue Refresh");
    dialogueRefresh->callback(dialogueRefreshCallback);

    Fl_Button* newSkyButton = new Fl_Button(960,174,120,30, "New Sky");
    newSkyButton->callback(newSkyCallback);


    //
    Fl_Button* protoTyping_Textures = new Fl_Button(720, 294, 240, 30, "Prototyping Textures");
    protoTyping_Textures->callback(protoTexCallback);

    Fl_Button* solidColorTextures = new Fl_Button(960, 294, 120, 30, "Solid Color");
    solidColorTextures->callback(solidColorCallback);    

    //SKY
    Fl_Box* Skylabel = new Fl_Box(600, 324, 120, 30, "SKY:");

    Fl_Value_Input* redInput = new Fl_Value_Input(720, 324, 120, 30);
    redInput->label("RED");
    redInput->align(FL_ALIGN_BOTTOM);
    redInput->soft(true);
    redInput->callback(redColorValueCallback, reinterpret_cast<void*>(2));

    Fl_Value_Input* blueInput = new Fl_Value_Input(840, 324, 120, 30);
    blueInput->label("BLUE");
    blueInput->align(FL_ALIGN_BOTTOM);
    blueInput->callback(blueColorValueCallback, reinterpret_cast<void*>(2));

    Fl_Value_Input* greenInput = new Fl_Value_Input(960, 324, 120, 30);
    greenInput->label("GREEN");
    greenInput->align(FL_ALIGN_BOTTOM);
    greenInput->callback(greenColorValueCallback, reinterpret_cast<void*>(2));

    //Wall
    Fl_Box* wallLabel = new Fl_Box(600, 354, 120, 30, "WALL:");

    Fl_Value_Input* wredInput = new Fl_Value_Input(720, 354, 120, 30);
    wredInput->label("RED");
    wredInput->align(FL_ALIGN_BOTTOM);
    wredInput->callback(redColorValueCallback, reinterpret_cast<void*>(0));

    Fl_Value_Input* wblueInput = new Fl_Value_Input(840, 354, 120, 30);
    wblueInput->label("BLUE");
    wblueInput->align(FL_ALIGN_BOTTOM);
    wblueInput->callback(blueColorValueCallback, reinterpret_cast<void*>(0));

    Fl_Value_Input* wgreenInput = new Fl_Value_Input(960, 354, 120, 30);
    wgreenInput->label("GREEN");
    wgreenInput->align(FL_ALIGN_BOTTOM);
    wgreenInput->callback(greenColorValueCallback, reinterpret_cast<void*>(0));

    //Floor
    Fl_Box* floorLabel = new Fl_Box(600, 294, 120, 30, "FLOOR:");

    Fl_Value_Input* fredInput = new Fl_Value_Input(720, 384, 120, 30);
    fredInput->label("RED");
    fredInput->align(FL_ALIGN_BOTTOM);
    fredInput->callback(redColorValueCallback);
    fredInput->callback(redColorValueCallback, reinterpret_cast<void*>(1));

    Fl_Value_Input* fblueInput = new Fl_Value_Input(840, 384, 120, 30);
    fblueInput->label("BLUE");
    fblueInput->align(FL_ALIGN_BOTTOM);
    fblueInput->callback(blueColorValueCallback, reinterpret_cast<void*>(1));

    Fl_Value_Input* fgreenInput = new Fl_Value_Input(960, 384, 120, 30);
    fgreenInput->label("GREEN");
    fgreenInput->align(FL_ALIGN_BOTTOM);
    fgreenInput->callback(greenColorValueCallback, reinterpret_cast<void*>(1));

    //save and load
    Fl_Button* saveButton = new Fl_Button(960, 500, 120, 30, "save");
    saveButton->callback(saveButtonCallback);

    Fl_Button* loadButton = new  Fl_Button(720, 500, 120, 30, "load");
    loadButton->callback(loadButtonCallback);

    fltkWindow->end();
    fltkWindow->show();
    Fl::run();
}