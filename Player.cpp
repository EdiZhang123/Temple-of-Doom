#include "Player.h"
#include "utilities.h"
#include "Game.h"
#include "Monsters.h"
#include "Scrolls.h"
#include <iostream>
using namespace std;

Player::Player(Level* level, int r, int c)
: Actor(20, 2, 2, 2, '@', level, "Player ") {
    ShortSword* s = new ShortSword(getLevel(), 1, 1, true); // Location of this sword is irrelevant, put dummy arguments
    inventory.push_back(s);
    setCurrentWeapon(s);
    setRow(r);
    setCol(c);
}

Player::~Player() {
    list<Object*>::iterator i;
    for (i = inventory.begin(); i != inventory.end(); i++) {
        delete *i;
    }
}

void Player::movePlayer(int dir){
    if (getSleep() == 0) { // Can only move if not asleep
        int r = getRow();
        int c = getCol();
        
        if (getLevel()->newDirection(r, c, dir, '@'))
        {
            switch (dir) {
                case 4: { // >
                    getLevel()->setReachedEnd(true);
                    Game* currentGame = getLevel()->getGame();
                    currentGame->newLevel(getLevel()->getLevelNumber());
                    break;
                }
                default: {
                    setRow(r);
                    setCol(c);
                    break;
                }
            }
        }
    }
}



void Player::wieldWeapon(Weapon* w){
    setCurrentWeapon(w);
}

void Player::readScroll(Scroll* s){
    s->applyScroll(this);
}

void Player::showInventory(int action){
    // 5 to wield
    // 6 for inv
    // 7 to read
    bool openInventory = true;
    
    // Display the inventory
    clearScreen();
    cout << "Inventory:" << endl;
    char start = 'a';
    list<Object*>::iterator i;
    for (i = inventory.begin(); i != inventory.end(); i++, start++){
        cout << start << ". " << (*i)->getRead() << endl;
    }
    while (openInventory) {
        char enter = getCharacter();
        if ('a' <= enter && enter <= 'z') {
            int p;
            if (enter - 96 <= inventory.size()){ // Only consider entries within the range of the inventory
                for (i = inventory.begin(), p = 0; p < enter - 97; i++, p++){} // Iterate until i points to the desired pointer
                
                Scroll* s = dynamic_cast<Scroll*>(*i); // Try to convert object pointer to scroll
                Weapon* w = dynamic_cast<Weapon*>(*i); // Try to convert object pointer to weapon
                if (s != nullptr) {
                    // *i points to a scroll
                    if (action == 7) {
                        string result = s->getRead();
                        string extra = s->getExtraLine();
                        readScroll(s);
                        discardScroll(s);
                        getLevel()->pushOnActionQueue("You read " + result);
                        getLevel()->pushOnActionQueue(extra);
                    }
                    else if (action == 5){ // Initially pressed w
                        getLevel()->pushOnActionQueue("You can't wield a scroll");
                    }
                }
                else if (w != nullptr) {
                    // *i points to a weapon
                    if (action == 5) {
                        wieldWeapon(w);
                        string result = w->getRead();
                        getLevel()->pushOnActionQueue("You are wielding a " + result);
                    }
                    else if (action == 7){ // Initially pressed r
                        getLevel()->pushOnActionQueue("You can't read a weapon");
                    }
                }
            }
        }
        openInventory = false;
    }
}

// Return true if successfully picked up item, false otherwise
bool Player::pickUpItem(Object* o){
    if (inventory.size() <= 25) {
        getLevel()->removeObject(o); // Remove object from level's object list
        inventory.push_back(o); // Add object to player's inventory
        return true;
    }
    return false;
}

// remove iterator from player inventory and delete memory allocated 
void Player::discardScroll(Scroll* s) {
    list<Object*>::iterator o;
    for (o = inventory.begin(); o != inventory.end(); o++) {
        if (*o != nullptr) {
            if ((*o) == s) {
                inventory.erase(o); // Remove the node in the list
                break;
            }
        }
    }
    // delete memory allocated
    delete s;
}
