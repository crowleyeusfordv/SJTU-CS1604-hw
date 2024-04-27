#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
using namespace std;
int getIndex(char);
vector<int> numGenerator(string);
//26-base multiplication of str1¡Ástr2.
int main()
{
    string str1;
    string str2;
    cin >> str1;
    cin >> str2;
    vector<int> num1 = numGenerator(str1);
    vector<int> num2 = numGenerator(str2);
    vector<int> numresult(str1.size() + str2.size(),0);//CORE!!!
    for (int j = 0;j < (int)str2.size();j++) {
        cout << j << endl;
        for (int i = 0;i < (int)str1.size();i++) {
            cout << i << endl;
            int posnum = num2[j] * num1[i];
            int sum = posnum % 26;
            int carry = posnum / 26;
            numresult[j + i] += sum;
            numresult[j + i + 1] += carry;
        }
    }
    const vector<char> chardigits = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
                                    'n','o','p','q','r','s','t','u','v','w','x','y','z'};
    //Delete invalid 0s.
    int p = numresult.size() - 1;
    while (numresult[p] == 0) {
        numresult.pop_back();
    }

    //Convert numbers to characters.
    string result;
    for (int i = numresult.size() - 1;i >= 0;i--) {
        char alphadigit = chardigits[i];
        result+=alphadigit;
    }
    cout << result;
    return 0;
}

// Convert characters to digits.
int getIndex(char digit)
{
    const vector<char> chardigits = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
                                      'n','o','p','q','r','s','t','u','v','w','x','y','z'};
    auto it = find(chardigits.begin(), chardigits.end(), digit);
    int index = it - chardigits.begin();
    return index;
}


//Reverse strings and convert to vector<int>
vector<int> numGenerator(string str){
    vector<int> num;
    for (int i = str.size() - 1;i >= 0;i--){
        num.push_back(getIndex(str[i]));
    }
    return num;
}
