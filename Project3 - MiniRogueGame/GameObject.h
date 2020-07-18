#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <string>

using namespace std;

struct CoordinatePair;
class Level;

/*
Game Object Class Hiercharchy

    Tier 1
        GameObject - Parent-Base
    Tier 2
        Weapon - Child-base
        Scrolls
    Tier 3
        Weapon Types
*/

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// ABC - Tier 1 - GameObject Base
class GameObject {


public:
    GameObject();
    virtual ~GameObject();

    virtual int getScrollStatBonus(); // Returns stat bonus granted by scroll

    void getHPBonus();
    void getArmorBonus();

    void getStrBonus();
    //void getDexBonus() const;
    void getSleepBonus();
    virtual CoordinatePair getTeleportationCoordinates();
        // Overwritten by scroll of teleportation 

    virtual string getItemType() const; // returns item type

private:


};
// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// ABC - Tier 2 - Weapon Base

class Weapon : public GameObject {
    // Actions strings
    // Dex bonus
    // Dmg
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
    Weapon();
    Weapon(string weaponType, int wpnDexBonus, int wpnDmg); // C
    virtual ~Weapon();

    virtual string getWpnTypeW() const; // PV - returns weapon type
        // make PV *********
    void setWeaponType(); //

    virtual int getWpnDmgW() const; // returns weapon type
        // make PV *****
    virtual int getWpnDexBonusW() const; // ** MAKE PURE V
    virtual int getWpnSleepTimeW() const;
        // Overwritten by magic fangs ONLY - else returns 0


private:
    string m_wpnType;    // shortSword, LongSword, MagicAxe ... etc
    int m_wpnDmg;
    int m_wpnDexBonus;

    //int m_row;
    //int m_col;

};

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// Tier 3 - Weapon Types

class Mace : public Weapon {


public:
    Mace(); // C
    virtual ~Mace();

    virtual string getWpnTypeW() const; // PV - returns weapon type
    virtual int getWpnDmgW() const; // returns weapon type
    virtual string getItemType() const; // returns item type

private:
    string m_wpnType;
    int m_wpnDexBonus;
    int m_wpnDmg;
    //int m_row;
    //int m_col;

};


// ===========================================================================================================================================================================

class ShortSword : public Weapon {


public:
    ShortSword(); // C
    virtual ~ShortSword();

    virtual int getWpnDexBonusW() const;
    virtual string getWpnTypeW() const; // PV - returns weapon type
    virtual int getWpnDmgW() const; // returns weapon type
    virtual string getItemType() const; // returns item type

private:
    string m_wpnType;
    int m_wpnDexBonus;
    int m_wpnDmg;

    //int m_row;
    //int m_col;

};


// ===========================================================================================================================================================================

class LongSword : public Weapon {


public:
    LongSword(); // C
    virtual ~LongSword();

    virtual string getWpnTypeW() const; // PV - returns weapon type
    virtual int getWpnDmgW() const; // returns weapon type
    virtual string getItemType() const; // returns item type

private:
    string m_wpnType;    // shortSword, LongSword, MagicAxe ... etc
    int m_wpnDexBonus;
    int m_wpnDmg;
    //int m_row;
    //int m_col;

};

// ===========================================================================================================================================================================

class MagicAxe : public Weapon {


public:

    MagicAxe(); // C
    virtual ~MagicAxe();

    virtual string getWpnTypeW() const; // PV - returns weapon type
    virtual int getWpnDmgW() const; // returns weapon type
    virtual string getItemType() const; // returns item type

private:

    string m_wpnType;    // shortSword, LongSword, MagicAxe ... etc
    int m_wpnDexBonus;
    int m_wpnDmg;
    //int m_row;
    //int m_col;

};

// ===========================================================================================================================================================================

class MagicFangs : public Weapon {

public:
    MagicFangs(); // C
    virtual ~MagicFangs();

    virtual string getWpnTypeW() const; // PV - returns weapon type
        //*** USED FOR COMABT
    virtual int getWpnDmgW() const; // returns weapon type
    int getWpnSleepTimeW() const; // Returns sleep time inflicted by magic fangs upon an attack
    virtual string getItemType() const; // returns item type

private:
    string m_wpnType;    // shortSword, LongSword, MagicAxe ... etc

    int m_wpnDexBonus;
    int m_wpnDmg;
    //int m_row;
    //int m_col;

};

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// Tier 2 - Scrolls

class Scroll : public GameObject {
    /*
    Construct on HEAP UPON entry into Player's inventory    
    */

public:
    Scroll();
    //Scroll(string scrollType);
    virtual ~Scroll();

private:
    char m_scrollTypeChar;
    string m_scrollType;    

};

// ===========================================================================================================================================================================

class TeleportationScroll : public Scroll {

public:
    TeleportationScroll(Level* currLevel);
    virtual  ~TeleportationScroll();

    CoordinatePair getTeleportationCoordinates();
    // IF (row, col == occupied)
            // generate new set
        // else set player pos == teleRow && teleCol
    virtual string getItemType() const; // returns item type

private:
    Level* m_currLevel;
    char m_scrollTypeChar;
    string m_scrollType; 
    int m_randRow;
    int m_randCol;



};

// ===========================================================================================================================================================================

class incrArmorScroll : public Scroll {

public:
    incrArmorScroll();
    virtual ~incrArmorScroll();

    int getArmorIncr() const; // rand(1, 3)
    virtual string getItemType() const; // returns item type

    virtual int getScrollStatBonus(); // Returns stat bonus granted by scroll


private:
    char m_scrollTypeChar;

    string m_scrollType;
    int m_armorIncr;
    int m_row;
    int m_col;

};

// ===========================================================================================================================================================================

class incrStrScroll : public Scroll {

public:
    incrStrScroll();
    virtual ~incrStrScroll();

    int getStrIncr() const; // rand(1, 3)
    virtual string getItemType() const; // returns item type
    virtual int getScrollStatBonus(); // Returns stat bonus granted by scroll

private:
    char m_scrollTypeChar;
    string m_scrollType;
    int m_strIncr;


};

// ===========================================================================================================================================================================

class incrHPScroll : public Scroll {
    //incr m_HPCapacity by randInt(3, 8)

public:
    incrHPScroll();
    virtual  ~incrHPScroll();

    int getHPCapacityIncr() const; // rand(3, 8)
    virtual string getItemType() const; // returns item type
    virtual int getScrollStatBonus(); // Returns stat bonus granted by scroll

private:
    char m_scrollTypeChar;
    string m_scrollType;
    int m_HPCapacityIncr;

    Scroll* m_scrollPtr;


};

// ===========================================================================================================================================================================


class incrDexScroll : public Scroll {

public:
    incrDexScroll();
    virtual  ~incrDexScroll();

    int getDexIncr() const;
    virtual string getItemType() const; // returns item type
    virtual int getScrollStatBonus(); // Returns stat bonus granted by scroll

private:
    char m_scrollTypeChar;
    string m_scrollType;   
    int m_dexIncr;



};

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================

class Descend : public GameObject {

public:
    Descend();
    virtual ~Descend();

private:
    string m_gameObjectType;


};



// ===========================================================================================================================================================================
// ===========================================================================================================================================================================


class GoldenIdol : public GameObject {

public:
    GoldenIdol();
    virtual ~GoldenIdol();

private:
    string m_gameObjectType;


};


// ===========================================================================================================================================================================


















#endif // GAMEOBJECT_H