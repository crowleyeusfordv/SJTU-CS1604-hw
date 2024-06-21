#include <sstream>
#include <iomanip>
#include <cassert>
#include <Queue.h>
#include <string>
#include <sstream>
#include "engine.h"
#include "units.h"
#include "vector.h"
#include "Stack.h"
#include "algorithms.h"
#include <cmath>
using namespace std;

bool fireball(Field& field, int srow, int scol, char d);
bool wfireball(Field& field, int col, int start);
bool sfireball(Field& field, int col, int start);
bool dfireball(Field& field, int row, int start);
bool afireball(Field& field, int row, int start);
void tornado(Field& field, int row, int col, bool& suicide);
void blow(Field& field, int row, int col, int vr, int vc, bool& suicide);
int moveable(Field& field, int row, int col, int vr, int vc, int& moves);
void dirmove(Field& field, int row, int col, int vr, int vc, int moves);
int checkWin(Vector<Unit*>& units);
bool killMage(Field& field, Unit* u);
void dirgmove(Field& field, Unit* u);
void dgmove(Field& field, Unit* u, int vr, int vc);
bool inVision(Unit* mage, Unit* tg);

// Load map and units
Field* loadMap(istream& is, Vector<Unit*>& units)
{

    //Read in M N NT NM NG
    string read;
    getline(is,read);
    istringstream iss(read);
    int M, N, NT, NM, NG;
    if (!(iss >> M >> N >> NT >> NM >> NG)) return NULL;

    Field* Map = new Field(M,N);

    //Read in terrains
    for (int i = 0; i < NT; i++) {
        getline(is,read);
        istringstream iss(read);
        int row, col;
        char t;
        if (!(iss >> row >> col >> t) || !Map->inBounds(row, col)) return NULL;
        if (t == 'W') Map->setTerrain(row, col, WATER);
        else if (t == 'M') Map->setTerrain(row, col, MOUNTAIN);
        else return NULL;
    }

    //Read in mages
    for (int i = 0; i < NM; i++) {
        getline(is,read);
        istringstream iss(read);
        int row, col;
        if (!(iss >> row >> col) || !Map->inBounds(row, col)) return NULL;
        Unit *u = new Unit(true, row, col);
        Map->setUnit(row, col, u);
        units.add(u);
    }

    //Read in goblins
    for (int i = 0; i < NG; i++) {
        getline(is,read);
        istringstream iss(read);
        int row, col;
        string pt;
        if (!(iss >> row >> col >> pt) || !Map->inBounds(row, col)) return NULL;
        if (pt == "PG") {
            char d;
            int moves;
            iss >> d >> moves;
            Unit *u = new Unit(false, row, col, true, d, moves);
            Map->setUnit(row, col, u);
            units.add(u);
        }
        else if (pt == "TG"){
            int vision;
            iss >> vision;
            Unit *u = new Unit(false, row, col, true, '/0', 0, vision);
            Map->setUnit(row, col, u);
            units.add(u);
        }
        else return NULL;
    }

    return Map;
}


// Main loop for playing the game
void play(Field& field, istream& is, ostream& os, Vector<Unit*>& units)
{

    int numTurns = 1;
    while (is)
    {
        os << "Turn " << numTurns << " begins:" << endl;
        // Print the new map
        os << field << endl;

        //No mage or goblin,the result is a draw
        if (units.isEmpty()) {
            cout << "A Draw!" << endl;
            return;
        }

        // Check winning
        int w = checkWin(units);
        if (w == 1) {
            cout << "You Win!" << endl;
            return;
        }
        if (w == -1) {
            cout << "You Lose!" << endl;
            return;
        }
        // unit moves
        int i = 0;
        //Each mage
        while (units[i]->getSide()) {
            //Skip the dead mage
            if (!units[i]->getLife()) {
                i++;
                continue;
            }
            os << "Please move the unit at " << "(" << units[i]->getRow() << "," << units[i]->getCol() << ")" << endl;
            string userInput;
            getline(is, userInput);
            istringstream iss(userInput);
            char d;
            Vector<char> dirs;
            while (iss >> d) dirs.add(d);
            int moves = 1;
            int index = 0;
            int s = dirs.size();
            bool suicide = false;
            while (index < s && !suicide) {
                if (moves < 4) {
                    switch (dirs[index]) {
                        //Move
                        case 'W':
                            if (field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow() - 1, units[i]->getCol()))
                            moves++;
                            break;
                        case 'A':
                            if (field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow(), units[i]->getCol() - 1))
                            moves++;
                            break;
                        case 'S':
                            if (field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow() + 1, units[i]->getCol()))
                            moves++;
                            break;
                        case 'D':
                            if (field.moveUnit(units[i]->getRow(), units[i]->getCol(), units[i]->getRow(), units[i]->getCol() + 1))
                            moves++;
                            break;
                    }
                }
                bool magicdone = false;
                switch (dirs[index]) {
                    //tornado
                    case 'Y':
                        tornado(field, units[i]->getRow(), units[i]->getCol(), suicide);
                        magicdone = true;
                        break;
                    //fireball
                    case 'X':
                        //Miss directions
                        if (index + 1 == dirs.size() || (dirs[index + 1] != 'W' &&
                            dirs[index + 1] != 'A' && dirs[index + 1] != 'S' &&
                            dirs[index + 1] != 'D')) {
                                cout << "Direction is missing!" << endl;
                                break;
                            }

                        char magicdir = dirs[index + 1];
                        //Kill a mage
                        suicide = fireball(field, units[i]->getRow(), units[i]->getCol(), magicdir);
                        magicdone = true;
                        index++;
                        break;
                }
                if (magicdone) break;
                index++;
            }
            os << field << endl;
            i++;
        }
        //goblins move
        int s = units.size();
        for (int j = i; j < s; j++) {
            //Dead goblin
            if (!units[j]->getLife()) continue;
            //PG
            if (units[j]->getVision() == 0) dirgmove(field, units[j]);
            //TG
            else {
                Vector<pair<int,Vector<point>>> paths;
                bool nextm = false;
                for (int k = 0; k < i;k++) {
                    //Dead mage
                    if (!units[k]->getLife()) continue;
                    //The mage is invisible
                    if (!inVision(units[k], units[j])) continue;

                    pair<int,Vector<point>> temp = searchShortestPath(field, units[j]->getRow(), units[j]->getCol(),
                                                 units[k]->getRow(), units[k]->getCol());
                    //The mage is inaccessible
                    if (temp.first == -1) continue;
                    //The mage is next to TG(the length of path is 1)
                    if (temp.first == 1) {
                        killMage(field, units[j]);
                        nextm = true;
                        break;
                    }
                    paths.add(temp);
                }

                //If the mage is next to TG, the kill has been done
                if (nextm) continue;
                //No mage is visable
                if (paths.isEmpty()) continue;

                //Find the nearest mage
                int s = paths.size();
                int minIndex = 0;
                for (int k = 0; k < s; k++) {
                    if (paths[k].first < paths[minIndex].first) minIndex = k;
                }
                int targetR = (paths[minIndex].second[paths[minIndex].second.size() - 2]).first;//ERROR
                int targetC = (paths[minIndex].second[paths[minIndex].second.size() - 2]).second;
                //Check if multiple mages have the same shortest distance
                Vector<pair<int,Vector<point>>> shortpaths;
                for (int k = 0; k < s; k++) {
                    if (paths[k].first == paths[minIndex].first)
                        shortpaths.add(paths[k]);
                }
                //If multiple mages have the same shortest distance
                if (shortpaths.size() > 1) {
                    int m = 0;
                    for (int k = 0; k < shortpaths.size(); k++) {
                        if (coordOrder(shortpaths[k].second[shortpaths[k].second.size() - 1].first,
                                       shortpaths[k].second[shortpaths[k].second.size() - 1].second,
                                       shortpaths[m].second[shortpaths[m].second.size() - 1].first,
                                       shortpaths[m].second[shortpaths[m].second.size() - 1].second))
                            m = k;
                    }
                    targetR = shortpaths[m].second[shortpaths[m].second.size() - 2].first;
                    targetC = shortpaths[m].second[shortpaths[m].second.size() - 2].second;
                }
                field.moveUnit(units[j]->getRow(), units[j]->getCol(), targetR, targetC);
                killMage(field, units[j]);
            }
        }
        numTurns++;
    }
}


//Launch fireball according to the direction
bool fireball(Field& field, int srow, int scol, char d) {
    switch (d) {
    case 'W':
        return wfireball(field, scol, srow - 1);
    case 'A':
        return afireball(field, srow, scol - 1);
    case 'S':
        return sfireball(field, scol, srow + 1);
    case 'D':
        return dfireball(field, srow, scol + 1);
    }
}

//Launch fireball towards W
bool wfireball(Field& field, int col, int start) {
    for (int j = start; j >= 0; j--) {
        if (field.getTerrain(j, col) == MOUNTAIN) return false;
        //Kill a mage
        if ((field.   getUnit(j, col) != nullptr) && (field.getUnit(j, col)->getSide())) {
            field.getUnit(j, col)->setDead();
            field.removeUnit(j, col);
            return true;
        }
        //Kill a goblin
        if ((field.getUnit(j, col) != nullptr) && (!field.getUnit(j, col)->getSide())) {
            field.getUnit(j, col)->setDead();
            field.removeUnit(j, col);
            return false;
        }
    }
}

//Launch fireball towards S
bool sfireball(Field& field, int col, int start) {
    int stop = field.getHeight() - 1;
    for (int j = start; j <= stop; j++) {
        if (field.getTerrain(j, col) == MOUNTAIN) return false;
        //Kill a mage
        if ((field.getUnit(j, col) != nullptr) && (field.getUnit(j, col)->getSide())) {
            field.getUnit(j, col)->setDead();
            field.removeUnit(j, col);
            return true;
        }
        //Kill a goblin
        if ((field.getUnit(j, col) != nullptr) && (!field.getUnit(j, col)->getSide())) {
            field.getUnit(j, col)->setDead();
            field.removeUnit(j, col);
            return false;
        }
    }
}

//Launch fireball in D
bool dfireball(Field& field, int row, int start) {
    int stop = field.getWidth() - 1;
    for (int j = start; j <= stop; j++) {
        if (field.getTerrain(row, j) == MOUNTAIN) {
                return false;}

        //Kill a mage
        if ((field.getUnit(row, j) != nullptr) && (field.getUnit(row, j)->getSide())) {
            field.getUnit(row, j)->setDead();
            field.removeUnit(row, j);
            return true;
        }
        //Kill a goblin
        if ((field.getUnit(row, j) != nullptr) && (!field.getUnit(row, j)->getSide())) {
            field.getUnit(row, j)->setDead();
            field.removeUnit(row, j);
            return false;
        }
    }
}
//Launch fireball in A
bool afireball(Field& field, int row, int start) {
    for (int j = start; j >= 0; j--) {
        if (field.getTerrain(row, j) == MOUNTAIN) return false;
        //Kill a mage
        if ((field.getUnit(row, j) != nullptr) && (field.getUnit(row, j)->getSide())) {//Error
            field.getUnit(row, j)->setDead();
            field.removeUnit(row, j);
            return true;
        }
        //Kill a goblin
        if ((field.getUnit(row, j) != nullptr) && (!field.getUnit(row, j)->getSide())) {
            field.getUnit(row, j)->setDead();
            field.removeUnit(row, j);
            return false;
        }
    }
}

//Tornadoes in eight directions
void tornado(Field& field, int row, int col, bool& suicide) {
    blow(field, row, col, -1, -1, suicide);
    blow(field, row, col, -1, 0, suicide);
    blow(field, row, col, -1, 1, suicide);
    blow(field, row, col, 0, -1, suicide);
    blow(field, row, col, 0, 1, suicide);
    blow(field, row, col, 1, -1, suicide);
    blow(field, row, col, 1, 0, suicide);
    blow(field, row, col, 1, 1, suicide);
    return;
}

//Tornado in direction(vr,vc)
void blow(Field& field, int row, int col, int vr, int vc, bool& suicide) {
    if (!field.inBounds(row + vr, col + vc) || field.getUnit(row + vr, col + vc) == nullptr) return;
    int moves = 0;
    int x = moveable(field, row, col, vr, vc, moves);
    switch (x) {
        case 1:
            return;
        case 2:
            field.getUnit(row + vr * moves, col +  vc * moves)->setDead();
            field.removeUnit(row + vr * moves, col +  vc * moves);
            dirmove(field, row, col, vr, vc, moves - 1);
            return;
        case 3:
            field.getUnit(row + vr * moves, col +  vc * moves)->setDead();
            field.removeUnit(row + vr * moves, col +  vc * moves);
            dirmove(field, row, col, vr, vc, moves - 1);
            suicide = true;
            return;
        case 4:
            dirmove(field, row, col, vr, vc, moves);
            return;
    }
}

//Determine the moving situation in direction(vr,vc)
int moveable(Field& field, int row, int col, int vr, int vc, int& moves) {
    //Base case 1: out of bound or encounter MOUNTAIN
    if (!field.inBounds(row + vr, col + vc) ||
        field.getTerrain(row + vr, col + vc) == MOUNTAIN) return 1;
    //Base case 2: a goblin falls into WATER
    if (field.getTerrain(row + vr, col + vc) == WATER &&
        !field.getUnit(row, col)->getSide()) return 2;
    //Base case 3: a mage falls into WATER
    if (field.getTerrain(row + vr, col + vc) == WATER &&
        field.getUnit(row, col)->getSide()) return 3;
    //Base case 4: a unit move to PLAIN
    if (field.getUnit(row + vr, col + vc) == nullptr) return 4;
    moves++;

    return moveable(field, row + vr, col + vc, vr, vc, moves);
}

//Move in direction(vr,vc)
void dirmove(Field& field, int row, int col, int vr, int vc, int moves) {
    while (moves > 0) {
        int r = row + moves*vr;
        int c = col + moves*vc;
        field.moveUnit(r, c, r + vr, c + vc);
        moves--;
    }
}

//Check win/lose/not end
int checkWin(Vector<Unit*>& units) {
    int s = units.size();
    bool x;
    int y = 0;
    //Find what the first alive unit is
    for (int i = 0; i < s; i++) {
        if (units[i]->getLife()) {
            x = units[i]->getSide();
            break;
        }
        y++;
    }
    //Case 0: not end
    for (int i = y; i < s; i++) {
        if ((units[i]->getLife()) && (units[i]->getSide() != x))
            return 0;
    }
    //Case 1: win
    if (x) return 1;
    //Case -1: lose
    else return -1;
}

//Goblins kill every mage near them
bool killMage(Field& field, Unit* u) {
    int r = u->getRow();
    int c = u->getCol();
    bool x = false;
    //W
    if (field.inBounds(r - 1, c) &&
        field.getUnit(r - 1, c) != nullptr &&
        field.getUnit(r - 1, c)->getSide()) {
        field.getUnit(r - 1, c)->setDead();
        field.removeUnit(r - 1, c);
        x = true;
    }
    //S
    if (field.inBounds(r + 1, c) &&
        field.getUnit(r + 1, c) != nullptr &&
        field.getUnit(r + 1, c)->getSide()) {
        field.getUnit(r + 1, c)->setDead();
        field.removeUnit(r + 1, c);
        x = true;
    }
    //A
    if (field.inBounds(r, c - 1) &&
        field.getUnit(r, c - 1) != nullptr &&
        field.getUnit(r, c - 1)->getSide()) {
        field.getUnit(r, c - 1)->setDead();
        field.removeUnit(r, c - 1);
        x = true;
    }
    //D
    if (field.inBounds(r, c + 1) &&
        field.getUnit(r, c + 1) != nullptr &&
        field.getUnit(r, c + 1)->getSide()) {
        field.getUnit(r, c + 1)->setDead();
        field.removeUnit(r, c + 1);
        x = true;
    }
    return x;
}

//a goblin moves according to its direction
void dirgmove(Field& field, Unit* u) {
    char d = u->getDir();
    switch (d)
    {
    case 'W':
        dgmove(field, u, -1, 0);
        break;
    case 'S':
        dgmove(field, u, 1, 0);
        break;
    case 'A':
        dgmove(field, u, 0, -1);
        break;
    case 'D':
        dgmove(field, u, 0, 1);
    }
}

//a goblin moves according to vector(vr, vc)
void dgmove(Field& field, Unit* u, int vr, int vc){
    int moves = u->getMoves();
    for (int i = 0; i < moves; i++) {
        if (!field.inBounds(u->getRow() + vr, u->getCol() + vc) ||
            field.getTerrain(u->getRow() + vr, u->getCol() + vc) == MOUNTAIN ||
            field.getTerrain(u->getRow() + vr, u->getCol() + vc) == WATER ||
            field.getUnit(u->getRow() + vr, u->getCol() + vc) != nullptr) {
            killMage(field, u);
            u->reverseDir();
            return;
        }
        if (killMage(field,u)) {
            u->reverseDir();
            return;
        }
        field.moveUnit(u->getRow(), u->getCol(), u->getRow() + vr, u->getCol() + vc);
    }
    u->reverseDir();
    killMage(field,u);
}

//Check if the mage is in TG's vision
bool inVision(Unit* mage, Unit* tg) {
    return (abs(tg->getRow() - mage->getRow()) +
            abs(tg->getCol() - mage->getCol())) <= tg->getVision();
}
