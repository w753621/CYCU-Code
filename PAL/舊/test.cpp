#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <stack>
#include <vector>
using namespace std;

int g = 1;
struct TokenData {                                            // token¦r¦ê¡BtokenªºÃþ§O¡C
  int token_line;
  int token_column;
  string token_name;
  string token_type;
}; // TokenData

struct TreeNode {
  int a;
  TokenData token_data;
  bool isStart;
  bool isEnd;
  TreeNode * left;
  TreeNode * right;
}; // TreeNode

class Pp {
  private:
    TreeNode * b;
  public:
    int c;

  Pp( int b ) {
    cout << c <<"\n";
    c = 1;
  } // pp

};


void test() {

} // test()

int main() {
  int * a = NULL;
  a = new int;
  *a = 7;
  a = new int;
  *a = 5;
  cout << *a << "\n";
  cout << "nnnb";
} // main()



//   fstream inputfile;
//  inputfile.open( "x.x" , ios::in);
 // char a;
 // inputfile >> a;
//  a = inputfile.peek();
//  cout << a;
 // cout << a;