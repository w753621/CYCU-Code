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

void BubbleSort2( vector<int> & read_number, int start, int end ) {
  for( int i = start; i + 1 < end; i++ ) {
    for( int j = start; j + 1 < end; j++ ) {
      if ( read_number[j] > read_number[j+1] ) {
        swap( read_number[j], read_number[j+1] );
      } // if

    } // for

  } // for

} // BubbleSort2()


// merge_number為要合併的前一個對象(前一個),bubble_number為當前要merge的對象(後一個)。
void MergeSort( vector<int> & read_number, int cur_pos, int left_num, int right_num ) {
  int left_start = cur_pos;
  int left_limit = left_num + cur_pos;
  int right_start = left_limit;
  int right_limit = right_num + left_limit;
  bool remainleft = false, remainright = false;
  int i = 0;
  int total_num = right_limit - left_limit;
  while ( right_start + 1 != right_limit && left_start + 1 != left_limit ) {
    if ( read_number[left_start] > read_number[right_start] ) {
      swap( read_number[left_start], read_number[right_start] );
      if ( right_start + 1 != right_limit ) {
        right_start++;
      } // if

    } // if
    else {
      if ( left_start + 1 != left_limit ) {
        left_start++;
      } // if

    } // else

  } // while

} // MergeSort()

vector<CutNumber> DealCutNumber( vector<int> read_number, int cut_number ) {
  vector<CutNumber> aCutNumber_combine;
  CutNumber aCut_number;
  int cur_cutnumber = 0, unit_number = 0, before_number = 0;
  int i;
  bool finish = false;
  unit_number  = read_number.size() / cut_number;  // 一次新增的上限。
  cur_cutnumber = cur_cutnumber + unit_number;
  for( i = 0; finish == false; cur_cutnumber = cur_cutnumber + unit_number ) {
    CutNumber pushCut_number = aCut_number;
    for( i = before_number; cur_cutnumber > i; i++ ) {
      pushCut_number.aCut_number.push_back( read_number[i] );
    } // for

    aCutNumber_combine.push_back( pushCut_number );
    before_number = i;

    if ( read_number.size() == cur_cutnumber ) finish = true;
    else if ( read_number.size() - cur_cutnumber < unit_number ) {
      unit_number = read_number.size() - cur_cutnumber;
    } // else if

  } // for

  return aCutNumber_combine;
} // DealCutNumber()

void DealResult( vector<int> sorted_number, fstream & output_file ) {
  for( int i = 0; sorted_number.size() > i; i++ ) {
    output_file << sorted_number[i];
    if ( ( i + 1 ) % 100 == 0 ) output_file << "\n";
    else output_file << " ";
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
    cout << end << " " <<start;
    double cpu_used_time = (double)( end - start ) / CLOCKS_PER_SEC; // 出來單位為秒
    // 輸出output。
    output_file.open( output_str.c_str(), ios::out);
    DealResult( sorted_number, output_file );
    output_file << "\n" << "CPU執行之時間: " << cpu_used_time << "\n";
  } // if
  else if ( command == 2 ) {  // 做thread
    int cut_number = 0;
    cout << "請輸入要切幾份: ";
    cin >> cut_number;
    vector<CutNumber> combine_number = DealCutNumber( read_number, cut_number );  // 將N個數目字切成k份
    output_file.open( output_str.c_str(), ios::out);
    int i = 0;
    clock_t start, end;
    start = clock();
    std::vector<std::thread> thread_Bubble;

    // while ( combine_number.size() > i ) {  // K個threads分別進行Bubble Sort
      // thread_Bubble.push_back( std::thread( BubbleSort, std::ref( combine_number[i].aCut_number ) ) );
      // cout << i << " ";
      // i++;
    // } // while


    end = clock();
    double cpu_used_time = (double)( end - start ) / CLOCKS_PER_SEC; // 出來單位為秒
    DealResult( read_number, output_file );
    output_file << "\n" << "CPU執行之時間: " << cpu_used_time << "\n";
  } // else if
  else if ( command == 3 ) {  // 多個process進行處理。
    int cut_number = 0;
    cout << "請輸入要切幾份: ";
    cin >> cut_number;
    vector<CutNumber> combine_number = DealCutNumber( read_number, cut_number );  // 將N個數目字切成k份
    output_file.open( output_str.c_str(), ios::out);
    int i = 0, pid = 0;
    bool unfinish = true;
    while ( combine_number.size() > i ) {  // K個process分別進行Bubble Sort

      i++;
    } // while


  } // else if
  else {    // command = 4，單一process進行處理。
    int cut_number = 0;
    clock_t start, end;
    cout << "請輸入要切幾份: ";
    cin >> cut_number;
    output_file.open( output_str.c_str(), ios::out);
    int i = 0;
    int unit = 0;
    float unit_float = read_number.size() / float( cut_number );
    if ( unit_float == (int) unit_float ) {
      unit = (int) unit_float;
    } // if
    else {
      unit = (int) unit_float + 1;
    } // else
    int end_pos = 0;
    int start_pos = 0;
    int all_num = read_number.size();
    int all_cut = cut_number;
    bool one_true = false;
    vector<int> parameter_pos;
    start = clock();
    while ( cut_number > i ) {  // 1個process分別進行Bubble Sort
      if ( i + 1 == cut_number ) {
        end_pos = end_pos + all_num;
      } // if
      else if ( all_num <= all_cut ) {
        if ( one_true == false ) {
          unit--;
          one_true = true;
          end_pos = end_pos + unit;
        } // else if
        else {
          end_pos = end_pos + unit;
        } // else

      } // else if
      else {
        end_pos = end_pos + unit;
      } // else

      parameter_pos.push_back( end_pos - start_pos );
      BubbleSort2( read_number, start_pos, end_pos );
      all_num = all_num - unit;
      all_cut--;
      start_pos = start_pos + unit;
      i++;
    } // while

    i = 0;
    MergeSort( read_number, i, parameter_pos[0], parameter_pos[1] );

    end = clock();
    double cpu_used_time = (double)( end - start ) / CLOCKS_PER_SEC; // 出來單位為秒
    DealResult( read_number, output_file );
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
