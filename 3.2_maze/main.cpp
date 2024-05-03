#include <iostream>
#include <utility>
#include "vector.h"
#include <string>
#include "set.h"
#include "queue.h"

using namespace std;

typedef pair<int,int> point;

bool searchable(point p,int& n,int& m,Set<point>& searched,Set<point>& wall);

//Determine whether walking from the given entrance(s) can reach the exit.
int main(){
    int n,m,k;//The number of the maze's rows,columns,entrances.
    cin >> n >> m >> k;
    point exit(n-1,m-1);
    Vector<point> entrance;
    for (int i = 1;i <= k;i++) {
        point p;
        cin >> p.first >> p.second;
        entrance.add(p);
    }
    Set<point> wall;
    for (int i = 0;i < n;i++) {
        string maze_row;
        cin >> maze_row;
        for (int j = 0; j < m;j++) {
            if (maze_row[j] == 'W') {
                point p(i,j);
                wall.add(p);
            }
        }
    }
    Set<point> reachable_points;//After ensuring an entrance is reachable,store its path to exit in this set.
    for (auto &x : entrance) {
        //If the given entrance is already a wall,it's obviously unreachable.
        if (wall.contains(x)) {
            cout << "unreachable" << endl;
            continue;
        }
        //If the points are checked reachable in previous cases,it's obviously unreachable.
        if (reachable_points.contains(x)) {
            cout << "reachable" << endl;
            continue;

        }
        Queue<point> to_be_searched;
        Set<point> searched;//Store the searched points.
        to_be_searched.enqueue(x);
        int flag = 0;
        while (!to_be_searched.isEmpty()) {
            point searching = to_be_searched.dequeue();
            if (searched.contains(searching)) {continue;}
            if (searching == exit || reachable_points.contains(searching)) {
            cout << "reachable" << endl;
            flag += 1;
            reachable_points += searched;
            break;
            }
            searched.add(searching);
            if (searching == exit) {
                cout << "reachable" << endl;
                flag += 1;
                reachable_points += searched;
                break;
            }
            //The adjacent points.
            point ap1(searching.first,searching.second - 1);
            point ap2(searching.first,searching.second + 1);
            point ap3(searching.first - 1,searching.second);
            point ap4(searching.first + 1,searching.second);
            if (searchable(ap1,n,m,searched,wall)) {
                to_be_searched.enqueue(ap1);
            }
            if (searchable(ap2,n,m,searched,wall)) {
                to_be_searched.enqueue(ap2);
            }
            if (searchable(ap3,n,m,searched,wall)) {
                to_be_searched.enqueue(ap3);
            }
            if (searchable(ap4,n,m,searched,wall)) {
                to_be_searched.enqueue(ap4);
            }

        }
        if (flag == 0) {
            cout << "unreachable" << endl;
        }

    }
    return 0;
}


//Determine whether the adjacent point is out of bound,searched or a wall.
bool searchable(point p,int& n,int& m,Set<point>& searched,Set<point>& wall) {
    return p.first >= 0 && p.first < n && p.second >=0 && p.second < m
            && !searched.contains(p) && !wall.contains(p);
}
