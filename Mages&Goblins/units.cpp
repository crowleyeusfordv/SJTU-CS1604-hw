#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

// Constructor
Unit::Unit(bool sd, int rw, int cl, bool lf, char d, int m, int vsn)
{
    side = sd;
    row = rw;
    col = cl;
    life = lf;
    dir = d;
    moves = m;
    vision = vsn;
}

// Get the side
bool Unit::getSide() const
{
    return side;
}

// Get and set the row[col] of a unit
int Unit::getRow() const
{
    return row;
}

int Unit::getCol() const
{
    return col;
}

void Unit::setRow(int r)
{
    row = r;
}

void Unit::setCol(int c)
{
    col = c;
}

//Get and set the life of a unit

bool Unit::getLife() const
{
    return life;
}

void Unit::setDead()
{
    life = false;
}

//Get the dir of PG
char Unit::getDir() const
{
    return dir;
}

//Reverse the dir of PG
void Unit::reverseDir()
{
    switch (dir)
    {
    case 'W':
        dir = 'S';
        break;
    case 'S':
        dir = 'W';
        break;
    case 'A':
        dir = 'D';
        break;
    case 'D':
        dir = 'A';
    }
}

//Get the moves of PG
int Unit::getMoves() const
{
    return moves;
}

//Get the vision of a goblin
int Unit::getVision() const
{
    return vision;
}

// Get the symbol of the unit
string getUnitSymbol(const Unit& u)
{
    bool side = u.getSide();
    if (side){
        return "@@";
    }
    else{
        if (u.getVision() == 0) return "PG";
        else return "TG";
    }
    assert (false);
}


