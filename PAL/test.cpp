#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <stack>
#include <vector>
using namespace std;

struct TokenData {                                            // token¦r¦ê¡BtokenªºÃþ§O¡C
  int token_line;
  int token_column;
  string token_name;
  string token_type;
}; // TokenData

struct TreeNode {
  TokenData token_data;
  bool isStart;
  bool isEnd;
  TreeNode * left;
  TreeNode * right;
}; // TreeNode

class Pp {
  public:
  int a;
  Pp() {
    a = 1;
  } // Pp
};

void test() {
  try {
    string a = "fff";
    throw a;
  } // try
  catch( string a ) {
   cout << "enter";
  } // catch

} // test()

int main() {
  fstream inputfile;
  try {
    test();
    cout << "ff1";
  } //
  catch( int i ) {

  } // catch

} // main()



//   fstream inputfile;
//  inputfile.open( "x.x" , ios::in);
 // char a;
 // inputfile >> a;
//  a = inputfile.peek();
//  cout << a;
 // cout << a;