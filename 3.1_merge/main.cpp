#include <iostream>
#include <string>
#include "vector.h"
#include<bits/stdc++.h>

using namespace std;

//Merge two arrays in to one in ascending order.
int main(){
    int n,m;
    cin >> n >> m;

    //Read in two arrays.
    int a;
    Vector<int> nv,mv;
    for(int i=0 ; i<n ; i++)
    {
        cin >> a;
        nv.add(a);
    }

    for(int i=0 ; i<m ; i++)
    {
        cin >> a;
        mv.add(a);
    }

    Vector<int> mergenm = nv + mv;
    mergenm.sort();

    //convert vectors to string using empty spaces as separator
    string str;
    for (auto &x : mergenm) {
        cout << x << " ";
    }
    return 0;
}
