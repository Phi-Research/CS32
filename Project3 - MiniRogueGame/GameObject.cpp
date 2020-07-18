
#include "utilities.h"
#include "GameObject.h"
#include "Dungeon.h"

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// ===========================================================================================================================================================================


GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

CoordinatePair GameObject::getTeleportationCoordinates()
{   // OVER WRITTEN BY TELEPORTATON SCROLL
    CoordinatePair placeHolder;
    placeHolder.col = 0;
    placeHolder.row = 0;
    return placeHolder;
}

string GameObject::getItemType() const
{
    string errorMsg = "This class hasnt overwritten the base class's funciton";
    return  errorMsg;
}

int GameObject::getScrollStatBonus()
{
    return 0;
}

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================
// ===========================================================================================================================================================================

Weapon::Weapon() {

}

Weapon::Weapon(string weaponType, int wpnDexBonus, int wpnDmg)
    : m_wpnType(weaponType), m_wpnDexBonus(wpnDexBonus), m_wpnDmg(wpnDmg)
{
    //maze[m_row][m_col] = ')';

}

Weapon::~Weapon()
{

}



int Weapon::getWpnDexBonusW() const
{
    return 999;
}

string Weapon::getWpnTypeW() const // PV - returns weapon type
{

    return m_wpnType;
}

int Weapon::getWpnDmgW() const // will be PV and overwrtten 
{
    return 999;
}

int Weapon::getWpnSleepTimeW() const {
    // VIRTUAL - overwritten ONLY by MaigcFangs class
    return 0;
}

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================


Mace::Mace()
    : m_wpnType("mace"), m_wpnDexBonus(0), m_wpnDmg(2)

{

}

Mace::~Mace()
{}

string Mace::getWpnTypeW() const {

    return m_wpnType;

}

int Mace::getWpnDmgW() const // will be PV and overwrtten 
{
    return m_wpnDmg;
}

string Mace::getItemType() const
{
    return m_wpnType;
}


// ===========================================================================================================================================================================

//ShortSword::ShortSword()
//    : Weapon("shortSword", 0, 2)
//{
//
//}

ShortSword::ShortSword()
    : m_wpnType("short sword"), m_wpnDexBonus(0), m_wpnDmg(2)
{ }

ShortSword::~ShortSword()
{ }

int ShortSword::getWpnDexBonusW() const {

    return m_wpnDexBonus;

}

string ShortSword::getWpnTypeW() const {

    return m_wpnType;

}

int ShortSword::getWpnDmgW() const // will be PV and overwrtten 
{
    return m_wpnDmg;
}

string ShortSword::getItemType() const
{
    return m_wpnType;
}


// ===========================================================================================================================================================================

LongSword::LongSword()
    : m_wpnType("long sword"), m_wpnDexBonus(2), m_wpnDmg(4)
{

}

LongSword::~LongSword()
{}

string LongSword::getWpnTypeW() const {

    return m_wpnType;

}

int LongSword::getWpnDmgW() const // will be PV and overwrtten 
{
    return m_wpnDmg;
}

string LongSword::getItemType() const
{
    return m_wpnType;
}

// ===========================================================================================================================================================================

MagicAxe::MagicAxe()
    : m_wpnType("magic axe"), m_wpnDexBonus(5), m_wpnDmg(5)
{

}

MagicAxe::~MagicAxe()
{}

string MagicAxe::getWpnTypeW() const {

    return m_wpnType;

}

int MagicAxe::getWpnDmgW() const // will be PV and overwrtten 
{
    return m_wpnDmg;
}

string MagicAxe::getItemType() const
{
    return m_wpnType;
}

// ===========================================================================================================================================================================

MagicFangs::MagicFangs()
    : m_wpnType("magic fangs"), m_wpnDexBonus(3), m_wpnDmg(2)
{

}

MagicFangs::~MagicFangs()
{}

string MagicFangs::getWpnTypeW() const {

    return m_wpnType;

}

int MagicFangs::getWpnDmgW() const // will be PV and overwrtten 
{
    return m_wpnDmg;
}

int MagicFangs::getWpnSleepTimeW() const {

    int sleepDurationInduced = 0;
    int sleepInductionRandomVariable = randInt(1, 5);

    if (sleepInductionRandomVariable == 1)
        sleepDurationInduced = randInt(2, 6);

    return sleepDurationInduced;
}

string MagicFangs::getItemType() const
{
    return m_wpnType;
}

// ===========================================================================================================================================================================
// ===========================================================================================================================================================================

//Scroll::Scroll(string scrollType)
//    : m_scrollType(scrollType)
//{
//    //maze[m_row][m_col] = '?';
//
//
//}

Scroll::Scroll()
{

}

Scroll::~Scroll()
{

}

// ===========================================================================================================================================================================

TeleportationScroll::TeleportationScroll(Level* currLevel)
    : m_randRow(0), m_randCol(0), m_scrollType("scroll of teleportation"), m_scrollTypeChar('T')
    , m_currLevel(currLevel)
{

}

TeleportationScroll::~TeleportationScroll()
{

}

CoordinatePair TeleportationScroll::getTeleportationCoordinates()  {

    // generate random position to teleport player to
    CoordinatePair teleportPos = m_currLevel->generateRandEmptyPos();
    m_randRow = teleportPos.row;
    m_randCol = teleportPos.col;

    return teleportPos;
}

string TeleportationScroll::getItemType() const
{
    return m_scrollType;
}

// ===========================================================================================================================================================================

incrArmorScroll::incrArmorScroll()
    : m_armorIncr(randInt(1, 3)), m_scrollType("scroll of armor"), m_scrollTypeChar('A')
{

}

incrArmorScroll::~incrArmorScroll()
{

}

int incrArmorScroll::getArmorIncr() const {

    return m_armorIncr;
}

string incrArmorScroll::getItemType() const
{
    return m_scrollType;
}

int incrArmorScroll::getScrollStatBonus()
{
    return m_armorIncr;
}


// ===========================================================================================================================================================================

incrStrScroll::incrStrScroll()
    : m_strIncr(randInt(1, 3)), m_scrollType("scroll of strength"), m_scrollTypeChar('S')

{

}

incrStrScroll::~incrStrScroll()
{

}

int incrStrScroll::getStrIncr() const {

    return m_strIncr;
}

string incrStrScroll::getItemType() const
{
    return m_scrollType;
}

int incrStrScroll::getScrollStatBonus()
{
    return m_strIncr;
}

// ===========================================================================================================================================================================

incrHPScroll::incrHPScroll()
    : m_HPCapacityIncr( randInt(3, 8)), m_scrollType("scroll of health"), m_scrollTypeChar('H')
{

}

incrHPScroll::~incrHPScroll()
{

}

int incrHPScroll::getHPCapacityIncr() const {    

    return m_HPCapacityIncr; // determined by initialzation list - RandInt()
}

string incrHPScroll::getItemType() const
{
    return m_scrollType;
}


int incrHPScroll::getScrollStatBonus()
{
    return m_HPCapacityIncr;
}
// ===========================================================================================================================================================================

incrDexScroll::incrDexScroll()
    : m_dexIncr(1), m_scrollType("scroll of dexterity"), m_scrollTypeChar('D')
{

}

incrDexScroll::~incrDexScroll()
{

}


int incrDexScroll::getDexIncr() const {

    return m_dexIncr;
}

string incrDexScroll::getItemType() const
{
    return m_scrollType;
}


int incrDexScroll::getScrollStatBonus()
{
    return m_dexIncr;
}


// ===========================================================================================================================================================================
// ===========================================================================================================================================================================

Descend::Descend()
    : m_gameObjectType("descend")
{
    //maze[m_row][m_col] = '&';


}

Descend::~Descend()
{}


// ===========================================================================================================================================================================
// ===========================================================================================================================================================================


GoldenIdol::GoldenIdol()
    : m_gameObjectType("Golden Idol")
{
    //maze[m_row][m_col] = '&';


}

GoldenIdol::~GoldenIdol()
{}

