#ifndef DUNGEON_H
#define DUNGEON_H

#include <iostream>
#include <string>
#include <list>

//#include "Actor.h"

class GameObject;

class Actor;
class Player;
class Monster;
class Game;

using namespace std;

struct PlayerNode {
    char playerChar = '@';
    int playerRow;
    int playerCol;
    int playerSleepTime;
    Player* playerPtr = nullptr;
};

struct MonsterNode {
    string monsterID;
    char monsterChar;
    int monsterRow;
    int monsterCol;
    int monsterSleepTime;
    Monster* monsterPtr = nullptr;
    int smellDistance = 1;
    bool playerSmelt = false;
};

struct GameObjectNode {
    char itemChar;
    int itemRow;
    int itemCol;
    GameObject* itemPtr = nullptr;
};

struct CoordinatePair {
    int row;
    int col;
};

struct InventoryNode {
    char itemSlotLetter;
    string itemDescrip;
    GameObject* itemPtr = nullptr;
};

class Dungeon {

public:
    Dungeon();
    ~Dungeon();

    //Actor* player(); // returns pointer to dynamic player object
        // player object gets recreated before every frame update


    //list<GameObjectNode> L0_GameObjectList;

private:

    //Player* m_player;                         

    // Maps

    // Player

    // Monster Lists

    // GameObject Lists
    //list<GameObjectNode> LCurr_GameObjectList;
    //list<GameObjectNode> L1_GameObjectList;
    //list<GameObjectNode> L2_GameObjectList;
    //list<GameObjectNode> L3_GameObjectList;
    //list<GameObjectNode> L4_GameObjectList;
};

// ==============================================================================================================================================================================

class Level : public Dungeon {


public:

    Level();
    //Level(int levelNumber, Player* player);
    Level(int levelNumber, Player* player, Game* game);


    //void constructNextLevel(int levelNumber, int m_monsterLimit, Player* player);
    void constructNextLevel(int levelNumber, int m_monsterLimit, Player* player, Game* game);
    bool pathExists(char tempMap[18][70], int sr, int sc, int er, int ec);
    bool pathExistsWithinStepCount(char tempMap[18][70], int sr, int sc, int er, int ec, int stepsRemaining);

    bool pathExistsWithinStepCount(char tempMap[18][70], int sr, int sc, int& nr, int& nc, int er, int ec,  int stepsRemaining, int &minPath);


    void printTestMap();
    CoordinatePair selectRandWallElement();
    void correctBounds(int& upperRowBound, int& lowerRowBound, int& leftColBound, int& rightColBound);
        // corrects bounds of randomly dimensioned rooms to stay within map bounds
    virtual ~Level();

    int getLevelNumber(); // Return number identifier of level
    int getMonsterLimit() const;
    void setCharAtCoordinate(int r, int c, char newState);
    char getCharAtCoordinate(int r, int c);

    void addNodeToPlayerStateHistory(PlayerNode NodeToAdd);
    void addNodeToMonsterList(MonsterNode NodeToAdd);
    void addNodeToGameObjectList(GameObjectNode NodeToAdd);
    void addMsgtoMsgList(string const &msg);

    void printGameObjectList();
    void printPlayerNodeList();
    void printMonsterNodeList();
    //void sendInventoryToScreen();


    void setPlayerCoordinates(int const &newRow, int const &newCol);
    PlayerNode getPlayerCoordinates();
    Player* getLevelToPlayerPtr(); // Returns Level's pointer to the HEAP's player object


    bool checkForWallAtPosition(int const &newRow, int const &newCol) const;
        // Returns TRUE if there is a '#' @parameter coordinates
    bool checkForMonsterAtPosition(int const& newRow, int const& newCol) const;
        // Returns TRUE if there is a MONSTER character @parameter coordinates

    CoordinatePair generateRandEmptyPos();
    
    void rcvAttackFromPlayer(int const& targetRow, int const& targetCol);
    void monsterDeadCheck();

    void getMonsterAttacks(); // Monsters attack if player is adjacent
    void getMonsterMoves(); // Moves monsters after player turn


    void deleteItemFromGameObjectList(const int& itemRow, const int &itemCol);
    void deleteItemFromIventory();


    void monsterHPRegain();
    
    GameObjectNode getObjectNodeFromLevel(); // returns object node from level's game object list

    void teleportPlayer(); // Executes teleportaton scroll

    void printMap();
        // Is called AFTER all UPDATES are complete and prints new map state to screen

private:

    char m_map[18][70]; // Map of the level that's continuously updated and printed
    //char **m_map; // Map of the level that's continuously updated and printed

    Player* m_levelToPlayerPtr;
    //Level* m_currLevelPtr; // Fucntions same as THIS pointer
    Game* m_levelToGamePtr;

    int m_levelNumber;
    // Maximum number of monsters allowed on each level
        // Minimum monsters on each level is ALWAYS 2
    int m_monsterUpperLimit;

    // Monster population weights - idosyncratic to EACH level
    float m_M; // TOTAL number of monsters - random variable
    float m_S; // Number of Snakewomen
    float m_G; // Number of Goblins
    float m_B; // Number of Bogeymen
    float m_D; // Number of Dragons

    list<PlayerNode> m_playerStateHistory; // list object that stores player's coordinates
    list<MonsterNode> m_monsterList; // List of all monsters on the level
    list<GameObjectNode> m_gameObjectList; // List of all game objects on the level
    list<string> m_msgList; // List of all messages from game activity

    list<InventoryNode> m_playerInventory;

};


// ========= Reference from previous project =====================
//class arena
//{
//public:
//    // constructor/destructor
//    arena(int nrows, int ncols);
//    ~arena();
//
//    // accessors
//    int      rows() const;
//    int      cols() const;
//
//    player* player() const; // returns pointer to player object - return memory address where player object is stored
//    int      vampirecount() const;
//    int      getcellstatus(int r, int c) const;
//    int      numberofvampiresat(int r, int c) const;
//    void     display(std::string msg) const;
//    history& history(); // returns reference to history object m_history - what does m_history do?
//
//    // mutators
//    void setcellstatus(int r, int c, int status);
//    bool addvampire(int r, int c);
//    bool addplayer(int r, int c);
//    void movevampires();
//
//private:
//    int      m_grid[maxrows][maxcols];
//    int      m_rows;
//    int      m_cols;
//    player* m_player; // pointer to player object address - on stack
//    vampire* m_vampires[maxvampires]; // pointer to vampire object array address - on stack
//    int      m_nvampires;
//    int      m_turns;
//    history  m_history; // history object on stack
//
//    // helper functions
//    void checkpos(int r, int c, std::string functionname) const;
//    bool isposinbounds(int r, int c) const;
//};

#endif DUNGEON_H