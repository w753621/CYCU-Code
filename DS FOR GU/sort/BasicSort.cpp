# include <stdio.h>
# include <stdlib.h>
# include <cstdlib> /* 亂數相關函數 */
# include <ctime>   /* 時間相關函數 */
# include <iostream>
using namespace std;
static int utestNum = -1;

void Rand( int max, int min, int n, int (&random_int)[9] ) {
  srand(time(NULL));
  for(int i = 0; i < n; i++) {
    int u = (double) rand() / (RAND_MAX + 1) * (max - min) + min;
    random_int[i] = u;
  } // for

} // Rand()

void BuildHeap( int random_int[] ) {

  cout << "BuildHeap:\n";
  for( int i = 0; i < 10; i++ ) {
    cout << random_int[i] << "\n";
  } // for()  
} // BuildHeap()

void InsertSort( int random_int[] ) {

  cout << "InsertSort:\n";
  for( int i = 0; i < 10; i++ ) {
    cout << random_int[i] << "\n";
  } // for()  
} // InsertSort()

void SelectionSort( int random_int[] ) {

  cout << "SelectionSort:\n";
  for( int i = 0; i < 10; i++ ) {
    cout << random_int[i] << "\n";
  } // for()  
} // SelectionSort



void QuickSort( int random_int[] ) {

  cout << "QuickSort:\n";
  for( int i = 0; i < 10; i++ ) {
    cout << random_int[i] << "\n";
  } // for()  
} // QuickSort()



int main() {
  int random_int[9];
  Rand( 100, 1, 10, random_int );
  for( int i = 0; i < 10; i++ ) {
    cout << random_int[i] << "\n";
  } // for()   

  InsertSort( random_int );
  // QuickSort( random_int ); 


} // main()
