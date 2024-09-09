#include "Weapons.h"
#include "utilities.h"
#include "Level.h"



Weapon::Weapon(int r, int c, bool random, string action, int dexBonus, int damage, Level* someLevel, string read)
: Object(someLevel, ')', read, r, c, random), m_action(action), m_dexBonus(dexBonus), m_damage(damage) {
}

Weapon::~Weapon(){}

string Weapon::getAction(){
    return m_action;
}

int Weapon::getDexBonus(){
    return m_dexBonus;
}

int Weapon::getDamage(){
    return m_damage;
}



Mace::Mace(Level* someLevel, int r, int c, bool random)
: Weapon(r, c, random, "swings mace at ", 0, 2, someLevel, "mace") {
}


ShortSword::ShortSword(Level* someLevel, int r, int c, bool random)
: Weapon(r, c, random, "slashes short sword at ", 0, 2, someLevel, "short sword") {}



LongSword::LongSword(Level* someLevel, int r, int c, bool random)
: Weapon(r, c, random, "slashes long sword at ", 2, 4, someLevel, "long sword") {}



MagicAxe::MagicAxe(Level* someLevel, int r, int c, bool random)
: Weapon(r, c, random, "chops magic axe at ", 5, 5, someLevel, "magic axe") {}



MagicFang::MagicFang(Level* someLevel, int r, int c, bool random)
: Weapon(r, c, random, "strikes magic fangs at ", 3, 2, someLevel, "magic fangs of sleep") {}
