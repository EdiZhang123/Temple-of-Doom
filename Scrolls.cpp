#include "Scrolls.h"
#include "utilities.h"
#include "Level.h"
#include "Player.h"


Scroll::Scroll(Level* someLevel, int r, int c, bool random, string read, string extra)
: Object(someLevel, '?', read, r, c, random), extraLine(extra)
{}

Scroll::~Scroll(){}

string Scroll::getExtraLine(){
    return extraLine;
}


Teleport::Teleport(Level *someLevel, int r, int c, bool random, string read, string extra)
: Scroll(someLevel, r, c, random, read, extra) {}




void Teleport::applyScroll(Player* player) {
    //teleport to random place
    player->getLevel()->clearCoord(player->getRow(), player->getCol());
    int r_player = randInt(1, 17);
    int c_player = randInt(1, 69);
    while (!(player->getLevel()->isNotWall(r_player, c_player) && player->getLevel()->isNotActor(r_player, c_player))) {
            r_player = randInt(1, 17);
            c_player = randInt(1, 69);
        }
    player->setRow(r_player);
    player->setCol(c_player);
}



MoreArmor::MoreArmor(Level *someLevel, int r, int c, bool random, string read, string extra)
: Scroll(someLevel, r, c, random, read, extra) {}

void MoreArmor::applyScroll(Player *player) {
    int currentArmor = player->getArmor();
    player->setArmor(currentArmor + randInt(1, 3));
}


MoreHealth::MoreHealth(Level *someLevel, int r, int c, bool random, string read, string extra)
: Scroll(someLevel, r, c, random, read, extra) {}

void MoreHealth::applyScroll(Player *player) {
    player->IncreaseMaxHealth(randInt(3, 8));
}



MoreStrength::MoreStrength(Level *someLevel, int r, int c, bool random, string read, string extra)
: Scroll(someLevel, r, c, random, read, extra) {}

void MoreStrength::applyScroll(Player *player) {
    int currentStrength = player->getStrength();
    player->setStrength(currentStrength + randInt(1,3));
}


MoreDexterity::MoreDexterity(Level *someLevel, int r, int c, bool random, string read, string extra)
: Scroll(someLevel, r, c, random, read, extra) {}

void MoreDexterity::applyScroll(Player *player) {
    int currentDex = player->getDex();
    player->setDex(currentDex + 1);
}
