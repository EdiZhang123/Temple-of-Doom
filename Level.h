#ifndef LEVEL_H
#define LEVEL_H

#include <list>
#include <queue>
#include <string>

using namespace std;

const int MAXROWS = 18;
const int MAXCOLS = 70;

const int r_maxRows = 8;
const int r_maxCols = 13;

const int r_minRows = 5;
const int r_minCols = 8;

class Player;
class Object;
class Monster;
class Actor;
class Game;

class Level{
public:
    Level(Player* m_player, Game* m_game, int goblinSmell);
    ~Level();
    
    void setUp();
    void buildRoom(char grid[18][70], int sr, int sc, int rows, int cols);
    void display();
    
    
    bool isNotWall(int r, int c) const;

    // Marking these function as const resulted in compile errors since I used iterators in the implementations
    bool isNotActor(int r, int c) ;
    bool isNotObject(int r, int c) ;
    bool isNotMonster(int r, int c) ;
    
    // Movement
    bool newDirection(int &r, int &c, int dir, char symbol);
    
    
    
    void moveMonstersLevel();
    
    int getGoblinSmellDistance() const;
    
    int getLevelNumber() const;
    void setLevelNumber(int num);
    
    bool isOnEndPoint(int r, int c, int type) const;
    void setEndCoords(int r, int c);
    void clearCoord(int r, int c);
    void setCoord(int r, int c, char x);
    
    // Pointer Accessors
    Game* getGame() const;
    Player* getPlayer() const;
    
    Object* getObjectOnSquare(int r, int c);
     
    // Adding to level
    void addStairWell();
    void addGoldenIdol();
    
    template <typename O>
    void addObject(int r, int c, bool random);

    template <typename M>
    void addMonster();

    void addPlayer();
    
    // Removing from level
    void removeMonster(Monster* m);
    void removeObject(Object* o);
    
    // Printing out actions
    void emptyActionQueue();
    void pushOnActionQueue(string s);
    
    // Sleep related
    void decrementMonsterSleep(Monster* m);
    void decrementPlayerSleep();
    
    // Used for checking if player reached end of level
    bool reachedEnd() const;
    void setReachedEnd(bool value);
private:
    list <Object*> objects;
    list <Monster*> monsters;
    
    queue<string> actions;

    Player* player; //pointer to the player
    Game* game;
    
    int level_number;
    int goblinSmellDistance;
    
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
    int endCoords[2];
    int endType; // 1 for stairwell, 2 for golden idol
    
    bool endOfLevelReached;
    
};


template <typename O>
void Level::addObject(int r, int c, bool random) {
    objects.push_back(new O(this, r, c, random));
}


#endif
