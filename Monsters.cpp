#include "Monsters.h"
#include "utilities.h"
#include "Level.h"
#include "Player.h"
#include "Scrolls.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>


Monster::Monster(Level *someLevel, int hp, int strength, int armor, int dex, char symbol, int smellDistance, string typeOfMonster)
: Actor(hp, strength, dex, armor,  symbol, someLevel, typeOfMonster), smellDistance(smellDistance) {
    int row = randInt(1, 1);
    int column = randInt(1, 69);
    while (!(someLevel->isNotActor(row, column) && someLevel->isNotWall(row, column))){
        row = randInt(1, 17);
        column = randInt(1, 69);
    }
    setRow(row);
    setCol(column);
    someLevel->setCoord(row, column, symbol);
}

Monster::~Monster(){}

void Monster::dropItem(){}

void Monster::setSmellDistance(int num){
    smellDistance = num;
}

int Monster::getSmellDistance() const{
    return smellDistance;
}


bool Monster::withinRange(int smellDistance){ //Check Manhattan distance
    int p_row = getLevel()->getPlayer()->getRow();
    int p_col = getLevel()->getPlayer()->getCol();
    if (abs(getRow() - p_row) + abs(getCol() - p_col) <= smellDistance)
        return true;
    return false;
}

//move left or up
//0 up
//1 down
//2 left
//3 right
void Monster::moveMonster(){
    if (getSleep() == 0) {
        if (withinRange(getSmellDistance())){
            int p_row = getLevel()->getPlayer()->getRow();
            int p_col = getLevel()->getPlayer()->getCol();
            int m_row = getRow();
            int m_col = getCol();
            if (m_row > p_row){
                if (m_col > p_col){
                    //choose either up or left (0 or 2)
                    if(!getLevel()->newDirection(m_row, m_col, 0, getSymbol()))
                        getLevel()->newDirection(m_row, m_col, 2, getSymbol());
                    setRow(m_row);
                    setCol(m_col);
                }
                else if (m_col < p_col){
                    //choose either up or right (0 or 3)
                    if(!getLevel()->newDirection(m_row, m_col, 0, getSymbol()))
                        getLevel()->newDirection(m_row, m_col, 3, getSymbol());
                    setRow(m_row);
                    setCol(m_col);
                }
                else{ //equal column
                    //only up
                    getLevel()->newDirection(m_row, m_col, 0, getSymbol());
                    setRow(m_row);
                    setCol(m_col);
                }
            }
            else if (m_row < p_row){
                if (m_col > p_col){
                    //choose either down or left (1 or 2)
                    if(!getLevel()->newDirection(m_row, m_col, 1, getSymbol()))
                        getLevel()->newDirection(m_row, m_col, 2, getSymbol());
                    setRow(m_row);
                    setCol(m_col);
                }
                else if (m_col < p_col){
                    //choose either down or right (1 or 3)
                    if(!getLevel()->newDirection(m_row, m_col, 1, getSymbol()))
                        getLevel()->newDirection(m_row, m_col, 3, getSymbol());
                    setRow(m_row);
                    setCol(m_col);
                }
                else{ //equal column
                    //only down
                    getLevel()->newDirection(m_row, m_col, 1, getSymbol());
                    setRow(m_row);
                    setCol(m_col);
                }
            }
            else { //equal row
                if (m_col < p_col){  //move right
                    getLevel()->newDirection(m_row, m_col, 3, getSymbol());
                    setRow(m_row);
                    setCol(m_col);
                }
                else{  //move left
                    getLevel()->newDirection(m_row, m_col, 2, getSymbol());
                    setRow(m_row);
                    setCol(m_col);
                }
            }
        }
    }
}

void Dragon::moveMonster() {
    // Regain some hp, not more than max
    if (trueWithProbability(0.1)) {
        setHp(getHp() + 1);
    }
    Monster::moveMonster();
}

// This function looks similar to the Monster::withinRange function,
// but the key distinction is that this one takes in the goblin distance as an argument,
// which is important because the when recursively checking a path, the goblin's actual distance
// can't be used because technically the goblin isn't moving when the algorithm is running
bool Goblin::withinRange(int g_row, int g_col, int distance) {
    int p_row = getLevel()->getPlayer()->getRow();
    int p_col = getLevel()->getPlayer()->getCol();
    if (abs(g_row - p_row) + abs(g_col - p_col) <= distance)
        return true;
    return false;
}

int Goblin::minimumSteps(int g_row, int g_col, int stepsTaken){
    // If there is no way the goblin could reach
    if (!withinRange(g_row, g_col, getSmellDistance() - stepsTaken))
        return 999;
    
    // Goblin reached the player
    if (g_row == getLevel()->getPlayer()->getRow() && g_col == getLevel()->getPlayer()->getCol())
        return stepsTaken;
    
    // If goblin is within range
    int north = 999;
    int south = 999;
    int east = 999;
    int west = 999;
    
    if (getLevel()->isNotWall(g_row, g_col) &&  getLevel()->isNotMonster(g_row, g_col)) {
        getLevel()->setCoord(g_row, g_col, '#');

        if (getLevel()->isNotWall(g_row + 1, g_col) && getLevel()->isNotMonster(g_row + 1, g_col)) {
            south = minimumSteps(g_row + 1, g_col, stepsTaken + 1);
        }
        if (getLevel()->isNotWall(g_row - 1, g_col) && getLevel()->isNotMonster(g_row - 1, g_col)) {
            north = minimumSteps(g_row - 1, g_col, stepsTaken + 1);
        }
        if (getLevel()->isNotWall(g_row, g_col + 1) && getLevel()->isNotMonster(g_row, g_col + 1)) {
            east = minimumSteps(g_row, g_col + 1, stepsTaken + 1);
        }
        if (getLevel()->isNotWall(g_row, g_col - 1) && getLevel()->isNotMonster(g_row, g_col - 1)) {
            west = minimumSteps(g_row, g_col - 1, stepsTaken + 1);
        }
        getLevel()->clearCoord(g_row, g_col);
    }


    return min({north, east, west, south});
}

void Goblin::moveMonster() {
    if (getSleep() == 0) {
        getLevel()->setCoord(getRow(), getCol(), '#');
        int moveUp = minimumSteps(getRow() - 1 , getCol(), 1);
        int moveDown = minimumSteps(getRow() + 1 , getCol(), 1);
        int moveLeft = minimumSteps(getRow(), getCol() - 1, 1);
        int moveRight = minimumSteps(getRow(), getCol() + 1, 1);
        getLevel()->clearCoord(getRow(), getCol());

        int dir = min({moveUp, moveDown, moveLeft, moveRight});
        if (dir != 999) {
            int row = getRow();
            int col = getCol();
            if (dir == moveUp)
                getLevel()->newDirection(row, col, 0, 'G');
            else if (dir == moveDown)
                getLevel()->newDirection(row, col, 1, 'G');
            else if (dir == moveLeft)
                getLevel()->newDirection(row, col, 2, 'G');
            else if (dir == moveRight)
                getLevel()->newDirection(row, col, 3, 'G');
            setRow(row);
            setCol(col);
        }
    }
}




BogeyMen::BogeyMen(Level *someLevel, int hp, int strength, int armor, int dex, char symbol, int smellDistance, string type)
: Monster(someLevel, hp, strength, armor, dex, symbol, smellDistance, type), m_shortSword(someLevel, 1, 1, true){ // Weapon location irrelevant
    setCurrentWeapon(&m_shortSword);
}



void BogeyMen::dropItem() {
    if (getLevel()->getObjectOnSquare(getRow(), getCol()) == nullptr) { // If no object on the square, may drop one
        if (trueWithProbability(0.1)) {
            getLevel()->addObject<MagicAxe>(getRow(), getCol(), false);
        }
    }
}

SnakeWomen::SnakeWomen(Level* someLevel, int hp, int strength, int armor, int dex, char symbol, int smellDistance, string type)
: Monster(someLevel, hp, strength, armor, dex, symbol, smellDistance, type), m_magicFang(someLevel, 1, 1, true) { // Weapon location irrelevant
    setCurrentWeapon(&m_magicFang);
}



void SnakeWomen::dropItem() {
    if (getLevel()->getObjectOnSquare(getRow(), getCol()) == nullptr) { // If no object on the square, may drop one
        if (trueWithProbability(0.33333)) {
            getLevel()->addObject<MagicFang>(getRow(), getCol(), false);
        }
    }
}

Dragon::Dragon(Level* someLevel, int hp, int strength, int armor, int dex, char symbol, int smellDistance, string type)
: Monster(someLevel, hp, strength, armor, dex, symbol, smellDistance, type), m_longSword(someLevel, 1, 1, true) { // Weapon location irrelevant
    setCurrentWeapon(&m_longSword);
}



void Dragon::dropItem() {
    if (getLevel()->getObjectOnSquare(getRow(), getCol()) == nullptr) { // If no object on the square, may drop one
        int chance = randInt(1,5);
        switch (chance) {
            case 1:
                getLevel()->addObject<Teleport>(getRow(), getCol(), false);
                break;
            case 2:
                getLevel()->addObject<MoreArmor>(getRow(), getCol(), false);
                break;
            case 3:
                getLevel()->addObject<MoreDexterity>(getRow(), getCol(), false);
                break;
            case 4:
                getLevel()->addObject<MoreStrength>(getRow(), getCol(), false);
                break;
            case 5:
                getLevel()->addObject<MoreHealth>(getRow(), getCol(), false);
                break;
        }
    }
}

Goblin::Goblin(Level* someLevel, int hp, int strength, int armor, int dex, char symbol, int smellDistance, string type)
: Monster(someLevel, hp, strength, armor, dex, symbol, smellDistance, type), m_shortSword(someLevel, 1, 1, true) { // Weapon location irrelevant
    setSmellDistance(getLevel()->getGoblinSmellDistance()); //override dummy smell distance with passed in smell distance;
    
    setCurrentWeapon(&m_shortSword);
}


void Goblin::dropItem() {
    if (getLevel()->getObjectOnSquare(getRow(), getCol()) == nullptr) { // If no object on the square, may drop one
        if (trueWithProbability(0.333)){
            int chance = randInt(1,2);
            switch (chance) {
                case 1:
                    getLevel()->addObject<MagicAxe>(getRow(), getCol(), false);
                    break;
                default:
                    getLevel()->addObject<MagicFang>(getRow(), getCol(), false);
                    break;
            }
        }
    }
}
