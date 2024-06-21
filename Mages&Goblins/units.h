#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <memory>
#include <string>
#include "field.h"


//enum GoblinType {Patrol, Tracking};


/* Class for units */
class Unit {
public:

    // Constructor
    Unit(bool sd, int row, int col, bool life = true, char dir = '\0', int moves = 0, int vision = 0);

    // Check which side the unit belongs to
    bool getSide() const;


    // Get and set row(col) of the unit
    int getRow() const;
    void setRow(int row);

    int getCol() const;
    void setCol(int col);

    //Get and set the life of a unit
    bool getLife() const;
    void setDead();
    //Reverse the dir of PG
    void reverseDir();
    //Get the dir of PG
    char getDir() const;
    //Get the moves of PG
    int getMoves() const;
    //Get the vision of TG
    int getVision() const;

private:
    // Position in the field
    int row;
    int col;
    bool side;
    bool life;
    //dir and moves are set for PG
    char dir;
    int moves;
    //vision is set for TG
    int vision;
};

std::string getUnitSymbol(const Unit& u);
#endif // UNITS_H_INCLUDED
