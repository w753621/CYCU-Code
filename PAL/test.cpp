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

int main() {
  fstream inputfile;
  TokenData aTokenData; 
  vector<TokenData> mTranslated_token_vector;
  string printed = "";
  if ( printed[2] == 'f' ) cout << "r";
  int number = printed[2];
  cout << number << "e";
} // main()



//   fstream inputfile;
//  inputfile.open( "x.x" , ios::in);
 // char a;
 // inputfile >> a;
//  a = inputfile.peek();
//  cout << a;
 // cout << a;