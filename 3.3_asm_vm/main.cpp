#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include "stack.h"
#include "map.h"
#include <ctype.h>
#include <limits>

using namespace std;

bool containEmpty (string& s);

//Separate a string by empty spaces.
template <typename Out>
void split(const string &s, char delim, Out result) {
    istringstream iss(s);
    string item;
    while (getline(iss, item, delim)) {
        *result++ = item;
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}


//Simulate ASM virtual machine.
int main(){
    int command_num;
    cin >> command_num;
    cin.ignore();
    vector<vector<string>> command_line;//Store all the command lines.
    for (int i = 0 ; i < command_num ; i++) {
        string command;
        //cin.ignore (numeric_limits <streamsize> ::max (), 'n');
        getline(cin, command);
        if (containEmpty (command)){
            vector<string> x = split(command, ' ' );
            command_line.push_back(x);
        }
        else {
            vector<string> x = {command};
            command_line.push_back(x);
        }
    }
    int pc = 0;
    Stack<int> opnum;
    Map<string, int> state;
    while (pc >= 0 && pc < command_num) {
        vector<string> current_command = command_line[pc];
        string action = current_command[0];
        bool y = action == "Const";
        if (current_command.size() == 1) {
            if (action == "Add") {
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int a = opnum.pop();
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int b = opnum.pop();
                int c = a + b;
                opnum.push(c);
            }
            else if (action == "Sub") {
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int a = opnum.pop();
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int b = opnum.pop();
                int c = b - a;
                opnum.push(c);
            }
            else if (action == "Mul") {
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int a = opnum.pop();
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int b = opnum.pop();
                int c = a * b;
                opnum.push(c);
            }
            else if (action == "Div") {
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int a = opnum.pop();
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int b = opnum.pop();
                if (a == 0) {
                    cout << "Error";
                    return 0;
                }
                int c = b / a;
                opnum.push(c);
            }
            else if (action == "Halt") {
                return 0;
            }
        }
        else {
            string x = current_command[1];
            if (action == "Assign") {
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int a = opnum.pop();
                state.put(x,a);
            }
            else if (action == "Var") {
                int a = state.get(x);
                opnum.push(a);
            }
            else if (action == "Jmp") {
                int n = stoi(x);
                pc = n - 1;
            }
            else if (action == "JmpEq") {
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int a = opnum.pop();
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int b = opnum.pop();
                if (a == b) {
                    int n = stoi(x);
                    pc = n - 1;
                }
            }
            else if (action == "JmpGt") {
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int a = opnum.pop();
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int b = opnum.pop();
                if (a < b) {
                    int n = stoi(x);
                    pc = n - 1;
                }
            }
            else if (action == "JmpLt") {
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int a = opnum.pop();
                if (opnum.isEmpty()) {
                    cout << "Error";
                    return 0;
                }
                int b = opnum.pop();
                if (a > b) {
                    int n = stoi(x);
                    pc = n - 1;
                }
            }
            else if (action == "Const") {
                int n = stoi(x);
                opnum.push(n);
            }
            else if (action == "Print") {
                if (!state.containsKey(x)) {
                    cout << "Error";
                    return 0;
                }
                int val = state.get(x);
                cout << val << endl;
            }
        }
        pc ++;
    }
    cout << "Error";//PC out of bound.
    return 0;
}



//Check if there's an empty space in the command.
bool containEmpty (string& s) {
    for (int i = 0 ; i < s.size() ; i++) {
        if (isspace(s[i])) {
            return true;
        }
    }
    return false;
}
