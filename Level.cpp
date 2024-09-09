#include "Level.h"
#include "utilities.h"
#include "Player.h"
#include "Objects.h"
#include "Monsters.h"
#include "Scrolls.h"
#include <iostream>

using namespace std;

Level::Level(Player* m_player, Game *m_game, int goblinSmell)
: player(m_player), level_number(0), game(m_game), goblinSmellDistance(goblinSmell), endOfLevelReached(false)
{}

Level::~Level(){
    list<Object*>::iterator i;
    for (i = objects.begin(); i != objects.end(); i++){
        Object* o = *i;
        delete o;
    }
    objects.clear();
     
    list<Monster*>::iterator j;
    for (j = monsters.begin(); j != monsters.end(); j++){
        Monster* m = *j;
        delete m;
    }
    monsters.clear();
    
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            grid[i][j] = ' ';
        }
    }
}

bool Level::isOnEndPoint(int r, int c, int type) const{
    return endCoords[0] == r && endCoords[1] == c && endType == type;
}

void Level::setEndCoords(int r, int c) { 
    endCoords[0] = r;
    endCoords[1] = c;
    if (level_number != 4)
        endType = 1; //stairwell
    else
        endType = 2; //golden idol
    
}


void Level::clearCoord(int r, int c) {
    grid[r][c] = ' ';
}


void Level::addStairWell() {
    objects.push_back(new StairWell(this));
}

void Level::addGoldenIdol() {
    objects.push_back(new GoldenIdol(this));
}

// Build a room by clearing a set number of coordinates, determined by arguments
void Level::buildRoom(char grid[18][70], int sr, int sc, int rows, int cols){
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            grid[sr + i][sc + j] = ' ';
}

bool Level::isNotActor(int r, int c) {
    if (getPlayer()->getRow() == r && getPlayer()->getCol() == c)
        return false;
    list<Monster*>::iterator a;
    for (a = monsters.begin(); a != monsters.end(); a++) {
        if ((*a)->getRow() == r && (*a)->getCol() == c)
            return false;
    }
    return true;
}

bool Level::isNotWall(int r, int c) const{
    return grid[r][c] != '#';
}

bool Level::isNotObject(int r, int c) {
    list<Object*>::iterator a;
    for (a = objects.begin(); a != objects.end(); a++) {
        if ((*a)->getRow() == r && (*a)->getCol() == c)
            return false;
    }
    return true;
}

bool Level::isNotMonster(int r, int c)  {
    list<Monster*>::iterator a;
    for (a = monsters.begin(); a != monsters.end(); a++) {
        if ((*a)->getRow() == r && (*a)->getCol() == c)
            return false;
    }
    return true;
}

int Level::getLevelNumber() const{
    return level_number;
}

void Level::setLevelNumber(int num) {
    level_number = num;
}

int Level::getGoblinSmellDistance() const{
    return goblinSmellDistance;
}

Game* Level::getGame() const{
    return game;
}

Player* Level::getPlayer() const{
    return player;
}

bool Level::reachedEnd() const {
    return endOfLevelReached;
}

void Level::setReachedEnd(bool value) {
    endOfLevelReached = value;
}

template <typename M>
void Level::addMonster(){
    monsters.push_back(new M(this));
}

void Level::addPlayer() {
    player = new Player(this, 1, 1);
}

void Level::removeMonster(Monster* m) {
    clearCoord(m->getRow(),m->getCol());
    
    list<Monster*>::iterator n;
    for (n = monsters.begin(); n != monsters.end(); n++)
    {
        if (*n != nullptr){
            if ((*n) == m) {
                monsters.erase(n);
                break;
            }
            
        }
    }
}

void Level::removeObject(Object* o) {
    list<Object*>::iterator p;
    for (p = objects.begin(); p != objects.end(); p++)
    {
        if (*p != nullptr) {
            if ((*p) == o) {
                objects.erase(p);
                break;
            }
        }
        
    }
}

void Level::moveMonstersLevel() {
    list<Monster*>::iterator m;
    for (m = monsters.begin(); m != monsters.end(); m++)
    {
        if (*m != nullptr){
            if ((*m)->getSleep() == 0) 
                (*m)->moveMonster();
            else
                (*m)->getLevel()->decrementMonsterSleep(*m);
        }
    }
}


Object* Level::getObjectOnSquare(int r, int c) {
    list<Object*>::iterator o;
    for (o = objects.begin(); o != objects.end(); o++){
        if ((*o) != nullptr) {
            if ((*o)->getRow() == r && (*o)->getCol() == c)
                return *o;
        }
    }
    return nullptr; // If no object on square
}

void Level::setCoord(int r, int c, char x) {
    grid[r][c] = x;
}


void Level::emptyActionQueue() { // Print out the actions in order
    while (!actions.empty()) {
        cout << actions.front() << endl;
        actions.pop();
    }
}

void Level::pushOnActionQueue(string s) {
    actions.push(s);
}

void Level::decrementMonsterSleep(Monster* m) {
    int monsterSleep = m->getSleep();
    if (monsterSleep > 0)
        m->setSleep(m->getSleep() - 1);
}

void Level::decrementPlayerSleep() {
    int playerSleep = getPlayer()->getSleep();
    if (playerSleep > 0)
        getPlayer()->setSleep(playerSleep - 1);
}


bool Level::newDirection(int &r, int&c, int dir, char symbol){ //move if you can
    list<Monster*>::iterator m;
    switch (dir)
    {
        case 0: //up
            if (!isNotWall(r-1, c))
                return false;
            else if (!isNotActor(r-1, c)) {
                // Player attacks monster
                if (symbol == '@') {
                    for (m = monsters.begin(); m != monsters.end(); m++) {
                        if (*m != nullptr) {
                            if ((*m)->getRow() == (r-1) && (*m)->getCol() == c) // iterate until *m points to the monster player is attacking
                                break;
                        }
                    }
                    player->attack(*m);
                }
                // Monster attacks player
                else {
                    if (player->getRow() == r-1 && player->getCol() == c){
                        for (m = monsters.begin(); m != monsters.end(); m++) {
                            if (*m != nullptr){
                                if ((*m)->getRow() == (r) && (*m)->getCol() == c)
                                    break;
                            }
                        }
                        (*m)->attack(player);
                    }
                }
                return false;
            }
            else {
                clearCoord(r,c);
                grid[r-1][c] = symbol;
                r--;
            }
            break;
            
        case 1: //down
            if (!isNotWall(r+1, c))
                return false;
            else if (!isNotActor(r+1, c)) {
                // Player attacks monster
                if (symbol == '@') {
                    for (m = monsters.begin(); m != monsters.end(); m++) {
                        if (*m != nullptr){
                            if ((*m)->getRow() == (r+1) && (*m)->getCol() == c)
                                break;
                        }
                    }
                    player->attack(*m);
                }
                // Monster attacks player
                else {
                    if (player->getRow() == r+1 && player->getCol() == c) {
                        for (m = monsters.begin(); m != monsters.end(); m++) {
                            if (*m != nullptr){
                                if ((*m)->getRow() == (r) && (*m)->getCol() == c)
                                    break;
                            }
                        }
                        (*m)->attack(player);
                    }
                }
                return false;
            }
            else{
                clearCoord(r,c);
                grid[r+1][c] = symbol;
                r++;
            }
            break;
        case 2: //left
            if (!isNotWall(r, c-1))
                return false;
            else if (!isNotActor(r, c-1)) {
                // Player attacks monster
                if (symbol == '@') {
                    for (m = monsters.begin(); m != monsters.end(); m++) {
                        if (*m != nullptr){
                            if ((*m)->getRow() == (r) && (*m)->getCol() == c-1)
                                break;
                        }
                    }
                    player->attack(*m);
                }
                // Monster attacks player
                else {
                    if (player->getRow() == r && player->getCol() == c-1){
                        for (m = monsters.begin(); m != monsters.end(); m++) {
                            if (*m != nullptr) {
                                if ((*m)->getRow() == (r) && (*m)->getCol() == c)
                                    break;
                            }
                        }
                        (*m)->attack(player);
                    }
                }
                return false;
            }
            else {
                clearCoord(r,c);
                grid[r][c-1] = symbol;
                c--;
            }
            
            break;
        case 3: //right
            if (!isNotWall(r, c+1))
                return false;
            else if (!isNotActor(r, c+1)) {
                // Player attacks monster
                if (symbol == '@'){
                    for (m = monsters.begin(); m != monsters.end(); m++) {
                        if (*m != nullptr){
                            if ((*m)->getRow() == (r) && (*m)->getCol() == c+1)
                                break;
                        }
                    }
                    player->attack(*m);
                }
                // Monster attacks player
                else {
                    if (player->getRow() == r && player->getCol() == c+1) {
                        for (m = monsters.begin(); m != monsters.end(); m++) {
                            if (*m != nullptr){
                                if ((*m)->getRow() == (r) && (*m)->getCol() == c)
                                    break;
                            }
                        }
                        (*m)->attack(player);
                    }
                }
                return false;
            }
            else {
                clearCoord(r,c);
                grid[r][c+1] = symbol;
                c++;
            }
            break;
        case 4: // >
            if (!isOnEndPoint(r, c, 1))
                return false;
            break;
        default:
            return false;
    }
    return true;
}

void Level::setUp() {
    // Fill the grid with '#'
    for (r = 0; r < MAXROWS; r++)
        for (c = 0; c < MAXCOLS; c++)
            grid[r][c] = '#';
        
    // Create Layout
    int n_rows;
    int n_cols;
    int sr = 1;
    int sc = 1;
    
    
    int rightMost = 1; // Right most point reached
    int corridorRow = 1;
    int corridorLength = 1;
    for (int i = 0; i < 4 ; i++){
        n_rows = randInt(r_minRows, r_maxRows);
        n_cols = randInt(r_minCols, r_maxCols);
        if (i != 0)
            sr = randInt(corridorRow - n_rows + 1, corridorRow);
        else
            sr = randInt(1, MAXROWS - n_rows - 1);
        
        // Correct if past left bound
        while (sr < 1) {
            sr ++;
        }
        // Correct if past right bound
        while (sr + n_rows >= 18) {
            sr --;
            
        }
         
        // Limit room to not exceed right bound
        sc = rightMost;
        while (sc + n_cols >= 70) {
            n_cols-- ;
        }
        buildRoom(grid, sr, sc, n_rows, n_cols);
        rightMost += n_cols;
        if (i != 3){ // Rooms 1-3 can have corridors and extended rooms
            if (trueWithProbability(0.75)){ // chance of making new room and corridor above or below
                if (sr >= 8){ // room above
                    int a = randInt(1,2);
                    int b = randInt(sc, sc + n_cols - 1);
                    buildRoom(grid, a, b, sr - a, randInt(1,2)); // Build a corridor going up
                    int c = b - randInt(0, 4);
                    while (c < 1) { // Correct if out of bounds
                        c++;
                    }
                    buildRoom(grid, a, c, randInt(3, 5), randInt(6, 14)); // Build another room from the external corridor
                }
                else if (sr + n_rows - 1 <= 9){ // room below
                    int a = randInt(2,3);
                    int b = randInt(sc, sc + n_cols - 1);
                    buildRoom(grid, sr + n_rows, b, a, randInt(1,2)); // Build a corridor going down
                    int c = b - randInt(0, 4);
                    while (c < 1) { // Correct if out of bounds
                        c++;
                    }
                    buildRoom(grid, sr + n_rows + a - 1, c, randInt(3, 5), randInt(6, 14)); // Build another room from the external corridor
                }
            }
            corridorRow = randInt(sr, sr + n_rows - 1);
            corridorLength = randInt(5, 7);
            buildRoom(grid, corridorRow, sc + n_cols, 1, corridorLength); // Build corridor
            rightMost += corridorLength; // Save right most point for reference in building next room
        }
    }

    
    // Place Player
    int r_player = randInt(1, 17);
    int c_player = randInt(1, 69);
    while (grid[r_player][c_player] == '#'){
        r_player = randInt(1, 17);
        c_player = randInt(1, 69);
    }
    grid[r_player][c_player] = '@';
    player->setRow(r_player);
    player->setCol(c_player);
    
    
    
    // Add golden idol if levelnum is 4, otherwise place stairwell
    if (level_number == 4)
        addGoldenIdol();
    else
        addStairWell();
    
    //Place monsters
    int numOfMonsters = randInt(2, 5*(level_number+1)+1);
    for (int i = 0; i < numOfMonsters; i++){
        switch (level_number) {
            case 4:
            case 3:
                if (trueWithProbability(0.35))
                    addMonster<Dragon>();
            case 2:
                if (trueWithProbability(0.35))
                    addMonster<BogeyMen>();
            default:
                if (trueWithProbability(0.5))
                    addMonster<SnakeWomen>();
                else
                    addMonster<Goblin>();
                break;
        }
    }
    
    //Place Weapons and Scrolls
    for (int i = 0; i < randInt(2,3); i++){ //should be between 2,3
        int type = randInt(1,2);
        switch (type) {
            case 1:{ //scroll
                int typeScroll = randInt(1,4);
                switch (typeScroll) {
                    case 1:
                        addObject<MoreArmor>(1,1, true); // True randomizes location, so 1,1 is a necessary dummy argument that gets overriden
                        break;
                    case 2:
                        addObject<MoreStrength>(1,1, true);
                        break;
                    case 3:
                        addObject<MoreHealth>(1,1, true);
                        break;
                    case 4:
                        addObject<MoreDexterity>(1,1, true);
                        break;
                }
                break;}
            case 2:{ //weapon
                // no magic fang of sleep
                // no magic axe
                int typeWeapon = randInt(1,3);
                switch (typeWeapon) {
                    case 1:
                        addObject<Mace>(1,1, true); // True randomizes location, so 1,1 is a necessary dummy argument that gets overriden
                        break;
                    case 2:
                        addObject<ShortSword>(1,1, true);
                        break;
                    case 3:
                        addObject<LongSword>(1,1, true);
                        break;
                }
            }
        }
    }
    
}
void Level::display(){
    
    // Set Objects on grid first because objects can be under Actors
    list<Object*>::iterator i;
    for (i = objects.begin(); i != objects.end(); i++){
        if (*i != nullptr){
            char& gridChar = grid[(*i)->getRow()][(*i)->getCol()];
            gridChar = (*i)->getSymbol();
        }
    }

    // Set Monsters on grid
    list<Monster*>::iterator j;
    for (j = monsters.begin(); j != monsters.end(); j++){
        if (*j != nullptr){
            char& gridChar = grid[(*j)->getRow()][(*j)->getCol()];
            gridChar = (*j)->getSymbol();
        }
    }
    
    // Set Player on grid
    if (player != nullptr)
    {
        char& gridChar = grid[player->getRow()][player->getCol()];
        gridChar = '@';
    }
    
    // Draw screen
    clearScreen();
    for (r = 0; r < MAXROWS; r++)
    {
        for (c = 0; c < MAXCOLS; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
    cout << "Level: " << getPlayer()->getLevel()->getLevelNumber();
    cout << ", Hit points: " << getPlayer()->getHp();
    cout << ", Armor: " << getPlayer()->getArmor();
    cout << ", Strength: " << getPlayer()->getStrength();
    cout << ", Dexterity: " << getPlayer()->getDex() << endl;
    cout << '\n';
    
}

