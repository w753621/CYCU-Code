#include<stdio.h>
#include<iostream>
#include<fstream>
#include<time.h>
#include<vector>
#include <pthread.h>
#include<process.h>
#include <unistd.h>


using namespace std;
typedef vector<int> Vint ;

struct bubble {
	vector<int> list ;
	int head ;
	int num ;
};

struct merge {
	bubble list1 ;
	bubble list2 ;
};

unsigned long start ;

void BubbleSort( vector<int> &input, int head, int num ) {
	int num2, temp ;
	for( int i = 0 ; i < num-1 ; i++ ) {
		num2 = num - i + head ;
		for( int k = head ; k < num2-1 ; k++ ) {
			if ( input[k] > input[k+1] ) {
				temp = input[k] ;
				input[k] = input[k+1] ;
				input[k+1] = temp ;
			} // if

		} // for

	} // for

} // BubbleSort()

void * BubbleSort2( void * ptr2 ) {
	bubble * ptr = (bubble * )ptr2;
	int num2, temp ;
	for( int i = 0 ; i < ptr->num-1 ; i++ ) {
		num2 = ptr->num - i ;
		for( int k = 0 ; k < num2-1 ; k++ ) {
			if ( ptr->list[k] > ptr->list[k+1] ) {
				temp = ptr->list[k] ;
				ptr->list[k] = ptr->list[k+1] ;
				ptr->list[k+1] = temp ;
			} // if

		} // for

	} // for
	pthread_exit(NULL);
} // BubbleSort()

void * MergeSort2( void * ptr2  ) {
	merge * ptr = (merge * )ptr2 ;
	int num = ptr->list1.list.size()-1 ;
	while ( !ptr->list2.list.empty()  ) {
		if( ptr->list2.list[ptr->list2.list.size()-1] > ptr->list1.list[num] ) {
			ptr->list1.list.insert(ptr->list1.list.begin()+num+1, ptr->list2.list[ptr->list2.list.size()-1] ) ;
			ptr->list2.list.pop_back() ;
		} // if
		else if ( num != 0 ) num-- ;
		else {
			ptr->list1.list.insert(ptr->list1.list.begin()+num, ptr->list2.list[ptr->list2.list.size()-1] ) ;
			ptr->list2.list.pop_back() ;
		} // else

	} // while

	pthread_exit(NULL);
} // MergeSort()

void MergeSort( vector<int> &input1, vector<int> input2  ) {
	int num = input1.size()-1 ;
	while ( !input2.empty()  ) {
		if( input2[input2.size()-1] > input1[num] ) {
			input1.insert(input1.begin()+num+1, input2[input2.size()-1] ) ;
			input2.pop_back() ;
		} // if
		else if ( num != 0 ) num-- ;
		else {
			input1.insert( input1.begin()+num, input2[input2.size()-1] ) ;
			input2.pop_back() ;
		} // else

	} // while

} // MergeSort()


void Print( vector<int> input ) {
	fstream fout ;
	fout.open( "output.txt", ios::out ) ;
	for( int num = 0 ; num < input.size() ; num++ ) {
		fout << input[num] << ' ' ;
	} // for

	fout.close() ;

} // Print()


int main() {
	int mission = 9, i = 0, k, num, head = 0 ;   //  num =每1份的的量
	string inputName ;
	inputName.clear() ;
	vector<int> dataStruct ;
	dataStruct.clear() ;
	fstream fin ;
    cout << "請輸入檔名：" ;
	cin >> inputName ;
	fin.open( inputName.c_str(), ios::in ) ;
	fin >> mission ;
	while ( fin >> i ) dataStruct.push_back(i) ;
	if ( mission == 1 ) {
		start = clock() ;
		BubbleSort( dataStruct, 0, dataStruct.size() ) ;
		Print( dataStruct ) ;
	} // if
	else if ( mission == 2 || mission == 3 || mission == 4 ) {
		cout << "請輸入要切成幾份：" ;
		cin >> k ;
		start = clock() ;
		num = dataStruct.size() / k ;
		if ( dataStruct.size() % k != 0 ) num++ ;
		pthread_t a[k] ;
		bubble ptr2[k] ;
		merge ptr3[k-1] ;
		vector<Vint> temp ;
		temp.clear() ;
		if ( mission == 2 ) {
			for( int s = 0, d = 0 ; s < k ; s++ ) {  //  d是每一分起點的index
				ptr2[s].head = d ;
				if( d+num <= dataStruct.size() ) ptr2[s].num = num ;
				else ptr2[s].num = dataStruct.size() - d ;
				for( int q = d ; q < d+num ; q++ ) ptr2[s].list.push_back( dataStruct[q] ) ;
				d = d+num ;
			} // for

			for( int num2 = 0 ; num2 < k ; num2++ ) pthread_create( &a[num2], NULL, BubbleSort2,(void*) &ptr2[num2] ) ;
			for( int num2 = 0 ; num2 < k ; num2++ ) pthread_join(a[num2], NULL) ;

			for( int num2 = 0 ; num2 < k ; num2++ ) temp.push_back( ptr2[num2].list ) ;


			for( int num2 = 0 ; 1 != temp.size() ; num2++ ) {
				if ( num2*2 < temp.size() ) {
					ptr3[num2].list1.list.clear() ;
					ptr3[num2].list2.list.clear() ;
					for( int ke = 0 ; ke < temp[num2*2].size() ; ke++ ) ptr3[num2].list1.list.push_back( temp[num2*2][ke] ) ;
					if( num2*2+1 < temp.size() ) {
						for( int ke = 0 ; ke < temp[num2*2+1].size() ; ke++ ) ptr3[num2].list2.list.push_back( temp[num2*2+1][ke] ) ;
					} // if

					pthread_create( &a[num2], NULL, MergeSort2,(void*) &ptr3[num2] ) ;
				} // if

				if ( num2*2+1 >= temp.size() ) {
					for( int num3 = 0 ; num3 < k-1 ; num3++ ) {
						pthread_join(a[num3], NULL) ;
						if( num3*2 < temp.size() ) {
							temp[num3*2].clear() ;
							for ( int ke = 0 ; ke < ptr3[num3].list1.list.size() ; ke++ ) temp[num3*2].push_back(  ptr3[num3].list1.list[ke] ) ;
						} // if

					} // for
					for( int o = 1 ; o < temp.size() ; o++ ) temp.erase( temp.begin()+o ) ;
					num2 = -1 ;
				} // if

			} // for

			Print( temp[0] ) ;
		} // if   mission == 2
		else if ( mission == 3 ) {
			for( int s = 0, d = 0 ; s < k ; s++ ) {
				ptr2[s].head = d ;
				if( d+num <= dataStruct.size() ) ptr2[s].num = num ;
				else ptr2[s].num = dataStruct.size() - d ;
				for( int q = d ; q < d+num ; q++ ) ptr2[s].list.push_back( dataStruct[q] ) ;
				d = d+num ;
			} // for

			for( int num2 = 0 ; num2 < k ; num2++ ) BubbleSort( ptr2[num2].list, 0, ptr2[num2].num ) ;
			for( int num2 = 1 ; num2 < k ; num2++ ) {
				MergeSort( ptr2[0].list, ptr2[num2].list ) ;
			} // for
			Print( ptr2[0].list ) ;


		} // if mission == 3
		else if ( mission == 4 ) {
			for( int s = 0, d = 0 ; s < k ; s++ ) {  //  d是每一分起點的index
				ptr2[s].head = d ;
				if( d+num <= dataStruct.size() ) ptr2[s].num = num ;
				else ptr2[s].num = dataStruct.size() - d ;
				for( int q = d ; q < d+num ; q++ ) ptr2[s].list.push_back( dataStruct[q] ) ;
				d = d+num ;
			} // for


			for( int num2 = 0 ; num2 < k ; num2++ ) BubbleSort( ptr2[num2].list, 0, ptr2[num2].num ) ;
			for( int num2 = 1 ; num2 < k ; num2++ ) {
				MergeSort( ptr2[0].list, ptr2[num2].list ) ;
			} // for
			Print( ptr2[0].list ) ;


		} // else



	} // if

	float a = clock() - start ;
	cout << '\n' << "花費 " << a/1000 << "s\n" ;
	cout << "總共有 " << dataStruct.size() << "筆資料\n" ;
	system("pause") ; 

} // main()
