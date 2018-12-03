// ��u�T�� 10520119 ���ժl
#include <stdlib.h>
#include<algorithm>
#include <string>
#include<sstream>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

struct Token {
  int table_type;
  int table_index;
  int column;
};

class TokenTable {
  private:
    vector<Token> token_table;
    string table_05[100];
    string table_06[100];
    string table_07[100];
  public:
    bool IsTable01( string tokenstring, int column ) {            // �B�z Instrution table
      int index = 1;
      fstream file;
      string inputstr;
      string lowstr;
      file.open( "Table1.table", ios::in );
      if ( file ) {
        while ( file.peek() != EOF ) {
          file >> inputstr;
          file >> inputstr;
          lowstr = inputstr;
          transform(lowstr.begin(), lowstr.end(), lowstr.begin(), ::toupper);
          if ( inputstr == tokenstring || lowstr == tokenstring ) {
            Token token;
            token.table_type = 1;
            token.table_index = index;
            token.column = column;
            token_table.push_back(token); 
            return true;   
          } // if

          index++;
        } // while
        
        file.close();   
      } // if
      return false;
    } // IsTable01()

    bool IsTable02( string tokenstring, int column ) {            // �B�z Pseduo table
      int index = 1;
      fstream file;
      string inputstr;
      string lowstr;
      file.open( "Table2.table", ios::in );
      if ( file ) {
        while ( file.peek() != EOF ) {
          file >> inputstr;
          file >> inputstr;
          lowstr = inputstr;
          transform(lowstr.begin(), lowstr.end(), lowstr.begin(), ::toupper);
          if ( inputstr == tokenstring || lowstr == tokenstring ) {
            Token token;
            token.table_type = 2;
            token.table_index = index;
            token.column = column;
            token_table.push_back(token);
            return true;   
          } // if

          index++;
        } // while
        
        file.close();   
      } // if
      return false;
    } // IsTable02()

    bool IsTable03( string tokenstring, int column ) {              // �B�z Register table
      int index = 1;
      fstream file;
      string inputstr;
      string lowstr;
      file.open( "Table3.table", ios::in );
      if ( file ) {
        while ( file.peek() != EOF ) {
          file >> inputstr;
          file >> inputstr;
          lowstr = inputstr;
          transform(lowstr.begin(), lowstr.end(), lowstr.begin(), ::toupper);
          if ( inputstr == tokenstring || lowstr == tokenstring ) {
            Token token;
            token.table_type = 3;
            token.table_index = index;
            token.column = column;
            token_table.push_back(token);
            return true;   
          } // if

          index++;
        } // while
        
        file.close();   
      } // if
      return false;
    } // IsTable03()

    bool IsTable04( string tokenstring, char delimiter, int & column, fstream & inputfile, bool & IsString ) {   //�B�z Delimiter table
      bool btable04 = false;
      string delimiterstr = "";
      delimiterstr.push_back(delimiter);
      int index = 1;                                                   // �B�z����token��
      fstream file;
      string inputstr;
      file.open( "Table4.table", ios::in );
      if ( file ) {
        while ( file.peek() != EOF ) {
          file >> inputstr;
          file >> inputstr;
          if ( inputstr == delimiterstr ) {
            btable04 = true;
            break;   
          } // if

          index++;
        } // while
        
        file.close();   
      } // if
      
      if ( btable04 ) {
        if ( tokenstring == "" ) IsString = true; 
        if ( tokenstring != "" ) {    
          if ( IsTable07( tokenstring, column, index )) IsString = false;          // �B�z�e����token��
          else if ( IsTable01( tokenstring, column ) );
          else if ( IsTable02( tokenstring, column ) );
          else if ( IsTable03( tokenstring, column ) );
          else if ( IsTable06( tokenstring, column ) );
          else {
            Table05( tokenstring, column );
          } // else
       
        } // if

        Token token;                                                  // �B�z�����Otoken��
        token.table_type = 4;
        token.table_index = index;
        token.column = column;
        token_table.push_back(token);
        if ( token.table_index == 9 ) {
          getline( inputfile, inputstr);  
          column++; //����Ū��
        } // if  
        return true;
      } // if
      
      return false;
    } // IsTable04()

    bool IsTable07( string tokenstring, int column, int index ) {      // �B�zString table
      if ( token_table.back().table_index == 13 && index == 13 ) {
        int position = ToHashTable( tokenstring, 7 );
        Token token;                                                  
        token.table_type = 7;
        token.table_index = position;
        token.column = column;
        token_table.push_back(token);
        return true;
      } // if

      return false;
    } //  IsTable07()

    bool IsTable06( string tokentointeger, int column ) {              // �B�zInteger table
      int strsize = tokentointeger.size() - 1; 
      int i = 0;
      int number = 0;
      number = tokentointeger[i];
      if ( number >= 48 && number <= 57 ) {       // �}�Y�O�Ʀr
        bool Is16 = true;
        bool Is10 = true;
        if ( tokentointeger[strsize] != 'H' || tokentointeger[strsize] != 'h' ) Is16 = false;
        while ( i != strsize && Is16 ) {             // �P�_�O�_16�i��
          number = tokentointeger[i];
          if ( ( number >= 48 && number <= 57 ) || ( number >= 65 && number <= 70 ) ) { 
          } // if
          else {
            Is16 = false;
          } // else
          i++;
        } // while

        i = 0;
        while ( tokentointeger[i] ) {                     // �P�_�O�_10�i��
          number = tokentointeger[i];
          if ( number >= 48 && number <= 57 ) {
          } // if
          else {
            Is10 = false;
          } // else
          i++;
        } // while

        if ( Is10 || Is16 ) {
          int position = ToHashTable( tokentointeger, 6 );
          Token token;                                                  
          token.table_type = 6;
          token.table_index = position;
          token.column = column;
          token_table.push_back(token);
          return true;
        }  // if

      } // if

      return false;
    } // IsTable06()

    bool IsString() {
      if ( !token_table.empty() ) {
        if ( token_table.back().table_index == 13 ) return true;
      } // if
      
      return false;
    } //  IsString()

    void Table05( string tokenstring, int column ) {                   // �B�zSymbol table
      int position = ToHashTable( tokenstring, 5 );
      Token token;                                                  
      token.table_type = 5;
      token.table_index = position;
      token.column = column;
      token_table.push_back(token);
    } //  Table05()

  
    int ToHashTable( string strtoint, int table5or6or7 ) {                               // �p��Xhash�å[�J��m
      char s;
      int i = 0, position = 0;
      while ( strtoint[i] ) {
        s = strtoint[i];
        position = position + s;
        i++;
      } // while

      position = position % 100;
      bool reapt = false;
      if ( table5or6or7 == 5 ) {
        if ( table_05[position] != "" ) {
          if ( table_05[position] == strtoint ) reapt = true; 
          while ( table_05[position] != "" && !reapt ) {        
            position++;
            if ( position == 100 ) position = 0;
          } // while

         } // if

         table_05[position] = strtoint;
       } // if

      if ( table5or6or7 == 6 ) {
        if ( table_06[position] != "" ) {
          while ( table_06[position] != "" ) {        
            position++;
            if ( position == 100 ) position = 0;
          } // while

         } // if

         table_06[position] = strtoint;
      } // if

      if ( table5or6or7 == 7 ) {
        if ( table_07[position] != "" ) {
          while ( table_07[position] != "" ) {        
            position++;
            if ( position == 100 ) position = 0;
          } // while

         } // if

         table_07[position] = strtoint;
      } // if

      return position;
    } // ToHashTable

    void OutputToken( string inputstr ) {                              // ��XToken
      fstream outputfile;
      fstream inputfile;
      inputfile.open( inputstr.c_str(), ios::in );
      string outstr = inputstr + " To " +"Output.txt";
      outputfile.open( outstr.c_str(), ios::out );
      string inputline;
      bool hastoken = false;
      int curcolumn = 1;
      int i = 0, j = 0;
      while ( inputfile.peek() != EOF ) { 
        getline( inputfile, inputline ) ;  //��J�@��
        outputfile << inputline << "\n";
        i = token_table[j].column;
        while ( curcolumn == i ) {
          hastoken = true;
          outputfile << "(" << token_table[j].table_type << "," << token_table[j].table_index << ")"; // ��X�o�@��(curcolumn)��token
          j++;
          i = token_table[j].column;
        } // while

        if ( hastoken ) outputfile << "\n";
        hastoken = false;
        curcolumn++;
      } // while 

      inputfile.close();
    
    } // OutputToken()
};

void GetToken( TokenTable & Token_Table, fstream & inputfile ) {
  int column = 1;
  string UnfinishedToken = "";
  string line = "";
  char inputchar = '\0';
  bool IsString = false;
  while ( inputfile.peek() != EOF || UnfinishedToken != "" ) {
    inputchar = inputfile.get();
    if ( Token_Table.IsString() && inputchar != 39 && IsString ) {
      UnfinishedToken.push_back( inputchar );
	    IsString = true;	
    } // if
    else if ( inputchar == ' ' || inputchar == '\n' || inputchar == '\t' || inputchar == EOF ) {  // �J��white space
      if ( UnfinishedToken != "" ) {                                     // ��1��token
        if ( Token_Table.IsTable01( UnfinishedToken, column ) );
        else if ( Token_Table.IsTable02( UnfinishedToken, column ) );
        else if ( Token_Table.IsTable03( UnfinishedToken, column ) );
        else if ( Token_Table.IsTable06( UnfinishedToken, column ) );
        else {
          Token_Table.Table05( UnfinishedToken, column );
        } // else
        UnfinishedToken = "";
      } // if
      if ( inputchar == '\n' ) column++;  
    } // if
    else if ( Token_Table.IsTable04( UnfinishedToken, inputchar, column, inputfile, IsString ) ) {       // �J��Delimiter  
      UnfinishedToken = "";
    } // else if
    else {
      UnfinishedToken.push_back( inputchar );
    } // else
    
  } // while
 
} // GetToken()

int main() {
  fstream inputfile; 
  string inputstr;
  cout << "Lexical Analysis 10520119 ��u�T�� ���ժl\n" << "Please input the file name!!\n";
  cin >> inputstr;
  inputfile.open( inputstr.c_str(), ios::in );
  TokenTable Token_Table;
  if ( inputfile )  {
    GetToken( Token_Table, inputfile );
    Token_Table.OutputToken(inputstr);
    cout << "Finished!\n";
  } // if  
  else cout << "Not Find File\n";
  system("pause");
  return 0;
} // main()
