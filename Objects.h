#ifndef OBJECTS_H
#define OBJECTS_H
#include <string>
using namespace std;

class Level;
class Actor;

class Object{
public:
    Object(Level *someLevel, char symbol, string read, int r, int c, bool random);
    virtual ~Object();
    
    // Accessors and Mutators
    void setLevel(Level &someLevel);
    
    Level* getLevel();
    
    void setRow(int row);
    void setCol(int col);
    int getRow();
    int getCol();
    
    char getSymbol();
    void setSymbol(char symbol);
    
    string getRead();
    
private:
    Level* m_level;
    
    // location
    int m_r;
    int m_c;
    
    // Symbol on Grid
    char m_symbol;

    // Name of object
    string m_read;
};

class StairWell : public Object{
public:
    StairWell(Level *someLevel);
};

class GoldenIdol : public Object{
public:
    GoldenIdol(Level *someLevel);
    //only construct when creating final level
};

#endif
