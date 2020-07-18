#ifndef	ACTOR_H
#define ACTOR_H

#include<string>

//#include "Dungeon.h"

using namespace std;

//class Dungeon {};
class Level;
class GameObject;
class Weapon;
class Game;

struct InventoryNode;

/*
Actor Class Hiercharchy
    Tier 1
        Actor
    Tier 2
        Player
        Monsters
            Snakewoman
            Goblin
            Bogeyman
            Dragon
*/

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// ABC - Tier 1 - Actor Base

class Actor {
    /*
    hitDefenderProb();
        attackerPoints = attackerDexterity + weaponDexterityBonus;
        defenderPoints = defenderDexterity + defenderArmorPoints;
        if (randInt(1, attackerPoints) >= randInt(1, defenderPoints))
            attacker has hit defender

    dmgDealt()
        damagePoints = randInt(0, attackerStrength + weaponDamageAmount - 1);
    */

public:
    Actor(); // ZP - C 
    Actor(int HP, int armor, int str, int dex, int sleepTimer); // OL - C
    virtual ~Actor(); // D

    void move();

    // MAKE PURE VIRTUAL ***
    virtual int getHP() const; // returns m_HP
    virtual int getArmor() const; // returns m_armor
    virtual int getStr() const; // returns m_str
    virtual int getDex() const; // returns m_dex
    virtual int getSleepTime() const; // returns m_sleepTimer

    virtual int getWpnDexBonusActr() const; // returns dex bonus given by weilded weapon
    virtual int getWpnDmgActr() const; // returns weapon damage 
        // ** MAKE PV

    virtual void setHP(int const &HPChange); // adds HPChange to HP - NEGATIVE values will DECREASE HP
        // *** make PV

    void getAttackSuccessProb(); // calcs percent chance of hitting the opponent
    void getDodgeProb(); // calcs percent chance of doding opponent's attack

    int calcHPDmgDealt(); // Calc total player attack damage
    int calcHPDmgRcvd(); // Updates m_HP after received attack

    virtual bool isAlive() const; // checks m_HP after EVERY attack to see if actor is still alive
        // ***MAKE PV

    virtual void regainHP(); // 1/ 10 chance +1 HP before each turn 
        //overwritten by player and dragon

private:
    int m_HP;
    int m_armor;
    int m_str;
    int m_dex;
    int m_sleepTime;

    Weapon* m_wpnWielded;

    //int m_wpnDexBonus;

    //GameObject* m_weapon;
    //GameObject* m_scroll;
    /*
    Use dynamic cast to know if GameObject points to a scroll or a weapon
    */

};

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// Tier 2 - Player

class Player : public Actor {

public:

    Player(Game* ptrToGameObject); // ZP - C
    //Player(int HP, int armor, int str, int dex, int sleepTime); // C
    virtual ~Player(); // D

    void move(char const direction);


    Level*& getPlayerCurrLevelPtr(); //Returns REFERENCE to player's pointer to the CURRENT level

    void setHPCapacity(const int &HPCapacityIncrement);
    virtual int getHP() const; // returns m_HP
    virtual int getArmor() const; // returns m_armor
    virtual int getStr() const; // returns m_str
    virtual int getDex() const; // returns m_dex
    virtual int getSleepTime() const; // returns m_sleepTimer
    virtual void setSleepTime(int newSleepTime);
    
    string getWpnTypeP() const; // returns weapon type weilded by player
    virtual int getWpnDexBonusActr() const; // returns dex bonus given by weilded weapon
    virtual int getWpnDmgActr() const; // returns weapon damage

    virtual void setHP(const int& HPChange); // adds HPChange to HP - NEGATIVE values will DECREASE HP
    virtual int getWpnSleepTimeActr() const; // returns sleep time inflicted by wielded magic fangs
    virtual bool isAlive() const; // checks m_HP after EVERY attack to see if actor is still alive

    void pickup();

    void sendInventoryToScreen();
    //void sendInventoryToScreen(char command);
    void readScroll(char inventorySlotSelected);
    void swapWeapon(char inventorySlotSelected); // swaps m_currWeapon with weapon in inventory
    

    Game* getPtrToGame();

// ===== to make
    void descend();
    void quit();
    void cheat();

    void regainHP();
    //void setPlayerCurrLevelPtr(Level* currLevel);
    

private:
    int m_HPCapacity;
    int m_HP;
    int m_armor;
    int m_str;
    int m_dex;
    int m_sleepTime;

    string m_currWeapon;
    Level* m_playertoCurrLevelPtr; // Player's pointer to CURRENT level
    Weapon* m_wpnWielded;
    string m_actorType;
    Game* m_playerToGamePtr;
    list<InventoryNode> m_inventory;

   //inventory* m_inventory; // used to print player inventory - current weapon AND items player has picked up
       // destruct items when used/read
        // MAX_ITEMS = 25;
       /// inventory slots labeled a - z
};


// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// Tier 2 - Monsters

class Monster : public Actor {

public:
    Monster(); // ZP - C
    //Monster(int HP, int armor, int str, int dex, int sleepTimer);
    virtual  ~Monster(); // D

    virtual void move(); // monster movement post-player move
    virtual void attack(); // attack if NEXT to player
    virtual string drop() const; // drop item upon death
        // returns EMPTY string if no item is dropped
        // **** make PV

    virtual string getWpnTypeM(); // returns mosnter's wielded weapon type

    virtual int getWpnSleepTimeActr() const; // return weapon's sleep time induced upon an attack
        // Virtual - retunrs 0 - OVERWRITTEN ONLY by Snakewoman class - ******** make PV - also overwrittne by player

    virtual void regainHP(); // 1/ 10 chance +1 HP before each turn 

    //virtual void setHP(int const& HPChange); // sets m_HP - following attack or HP gain

private:
    string m_currWeapon;

};



// ===========================================================================================================================================================================

class Snakewoman : public Monster {
    // Appear on ANY dungeon level
    // Similar to Bogeyman
    // smells player within 3 steps (obstructed or not)
    // MOVES ONLY IFF player is smelt
    // Other monsters can obstrcut Snakewomen
    // CANNOT NAVIGATE the maze - moves only absolutely

public:
    Snakewoman(); // ZP - C
    virtual ~Snakewoman(); // D

    virtual int getHP() const; // returns m_HP
    virtual int getArmor() const; // returns m_armor
    virtual int getStr() const; // returns m_str
    virtual int getDex() const; // returns m_dex
    virtual int getSleepTime() const; // returns m_sleepTimer

    virtual int getWpnDexBonusActr() const;
    virtual int getWpnDmgActr() const; // returns weapon damage

    virtual void setHP(int const& HPChange);// adds HPChange to HP - NEGATIVE values will DECREASE HP
    virtual int getWpnSleepTimeActr() const; // returns sleep time inflicted by wielded magic fangs
    virtual bool isAlive() const; // checks m_HP after EVERY attack to see if actor is still alive

    virtual string getWpnTypeM(); // returns mosnter's wielded weapon type
   
    virtual string drop() const; // drop item upon death
    // returns EMPTY string if no item is dropped

    virtual void move(); // monster movement post-player move
     // move towards player if m_playerSmelt == true
        // never one step further from palers row,col
        // Stupid - moves only one tick indiscriminately

// ==== To Do    
    
    virtual void attack(); // attack if NEXT to player


                                /// ***

                    //if (m->monsterSleepTime >= 1) {

                        //addMsgtoMsgList("ZzZzZzzz - Player is fast a asleep");

                    //    return;

                    //}

    void setPlayerSmelt(); // toggles player smelt



private:
    string m_currWeapon;
    bool m_playerSmelt;

    int m_HP;
    int m_armor;
    int m_str;
    int m_dex;
    int m_sleepTime;

    Weapon* m_wpnWielded;
    Level* m_levelPtr;
};



// ===========================================================================================================================================================================

class Bogeyman : public Monster {
    // IF level >= 2
    // smells player within 5 steps (obstructed or not)
    // MOVES ONLY IFF player is smelt
    // Other monsters can obstrcut Bogeymen
    // CANNOT NAVIGATE the maze - moves only absolutely

public:
    Bogeyman(); // ZP - C
    virtual ~Bogeyman(); // D

    void setPlayerSmelt(); // toggles player smelt

    virtual int getHP() const; // returns m_HP
    virtual int getArmor() const; // returns m_armor
    virtual int getStr() const; // returns m_str
    virtual int getDex() const; // returns m_dex
    virtual int getSleepTime() const; // returns m_sleepTimer

    virtual int getWpnDexBonusActr() const;
    virtual int getWpnDmgActr() const; // returns weapon damage

    virtual void setHP(int const& HPChange);// adds HPChange to HP - NEGATIVE values will DECREASE HP
    virtual bool isAlive() const; // checks m_HP after EVERY attack to see if actor is still alive

    virtual string getWpnTypeM(); // returns mosnter's wielded weapon type

    virtual string drop() const; // drop item upon death
    // returns EMPTY string if no item is dropped


    virtual void move(); // monster movement post-player move
    // move towards player if m_playerSmelt == true
        // never one step further from palers row,col
        // Stupid - moves only one tick indiscriminately

//===TO do 
    virtual void attack(); // attack if NEXT to player

private:
    string m_currWeapon;
    bool m_playerSmelt;

    int m_HP;
    int m_armor;
    int m_str;
    int m_dex;
    int m_sleepTime;

    Weapon* m_wpnWielded;



};

// ===========================================================================================================================================================================

class Dragon : public Monster {
    // IF level >= 3
    // DOES NOT MOVE
    // Drops some scroll

public:
    Dragon(); // ZP - C
    virtual ~Dragon(); // D

    virtual int getHP() const; // returns m_HP
    virtual int getArmor() const; // returns m_armor
    virtual int getStr() const; // returns m_str
    virtual int getDex() const; // returns m_dex
    virtual int getSleepTime() const; // returns m_sleepTimer

    virtual int getWpnDexBonusActr() const;
    virtual int getWpnDmgActr() const; // returns weapon damage

    virtual void setHP(int const& HPChange); // sets m_HP - following attack or HP gain
    virtual bool isAlive() const; // checks m_HP after EVERY attack to see if actor is still alive


    virtual void attack(); // attack if NEXT to player

    virtual string getWpnTypeM(); // returns mosnter's wielded weapon type

    virtual void regainHP(); // 1/ 10 chance +1 HP before each turn

    virtual string drop() const; // drop item upon death
    // returns EMPTY string if no item is dropped


    virtual void move(); // monster movement post-player move
        // Dragon doesn't move - function just returns

// === To do



private:
    string m_currWeapon;

    int m_HPCapacity;
    int m_HP;
    int m_armor;
    int m_str;
    int m_dex;
    int m_sleepTime;

    Weapon* m_wpnWielded;


};

// ===========================================================================================================================================================================

class Goblin : public Monster {
    // Appear on ANY dungeon level
    // Moves ONLY if player smelt
    // smells player within >= 15 steps UNOBSTRCUTED by wall or other monsters
    // Makes OPTIMAL moves, such to navigate maze towards player
          // RECURSION
          // calcs SHORTEST path to player
          // can reduce argument to ggame constructed to smaller numebr when designing recursion to speed of process
    // drops magic axe OR magic fangs if no item at pos

public:
    Goblin(); // ZP - C
    virtual ~Goblin(); // D

    virtual int getHP() const; // returns m_HP
    virtual int getArmor() const; // returns m_armor
    virtual int getStr() const; // returns m_str
    virtual int getDex() const; // returns m_dex
    virtual int getSleepTime() const; // returns m_sleepTimer

    virtual int getWpnDexBonusActr() const;
    virtual int getWpnDmgActr() const; // returns weapon damage

    virtual void setHP(int const& HPChange); // adds HPChange to HP - NEGATIVE values will DECREASE HP
    virtual bool isAlive() const; // checks m_HP after EVERY attack to see if actor is still alive


    virtual string getWpnTypeM(); // returns mosnter's wielded weapon type

    virtual string drop() const; // drop item upon death
    // returns EMPTY string if no item is dropped


    void setPlayerSmelt(); // toggles player smelt
        // runs pathExistsWithinStepCount()
    virtual void move(); // monster movement post-player move

// to do == 
    // move towards player if m_playerSmelt == true
        // never one step further from palers row,col

    void attack(); // attack if NEXT to player
    void drop(); // drop item upon death IFF no item occupies pos



private:
    string m_currWeapon;
    bool m_playerSmelt;

    int m_HP;
    int m_armor;
    int m_str;
    int m_dex;
    int m_sleepTime;

    Weapon* m_wpnWielded;
    Level* m_level;


};


#endif // ACTOR_H