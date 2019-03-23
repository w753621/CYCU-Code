#include <iostream>
#include <string>
#include <iomanip>
#include<vector>
#include<fstream>
using namespace std;

    void test() {
        int a = 1515555;
        string s = "dd ";
        string aw =  "dw";
        s = aw;
        cout << s;
        cout << setw(2) << setfill('t') << hex << a;
    } // void

    void test2( int (&test)[5] ) {
     cout << test[0] << "\n" << test[1] ;
     test[2] = 5;

    } // test2()
int main() {
    int a = 1;
    if ( a == 1 ) cout << 22; cout << 54;
    return 0;
}