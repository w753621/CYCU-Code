// 資工三甲 10520119 黃博泓
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <time.h>
#include <thread>
using namespace std;


struct CutNumber {
  vector<int> aCut_number;
}; // CutNumber()

void BubbleSort( vector<int> & read_number ) {

  for( int i = 0; read_number.size() > i + 1; i++ ) {
    for( int j = 0; read_number.size() > j + 1; j++ ) {
      if ( read_number[j] > read_number[j+1] ) {
        swap( read_number[j], read_number[j+1] );
      } // if

    } // for

  } // for

} // BubbleSort()

// merge_number為要合併的前一個對象(前一個),bubble_number為當前要merge的對象(後一個)。
void MergeSort( vector<int> & left, vector<int> right ) {
  // right合併to merge_sorted_number。
  int idxleft = 0, idxright = 0;
  bool notover = true;
  int total = right.size();
  // cout <<  left[0] << right[0] << "\n";
  for ( int i = 0; notover; i++ ) {
    if ( left[idxleft] < right[idxright] ) {
      if ( idxleft + 1 == left.size() ) {
        while ( idxright != right.size() ) {
          left.push_back( right[idxright] );
          idxright++;
        } // while

        notover = false;
      } // if
      else {
        idxleft++;
      } // else

    } // if
    else {

      left.insert( left.begin() + idxleft, right[idxright] );
      if ( idxright + 1 != right.size() ) {
        idxright++;
      } // if
      else {
        notover = false;
      } // else

    } // else

  } // for


} // MergeSort()

vector<CutNumber> DealCutNumber( vector<int> read_number, int cut_number ) {
  vector<CutNumber> aCutNumber_combine;
  CutNumber aCut_number;
  int i = 0;
  int unit = 0;
  int end_pos = 0;
  int start_pos = 0;
  int all_num = read_number.size();
  int all_cut = cut_number;
  bool one_true = false;
  while ( cut_number > i ) {
    float unit_float = all_num / float( all_cut );
    if ( unit_float == (int) unit_float ) {
      unit = (int) unit_float;
    } // if
    else {
      unit = (int) unit_float + 1;
    } // else


    start_pos = end_pos;
    end_pos = end_pos + unit;

    CutNumber newCut_number;
    newCut_number = aCut_number;
    int k = 0;
    int a = start_pos;
    while ( end_pos - start_pos > k ) {
      // cout << k << "\n";
      newCut_number.aCut_number.push_back( read_number[a] );
      a++;
      k++;
    } // while

    aCutNumber_combine.push_back( newCut_number );
    all_num = all_num - unit;
    all_cut--;
    i++;
  } // while

  return aCutNumber_combine;
} // DealCutNumber()

void DealResult( vector<int> sorted_number, fstream & output_file ) {
  for( int i = 0; sorted_number.size() > i; i++ ) {
    output_file << sorted_number[i];
    //if ( ( i + 1 ) % 100 == 0 ) output_file << "\n";
    output_file << " ";
  } // for()

} // DealResult()

void DealThread( int command, string output_str, fstream & input_file ) {
  vector<int> read_number;
  char white_space = '\0';
  fstream output_file;
  for( int number = 0; input_file.peek() != EOF; number = 0 ) {
    if ( input_file.peek() == '\n' || input_file.peek() == ' ' ) {
      input_file.get( white_space) ;
    } // if
    else {
      input_file >> number;
      read_number.push_back( number );
    } // else

  } // for

  if ( command == 1 ) {
    clock_t start, end;
    start = clock();
    BubbleSort( read_number );
    vector<int> sorted_number = read_number;
    end = clock();
    double cpu_used_time = (double)( end - start ) / CLOCKS_PER_SEC; // 出來單位為秒
    // 輸出output。
    output_file.open( output_str.c_str(), ios::out);
    DealResult( sorted_number, output_file );
    output_file << "\n" << "CPU執行之時間: " << cpu_used_time << "\n";
  } // if
  else if ( command == 2 ) {  // 做thread
    int cut_number = 0;
    clock_t start, end;
    cout << "請輸入要切幾份: ";
    cin >> cut_number;
    vector<CutNumber> combine_number = DealCutNumber( read_number, cut_number );  // 將N個數目字切成k份
    output_file.open( output_str.c_str(), ios::out);
    int i = 0;
    std::vector<std::thread> thread_Bubble;
    start = clock();
    while ( combine_number.size() > i ) {  // K個threads分別進行Bubble Sort
      thread_Bubble.push_back( std::thread( BubbleSort, std::ref( combine_number[i].aCut_number ) ) );
      i++;
    } // while

    for( i = 0; i < thread_Bubble.size(); i++ ) {
	    if ( thread_Bubble[i].joinable() ) {
	      thread_Bubble[i].join();
		  } // if
    } // for

    std::vector<std::thread> thread_Merge;
    int total = combine_number.size();
    int gap = 0; // gap every level
    int all_level = 0, cur_level = 0;
    bool odd = false;
    while( combine_number.size() != all_level + 1 ) {                    // 之後merge_number中，跟下一組combine
      if ( total % 2 ) {
        odd = true;
        total = total / 2;
        cur_level = total + 1;
      } // of
      else {
        odd = false;
        total = total / 2;
        cur_level = total;
      } // else

      all_level = all_level + cur_level;
      int start_pos = 0;
      if ( gap == 0 ) gap = 1;
      else gap = gap * 2;
      bool unfinish = true;
      i = 0;

      while ( cur_level > i ) {
        //cout << start_pos << " " << start_pos+gap << "z\n";
        // cout << start_pos << " " << combine_number[start_pos].aCut_number.size() << "s\n";
        thread_Merge.push_back( std::thread( MergeSort, std::ref( combine_number[start_pos].aCut_number ), combine_number[start_pos+gap].aCut_number ) );
        //cout << start_pos << " " << start_pos+gap << "t\n";
        if ( odd == true && cur_level == i + 2 ) {
          for( int k = 0; k < thread_Merge.size(); k++ ) {
	          if ( thread_Merge[k].joinable() ) {
	            thread_Merge[k].join();
		        } // if
          } // for

          thread_Merge.push_back( std::thread( MergeSort, std::ref( combine_number[start_pos].aCut_number ), combine_number[start_pos+2*gap].aCut_number ) );
          i++;
        } // if

        start_pos = start_pos + 2 * gap;
        i++;
      } // while

      for( int k = 0; k < thread_Merge.size(); k++ ) {
	      if ( thread_Merge[k].joinable() ) {
	        thread_Merge[k].join();
		    } // if
      } // for

    } // while

    //cout << combine_number.size() << "\n";
    end = clock();
    double cpu_used_time = (double)( end - start ) / CLOCKS_PER_SEC; // 出來單位為秒
    DealResult( combine_number[0].aCut_number, output_file );
    output_file << "\n" << "CPU執行之時間: " << cpu_used_time << "\n";
  } // else if
  else if ( command == 3 ) {  // 多個process進行處理。
    int cut_number = 0;
    clock_t start, end;
    cout << "請輸入要切幾份: ";
    cin >> cut_number;
    vector<CutNumber> combine_number = DealCutNumber( read_number, cut_number );  // 將N個數目字切成k份
    output_file.open( output_str.c_str(), ios::out);
    int i = 0;
    std::vector<std::thread> thread_Bubble;
    start = clock();
    while ( combine_number.size() > i ) {  // K個threads分別進行Bubble Sort
      thread_Bubble.push_back( std::thread( BubbleSort, std::ref( combine_number[i].aCut_number ) ) );
      i++;
    } // while

    for( i = 0; i < thread_Bubble.size(); i++ ) {
	    if ( thread_Bubble[i].joinable() ) {
	      thread_Bubble[i].join();
		  } // if
    } // for

    std::vector<std::thread> thread_Merge;
    int total = combine_number.size();
    int gap = 0; // gap every level
    int all_level = 0, cur_level = 0;
    bool odd = false;
    while( combine_number.size() != all_level + 1 ) {                    // 之後merge_number中，跟下一組combine
      if ( total % 2 ) {
        odd = true;
        total = total / 2;
        cur_level = total + 1;
      } // of
      else {
        odd = false;
        total = total / 2;
        cur_level = total;
      } // else

      all_level = all_level + cur_level;
      int start_pos = 0;
      if ( gap == 0 ) gap = 1;
      else gap = gap * 2;
      bool unfinish = true;
      i = 0;

      while ( cur_level > i ) {
        //cout << start_pos << " " << start_pos+gap << "z\n";
        // cout << start_pos << " " << combine_number[start_pos].aCut_number.size() << "s\n";
        thread_Merge.push_back( std::thread( MergeSort, std::ref( combine_number[start_pos].aCut_number ), combine_number[start_pos+gap].aCut_number ) );
        //cout << start_pos << " " << start_pos+gap << "t\n";
        if ( odd == true && cur_level == i + 2 ) {
          for( int k = 0; k < thread_Merge.size(); k++ ) {
	          if ( thread_Merge[k].joinable() ) {
	            thread_Merge[k].join();
		        } // if
          } // for

          thread_Merge.push_back( std::thread( MergeSort, std::ref( combine_number[start_pos].aCut_number ), combine_number[start_pos+2*gap].aCut_number ) );
          i++;
        } // if

        start_pos = start_pos + 2 * gap;
        i++;
      } // while

      for( int k = 0; k < thread_Merge.size(); k++ ) {
	      if ( thread_Merge[k].joinable() ) {
	        thread_Merge[k].join();
		    } // if
      } // for

    } // while

    //cout << combine_number.size() << "\n";
    end = clock();
    double cpu_used_time = (double)( end - start ) / CLOCKS_PER_SEC; // 出來單位為秒
    DealResult( combine_number[0].aCut_number, output_file );
    output_file << "\n" << "CPU執行之時間: " << cpu_used_time << "\n";
  } // else if
  else {    // command = 4，單一process進行處理。
    int cut_number = 0;
    clock_t start, end;
    cout << "請輸入要切幾份: ";
    cin >> cut_number;
    vector<CutNumber> combine_number = DealCutNumber( read_number, cut_number );  // 將N個數目字切成k份
    output_file.open( output_str.c_str(), ios::out);
    int i = 0;
    start = clock();
    while ( combine_number.size() > i ) {  // 1個process分別進行Bubble Sort
      BubbleSort( combine_number[i].aCut_number );
      i++;
    } // while


    int total = combine_number.size();
    int gap = 0; // gap every level
    int all_level = 0, cur_level = 0;
    bool odd = false;
    while( combine_number.size() != all_level + 1 ) {                    // 之後merge_number中，跟下一組combine
      if ( total % 2 ) {
        odd = true;
        total = total / 2;
        cur_level = total + 1;
      } // of
      else {
        odd = false;
        total = total / 2;
        cur_level = total;
      } // else

      all_level = all_level + cur_level;
      int start_pos = 0;
      if ( gap == 0 ) gap = 1;
      else gap = gap * 2;
      bool unfinish = true;
      i = 0;

      while ( cur_level > i ) {
        //cout << start_pos << " " << start_pos+gap << "z\n";
        // cout << start_pos << " " << combine_number[start_pos].aCut_number.size() << "s\n";
        MergeSort( combine_number[start_pos].aCut_number, combine_number[start_pos+gap].aCut_number );
        //cout << start_pos << " " << start_pos+gap << "t\n";
        if ( odd == true && cur_level == i + 2 ) {
          MergeSort( combine_number[start_pos].aCut_number, combine_number[start_pos+2*gap].aCut_number );
          i++;
        } // if

        start_pos = start_pos + 2 * gap;
        i++;
      } // while

    } // while

    //cout << combine_number.size() << "\n";
    end = clock();
    double cpu_used_time = (double)( end - start ) / CLOCKS_PER_SEC; // 出來單位為秒
    DealResult( combine_number[0].aCut_number, output_file );
    output_file << "\n" << "CPU執行之時間: " << cpu_used_time << "\n";
  } // else

} // DealThread()

int main() {
  fstream input_file;
  string input_str;
  cout << "Thread Programming 10520119 資工三甲 黃博泓\n"
       << "Please input the file name!!\n";
  cin >> input_str; // cin input_name
  input_file.open( input_str.c_str(), ios::in );
  if ( input_file ) {
    int command;
    input_file >> command;
    if ( command >= 1 && command <= 4 ) {
      int find_dot = input_str.find( "." );
      input_str.replace( find_dot, input_str.size() - 1, "_output.txt" );
      DealThread( command, input_str, input_file );
    } // if
    else {
      cout << "Error Command\n";
    } // else

  } // if
  else {
    cout << "Not Find File\n";
  } // else

  system("pause");
} // main()
