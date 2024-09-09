// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.

#include "Player.h"
#include "Level.h"

class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game();
    
    int decodeDirection(char dir);
    void play();
    void newLevel(int prevLevel);
    
    // Accessors
    Player* getPlayer() const;
    Level* getLevel() const;
    
    bool getGameWon() const;
    void setGameWon(bool value);
private:
    Player* m_player;
    Level* level;
    
    bool gameWon;
    int goblinSmellDistance;
};

#endif // GAME_INCLUDED
