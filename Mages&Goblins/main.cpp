#include <iostream>
#include <fstream>
#include "field.h"
#include "units.h"
#include "engine.h"

using namespace std;
//Entrance to the game
int main(int argc, char* argv[])
{

    //string filename = "map.txt";
    /*ifstream ifs;
    ifs.open(filename.c_str());
    if (!ifs) {
        cout << "Cannot open the file: " << filename << endl;
        return -1;
    }*/
    Vector<Unit*> units;
    Field* f = loadMap(cin, units);
    //Field* f = loadMap(ifs, units);
    if (f == NULL) {
        cout << "Failed to load map!" << endl;
        return 0;
    }
    play(*f, cin, cout,units);
    /*string ofilename = "1.out";
    ofstream ofs;
    ofs.open(ofilename.c_str());
    play(*f, ifs, ofs, units);
     if (!ofs) {
        cout << "Cannot open the file: " << ofilename << endl;
        return -1;
    }*/
    delete f;
    for (int i = 0; i < units.size(); i++) {delete units[i];}

    //ifs.close();
    return 0;
}
