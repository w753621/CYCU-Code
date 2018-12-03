#include <iostream>
#include <string>
#include <iomanip>
using namespace std;


int main() {
    int a = 1515555;
    string s = "dd ";
    cout << setw(2) << setfill('t') << hex << a;
    return 0;
}