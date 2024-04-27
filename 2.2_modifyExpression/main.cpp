#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <bits/stdc++.h>
using namespace std;
//Return which modified expression can get the result closest to the key.
int main()
{
    string expression;
    cin >> expression;
    int key;
    cin >> key;
    int addpos = expression.find("+");//Find where "+" is.
    string a = expression.substr(0,addpos);
    string b = expression.substr(addpos + 1);
    //Generate the result of each modified expression.
    vector<string> trick;//Store the changed expression.
    vector<int> result;//Store the result of the corresponding changed expression.
    for (int i = 0;i <= (int)a.length()-1;i++) {
        int adderA = stoi(a.substr(i));
        for (int j = 1;j <= (int)b.length();j++) {
            string expcopy = expression;
            string cexp = expcopy.insert(i,"(");
            cexp.insert((int)a.length()+2+j,")");
            trick.push_back(cexp);
            int adderB = stoi(b.substr(0,j));
            int sum = adderA + adderB;
            if (i != 0) {
                sum *= stoi(a.substr(0,i));
            }
            if (j != (int)b.length()) {
                sum *= stoi(b.substr(j));
            }
            result.push_back(sum);
        }
    }
    //Determine which result is closest to the key.
    for (int &var : result) {
        var = abs(var - key);
    }
    int m = *min_element (result.begin(), result.end());
    int index;
    for (int i = 0;i < (int)result.size();i++) {
        if (result[i] == m){
            index = i;
            break;
        }
    }
    cout << trick[index];
    return 0;
}
