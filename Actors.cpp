#include "Actors.h"
#include "utilities.h"
#include "Level.h"
#include "Game.h"
#include "Monsters.h"
#include <algorithm>
#include <iostream>
using namespace std;

Actor::Actor(int hp, int strength, int dex, int armor, char symbol, Level* someLevel, string name)
: m_hp(hp), m_strength(strength), m_dex(dex), m_armor(armor), MAXHEALTH(hp), m_symbol(symbol), m_level(someLevel), m_name(name), m_sleep(0) {}

Actor::~Actor(){}

int Actor::getRow() const {
    return m_row;
}

int Actor::getCol() const {
    return m_col;
}

bool Actor::isAlive() const {
    return m_hp > 0;
}

void Actor::setRow(int row){
    m_row = row;
}
void Actor::setCol(int col){
    m_col = col;
}


char Actor::getSymbol() const{
    return m_symbol;
}

void Actor::setSymbol(char symbol){
    m_symbol = symbol;
}

Level* Actor::getLevel() const{
    return m_level;
}

void Actor::setLevel(Level *someLevel){
    m_level = someLevel;
}
 
int Actor::getHp() const{
    return m_hp;
}

void Actor::setHp(int amount){
    if (amount > MAXHEALTH)
        m_hp = MAXHEALTH;
    else
        m_hp = amount;
}

int Actor::getArmor() const{
    return m_armor;
}

void Actor::setArmor(int amount){
    if (amount > 99)
        m_armor = 99;
    else
        m_armor = amount;
}

int Actor::getDex() const{
    return m_dex;
}

void Actor::setDex(int amount) {
    if (amount > 99)
        m_dex = 99;
    else
        m_dex = amount;
}

int Actor::getStrength() const{
    return m_strength;
}

void Actor::setStrength(int amount) {
    if (amount > 99)
        m_strength = 99;
    else
        m_strength = amount;
}


Weapon* Actor::getCurrentWeapon(){
    return currentWeapon;
}

void Actor::setCurrentWeapon(Weapon *w){
    currentWeapon = w;
}

string Actor::getName() const{
    return m_name;
}

int Actor::getSleep() const{
    return m_sleep;
}

void Actor::setSleep(int amount) {
    m_sleep = amount;
}

int Actor::getMaxHealth() const {
    return MAXHEALTH;
}

void Actor::IncreaseMaxHealth(int add){
    MAXHEALTH += add;
}

// Actor calling attacks the actor passed in
// this-> is the attacker, while a-> is the defender
void Actor::attack(Actor *a){
    string hitOrMiss;
    string nameOfAttacked = a->getName();
    int attack = this->getDex() + this->getCurrentWeapon()->getDexBonus();
    int defend = a->getDex() + a->getArmor();
    int damage = randInt(0, getStrength() + getCurrentWeapon()->getDamage() - 1);
    if (randInt(1, attack) >= randInt(1, defend)) {
        if (a->getHp() - damage <= 0) {
            hitOrMiss = "and delivers a final blow.";
            
            if (this->getSymbol() == '@') {
                // Case of player killing monster
                
                Monster *m = dynamic_cast<Monster*>(a); // Need to convert because only monsters can drop items
                m->dropItem();
                a->getLevel()->removeMonster(m); // Remove from linked list
                delete m; // Delete the monster
            }
            else {
                // Case of monster killing player
                a->setHp(0);
            }
        }
        else {
            // Magic fangs special ability
            if (this->getCurrentWeapon()->getRead() == "magic fangs of sleep") {
                if (trueWithProbability(0.2)) {
                    int X = randInt(2, 6);
                    if (a->getSleep() != 0) { // Already asleep
                        a->setSleep(max(X, a->getSleep()));
                    }
                    else {
                        a->setSleep(X);
                    }
                    hitOrMiss = "and hits, putting " + a->getName() + "to sleep.";
                }
                else {
                    hitOrMiss = "and hits.";
                    a->setHp(a->getHp() - damage);
                }
            }
            else {
                hitOrMiss = "and hits.";
                a->setHp(a->getHp() - damage);
            }
        }
    }
    else
        hitOrMiss = "and misses.";
    string s = this->getName() + getCurrentWeapon()->getAction() + nameOfAttacked + hitOrMiss; // Concatenate string
    getLevel()->pushOnActionQueue(s);
}



