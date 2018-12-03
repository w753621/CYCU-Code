// 資工三甲 10520119 黃博泓
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip> // 在數字前面補零
using namespace std;

struct Token
{
  int table_type;
  int table_index;
  int column;
};

struct Label
{
  string name;
  int address;
};

class TokenTable
{
private:
  vector<Token> token_table;
  Label label[100];
  string table_05[100];
  string table_06[100];
  string table_07[100];

public:
  bool IsTable01(string tokenstring, int column)
  { // 處理 Instrution table
    int index = 1;
    fstream file;
    string inputstr;
    string lowstr;
    file.open("Table1.table", ios::in);
    if (file)
    {
      while (file.peek() != EOF)
      {
        file >> inputstr;
        file >> inputstr;
        lowstr = inputstr;
        transform(lowstr.begin(), lowstr.end(), lowstr.begin(), ::toupper);
        if (inputstr == tokenstring || lowstr == tokenstring)
        {
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

  bool IsTable02(string tokenstring, int column)
  { // 處理 Pseduo table
    int index = 1;
    fstream file;
    string inputstr;
    string lowstr;
    file.open("Table2.table", ios::in);
    if (file)
    {
      while (file.peek() != EOF)
      {
        file >> inputstr;
        file >> inputstr;
        lowstr = inputstr;
        transform(lowstr.begin(), lowstr.end(), lowstr.begin(), ::toupper);
        if (inputstr == tokenstring || lowstr == tokenstring)
        {
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

  bool IsTable03(string tokenstring, int column)
  { // 處理 Register table
    int index = 1;
    fstream file;
    string inputstr;
    string lowstr;
    file.open("Table3.table", ios::in);
    if (file)
    {
      while (file.peek() != EOF)
      {
        file >> inputstr;
        file >> inputstr;
        lowstr = inputstr;
        transform(lowstr.begin(), lowstr.end(), lowstr.begin(), ::toupper);
        if (inputstr == tokenstring || lowstr == tokenstring)
        {
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

  bool IsTable04(string tokenstring, char delimiter, int &column, fstream &inputfile, bool &IsString)
  { //處理 Delimiter table
    bool btable04 = false;
    string delimiterstr = "";
    delimiterstr.push_back(delimiter);
    int index = 1; // 處理本身token時
    fstream file;
    string inputstr;
    file.open("Table4.table", ios::in);
    if (file)
    {
      while (file.peek() != EOF)
      {
        file >> inputstr;
        file >> inputstr;
        if (inputstr == delimiterstr)
        {
          btable04 = true;
          break;
        } // if

        index++;
      } // while

      file.close();
    } // if

    if (btable04)
    {
      if (tokenstring == "")
        IsString = true;
      if (tokenstring != "")
      {
        if (IsTable07(tokenstring, column, index))
          IsString = false; // 處理前面有token時
        else if (IsTable01(tokenstring, column))
          ;
        else if (IsTable02(tokenstring, column))
          ;
        else if (IsTable03(tokenstring, column))
          ;
        else if (IsTable06(tokenstring, column))
          ;
        else
        {
          Table05(tokenstring, column);
        } // else

      } // if

      Token token; // 處理本身是token時
      token.table_type = 4;
      token.table_index = index;
      token.column = column;
      token_table.push_back(token);
      if (token.table_index == 9)
      {
        getline(inputfile, inputstr);
        column++; //註解讀掉
      }           // if
      return true;
    } // if

    return false;
  } // IsTable04()

  bool IsTable07(string tokenstring, int column, int index)
  { // 處理String table
    if (token_table.back().table_index == 13 && index == 13)
    {
      int position = ToHashTable(tokenstring, 7);
      Token token;
      token.table_type = 7;
      token.table_index = position;
      token.column = column;
      token_table.push_back(token);
      return true;
    } // if

    return false;
  } //  IsTable07()

  bool IsTable06(string tokentointeger, int column)
  { // 處理Integer table
    int strsize = tokentointeger.size() - 1;
    int i = 0;
    int number = 0;
    number = tokentointeger[i];
    if (number >= 48 && number <= 57)
    { // 開頭是數字
      bool Is16 = true;
      bool Is10 = true;
      if (tokentointeger[strsize] != 'H')
        Is16 = false;
      while (i != strsize && Is16)
      { // 判斷是否16進位
        number = tokentointeger[i];
        if ((number >= 48 && number <= 57) || (number >= 65 && number <= 70))
        {
        } // if
        else
        {
          Is16 = false;
        } // else
        i++;
      } // while

      i = 0;
      while (tokentointeger[i])
      { // 判斷是否10進位
        number = tokentointeger[i];
        if (number >= 48 && number <= 57)
        {
        } // if
        else
        {
          Is10 = false;
        } // else
        i++;
      } // while

      if (Is10 || Is16)
      {
        int position = ToHashTable(tokentointeger, 6);
        Token token;
        token.table_type = 6;
        token.table_index = position;
        token.column = column;
        token_table.push_back(token);
        return true;
      } // if

    } // if

    return false;
  } // IsTable06()

  bool IsString()
  {
    if (!token_table.empty())
    {
      if (token_table.back().table_index == 13)
        return true;
    } // if

    return false;
  } //  IsString()

  void Table05(string tokenstring, int column)
  { // 處理Symbol table
    int position = ToHashTable(tokenstring, 5);
    Token token;
    token.table_type = 5;
    token.table_index = position;
    token.column = column;
    token_table.push_back(token);
  } //  Table05()

  int ToHashTable(string strtoint, int table5or6or7)
  { // 計算出hash並加入位置
    char s;
    int i = 0, position = 0;
    while (strtoint[i])
    {
      s = strtoint[i];
      position = position + s;
      i++;
    } // while

    position = position % 100;
    bool reapt = false;
    if (table5or6or7 == 5)
    {
      if (table_05[position] != "")
      {
        if (table_05[position] == strtoint)
          reapt = true;
        while (table_05[position] != "" && !reapt)
        {
          position++;
          if (position == 100)
            position = 0;
        } // while

      } // if

      table_05[position] = strtoint;
    } // if

    if (table5or6or7 == 6)
    {
      if (table_06[position] != "")
      {
        while (table_06[position] != "")
        {
          position++;
          if (position == 100)
            position = 0;
        } // while

      } // if

      table_06[position] = strtoint;
    } // if

    if (table5or6or7 == 7)
    {
      if (table_07[position] != "")
      {
        while (table_07[position] != "")
        {
          position++;
          if (position == 100)
            position = 0;
        } // while

      } // if

      table_07[position] = strtoint;
    } // if

    return position;
  } // ToHashTable

  void OutputToken(string inputstr)
  { // 輸出Token
    fstream outputfile;
    fstream inputfile;
    inputfile.open(inputstr.c_str(), ios::in);
    string outstr = inputstr + " To " + "Output.txt";
    outputfile.open(outstr.c_str(), ios::out);
    string inputline;
    bool hastoken = false;
    int curcolumn = 1;
    int i = 0, j = 0;
    while (inputfile.peek() != EOF)
    {
      getline(inputfile, inputline); //輸入一行
      outputfile << inputline << "\n";
      i = token_table[j].column;
      while (curcolumn == i)
      {
        hastoken = true;
        outputfile << "(" << token_table[j].table_type << "," << token_table[j].table_index << ")"; // 輸出這一行(curcolumn)的token
        j++;
        i = token_table[j].column;
      } // while

      if (hastoken)
        outputfile << "\n";
      hastoken = false;
      curcolumn++;
    } // while

    inputfile.close();

  } // OutputToken()

  void CrossAssembler(string inputstr)
  {
    fstream outputfile, inputfile;
    inputfile.open(inputstr.c_str(), ios::in);
    string outstr = inputstr + " To " + "output2.txt";
    outputfile.open(outstr.c_str(), ios::out);
    string inputline;
    int hexAddress = 0, curline = 1, i = 0, j = 0;
    int filesize = token_table.size();
    int linesize = token_table[filesize - 1].column;
    while (curline <= linesize)
    { // 一行一行處理
      getline(inputfile, inputline);
      i = token_table[j].column;
      DealInstruction(curline, i, j, hexAddress, outputfile); // curline控制行，i控制token的column，j控制第幾個token
      outputfile << inputline << "\n";                        // 輸出一行原始行
      curline++;
    } // while

  } // CrossAssembler()

  void DealInstruction(int curline, int i, int &j, int &hexAddress, fstream &outputfile)
  {
    // curline控制行，i控制token的column，j控制第幾個token
    int numberhex = 0;
    bool highlowbyte = false;
    outputfile << setw(4) << setfill('0') << hex << hexAddress << "\t";
    while (curline == i)
    {
      if (token_table[j].table_type == 5)
      {
      } // if
      else if (token_table[j].table_type == 4)
      {
      } // else if
      else if (token_table[j].table_type == 2)
      {
        MachineTable2(j, outputfile);
      } // else if
      else if (token_table[j].table_type == 3)
      {

      } // else if
      else if (token_table[j].table_type == 1)
      {
        MachineTable1(j, numberhex, outputfile, highlowbyte);
      } // else if
      else if (token_table[j].table_type == 7)
      {
        // 直接翻成機器碼。
        MachineTable7(j, numberhex, outputfile);
      } // else if
      else if (token_table[j].table_type == 6)
      {
        // 直接翻成機器碼。
        MachineTable6(j, numberhex, outputfile, highlowbyte);
      } // else if

      j++;
      i = token_table[j].column;
    } // while

    // 處理完了當前行的所有指令。
    hexAddress = hexAddress + numberhex;
    numberhex = 0;
    highlowbyte = false;
    outputfile << "\t";
  } // DealInstruction()

  void MachineTable2(int j, fstream &outputfile)
  {
    // PTR if ( token_table[j].table_index == 15 )

  } // MachineTable2()

  void MachineTable1(int j, int &numberhex, fstream &outputfile, bool highlowbyte)
  {
    if (token_table[j].table_index == 51)
    {

    } // call

  } // MachineTable1()

  void MachineTable6(int j, int &numberhex, fstream &outputfile, bool highlowbyte)
  {
    int position = token_table[j].table_index;
    int size = table_06[position].size();
    string dealstring = table_06[position];
    string dealstring16 = "";
    int i = 0, hexvalue = 0;
    if (dealstring[size - 1] == 'H')
    { //..hex
      while (i < size - 1)
      {
        dealstring16 = dealstring16 + dealstring[i];
        i++;
      } // while

      if (dealstring16.size() > 2)
      { // 處理highlowbyte反過來
        if (highlowbyte)
          Switchtwohex(dealstring16);
      } // if

      numberhex = numberhex + aStringtoMC(dealstring16);
      cout << numberhex << "\n";
      outputfile << dealstring16 << " ";
    } // if
    else
    { //..demical
      stringstream ss;
      ss << dealstring;
      ss >> hexvalue;
      outputfile << hex << hexvalue << " ";
      while (hexvalue > 0)
      {
        numberhex++;
        hexvalue = hexvalue / 16;
      } // while
    }   // else

  } // MachineTable6()

  void MachineTable7(int j, int &numberhex, fstream &outputfile)
  {
    int position = token_table[j].table_index;
    int size = table_07[position].size();
    string dealstring = table_07[position];
    int i = 0, hexvalue = 0;
    while (i < size)
    {
      hexvalue = aChartoint(dealstring[i]);
      outputfile << hex << hexvalue << " ";
      i++;
    } // while

    numberhex = numberhex + i;
  } // MachineTable7()

  void Switchtwohex(string &switchstring)
  {
    string test = "";
    test = test + switchstring[2] + switchstring[3] + switchstring[0] + switchstring[1];
    switchstring = test;
  } // Switchtwohex()

  int aChartoint(char ch)
  { // char轉int
    int toint = ch;
    return toint;
  } //  aChartoHex()

  int aStringtoMC(string &mc)
  { // 16進位string轉machine code輸出格式，回傳個數。
    int number = 0, Istwo = 0;
    string newmc = "";
    int i = 0;
    while (i < mc.size())
    {
      Istwo++;
      newmc = newmc + mc[i];
      if (Istwo == 2)
      {
        Istwo = 0;
        number++;
        newmc = newmc + " ";
      } // if
      i++;
    } // while

    mc = newmc;
    return number;
  } // aStringtoMC

  void TwoPass()
  { // 處理label與其指令的相對距離。

  } // TwoPass()

  void DealTwoHex(string twohex, int &hex1, int &hex2)
  {
    // 一次進來就是8個二進位，拆成四四。
    string int1 = "", int2 = "";
    int1 = int1 + twohex[0] + twohex[1] + twohex[2] + twohex[3];
    int2 = int2 + twohex[4] + twohex[5] + twohex[6] + twohex[7];
    if (int1[0] == '1')
      hex1 = hex1 + 8;
    if (int1[1] == '1')
      hex1 = hex1 + 4;
    if (int1[2] == '1')
      hex1 = hex1 + 2;
    if (int1[3] == '1')
      hex1 = hex1 + 1;
    if (int1[0] == '0')
      hex1 = hex1 + 1;
    if (int1[1] == '0')
      hex1 = hex1 + 1;
    if (int1[2] == '0')
      hex1 = hex1 + 1;
    if (int1[3] == '0')
      hex1 = hex1 + 1;

    if (int2[0] == '1')
      hex2 = hex2 + 8;
    if (int2[1] == '1')
      hex2 = hex2 + 4;
    if (int2[2] == '1')
      hex2 = hex2 + 2;
    if (int2[3] == '1')
      hex2 = hex2 + 1;
    if (int2[0] == '0')
      hex2 = hex2 + 1;
    if (int2[1] == '0')
      hex2 = hex2 + 1;
    if (int2[2] == '0')
      hex2 = hex2 + 1;
    if (int2[3] == '0')
      hex2 = hex2 + 1;

  } // DealTwoHex
};

void GetToken(TokenTable &Token_Table, fstream &inputfile)
{
  int column = 1;
  string UnfinishedToken = "";
  string line = "";
  char inputchar = '\0';
  bool IsString = false;
  while (inputfile.peek() != EOF || UnfinishedToken != "")
  {
    inputchar = inputfile.get();
    if (Token_Table.IsString() && inputchar != 39 && IsString)
    {
      UnfinishedToken.push_back(inputchar);
      IsString = true;
    } // if
    else if (inputchar == ' ' || inputchar == '\n' || inputchar == '\t' || inputchar == EOF)
    { // 遇到white space
      if (UnfinishedToken != "")
      { // 有1個token
        if (Token_Table.IsTable01(UnfinishedToken, column))
          ;
        else if (Token_Table.IsTable02(UnfinishedToken, column))
          ;
        else if (Token_Table.IsTable03(UnfinishedToken, column))
          ;
        else if (Token_Table.IsTable06(UnfinishedToken, column))
          ;
        else
        {
          Token_Table.Table05(UnfinishedToken, column);
        } // else
        UnfinishedToken = "";
      } // if
      if (inputchar == '\n')
        column++;
    } // if
    else if (Token_Table.IsTable04(UnfinishedToken, inputchar, column, inputfile, IsString))
    { // 遇到Delimiter
      UnfinishedToken = "";
    } // else if
    else
    {
      UnfinishedToken.push_back(inputchar);
    } // else

  } // while

} // GetToken()

int main()
{
  fstream inputfile;
  string inputstr;
  cout << "Cross Assembler 10520119 資工三甲 黃博泓\n"
       << "Please input the file name!!\n";
  cin >> inputstr;
  inputfile.open(inputstr.c_str(), ios::in);
  TokenTable Token_Table;
  if (inputfile)
  {
    GetToken(Token_Table, inputfile);
    Token_Table.CrossAssembler(inputstr);
    Token_Table.OutputToken(inputstr);
    cout << "Finished!\n";
  } // if
  else
    cout << "Not Find File\n";
  system("pause");
  return 0;
} // main()
