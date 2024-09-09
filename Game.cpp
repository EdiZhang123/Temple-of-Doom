// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
#include "Level.h"
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
: gameWon(false), goblinSmellDistance(goblinSmellDistance) {
    level = new Level(nullptr, this, goblinSmellDistance);
    level->addPlayer();
    m_player = level->getPlayer();
    level->setUp();
    level->display();
}

Game::~Game() {
    delete level;
    delete m_player;
}

Player* Game::getPlayer() const {
    return m_player;
}

Level* Game::getLevel() const {
    return level;
}
void Game::newLevel(int prevLevel){
    delete level;
    level = new Level(m_player, this, goblinSmellDistance);
    level->setLevelNumber(prevLevel + 1);
    m_player->setLevel(level);
    level->setUp();
}


int Game::decodeDirection(char dir){
    switch (dir)
    {
        case ARROW_UP:  return 0;
        case ARROW_DOWN:  return 1;
        case ARROW_LEFT:  return 2;
        case ARROW_RIGHT:  return 3;
        case '>': return 4;
        case 'w': return 5;
        case 'i': return 6;
        case 'r': return 7;

    }
    return -1;  // bad argument passed in!
    
}

void Game::play()
{
    while (m_player->isAlive() && gameWon == false) { //more conditions needed
        if (trueWithProbability(0.1)) {
            if (m_player->getHp() < m_player->getMaxHealth())
                m_player->setHp(m_player->getHp() + 1);
        }
    
        char action = getCharacter();
        
        if (m_player->getSleep() == 0) {
            switch (action) {
                case 'q': { // Quit game
                    return;
                }
                case 'c': { // Cheat
                    m_player->IncreaseMaxHealth(50 - m_player->getMaxHealth());
                    m_player->setHp(50);
                    m_player->setStrength(9);
                    m_player->movePlayer(decodeDirection(action)); // Move player
                    break;
                }
                case 'g': {
                    Object* o = getLevel()->getObjectOnSquare(m_player->getRow(), m_player->getCol()); // Get Pointer to object on square
                    if (o != nullptr) { // If there is object on square
                        GoldenIdol* g = dynamic_cast<GoldenIdol*>(o);
                        if (g != nullptr) { // o points to the golden idol
                            getLevel()->setReachedEnd(true);
                            getLevel()->pushOnActionQueue("You pick up the golden idol");
                            getLevel()->pushOnActionQueue("Congratulations, you won!");
                            getLevel()->getGame()->setGameWon(true);
                            break;
                            
                        }
                        StairWell* sw = dynamic_cast<StairWell*>(o);
                        // o is not pointing to the stairwell (Cannot pick up the stairwell
                        if (sw == nullptr) { // Some item
                            if (m_player->pickUpItem(o))
                                getLevel()->pushOnActionQueue("You pick up " + o->getRead());
                            else
                                getLevel()->pushOnActionQueue("Your knapsack is full; you can't pick that up.");
                        }
                    }
                    // If o is nullptr, there is no object on the square, so do nothing
                    break;
                }
                
                // Cases that clear screen and open inventory
                case 'i':
                case 'w':
                case 'r':
                    m_player->showInventory(decodeDirection(action));
                default: {
                    m_player->movePlayer(decodeDirection(action)); // Move player
                    break;
                }
            }
        }
        else
            getLevel()->decrementPlayerSleep();
        
        if (!getLevel()->reachedEnd()) // Monsters can't attack you if your move finishes the level
            m_player->getLevel()->moveMonstersLevel(); // Move monsters
        getLevel()->display(); // Show the screen
        getLevel()->emptyActionQueue(); // Print out all the actions in order
    }

    cout << "Press q to exit game." << endl;
    while (getCharacter() != 'q')
    {}
}

bool Game::getGameWon() const{
    return gameWon;
}

void Game::setGameWon(bool value) {
    gameWon = value;
}

