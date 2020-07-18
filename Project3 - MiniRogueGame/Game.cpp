// Game.cpp

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
// Implement these and other Game member functions you may have added.

#include <iostream>
#include <string>
#include<list>

#include "utilities.h"
#include "Game.h"
#include "Dungeon.h"
#include "Actor.h"
#include "GameObject.h"

using namespace std;

Game::Game(int goblinSmellDistance)
    : m_goblinSmellDist(goblinSmellDistance)

{
    //setCurrLevelPtr(0, m_lvlPtrArray);
        //m_currLevelPtr now points to Level 0

    // Create player object on HEAP
    m_gameToPlayerPtr = new Player(this);
        // NO OTHER PLAYER OBJECT WILL EVER BE CREATED

    // Generate level 0 on HEAP
    int currLevel = 0;
    m_gameToCurrLevelPtr = new Level(currLevel, m_gameToPlayerPtr   , this   );

    // Set Player's currLevel pointer to newly concstructed Level
    m_gameToPlayerPtr->getPlayerCurrLevelPtr() = m_gameToCurrLevelPtr;
}

Game::~Game() {

    delete m_gameToPlayerPtr;
    delete m_gameToCurrLevelPtr;

}

void Game::play()
{    

    cout << "Press q to exit game." << endl;
    //int count = 0;
    bool playerIsAlive = m_gameToPlayerPtr->isAlive();

    while (playerIsAlive) // While player is alive
    //count++;
        //char command = ' ';
        switch (getCharacter())
        {
        case 'k':
            m_gameToPlayerPtr->move('U'); // UP
            break;
        case 'j':
            m_gameToPlayerPtr->move('D'); // DOWN
            break;
        case 'h':
            m_gameToPlayerPtr->move('L'); // LEFT
            break;
        case 'l':
            m_gameToPlayerPtr->move('R'); // RIGHT
            break;
        case 'g':
            m_gameToPlayerPtr->pickup();
            break;
        case 'w':
            m_gameToPlayerPtr->sendInventoryToScreen();
            m_gameToPlayerPtr->swapWeapon(getCharacter());
            break;
        case 'r':
            m_gameToPlayerPtr->sendInventoryToScreen();
            m_gameToPlayerPtr->readScroll(getCharacter());
            break;
        case 'i':
            m_gameToPlayerPtr->sendInventoryToScreen();
            break;
        case '>':
            m_gameToPlayerPtr->descend();
            break;
        case 'q':
            cerr << "Quitting" << endl;
            return;
        case 'c':
            m_gameToPlayerPtr->cheat();
            break;

        default:
            cerr << "Invalid command" << endl;
            break;
        }    
        //m_gameToCurrLevelPtr->addMsgtoMsgList("count: " + to_string(count));
    ;


    /*
    ### SCRAP ###
        //while (getCharacter() != 'q')
    //    ;


        //player.move(lvl0.m_map, "UP");
    //(*ptrToMap)[1][3]; // *******
    //*(ptrToMap + 1 * 70 + 3) = '@';
    //char** ptrToMap = m_gameToCurrLevelPtr->getMap();
    //ptrToMap[1][3] = '(';
    //m_gameToPlayerPtr->move(m_gameToCurrLevelPtr->getMap(), 'U');
    */

}



Player* Game::getPlayerPtr()
{
    return m_gameToPlayerPtr;
}

Level* Game::getLevelPtr()
{
    return m_gameToCurrLevelPtr;
}

Game* Game::getGamePtr()
{
    return this;
}


void Game::deleteCurrLevel()
{

    delete m_gameToCurrLevelPtr;


}


void Game::setLevelptr(Level* newLevel)
{
    m_gameToCurrLevelPtr = newLevel;
}


void Game::setPlayerPtr(Player* player)
{
    m_gameToPlayerPtr = player;
}

int Game::getGoblinSmellDist()
{

    return m_goblinSmellDist;

}


//void Game::generateNextLevel(int const& levelNumber, Level* m_levelPtrArray[5]) {
//
//
//
//}




