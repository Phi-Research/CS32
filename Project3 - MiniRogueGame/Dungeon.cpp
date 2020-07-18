
#include <iostream>
#include <string>
#include <cmath>


#include "utilities.h"
#include "Dungeon.h"
#include "Actor.h"
#include "GameObject.h"
#include "Game.h"

//#include <list>
//
//using namespace std;

Dungeon::Dungeon()
{

}

Dungeon::~Dungeon()
{}

// ==============================================================================================================================================================================
// ==============================================================================================================================================================================

Level::Level()
    : m_levelNumber(0), m_monsterUpperLimit(6)
{

}

Level::Level(int levelNumber, Player* player, Game* game  )
    : m_levelNumber(levelNumber), m_monsterUpperLimit(5 * (levelNumber + 1) + 1)
    , m_levelToPlayerPtr(player)
    , m_levelToGamePtr(game)
    ,m_M((randInt(2, 5 * (levelNumber + 1) + 1))), m_S(0), m_G(0), m_B(0), m_D(0)
{
    // M = (randInt(2, 5*(levelNum+1) + 1) )
    
    constructNextLevel(levelNumber, m_M, player, game   );

}

void Level::constructNextLevel(int levelNumber, int m_M, Player* player, Game* game )
{
    //cerr << "### constructLevel0() start ###" << endl;
    //cerr << "m_M: " << m_M << endl;         

    // Set LEVEL's pointers
        // Set THIS level's pointer to player
            // ALREADY set by Level constructor
                //m_levelToPlayerPtr = player;

        // Set THIS level's pointer to GAME
            // Already set by constructor

    // Set PLAYER's pointers
        // Set player's pointer to THIS level
            m_levelToPlayerPtr->getPlayerCurrLevelPtr() = this;
        // Player ALREADY has pointer to game assigned upon player construction by game -  m_playerToGamePtr
        // Player's weapon ptr ALREADY assigned by player constructor

    // Set GAME's pointers
        // Game already has pointer to player

        // ### AT END OF THIS CONSTRUCTION ###
        // AFTER printing THIS level
        // Destroy old level - using game's pointer to currLevel
            //m_levelToGamePtr->deleteCurrLevel();

        // Set Game's pointer to currLevel to THIS level
            //m_levelToGamePtr->setLevelptr(this);   

    
    // --- For testing only ------------------------------------------------
    // Set Player's level0 element in lvlPtrArray to THIS ptr
    //*(m_levelToPlayerPtr->getLvlPtrArrayPtr() + levelNumber) = this;
    // ------------------------------------------------------------------------
    
// === Create Map ===

     //Fill level entirely with wall
    for (int r = 0; r < 18; r++) {
        for (int c = 0; c < 70; c++) {
            m_map[r][c] = '#';
        }
    }

    // Create varibles to used
    int upperRowBound = 0;
    int lowerRowBound = 0;
    int leftColBound = 0;
    int rightColBound = 0;

    int UPPER_ROW_LIMIT = 0;
    int LOWER_ROW_LIMIT = 18;
    int LEFT_COL_LIMIT = 0;
    int RIGHT_COL_LIMIT = 70;    

    int ROW_MAX = 17;
    int COL_MAX = 69;

    // Spawn player at random location
    PlayerNode playerSpawn;
    playerSpawn.playerChar = '@';
    playerSpawn.playerRow = randInt(2, 16);
    playerSpawn.playerCol = randInt(2, 68);
    playerSpawn.playerPtr = player;

    //bool validSpawn = false;
    //while (!validSpawn)
    //{
    //// ###### OPTIONAL - USE THESE COORDINATES IF WANT TO RESTRICT PLAYER TO SPAWNING ONLY AROUND PERIPHERY ######
    //    CoordinatePair playerPerimeterSpawn;
    //        playerPerimeterSpawn.row = randInt(2, 16);
    //        playerPerimeterSpawn.col = randInt(2, 68);
    //    // Player can spawn around periphery        
    //    bool playerWithinUpperLowerBounds = false;
    //    if (   (playerPerimeterSpawn.row > 14 && playerPerimeterSpawn.row < 18)
    //        || (playerPerimeterSpawn.row > 0 && playerPerimeterSpawn.row < 5)    )
    //    {
    //        playerWithinUpperLowerBounds = true;
    //    }
    //    bool playerWithinLeftRightBounds = false;
    //    if (   (playerPerimeterSpawn.col > 0 && playerPerimeterSpawn.col < 5)
    //        || (playerPerimeterSpawn.col > 65 && playerPerimeterSpawn.col < 70)     )
    //    {
    //        playerWithinLeftRightBounds = true;
    //    }

    //    if (playerWithinUpperLowerBounds && playerWithinLeftRightBounds)
    //    {
    //        validSpawn = true;
    //        playerSpawn.playerRow = playerPerimeterSpawn.row;
    //        playerSpawn.playerCol = playerPerimeterSpawn.col;
    //    }
    //}
    // ###### PERIPHERY SPAWN END ######
    addNodeToPlayerStateHistory(playerSpawn);
    m_map[playerSpawn.playerRow][playerSpawn.playerCol] = '@';

    GameObjectNode descend;
    // Generate Descend icon placed at random location in level
    descend.itemRow = ROW_MAX - playerSpawn.playerRow; // OPPOSITE of player spawn
    descend.itemCol = COL_MAX - playerSpawn.playerCol; 
    descend.itemChar = '>';
    descend.itemPtr = new Descend();
    addNodeToGameObjectList(descend);
    m_map[descend.itemRow][descend.itemCol] = '>';

    // Test pathExists
    // Make COPY of level's map for pathExists to be able to mark positions as VISITED
        // *** for some reason, copying the map INSIDE pathExists creates a stack overflow error
    //char tempMap[18][70];
    //for (int r = 0; r < 18; r++)
    //{
    //    for (int c = 0; c < 70; c++)
    //    {
    //        tempMap[r][c] = m_map[r][c];
    //    }
    //}
    bool pathPlayerToDescendExists = false;
    //pathPlayerToDescendExists = pathExists(tempMap, playerSpawn.playerRow, playerSpawn.playerCol, descend.itemRow, descend.itemCol);
    //cerr << "pathPlayerToDescendExists: " << pathPlayerToDescendExists << endl;

    // Make Room around player
    for (int r = upperRowBound; r < lowerRowBound; r++)
    {
        for (int c = leftColBound; c < rightColBound; c++)
        {
            m_map[r][c] = ' ';
        }
    }


    int count = 0; // count for testing purposes

     //If path DOESNT exist - generate another room
    while ( ! pathPlayerToDescendExists)
    {
        //cerr << "--- count: " << count << " --- " << endl;

        // Make room around player spawn
        upperRowBound = playerSpawn.playerRow - randInt(3, 6);
        lowerRowBound = playerSpawn.playerRow + randInt(3, 6);
        leftColBound = playerSpawn.playerCol - randInt(3, 6);
        rightColBound = playerSpawn.playerCol + randInt(3, 6);
        correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
        // Put room on map
        for (int r = upperRowBound; r < lowerRowBound; r++)
        {
            for (int c = leftColBound; c < rightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }

        // Make cooridor CROSS centered at player spawn
        // Make horizontal part of player cross
        int cooridorUpperRowBound = playerSpawn.playerRow;
        int cooridorLowerRowBound = playerSpawn.playerRow + 1;
        int cooridorLeftColBound = 0;
        int cooridorRightColBound = 70;
        // Put horizontal cooridor on map
        correctBounds(cooridorUpperRowBound, cooridorLowerRowBound, cooridorLeftColBound, cooridorRightColBound);
        for (int r = cooridorUpperRowBound; r < cooridorLowerRowBound; r++)
        {
            for (int c = cooridorLeftColBound; c < cooridorRightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }
        // Make small rooom on end of horiztonal cooridor
        int roomCenterRow = cooridorUpperRowBound;
        int roomCenterCol = cooridorLeftColBound;
        upperRowBound = roomCenterRow - randInt(2, 3);
        lowerRowBound = roomCenterRow + randInt(2, 3);
        leftColBound = roomCenterCol - randInt(2, 3);
        rightColBound = roomCenterCol + randInt(2, 3);
        correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
        // Put room on map
        for (int r = upperRowBound; r < lowerRowBound; r++)
        {
            for (int c = leftColBound; c < rightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }

        // Make vertical part of player cross
        cooridorUpperRowBound = 0;
        cooridorLowerRowBound = 18;
        cooridorLeftColBound = playerSpawn.playerCol;
        cooridorRightColBound = playerSpawn.playerCol + 1;
        correctBounds(cooridorUpperRowBound, cooridorLowerRowBound, cooridorLeftColBound, cooridorRightColBound);
        // Put vertical cooridor on map
        for (int r = cooridorUpperRowBound; r < cooridorLowerRowBound; r++)
        {
            for (int c = cooridorLeftColBound; c < cooridorRightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }
        // Make small rooom on end of vertical cooridor
        roomCenterRow = cooridorUpperRowBound;
        roomCenterCol = cooridorLeftColBound;
        upperRowBound = roomCenterRow - randInt(2, 3);
        lowerRowBound = roomCenterRow + randInt(2, 3);
        leftColBound = roomCenterCol - randInt(2, 3);
        rightColBound = roomCenterCol + randInt(2, 3);
        correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
        // Put room on map
        for (int r = upperRowBound; r < lowerRowBound; r++)
        {
            for (int c = leftColBound; c < rightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }


        // Make room around descend location
        upperRowBound = descend.itemRow - randInt(3, 6);
        lowerRowBound = descend.itemRow + randInt(3, 6);
        leftColBound = descend.itemCol - randInt(3, 6);
        rightColBound = descend.itemCol + randInt(3, 6);
        correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
        // Put room on map
        for (int r = upperRowBound; r < lowerRowBound; r++)
        {
            for (int c = leftColBound; c < rightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }
        // Make cooridor CROSS centered at descend point
        // Make horizontal part of descend cross
        cooridorUpperRowBound = descend.itemRow;
        cooridorLowerRowBound = descend.itemRow + 1;
        cooridorLeftColBound = 0;
        cooridorRightColBound = 70;
        // Put horizontal cooridor on map
        correctBounds(cooridorUpperRowBound, cooridorLowerRowBound, cooridorLeftColBound, cooridorRightColBound);
        for (int r = cooridorUpperRowBound; r < cooridorLowerRowBound; r++)
        {
            for (int c = cooridorLeftColBound; c < cooridorRightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }
        // Make small rooom on end of horizontal cooridor
        roomCenterRow = cooridorUpperRowBound;
        roomCenterCol = cooridorLeftColBound;
        upperRowBound = roomCenterRow - randInt(2, 3);
        lowerRowBound = roomCenterRow + randInt(2, 3);
        leftColBound = roomCenterCol - randInt(2, 3);
        rightColBound = roomCenterCol + randInt(2, 3);
        correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
        // Put room on map
        for (int r = upperRowBound; r < lowerRowBound; r++)
        {
            for (int c = leftColBound; c < rightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }
        // Make vertical part of descend cross
        cooridorUpperRowBound = 0;
        cooridorLowerRowBound = 18;
        cooridorLeftColBound = descend.itemCol;
        cooridorRightColBound = descend.itemCol + 1;
        correctBounds(cooridorUpperRowBound, cooridorLowerRowBound, cooridorLeftColBound, cooridorRightColBound);
        // Put vertical cooridor on map
        for (int r = cooridorUpperRowBound; r < cooridorLowerRowBound; r++)
        {
            for (int c = cooridorLeftColBound; c < cooridorRightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }
        // Make rooom on end of vertical cooridor        
        roomCenterRow = cooridorUpperRowBound;
        roomCenterCol = cooridorLeftColBound;
        upperRowBound = roomCenterRow - randInt(2, 3);
        lowerRowBound = roomCenterRow + randInt(2, 3);
        leftColBound = roomCenterCol - randInt(2, 3);
        rightColBound = roomCenterCol + randInt(2, 3);
        correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
        // Put room on map
        for (int r = upperRowBound; r < lowerRowBound; r++)
        {
            for (int c = leftColBound; c < rightColBound; c++)
            {
                m_map[r][c] = ' ';
            }
        }




        // Make small rooms at end of one of the cross cooridors


        // === Generate Random Rooms ===
        int numberOfRooms = randInt(3, 4);
        char northSouth = 'X';
        char westEast = 'X';
        int verticalCooridor = 1;
        int prevRoomCenterRow = randInt(2, 16);
        int prevRoomCenterCol = randInt(2, 68);

        for (int i = 0; i < numberOfRooms; i++)
        {
            // Random midpoint
            roomCenterRow = ROW_MAX - prevRoomCenterRow;
            roomCenterCol = COL_MAX - prevRoomCenterCol;

            // The rooms now become marked "previous" for use in next iteration
            prevRoomCenterRow = roomCenterRow;
            prevRoomCenterCol = roomCenterCol;

            // Make room around midpoint
            upperRowBound = roomCenterRow - randInt(2, 6);
            lowerRowBound = roomCenterRow + randInt(2, 6);
            leftColBound = roomCenterCol - randInt(2, 6);
            rightColBound = roomCenterCol + randInt(2, 6);
            correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
            // Put room on map
            for (int r = upperRowBound; r < lowerRowBound; r++)
            {
                for (int c = leftColBound; c < rightColBound; c++)
                {
                    m_map[r][c] = ' ';
                }
            }

            // Test for direction TOWARDS DESCEND from ROOM's MIDPOINT
            if ((descend.itemRow - roomCenterRow) > 0)
            {   // Descend point is NORTH of MIDPOINT
                northSouth = 'N';
            }
            if ( (descend.itemRow - roomCenterRow) < 0 )
            {   // Descend point is SOUTH of MIDPOINT
                northSouth = 'S';
            }
            if ((descend.itemRow - roomCenterRow) == 0)
            {   // Descend point == START MIDPOINT
                northSouth = '0';
            }

            if ( (descend.itemCol - roomCenterCol) > 0 )
            { // Descend point is EAST of MIDPOINT
                westEast = 'E';
            }
            if ((descend.itemCol - roomCenterCol) < 0)
            { // Descend point is WEST of MIDPOINT
                westEast = 'W';
            }
            if ((descend.itemCol - roomCenterCol) == 0)
            { // Descend point == MIDPOINT
                westEast = '0';
            }            

            int cooridorLength = randInt(5, 70); // Random length
                   
            cooridorLength = randInt(10, 20); // Random length
            // Choose vertical OR horizontal cooridor - 50/50 chance
            double p = 0.5;
            verticalCooridor = trueWithProbability(p);

            if (verticalCooridor)
            {
                switch (northSouth)
                {
                case 'N': // Vertical cooridor extending from NORTH wall
                    cooridorUpperRowBound = upperRowBound - cooridorLength;
                    cooridorLowerRowBound = upperRowBound;

                    cooridorLeftColBound = randInt(leftColBound, rightColBound);
                    cooridorRightColBound = cooridorLeftColBound + 1;
                    break;

                case 'S': // Vertical cooridor extending from SOUTH wall
                    cooridorUpperRowBound = lowerRowBound + cooridorLength;
                    cooridorLowerRowBound = lowerRowBound;

                    cooridorLeftColBound = randInt(leftColBound, rightColBound);
                    cooridorRightColBound = cooridorLeftColBound + 1;
                    break;
                }
                // Put vertical cooridor on map
                correctBounds(cooridorUpperRowBound, cooridorLowerRowBound, cooridorLeftColBound, cooridorRightColBound);
                for (int r = cooridorUpperRowBound; r < cooridorLowerRowBound; r++)
                {
                    for (int c = cooridorLeftColBound; c < cooridorRightColBound; c++)
                    {
                        m_map[r][c] = ' ';
                    }
                }

                // Make rooom on end of vertical cooridor
                roomCenterRow = cooridorUpperRowBound;
                roomCenterCol = cooridorLeftColBound;
                upperRowBound = roomCenterRow - randInt(2, 5);
                lowerRowBound = roomCenterRow + randInt(2, 5);
                leftColBound = roomCenterCol - randInt(2, 5);
                rightColBound = roomCenterCol + randInt(2, 5);
                correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
                // Put room on map
                for (int r = upperRowBound; r < lowerRowBound; r++)
                {
                    for (int c = leftColBound; c < rightColBound; c++)
                    {
                        m_map[r][c] = ' ';
                    }
                }

            } // END VERTICAL COORDIOR IF

            if (!verticalCooridor)
            {

                // Attach horiztonal cooridor to room
                switch (westEast)
                {
                case 'W': // Horizontal cooridor extending from room's EAST wall
                    cooridorUpperRowBound = randInt(upperRowBound, lowerRowBound);
                    cooridorLowerRowBound = cooridorUpperRowBound + 1;

                    cooridorLeftColBound = rightColBound;
                    cooridorRightColBound = rightColBound + cooridorLength;
                    break;

                case 'E': // Horizontal cooridor extending from WEST wall
                    cooridorUpperRowBound = randInt(upperRowBound, lowerRowBound);
                    cooridorLowerRowBound = cooridorUpperRowBound + 1;

                    cooridorLeftColBound = leftColBound - cooridorLength;
                    cooridorRightColBound = leftColBound;
                    break;
                }
                // Put horizontal cooridor on map
                correctBounds(cooridorUpperRowBound, cooridorLowerRowBound, cooridorLeftColBound, cooridorRightColBound);
                for (int r = cooridorUpperRowBound; r < cooridorLowerRowBound; r++)
                {
                    for (int c = cooridorLeftColBound; c < cooridorRightColBound; c++)
                    {
                        m_map[r][c] = ' ';
                    }
                }
                // Make rooom on end of horizontal cooridor
                roomCenterRow = cooridorUpperRowBound;
                roomCenterCol = cooridorLeftColBound;
                upperRowBound = roomCenterRow - randInt(2, 5);
                lowerRowBound = roomCenterRow + randInt(2, 5);
                leftColBound = roomCenterCol - randInt(2, 5);
                rightColBound = roomCenterCol + randInt(2, 5);
                correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
                // Put room on map
                for (int r = upperRowBound; r < lowerRowBound; r++)
                {
                    for (int c = leftColBound; c < rightColBound; c++)
                    {
                        m_map[r][c] = ' ';
                    }
                }

            } // END HORIZONTAL COORIDOR IF

        }// END ROOM GENERATING FOR LOOP

        // ReMark player and descend in case they got overwritten 
        m_map[playerSpawn.playerRow][playerSpawn.playerCol] = '@';
        m_map[descend.itemRow][descend.itemCol] = '>';
        //printMap();
        //for (int r = 0; r < 18; r++) {
        //    for (int c = 0; c < 70; c++) {
        //        cout << m_map[r][c];
        //    }
        //    cout << "row: " << r << endl;
        //}

        // Make copy of Level map for pathExists function to use
        char tempMap[18][70];
        for (int r = 0; r < 18; r++)
        {
            for (int c = 0; c < 70; c++)
            {
                tempMap[r][c] = m_map[r][c];
            }
        }
        // Test if path from player to descend exists
        pathPlayerToDescendExists = pathExists(tempMap, playerSpawn.playerRow, playerSpawn.playerCol, descend.itemRow, descend.itemCol);

         //Replace player and decend icons on map if they've bene overwritten
        m_map[playerSpawn.playerRow][playerSpawn.playerCol] = '@';
        m_map[descend.itemRow][descend.itemCol] = '>';

        if (levelNumber == 4)
        {   // Generate Golden Idol icon placed at random location in level
            GameObjectNode goldenIdol;
            goldenIdol.itemRow = descend.itemRow;
            goldenIdol.itemCol = descend.itemCol;
            goldenIdol.itemChar = '&';
            goldenIdol.itemPtr = new GoldenIdol();

            delete descend.itemPtr;
            descend.itemPtr = nullptr;
            descend.itemRow = 0;
            descend.itemRow = 0;
            descend.itemChar = '#';

            // MUST ERASRE to work
            //deleteItemFromGameObjectList(descend.itemRow, descend.itemCol);

            addNodeToGameObjectList(goldenIdol);
            m_map[goldenIdol.itemRow][goldenIdol.itemCol] = '&';
        }

        count++; // count is for testing purposes only
    }

    // Repalce peripheral walls if they've been overwritten
    // Upper peripheral wall
    upperRowBound = 0;
    lowerRowBound = 1;
    leftColBound = 0;
    rightColBound = 70;
    for (int r = upperRowBound; r < lowerRowBound; r++) {
        for (int c = leftColBound; c < rightColBound; c++) {
            m_map[r][c] = '#';
        }
    }
    // Bottom peripheral wall
    upperRowBound = 17;
    lowerRowBound = 18;
    leftColBound = 0;
    rightColBound = 70;
    for (int r = upperRowBound; r < lowerRowBound; r++) {
        for (int c = leftColBound; c < rightColBound; c++) {
            m_map[r][c] = '#';
        }
    }
    // Left peripheral wall
    upperRowBound = 0;
    lowerRowBound = 18;
    leftColBound = 0;
    rightColBound = 1;
    for (int r = upperRowBound; r < lowerRowBound; r++) {
        for (int c = leftColBound; c < rightColBound; c++) {
            m_map[r][c] = '#';
        }
    }
    // Right peripheral wall
    upperRowBound = 0;
    lowerRowBound = 18;
    leftColBound = 69;
    rightColBound = 70;
    for (int r = upperRowBound; r < lowerRowBound; r++) {
        for (int c = leftColBound; c < rightColBound; c++) {
            m_map[r][c] = '#';
        }
    }

// ###################################################
// ############# MONSTER GENERATION ##################
    // ACTOR LISTS ARE UPDATED AFTER EVERY COMMAND   
    // Each generated monster get it's own MonsterNode - containts monster data and POINTER to dynamic monster object
    // Each monster node is pushed onto the level's Monster List

    // Set Level's idiosyncratic monster population weights    
    switch (levelNumber)
    {
    case 0 :
        m_S = 0.8 * m_M;
        m_G = 0.2 * m_M;
        m_B = 0.0 * m_M;
        m_D = 0.0 * m_M;
        break;

    case 1 :
        m_S = 0.5 * m_M;
        m_G = 0.5 * m_M;
        m_B = 0.0 * m_M;
        m_D = 0.0 * m_M;
        break;

    case 2 :
        m_S = 0.2 * m_M;
        m_G = 0.1 * m_M;
        m_B = 0.7 * m_M;
        m_D = 0.0 * m_M;
        break;

    case 3 :
        m_S = 0.1 * m_M;
        m_G = 0.7 * m_M;
        m_B = 0.1 * m_M;
        m_D = 0.1 * m_M;
        break;

    case 4 :
        m_S = 0.1 * m_M;
        m_G = 0.1 * m_M;
        m_B = 0.4 * m_M;
        m_D = 0.4 * m_M;
        break;
    }
    if (levelNumber > 4)
    {
        m_S = 0.25*m_M;
        m_G = 0.25*m_M;
        m_B = 0.25*m_M;
        m_D = 0.25*m_M;
    }
    int numberOfScrolls = randInt(1, 2);
    //numberOfScrolls = 50; //*************************** 
    int numerOfWeapons = randInt(1, 2);

    // Test Level
    //m_S = 2 * m_M;
    //m_G = 2 * m_M;
    //m_B = 2 * m_M;
    //m_D = 2 * m_M;

    // Generate Snakewomen
    string monsterType = "Snakewoman";
    string monsterID = "";
    for (int i = 0; i < m_S; i++) {
        //MonsterNode newBornMonster;
        MonsterNode newBornMonster;

        // Generate monster identifiers
        string istring = to_string(i);
        monsterID = monsterType + istring;
        newBornMonster.monsterID = monsterID;
        newBornMonster.monsterChar = 'S';

        // Generate monster spawn position
        CoordinatePair randomSpawnPos = generateRandEmptyPos();
        newBornMonster.monsterRow = randomSpawnPos.row;
        newBornMonster.monsterCol = randomSpawnPos.col;        

        // Allocate dynamic monster object
        newBornMonster.smellDistance = 3;
        newBornMonster.monsterPtr = new Snakewoman();

        // Push new monster to Level's Monster List
        addNodeToMonsterList(newBornMonster);
    }

    // Generate Goblins
    monsterType = "Goblin";
    monsterID = "";
    for (int i = 0; i < m_G; i++) {
        //MonsterNode newBornMonster;
        MonsterNode newBornMonster;

        // Generate monster identifiers
        string istring = to_string(i);
        monsterID = monsterType + istring;
        newBornMonster.monsterID = monsterID;
        newBornMonster.monsterChar = 'G';

        // Generate monster spawn position
        CoordinatePair randomSpawnPos = generateRandEmptyPos();
        newBornMonster.monsterRow = randomSpawnPos.row;
        newBornMonster.monsterCol = randomSpawnPos.col;

        // Allocate dynamic monster object
        newBornMonster.smellDistance = m_levelToGamePtr->getGoblinSmellDist();
        newBornMonster.monsterPtr = new Goblin();

        // Push new monster to Level's Monster List
        addNodeToMonsterList(newBornMonster);
    }

    // Generate Bogeymen
    monsterType = "Bogeyman";
    monsterID = "";
    for (int i = 0; i < m_B; i++) {
        //MonsterNode newBornMonster;
        MonsterNode newBornMonster;

        // Generate monster identifiers
        string istring = to_string(i);
        monsterID = monsterType + istring;
        newBornMonster.monsterID = monsterID;
        newBornMonster.monsterChar = 'B';

        // Generate monster spawn position
        CoordinatePair randomSpawnPos = generateRandEmptyPos();
        newBornMonster.monsterRow = randomSpawnPos.row;
        newBornMonster.monsterCol = randomSpawnPos.col;

        // Allocate dynamic monster object
        newBornMonster.smellDistance = 5;
        newBornMonster.monsterPtr = new Bogeyman();

        // Push new monster to Level's Monster List
        addNodeToMonsterList(newBornMonster);
    }

    // Generate Dragons
    monsterType = "Dragon";
    monsterID = "";
    for (int i = 0; i < m_D; i++) {
        //MonsterNode newBornMonster;
        MonsterNode newBornMonster;

        // Generate monster identifiers
        string istring = to_string(i);
        monsterID = monsterType + istring;
        newBornMonster.monsterID = monsterID;
        newBornMonster.monsterChar = 'D';

        // Generate monster spawn position
        CoordinatePair randomSpawnPos = generateRandEmptyPos();
        newBornMonster.monsterRow = randomSpawnPos.row;
        newBornMonster.monsterCol = randomSpawnPos.col;

        // Allocate dynamic monster object
        newBornMonster.smellDistance = 0;
        newBornMonster.monsterPtr = new Dragon();

        // Push new monster to Level's Monster List
        addNodeToMonsterList(newBornMonster);
    }

    // === GameObject Placement ===
    // Generate random weapon placed at random location in level
    for (int i = 0; i < numerOfWeapons; i++)
    {
        GameObjectNode randWeapon;
            CoordinatePair objectSpawnPos = generateRandEmptyPos();
            randWeapon.itemRow = objectSpawnPos.row;
            randWeapon.itemCol = objectSpawnPos.col;
            randWeapon.itemChar = ')';
            int randWeaponType = randInt(0, 4);
                switch (randWeaponType)
                {
                case 0 : 
                    randWeapon.itemPtr = new Mace();
                    break;
                case 1:
                    randWeapon.itemPtr = new ShortSword();
                    break;
                case 2:
                    randWeapon.itemPtr = new LongSword();
                    break;
                case 3:
                    randWeapon.itemPtr = new MagicAxe();
                    break;
                case 4:
                    randWeapon.itemPtr = new MagicFangs();
                    break;
                }
            addNodeToGameObjectList(randWeapon); // Add to Level's Game Object List
    }

    // Generate random scroll(s) placed at random location in level
    for (int i = 0; i < numberOfScrolls; i++)
    {
        GameObjectNode randScroll;
            CoordinatePair objectSpawnPos = generateRandEmptyPos();
            randScroll.itemRow = objectSpawnPos.row;
            randScroll.itemCol = objectSpawnPos.col;
            randScroll.itemChar = '?';
            int randScrollType = randInt(0, 4);
                switch (randScrollType)
                {
                case 0:
                    randScroll.itemPtr = new TeleportationScroll(this);
                    break;
                case 1:
                    randScroll.itemPtr = new incrHPScroll();
                    break;
                case 2:
                    randScroll.itemPtr = new incrArmorScroll();
                    break;
                case 3:
                    randScroll.itemPtr = new incrStrScroll();
                    break;
                case 4:
                    randScroll.itemPtr = new incrDexScroll();
                    break;
                }
        addNodeToGameObjectList(randScroll); // Add to Level's Game Object List
    }

// Set GAME's pointers
        // Destroy old level - using game's pointer to currLevel
            if (levelNumber > 0)
            {
                m_levelToGamePtr->deleteCurrLevel();
            }
        // Set Game's pointer to currLevel to THIS level
            m_levelToGamePtr->setLevelptr(this);

//Level construction complete
    printMap(); // This is FIRST PRINT OF SCREEN
}

void Level::correctBounds(int &upperRowBound, int &lowerRowBound, int &leftColBound, int &rightColBound)
{
    if (upperRowBound < 1)
        upperRowBound = 1;

    if (lowerRowBound > 17)
        lowerRowBound = 17;

    if (leftColBound < 1)
        leftColBound = 1;

    if (rightColBound > 69)
        rightColBound = 69;
}



CoordinatePair Level::generateRandEmptyPos() {
    
    CoordinatePair randPos;

    int randRow = randInt(1, 17);
    int randCol = randInt(1, 69);
    while ( m_map[randRow][randCol] != ' ' || (randRow > 17 || randCol > 69) ) {
        randRow = randInt(1, 17);
        randCol = randInt(1, 69);
    }
    randPos.row = randRow;
    randPos.col = randCol;

    return randPos;
}

CoordinatePair Level::selectRandWallElement() {

    CoordinatePair randPos;

    int randRow = randInt(1, 17);
    int randCol = randInt(1, 69);
    while (m_map[randRow][randCol] != '#' || (randRow > 17 || randCol > 69)) {
        randRow = randInt(1, 17);
        randCol = randInt(1, 69);
    }
    randPos.row = randRow;
    randPos.col = randCol;

    return randPos;
}



Level::~Level()
{ 
    // FOR LOOP - delete all these lists
    //list<PlayerNode> m_playerStateHistory; // list object that stores player's coordinates
    //list<MonsterNode> m_monsterList; // List of all monsters on the level
    //list<GameObjectNode> m_gameObjectList; // List of all game objects on the level
    //list<string> m_msgList; // List of all messages from game activity

}



Player* Level::getLevelToPlayerPtr() {
    return m_levelToPlayerPtr;
}

int Level::getLevelNumber() {
    return m_levelNumber;
}

int Level::getMonsterLimit() const {
    return m_monsterUpperLimit;
}

void Level::addNodeToPlayerStateHistory(PlayerNode NodeToAdd) {

// Retrieve player's coordinate from state history list

    if (m_playerStateHistory.size() >= 1) { // If list is NOT empty - blank out current map posiiton

        int currRow = (m_playerStateHistory.front()).playerRow;
        int currCol = (m_playerStateHistory.front()).playerCol;
        //cerr << "currRow: " << currRow << endl;
        //cerr << "currCol: " << currCol << endl;

        // Set player's current on map to VACANT
        m_map[currRow][currCol] = ' ';
    }
    
    // Push player's NEW position into state history list
    m_playerStateHistory.push_front(NodeToAdd);    
}

void Level::addNodeToMonsterList(MonsterNode NodeToAdd) {

    m_monsterList.push_front(NodeToAdd);
}

void Level::addNodeToGameObjectList(GameObjectNode NodeToAdd) {

    m_gameObjectList.push_front(NodeToAdd);
}

void Level::addMsgtoMsgList(string const &msg) {

    m_msgList.push_front(msg);
}

void Level::setPlayerCoordinates(int const &newRow, int const &newCol) {

    PlayerNode newPlayerCoord;
    newPlayerCoord.playerRow = newRow;
    newPlayerCoord.playerCol = newCol;
    m_playerStateHistory.push_front(newPlayerCoord);
        // Most RECENT player coordinates will be AT FRONT of m_playerList  
        // Since old nodes are NOT deleted, this LIST will record ALL PREVIOUS coordinates visited by the player
}

PlayerNode Level::getPlayerCoordinates() {

    //Returns struct that contains data about player's CURRENT state
    PlayerNode currentPlayerCoord = m_playerStateHistory.front();        
    return currentPlayerCoord;
}

bool Level::checkForWallAtPosition(int const& newRow, int const& newCol) const {

    // Checks to see if there is a wall at specified coorddinates
    if (m_map[newRow][newCol] == '#')
        return true;

    else
        return false;
}

bool Level::checkForMonsterAtPosition(int const& newRow, int const& newCol) const {

    // Checks to see if there is a MONSTER at specified coorddinates
    if (   m_map[newRow][newCol] == 'S'
        || m_map[newRow][newCol] == 'G'
        || m_map[newRow][newCol] == 'B'
        || m_map[newRow][newCol] == 'D' )   
        return true;

    else
        return false;
}


void Level::setCharAtCoordinate(int r, int c, char newChar) {
    // Updates char of given map coordinate
    m_map[r][c] = newChar;
}

char Level::getCharAtCoordinate(int r, int c) {
    return m_map[r][c];
}


void Level::rcvAttackFromPlayer(int const& targetRow, int const& targetCol) {

    list<MonsterNode>::iterator m = m_monsterList.begin();
    for (; m != m_monsterList.end(); m++ ) { // Cycle level's entire monster list

        if (m->monsterRow == targetRow && m->monsterCol == targetCol) 
        { // If there is a monster at target cooridinates            
            // === Determine hit/miss ===

                // Calc attacker points
                int attackerPts = 0, attackerDex = 0, attackerWpnDexBonus = 0;
                attackerDex = m_levelToPlayerPtr->getDex(); // get attacker dex
                attackerWpnDexBonus = m_levelToPlayerPtr->getWpnDexBonusActr(); // get Dex bonus of wielded weapon
                attackerPts = attackerDex + attackerWpnDexBonus;                

                // Calc defender points
                int defenderPts = 0, defenderDex = 0, defenderArmorPts = 0;
                defenderDex = m->monsterPtr->getDex();
                defenderArmorPts = m->monsterPtr->getArmor();
                defenderPts = defenderDex + defenderArmorPts;

                // Compare attacker points to defender points to determine if attack is succcessful
                if (randInt(1, attackerPts) >= randInt(1, defenderPts)) 
                {
                    // Calc damage points player deals to monster
                    int dmgPts = 0, attackerStr = 0, attackerWpnDmg = 0;
                    attackerStr = m_levelToPlayerPtr->getStr();
                    attackerWpnDmg = m_levelToPlayerPtr->getWpnDmgActr();
                    dmgPts = randInt(0, attackerStr + attackerWpnDmg - 1);

                    // update monster's HP   
                    m->monsterPtr->setHP(-1*dmgPts); // DECREMENTS HP

                    // Update monster's sleep time
                    if (m_levelToPlayerPtr->getWpnTypeP() == "magic fangs") 
                    {
                        int sleepDurationInduced = m_levelToPlayerPtr->getWpnSleepTimeActr();                        
                        if (sleepDurationInduced > 0)
                        {                            
                            if (m->monsterSleepTime < sleepDurationInduced) 
                            {
                                m->monsterSleepTime = sleepDurationInduced;
                            }
                            // Generate activity message - SLEEP induced
                            string playerInducesSleepMsg;
                            char space = ' ';
                            char period = '.';
                            string theMagicFangsPut = "The magic fangs put";                       
                            string toSleep = "to sleep";                       
                            playerInducesSleepMsg = theMagicFangsPut + space
                                    + m->monsterID + space + toSleep + period;

                            addMsgtoMsgList(playerInducesSleepMsg);
                        } // IF SLEEP induced END
                    } // IF Magic Fang END

                    // Generate activity message - HIT
                    char space = ' ';
                    string playerStrikes = "Player strikes";
                    string with = "with";
                    string inflicting = "inflicting";
                    string commaSpace = ", ";
                    string damage = "damage";
                    char period = '.';
                    string playerHitMsg = playerStrikes + space + m->monsterID + space + with + space
                        + m_levelToPlayerPtr->getWpnTypeP() + commaSpace + inflicting + space
                        + to_string(dmgPts) + space + damage + period;   

                    addMsgtoMsgList(playerHitMsg);    

                    //monsterDeadCheck();

                } // IF HIT END

                else 
                {
                    // Generate activity message - MISS
                    string swooshPlayerSwings = "Player swings";
                    char space = ' ';
                    string at = "at";
                    string andMisses = "and misses";
                    char period = '.';                               
                    string playerMissMsg = swooshPlayerSwings + space + m_levelToPlayerPtr->getWpnTypeP()
                        + space + at + space + m->monsterID + space + andMisses + period;

                    addMsgtoMsgList(playerMissMsg);
                }

                // SCRAP
                //m->monsterPtr->getHP();
                //m->monsterPtr->getArmor();
                //m->monsterPtr->getStr();
                //m->monsterPtr->getDex();
                //m->monsterPtr->getSleepTime();

                //m_levelToPlayerPtr->getHP();
                //m_levelToPlayerPtr->getArmor();
                //m_levelToPlayerPtr->getStr();
                //m_levelToPlayerPtr->getDex();
                //m_levelToPlayerPtr->getSleepTime();

        } // IF TARGET present END
    } // Monster SEARCH END

    // SCRAP
    //attackerWpnDexBonus = m_levelToPlayerPtr->m_wpnWielded->getWpnDexBonusW();
         // ILLEGAL because m_wpnWielded is PRIVATE
}

void Level::monsterDeadCheck() {
// NOTE: this could be done more efficeintly by not re-constructing the list, but I was still experimenting with lists while writing this funciton
    // Removes DEAD monsters from LIST
    // Stores ALIVE monster in TEMP list
    // Reconstructs Level's monster list as a COPY of temp
    // TempList = m_monsterList - dead monsters



    int monsterListSize = m_monsterList.size();

    //// **** NEW 
    //list<MonsterNode>::iterator m = m_monsterList.begin();
    //while (m != m_monsterList.end())
    //{



    //}

    //********************************



    list<MonsterNode> tempList;
    list<MonsterNode>::iterator mm = m_monsterList.begin();
    int count = 0;
    count = 0;
    while (count < monsterListSize)
    {
        //cerr << "deadcheck running" << endl;

        bool monsterIsAlive = mm->monsterPtr->isAlive();

        if (monsterIsAlive) { // If monster is ALIVE - keep on monsterList
            //cerr << "monster is alive" << endl;
            MonsterNode keepAlive;
            keepAlive.monsterChar = mm->monsterChar;
            keepAlive.monsterID = mm->monsterID;
            keepAlive.monsterRow = mm->monsterRow;
            keepAlive.monsterCol = mm->monsterCol;
            keepAlive.monsterSleepTime = mm->monsterSleepTime;
            keepAlive.monsterPtr = mm->monsterPtr;
            keepAlive.smellDistance = mm->smellDistance;

            mm++;
            m_monsterList.pop_front(); // removes monster from list
            tempList.push_front(keepAlive); // stores monster in tempList
            count++;
        }

        if (!monsterIsAlive)
        { // IF monster is NOT alive

            // Check for pre-existing item at monster's positon
            bool preExistingItemAtMonstersPosition = false;
            list<GameObjectNode>::iterator o = m_gameObjectList.begin();
            for (; o != m_gameObjectList.end(); o++)
            {
                if (mm->monsterRow == o->itemRow && mm->monsterCol == o->itemCol)
                { // IF an object has SAME position as the dead monster
                    preExistingItemAtMonstersPosition = true;
                }
            }
            // IFF there is no pre-existing item
            if (!preExistingItemAtMonstersPosition)
            {
                 GameObjectNode wpnDrop;
                //// Determine what item is dropped
                switch (mm->monsterChar)
                {
                    case 'S':
                    {
                        // Decide what item is dropped - EMPTY string is returned if nothing is dropped and none of the if statements excute
                        string itemDropped = mm->monsterPtr->drop();
                        if (itemDropped == "magic fangs")
                        {
                            m_map[mm->monsterRow][mm->monsterCol] = ')';
                            wpnDrop.itemChar = ')';
                            wpnDrop.itemRow = mm->monsterRow;
                            wpnDrop.itemCol = mm->monsterCol;
                            wpnDrop.itemPtr = new MagicFangs();
                        }
                        //m_gameObjectList.push_front(wpnDrop); // ************
                        break;
                    } // END CASE S

                    case 'G':
                    {
                        // Decide what item is dropped - EMPTY string is returned if nothing is dropped and none of the if statements excute                        
                        string itemDropped = mm->monsterPtr->drop();
                        GameObjectNode wpnDrop;
                        if (itemDropped != "")
                        {
                            m_map[mm->monsterRow][mm->monsterCol] = ')';
                            wpnDrop.itemChar = ')';
                            wpnDrop.itemRow = mm->monsterRow;
                            wpnDrop.itemCol = mm->monsterCol;
                        }
                        if (itemDropped == "magic fangs")
                        {
                            wpnDrop.itemPtr = new MagicFangs();
                        }
                        if (itemDropped == "magic axe")
                        {
                            wpnDrop.itemPtr = new MagicAxe();
                        }
                        //m_gameObjectList.push_front(wpnDrop); // **********
                        break;
                    } // END CASE G

                case 'B':

                    break;

                case 'D':

                    break;

                default:
                    m_map[mm->monsterRow][mm->monsterCol] = ' '; // shouldn't need *** covered by printMap()               
                    break;

                } // MONSTER TYPE SWITCH END
                //m_gameObjectList.push_front(droppedItem); // done inside switch instead

            } // IFF NO ITEM PRESENT END

            // Generate activity message
            string monsterDeadMsg;
            string playerHasSlain = "Player has slain";
            char space = ' ';
            char exclamationPoint = '!';
            monsterDeadMsg = playerHasSlain + space + mm->monsterID + exclamationPoint;
            addMsgtoMsgList(monsterDeadMsg);

            m_map[mm->monsterRow][mm->monsterCol] = ' '; // shouldn't need ***                 
            delete mm->monsterPtr;
            mm->monsterPtr = nullptr;
            mm++;
            m_monsterList.pop_front(); // removes monster from list permanently

            count++;

        } // END IF MONSTER NOT ALIVE

    } // WHILE LOOP END

    // Reconstruct Level's Monster List - copy of temp
    list<MonsterNode>::iterator temp = tempList.begin();
    for (; temp != tempList.end(); temp++)
    {
        //cerr << "reconstructing monster list" << endl;
        MonsterNode rebirthedMonster;
        rebirthedMonster.monsterChar = temp->monsterChar;
        rebirthedMonster.monsterID = temp->monsterID;
        rebirthedMonster.monsterRow = temp->monsterRow;
        rebirthedMonster.monsterCol = temp->monsterCol;
        rebirthedMonster.monsterSleepTime = temp->monsterSleepTime;
        rebirthedMonster.monsterPtr = temp->monsterPtr;
        rebirthedMonster.smellDistance = temp->smellDistance;

        m_monsterList.push_front(rebirthedMonster);
    }

    //SCRAP
                            //m->erase(); // not defined for list of STRUCTS
                //m->remove(); // ****
                                //MonsterNode deadMonster;
                    //deadMonster.monsterChar = '#';
                    //deadMonster.monsterID = "";
                    //deadMonster.monsterRow = 0;
                    //deadMonster.monsterCol = 0;
                    //deadMonster.monsterSleepTime = 0;
                    //deadMonster.monsterPtr = nullptr;

                    //mm->monsterChar = '#';
                    //mm->monsterRow = 0;
                    //mm->monsterCol = 0;

                    //m_map[mm->monsterRow][mm->monsterCol] = ' ';
                    // Monster turns to stone and is placed at the origin - where their existence will be invisible           
            // ERASE() ONLY WORKS FOR STL LIST
            // need to define own for MOnsterNode type

}

void Level::getMonsterAttacks()
{
    list<MonsterNode>::iterator m = m_monsterList.begin();
    for (; m != m_monsterList.end(); m++)
    {
        // Check neighbors of all monsters for player 

        if ( m_map[m->monsterRow - 1][m->monsterCol] == '@') // North
        {
            //rcvAttackFromMonster(m->monsterRow - 1, m->monsterCol);  //*** Can create this funciton, passing in iterator as parameter

            // === Determine hit/miss ===
                // Calc attacker points (monster)
            int attackerPts = 0, attackerDex = 0, attackerWpnDexBonus = 0;
            attackerDex = m->monsterPtr->getDex(); // get attacker dex
            attackerWpnDexBonus = m->monsterPtr->getWpnDexBonusActr(); // get Dex bonus of wielded weapon
            attackerPts = attackerDex + attackerWpnDexBonus;

            // Calc defender points (player)
            int defenderPts = 0, defenderDex = 0, defenderArmorPts = 0;
            defenderDex = m_levelToPlayerPtr->getDex();
            defenderArmorPts = m_levelToPlayerPtr->getArmor();
            defenderPts = defenderDex + defenderArmorPts;

            // Compare attacker points to defender points to determine if attack is succcessful
            if (randInt(1, attackerPts) >= randInt(1, defenderPts))
            {
                // Calc damage points monster deals to player
                int dmgPts = 0, attackerStr = 0, attackerWpnDmg = 0;
                attackerStr = m->monsterPtr->getStr();
                attackerWpnDmg = m->monsterPtr->getWpnDmgActr();
                dmgPts = randInt(0, attackerStr + attackerWpnDmg - 1);

                // update player's HP   
                m_levelToPlayerPtr->setHP(-1 * dmgPts); // DECREMENTS HP

                // Update monster's sleep time
                if (m->monsterPtr->getWpnTypeM() == "magic fangs")
                {
                    int sleepDurationInduced = m->monsterPtr->getWpnSleepTimeActr();
                    if (sleepDurationInduced > 0)
                    {
                        if (m_levelToPlayerPtr->getSleepTime() < sleepDurationInduced)
                        {
                            m_levelToPlayerPtr->setSleepTime(sleepDurationInduced);
                        }
                        // Generate activity message - SLEEP induced
                        addMsgtoMsgList("The magic fangs put player to sleep");
                    } // IF SLEEP induced END
                } // IF Magic Fang END

                // Generate activity message - HIT
                string monsterHitMsg;
                char space = ' ';
                string swings = "swings";
                string atPlayerAndHits = "at player and hits";
                string commaSpace = ", ";
                string inflicting = "inflicting";
                string damage = "damage";
                char period = '.';
                monsterHitMsg = m->monsterID + space + swings + space + m->monsterPtr->getWpnTypeM() + space
                    + atPlayerAndHits + commaSpace + inflicting + space + to_string(dmgPts) + space + damage + period;
                addMsgtoMsgList(monsterHitMsg);           

            } // IF HIT END

            else
            {
                // Generate activity message - MISS
                string monsterMissMsg;
                //string swoosh = "Swoosh!";
                char space = ' ';
                string atPlayerAndMisses = "at player and misses.";
                string swings = "swings";
                monsterMissMsg = m->monsterID + space + swings + space + m->monsterPtr->getWpnTypeM()
                    + space + atPlayerAndMisses;           
                addMsgtoMsgList(monsterMissMsg);
            }

        } // MONSTER NORTH CHECK END


        if (m_map[m->monsterRow + 1][m->monsterCol] == '@') // South
        {
            //rcvAttackFromMonster(m->monsterRow - 1, m->monsterCol);
            // === Determine hit/miss ===
            // Calc attacker points (monster)
            int attackerPts = 0, attackerDex = 0, attackerWpnDexBonus = 0;
            attackerDex = m->monsterPtr->getDex(); // get attacker dex
            attackerWpnDexBonus = m->monsterPtr->getWpnDexBonusActr(); // get Dex bonus of wielded weapon
            attackerPts = attackerDex + attackerWpnDexBonus;

            // Calc defender points (player)
            int defenderPts = 0, defenderDex = 0, defenderArmorPts = 0;
            defenderDex = m_levelToPlayerPtr->getDex();
            defenderArmorPts = m_levelToPlayerPtr->getArmor();
            defenderPts = defenderDex + defenderArmorPts;

            // Compare attacker points to defender points to determine if attack is succcessful
            if (randInt(1, attackerPts) >= randInt(1, defenderPts))
            {
                // Calc damage points monster deals to player
                int dmgPts = 0, attackerStr = 0, attackerWpnDmg = 0;
                attackerStr = m->monsterPtr->getStr();
                attackerWpnDmg = m->monsterPtr->getWpnDmgActr();
                dmgPts = randInt(0, attackerStr + attackerWpnDmg - 1);

                // update player's HP   
                m_levelToPlayerPtr->setHP(-1 * dmgPts); // DECREMENTS HP

                // Update monster's sleep time
                if (m->monsterPtr->getWpnTypeM() == "magic fangs")
                {
                    int sleepDurationInduced = m->monsterPtr->getWpnSleepTimeActr();
                    if (sleepDurationInduced > 0)
                    {
                        if (m_levelToPlayerPtr->getSleepTime() < sleepDurationInduced)
                        {
                            m_levelToPlayerPtr->setSleepTime(sleepDurationInduced);
                        }
                        // Generate activity message - SLEEP induced
                        addMsgtoMsgList("The magic fangs put player to sleep");
                    } // IF SLEEP induced END
                } // IF Magic Fang END

                // Generate activity message - HIT
                string monsterHitMsg;
                char space = ' ';
                string swings = "swings";
                string atPlayerAndHits = "at player and hits";
                string commaSpace = ", ";
                string inflicting = "inflicting";
                string damage = "damage";
                char period = '.';
                monsterHitMsg = m->monsterID + space + swings + space + m->monsterPtr->getWpnTypeM() + space
                    + atPlayerAndHits + commaSpace + inflicting + space + to_string(dmgPts) + space + damage + period;
                addMsgtoMsgList(monsterHitMsg);


            } // IF HIT END

            else
            {
                // Generate activity message - MISS
                string monsterMissMsg;
                //string swoosh = "Swoosh!";
                char space = ' ';
                string atPlayerAndMisses = "at player and misses.";
                string swings = "swings";
                monsterMissMsg = m->monsterID + space + swings + space + m->monsterPtr->getWpnTypeM()
                    + space + atPlayerAndMisses;
                addMsgtoMsgList(monsterMissMsg);
            }

        }// MONSTER SOUTH CHECK END


        if (m_map[m->monsterRow][m->monsterCol - 1] == '@') // West
        {
            //rcvAttackFromMonster(m->monsterRow - 1, m->monsterCol); //****

            // === Determine hit/miss ===
            // Calc attacker points (monster)
            int attackerPts = 0, attackerDex = 0, attackerWpnDexBonus = 0;
            attackerDex = m->monsterPtr->getDex(); // get attacker dex
            attackerWpnDexBonus = m->monsterPtr->getWpnDexBonusActr(); // get Dex bonus of wielded weapon
            attackerPts = attackerDex + attackerWpnDexBonus;

            // Calc defender points (player)
            int defenderPts = 0, defenderDex = 0, defenderArmorPts = 0;
            defenderDex = m_levelToPlayerPtr->getDex();
            defenderArmorPts = m_levelToPlayerPtr->getArmor();
            defenderPts = defenderDex + defenderArmorPts;

            // Compare attacker points to defender points to determine if attack is succcessful
            if (randInt(1, attackerPts) >= randInt(1, defenderPts))
            {
                // Calc damage points monster deals to player
                int dmgPts = 0, attackerStr = 0, attackerWpnDmg = 0;
                attackerStr = m->monsterPtr->getStr();
                attackerWpnDmg = m->monsterPtr->getWpnDmgActr();
                dmgPts = randInt(0, attackerStr + attackerWpnDmg - 1);

                // update player's HP   
                m_levelToPlayerPtr->setHP(-1 * dmgPts); // DECREMENTS HP

                // Update monster's sleep time
                if (m->monsterPtr->getWpnTypeM() == "magic fangs")
                {
                    int sleepDurationInduced = m->monsterPtr->getWpnSleepTimeActr();
                    if (sleepDurationInduced > 0)
                    {
                        if (m_levelToPlayerPtr->getSleepTime() < sleepDurationInduced)
                        {
                            m_levelToPlayerPtr->setSleepTime(sleepDurationInduced);
                        }
                        // Generate activity message - SLEEP induced
                        addMsgtoMsgList("The magic fangs put player to sleep");
                    } // IF SLEEP induced END
                } // IF Magic Fang END

                // Generate activity message - HIT
                string monsterHitMsg;
                char space = ' ';
                string swings = "swings";
                string atPlayerAndHits = "at player and hits";
                string commaSpace = ", ";
                string inflicting = "inflicting";
                string damage = "damage";
                char period = '.';
                monsterHitMsg = m->monsterID + space + swings + space + m->monsterPtr->getWpnTypeM() + space
                    + atPlayerAndHits + commaSpace + inflicting + space + to_string(dmgPts) + space + damage + period;
                addMsgtoMsgList(monsterHitMsg);

            } // IF HIT END

            else
            {
                // Generate activity message - MISS
                string monsterMissMsg;
                //string swoosh = "Swoosh!";
                char space = ' ';
                string atPlayerAndMisses = "at player and misses.";
                string swings = "swings";
                monsterMissMsg = m->monsterID + space + swings + space + m->monsterPtr->getWpnTypeM()
                    + space + atPlayerAndMisses;
                addMsgtoMsgList(monsterMissMsg);
            }
        }// MONSTER WEST CHECK END


        if (m_map[m->monsterRow][m->monsterCol + 1] == '@') // East
        {
            //rcvAttackFromMonster(m->monsterRow - 1, m->monsterCol);

            // === Determine hit/miss ===
                // Calc attacker points (monster)
            int attackerPts = 0, attackerDex = 0, attackerWpnDexBonus = 0;
            attackerDex = m->monsterPtr->getDex(); // get attacker dex
            attackerWpnDexBonus = m->monsterPtr->getWpnDexBonusActr(); // get Dex bonus of wielded weapon
            attackerPts = attackerDex + attackerWpnDexBonus;

            // Calc defender points (player)
            int defenderPts = 0, defenderDex = 0, defenderArmorPts = 0;
            defenderDex = m_levelToPlayerPtr->getDex();
            defenderArmorPts = m_levelToPlayerPtr->getArmor();
            defenderPts = defenderDex + defenderArmorPts;

            // Compare attacker points to defender points to determine if attack is succcessful
            if (randInt(1, attackerPts) >= randInt(1, defenderPts))
            {
                // Calc damage points monster deals to player
                int dmgPts = 0, attackerStr = 0, attackerWpnDmg = 0;
                attackerStr = m->monsterPtr->getStr();
                attackerWpnDmg = m->monsterPtr->getWpnDmgActr();
                dmgPts = randInt(0, attackerStr + attackerWpnDmg - 1);

                // update player's HP   
                m_levelToPlayerPtr->setHP(-1 * dmgPts); // DECREMENTS HP

                // Update monster's sleep time
                if (m->monsterPtr->getWpnTypeM() == "magic fangs")
                {
                    int sleepDurationInduced = m->monsterPtr->getWpnSleepTimeActr();
                    if (sleepDurationInduced > 0)
                    {
                        if (m_levelToPlayerPtr->getSleepTime() < sleepDurationInduced)
                        {
                            m_levelToPlayerPtr->setSleepTime(sleepDurationInduced);
                        }
                        // Generate activity message - SLEEP induced
                        addMsgtoMsgList("The magic fangs put player to sleep");
                    } // IF SLEEP induced END
                } // IF Magic Fang END

                // Generate activity message - HIT
                string monsterHitMsg;
                char space = ' ';
                string swings = "swings";
                string atPlayerAndHits = "at player and hits";
                string commaSpace = ", ";
                string inflicting = "inflicting";
                string damage = "damage";
                char period = '.';
                monsterHitMsg = m->monsterID + space + swings + space + m->monsterPtr->getWpnTypeM() + space
                    + atPlayerAndHits + commaSpace + inflicting + space + to_string(dmgPts) + space + damage + period;
                addMsgtoMsgList(monsterHitMsg);

            } // IF HIT END

            else
            {
                // Generate activity message - MISS
                string monsterMissMsg;
                //string swoosh = "Swoosh!";
                char space = ' ';
                string atPlayerAndMisses = "at player and misses.";
                string swings = "swings";
                monsterMissMsg = m->monsterID + space + swings + space + m->monsterPtr->getWpnTypeM()
                    + space + atPlayerAndMisses;
                addMsgtoMsgList(monsterMissMsg);
            }

        }// MONSTER EAST CHECK END


    } // END MONSTLIST LOOP

}



bool Level::pathExists(char tempMap[18][70], int sr, int sc, int er, int ec)
/*
Determine direction of END point relative to START point 
    == N S E W

Recursively call function based on direction
    BUT *** what if goblin has to move in oppposite direction because of wall??

*/
{

     //if (sr < 0 || sr > 9 || sc < 0 || sc > 9 ||
    //    er < 0 || er > 9 || ec < 0 || ec > 9 ||
    //    m_map[sr][sc] != OPEN || m_map[er][ec] != OPEN)
    //    return false;

// ======== Prof Solution Algorithm ===================================

    //cerr << endl;
    //cerr << "--- pathExists() call ---" << endl;
    //cerr << "start: " << sr << ", " << sc << " | end: " << er << ", " << ec << endl;

    //// Print CURRENT state of map
    //cerr << "map INSIDE pathExists: " << endl;
    //for (int r = 0; r < 18; r++) {
    //    for (int c = 0; c < 70; c++) {
    //        cout << m_map[r][c];
    //    }
    //    cout << "row: " << r << endl;
    //}

    // ========= Operating on PARAMETER map ==================================================

     // Base Cases
    if (tempMap[sr][sc] == '#')
    {
        //cerr << "returning false - path not open" << endl;
        return false;
    }

    if (sr == er && sc == ec)
    {
        //cerr << "returnin true - path complete" << endl;
        return true;
    }

    tempMap[sr][sc] = 'V'; // NOT neceesarry to mark*****
        // can just pass 2 new parameters
        // nextR nextC
        // Record - nect point you want mosnter to go to
    // can pass by ref
    // change parameters driectly and use outside funciton
    // when next step is the end
    // set current SR SC to be next

    // Recursive Cases
    bool north = false, south = false, west = false, east = false;
    if (tempMap[sr - 1][sc] != '#' && tempMap[sr - 1][sc] != 'V')
    {
        //cerr << "recursive call - North" << endl;
        north = pathExists(tempMap, sr - 1, sc, er, ec);
    }
    if (tempMap[sr + 1][sc] != '#' && tempMap[sr + 1][sc] != 'V')
    {
        //cerr << "recursive call - South" << endl;
        south = pathExists(tempMap, sr + 1, sc, er, ec);
    }
    if (tempMap[sr][sc - 1] != '#' && tempMap[sr][sc - 1] != 'V')
    {
        //cerr << "recursive call - West" << endl;
        west = pathExists(tempMap, sr, sc - 1, er, ec);
    }
    if (tempMap[sr][sc + 1] != '#' && tempMap[sr - 1][sc + 1] != 'V')
    {
        //cerr << "recursive call - East" << endl;
        east = pathExists(tempMap, sr, sc + 1, er, ec);
    }

    // Tree Cases
    if (north == true)
    {
        return true;
    }

    if (south == true)
    {
        return true;
    }

    if (west == true)
    {
        return true;
    }

    if (east == true)
    {
        return true;
    }

    else {
        //cerr << "returning false - backtracking??" << endl;
        return false;
    }


   // ========= Making temp copy INSIDE function ==================================================


    //// Make copy of dungeon map to mark spots as visited on 
    //char tempMap[18][70];
    //for (int r = 0; r < 18; r++)
    //{
    //    for (int c = 0; c < 70; c++)
    //    {
    //        tempMap[r][c] = m_map[r][c];
    //    }
    //}

    //// Base Cases
    //if (tempMap[sr][sc] == '#')
    //{
    //    //cerr << "returning false - path not open" << endl;
    //    return false;
    //}

    //if (sr == er && sc == ec)
    //{
    //    //cerr << "returnin true - path complete" << endl;
    //    return true;
    //}

    //tempMap[sr][sc] = 'V';

    //// Recursive Cases
    //bool north = false, south = false, west = false, east = false;
    //if (tempMap[sr - 1][sc] != '#' && tempMap[sr - 1][sc] != 'V')
    //{
    //    //cerr << "recursive call - North" << endl;
    //    north = pathExists(tempMap, sr - 1, sc, er, ec);
    //}
    //if (tempMap[sr + 1][sc] != '#' && tempMap[sr + 1][sc] != 'V')
    //{
    //    //cerr << "recursive call - South" << endl;
    //    south = pathExists(tempMap, sr + 1, sc, er, ec);
    //}
    //if (tempMap[sr][sc - 1] != '#' && tempMap[sr][sc - 1] != 'V')
    //{
    //    //cerr << "recursive call - West" << endl;
    //    west = pathExists(tempMap, sr, sc - 1, er, ec);
    //}
    //if (tempMap[sr][sc + 1] != '#' && tempMap[sr - 1][sc + 1] != 'V')
    //{
    //    //cerr << "recursive call - East" << endl;
    //    east = pathExists(tempMap, sr, sc + 1, er, ec);
    //}

    //// Tree Cases
    //if (north == true)
    //{
    //    //tempMap[sr - 1][sc] = ' ';
    //    //tempMap[sr][sc] = ' ';
    //    return true;
    //}

    //if (south == true)
    //{
    //    //tempMap[sr + 1][sc] = ' ';
    //    //tempMap[sr][sc] = ' ';
    //    return true;
    //}

    //if (west == true)
    //{
    //    //tempMap[sr][sc - 1] = ' ';
    //    //tempMap[sr][sc] = ' ';
    //    return true;
    //}

    //if (east == true)
    //{
    //    //tempMap[sr][sc + 1] = ' ';
    //    //tempMap[sr][sc] = ' ';
    //    return true;
    //}

    //else {
    //    //cerr << "returning false - backtracking??" << endl;
    //    return false;
    //}


    // =========Re-Marking spaces as OPEN ==================================================


    //m_map[sr][sc] = 'V';

    //// Recursive Cases
    //if (m_map[sr - 1][sc] != '#' && m_map[sr - 1][sc] != 'V')
    //{
    //    //cerr << "recursive call - North" << endl;
    //    //m_map[sr][sc] = ' ';
    //    north = pathExists(m_map, sr - 1, sc, er, ec);
    //}
    //if (m_map[sr + 1][sc] != '#' && m_map[sr + 1][sc] != 'V')
    //{
    //    //cerr << "recursive call - South" << endl;
    //    //m_map[sr][sc] = ' ';
    //    south = pathExists(m_map, sr + 1, sc, er, ec);
    //}
    //if (m_map[sr][sc - 1] != '#' && m_map[sr][sc - 1] != 'V')
    //{
    //    //cerr << "recursive call - West" << endl;
    //    //m_map[sr][sc] = ' ';
    //    west = pathExists(m_map, sr, sc - 1, er, ec);
    //}
    //if (m_map[sr][sc + 1] != '#' && m_map[sr - 1][sc + 1] != 'V')
    //{
    //    //cerr << "recursive call - East" << endl;
    //    //m_map[sr][sc] = ' ';
    //    east = pathExists(m_map, sr, sc + 1, er, ec);
    //}

    //if (north == true)
    //{
    //    //m_map[sr - 1][sc] = ' ';
    //    m_map[sr][sc] = ' ';
    //    return true;
    //}

    //if (south == true)
    //{
    //    //m_map[sr + 1][sc] = ' ';
    //    m_map[sr][sc] = ' ';
    //    return true;
    //}

    //if (west == true)
    //{
    //    //m_map[sr][sc - 1] = ' ';
    //    m_map[sr][sc] = ' ';
    //    return true;
    //}

    //if (east == true)
    //{
    //    //m_map[sr][sc + 1] = ' ';
    //    m_map[sr][sc] = ' ';
    //    return true;
    //}
    //else {
    //    //cerr << "returning false - backtracking??" << endl;

    //    return false;
    //}

    // ===========================================================

    // Stack - DFS
    /*
        add ALL coordinates

        ...  

    */


    // List
    /*
        struct unseenCoordinate{
            
            bool visited = false;
            int row
            int col

        };

        list<unseenCoordinate> toDoList

        push ALL coordinates that != #

        pathExists()
            remove visited coordinates from list


        toVisitList - all coordinates that ARE NOT NEIGHBORS of '>'

        while ( pathNotFound )
        {

            sr,sc = list.front()     


        }

        have helper function that return the path your looking at for every possible path    



        
        choose SHORTEST path to follow towards player

    */

    // If algorithm is boxed in // *****
    //if (   tempMap[sr - 1][sc] == '#' || tempMap[sr - 1][sc] == 'V'
    //    || tempMap[sr + 1][sc] == '#' || tempMap[sr + 1][sc] == 'V'
    //    || tempMap[sr][sc - 1] == '#' || tempMap[sr][sc - 1] == 'V'
    //    || tempMap[sr][sc + 1] == '#' || tempMap[sr][sc + 1] == 'V'  ) 
    //{



    //}


    //if (tempMap[sr][sc] != ' '
//    || tempMap[sr][sc] != '@' 
//    || tempMap[sr][sc] != '>' 
//    || tempMap[sr][sc] != ')' 
//    || tempMap[sr][sc] != '?'
//    || tempMap[sr][sc] != 'S'
//    || tempMap[sr][sc] != 'G'
//    || tempMap[sr][sc] != 'B'
//    || tempMap[sr][sc] != 'D' )
//{
//    cerr << "returning false - path not open" << endl;
//    return false;
//}

}

bool Level::pathExistsWithinStepCount(char tempMap[18][70], int sr, int sc, int er, int ec, int stepsRemaining)
/*
wheverr you find shortest path - update min
    return false if COUNT if large than minium

dont need helper function


//reutnr true
    when current sr sc, when you can find target in one step in any direciton // BASE CASE
    // ref the nextR and nextC

*/

{
//    //if (sr < 0 || sr > 9 || sc < 0 || sc > 9 ||
//    //    er < 0 || er > 9 || ec < 0 || ec > 9 ||
//    //    m_map[sr][sc] != OPEN || m_map[er][ec] != OPEN)
//    //    return false;
//
//// ======== Prof Solution Algorithm ===================================
//
//    //cerr << endl;
//    //cerr << "--- pathExists() call ---" << endl;
//    //cerr << "start: " << sr << ", " << sc << " | end: " << er << ", " << ec << endl;
//
//     // Base Cases
//    if (tempMap[sr][sc] == '#')
//    {
//        //cerr << "returning false - path not open" << endl;
//        return false;
//    }
//
//    if (sr == er && sc == ec)
//    {
//        //cerr << "returnin true - path complete" << endl;
//        return true;
//    }
//
//
//    tempMap[sr][sc] = 'V';
//
//    //*** PASS IN STEP COUNT BY REF
//
//    // Recursive Cases
//    bool north = false, south = false, west = false, east = false;
//    if (tempMap[sr - 1][sc] != '#' && tempMap[sr - 1][sc] != 'V')
//    {
//        //cerr << "recursive call - North" << endl;
//        north = pathExistsWithinStepCount(tempMap, sr - 1, sc, er, ec);
//    }
//    if (tempMap[sr + 1][sc] != '#' && tempMap[sr + 1][sc] != 'V')
//    {
//        //cerr << "recursive call - South" << endl;
//        south = pathExistsWithinStepCount(tempMap, sr + 1, sc, er, ec);
//    }
//    if (tempMap[sr][sc - 1] != '#' && tempMap[sr][sc - 1] != 'V')
//    {
//        //cerr << "recursive call - West" << endl;
//        west = pathExistsWithinStepCount(tempMap, sr, sc - 1, er, ec);
//    }
//    if (tempMap[sr][sc + 1] != '#' && tempMap[sr - 1][sc + 1] != 'V')
//    {
//        //cerr << "recursive call - East" << endl;
//        east = pathExistsWithinStepCount(tempMap, sr, sc + 1, er, ec);
//    }
//
//    // Tree Cases
//    if (north == true)
//    {
//        //tempMap[sr - 1][sc] = ' ';
//        //tempMap[sr][sc] = ' ';
//        return true;
//    }
//
//    if (south == true)
//    {
//        //tempMap[sr + 1][sc] = ' ';
//        //tempMap[sr][sc] = ' ';
//        return true;
//    }
//
//    if (west == true)
//    {
//        //tempMap[sr][sc - 1] = ' ';
//        //tempMap[sr][sc] = ' ';
//        return true;
//    }
//
//    if (east == true)
//    {
//        //tempMap[sr][sc + 1] = ' ';
//        //tempMap[sr][sc] = ' ';
//        return true;
//    }
//
//    else {
//        //cerr << "returning false - backtracking??" << endl;
//        return false;
//    }

return 0; //******************
}

bool Level::pathExistsWithinStepCount(char tempMap[18][70], int sr, int sc, int& nr, int& nc, int er, int ec, int stepsRemaining, int& minPath)
{
        //if (sr < 0 || sr > 9 || sc < 0 || sc > 9 ||
    //    er < 0 || er > 9 || ec < 0 || ec > 9 ||
    //    m_map[sr][sc] != OPEN || m_map[er][ec] != OPEN)
    //    return false;

// ======== Prof Solution Algorithm ===================================

    //cerr << endl;
    //cerr << "--- pathExists() call ---" << endl;
    //cerr << "start: " << sr << ", " << sc << " | end: " << er << ", " << ec << endl;

     // Base Cases
    if (tempMap[sr][sc] == '#')
    {
        //cerr << "returning false - path not open" << endl;
        return false;
    }

    if (sr == er && sc == ec)
    {
        //cerr << "returning true - path complete" << endl;
        return true;
    }

    if (nr == er && nc == ec)
    {
        //cerr << "returning true - path complete" << endl;
        return true;
    }

    if (stepsRemaining <= 0)
    {
        return false;
    }


    tempMap[sr][sc] = 'V';

    //*** PASS IN STEP COUNT BY REF

    // Recursive Cases
    bool north = false, south = false, west = false, east = false;
    if (tempMap[nr - 1][nc] != '#' && tempMap[nr - 1][nc] != 'V')
    {
        //cerr << "recursive call - North" << endl;
        //cerr << "stepsRemaining: " << stepsRemaining << endl;

        nr--;
        stepsRemaining--;
        north = pathExistsWithinStepCount(tempMap, sr, sc, nr, nc, er, ec, stepsRemaining, minPath);
    }
    if (tempMap[nr + 1][nc] != '#' && tempMap[nr + 1][nc] != 'V')
    {
        //cerr << "recursive call - South" << endl;
        //cerr << "stepsRemaining: " << stepsRemaining << endl;

        nr++;
        stepsRemaining--;
        south = pathExistsWithinStepCount(tempMap, sr, sc, nr, nc, er, ec, stepsRemaining, minPath);
    }
    if (tempMap[nr][nc - 1] != '#' && tempMap[nr][nc - 1] != 'V')
    {
        //cerr << "recursive call - West" << endl;
        //cerr << "stepsRemaining: " << stepsRemaining << endl;

        nc--;
        stepsRemaining--;

        west = pathExistsWithinStepCount(tempMap, sr, sc, nr, nc, er, ec, stepsRemaining, minPath);
    }
    if (tempMap[nr][nc + 1] != '#' && tempMap[nr - 1][nc + 1] != 'V')
    {
        //cerr << "recursive call - East" << endl;
        //cerr << "stepsRemaining: " << stepsRemaining << endl;

        nc++;
        stepsRemaining--;
        east = pathExistsWithinStepCount(tempMap, sr, sc, nr, nc, er, ec, stepsRemaining, minPath);
    }

    // More Base Cases
    if (north == true)
    {
        //tempMap[sr - 1][sc] = ' ';
        //tempMap[sr][sc] = ' ';
        return true;
    }

    if (south == true)
    {
        //tempMap[sr + 1][sc] = ' ';
        //tempMap[sr][sc] = ' ';
        return true;
    }

    if (west == true)
    {
        //tempMap[sr][sc - 1] = ' ';
        //tempMap[sr][sc] = ' ';
        return true;
    }

    if (east == true)
    {
        //tempMap[sr][sc + 1] = ' ';
        //tempMap[sr][sc] = ' ';
        return true;
    }

    else {
        //cerr << "returning false - backtracking??" << endl;
        return false;
    }
}


void Level::getMonsterMoves()
{
    int playerRow = (m_playerStateHistory.front()).playerRow;
    int playerCol = (m_playerStateHistory.front()).playerCol;

    list<MonsterNode>::iterator m = m_monsterList.begin();
    int count = -1;
    while (m != m_monsterList.end())
    {
        count++;

        if (m->monsterChar == 'S' || m->monsterChar == 'B'
            || m->monsterChar == 'G'
            )
        {
            // Calc distance to player
            double dist = sqrt( (pow((m->monsterRow - playerRow), 2)) + (pow((m->monsterCol - playerCol), 2)));

            // Compare player distance to mosnter's smell distance
            if (dist <= m->smellDistance) 
            {               
                if (m->monsterChar == 'G') // GOBLINE MOVEMENT ONLY
                {
                    // Make copy of map for pathExistsWithinStepCount();
                    char tempMap[18][70];
                    for (int r = 0; r < 18; r++)
                    {
                        for (int c = 0; c < 70; c++)
                        {
                            tempMap[r][c] = m_map[r][c];
                        }
                    }
                    int monsterNextRow = m->monsterRow;
                    int monsterNextCol = m->monsterCol;
                    int minPath = 99;
                    int stepsRemaining = m->smellDistance;
                    //stepsRemaining = 5; //******** TESTING

                    int pathExistsResult = -1;

                    m->playerSmelt = pathExistsWithinStepCount(tempMap, m->monsterRow, m->monsterCol, monsterNextRow, monsterNextCol, playerRow, playerCol, stepsRemaining, minPath);
                    // also pas sin NEXTrow and NEXTcol
                    // that way you dont need to mark 'V'
                    // mnimum number of paths
                    /// anytime current count is larger than mininumber - return                     
                    m->playerSmelt = pathExistsResult;
                    //cerr << "pathExistsResult: " << m->playerSmelt << endl;

                        if (pathExistsResult == 0)
                        {
                            break; // IF GOBLIN DOEESNT SMELL player - it won't move
                        }
                   }


                // Test for direction TOWARDS PLAYER from MONSTER's POSITION
                char northSouth = 'X';
                if ( (playerRow - m->monsterRow) < 0)
                {   // PLAYER  is NORTH of MONSTER
                    northSouth = 'N';
                }
                if ( (playerRow - m->monsterRow) > 0)
                {   // PLAYER  is SOUTH of MONSTER
                    northSouth = 'S';
                }
                if ( (playerRow - m->monsterRow) == 0)
                {   // PLAYER row == START MONSTER
                    northSouth = '0';
                }

                char westEast = 'X';
                if ( (playerCol - m->monsterCol) > 0)
                { // PLAYER  is EAST of MONSTER
                    westEast = 'E';
                }
                if ( (playerCol - m->monsterCol) < 0)
                { // PLAYER  is WEST of MONSTER
                    westEast = 'W';
                }
                if ( (playerCol - m->monsterCol) == 0)
                { // PLAYER col == MONSTER
                    westEast = '0';
                }

/*                cerr << "Rowdist "  << m->monsterRow - playerRow << endl;
                cerr << "Coldist " << m->monsterCol - playerCol << endl;
                cerr << m->monsterID << " dist: " << dist << " | smellDist: " << m->smellDistance << endl;
                cerr << "northSouth: " << northSouth << endl;
                cerr << "westEast: " << westEast << endl;  */             

                int newMonsterRow = 0;
                int newMonsterCol = 0;                

                bool pathObstructed = true;

                    //cerr << "pathObstructed: " << pathObstructed << endl;
                    // Random 50/50 decision on vertical vs horizontal movement
                    double p = 0.5;
                    bool verticalMovement = trueWithProbability(p);
                    if (verticalMovement) // move vertical towards player
                    {
                        switch (northSouth) // PLyer Position
                        {
                        case 'N' :
                            newMonsterRow = m->monsterRow - 1; // MOVE NORTH
                            newMonsterCol = m->monsterCol;                            
                            break;
                        case 'S':
                            newMonsterRow = m->monsterRow + 1; // MOVE SOUTH
                            newMonsterCol = m->monsterCol;
                            break;

                        case '0': // Player is DIRECTLY West or East
                        {
                            switch (westEast)
                            {
                            case 'E':
                                newMonsterRow = m->monsterRow;
                                newMonsterCol = m->monsterCol + 1; // MOVE EAST
                                break;
                            case 'W':
                                newMonsterRow = m->monsterRow;
                                newMonsterCol = m->monsterCol - 1; // MOVE WEST
                                break;
                            }
                            break;
                        } // CASE W/E END
                        default :
                            newMonsterRow = 0;
                            newMonsterCol = 0;
                            //cerr << "northSouth assignment FAILED" << endl;
                            addMsgtoMsgList("northSouth assignment FAILED");
                                break;
                        }// N/S SWITCH END
                    }                    

                    if (!verticalMovement) // Move horiztonal towards player
                    {
                        switch (westEast) // Player postion
                        {
                        case 'E':
                            newMonsterRow = m->monsterRow;
                            newMonsterCol = m->monsterCol + 1; // MOVE EAST
                            break;
                        case 'W':
                            newMonsterRow = m->monsterRow;
                            newMonsterCol = m->monsterCol - 1; // MOVE WEST
                            break;

                        case '0': // Player is DIRECTLY North or South
                        {
                            switch (northSouth)
                            {
                            case 'N':
                                newMonsterRow = m->monsterRow - 1; // MOVE NORTH
                                newMonsterCol = m->monsterCol;
                                break;
                            case 'S':
                                newMonsterRow = m->monsterRow + 1; // MOVE SOUTH
                                newMonsterCol = m->monsterCol;
                                break;
                            }
                            break;
                        } // CASE N/S END
                        default:
                            //cerr << "westEast assignment FAILED" << endl;
                            addMsgtoMsgList("westEast assignment FAILED");
                            newMonsterRow = 0;
                            newMonsterCol = 0;
                            break;
                        }

                    }
                     //Make sure there is no obstruction in new monster position
                    pathObstructed = false; // Assumption
                    if (checkForMonsterAtPosition(newMonsterRow, newMonsterCol))
                    {
                        pathObstructed = true; // Contiues while loop
                        newMonsterRow = m->monsterRow; // POS STAYS THE SAME
                        newMonsterCol = m->monsterCol;
                    }
                    if (checkForWallAtPosition(newMonsterRow, newMonsterCol))
                    {
                        pathObstructed = true; // Continues while loop
                        newMonsterRow = m->monsterRow; // POS STAYS THE SAME
                        newMonsterCol = m->monsterCol;
                    }
                    if (newMonsterRow == playerRow && newMonsterCol == playerCol)
                    {
                        pathObstructed = false; // to end while loop - attackes will be conducted through other functions
                        newMonsterRow = m->monsterRow; // POS STAYS THE SAME
                        newMonsterCol = m->monsterCol;
                    }
                    m_map[m->monsterRow][m->monsterCol] = ' ';
                    m->monsterRow = newMonsterRow;
                    m->monsterCol = newMonsterCol;
                    m_map[newMonsterRow][newMonsterCol] = m->monsterChar;

            } // END IF WITHIN SMELL DIST

        } // END S OR B

        m++;
    } // END MONSTER LIST FOR LOOP


}

void Level::deleteItemFromGameObjectList(const int& itemRow, const int& itemCol)
{
    list<GameObjectNode>::iterator it = m_gameObjectList.begin();
    while (it != m_gameObjectList.end())
    {

        if (it->itemRow == itemRow && it->itemCol == itemCol)
        {
            delete it->itemPtr;

            it = m_gameObjectList.erase(it); // This erase item AND increments iterator
        }
        else
            it++;

    }

}

GameObjectNode Level::getObjectNodeFromLevel()
{
    GameObjectNode returnedObjectNode;
    returnedObjectNode.itemChar = ' ';


    int playerRow = (m_playerStateHistory.front()).playerRow;
    int playerCol = (m_playerStateHistory.front()).playerCol;

    list<GameObjectNode>::iterator o = m_gameObjectList.begin();
    while (o != m_gameObjectList.end())
    {
        if (o->itemRow == playerRow && o->itemCol == playerCol)
        {   // COPY object node
            returnedObjectNode.itemChar = o->itemChar;
            returnedObjectNode.itemRow = o->itemRow;
            returnedObjectNode.itemCol = o->itemCol;
            returnedObjectNode.itemPtr = o->itemPtr;

            // Erase object from LEVEL's game object list
            o = m_gameObjectList.erase(o);
                // erase also increments o
        }
        else
            o++;
    }
    return returnedObjectNode; // return object to player
}



void Level::monsterHPRegain()
{
    list<MonsterNode>::iterator m = m_monsterList.begin();
    for (; m != m_monsterList.end() ; m++)
    {
        if (m->monsterChar == 'D') // CANNOT DEREFERENCE END LIST ITERATOR??? ******
        {
            m->monsterPtr->regainHP(); // **** DONT NEED DYNAMIC CAST if dragon has override for this funciton in Actor class
        }
    }
}

void Level::teleportPlayer()
{   
    m_map[m_playerStateHistory.front().playerRow][m_playerStateHistory.front().playerCol] = ' ';
    PlayerNode newPlayerPositon;
        CoordinatePair randPos = generateRandEmptyPos();
        newPlayerPositon.playerRow = randPos.row;
        newPlayerPositon.playerCol = randPos.col;
    m_playerStateHistory.push_front(newPlayerPositon);
}


// ============================ Print Functions ==============================

void Level::printPlayerNodeList() {

    // Test Print - 1st item displayed will be current state
    list<PlayerNode>::iterator p = m_playerStateHistory.begin();
    int i = 0;
    for (; p != m_playerStateHistory.end(); p++, i++) {
        cerr << "--- Player Node " << i << " --- " << endl;
        cerr << p->playerChar << endl;
        cerr << p->playerRow << endl;
        cerr << p->playerCol << endl;
        cerr << endl;
    }
}

void Level::printMonsterNodeList() {

    // Test Print - 1st item displayed will be current state
    list<MonsterNode>::iterator p = m_monsterList.begin();
    int i = 0;
    for (; p != m_monsterList.end(); p++, i++) {
        cerr << "--- Monster Node " << i << " --- " << endl;
        cerr << p->monsterChar << endl;
        cerr << p->monsterRow << endl;
        cerr << p->monsterCol << endl;
        cerr << endl;
    }
}

void Level::printGameObjectList() {

    // Test Print - 1st item displayed will be current state
    list<GameObjectNode>::iterator p = m_gameObjectList.begin();
    int i = 0;
    for (; p != m_gameObjectList.end(); p++, i++) {
        cerr << "--- Object Node " << i << " --- " << endl;
        cerr << p->itemChar << endl;
        cerr << p->itemRow << endl;
        cerr << p->itemCol << endl;
        cerr << endl;
    }

}
void Level::printTestMap()
{
    for (int r = 0; r < 18; r++) {
        for (int c = 0; c < 70; c++) {
            cerr << m_map[r][c];
        }
        cerr << "row: " << r << endl;
    }

}

//void Level::sendInventoryToScreen()
//{
//    //clearScreen(); // *******************************
//
//    list<InventoryNode>::iterator p = m_playerInventory.begin();
//    while (p != m_playerInventory.end())
//    {
//        cout << p->itemSlotLetter << " " << p->itemDescrip << endl;
//        p++;
//    }
//    cout << endl;
//}

void Level::printMap() {
/*
Scan ALL lists - extract positon states of EVERY element - update map

Print game object icons

Print monster icons

Print player icon

Print map
*/    

    // Check if any objects have been picked up or dropped
        // Remove scroll when read and reprint entire object list

    // Retrieve Game Object state information from list of Game Object nodes
    list<GameObjectNode>::iterator o = m_gameObjectList.begin();
    for (; o != m_gameObjectList.end(); o++) {
        // Marks the positions of EVERY game object on the map
        m_map[o->itemRow][o->itemCol] = o->itemChar;
    }  


    // Check if any monsters died
    monsterDeadCheck();
    // Retrieve now UPDATED position of ALL monsters from Monster List
    list<MonsterNode>::iterator m = m_monsterList.begin();
    while (m != m_monsterList.end()) {
        //cerr << "m-Row: " << m->monsterRow << " m->col: " << m->monsterCol << endl;
        // Marks position of EVERY monster on the map
        m_map[m->monsterRow][m->monsterCol] = m->monsterChar;
        m++;
    }

    // Retrieve player's now UPDATED position from state history list
    int newRow = (m_playerStateHistory.front()).playerRow;
    int newCol = (m_playerStateHistory.front()).playerCol;
    // Mark player's current posiiton on map
    m_map[newRow][newCol] = '@';

    clearScreen();

    for (int r = 0; r < 18; r++) {
        for (int c = 0; c < 70; c++) {
            cout << m_map[r][c];
        }
        //cerr << "row: " << r << endl;
        cout << endl;
    }

    // Print Player status bar
    int dungeonLevel = m_levelNumber;
    int playerHP = m_levelToPlayerPtr->getHP();
    int playerArmor = m_levelToPlayerPtr->getArmor();
    int playerStr = m_levelToPlayerPtr->getStr();
    int playerDex = m_levelToPlayerPtr->getDex();
    cout << "      Dungeon Level: " << dungeonLevel << "   HP: " << playerHP << "   Armor: " << playerArmor << "   Str: " << playerStr << "   Dex: " << playerDex;
    cout << endl << endl;

    // Print activity messages
    list<string>::iterator msg = m_msgList.begin();
    //list<string>::iterator f = m_msgList.begin();
    while (msg != m_msgList.end()) {
        cout << *msg << endl;

        msg->erase(); // DOESNT pop element

        //msg = m_msgList.erase(msg); // also doesnt pop element

        msg++; // ** appparanelty this is undefined behavior
        m_msgList.pop_front();
    }


    //printMonsterNodeList();

    //printGameObjectList();

    // print inventory 


    //cerr << "getMonsterLimit: " << this->getMonsterLimit() << endl;
    //cerr << endl;
}










// ######################################################
// ################## SCRAP #############################
/*
===== autmoated onster gen =========


        //int randRow = randInt(1, 17);
        //int randCol = randInt(1, 69);

        //while (m_map[randRow][randCol] != ' ' || (randRow > 17 || randCol > 69) ) {
        //    //spawnCoordinates = generateMonsterSpawnPos();
        //    randRow = randInt(1, 17);
        //    randCol = randInt(1, 69);
        //}

        //spawnCoordinates.monsterRow = randRow;
        //spawnCoordinates.monsterCol = randCol;

        //cerr << "spawnCoordinates.monsterRow: " << spawnCoordinates.monsterRow << endl;
        //cerr << "spawnCoordinates.monsterCol: " << spawnCoordinates.monsterCol << endl;

        //newBornMonster.monsterRow = spawnCoordinates.monsterRow;
        //newBornMonster.monsterCol = spawnCoordinates.monsterCol; // ** wont need these 2


        //cerr << "newBornMonster.monsterRow: " << newBornMonster.monsterRow << endl;
        //cerr << "newBornMonster.monsterCol: " << newBornMonster.monsterCol << endl;


============
char** Level::getMap() { //********** WONT NEED - don't break encapsulation
    //char* p = &m_map[0][0];

    char** ptrToMap;
    ptrToMap = m_map;

    return ptrToMap;
}



*/



//// Add Goblins
//    // Add to level map
//int L0_Goblin0_RightRoomRow = randInt((RightRoomUpperHorizontalWall + 1), (RightRoomLowerHorizontalWall - 1));
//int L0_Goblin0_RightRoomCol = randInt((RightRoomLeftVerticalWall + 1), (RightRoomRightVerticalWall - 1));
//m_map[L0_Goblin0_RightRoomRow][L0_Goblin0_RightRoomCol] = 'G';
//    // Add to level's monster list
//MonsterNode L0_Goblin0_RightRoom;
//L0_Goblin0_RightRoom.monsterChar = 'G';
//L0_Goblin0_RightRoom.monsterRow = L0_Goblin0_RightRoomRow;
//L0_Goblin0_RightRoom.monsterCol = L0_Goblin0_RightRoomCol;
//this->addNodeToMonsterList(L0_Goblin0_RightRoom);
//
//// Add Snakewomen
//    // Add to level map
//int L0_Snakewoman0_MidRoomRow = randInt((MidRoomUpperHorizontalWall + 1), (MidRoomLowerHorizontalWall - 1));
//int L0_Snakewoman0_MidRoomCol = randInt((MidRoomLeftVerticalWall + 1), (MidRoomRightVerticalWall - 1));
//m_map[L0_Snakewoman0_MidRoomRow][L0_Snakewoman0_MidRoomCol] = 'S';
//// Add to level's monster list
//MonsterNode L0_Snakewoman0_MidRoom;
//L0_Snakewoman0_MidRoom.monsterChar = 'S';
//L0_Snakewoman0_MidRoom.monsterRow = L0_Snakewoman0_MidRoomRow;
//L0_Snakewoman0_MidRoom.monsterCol = L0_Snakewoman0_MidRoomCol;
//this->addNodeToMonsterList(L0_Snakewoman0_MidRoom);



//MonsterNode Level::generateMonsterSpawnPos() {
//    cerr << "--- generateMonsterSpawnPos() CALLED --- " << endl;
//
//    //MonsterNode spawnCoords;
//    //int spawnRow = randInt(1, 17);
//    //int spawnCol = randInt(1, 69);
//
//    //int spawnRow;
//    //int spawnCol;
//
//    //int spawnRow = randInt(1, 17);
//    //int spawnCol = randInt(1, 69);
//
//    //int spawnRow = rand() % 17 + 1;
//    //int spawnCol = rand() % 69 + 1;
//
//    //int randRow = randInt(1, 17);
//    //int randCol = randInt(1, 69);
//
//    //spawnCoords.monsterRow = randRow;
//    //spawnCoords.monsterCol = randCol;
//
//    //return spawnCoords;
//
//    //if (m_map[spawnRow][spawnCol] != ' ') {
//    //    spawnCoords = generateMonsterSpawnPos();    
//    //}
//    //
//    //if(spawnRow >= 18 || spawnCol >= 70 )
//    //    spawnCoords = generateMonsterSpawnPos();
//
//
//    //if (spawnRow < 18 && spawnCol < 70 && m_map[spawnRow][spawnCol] == ' ') {
//    //    spawnCoords.monsterRow = spawnRow;
//    //    spawnCoords.monsterCol = spawnCol;
//    //    //spawnCoords.monsterPtr = nullptr;
//    //    cerr << "returning spawn cords" << "spawnRow: " << spawnRow << " spawnCol: " << spawnCol << endl;
//
//        //return spawnCoords;
//    //}
//
//    //else 
//    //    //(m_map[spawnRow][spawnRow] == '#') // add mosnter icons******
//    //{
//    //    cerr << " - recursive call to genMonstSpawnPos() - " << endl;
//    //}
//
//
//}


/*

        //for (int i = 0; i < numberOfCooridorsExtendingFromARoom; i++)
        {

            //switch (northSouth)
            //{
            //case 'N' : // Vertical cooridor extending from NORTH wall
            //    cooridorUpperRowBound = upperRowBound - cooridorLength;
            //    cooridorLowerRowBound = upperRowBound;

            //    cooridorLeftColBound = randInt(leftColBound, rightColBound);
            //    cooridorRightColBound = cooridorLeftColBound + 1;
            //    break;

            //case 'S' : // Vertical cooridor extending from SOUTH wall
            //    cooridorUpperRowBound = lowerRowBound + cooridorLength;
            //    cooridorLowerRowBound = lowerRowBound;

            //    cooridorLeftColBound = randInt(leftColBound, rightColBound);
            //    cooridorRightColBound = cooridorLeftColBound + 1;
            //    break;
            //}


            //switch (westEast)
            //{
            //case 'W' : // Horizontal cooridor extending from room's EAST wall
            //    cooridorUpperRowBound = randInt(upperRowBound, lowerRowBound);
            //    cooridorLowerRowBound = cooridorUpperRowBound + 1;

            //    cooridorLeftColBound = rightColBound;
            //    cooridorRightColBound = rightColBound + cooridorLength;
            //    break;

            //case 'E' : // Horizontal cooridor extending from WEST wall
            //    cooridorUpperRowBound = randInt(upperRowBound, lowerRowBound);
            //    cooridorLowerRowBound = cooridorUpperRowBound + 1;

            //    cooridorLeftColBound = leftColBound - cooridorLength;
            //    cooridorRightColBound = leftColBound;
            //    break;
            //}

            //// Put cooridor on map
            //correctBounds(cooridorUpperRowBound, cooridorLowerRowBound, cooridorLeftColBound, cooridorRightColBound);
            //for (int r = cooridorUpperRowBound; r < cooridorLowerRowBound; r++)
            //{
            //    for (int c = cooridorLeftColBound; c < cooridorRightColBound; c++)
            //    {
            //        m_map[r][c] = ' ';
            //    }
            //}



        } // END COORIDOR ATTACHMENT LOOP

*/



/*



        //int wallCooridorWillExtendFrom = randInt(0, 3); // Select a random wall to attach cooridor to
        //// Attach a random number of cooridors to the room
        //int numberOfCooridorsExtendingFromARoom = randInt(1, 2);





        //// Generate randoom room
        //// Find a random wall element to be the upper left corner of the new room
        //roomUpperLeftCorner = selectRandWallElement();
        //height = randInt(4, 15); // random height
        //width = randInt(4, 15); // random width

        //upperRowBound = roomUpperLeftCorner.row;
        //lowerRowBound = upperRowBound + height;
        //leftColBound = roomUpperLeftCorner.col;
        //rightColBound = leftColBound + width;
        //// Bounds correction
        //correctBounds(upperRowBound, lowerRowBound, leftColBound, rightColBound);
        //// Put room on map
        //for (int r = upperRowBound; r < lowerRowBound; r++)
        //{
        //    for (int c = leftColBound; c < rightColBound; c++)
        //    {
        //        m_map[r][c] = ' ';
        //    }
        //}


*/


/*
Original test LEVEL

    // ############################################## OLD ROOMS #########
    //// Make long horizontal cooridor
    //int L0HorizCooridorTailRow = 10;
    //int L0HorizCooridorTailCol = 67;
    //int L0HorizCooridorHeadRow = 10;
    //int L0HorizCooridorHeadCol = 1;
    //for (int r = 10; r < 11; r++) {
    //    for (int c = 1; c < 69; c++) {
    //        m_map[r][c] = ' ';
    //    }
    //}

    //// Make room - LEFT
    //int leftRoomUpperHorizontalWall = randInt(1, 3);
    //int leftRoomLowerHorizontalWall = randInt(15, 17);
    //int LeftRoomLeftVerticalWall = randInt(2, 4);
    //int LeftRoomRightVerticalWall = randInt(18, 20);
    //int LeftRoomMidRow = (leftRoomUpperHorizontalWall + leftRoomLowerHorizontalWall) / 2;
    //int LeftroomMidCol = (LeftRoomLeftVerticalWall + LeftRoomRightVerticalWall) / 2;

    //for (int r = leftRoomUpperHorizontalWall; r < leftRoomLowerHorizontalWall; r++) {
    //    for (int c = LeftRoomLeftVerticalWall; c < LeftRoomRightVerticalWall; c++) {
    //        m_map[r][c] = ' ';
    //    }
    //}
    //
    //// Make room - MID
    //int MidRoomUpperHorizontalWall = randInt(5, 7);
    //int MidRoomLowerHorizontalWall = randInt(15, 17);
    //int MidRoomLeftVerticalWall = randInt(22, 24);
    //int MidRoomRightVerticalWall = randInt(45, 48);
    //
    //int MidRoomMidRow = (MidRoomUpperHorizontalWall + MidRoomLowerHorizontalWall) / 2;
    //int MidroomMidCol = (MidRoomLeftVerticalWall + MidRoomRightVerticalWall) / 2;
    //
    //for (int r = MidRoomUpperHorizontalWall; r < MidRoomLowerHorizontalWall; r++) {
    //    for (int c = MidRoomLeftVerticalWall; c < MidRoomRightVerticalWall; c++) {
    //        m_map[r][c] = ' ';
    //    }
    //}
    //
    //// Make room - RIGHT
    //int RightRoomUpperHorizontalWall = randInt(7, 8);
    //int RightRoomLowerHorizontalWall = randInt(12, 15);
    //int RightRoomLeftVerticalWall = randInt(50, 52);
    //int RightRoomRightVerticalWall = randInt(60, 68);
    //
    //int RightRoomMidRow = (RightRoomUpperHorizontalWall + RightRoomLowerHorizontalWall) / 2;
    //int RightRoomMidCol = (RightRoomLeftVerticalWall + RightRoomRightVerticalWall) / 2;
    //
    //for (int r = RightRoomUpperHorizontalWall; r < RightRoomLowerHorizontalWall; r++) {
    //    for (int c = RightRoomLeftVerticalWall; c < RightRoomRightVerticalWall; c++) {
    //        m_map[r][c] = ' ';
    //    }
    //}

    // ###################################### OLD ROOMS END #########





*/




//void Level::createOLDLvl0Map() {

//    //Fill level entirely with wall
//    for (int r = 0; r < 18; r++) {
//        for (int c = 0; c < 70; c++) {
//            m_map[r][c] = '#';
//        }
//    }
//
//    // Make long horizontal cooridor
//    int L0HorizCooridorTailRow = 10;
//    int L0HorizCooridorTailCol = 67;
//    int L0HorizCooridorHeadRow = 10;
//    int L0HorizCooridorHeadCol = 1;
//    for (int r = 10; r < 11; r++) {
//        for (int c = 1; c < 69; c++) {
//            m_map[r][c] = ' ';
//        }
//    }
//
//    // Make room - LEFT
//    int leftRoomUpperHorizontalWall = randInt(1, 3);
//    int leftRoomLowerHorizontalWall = randInt(15, 17);
//    int LeftRoomLeftVerticalWall = randInt(2, 4);
//    int LeftRoomRightVerticalWall = randInt(18, 20);
//    int LeftRoomMidRow = (leftRoomUpperHorizontalWall + leftRoomLowerHorizontalWall) / 2;
//    int LeftroomMidCol = (LeftRoomLeftVerticalWall + LeftRoomRightVerticalWall) / 2;
//
//    for (int r = leftRoomUpperHorizontalWall; r < leftRoomLowerHorizontalWall; r++) {
//        for (int c = LeftRoomLeftVerticalWall; c < LeftRoomRightVerticalWall; c++) {
//            m_map[r][c] = ' ';
//        }
//    }
//
//    // Make room - MID
//    int MidRoomUpperHorizontalWall = randInt(5, 7);
//    int MidRoomLowerHorizontalWall = randInt(15, 17);
//    int MidRoomLeftVerticalWall = randInt(22, 24);
//    int MidRoomRightVerticalWall = randInt(45, 48);
//
//    int MidRoomMidRow = (MidRoomUpperHorizontalWall + MidRoomLowerHorizontalWall) / 2;
//    int MidroomMidCol = (MidRoomLeftVerticalWall + MidRoomRightVerticalWall) / 2;
//
//    for (int r = MidRoomUpperHorizontalWall; r < MidRoomLowerHorizontalWall; r++) {
//        for (int c = MidRoomLeftVerticalWall; c < MidRoomRightVerticalWall; c++) {
//            m_map[r][c] = ' ';
//        }
//    }
//
//    // Make room - RIGHT
//    int RightRoomUpperHorizontalWall = randInt(7, 8);
//    int RightRoomLowerHorizontalWall = randInt(12, 15);
//    int RightRoomLeftVerticalWall = randInt(50, 52);
//    int RightRoomRightVerticalWall = randInt(60, 68);
//
//    int RightRoomMidRow = (RightRoomUpperHorizontalWall + RightRoomLowerHorizontalWall) / 2;
//    int RightRoomMidCol = (RightRoomLeftVerticalWall + RightRoomRightVerticalWall) / 2;
//
//    for (int r = RightRoomUpperHorizontalWall; r < RightRoomLowerHorizontalWall; r++) {
//        for (int c = RightRoomLeftVerticalWall; c < RightRoomRightVerticalWall; c++) {
//            m_map[r][c] = ' ';
//        }
//    }
//
//
//    // === L0 Actor Placement ===  -    *** UPDATE ACTOR LISTS AFTER EVERY COMMAND   
//    // Player Spawn
//        // Add icon to level map
//    int L0_playerSpawnRow = randInt((leftRoomUpperHorizontalWall + 1), (leftRoomLowerHorizontalWall - 1));
//    int L0_playerSpawnCol = randInt((LeftRoomLeftVerticalWall + 1), (LeftRoomRightVerticalWall - 1));
//    m_map[L0_playerSpawnRow][L0_playerSpawnCol] = '@';
//    // Add to level's player list
//    PlayerNode L0_player;
//    L0_player.playerChar = '@';
//    L0_player.playerRow = L0_playerSpawnRow;
//    L0_player.playerCol = L0_playerSpawnCol;
//    L0_player.playerPtr = player;
//    this->addNodeToPlayerStateHistory(L0_player); // MUST add NEW NODE for EVERY position update
//
//    // Test Print
//    //printPlayerNodeList();    
//
//
//// Automated monster generation
//    // Each generated monster get it's own MonsterNode - containts monster data and POINTER to dynamic monster object
//    // Each monster node is pushed onto the level's Monster List
//
//    // Set Level's idiosyncratic monster population weights
//    m_S = .2 * m_M;
//    m_G = .8 * m_M;
//    m_B = 0 * m_M;
//    m_D = 0 * m_M;
//
//    // Generate Snakewomen
//    string monsterType = "snakeWoman";
//    string monsterID = "";
//
//    for (int i = 0; i < m_S; i++) {
//        MonsterNode newBornMonster;
//
//        // Generate monster identifiers
//        string istring = to_string(i);
//        monsterID = monsterType + istring;
//        newBornMonster.monsterID = monsterID;
//        newBornMonster.monsterChar = 'S';
//
//        // Generate monster spawn position
//        CoordinatePair randomSpawnPos = generateRandPos();
//        newBornMonster.monsterRow = randomSpawnPos.row;
//        newBornMonster.monsterCol = randomSpawnPos.col;
//
//        // Allocate dynamic monster object
//        newBornMonster.monsterPtr = new Snakewoman(); // ************
//
//        // Push new monster to Level's Monster List
//        addNodeToMonsterList(newBornMonster);
//    }
//
//    // Generate Goblins
//    monsterType = "goblin";
//    monsterID = "";
//
//    for (int i = 0; i < m_G; i++) {
//        MonsterNode newBornMonster;
//
//        // Generate monster identifiers
//        string istring = to_string(i);
//        monsterID = monsterType + istring;
//        newBornMonster.monsterID = monsterID;
//        newBornMonster.monsterChar = 'G';
//
//        // Generate monster spawn position
//        CoordinatePair randomSpawnPos = generateRandPos();
//        newBornMonster.monsterRow = randomSpawnPos.row;
//        newBornMonster.monsterCol = randomSpawnPos.col;
//
//        // Allocate dynamic monster object
//        newBornMonster.monsterPtr = new Goblin(); // ********
//
//        // Push new monster to Level's Monster List
//        addNodeToMonsterList(newBornMonster);
//    }
//
//    //cerr << "m_monsterList.size() : " << m_monsterList.size() << endl;
//
//
//    // Add Bogeymen - NONE on this level
//
//    // Add Dragons - NONE on this level
//
//    // Test print
//    //printMonsterNodeList();
//
//
//
//    // === L0 GameObject Placement ===
//    // Weapon in Left Room
//    int L0_Weapon1Row = L0HorizCooridorHeadRow;
//    int L0_Weapon1Col = randInt((L0HorizCooridorHeadCol + 1), (LeftRoomRightVerticalWall - 1));
//    m_map[L0_Weapon1Row][L0_Weapon1Col] = ')'; // Update map
//    GameObjectNode L0_Weapon1Node;
//    L0_Weapon1Node.itemChar = ')';
//    L0_Weapon1Node.itemRow = L0_Weapon1Row;
//    L0_Weapon1Node.itemCol = L0_Weapon1Col;
//    this->addNodeToGameObjectList(L0_Weapon1Node); // Update list
//
//    // Scroll in Mid Room
//    int L0_Scroll_MidRoomRow = randInt((MidRoomUpperHorizontalWall + 1), (MidRoomLowerHorizontalWall - 1));
//    int L0_Scroll_MidRoomCol = randInt((MidRoomLeftVerticalWall + 1), (MidRoomRightVerticalWall - 1));
//    m_map[L0_Scroll_MidRoomRow][L0_Scroll_MidRoomCol] = '?'; // Update map
//    GameObjectNode L0_Scroll_MidRoomNode;
//    L0_Scroll_MidRoomNode.itemChar = '?';
//    L0_Scroll_MidRoomNode.itemRow = L0_Scroll_MidRoomRow;
//    L0_Scroll_MidRoomNode.itemCol = L0_Scroll_MidRoomCol;
//    this->addNodeToGameObjectList(L0_Scroll_MidRoomNode); // Update list
//
//    // Scroll in Right Room
//    int L0_Scroll_RightRoomRow = randInt((RightRoomUpperHorizontalWall + 1), (RightRoomLowerHorizontalWall - 1));
//    int L0_Scroll_RightRoomCol = randInt((RightRoomLeftVerticalWall + 1), (RightRoomRightVerticalWall - 1));
//    m_map[L0_Scroll_RightRoomRow][L0_Scroll_RightRoomCol] = '?'; // Update map
//    GameObjectNode L0_Scroll_RightRoomNode;
//    L0_Scroll_RightRoomNode.itemChar = '?';
//    L0_Scroll_RightRoomNode.itemRow = L0_Scroll_RightRoomRow;
//    L0_Scroll_RightRoomNode.itemCol = L0_Scroll_RightRoomCol;
//    this->addNodeToGameObjectList(L0_Scroll_RightRoomNode); // Update list
//
//    // Descend icon in Right Room
//    m_map[L0HorizCooridorTailRow][L0HorizCooridorTailCol] = '>'; // Update map
//    GameObjectNode L0_DescendNode;
//    L0_DescendNode.itemChar = '>';
//    L0_DescendNode.itemRow = L0HorizCooridorTailRow;
//    L0_DescendNode.itemCol = L0HorizCooridorTailCol;
//    this->addNodeToGameObjectList(L0_DescendNode); // Update list





    // print to verify object list
    //this->printGameObjectList();   



    //printMap();

    //cerr << "left room mid (r, c): " << LeftRoomMidRow << ", " << LeftroomMidCol << endl;
    //cerr << "left room mid (r, c): " << MidRoomMidRow << ", " << MidroomMidCol << endl;
    //cerr << "left room mid (r, c): " << RightRoomMidRow << ", " << RightRoomMidCol << endl;
    //cerr << endl;
    //cerr << endl;
    //
    //cerr << "getMonsterLimit: " << this->getMonsterLimit() << endl;


//}
