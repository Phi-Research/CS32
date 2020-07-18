

#include <iostream>
#include <string>

#include "utilities.h"
#include "Dungeon.h"
#include "Actor.h"
#include "GameObject.h"
#include "Game.h"


// ===========================================================================================================================================================================
// ================ ACTOR ABC ===========================================================================================================================================================

Actor::Actor() // ZP - C
{

}

Actor::~Actor() 
{ }

void Actor::move()
{ 

}

int Actor::getHP() const {

    return m_HP;
}// returns m_HP
int Actor::getArmor() const {

    return m_armor;
}// returns m_armor
int Actor::getStr() const {

    return m_str;
}// returns m_str
int Actor::getDex() const {

    return m_dex;
}// returns m_dex
int Actor::getSleepTime() const {

    return m_sleepTime;
}// returns m_sleepTimer


int Actor::getWpnDexBonusActr() const {
    
    return 0; // BECAUSE THIS SHOULD BE OVERWRITTEN  BY DERIVED CLASSES
}

void Actor::setHP(int const &HPChange) {

    m_HP = m_HP + HPChange;
}

int Actor::getWpnDmgActr() const {

    return 0; // OVERWRITTEN by all derived classes - PV
}

bool Actor::isAlive() const {

    if (m_HP <= 0)
    {
        //addMsgtoMsgList("You died - Game over");
        return false;
    }

    else
        return true;
}

void Actor::regainHP()
{
    return;
}

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================


Player::Player(Game* ptrToGameObject)
    : m_HP(20), m_armor(2), m_str(2), m_dex(2), m_sleepTime(0), m_HPCapacity(20), m_currWeapon("shortSword")
    , m_playertoCurrLevelPtr(nullptr)
    , m_wpnWielded(new ShortSword())
    , m_actorType("Player")
    , m_playerToGamePtr(ptrToGameObject)
{
    // Player object is contructed on HEAP BEFORE the game constructor constructs the 0th level

    // Make dummmy item in player's inventory
    InventoryNode dummyItem;
    dummyItem.itemSlotLetter = ' ';
    dummyItem.itemDescrip = "--- Player Inventory ---";
    dummyItem.itemPtr = nullptr;
    m_inventory.push_back(dummyItem);

    //// Test items for player's inventory
    //InventoryNode itemA;
    //itemA.itemSlotLetter = 'a';
    //itemA.itemPtr = new incrHPScroll();
    //itemA.itemDescrip = itemA.itemPtr->getItemType();
    //m_inventory.push_back(itemA);

    //InventoryNode itemB;
    //itemB.itemSlotLetter = 'm';
    //itemB.itemPtr = new incrHPScroll();
    //itemB.itemDescrip = itemB.itemPtr->getItemType();
    //m_inventory.push_back(itemB);

    ////InventoryNode itemC;
    ////itemC.itemSlotLetter = "d.";
    ////itemC.itemPtr = new TeleportationScroll(this);
    ////itemC.itemDescrip = itemC.itemPtr->getItemType();
    ////m_inventory.push_back(itemC);

    ////itemC.itemPtr->getTeleportationCoordinates();
    ////TeleportationScroll testTeleScroll(this);
    ////testTeleScroll.getTeleportationCoordinates();


    //InventoryNode wpn1;
    //wpn1.itemSlotLetter = 'e';
    //wpn1.itemPtr = new MagicFangs();
    //wpn1.itemDescrip = wpn1.itemPtr->getItemType();
    //m_inventory.push_back(wpn1);

    //InventoryNode wpn2;
    //wpn2.itemSlotLetter = 'f';
    //wpn2.itemPtr = new ShortSword();
    //wpn2.itemDescrip = wpn2.itemPtr->getItemType();
    //m_inventory.push_back(wpn2);    

}


Player::~Player() {

    delete m_wpnWielded;
    /*
    delete objects in inventory
    
    */
}

Level*& Player::getPlayerCurrLevelPtr() {
    //Returns REFERENCE to player's pointer to the CURRENT level
    return m_playertoCurrLevelPtr;
}

//void Player::setPlayerCurrLevelPtr(Level* currLevel) {
//    m_playertoCurrLevelPtr = currLevel;
//}

//Level** Player::getLvlPtrArrayPtr() {
//    // Returns pointer to lvlPtrArray
//    return m_lvlPtrArray;
//}

void Player::move(char const direction)
/*
Move player
    Attack if obstructed by monster
Move monsters
    move
    Attack if player is adjacent
*/
{   

    // Call level object
        // get NODE that describes player's CURRENT state - which includes the player's CURRENT coordinats
    PlayerNode playerCurrentCoord;
    playerCurrentCoord = m_playertoCurrLevelPtr->getPlayerCoordinates();

    // Store the player's current coordinates - for readbility sake in the code below
    int currRow = playerCurrentCoord.playerRow;
    int currCol = playerCurrentCoord.playerCol;

    // Calculate NEW coordinates based on DESIRED direction of travel
    int newRow, newCol;
    switch (direction) {

    case 'U' : // UP - (NORTH)
        newRow = currRow - 1;
        newCol = currCol;
        break;

    case 'D': // DOWN - (SOUTH) 
        newRow = currRow + 1;
        newCol = currCol;
        break;

    case 'L': // LEFT - (WEST)
        newRow = currRow;
        newCol = currCol - 1;
        break;

    case 'R': // RIGHT - (EAST)
        newRow = currRow;
        newCol = currCol + 1;
        break;
    }

    PlayerNode newPlayerPos; // Allocates node to store new coordinates

// === Check for obstruction ===
    // --- Wall check ---
    // Call level - check for wall at new coordiates
    bool wallObstructingPath = m_playertoCurrLevelPtr->checkForWallAtPosition(newRow, newCol);

    if (wallObstructingPath == true) {
        //Call level
        m_playertoCurrLevelPtr->addMsgtoMsgList("A wall obstructs your path");
     }
   
    // --- Monster check ---
    // Call level - check monster positions to see if one matches new coordinates
    bool monsterObstructingPath = m_playertoCurrLevelPtr->checkForMonsterAtPosition(newRow, newCol);
    if (monsterObstructingPath == true) {
        // Call Level - conduct attack
        m_playertoCurrLevelPtr->rcvAttackFromPlayer(newRow, newCol);
    }

    // --- IFF no walls AND no monsters ---
    else if (wallObstructingPath == false && monsterObstructingPath == false )
    {
        newPlayerPos.playerRow = newRow;
        newPlayerPos.playerCol = newCol;
        // Call level 
            //Add NEW node player's state history - to FRONT of list        
        m_playertoCurrLevelPtr->addNodeToPlayerStateHistory(newPlayerPos);
    }

    // Monster's attack/move turn
         // Call level
            // print map - for monsters to see player's movement
        m_playertoCurrLevelPtr->printMap();
        m_playertoCurrLevelPtr->getMonsterAttacks();
        m_playertoCurrLevelPtr->getMonsterMoves();           

    regainHP(); // 10% chnace this funciton increments player's HP
   
    //Call level - tell monsters to regain HP if they are self-healing
    m_playertoCurrLevelPtr->monsterHPRegain();
}

void Player::setSleepTime(int newSleepTime)
{
    m_sleepTime = newSleepTime;
}

void Player::descend()
{
    m_playertoCurrLevelPtr->addMsgtoMsgList("You descend further into the depths of the dungeon");
   
    int currLevel = m_playertoCurrLevelPtr->getLevelNumber();
    int nextLevel = currLevel + 1;

    // Delete current level from HEAP
    //delete m_playertoCurrLevelPtr;

    // Get pointer to PLAYER from GAME object (Game is parent object on STACK)
    Player* ptrToPlayer = m_playerToGamePtr->getPlayerPtr();
    Game* ptrToGame = m_playerToGamePtr->getGamePtr();

    // Generates next level on HEAP
    // Call Level
        // Construct new level - incremented level number
    m_playertoCurrLevelPtr = new Level(nextLevel, ptrToPlayer, ptrToGame);    
}

Game* Player::getPtrToGame()
{
    return m_playerToGamePtr;
}

void Player::cheat()
{
    m_HPCapacity = 50;
    m_HP = 50;
    m_armor = 9;
    m_str = 9;
    m_dex = 9;

    m_playertoCurrLevelPtr->addMsgtoMsgList("Cheat mode activated - You meditate and absorb the energies of the universe");

    m_playertoCurrLevelPtr->getMonsterMoves();
    m_playertoCurrLevelPtr->getMonsterAttacks();
}

void Player::quit()
{
    cout << "Godspeed" << endl;
}

void Player::regainHP()
{
    // 1/10 chance of regain post-player commmand

    double p = 0.1;    
    bool HPRegainDecision = trueWithProbability(p);
    if (m_HP == m_HPCapacity) {
        return;
    }
    if (HPRegainDecision == true) {
        m_playertoCurrLevelPtr->addMsgtoMsgList("You regained 1HP");
        m_HP++;
    }
    else
        return;
}

void Player::setHPCapacity(int const &HPCapacityIncrement) {

    m_HP = m_HP + HPCapacityIncrement;
}

int Player::getHP() const { // returns m_HP

    return m_HP;
}
int Player::getArmor() const { // returns m_armor

    return m_armor;
}
int Player::getStr() const { // returns m_str

    return m_str;
}
int Player::getDex() const  // returns m_dex
{
    return m_dex;
}
int Player::getSleepTime() const { // returns m_sleepTime

    return m_sleepTime;
}

int Player::getWpnDexBonusActr() const {

    return m_wpnWielded->getWpnDexBonusW();
}

int Player::getWpnDmgActr() const {

    return m_wpnWielded->getWpnDmgW();
}

void Player::setHP(int const& HPChange) { // PV

    m_HP = m_HP + HPChange;
}

string Player::getWpnTypeP() const{

    return m_wpnWielded->getWpnTypeW();
}

int Player::getWpnSleepTimeActr() const {   

    // Determines if sleep is induced by weapon AND sleep duration
    // Dungeon.cpp sets sleep timer of actor
    return m_wpnWielded->getWpnSleepTimeW();
}

bool Player::isAlive() const {

    if (m_HP <= 0)
    {   
        m_playertoCurrLevelPtr->addMsgtoMsgList("You died - Game over");
        return false;
    }
    else
        return true;
}


void Player::pickup()
/*
Check inventory size to make sure it's not > 25

Look at player's location
Get object with SAME coordinates from LEVEL's game object list
copy item data FROM game object list TO inventory list
    COPY POINTER - DO NOT CREATE NEW ITEM (because would require a switch with a ton of cases)
pop item from game object list - DO NOT DELETE
Create new inventory node which contains the pointer to the game object node
*/
{
    // Get object node from level
    GameObjectNode itemFromLevel = m_playertoCurrLevelPtr->getObjectNodeFromLevel();

    // If there is no item at player's location
    bool itemPresent = false;
    if (itemFromLevel.itemChar == ' ')
    {
        m_playertoCurrLevelPtr->addMsgtoMsgList("There's no item at this location");
        return;
    }

    else
    {
        itemPresent = true;

        if (m_inventory.size() >= 25)
        {
            m_playertoCurrLevelPtr->addMsgtoMsgList("Your inventory is full! you can't pick that up you TP hoarding bastard.");
        }

        if (itemPresent)
        {   // Create inventory node
            InventoryNode newInventoryNode;
            //ASCI a == 97; z == 122;  
            char slotChar = 'a' + m_inventory.size();            

            newInventoryNode.itemPtr = itemFromLevel.itemPtr;
            newInventoryNode.itemSlotLetter = slotChar;
            newInventoryNode.itemDescrip = itemFromLevel.itemPtr->getItemType();

            // Push inventory node onto player's invenotry list
            m_inventory.push_back(newInventoryNode);
        }
    }

    m_playertoCurrLevelPtr->getMonsterMoves();
    m_playertoCurrLevelPtr->getMonsterAttacks();

    return;
}

//void Player::sendInventoryToScreen(char command)
void Player::sendInventoryToScreen()
{
    //cerr << "command: " << command << endl;

    clearScreen(); 
    list<InventoryNode>::iterator p = m_inventory.begin();
    while (p != m_inventory.end())
    {
        cout << p->itemSlotLetter << ". " << p->itemDescrip << endl;
        p++;
    }
    cout << endl;

    m_playertoCurrLevelPtr->getMonsterMoves();
    m_playertoCurrLevelPtr->getMonsterAttacks();



}


void Player::readScroll(char inventorySlotSelected)
{
    cerr << "inventorySlotSelected: " << inventorySlotSelected << endl;

    list<InventoryNode>::iterator it = m_inventory.begin();
    int count = -1;
    while (it != m_inventory.end()) // Cycle ENTIRE inventory
    {
        //count++;
        //cerr << "inven size: " << m_inventory.size() << endl;
        //cerr << "--- count " << count << "---" << endl;

        if (it->itemSlotLetter == inventorySlotSelected)
        {
            string itemType = it->itemPtr->getItemType();
            char scrollTypeChar = ' ';
            scrollTypeChar = itemType.at(10);
            // return 10th letter of item type - scroll of _

            switch (scrollTypeChar)
            {
            case 't':
                // Call level to perform teleport
                m_playertoCurrLevelPtr->teleportPlayer();
                delete it->itemPtr;
                it = m_inventory.erase(it);
                m_playertoCurrLevelPtr->addMsgtoMsgList("You read a scroll of teleportaton! Hold on tight ...gaa-Zing!");
                break;

            case 'a':
                m_armor += it->itemPtr->getScrollStatBonus();
                delete it->itemPtr;
                it = m_inventory.erase(it);
                m_playertoCurrLevelPtr->addMsgtoMsgList("You read a scroll of armor! You feel like you could smash through a brick wall.");
                break;

            case 's':
                m_str += it->itemPtr->getScrollStatBonus();
                delete it->itemPtr;
                it = m_inventory.erase(it);
                m_playertoCurrLevelPtr->addMsgtoMsgList("You read a scroll of strength! Now you're fuckin amped!");
                break;

            case 'h':
                m_HPCapacity += it->itemPtr->getScrollStatBonus();
                delete it->itemPtr;
                it = m_inventory.erase(it);
                m_playertoCurrLevelPtr->addMsgtoMsgList("You read a scroll of health! It's not quite a magic mushroom, but you feel better.");
                break;

            case 'd':
                m_dex += it->itemPtr->getScrollStatBonus();
                delete it->itemPtr;
                it = m_inventory.erase(it);
                m_playertoCurrLevelPtr->addMsgtoMsgList("You read a scroll of dexterity! Your focus sharpens and instincts quicken. One mind, any weapon");
                break;

            default:
                m_playertoCurrLevelPtr->addMsgtoMsgList("That's not a scroll!");
                //it++;
                break;
            }
        }
        else
            it++;
    }
    m_playertoCurrLevelPtr->getMonsterMoves();
    m_playertoCurrLevelPtr->getMonsterAttacks();    
    m_playertoCurrLevelPtr->printMap();
}

void Player::swapWeapon(char inventorySlotSelected)
{
    cerr << "inventorySlotSelected: " << inventorySlotSelected << endl;

    list<InventoryNode>::iterator it = m_inventory.begin();
    int count = -1;
    while (it != m_inventory.end()) // Cycle ENTIRE inventory
    {
        count++;
        cerr << "inven size: " << m_inventory.size() << endl;
        cerr << "--- count " << count << "---" << endl;

        if (it->itemSlotLetter == inventorySlotSelected)
        {
            string itemType = it->itemPtr->getItemType();       
            if (itemType == "mace")
            {
                delete m_wpnWielded;
                m_wpnWielded = new Mace();
                m_playertoCurrLevelPtr->addMsgtoMsgList("You now wield a mace!");
            }
            if (itemType == "short sword")
            {
                delete m_wpnWielded;
                m_wpnWielded = new ShortSword();
                m_playertoCurrLevelPtr->addMsgtoMsgList("You now wield a short sword!");

            }
            if (itemType == "long sword")
            {
                delete m_wpnWielded;
                m_wpnWielded = new LongSword();
                m_playertoCurrLevelPtr->addMsgtoMsgList("You now wield a long sword!");

            }
            if (itemType == "magic axe")
            {
                delete m_wpnWielded;
                m_wpnWielded = new MagicAxe();
                m_playertoCurrLevelPtr->addMsgtoMsgList("You now wield a magic axe!");

            }
            if (itemType == "magic fangs")
            {
                delete m_wpnWielded;
                m_wpnWielded = new MagicFangs();
                m_playertoCurrLevelPtr->addMsgtoMsgList("You now wield magic fangs!");

            }
            else
            {
                m_playertoCurrLevelPtr->addMsgtoMsgList("That's not a weapon!");
                //it++;
            }       
        }
        // DO NOT PUT AN ELSE BEFORE THE ITERATOR
        it++;
    }
    m_playertoCurrLevelPtr->getMonsterMoves();
    m_playertoCurrLevelPtr->getMonsterAttacks();
    m_playertoCurrLevelPtr->printMap();
}




// ===========================================================================================================================================================================
// ===========================================================================================================================================================================




Monster::Monster()
{

}


Monster::~Monster() {

    //delete m_currWeaponPtr;
}

void Monster::attack()
{

}

string Monster::drop() const
// rand chance of dropping magic fang, magic axe, teleport scroll
// PV - overwritten by ALL mosnters
{
    string itemDropped = "";
    return itemDropped;
}


string Monster::getWpnTypeM()
{
    return m_currWeapon;

}

int Monster::getWpnSleepTimeActr() const
{
    // Overwritten by ONLY Snakewoman - and player
    return 0;
}

void Monster::regainHP()
{
    return;
}

void Monster::move()
{
// overwriten by all monster classes
    return;
}



// ===========================================================================================================================================================================

Snakewoman::Snakewoman() // ZP - C
    : m_HP(randInt(3, 6)), m_armor(3), m_str(2), m_dex(3), m_sleepTime(0),
    m_playerSmelt(false), m_currWeapon("magicFangs"), m_wpnWielded(new MagicFangs())
{

}

Snakewoman::~Snakewoman() {
    delete m_wpnWielded;
}

int Snakewoman::getHP() const { // returns m_HP

    return m_HP;
}
int Snakewoman::getArmor() const { // returns m_armor

    return m_armor;
}
int Snakewoman::getStr() const { // returns m_str

    return m_str;
}
int Snakewoman::getDex() const  // returns m_dex
{
    return m_dex;
}
int Snakewoman::getSleepTime() const { // returns m_sleepTime

    return m_sleepTime;
}

void Snakewoman::setHP(int const& HPChange) {

    m_HP = m_HP + HPChange;
}

int Snakewoman::getWpnDexBonusActr() const {

    return m_wpnWielded->getWpnDexBonusW();
}

int Snakewoman::getWpnDmgActr() const {

    return m_wpnWielded->getWpnDmgW();
}

int Snakewoman::getWpnSleepTimeActr() const {

    // Determines if sleep is induced by weapon AND sleep duration
    // Dungeon.cpp sets sleep timer of actor
    return m_wpnWielded->getWpnSleepTimeW();
}

bool Snakewoman::isAlive() const {

    if (m_HP <= 0)
    {        
        //m_levelPtr->addMsgtoMsgList("You have slain the Snakewoman!");        
        return false;
    }
    else
        return true;
}

string Snakewoman::getWpnTypeM()
{
    return m_currWeapon;
}

void Snakewoman::attack()
{

}

string Snakewoman::drop() const
// 1/3 rand chance of dropping magic fang of sleep
{
    string itemDropped = "";
    double p = 0.33;
    bool dropDecision = trueWithProbability(p);

    if (dropDecision == true) {
        itemDropped = "magic fangs";
    }

    return itemDropped;
}

void Snakewoman::setPlayerSmelt()
/*
IF player within <= 3 steps
    m_playerSmelt = true;
*/
{


}

void Snakewoman::move()

{
    if (m_playerSmelt == true) {
        // move towards player

    }

    else {
        // DO NOT MOVE

    }
}



// ===========================================================================================================================================================================


Bogeyman::Bogeyman() // ZP - C
    : m_HP(randInt(5, 10)), m_armor(2), m_str(randInt(2, 3)), m_dex(randInt(2, 3)), m_sleepTime(0),
    m_playerSmelt(false), m_currWeapon("shortSword"), m_wpnWielded(new ShortSword())
{ }

Bogeyman::~Bogeyman() 
{ 
    delete m_wpnWielded;
}

int Bogeyman::getHP() const { // returns m_HP

    return m_HP;
}
int Bogeyman::getArmor() const { // returns m_armor

    return m_armor;
}
int Bogeyman::getStr() const { // returns m_str

    return m_str;
}
int Bogeyman::getDex() const  // returns m_dex
{
    return m_dex;
}
int Bogeyman::getSleepTime() const { // returns m_sleepTime

    return m_sleepTime;
}

int Bogeyman::getWpnDexBonusActr() const {

    return m_wpnWielded->getWpnDexBonusW();
}

int Bogeyman::getWpnDmgActr() const {

    return m_wpnWielded->getWpnDmgW();
}

void Bogeyman::setHP(int const& HPChange) {

    m_HP = m_HP + HPChange;
}

bool Bogeyman::isAlive() const {

    if (m_HP <= 0)
    {
        return false;
    }
    else
        return true;
}


string Bogeyman::getWpnTypeM()
{
    return m_currWeapon;

}

void Bogeyman::attack()
{

}


string Bogeyman::drop() const
// 1/3 rand chance of dropping magic fang of sleep
{
    string itemDropped = "";
    double p = 0.1;
    bool dropDecision = trueWithProbability(p);

    if (dropDecision == true) {
        itemDropped = "magic axe";
    }

    return itemDropped;
}


void Bogeyman::setPlayerSmelt()
/*
IF player within <= 5 steps
    m_playerSmelt = true;
*/
{



}

void Bogeyman::move()
{
    if (m_playerSmelt == true) {
        // move towards player

    }
    else {
        // DO NOT MOVE

    }

}

// ===========================================================================================================================================================================


//Dragon::Dragon() // ZP - C
//    : Monster(randInt(20, 25), 4, 4, 4, 0), m_currWeapon("longSword")
//    // Monster(int HP, int armor, int str, int dex, int sleepTimer)
//{
//
//}

Dragon::Dragon()
    : m_HP(randInt(20, 25)), m_armor(4), m_str(4), m_dex(4), m_sleepTime(0),
    m_currWeapon("longSword"), m_wpnWielded(new LongSword())
    , m_HPCapacity(m_HP)
{

}

Dragon::~Dragon() {
    delete m_wpnWielded;
}

int Dragon::getHP() const { // returns m_HP

    return m_HP;
}
int Dragon::getArmor() const { // returns m_armor

    return m_armor;
}
int Dragon::getStr() const { // returns m_str

    return m_str;
}
int Dragon::getDex() const  // returns m_dex
{
    return m_dex;
}
int Dragon::getSleepTime() const { // returns m_sleepTime

    return m_sleepTime;
}

int Dragon::getWpnDexBonusActr() const {

    return m_wpnWielded->getWpnDexBonusW();
}

int Dragon::getWpnDmgActr() const {

    return m_wpnWielded->getWpnDmgW();
}

void Dragon::setHP(int const& HPChange) {

    m_HP = m_HP + HPChange;
}

bool Dragon::isAlive() const {

    if (m_HP <= 0)
    {
        return false;
    }
    else
        return true;
}

string Dragon::getWpnTypeM()
{
    return m_currWeapon;

}

void Dragon::regainHP()
{
    // 1/10 chance of regain post-player commmand
    double p = 0.1;
    bool HPRegainDecision = trueWithProbability(p);

    if (m_HP == m_HPCapacity) {
        return;
    }

    if (HPRegainDecision == true) {
       m_HP++;
    }

    else
        return;
}

void Dragon::attack()
{

}


string Dragon::drop() const
// 1/3 rand chance of dropping magic fang of sleep
{
    string itemDropped = "";
    double p = 1;
    bool dropDecision = trueWithProbability(p);

    if (dropDecision == true) {   

        int scrollTypeDecision = randInt(0, 4);
        switch (scrollTypeDecision)
        {
        case 0 :
            itemDropped = "scroll of teleportation";
            break;
        case 1 : 
            itemDropped = "scroll of armor";
            break;
        case 2 :
            itemDropped = "scroll of strength";
            break;
        case 3 :
            itemDropped = "scroll of health";
            break;
        case 4 :
            itemDropped = "scroll of dexterity";
            break;
        }
    }

    return itemDropped;
}

void Dragon::move()
{
    return;
}

// ===========================================================================================================================================================================


Goblin::Goblin() // ZP - C
    : m_HP(randInt(15, 20)), m_armor(1), m_str(3), m_dex(1), m_sleepTime(0),
    m_playerSmelt(false), m_currWeapon("short sword"), m_wpnWielded(new ShortSword())
{

}

Goblin::~Goblin() {
    delete m_wpnWielded;
}

int Goblin::getHP() const { // returns m_HP

    return m_HP;
}
int Goblin::getArmor() const { // returns m_armor

    return m_armor;
}
int Goblin::getStr() const { // returns m_str

    return m_str;
}
int Goblin::getDex() const  // returns m_dex
{
    return m_dex;
}
int Goblin::getSleepTime() const { // returns m_sleepTime

    return m_sleepTime;
}

int Goblin::getWpnDexBonusActr() const {

    return m_wpnWielded->getWpnDexBonusW();
}

int Goblin::getWpnDmgActr() const {

    return m_wpnWielded->getWpnDmgW();
}

void Goblin::setHP(int const& HPChange) {

    m_HP = m_HP + HPChange;
}

bool Goblin::isAlive() const {

    if (m_HP <= 0)
    {
        return false;
    }
    else
        return true;
}


string Goblin::getWpnTypeM()
{
    return m_currWeapon;

}


string Goblin::drop() const
// 1/3 rand chance of dropping either magic axe OR magic fang of sleep
{
    string itemDropped = "";
    double p = .33;
    bool dropDecision = trueWithProbability(p);

    if (dropDecision == true) {

        int wpnTypeDecision = randInt(0, 1);
        switch (wpnTypeDecision)
        {
        case 0:
            itemDropped = "magic axe";
            break;
        case 1:
            itemDropped = "magic fangs";
            break;
        }
    }

    return itemDropped;
}

void Goblin::attack()
{

}

void Goblin::setPlayerSmelt()
/*
IF player within <= 15 steps - if path can be found that's <= 15 steps
    m_playerSmelt = true;
*/
{
    // Get Monster location - START point   


    //Get player location - END point


    //m_level->pathExistsWithinStepCount();







    m_playerSmelt = true;

}

void Goblin::move()

{

    if (m_playerSmelt == false) // BASE CASE
    {
        return;
    }

    else {





    }
}

