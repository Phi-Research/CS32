// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

//#include "Dungeon.h"
//#include "Actor.h"

//class Dungeon; // Dont need partially defined base classes
class Level;
//class Actor;
class Player;

// You may add data members and other member functions to this class.

class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game();
    void play();

    Player* getPlayerPtr();
    Level* getLevelPtr();
    Game* getGamePtr();

    void setLevelptr(Level* newLevel);
    void setPlayerPtr(Player* player);

    void deleteCurrLevel();

    int getGoblinSmellDist();


private:

    int m_goblinSmellDist;

    //Dungeon* m_dungeonPtr; // Entire program only has ONE dungeon object - this is a pointer to it

    Level* m_gameToCurrLevelPtr; // Serves as pointer to CURRENT level
        // Game's constructor 
    Player* m_gameToPlayerPtr; // Entire program only has ONE player object - this is a pointer to it


};




#endif // GAME_INCLUDED
