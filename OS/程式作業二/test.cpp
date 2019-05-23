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
  int a = 90;
  char b;
  b = (char) a;
  string c = c + b;
  cout << c;
          if ( waiting_queue.size() > 0 && waiting_queue[0].priority_level > input_ProcessInfo[i+1].priority_level ) { // input_ProcessInfo is big
            is_preemptive = true;
            input_ProcessInfo[i].turnaround_time = cpu_running_time - input_ProcessInfo[i].arrival_time;
            i++;
          } // if
} // main()



//   fstream inputfile;
//  inputfile.open( "x.x" , ios::in);
 // char a;
 // inputfile >> a;
//  a = inputfile.peek();
//  cout << a;
 // cout << a;