#ifndef SCROLLS_H
#define SCROLLS_H

#include "Objects.h"
#include <string>
using namespace std;

class Player;

class Scroll : public Object{
public:
    // When a scroll is read, it performs its effect and then magically self-destructs (removed from inventory)
    Scroll(Level* someLevel, int r, int c, bool random, string read, string extra);
    
    virtual void applyScroll(Player* player) = 0;
    
    virtual ~Scroll(); //remove from player's inventory
    
    string getExtraLine();
private:
    string extraLine; // Extra line when applying scrolls
};

class Teleport : public Scroll{
public:
    Teleport(Level* someLevel, int r, int c, bool random, string read = "a scroll called scroll of teleportation", string extra = "You feel your body wrenched in space and time.");
    
    // The player is randomly moved to another place in the level that is not occupied by a wall or a monster.
    void applyScroll(Player* player);
};

class MoreArmor : public Scroll{
public:
    MoreArmor(Level* someLevel, int r, int c, bool random, string read = "a scroll called scroll of improved armor", string extra = "Your armor glows blue.");

    //The player's armor points are increased by a random integer from 1 to 3.
    void applyScroll(Player* player);
};

class MoreStrength : public Scroll{
public:
    MoreStrength(Level* someLevel, int r, int c, bool random, string read = "a scroll called scroll of strength", string extra = "Your muscles bulge.");

    //The player's strength points are increased by a random integer from 1 to 3.
    void applyScroll(Player* player);
};

class MoreHealth : public Scroll{
public:
    
    MoreHealth(Level* someLevel, int r, int c, bool random, string read = "a scroll called scroll of enhanced health", string extra = "You feel your heart beating stronger.");

    //The player's maximum hit point value is increased by a random integer from 3 to 8. This scroll does not affect the player's current number of hit points.
    void applyScroll(Player* player);
};

class MoreDexterity : public Scroll{
public:
    
    MoreDexterity(Level* someLevel, int r, int c, bool random, string read = "a scroll called scroll of enhance dexterity", string extra = "You feel like less of a klutz.");

    //The player's dexterity is increased by 1.
    void applyScroll(Player* player);
};

#endif
