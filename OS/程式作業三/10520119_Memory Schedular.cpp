// 資工三甲 10520119 黃博泓
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

struct ProcessInfo {
  int process_id;
  int cpu_burst_time;
  int arrival_time;
  int priority_level;
  int waiting_time;
  int turnaround_time;
  bool is_Finished;
  bool is_Get_Used;
  int eva_time; // <= cpu_burst_time
}; // ProcessInfo

struct Waiting_PrcoessInfo {
  ProcessInfo aProcessInfo;
  int origin_position;
}; // Waiting_PrcoessInfo

string Int_To_string( int process_id ) {
  string change_string = "";
  if ( process_id < 10 ) {
    change_string = to_string( process_id );
  } // if
  else {
    process_id = process_id + 55;
    char temp = '\0';
    temp = (char) process_id;
    change_string = change_string + temp;
  } // else

  return change_string;
} // Int_To_string()

bool CheckAllProcessFinish( vector<ProcessInfo> input_ProcessInfo ) {
  int i = 0, true_number = 0;
  while ( input_ProcessInfo.size() > i ) {
    if ( input_ProcessInfo[i].is_Finished == true ) true_number++;
    i++;
  } // while

  if ( true_number == input_ProcessInfo.size() ) return true;
  return false;
} // CheckAllProcessFinish()

void Sort_Arrival_PID_Time( vector<ProcessInfo> & input_ProcessInfo ) {
  int i = 0;
  while ( input_ProcessInfo.size() > i + 1 ) {
    int k = i;
    int j = i + 1;
    while ( input_ProcessInfo.size() > j ) {
      if ( input_ProcessInfo[k].arrival_time > input_ProcessInfo[j].arrival_time ) {
        k = j;
      } // if
      else if ( input_ProcessInfo[k].arrival_time == input_ProcessInfo[j].arrival_time ) {
        if ( input_ProcessInfo[k].process_id > input_ProcessInfo[j].process_id ) {
          k = j;
        } // if

      } // else if

      j++;
    } // while

    swap( input_ProcessInfo[k], input_ProcessInfo[i] );
    i++;
  } // while

} // Sort_Arrival_PID_Time() 由小排到大排序(相同的話process_id小的優先)

void Sort_Arrival_Priority_PID_Time( vector<ProcessInfo> & input_ProcessInfo ) {
  int i = 0;
  while ( input_ProcessInfo.size() > i + 1 ) {
    int k = i;
    int j = i + 1;
    while ( input_ProcessInfo.size() > j ) {
      if ( input_ProcessInfo[k].arrival_time > input_ProcessInfo[j].arrival_time ) {
        k = j;
      } // if
      else if ( input_ProcessInfo[k].arrival_time == input_ProcessInfo[j].arrival_time ) {
        if ( input_ProcessInfo[k].priority_level > input_ProcessInfo[j].priority_level ) {
          k = j;
        } // if
        else if ( input_ProcessInfo[k].priority_level == input_ProcessInfo[j].priority_level ) {
          if ( input_ProcessInfo[k].process_id > input_ProcessInfo[j].process_id ) {
            k = j;
          } // if
        } // else if

      } // else if

      j++;
    } // while

    swap( input_ProcessInfo[k], input_ProcessInfo[i] );
    i++;
  } // while

} // Sort_Arrival_Priority_PID_Time() 由小排到大排序(相同的話Priority優先，再相同則process_id小的優先)

void Sort_Arrival_CPUTime_PID_Time( vector<ProcessInfo> & input_ProcessInfo ) {
  int i = 0;
  while ( input_ProcessInfo.size() > i + 1 ) {
    int k = i;
    int j = i + 1;
    while ( input_ProcessInfo.size() > j ) {
      if ( input_ProcessInfo[k].arrival_time > input_ProcessInfo[j].arrival_time ) {
        k = j;
      } // if
      else if ( input_ProcessInfo[k].arrival_time == input_ProcessInfo[j].arrival_time ) {
        if ( input_ProcessInfo[k].cpu_burst_time > input_ProcessInfo[j].cpu_burst_time ) {
          k = j;
        } // if
        else if ( input_ProcessInfo[k].cpu_burst_time == input_ProcessInfo[j].cpu_burst_time ) {
          if ( input_ProcessInfo[k].process_id > input_ProcessInfo[j].process_id ) {
            k = j;
          } // if

        } // else if

      } // else if

      j++;
    } // while

    swap( input_ProcessInfo[k], input_ProcessInfo[i] );
    i++;
  } // while

} // Sort_Arrival_CPUTime_PID_Time()

void InsertProcessToPriorityQueue( vector<Waiting_PrcoessInfo> & waiting_ProcessInfo, ProcessInfo aProcessInfo, int pos ) {
  // 被插入與插入。
  Waiting_PrcoessInfo aWaiting_PrcoessInfo;
  aWaiting_PrcoessInfo.aProcessInfo = aProcessInfo;
  aWaiting_PrcoessInfo.origin_position = pos;
  if ( waiting_ProcessInfo.size() == 0 ) {
    waiting_ProcessInfo.push_back( aWaiting_PrcoessInfo );
  } // if
  else {
    // 比較權重，插入在合適位置。
    int i = 0;
    while ( waiting_ProcessInfo.size() > i ) {
      ProcessInfo aWait_ProcessInfo = waiting_ProcessInfo[i].aProcessInfo;
      if ( aWait_ProcessInfo.priority_level > aProcessInfo.priority_level ) {
        waiting_ProcessInfo.insert( waiting_ProcessInfo.begin() + i, aWaiting_PrcoessInfo );
        return;
      } // if

      i++;
    } // while

    if ( i == waiting_ProcessInfo.size() ) waiting_ProcessInfo.push_back( aWaiting_PrcoessInfo );

  } // else

} // InsertProcessToPriorityQueue() weighted small to big ( if process_id is same, smaller first ) return position

void InsertProcessToRRQueue( vector<Waiting_PrcoessInfo> & waiting_ProcessInfo, ProcessInfo aProcessInfo, int pos ) {
  Waiting_PrcoessInfo aWaiting_PrcoessInfo;
  aWaiting_PrcoessInfo.aProcessInfo = aProcessInfo;
  aWaiting_PrcoessInfo.origin_position = pos;
  waiting_ProcessInfo.push_back( aWaiting_PrcoessInfo );
} // InsertProcessToRRQueue*() Queue rule: First in First out

void InsertPrcoessToSJFQueue( vector<Waiting_PrcoessInfo> & waiting_ProcessInfo, ProcessInfo aProcessInfo, int pos ) {
  Waiting_PrcoessInfo aWaiting_PrcoessInfo;
  aWaiting_PrcoessInfo.aProcessInfo = aProcessInfo;
  aWaiting_PrcoessInfo.origin_position = pos;
  int i = 0;
  while ( waiting_ProcessInfo.size() > i ) {
    ProcessInfo aWait_ProcessInfo = waiting_ProcessInfo[i].aProcessInfo;
    if ( aWait_ProcessInfo.cpu_burst_time > aProcessInfo.cpu_burst_time ) {
      waiting_ProcessInfo.insert( waiting_ProcessInfo.begin() + i, aWaiting_PrcoessInfo );
      return;
    } // if

      i++;
  } // while

  if ( i == waiting_ProcessInfo.size() ) waiting_ProcessInfo.push_back( aWaiting_PrcoessInfo );
} // InsertPrcoessToSJFQueue()

void Sort_ProcessID( vector<ProcessInfo> & input_ProcessInfo ) {
  int i = 0;
  while ( input_ProcessInfo.size() > i ) {
    int k = i;
    int j = i + 1;
    while ( input_ProcessInfo.size() > j ) {
      if ( input_ProcessInfo[k].process_id > input_ProcessInfo[j].process_id ) {
        k = j;
      } // if

      j++;
    } // while

    swap( input_ProcessInfo[k], input_ProcessInfo[i] );
    i++;
  } // while

} // Sort_ProcessID()

string EVA_FCFS( vector<ProcessInfo> & input_ProcessInfo ) {
  string ganttchart_FCFS = "";
  int i = 0;
  int cpu_running_time = 0;
  Sort_Arrival_PID_Time( input_ProcessInfo ); // 排序
  cpu_running_time = input_ProcessInfo[0].arrival_time;
  while ( input_ProcessInfo.size() > i ) { // 運算每個process的waiting、turnaround time，及ganttchart。
    string eva_string = Int_To_string( input_ProcessInfo[i].process_id );
    input_ProcessInfo[i].waiting_time = cpu_running_time - input_ProcessInfo[i].arrival_time;
    while ( input_ProcessInfo[i].cpu_burst_time > 0 ) {
      ganttchart_FCFS = ganttchart_FCFS + eva_string;
      input_ProcessInfo[i].cpu_burst_time--;
      cpu_running_time++;
    } // while

    input_ProcessInfo[i].turnaround_time = cpu_running_time - input_ProcessInfo[i].arrival_time;
    i++;
  } // while

  Sort_ProcessID( input_ProcessInfo );
  return ganttchart_FCFS;
} // EVA_FCFS()

string EVA_RR( vector<ProcessInfo> & input_ProcessInfo, int time_slice ) {
  string ganttchart_RR = "";
  int i = 0;
  int cpu_running_time = 0;
  int cur_eva_pos = i;
  Sort_Arrival_PID_Time( input_ProcessInfo ); // 排序
  vector<Waiting_PrcoessInfo> waiting_queue;  // keep in waiting_queue
  while ( !CheckAllProcessFinish( input_ProcessInfo ) ) { // 運算每個process的waiting、turnaround time，及ganttchart。
    if ( cpu_running_time < input_ProcessInfo[i].arrival_time ) cpu_running_time++;
    if ( !input_ProcessInfo[i].is_Finished && cpu_running_time >= input_ProcessInfo[i].arrival_time ) {
      string eva_string = Int_To_string( input_ProcessInfo[i].process_id );
      if ( input_ProcessInfo[i].eva_time == 0 ) input_ProcessInfo[i].eva_time = input_ProcessInfo[i].arrival_time;
      input_ProcessInfo[i].waiting_time = cpu_running_time - input_ProcessInfo[i].eva_time;
      int next_pos = cur_eva_pos + 1;
      int per_time = time_slice;
      while ( !input_ProcessInfo[i].is_Finished && per_time > 0 ) {
        ganttchart_RR = ganttchart_RR + eva_string;
        input_ProcessInfo[i].cpu_burst_time--;
        input_ProcessInfo[i].eva_time++;
        cpu_running_time++;
        per_time--;

        while ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) {
          if ( input_ProcessInfo[next_pos].eva_time == 0 ) input_ProcessInfo[next_pos].eva_time = input_ProcessInfo[next_pos].arrival_time;
          InsertProcessToRRQueue( waiting_queue, input_ProcessInfo[next_pos], next_pos );
          next_pos++;
          cur_eva_pos++;
        } // while

        if ( input_ProcessInfo[i].cpu_burst_time == 0 ) {
          input_ProcessInfo[i].turnaround_time = cpu_running_time - input_ProcessInfo[i].arrival_time;
          input_ProcessInfo[i].is_Finished = true;
        } // if
        else if ( per_time == 0 ) {
          InsertProcessToRRQueue( waiting_queue, input_ProcessInfo[i], i );
        } // else if

      } // while

      if ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) { // compare to waiting_queue
        if ( waiting_queue.size() > 0 && waiting_queue[0].aProcessInfo.eva_time < input_ProcessInfo[next_pos].arrival_time ) {
          i = waiting_queue[0].origin_position;
          waiting_queue.erase( waiting_queue.begin() + 0 );
        } // if
        else if ( waiting_queue.size() > 0 && waiting_queue[0].aProcessInfo.eva_time == input_ProcessInfo[next_pos].arrival_time ) {
          if ( waiting_queue[0].aProcessInfo.process_id < input_ProcessInfo[next_pos].process_id ) {
            i = waiting_queue[0].origin_position;
            waiting_queue.erase( waiting_queue.begin() + 0 );
          } // if
          else {
            i = next_pos;
            cur_eva_pos = next_pos;
          } // else

        } // else if
        else {
          i = next_pos;
          cur_eva_pos = next_pos;
        } // else

      } // if
      else if ( waiting_queue.size() > 0 ) { // call waiting_queue
        i = waiting_queue[0].origin_position;
        waiting_queue.erase( waiting_queue.begin() + 0 );
      } // else if
      else {
        i = next_pos;
        cur_eva_pos = next_pos;
      } // else

    } // if
    else if ( waiting_queue.size() > 0 ) {
      i = waiting_queue[0].origin_position;
      waiting_queue.erase( waiting_queue.begin() + 0 );
    } // else if

  } // while

  Sort_ProcessID( input_ProcessInfo );
  return ganttchart_RR;
} // EVA_RRI()

string EVA_PSJF( vector<ProcessInfo> & input_ProcessInfo ) {
  string ganttchart_PSJF = "";
  int i = 0;
  int cpu_running_time = 0;
  int cur_eva_pos = i;
  Sort_Arrival_CPUTime_PID_Time( input_ProcessInfo ); // 排序
  vector<Waiting_PrcoessInfo> waiting_queue;  // keep in waiting_queue
  while ( !CheckAllProcessFinish( input_ProcessInfo ) ) { // 運算每個process的waiting、turnaround time，及ganttchart。
    bool is_preemptive = false;
    if ( cpu_running_time < input_ProcessInfo[i].arrival_time ) cpu_running_time++;
    if ( !input_ProcessInfo[i].is_Finished && cpu_running_time >= input_ProcessInfo[i].arrival_time ) {
      string eva_string = Int_To_string( input_ProcessInfo[i].process_id );
      if ( input_ProcessInfo[i].eva_time == 0 ) input_ProcessInfo[i].eva_time = input_ProcessInfo[i].arrival_time;
      input_ProcessInfo[i].waiting_time = cpu_running_time - input_ProcessInfo[i].eva_time;
      int next_pos = cur_eva_pos + 1;
      while ( !input_ProcessInfo[i].is_Finished && !is_preemptive ) {
        ganttchart_PSJF = ganttchart_PSJF + eva_string;
        input_ProcessInfo[i].cpu_burst_time--;
        input_ProcessInfo[i].eva_time++;
        cpu_running_time++;
        if ( input_ProcessInfo[i].cpu_burst_time == 0 ) {
          input_ProcessInfo[i].turnaround_time = cpu_running_time - input_ProcessInfo[i].arrival_time;
          input_ProcessInfo[i].is_Finished = true;
        } // if

        if ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) {
          if ( input_ProcessInfo[i].cpu_burst_time > input_ProcessInfo[next_pos].cpu_burst_time && !input_ProcessInfo[i].is_Finished ) {
            InsertPrcoessToSJFQueue( waiting_queue, input_ProcessInfo[i], i );
            i = next_pos;
            cur_eva_pos = next_pos;
            is_preemptive = true;
          } // if
          else if ( input_ProcessInfo[i].cpu_burst_time == input_ProcessInfo[next_pos].cpu_burst_time && !input_ProcessInfo[i].is_Finished ) {
            if ( input_ProcessInfo[i].arrival_time > input_ProcessInfo[next_pos].arrival_time ) {
              InsertPrcoessToSJFQueue( waiting_queue, input_ProcessInfo[i], i );
              i = next_pos;
              cur_eva_pos = next_pos;
              is_preemptive = true;
            } // if
            else if ( input_ProcessInfo[i].arrival_time == input_ProcessInfo[next_pos].arrival_time ) {
              if ( input_ProcessInfo[i].process_id > input_ProcessInfo[next_pos].process_id ) {
                InsertPrcoessToSJFQueue( waiting_queue, input_ProcessInfo[i], i );
                i = next_pos;
                cur_eva_pos = next_pos;
                is_preemptive = true;
              } // if

            } // else if

          } // else if
          else {
            while ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) {
              if ( input_ProcessInfo[next_pos].eva_time == 0 ) input_ProcessInfo[next_pos].eva_time = input_ProcessInfo[next_pos].arrival_time;
              InsertPrcoessToSJFQueue( waiting_queue, input_ProcessInfo[next_pos], next_pos );
              next_pos++;
              cur_eva_pos++;
            } // while

          } // else

        } // if
        else if ( waiting_queue.size() > 0  ) {
          if ( waiting_queue[0].aProcessInfo.cpu_burst_time < input_ProcessInfo[i].cpu_burst_time ) {
            InsertPrcoessToSJFQueue( waiting_queue, input_ProcessInfo[i], i );
            i = waiting_queue[0].origin_position;
            waiting_queue.erase( waiting_queue.begin() + 0 );
            is_preemptive = true;
          } // if
          else if ( waiting_queue[0].aProcessInfo.cpu_burst_time == input_ProcessInfo[i].cpu_burst_time ) {
            if ( waiting_queue[0].aProcessInfo.arrival_time < input_ProcessInfo[i].arrival_time ) {
              InsertPrcoessToSJFQueue( waiting_queue, input_ProcessInfo[i], i );
              i = waiting_queue[0].origin_position;
              waiting_queue.erase( waiting_queue.begin() + 0 );
              is_preemptive = true;
            } // if
            else if ( waiting_queue[0].aProcessInfo.arrival_time == input_ProcessInfo[i].arrival_time ) {
              if ( waiting_queue[0].aProcessInfo.process_id < input_ProcessInfo[i].process_id ) {
                InsertPrcoessToSJFQueue( waiting_queue, input_ProcessInfo[i], i );
                i = waiting_queue[0].origin_position;
                waiting_queue.erase( waiting_queue.begin() + 0 );
                is_preemptive = true;
              } // if

            } // else if

          } // else if

        } // else if

      } // while

      if ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time && !is_preemptive ) { // compare to waiting_queue
        if ( waiting_queue.size() > 0 && waiting_queue[0].aProcessInfo.cpu_burst_time < input_ProcessInfo[next_pos].cpu_burst_time ) {
          i = waiting_queue[0].origin_position;
          waiting_queue.erase( waiting_queue.begin() + 0 );
        } // if
        else if ( waiting_queue.size() > 0 && waiting_queue[0].aProcessInfo.cpu_burst_time == input_ProcessInfo[next_pos].cpu_burst_time ) {
          if ( waiting_queue[0].aProcessInfo.process_id < input_ProcessInfo[next_pos].process_id ) {
            i = waiting_queue[0].origin_position;
            waiting_queue.erase( waiting_queue.begin() + 0 );
          } // if
          else {
            i = next_pos;
            cur_eva_pos = next_pos;
          } // else

        } // else if
        else {
          i = next_pos;
          cur_eva_pos = next_pos;
        } // else

      } // if
      else if ( waiting_queue.size() > 0 && !is_preemptive ) { // call waiting_queue
        i = waiting_queue[0].origin_position;
        waiting_queue.erase( waiting_queue.begin() + 0 );
      } // else if
      else if ( !is_preemptive ) {
        i = next_pos;
        cur_eva_pos = next_pos;
      } // else if

    } // if
    else if ( waiting_queue.size() > 0  ) {
      i = waiting_queue[0].origin_position;
      waiting_queue.erase( waiting_queue.begin() + 0 );
    } // else if

  } // while

  Sort_ProcessID( input_ProcessInfo );
  return ganttchart_PSJF;
} // EVA_PSJF()

string EVA_Non_PSJF( vector<ProcessInfo> & input_ProcessInfo ) {
  string ganttchart_NPSJF = "";
  int i = 0;
  int cpu_running_time = 0;
  int cur_eva_pos = i;
  Sort_Arrival_CPUTime_PID_Time( input_ProcessInfo ); // 排序
  vector<Waiting_PrcoessInfo> waiting_queue;  // keep in waiting_queue
  while ( !CheckAllProcessFinish( input_ProcessInfo ) ) { // 運算每個process的waiting、turnaround time，及ganttchart。
    if ( cpu_running_time < input_ProcessInfo[i].arrival_time ) cpu_running_time++;
    if ( !input_ProcessInfo[i].is_Finished && cpu_running_time >= input_ProcessInfo[i].arrival_time ) {
      string eva_string = Int_To_string( input_ProcessInfo[i].process_id );
      if ( input_ProcessInfo[i].eva_time == 0 ) input_ProcessInfo[i].eva_time = input_ProcessInfo[i].arrival_time;
      input_ProcessInfo[i].waiting_time = cpu_running_time - input_ProcessInfo[i].eva_time;
      int next_pos = cur_eva_pos + 1;
      while ( !input_ProcessInfo[i].is_Finished ) {
        ganttchart_NPSJF = ganttchart_NPSJF + eva_string;
        input_ProcessInfo[i].cpu_burst_time--;
        input_ProcessInfo[i].eva_time++;
        cpu_running_time++;

        while ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) {
          if ( input_ProcessInfo[next_pos].eva_time == 0 ) input_ProcessInfo[next_pos].eva_time = input_ProcessInfo[next_pos].arrival_time;
          InsertPrcoessToSJFQueue( waiting_queue, input_ProcessInfo[next_pos], next_pos );
          next_pos++;
          cur_eva_pos++;
        } // while

        if ( input_ProcessInfo[i].cpu_burst_time == 0 ) {
          input_ProcessInfo[i].turnaround_time = cpu_running_time - input_ProcessInfo[i].arrival_time;
          input_ProcessInfo[i].is_Finished = true;
        } // if

      } // while

      if ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) { // compare to waiting_queue
        if ( waiting_queue.size() > 0 && waiting_queue[0].aProcessInfo.cpu_burst_time < input_ProcessInfo[next_pos].cpu_burst_time ) {
          i = waiting_queue[0].origin_position;
          waiting_queue.erase( waiting_queue.begin() + 0 );
        } // if
        else if ( waiting_queue.size() > 0 && waiting_queue[0].aProcessInfo.cpu_burst_time == input_ProcessInfo[next_pos].cpu_burst_time ) {
          if ( waiting_queue[0].aProcessInfo.process_id < input_ProcessInfo[next_pos].process_id ) {
            i = waiting_queue[0].origin_position;
            waiting_queue.erase( waiting_queue.begin() + 0 );
          } // if
          else {
            i = next_pos;
            cur_eva_pos = next_pos;
          } // else

        } // else if
        else {
          i = next_pos;
          cur_eva_pos = next_pos;
        } // else

      } // if
      else if ( waiting_queue.size() > 0 ) { // call waiting_queue
        i = waiting_queue[0].origin_position;
        waiting_queue.erase( waiting_queue.begin() + 0 );
      } // else if
      else {
        i = next_pos;
        cur_eva_pos = next_pos;
      } // else

    } // if
    else if ( waiting_queue.size() > 0 ) {
      i = waiting_queue[0].origin_position;
      waiting_queue.erase( waiting_queue.begin() + 0 );
    } // else if

  } // while

  Sort_ProcessID( input_ProcessInfo );
  return ganttchart_NPSJF;
} // EVA_Non_PSJF()

string EVA_Priority( vector<ProcessInfo> & input_ProcessInfo ) {
  string ganttchart_PP = "";
  int i = 0;
  int cpu_running_time = 0;
  int cur_eva_pos = i;
  Sort_Arrival_Priority_PID_Time( input_ProcessInfo ); // 排序
  vector<Waiting_PrcoessInfo> waiting_queue;  // keep in waiting_queue
  while ( !CheckAllProcessFinish( input_ProcessInfo ) ) { // 運算每個process的waiting、turnaround time，及ganttchart。
    if ( cpu_running_time == 0 ) cpu_running_time++;
    if ( input_ProcessInfo.size() > cur_eva_pos && cpu_running_time >= input_ProcessInfo[i].arrival_time ) {
      string eva_string = Int_To_string( input_ProcessInfo[i].process_id );
      if ( input_ProcessInfo[i].eva_time == 0 ) input_ProcessInfo[i].eva_time = input_ProcessInfo[i].arrival_time;
      input_ProcessInfo[i].waiting_time = cpu_running_time - input_ProcessInfo[i].eva_time;
      bool is_preemptive = false;
      int next_pos = cur_eva_pos + 1;
      while ( !input_ProcessInfo[i].is_Finished && !is_preemptive ) {
        ganttchart_PP = ganttchart_PP + eva_string;
        input_ProcessInfo[i].cpu_burst_time--;
        input_ProcessInfo[i].eva_time++;
        cpu_running_time++;
        if ( input_ProcessInfo[i].cpu_burst_time == 0 ) {
          input_ProcessInfo[i].turnaround_time = cpu_running_time - input_ProcessInfo[i].arrival_time;
          input_ProcessInfo[i].is_Finished = true;
          break;
        } // if

        // 如下一個到達時間到了，比較priority就好(一樣再比較process id)。 A:3 B:1
        if ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time
             && input_ProcessInfo[i].priority_level > input_ProcessInfo[next_pos].priority_level ) {

          InsertProcessToPriorityQueue( waiting_queue, input_ProcessInfo[i], i );
          is_preemptive = true;
        } // if
        else if ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time
                  && input_ProcessInfo[i].priority_level == input_ProcessInfo[next_pos].priority_level ) { // A:3 B:3

          if ( input_ProcessInfo[i].process_id > input_ProcessInfo[next_pos].process_id ) {
            InsertProcessToPriorityQueue( waiting_queue, input_ProcessInfo[i], i );
            is_preemptive = true;
          } // if

        } // else if
        else if ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) {
          while ( cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) {
            InsertProcessToPriorityQueue( waiting_queue, input_ProcessInfo[next_pos], next_pos );
            next_pos++;
            cur_eva_pos++;
          } // while

        } // else if

      } // while

      if ( input_ProcessInfo[i].is_Finished ) { // think about waiting_queue and next position
        input_ProcessInfo[i].is_Get_Used = true;
        if ( input_ProcessInfo.size() > next_pos && cpu_running_time >= input_ProcessInfo[next_pos].arrival_time ) { // compare to waiting_queue
          if ( waiting_queue.size() > 0 && waiting_queue[0].aProcessInfo.priority_level < input_ProcessInfo[next_pos].priority_level ) {

            i = waiting_queue[0].origin_position;
            waiting_queue.erase( waiting_queue.begin() + 0 );
          } // if
          else if ( waiting_queue.size() > 0 && waiting_queue[0].aProcessInfo.priority_level == input_ProcessInfo[next_pos].priority_level ) {
            if ( waiting_queue[0].aProcessInfo.process_id < input_ProcessInfo[next_pos].process_id ) {

              i = waiting_queue[0].origin_position;
              waiting_queue.erase( waiting_queue.begin() + 0 );
            } // if
            else {
              i = next_pos;
              cur_eva_pos = next_pos;
            } // else

          } // else if
          else {
            i = next_pos;
            cur_eva_pos = next_pos;
          } // else

        } // if
        else if ( waiting_queue.size() > 0 ) { // call waiting_queue
          i = waiting_queue[0].origin_position;
          waiting_queue.erase( waiting_queue.begin() + 0 );
        } // else if
        else {
          i = next_pos;
          cur_eva_pos = next_pos;
        } // else

      } // if
      else {
        input_ProcessInfo[i].is_Get_Used = true;
        i = next_pos;
        cur_eva_pos = next_pos;
      } // else

    } // if
    else if ( waiting_queue.size() > 0 ) {
      i = waiting_queue[0].origin_position;
      waiting_queue.erase( waiting_queue.begin() + 0 );
    } // else if

  } // while

  Sort_ProcessID( input_ProcessInfo );
  return ganttchart_PP;
} // EVA_Priority()

void PrintProcess_GanttChart( int command, vector<string> ganttchart_string, fstream & output_file ) {
  if ( ganttchart_string.size() == 1 ) {
    if ( command == 1 ) {
      output_file << "==  FCFS (First Come First Serve) ==\n";
    } // if
    else if ( command == 2 ) {
      output_file << "== RR (Round Robin) ==\n";
    } // else if
    else if ( command == 3 ) {
      output_file << "== PSJF (Preemptive Shortest Job First) ==\n";
    } // else if
    else if ( command == 4 ) {
      output_file << "== NSJF (Non-preemptive Shortest Job First) ==\n";
    } // else if
    else if ( command == 5 ) {
      output_file << "== PP (Preemptive Priority) ==\n";
    } // else if

    output_file << ganttchart_string[0] << "\n";
  } // if
  else {
    int type_int = 0;
    while ( ganttchart_string.size() > type_int ) {
      if ( type_int == 0 ) {
        output_file << "==  FCFS (First Come First Serve) ==\n";
      } // if
      else if ( type_int == 1 ) {
        output_file << "== RR (Round Robin) ==\n";
      } // else if
      else if ( type_int == 2 ) {
        output_file << "== PSJF (Preemptive Shortest Job First) ==\n";
      } // else if
      else if ( type_int == 3 ) {
        output_file << "== NSJF (Non-preemptive Shortest Job First) ==\n";
      } // else if
      else if ( type_int == 4 ) {
        output_file << "== PP (Preemptive Priority) ==\n";
      } // else if

      output_file << ganttchart_string[type_int] << "\n";
      type_int++;
    } // while

  } // else

} // PrintProcess_GanttChart()

void DealProcessType( int command, string output_str, fstream & input_file ) {
  int time_slice = 0, number = 1;
  int i = 0;
  input_file >> time_slice;
  input_file.ignore();
  string name_line;
  getline( input_file, name_line );
  vector<ProcessInfo> input_ProcessInfo;
  ProcessInfo newProcessInfo;
  newProcessInfo.waiting_time = 0;
  newProcessInfo.turnaround_time = 0;
  newProcessInfo.is_Finished = false;
  newProcessInfo.is_Get_Used = false;
  newProcessInfo.eva_time = 0;
  char white_space = '\0';
  while ( input_file.peek() != EOF ) {
    if ( input_file.peek() == '\n' || input_file.peek() == ' ' ||  input_file.peek() == '\t' ) {
      input_file.get( white_space) ;
    } // if
    else if ( number == 1 ) {
      input_file >> newProcessInfo.process_id;
      number++;
    } // else if
    else if ( number == 2 ) {
      input_file >> newProcessInfo.cpu_burst_time;
      number++;
    } // else if
    else if ( number == 3 ) {
      input_file >> newProcessInfo.arrival_time;
      number++;
    } // else if
    else if ( number == 4 ) {
      input_file >> newProcessInfo.priority_level;
      input_ProcessInfo.push_back( newProcessInfo );
      number = 1;
    } // else if

  } // while

  fstream output_file;
  output_file.open( output_str.c_str(), ios::out);
  string ganttchart_FCFS = "";
  string ganttchart_RR = "";
  string ganttchart_PSJF = "";
  string ganttchart_NON_PSJF = "";
  string ganttchart_Priority = "";
  vector<ProcessInfo> aFCFS_ProcessInfo;
  vector<ProcessInfo> aRR_ProcessInfo;
  vector<ProcessInfo> aPSJF_ProcessInfo;
  vector<ProcessInfo> aNPSJF_ProcessInfo;
  vector<ProcessInfo> aPriority_ProcessInfo;
  vector<string> ganttchart_string;

  if ( command == 1 ) {
    aFCFS_ProcessInfo = input_ProcessInfo;
    ganttchart_FCFS = EVA_FCFS( input_ProcessInfo );
    ganttchart_string.push_back( ganttchart_FCFS );
    PrintProcess_GanttChart( command, ganttchart_string, output_file );
    output_file << "===========================================================\n";
    output_file << "\n" << "Waiting Time\n";
    output_file << "ID\t" << "FCFS\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].waiting_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
    i = 0;
    output_file << "\n" << "Turnaround Time" << "\n";
    output_file << "ID\t" << "FCFS\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].turnaround_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
  } // if
  else if ( command == 2 ) {
    aRR_ProcessInfo = input_ProcessInfo;
    ganttchart_Priority = EVA_RR( input_ProcessInfo, time_slice );
    ganttchart_string.push_back( ganttchart_Priority );
    PrintProcess_GanttChart( command, ganttchart_string, output_file );
    output_file << "===========================================================\n";
    output_file << "\n" << "Waiting Time\n";
    output_file << "ID\t" << "RR\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].waiting_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
    i = 0;
    output_file << "\n" << "Turnaround Time" << "\n";
    output_file << "ID\t" << "RR\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].turnaround_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
  } // else if
  else if ( command == 3 ) {
    aPSJF_ProcessInfo = input_ProcessInfo;
    ganttchart_Priority = EVA_PSJF( input_ProcessInfo );
    ganttchart_string.push_back( ganttchart_Priority );
    PrintProcess_GanttChart( command, ganttchart_string, output_file );
    output_file << "===========================================================\n";
    output_file << "\n" << "Waiting Time\n";
    output_file << "ID\t" << "PSJF\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].waiting_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
    i = 0;
    output_file << "\n" << "Turnaround Time" << "\n";
    output_file << "ID\t" << "PSJF\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].turnaround_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
  } // else if
  else if ( command == 4 ) {
    aNPSJF_ProcessInfo = input_ProcessInfo;
    ganttchart_Priority = EVA_Non_PSJF( input_ProcessInfo );
    ganttchart_string.push_back( ganttchart_Priority );
    PrintProcess_GanttChart( command, ganttchart_string, output_file );
    output_file << "===========================================================\n";
    output_file << "\n" << "Waiting Time\n";
    output_file << "ID\t" << "NPSJF\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].waiting_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
    i = 0;
    output_file << "\n" << "Turnaround Time" << "\n";
    output_file << "ID\t" << "NPSJF\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].turnaround_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
  } // else if
  else if ( command == 5 ) {
    aPriority_ProcessInfo = input_ProcessInfo;
    ganttchart_Priority = EVA_Priority( input_ProcessInfo );
    ganttchart_string.push_back( ganttchart_Priority );
    PrintProcess_GanttChart( command, ganttchart_string, output_file );
    output_file << "===========================================================\n";
    output_file << "\n" << "Waiting Time\n";
    output_file << "ID\t" << "Priority\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].waiting_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
    i = 0;
    output_file << "\n" << "Turnaround Time" << "\n";
    output_file << "ID\t" << "Priority\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << input_ProcessInfo[i].process_id << "\t" << input_ProcessInfo[i].turnaround_time << "\n";
      i++;
    } // while

    output_file << "===========================================================\n";
  } // else if
  else if ( command == 6 ) {
    aFCFS_ProcessInfo = input_ProcessInfo;
    aRR_ProcessInfo = input_ProcessInfo;
    aPSJF_ProcessInfo = input_ProcessInfo;
    aNPSJF_ProcessInfo = input_ProcessInfo;
    aPriority_ProcessInfo = input_ProcessInfo;
    ganttchart_FCFS = EVA_FCFS( aFCFS_ProcessInfo );
    ganttchart_string.push_back( ganttchart_FCFS );
    ganttchart_RR = EVA_RR( aRR_ProcessInfo, time_slice );
    ganttchart_string.push_back( ganttchart_RR );
    ganttchart_PSJF = EVA_PSJF( aPSJF_ProcessInfo );
    ganttchart_string.push_back( ganttchart_PSJF );
    ganttchart_NON_PSJF = EVA_Non_PSJF( aNPSJF_ProcessInfo );
    ganttchart_string.push_back( ganttchart_NON_PSJF );
    ganttchart_Priority = EVA_Priority( aPriority_ProcessInfo );
    ganttchart_string.push_back( ganttchart_Priority );
    PrintProcess_GanttChart( command, ganttchart_string, output_file );

    output_file << "\n" << "Waiting Time\n";
    output_file << "ID\t" << "FCFS\t" << "RR\t" << "PSJF\t" << "NPSJF\t" << "Priority\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << aFCFS_ProcessInfo[i].process_id << "\t" << aFCFS_ProcessInfo[i].waiting_time << "\t"
                  << aRR_ProcessInfo[i].waiting_time << "\t" << aPSJF_ProcessInfo[i].waiting_time << "\t"
                  << aNPSJF_ProcessInfo[i].waiting_time << "\t" << aPriority_ProcessInfo[i].waiting_time << "\n";
      i++;
    } // while

    i = 0;
    output_file << "\n" << "Turnaround Time" << "\n";
    output_file << "ID\t" << "FCFS\t" << "RR\t" << "PSJF\t" << "NPSJF\t" << "Priority\n";
    output_file << "===========================================================\n";
    while ( input_ProcessInfo.size() > i ) {
      output_file << aFCFS_ProcessInfo[i].process_id << "\t" << aFCFS_ProcessInfo[i].turnaround_time << "\t"
                  << aRR_ProcessInfo[i].turnaround_time << "\t" << aPSJF_ProcessInfo[i].turnaround_time << "\t"
                  << aNPSJF_ProcessInfo[i].turnaround_time << "\t" << aPriority_ProcessInfo[i].turnaround_time << "\n";
      i++;
    } // while


  } // else if


} // DealProcessType()

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
    if ( command >= 1 && command <= 6 ) {
      int find_dot = input_str.find( "." );
      input_str.replace( find_dot, input_str.size() - 1, "output_f.txt" );
      DealProcessType( command, input_str, input_file );
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
