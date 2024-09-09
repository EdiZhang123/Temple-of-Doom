#ifndef ACTORS_H
#define ACTORS_H

#include "Weapons.h"
#include <list>
#include <string>

using namespace std;

class Actor {
public:
    Actor(int hp, int strength, int dex, int armor, char symbol, Level* someLevel, string name);
    virtual ~Actor();
    
    int getRow() const;
    int getCol() const;
    bool isAlive() const;

    void setRow(int row);
    void setCol(int col);
    //~Actor();
    char getSymbol() const;
    void setSymbol(char symbol);
    
    Level* getLevel() const;
    void setLevel(Level* someLevel);
    
    void setHp(int amount);
    int getHp() const;
    
    void setArmor(int amount);
    int getArmor() const;
    
    void setStrength(int amount);
    int getStrength() const;
    
    void setDex(int amount);
    int getDex() const;
    
    void attack(Actor* a);
    
    Weapon* getCurrentWeapon();
    void setCurrentWeapon(Weapon* w);
    
    string getName() const;
    int getSleep() const;
    void setSleep(int amount);
    
    void emptyActionQueue();

    int getMaxHealth() const;
    void IncreaseMaxHealth(int max);
    
private:
    
    //attributes
    int m_hp;
    int m_strength;
    int m_dex;
    int m_armor;
    int MAXHEALTH;
    
    char m_symbol;
    string m_name;
    int m_sleep;
    
    // Level
    Level* m_level;

    //Some weapon
    Weapon* currentWeapon;

    //Location
    int m_row;
    int m_col;
    
    
};


#endif
