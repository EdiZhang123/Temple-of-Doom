#ifndef WEAPONS_H
#define WEAPONS_H


#include "Objects.h"
#include <string>

using namespace std;

class Weapon : public Object{
public:
    Weapon(int r, int c, bool random, string action, int dexBonus, int damage, Level* someLevel, string read);
    virtual ~Weapon();
    
    // Accessors
    string getAction();
    int getDexBonus();
    int getDamage();
private:
    string m_action;
    int m_dexBonus;
    int m_damage;
};

class Mace : public Weapon{
public:
    Mace(Level* someLevel, int r, int c, bool random);

    // with a dexterity bonus of 0 and a damage amount of 2.
};
class ShortSword : public Weapon{
public:
    ShortSword(Level* someLevel, int r, int c, bool random );

    // with a dexterity bonus of 0 and a damage amount of 2.
};
class LongSword : public Weapon{
public:
    LongSword(Level* someLevel, int r, int c, bool random);

    // with a dexterity bonus of 2 and a damage amount of 4.
};
class MagicAxe : public Weapon{
public:
    MagicAxe(Level* someLevel, int r, int c, bool random);

    // with a dexterity bonus of 5 and a damage amount of 5.
};
class MagicFang : public Weapon{
public:
    MagicFang(Level* someLevel, int r, int c, bool random);

    //with a dexterity bonus of 3 and a damage amount of 2.
    //When the attacker hits the defender, 1 in 5 chance of putting the defender to sleep.
    //The number of moves the sleep will last (the sleep time) is a random integer from 2 to 6, call it X.
    //If the defender is already asleep, with current sleep time Y, then the defender's sleep time becomes the maximum of X and Y (and not, say, Y+X).
};



#endif 
