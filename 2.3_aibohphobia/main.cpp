#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include <bits/stdc++.h>
#include <iterator>
#include <vector>
using namespace std;
bool isPalindrome(string);

/*Determine whether a sentence is a palindrome.
If so,print "No!",else delete the palindrome word in the sentence if there are any
and print the modified sentence.*/
int main()
{
    string sentence;
    getline(cin,sentence);
    string sentcopy = sentence;
    //Delete all empty spaces.
    sentcopy.erase(remove_if(sentcopy.begin(),
                              sentcopy.end(),
                              [](unsigned char x) { return isspace(x); }),
               sentcopy.end());

    if (isPalindrome(sentcopy)){
        cout << "No!";
        return 0;
    }
    //Delete palindrome words.
    stringstream ss(sentence);
    vector<string> v;
    while (getline(ss, sentence, ' ')) {
        v.push_back(sentence);
    }
    //Generate the modified string not containing palindrome words.
    string newsent = "";
    for (auto& x : v){
        if (!isPalindrome(x)){
            newsent += x;
            newsent += " ";

        }
    }
    newsent.pop_back();//Delete the last " ".
    cout << newsent;

    return 0;
}

//Determine whether the string is palindrome.
bool isPalindrome(string str){
    string cstr = str;
    reverse(cstr.begin(),cstr.end());
    //Convert to lowercases.
    for (auto& x : str) {
        x = tolower(x);
    }
    for (auto& x : cstr) {
        x = tolower(x);
    }
    return (cstr == str);
}
