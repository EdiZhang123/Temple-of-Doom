#include "Objects.h"
#include "Actors.h"
#include "utilities.h"
#include "Level.h"


Object::Object(Level *someLevel, char symbol, string read, int r, int c, bool random)
: m_level(someLevel), m_symbol(symbol), m_read(read){
    if (random){ // Object needs to be randomly placed
        int row = randInt(1, 1);
        int column = randInt(1, 69);
        while (!(someLevel->isNotWall(row, column) && someLevel->isNotObject(row, column))) {
            row = randInt(1, 17);
            column = randInt(1, 69);
        }
        m_r = row;
        m_c = column;
        if (symbol == '>' || symbol == '&')
            someLevel->setEndCoords(row, column);
    }
    else { // Object needs to be placed in set location (when monster dies and drops item)
        m_r = r;
        m_c = c;
    }
}

Object::~Object() {}

StairWell::StairWell(Level *someLevel)
: Object(someLevel, '>', "", 1, 1, true) {
}

GoldenIdol::GoldenIdol(Level *someLevel)
: Object(someLevel, '&', "", 1, 1, true) {
}


void Object::setLevel(Level &someLevel) {
    m_level = &someLevel;
}

Level* Object::getLevel(){
    return m_level;
}


void Object::setRow(int row){
    m_r = row;
}
void Object::setCol(int col){
    m_c = col;
}

int Object::getRow(){
    return m_r;
}

int Object::getCol(){
    return m_c;
}

char Object::getSymbol(){
    return m_symbol;
}

void Object::setSymbol(char symbol){
    m_symbol = symbol;
}

string Object::getRead(){
    return m_read;
}
