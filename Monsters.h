#ifndef MONSTERS_H
#define MONSTERS_H

#include "Actors.h"
#include "Weapons.h"
#include "utilities.h"

class Player;

class Monster : public Actor{
public:
    
    Monster(Level* someLevel, int hp, int strength, int armor, int dex, char symbol, int smellDistance, string typeOfMonster);
    virtual ~Monster();
    virtual bool withinRange(int smellDistance); //don't use for goblin
    
    void setSmellDistance(int num);
    int getSmellDistance() const;
    
    virtual void moveMonster();
    virtual void dropItem();

    
private:
    int smellDistance;
};

class BogeyMen : public Monster{
public:
    // Bogeymen appear only at temple level 2 or deeper
    BogeyMen(Level* someLevel, int hp = randInt(5, 10), int strength = randInt(2, 3), int armor = 2, int dex = randInt(2, 3), char symbol = 'B', int smellDistance = 5, string typeOfMonster = "the Bogeyman ");
    
    
    void dropItem();
   
private:
    ShortSword m_shortSword;

};


class SnakeWomen : public Monster{
    // Snakewomen may appear on any temple level
public:
    SnakeWomen(Level* someLevel, int hp = randInt(3, 6), int strength = 2, int armor = 3, int dex = 3, char symbol = 'S', int smellDistance = 3, string typeOfMonster = "the Snakewoman "); //Initialize with Actor constructor
    
    void dropItem();
    // Hit points: a random integer from 3 to 6
    // Weapon: magic fangs of sleep
    // Strength: 2
    // Dexterity: 3
    // Armor: 3
    
    
private:
    MagicFang m_magicFang;
    
};

class Dragon : public Monster{
public:
    Dragon(Level* someLevel, int hp = randInt(20,25), int strength = 4, int armor = 4, int dex = 4, char symbol = 'D', int smellDistance = 1, string typeOfMonster = "the Dragon ");
    
    void moveMonster();
    void dropItem();

private:
    LongSword m_longSword;
};

class Goblin : public Monster{
public:
    Goblin(Level* someLevel, int hp = randInt(15, 20), int strength = 3, int armor = 1, int dex = 1, char symbol = 'G', int smellDistance = 15, string typeOfMonster = "the Goblin ");
    //dummy value smellDistance
    
    bool pathExists();
    bool withinRange(int g_row, int g_col, int distance);
    int minimumSteps(int g_row, int g_col, int stepsTaken);
    void moveMonster();
    void dropItem();

    
private:
    ShortSword m_shortSword;
};

#endif 
