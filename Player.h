#ifndef Player_H
#define Player_H

#include "Actors.h"
#include "Level.h"
#include "Objects.h"
#include <string>
using namespace std;

class Level;
class Monster;
class Weapon;
class Scroll;

class Player : public Actor{

public:
    Player(Level *level, int r, int c);
    ~Player();
    void movePlayer(int dir);
    
    void showInventory(int action);
    
    void wieldWeapon(Weapon* w);
    void readScroll(Scroll* s);

    
    
    bool pickUpItem(Object* o);
    void discardScroll(Scroll* s);
    
private:
    list <Object*> inventory; //inventory is a linked list of pointers to objects (max 25)
};

#endif
