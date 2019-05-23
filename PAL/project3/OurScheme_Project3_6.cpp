# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>
# include <stack>
# include <queue>
# include <iomanip>  // �����X�p�Ʀ��
# include <sstream>
# include <iostream>
using namespace std;
static int utestNum = -1;

struct TokenData {  // token�r��Btoken�����O�C
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

struct LPforRP {
  int real_num;    // �C�@�ӳ��n���J.nil
  bool can_insert;
}; // LPforRP

struct Finished {
  bool exit;
}; // Finished

struct SystemSymbol {
  string symbol_name;
  string symbol_type;
}; // SystemSymbol

struct StringErrorInfo {
  int line;
  int column;
}; // StringErrorInfo

struct NoAtomQuoteErrorInfo {
  string token_name;
  int line;
  int column;
}; // NoAtomQuoteErrorInfo

struct NoRightParenErrorInfo {
  string token_name;
  int line;
  int column;
}; // NoRightParenErrorInfo

struct SaveFunctionParameter {
  string fun_Name;
  string fun_Type;
  int fun_Pos;
  int argument_Num;         // �p���function�h���X�ӰѼ�(�]�t�i��OFunction)�C
  queue<int> parameter_Pos; // �p��ѼƮɩҦs���ѼơC
  TreeNode * result_Node;   // �s�Ӽh���ѼơC
}; // SaveFunctionParameter

struct UnboundSymbolErrorInfo {
  string symbol_name;
}; // UnboundSymbolErrorInfo

struct NonFunctionErrorInfo {
  TreeNode * nonfun_name;
}; // NonFunctionErrorInfo

struct AgumentNumberErrorInfo {
  string fun_name;
}; // AgumentNumberErrorInfo

// ���U�s���O������k���X�k��
struct NonListErrorInfo {
  TreeNode * nonlist_node;
}; // NonListErrorInfo

struct FormatErrorInfo {
  string format_fun_name;
  TreeNode * format_error_node;
}; // FormatErrorInfo

struct LevelErrorInfo {
  string level_function_name;
}; // LevelErrorInfo

struct ArgumentTypeErrorInfo {
  string function_name;
  TreeNode * argumentype_error_node;
}; // ArgumentTypeErrorInfo

struct DivisionByZero {
  string error_name;
}; // DivisionByZero

struct NoReturnValueErrorInfo {
  TreeNode * no_return_error_node;
}; // NoReturnValueErrorInfo

struct UserFunctionInfo {
  string function_type;
  int function_pos;
  vector<TreeNode*> real_para;
}; // UserFunctionInfo

struct DefineTable {
  string user_define_name;
  string user_define_type; // "Parameter"�B"Define-Function"�B"Lambda-Function"�C
  TreeNode * define_value; // "Parameter"�s���ѼƩΡB"Lambda-Function"�s��key node(check for keyvalue)�C
  vector<TreeNode*> fun_argument;
  vector<TreeNode*> eva_function;
}; // DefineTable

struct UnboundParameterErrorInfo {
  TreeNode * unbound_Parameter_node;
}; // UnboundParameterErrorInfo

struct UnboundTestConditionErrorInfo {
  TreeNode * unbound_Testcondition_node;
}; // UnboundTestConditionErrorInfo

struct UnboundConditionErrorInfo {
  TreeNode * unbound_Condition_node;
}; // UnboundConditionErrorInfo

vector<DefineTable> gDefineTable;

class Scanner {         // �u�t�d���XGetToken()�A��PeekToken()�A�æ^�Ǹ�Token�r��C
  private:
  int mscan_line, mscan_column;
  void ASeparatorType( char sexpchar, TokenData & token_data ) {
    if ( sexpchar == '(' ) {
      if ( cin.peek() == ')' ) {                    //  case1:() ���ݤU�@�Ӧr���A�p�G�O')'�NŪ�J�A�ç⦹token���O�令"NIL"�C
        char sexpchar = '\0';
        cin.get( sexpchar );
        TraceTokenLineColumn( sexpchar );
        token_data.token_name = "nil";
        token_data.token_type = "NIL";
      } // if
      //  case2: ( ) LEFT-PAREN RIGHT-PAREN
      else if ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) {
        char sexpchar = '\0';
        cin.get( sexpchar );
        TraceTokenLineColumn( sexpchar );
        bool finished = false;
        while ( ( sexpchar == ' ' || sexpchar == '\n' || sexpchar == '\t' ) && !finished ) {
          if ( cin.peek() == ' ' || cin.peek() == '\n' || cin.peek() == '\t' ) {
            cin.get( sexpchar );
            TraceTokenLineColumn( sexpchar );
          } // if
          else if ( cin.peek() == ')' ) {
            cin.get( sexpchar );
            finished = true;
            TraceTokenLineColumn( sexpchar );
            token_data.token_name = "nil";
            token_data.token_type = "NIL";
          } // else if
          else {
            finished = true;
            token_data.token_name = "(";                   // case3: ( LEFT-PAREN
            token_data.token_type = "LEFT-PAREN";
          } // else

        } // while

      } // else if
      else {
        token_data.token_name = "(";                   // case3: ( LEFT-PAREN
        token_data.token_type = "LEFT-PAREN";
      } // else

    } // if
    else if ( sexpchar == ')' ) {
      token_data.token_name = ")";
      token_data.token_type = "RIGHT-PAREN";
    }  // else if
    else if ( sexpchar == '\'' ) {
      token_data.token_name = "'";
      token_data.token_type = "QUOTE";
    } // else if
    else if ( sexpchar == ';' ) {                            // �o�O���ѡA�Q��ReadCommentLine()Ū���A�ðO����escan��C�C
      token_data.token_name = ";";
      token_data.token_type = "COMMENT";
      ReadCommentLine();
      mCommentLine++;
    } // else if
    else if ( sexpchar == '\"' ) {                           // �o�Ostring�A�p��5��case(wait deal)
      string aString = "";
      aString = aString + sexpchar;
      if ( IsString( aString ) ) {
        token_data.token_name = aString;
        token_data.token_type = "STRING";
      } // if
      else {
        // IsString()�p�Gfalse�|����throw�@��StringErrorInfo�^main()���C
      } // else

    } // else if
  } // ASeparatorType()

  // IsString()�|�P�_string����k�A"���᪺��line-eneter���e�n��"�A�S���ܴNthrow StringErrorInfo�A
  bool IsString( string & aString ) {
    bool result = false;                    // �ç�᭱��line-enter�e����Ū���C
    StringErrorInfo aStringErrorInfo;
    char sexpchar = '\0';
    if ( cin.peek() == '\n' ) {             // �U�@�ӧY�O����r��gg�A���Oquote�A������᭱����Ƥ]��Ū��(���ݭn)�C
      cin.get( sexpchar );
      aStringErrorInfo.column = mscan_column + 1;
      aStringErrorInfo.line = mscan_line;
      TraceTokenLineColumn( sexpchar );
      ReadErrorLine();
      throw aStringErrorInfo;
    } // if

    if ( cin.peek() != EOF ) cin.get( sexpchar );
    TraceTokenLineColumn( sexpchar );
    bool finished = false;
    while ( sexpchar != '\n' && !finished ) {
      if ( sexpchar == '\"' ) {                   // �J��"������������
        aString = aString + sexpchar;
        result = true;
        finished = true;
      } // if
      else if ( sexpchar == '\\' ) {            // �J������r���A�Ҽ{��@�Ӧr����O���On�Bt�B"�B\�o�|�ر��p�A�O���ܰ��A��B�z�A���O���ܴN�O���q�������r���C
        if ( cin.peek() == 'n' ) {
          cin.get( sexpchar );
          aString = aString + "\n";
          mscan_column = mscan_column + 1;
        } // if
        else if ( cin.peek() == 't' ) {
          cin.get( sexpchar );
          aString = aString + "\t";
          mscan_column = mscan_column + 1;
        } // else if
        else if ( cin.peek() == '\"' ) {
          cin.get( sexpchar );
          aString = aString + "\"";
          mscan_column = mscan_column + 1;
        } // else if
        else if ( cin.peek() == '\\' ) {
          cin.get( sexpchar );
          aString = aString + "\\";
          mscan_column = mscan_column + 1;
        } // else if
        else {                                // ���������L�N�u�O�@�Ӵ��q���r���Ӥw
          aString = aString + sexpchar;
        } // else

      } // else if
      else {
        aString = aString + sexpchar;
      } // else

      if ( !finished ) {
        if ( cin.peek() != EOF ) cin.get( sexpchar );
        if ( sexpchar == '\n' || cin.peek() == EOF ) {
          aStringErrorInfo.column = mscan_column + 1;
          aStringErrorInfo.line = mscan_line;
          if ( cin.peek() != EOF ) TraceTokenLineColumn( sexpchar );
          if ( cin.peek() != EOF ) ReadErrorLine();
          throw aStringErrorInfo;
        } // if
        else {
          TraceTokenLineColumn( sexpchar );
        } // else
      } // if
    } // while 

    return result;
  } // IsString()

  bool IsDOT( string token, TokenData & token_data ) {
    bool result = false;
    if ( token == "." ) {
      token_data.token_name = token;
      token_data.token_type = "DOT";
      result = true;
    } // if

    return result;
  } // IsDOT()

  bool IsINT( string token, TokenData & token_data ) {
    string intstring = "";
    bool result = false;
    int i = 0, number = 0;
    if ( token[0] == '+' || token[0] == '-' ) {           // '+'�B'-'�B'�Ʀr'�T�ر��p�C
      if ( token[0] == '-' ) intstring = intstring + '-';
      i++;
    } // if

    bool finished = false;
    while ( token[i] && !finished ) {
      number = token[i];
      if ( number >= 48 && number <= 57 ) {
        intstring = intstring + token[i];
        result = true;
      } // if
      else {
        result = false;
        finished = true;
      } // else if

      i++;
    } // while

    if ( result ) {
      token_data.token_name = intstring;
      token_data.token_type = "INT";
    } // if

    return result;
  } // IsINT()

  bool IsFLOAT( string token, TokenData & token_data ) {
    string floatstring = "";
    bool result = false, hasDotOnce = false;
    int i = 0, number = 0;
    // case1:.�}�Y case2:+�B-�}�Y case3:+�B-�}�Y�ĤG�r�����I
    if ( token[0] == '+' || token[0] == '-' || token[0] == '.' ) {
      if ( token[0] == '.' ) {
        floatstring = floatstring + "0.";
        hasDotOnce = true;
        i++;
      } // if
      else {
        if ( token[0] == '-' ) {
          floatstring = floatstring + "-";
          i++;
        } // if
        else if ( token[0] == '+' ) {
          i++;
        } // else if

        if ( token[1] == '.' ) {
          floatstring = floatstring + "0.";
          hasDotOnce = true;
          i++;
        } // if

      } // else

    } // if

    bool finished = false;
    while ( token[i] && !finished ) {
      number = token[i];
      if ( ( number >= 48 && number <= 57 ) ) {
        floatstring = floatstring + token[i];
        result = true;
      } // if
      else if ( ( token[i] == '.' ) && ( !hasDotOnce ) ) {
        floatstring = floatstring + token[i];
        result = true;
        hasDotOnce = true;
      } // else if
      else {
        hasDotOnce = false;
        result = false;
        finished = true;
      } // else

      i++;
    } // while

    if ( result && hasDotOnce ) {
      token_data.token_name = floatstring;
      token_data.token_type = "FLOAT";
    } // if

    return result;
  } // IsFLOAT()

  bool IsT( string token, TokenData & token_data ) {
    string tstring = "";
    bool result = false;
    if ( token == "t" || token == "#t" ) {
      tstring = "#t";
      result = true;
    } // if

    if ( result ) {
      token_data.token_name = tstring;
      token_data.token_type = "T";
    } // if

    return result;
  } // IsT()

  bool IsNIL( string token, TokenData & token_data ) {
    string nilstring = "";
    bool result = false;
    if ( token == "nil" || token == "#f" ) {
      nilstring = "nil";
      result = true;
    } // if

    if ( result ) {
      token_data.token_name = nilstring;
      token_data.token_type = "NIL";
    } // if

    return result;
  } // IsNIL()

  void TraceTokenLineColumn( char sexpchar ) {
    if ( sexpchar == '\n' ) {
      mscan_line++;
      mscan_column = 0;
    } // if
    else if ( sexpchar == '\t' ) {
      mscan_column = mscan_column + 8;
    } // else if
    else {
      mscan_column++;
    } // else

  } // TraceTokenLineColumn()

  public:
  int mCommentLine;
  Scanner( int lastspace ) {
    mscan_line = 1;
    mscan_column = lastspace;
    mCommentLine = 0;
  } // Scanner()

  TokenData GetOneToken() {         // ��U�@��token
    char sexpchar = '\0';
    if ( cin.peek() == EOF ) {                                // ���T�w�@�}�lŪ�����OEOF
      throw -1;
    } // if

    // �ϥΤ@�Ӧr���U�hŪ�A(�@) �����L�Ҧ�white space����EOF�ΫDWhite space
    TokenData token_data;
    cin.get( sexpchar );
    TraceTokenLineColumn( sexpchar );
    while ( sexpchar == '\n' || sexpchar == ' ' || sexpchar == '\t' ) {
      if ( cin.peek() == EOF ) {
        throw -1;
      } // if

      cin.get( sexpchar );
      TraceTokenLineColumn( sexpchar );
    } // while

    token_data.token_column = mscan_column;
    token_data.token_line = mscan_line;
    // �o�ɭԮ��쪺sexpchar�O�u�Dwhite space�v�A�i��O�useparator�v�Ρu�Dseparator�v�Aseparator�r���]
    // �Dseparator�A�B�z��white space(�@��token) or separator(���token)
    // ��token�i��O1.INT�B2.STRING(")�B3.DOT(.)�B4.FLOAT�B5.NIL(nil�B#f�B())�B6.T(t�B#t)�BQUOTE�BSYMBOL
    if ( sexpchar == '(' || sexpchar == ')' || sexpchar == '\'' || sexpchar == '\"' || sexpchar == ';' ) {
      ASeparatorType( sexpchar, token_data );
      return token_data;
    } // if
    else {
      string token = "";
      token = token + sexpchar;
      bool finished = false;
      while ( ( sexpchar != '\n' || sexpchar != ' ' || sexpchar != '\t' ) && !finished ) {
        // �w���U�Ӧr���p�G�Oseparator��white space�A����Ū�J�A�óB�z��separator���e�C
        char nextoken_char = '\0';
        nextoken_char = cin.peek();
        if ( ( nextoken_char == '\'' || nextoken_char == '\"' || nextoken_char == ';'
               || nextoken_char == '(' || nextoken_char == ')' || nextoken_char == EOF ) && !finished ) {
          if ( IsDOT( token, token_data ) ) ;
          else if ( IsINT( token, token_data ) ) ;
          else if ( IsFLOAT( token, token_data ) ) ;
          else if ( IsT( token, token_data ) ) ;
          else if ( IsNIL( token, token_data ) ) ;
          else {
            token_data.token_name = token;
            token_data.token_type = "SYMBOL";
          } // else

          finished = true;
        } // if

        if ( ( nextoken_char == '\n' || nextoken_char == ' ' || nextoken_char == '\t' ) && !finished ) {
          if ( IsDOT( token, token_data ) ) ;
          else if ( IsINT( token, token_data ) ) ;
          else if ( IsFLOAT( token, token_data ) ) ;
          else if ( IsT( token, token_data ) ) ;
          else if ( IsNIL( token, token_data ) ) ;
          else {
            token_data.token_name = token;
            token_data.token_type = "SYMBOL";
          } // else

          finished = true;
        } // if
        else if ( !finished ) { // �~��Token�������C
          cin.get( sexpchar );
          token = token + sexpchar;
          TraceTokenLineColumn( sexpchar );
        } // else if

      } // while

    } // else

    return token_data;
  } // GetOneToken()

  TokenData PeekToken() {           // Parser�w�����D�U�@��token��function�A�n�P�_��k���V�C
    // �ӦbGetNextTokenForParser()���A������lookahead�令true�A�~���|�SŪ�F�@�ӷs��token�i�ӡC
    TokenData token_data;
    token_data = GetOneToken();
    while ( token_data.token_type == "COMMENT" ) {
      token_data = GetOneToken();
    } // while

    return token_data;
  } // PeekToken()

  void ReadErrorLine() {
    char getnext = '\0';
    if ( mscan_column != 0 ) {
      if ( cin.peek() == EOF ) return;
      cin.get( getnext );
      while ( getnext != '\n' ) {
        if ( cin.peek() == EOF ) return;
        cin.get( getnext );
      } // while

    } // if
  } // ReadErrorLine()

  int ReadEndLine() {
    char getnext = '\0';
    int i = 0;
    while ( cin.peek() == ' ' || cin.peek() == ';' ) {
      cin.get( getnext );
      if ( cin.peek() == EOF ) return 0;
      i++;
      if ( getnext == ';' ) {
        ReadCommentLine();
        return 0;
      } // if

    } // while

    if ( cin.peek() == '\n' ) {
      cin.get( getnext );
      i = 0;
    } // if

    return i;
  } // ReadEndLine()

  void ReadCommentLine() {
    char getnext = '\0';
    if ( mscan_column != 0 ) {
      if ( cin.peek() == EOF ) return;
      cin.get( getnext );
      TraceTokenLineColumn( getnext );
      while ( getnext != '\n' ) {
        if ( cin.peek() == EOF ) return;
        cin.get( getnext );
        TraceTokenLineColumn( getnext );
      } // while

    } // if
  } // ReadCommentLine()

}; // Scanner

class Parser {
  private:
  vector<TokenData> mAccurate_token_vector;
  vector<TokenData> mTranslated_token_vector;
  bool mlookahead;                                     // �x��GetOneToken�ɪ��ǫh
  public:
  TokenData maPeekToken;
  Parser() {
    mlookahead = false;
  } // Parser()

  TokenData GetNextTokenForParser( Scanner & aScanner ) {      // �^�ǳo��Getoken
    TokenData get_token_data;
    if ( mlookahead ) {    // GetToken�ɡA�T�wlookahead�n�Ofalse�����p�U�~�i�H���Ugetoken�A�_�h�ݥX�wŪ���n�B�z��token�i�hparser
      get_token_data = maPeekToken;
      mlookahead = false;
    } // if
    else {
      get_token_data = aScanner.GetOneToken();
      while ( get_token_data.token_type == "COMMENT" ) {
        get_token_data = aScanner.GetOneToken();
      } // while

    } // else

    mAccurate_token_vector.push_back( get_token_data );
    return get_token_data;
  } // GetNextTokenForParser()

  bool IsSexp( TokenData & token_IsSexp, Scanner & aScanner ) {
    token_IsSexp = GetNextTokenForParser( aScanner );
    if ( IsAtom( token_IsSexp ) ) {                 // <ATOM>
      return true;
    } // if
    else if ( token_IsSexp.token_type == "LEFT-PAREN" ) {
      // LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
      if ( IsSexp( token_IsSexp, aScanner ) ) {
        // <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN ���U�~��
      } // if
      else {
        NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;        // return NoAtomQuoteErrorInfo
        aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
        aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
        aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line - aScanner.mCommentLine;
        aScanner.ReadErrorLine();
        throw aNoAtomQuoteErrorInfo;
      } // else

      token_IsSexp = aScanner.PeekToken();    // ���ݤU�@��Token { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
      maPeekToken = token_IsSexp;              // �]�waPeekToken
      mlookahead = true;
      if ( token_IsSexp.token_type != "RIGHT-PAREN" && token_IsSexp.token_type != "DOT" ) {
        bool loop = true;
        while ( loop ) {    // { <S-exp> }
          if ( IsSexp( token_IsSexp, aScanner ) ) {
            token_IsSexp = aScanner.PeekToken();
            maPeekToken = token_IsSexp;
            mlookahead = true;
            if ( token_IsSexp.token_type == "RIGHT-PAREN" || token_IsSexp.token_type == "DOT" ) {
              loop = false;
            } // if
          } // if
          else {
            NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;    // return NoAtomQuoteErrorInfo
            aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
            aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
            aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line;
            aScanner.ReadErrorLine();
            throw aNoAtomQuoteErrorInfo;
          } // else

        } // while

      } // if

      if ( token_IsSexp.token_type == "DOT" ) {
        mAccurate_token_vector.push_back( token_IsSexp );
        mlookahead = false;
        if ( IsSexp( token_IsSexp, aScanner ) ) {
          // ���U�~��
        } // if
        else {
          NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;     // return NoAtomQuoteErrorInfo
          aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
          aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
          aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line;
          aScanner.ReadErrorLine();
          throw aNoAtomQuoteErrorInfo;
        } // else

      } // if

      if ( mlookahead == false ) {               // �w��"DOT"���B�z�A�A�~��Peek�U�@�ӬO���O')'�C
        token_IsSexp = aScanner.PeekToken();    // ���ݤU�@��Token RIGHT-PAREN
        maPeekToken = token_IsSexp;              // �]�waPeekToken
        mlookahead = true;
      } // if

      if ( token_IsSexp.token_type == "RIGHT-PAREN" ) {
        mAccurate_token_vector.push_back( token_IsSexp );
        mlookahead = false;
        return true;
      } // if
      else {
        NoRightParenErrorInfo aNoRightParen;            // return NoRightParen
        aNoRightParen.token_name = token_IsSexp.token_name;
        aNoRightParen.column = token_IsSexp.token_column;
        aNoRightParen.line = token_IsSexp.token_line;
        aScanner.ReadErrorLine();
        throw aNoRightParen;
      } // else

    } // else if
    else if ( token_IsSexp.token_type == "QUOTE" ) {            // QUOTE <S-exp>
      if ( IsSexp( token_IsSexp, aScanner ) ) {
        return true;
      } // if
      else {
        NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;
        aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
        aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
        aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line;
        aScanner.ReadErrorLine();
        throw aNoAtomQuoteErrorInfo;
      } // else

    } // else if
    else {
      return false;
    } // else

    return false;
  } // IsSexp()

  bool IsAtom( TokenData token_IsAtom ) {
    if ( token_IsAtom.token_type == "SYMBOL" || token_IsAtom.token_type == "INT"
         || token_IsAtom.token_type == "FLOAT" || token_IsAtom.token_type == "STRING"
         || token_IsAtom.token_type == "NIL" || token_IsAtom.token_type == "LEFT-PAREN RIGHT-PAREN"
         || token_IsAtom.token_type == "T" ) {
      return true;
    } // if

    return false;
  } // IsAtom()

  void TranslateToken( queue<int> & dotforquote ) {      // ���J��sexp�নdotted pair���Φ��C
    TokenData token_Dot_data;
    token_Dot_data.token_name = ".";
    token_Dot_data.token_type = "DOT";
    TokenData token_LP_data;
    token_LP_data.token_name = "(";
    token_LP_data.token_type = "LEFT-PAREN";
    TokenData token_RP_data;
    token_RP_data.token_name = ")";
    token_RP_data.token_type = "RIGHT-PAREN";
    TokenData token_NIL_data;
    token_NIL_data.token_name = "nil";
    token_NIL_data.token_type = "NIL";
    TokenData token_QUOTE_data;
    token_QUOTE_data.token_name = "quote";
    token_QUOTE_data.token_type = "QUOTE";
    int anumber = 0;
    int realnumber = 0; // �J��U�@�ӥ��A�����e���٨S�B�z��quote�ƥ�(�n�b�k�A���B�z�ɷs�W.nil)
    stack<LPforRP> lpinfo; // �s�P�u��k�A���۹������u�ꥪ�A����T
    bool continue_lp = true;
    if ( mAccurate_token_vector.size() == 1 ) {
      mTranslated_token_vector.push_back( mAccurate_token_vector[0] );   // ATOM
      return;
    } // if
    else {
      while ( mAccurate_token_vector.size() > anumber ) {

        if ( mAccurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {    // ���O���Ӥl�Τ���sexp�����X�Ӫťե�
          LPforRP newLPforRP;
          continue_lp = true;
          newLPforRP.can_insert = continue_lp;
          int cur = mTranslated_token_vector.size() - 1;
          if ( cur != -1 ) {

            if ( mTranslated_token_vector[cur].token_type != "DOT"
                 && mTranslated_token_vector[cur].token_type != "LEFT-PAREN" ) {
              mTranslated_token_vector.push_back( token_Dot_data );
              mTranslated_token_vector.push_back( token_LP_data );
            } // if
            else if ( mTranslated_token_vector[cur].token_type == "DOT" ) {
              newLPforRP.can_insert = false;
            } // else if

          } // if

          mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );

          newLPforRP.real_num = 0;
          newLPforRP.real_num = newLPforRP.real_num + realnumber;
          lpinfo.push( newLPforRP );
          realnumber = 0;
          anumber++;

          if ( mAccurate_token_vector[anumber].token_type != "QUOTE"
               && mAccurate_token_vector[anumber].token_type != "LEFT-PAREN" ) {
            mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );    // �s(����U�@��sexp�i���O(�F�j
            anumber++;
          } // if
          else if ( mAccurate_token_vector[anumber].token_type != "LEFT-PAREN" ) {
            mTranslated_token_vector.push_back( token_LP_data );                       // ���ؤ@��(
            mTranslated_token_vector.push_back( token_QUOTE_data );
            realnumber++;
            anumber++;
          } // else if


        } // if
        else if ( mAccurate_token_vector[anumber].token_type == "QUOTE" ) {
          int cur = mTranslated_token_vector.size() - 1;
          if ( cur != -1 ) {
            if ( mTranslated_token_vector[cur].token_type != "DOT" ) {
              mTranslated_token_vector.push_back( token_Dot_data );
            } // if

            mTranslated_token_vector.push_back( token_LP_data );
            mTranslated_token_vector.push_back( token_LP_data );                       // ���ؤ@��(
            mTranslated_token_vector.push_back( token_QUOTE_data );                     // �ç�Quote��
          } // if
          else {
            mTranslated_token_vector.push_back( token_LP_data );                       // ���ؤ@��(
            mTranslated_token_vector.push_back( token_QUOTE_data );                     // �ç�Quote��
          } // else

          anumber++;
          realnumber++;
          continue_lp = true;
        } // else if
        else if ( mAccurate_token_vector[anumber].token_type == "RIGHT-PAREN" ) {
          LPforRP aLPforRP;
          aLPforRP = lpinfo.top();
          aLPforRP.real_num = aLPforRP.real_num + realnumber;
          int cur = mTranslated_token_vector.size() - 1;
          if ( mTranslated_token_vector[cur-1].token_type == "LEFT-PAREN" ) {
            mTranslated_token_vector.push_back( token_Dot_data );
            mTranslated_token_vector.push_back( token_NIL_data );
          } // if
          else if ( mTranslated_token_vector[cur].token_type == "RIGHT-PAREN" && continue_lp ) {
            mTranslated_token_vector.push_back( token_Dot_data );
            mTranslated_token_vector.push_back( token_NIL_data );
          } // else if

          mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          while ( aLPforRP.real_num > 0 ) {
            mTranslated_token_vector.push_back( token_Dot_data );
            mTranslated_token_vector.push_back( token_NIL_data );
            mTranslated_token_vector.push_back( token_RP_data );
            aLPforRP.real_num--;
          } // while

          continue_lp = aLPforRP.can_insert;
          anumber++;
          lpinfo.pop();
          realnumber = 0;
        } // else if
        else if ( mAccurate_token_vector[anumber].token_type == "DOT" ) {
          mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          if ( mAccurate_token_vector[anumber+1].token_type == "QUOTE" ) {
            dotforquote.push( mTranslated_token_vector.size() );
          } // if

          anumber++;
          continue_lp = true;
        } // else if
        else {                   // �h�W�[�@��.(
          int cur = mTranslated_token_vector.size() - 1;
          if ( mTranslated_token_vector[cur].token_type != "DOT"
               && mTranslated_token_vector[cur].token_type != "QUOTE" ) {
            mTranslated_token_vector.push_back( token_Dot_data );
            mTranslated_token_vector.push_back( token_LP_data );
            mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          } // if
          else if ( mTranslated_token_vector[cur].token_type == "QUOTE" ) {
            mTranslated_token_vector.push_back( token_Dot_data );
            mTranslated_token_vector.push_back( token_LP_data );
            mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
            mTranslated_token_vector.push_back( token_Dot_data );
            mTranslated_token_vector.push_back( token_NIL_data );
            mTranslated_token_vector.push_back( token_RP_data );
            realnumber--;
            while ( realnumber > 0 ) {
              mTranslated_token_vector.push_back( token_Dot_data );
              mTranslated_token_vector.push_back( token_NIL_data );
              mTranslated_token_vector.push_back( token_RP_data );
              realnumber--;
            } // while

          } // else if
          else {
            mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          } // else

          anumber++;
          continue_lp = true;
        } // else

      } // while

    } // else

  } // TranslateToken()

  TreeNode * BuildTree( TreeNode * aTreeRoot, int & i, queue<int> dotforquote, int & record_pos ) {
    record_pos++;
    TreeNode * newnode;
    newnode = new TreeNode;
    if ( mTranslated_token_vector.size() <= i ) {
      return NULL;
    } // if
    else if ( mTranslated_token_vector[i].token_type == "DOT" ) {
      // ���^�é��k�䨫�C
      return NULL;
    } // else if
    else {
      newnode -> right = NULL;
      newnode -> left = NULL;
      newnode -> isStart = false;
      newnode -> isEnd = false;
      newnode -> token_data = mTranslated_token_vector[i];
      if ( mTranslated_token_vector.size() == 1 ) return newnode;
      // �]�wSexp�}�l�A(�e�@�ӨS.�N��Start
      if ( i == 0 ) {
        newnode -> isStart = true;
      } // if
      else {
        if ( mTranslated_token_vector[i].token_type == "LEFT-PAREN"
             && mTranslated_token_vector[i-1].token_type != "DOT" ) {
          newnode -> isStart = true;
        } // if

      } // else

      if ( dotforquote.size() > 0 ) {
        int pos = dotforquote.front();
        if ( pos == record_pos ) {
          newnode -> isEnd = true;
          dotforquote.pop();
        } // if

      } // if

      i++;
      bool finished = false;
      // �w���U�@�ӬO���O)�A�O���ܪ����]��IsEnd��Ū���Ҧ�)
      while ( mTranslated_token_vector[i].token_type == "RIGHT-PAREN" && !finished ) {
        newnode -> isEnd = true;
        i++;
        if ( mTranslated_token_vector.size() > i ) {
          if ( mTranslated_token_vector[i].token_type != "RIGHT-PAREN" ) {
            finished = true;
          } // if

        } // if
        else {
          return newnode;
        } // else

      } // while

      newnode -> left = BuildTree( newnode, i, dotforquote, record_pos );      // �����@�Ӥlsexp(expr)����
      if ( mTranslated_token_vector.size() <= i ) return newnode;
      if ( mTranslated_token_vector[i].token_type == "DOT" ) {
        i++;
        return newnode; // �J��DOT���_
      } // if

      newnode -> right = BuildTree( newnode, i, dotforquote, record_pos );
      return newnode;
    } // else

    return newnode;
  } // BuildTree()

  vector<TokenData> ReturnToken_vector() {
    return mAccurate_token_vector;
  } // ReturnToken_vector()

  void PrintVector() {
    int i = 0;

    i = 0;
    cout << "\n";
    while ( mTranslated_token_vector.size() > i ) {
      cout << mTranslated_token_vector[i].token_name;
      i++;
    } // while

    cout << "\n";
  } // PrintVector()

}; // Parser

class Tree {
  private:
  vector<TreeNode*> mAllFunPara;  // �Ҧ��s�J��function & parameter�A(�s���A���s���I)�C
  vector<SystemSymbol> mSystemSymbolTable;
  vector<UserFunctionInfo> mUserFunctionInfo;
  SaveFunctionParameter mSave_Table;
  TreeNode * mStart_ResultSExp;   // �}�l���Y�A�������G�C
  bool mDoingQuote;          // ��sexp�u��quote�A�p��h�l���Ĥ@��quote�ΡC
  int mCurrent_pos;         // �C�@���I�s���W�[1�A�]���n�Φ��Ӵ��JmAllFunPara�C
  int mLevel_num;
  int mContorl_value;

  // KEY:�J��QUOTE�N�O�@��List�A���M�U�@��Is_Start���䱵���N�Ofunction�C
  TreeNode * EvaluateParameter( TreeNode * inputSExp ) {
    if ( inputSExp ) {
      if ( inputSExp -> isStart ) {  // �Ӽh�}�l�C�L�����n���������䪺function��b�k���䪺�Ҧ��ѼơC
        // Preorder( inputSExp );
        // �x�s��function node isStart����m�C
        mLevel_num++;
        ReSetmSave_Table();
        mAllFunPara.push_back( inputSExp );         // *�n���ק�(����node)*
        mSave_Table.fun_Pos = mCurrent_pos;         // *�n���ק�(����fun��m)*
        mCurrent_pos++;
        EvaluateParameter( inputSExp -> right );
        return CheckAllErrors_EVAFunction( mSave_Table );   // �}�l���B��(�}�h�}�l�p��C)
      } // if
      else if ( inputSExp -> isEnd ) {
        // �禬�ɶ�!!! non-list���u�����Ǥj��@���C���P�_���S��non-list�C
        NonList_Chcek( inputSExp, mSave_Table );
      } // else if
      else if ( inputSExp -> token_data.token_type == "LEFT-PAREN" ) {  // �J��s���Ϊ����A���C
        mSave_Table.argument_Num++;                                    // ����s��node�n�O�Ѽ�()�C
        // �x�s�ѼƦ�m�C
        mSave_Table.parameter_Pos.push( mCurrent_pos );       // �s���I��m
        mAllFunPara.push_back( inputSExp );
        mCurrent_pos++;
        EvaluateParameter( inputSExp -> right );
      } // else if

    } // if

    return NULL;
  } // EvaluateParameter()

  TreeNode * CheckAllErrors_EVAFunction( SaveFunctionParameter aSaveFunPara ) {
    int fun_pos = aSaveFunPara.fun_Pos;         // fun��m
    int argu_num = aSaveFunPara.argument_Num;   // argu�ƥ�
    TreeNode * fun_node = mAllFunPara[fun_pos] -> left; // ��즹function_node�F
    TreeNode * no_return_node = fun_node;
    if ( fun_node -> isStart ) {        // �p�G�S�J�쥪�A���A�N�}�l���j���ȡA����ڭn��function node�C
      fun_node = EvaluateParameter( fun_node );
    } // if

    // �A���ˬdFunction�A�̫��ˬdArgument Number�C
    if ( fun_node == NULL ) {
      NoReturnValueErrorInfo aNoReturnValueErrorInfo;
      aNoReturnValueErrorInfo.no_return_error_node = no_return_node;
      throw aNoReturnValueErrorInfo;
    } // if

    int user_function_pos = Function_Check( fun_node, aSaveFunPara.fun_Name, aSaveFunPara.fun_Type );
    ArgumentNum_Check( user_function_pos, argu_num, aSaveFunPara.fun_Name, aSaveFunPara.fun_Type );
    return EvaluateFunction( user_function_pos, aSaveFunPara );
  } // CheckAllErrors_EVAFunction()

  TreeNode * EvaluateFunction( int user_function_pos, SaveFunctionParameter aSaveFunPara ) {
    string fun_name = aSaveFunPara.fun_Name;
    string fun_type = aSaveFunPara.fun_Type;
    if ( fun_type == "Constructors" ) {
      if ( fun_name == "cons" ) return EVAcons( aSaveFunPara );
      else return EVAlist( aSaveFunPara );
    } // if
    else if ( fun_type == "Quote" ) {
      // �npassone quote�A�^�ǰѼơC
      return EVAquote( aSaveFunPara );
    } // else if
    else if ( fun_type == "Part accessors" ) {
      return EVAcar_cdr( aSaveFunPara );
    } // else if
    else if ( fun_type == "Bounding" ) {
      // �ھڲĤ@�ӰѼƨӧP�_�n�i�J���@����define��function�C
      int fun_pos = aSaveFunPara.parameter_Pos.front();
      if ( IsList( mAllFunPara[fun_pos] -> left ) ) EVAdefine_Type2( aSaveFunPara );
      else EVAdefine_Type1( aSaveFunPara );
    } // else if
    else if ( fun_type == "Primitive predicates" ) {
      return EVAprimitive_predicates( aSaveFunPara );
    } // else if
    else if ( fun_type == "Number arithmetic" ) {
      return EVAnumber_arithmetic( aSaveFunPara );
    } // else if
    else if ( fun_type == "Logical" ) {
      return EVAlogical( aSaveFunPara );
    } // else if
    else if ( fun_type == "Number compare" || fun_type == "String compare" ) {
      return EVAnumber_string_compare( aSaveFunPara );
    } // else if
    else if ( fun_type == "Eqivalence tester" ) {
      return EVAequal_eqv( aSaveFunPara );
    } // else if
    else if ( fun_type == "Sequencing" ) {
      return EVAbegin( aSaveFunPara );
    } // else if
    else if ( fun_type == "Conditionals" ) {
      if ( fun_name == "if" ) return EVAif( aSaveFunPara );
      else return EVAcond( aSaveFunPara );
    } // else if
    else if ( fun_type == "Let" ) {
      return EVAlet( aSaveFunPara );
    } // else if
    else if ( fun_type == "User-Defined-Function" ) {
      return EVAuser_Function( user_function_pos, aSaveFunPara );
    } // else if
    else if ( fun_type == "Lambda" ) {
      return EVAlambda( aSaveFunPara );
    } // else if

    return NULL;
  } // EvaluateFunction()

  // �ѼƦ��|�ت��i��A1.function 2.quote(�ܺ�)�Ѽ�  3.sybol(�ܺ�define)�Ѽ� 4.�@��Ѽ�
  // �p�G�Ofunction�|�^��false�A�}�l�i�J�s��function�p��C(��e�᭱���ѼƼȰ��p��)�C
  TreeNode * DealParameterType( TreeNode * para_node, string & parameter_type, bool & eva_functoin ) {
    int define_pos = -1; // ���P�_���O���OSYMBOL
    bool isExpect_else = false;
    bool is_global = true;
    string name = "";
    string type = "";
    if ( para_node -> token_data.token_name == "else" ) isExpect_else = true;
    // �p�G��e��function�Ouser-define-funcition�C
    if ( mUserFunctionInfo.size() > 0 && para_node -> token_data.token_type == "SYMBOL" ) {
      UserFunctionInfo aUserFunctionInfo;
      aUserFunctionInfo = mUserFunctionInfo[mUserFunctionInfo.size()-1];
      DefineTable aDefineTable;
      aDefineTable = gDefineTable[aUserFunctionInfo.function_pos];
      int info_pos = -1;
      if ( IsUserFunctionParameter( para_node -> token_data, aDefineTable, info_pos ) ) {
        is_global = false;
        para_node = aUserFunctionInfo.real_para[info_pos];
        name = para_node -> token_data.token_name;
        type = para_node -> token_data.token_type;
        if ( parameter_type == "Define" ) parameter_type = "NoCreateMemory";
        else if ( parameter_type == "Cond-LastElse" && isExpect_else ) {
          return para_node;
        } // else if
        else {
          TreeNode * new_eval_tree = NULL;
          para_node = Return_NewEval_Tree( para_node, new_eval_tree );
        } // else

        define_pos = info_pos;
      } // if
      else if ( IsBoundSymbol( para_node -> token_data, define_pos ) ) {
        is_global = true;
      } // else if
      else {
        is_global = false;
        name = para_node -> token_data.token_name;
        type = para_node -> token_data.token_type;
        if ( IsSystemSymbol( name, type ) ) {
          TreeNode * procedure_node = NULL;
          int fun_pos = -1;
          fun_pos = FindName_UserFunctionPos( name );
          procedure_node = CreateProcedureNode( para_node -> token_data.token_name );
          para_node = procedure_node;
        } // if
        else if ( parameter_type == "Cond-LastElse" ) {
          if ( para_node -> token_data.token_type == "SYMBOL"
               && para_node -> token_data.token_name == "else" ) {
            return para_node;
          } // if
          else if ( para_node -> token_data.token_type == "SYMBOL" ) {
            UnboundSymbolErrorInfo aUnboundSymbolErrorInfo;
            aUnboundSymbolErrorInfo.symbol_name = para_node -> token_data.token_name;
            throw aUnboundSymbolErrorInfo;
          } // else if

        } // else if
        else if ( para_node -> token_data.token_type == "SYMBOL" ) {
          UnboundSymbolErrorInfo aUnboundSymbolErrorInfo;
          aUnboundSymbolErrorInfo.symbol_name = para_node -> token_data.token_name;
          throw aUnboundSymbolErrorInfo;
        } // else if

      } // else

    } // if

    if ( IsBoundSymbol( para_node -> token_data, define_pos ) && is_global ) {
      name = para_node -> token_data.token_name;
      type = para_node -> token_data.token_type;
      if ( parameter_type == "Define" || parameter_type == "Eq" ) {
        para_node = gDefineTable[define_pos].define_value;
        if ( parameter_type == "Define" ) parameter_type = "NoCreateMemory";

      } // if
      else if ( parameter_type == "Cond-LastElse" && isExpect_else ) {
        return para_node;
      } // else if
      else if ( IsSystemSymbol( name, type ) ) {
        TreeNode * procedure_node = NULL;
        int fun_pos = -1;
        fun_pos = FindName_UserFunctionPos( name );
        procedure_node = CreateProcedureNode( para_node -> token_data.token_name );
        para_node = procedure_node;
      } // else if
      else {
        TreeNode * new_eval_tree = NULL;
        para_node = Return_NewEval_Tree( gDefineTable[define_pos].define_value, new_eval_tree );
        name = para_node -> token_data.token_name;
        type = para_node -> token_data.token_type;
      } // else

    } // if
    else if ( is_global ) {
      name = para_node -> token_data.token_name;
      type = para_node -> token_data.token_type;
      if ( IsSystemSymbol( name, type ) ) {
        TreeNode * procedure_node = NULL;
        int fun_pos = -1;
        fun_pos = FindName_UserFunctionPos( name );
        procedure_node = CreateProcedureNode( para_node -> token_data.token_name );
        para_node = procedure_node;
      } // if
      else if ( parameter_type == "Cond-LastElse" ) {
        if ( para_node -> token_data.token_type == "SYMBOL"
             && para_node -> token_data.token_name == "else" ) {
          return para_node;
        } // if
        else if ( para_node -> token_data.token_type == "SYMBOL" ) {
          UnboundSymbolErrorInfo aUnboundSymbolErrorInfo;
          aUnboundSymbolErrorInfo.symbol_name = para_node -> token_data.token_name;
          throw aUnboundSymbolErrorInfo;
        } // else if

      } // else if
      else if ( para_node -> token_data.token_type == "SYMBOL" ) {
        UnboundSymbolErrorInfo aUnboundSymbolErrorInfo;
        aUnboundSymbolErrorInfo.symbol_name = para_node -> token_data.token_name;
        throw aUnboundSymbolErrorInfo;
      } // else if

    } // else if

    //  �p�Gnode���ѼƬO�}�l�B�Ounbound�~�i�H�i�J�s��function�B��C
    if ( para_node -> isStart && define_pos == -1 ) {
      para_node = EvaluateParameter( para_node );
      eva_functoin = true;
    } // if
    // �}�l�i��Ѽƿz��C

    if ( mDoingQuote ) {
      parameter_type = "DoingQuote";
    } // if

    mDoingQuote = false;
    return para_node;
  } // DealParameterType()

  TreeNode * EVAcons( SaveFunctionParameter aSaveFunPara ) {
    int i = 0;
    string parameter_type = "Parameter";
    string new_string = "NEW", link = "LINK";
    aSaveFunPara.result_Node = ReSetLinkResult( new_string );
    // ��������A
    int para_pos = aSaveFunPara.parameter_Pos.front();
    bool eva_fun = false;
    TreeNode * left_node = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type, eva_fun );
    if ( eva_fun && CheckNodeIsNULL( left_node ) ) {
      UnboundParameterErrorInfo aUnboundParameterErrorInfo;
      aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para_pos] -> left;
      throw aUnboundParameterErrorInfo;
    } // if

    aSaveFunPara.result_Node -> left = left_node;
    aSaveFunPara.parameter_Pos.pop();
    eva_fun = false;
    int para2_pos = aSaveFunPara.parameter_Pos.front();
    TreeNode * right_node = DealParameterType( mAllFunPara[para2_pos] -> left, parameter_type, eva_fun );
    if ( eva_fun && CheckNodeIsNULL( right_node ) ) {
      UnboundParameterErrorInfo aUnboundParameterErrorInfo;
      aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para2_pos] -> left;
      throw aUnboundParameterErrorInfo;
    } // if

    aSaveFunPara.parameter_Pos.pop();
    if ( right_node -> token_data.token_type != "LEFT-PAREN" ) {
      right_node -> isStart = false;
      right_node -> isEnd = true;
    } // if
    else if ( right_node -> token_data.token_type == "LEFT-PAREN"
              && right_node -> left && right_node -> left -> token_data.token_type != "QUOTE" ) {
      right_node -> isStart = false;
      right_node -> isEnd = false;
    } // else if
    else if ( right_node -> token_data.token_type == "LEFT-PAREN"
              && right_node -> left && right_node -> left -> token_data.token_type == "QUOTE" ) {
      TreeNode * connect = ReSetLinkResult( link );
      connect -> isStart = false;
      connect -> isEnd = true;
      TreeNode * nil = NULL;
      nil = new TreeNode;
      nil -> left = NULL;
      nil -> right = NULL;
      nil -> isStart = false;
      nil -> isEnd = true;
      nil -> token_data.token_name = "nil";
      nil -> token_data.token_type = "NIL";
      connect -> left = right_node;
      connect -> right = nil;
      right_node = connect;
    } // else if

    aSaveFunPara.result_Node -> right = right_node;
    return aSaveFunPara.result_Node;
  } // EVAcons()

  TreeNode * EVAlist( SaveFunctionParameter aSaveFunPara ) {
    if ( aSaveFunPara.argument_Num == 0 ) {   // (list) �|return nil_node�C
      mAllFunPara[aSaveFunPara.fun_Pos] -> right -> isEnd = false;
      return mAllFunPara[aSaveFunPara.fun_Pos] -> right;
    } // if

    int i = 0;
    string parameter_type = "Parameter";
    string new_string = "NEW", link = "LINK";
    aSaveFunPara.result_Node = ReSetLinkResult( new_string );
    TreeNode * cur_node = aSaveFunPara.result_Node;
    TreeNode * link_node;
    bool eva_fun = false;
    while ( aSaveFunPara.argument_Num > i ) {
      int para_pos = aSaveFunPara.parameter_Pos.front();
      TreeNode * real_node = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type, eva_fun );
      if ( eva_fun && CheckNodeIsNULL( real_node ) ) {
        UnboundParameterErrorInfo aUnboundParameterErrorInfo;
        aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para_pos] -> left;
        throw aUnboundParameterErrorInfo;
      } // if

      eva_fun = false;
      if ( aSaveFunPara.argument_Num == i + 1 ) {
        if ( i == 0 ) {
          cur_node -> left = real_node;
          cur_node -> right = mAllFunPara[para_pos] -> right;
        } // if
        else {
          link_node = ReSetLinkResult( link );
          TreeNode * nil = NULL;
          nil = new TreeNode;
          nil -> left = NULL;
          nil -> right = NULL;
          nil -> isStart = false;
          nil -> isEnd = true;
          nil -> token_data.token_name = "nil";
          nil -> token_data.token_type = "NIL";
          link_node -> left = real_node;
          link_node -> right = nil;
          cur_node -> right = link_node;
        } // else

      } // if
      else {
        if ( i == 0 ) {
          cur_node -> left = real_node;
        } // if
        else {
          link_node = ReSetLinkResult( link );
          cur_node -> right = link_node;
          cur_node = cur_node -> right;
          cur_node -> left = real_node;
        } // else

      } // else

      parameter_type = "Parameter";
      aSaveFunPara.parameter_Pos.pop();
      i++;
    } // while

    return aSaveFunPara.result_Node;
  } // EVAlist()

  TreeNode * EVAquote( SaveFunctionParameter aSaveFunPara ) {
    // �^��quote�������Ĥ@�ӰѼơC
    mLevel_num--;
    int para_pos = aSaveFunPara.parameter_Pos.front();
    aSaveFunPara.parameter_Pos.pop();
    if (  mAllFunPara[para_pos] -> left -> token_data.token_type != "LEFT-PAREN" ) {
      aSaveFunPara.result_Node = mAllFunPara[para_pos] -> left;
    } // if
    else {
      aSaveFunPara.result_Node = mAllFunPara[para_pos] -> left;
      mDoingQuote = true;
    } // else

    return aSaveFunPara.result_Node;
  } // EVAquote()

  TreeNode * EVAcar_cdr( SaveFunctionParameter aSaveFunPara ) {
    string parameter_type = "Parameter";
    int para_pos = aSaveFunPara.parameter_Pos.front();
    bool eva_fun = false;
    TreeNode * part_access = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type, eva_fun );
    if ( eva_fun && CheckNodeIsNULL( part_access ) ) {
      UnboundParameterErrorInfo aUnboundParameterErrorInfo;
      aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para_pos] -> left;
      throw aUnboundParameterErrorInfo;
    } // if

    // �ˬd��part���L���~�A���~�Nthrow incorrect argument type��error�C
    if ( part_access -> token_data.token_type != "LEFT-PAREN" ) {
      ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
      aArgumentTypeErrorInfo.function_name = aSaveFunPara.fun_Name;
      aArgumentTypeErrorInfo.argumentype_error_node = part_access;
      throw aArgumentTypeErrorInfo;
    } // if
    else if ( part_access -> left -> token_data.token_type == "QUOTE" ) {
      ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
      aArgumentTypeErrorInfo.function_name = aSaveFunPara.fun_Name;
      aArgumentTypeErrorInfo.argumentype_error_node = part_access;
      throw aArgumentTypeErrorInfo;
    } // else if

    TreeNode * left_part = part_access;
    TreeNode * right_part = part_access;
    // �}�l���p��A�T�{�O�_���F�Ӽh���ѼƤ~��B��C
    // (�@) ���T�w�h�ơA�A�T�w�ѼơC
    if ( aSaveFunPara.fun_Name == "car" ) {
      left_part = left_part -> left;
      aSaveFunPara.result_Node = left_part;
    } // if
    else {
      // Preorder( right_part );
      right_part = right_part -> right;
      // cout << right_part -> token_data.token_name << right_part -> token_data.token_type << "2\n";
      if ( right_part -> isEnd ) {  // �J����I�Onil��isEnd�C
        right_part -> isStart = false;
        right_part -> isEnd = false;
      } // if
      else {
        right_part -> isStart = true;
        right_part -> isEnd = false;
      } // else

      aSaveFunPara.result_Node = right_part;
    } // else

    return aSaveFunPara.result_Node;
  } // EVAcar_cdr()

  TreeNode * EVAprimitive_predicates( SaveFunctionParameter aSaveFunPara ) {
    TreeNode * true_or_false = NULL;
    true_or_false = new TreeNode;
    true_or_false -> isStart = false;
    true_or_false -> isEnd = false;
    true_or_false -> left = NULL;
    true_or_false -> right = NULL;
    TokenData token_true_data;
    token_true_data.token_name = "#t";
    token_true_data.token_type = "T";
    TokenData token_false_data;
    token_false_data.token_name = "nil";
    token_false_data.token_type = "NIL";
    string parameter_type = "Parameter";
    int para_pos = aSaveFunPara.parameter_Pos.front();
    bool eva_fun = false;
    TreeNode * real_para = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type, eva_fun );
    if ( eva_fun && CheckNodeIsNULL( real_para ) ) {
      UnboundParameterErrorInfo aUnboundParameterErrorInfo;
      aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para_pos] -> left;
      throw aUnboundParameterErrorInfo;
    } // if

    if ( aSaveFunPara.fun_Name == "atom?" ) {
      if ( IsAtom( real_para -> token_data ) ) true_or_false -> token_data = token_true_data;
      else true_or_false -> token_data = token_false_data;
    } // if
    else if ( aSaveFunPara.fun_Name == "pair?" ) {
      if ( real_para -> token_data.token_type == "LEFT-PAREN" ) {
        true_or_false -> token_data = token_true_data;
        if ( real_para -> left -> token_data.token_type == "QUOTE" ) {
          true_or_false -> token_data = token_false_data;
        } // if
        else true_or_false -> token_data = token_true_data;
      } // if
      else {
        true_or_false -> token_data = token_false_data;
      } // else

    } // else if
    else if ( aSaveFunPara.fun_Name == "list?" ) {
      if ( real_para -> token_data.token_type == "LEFT-PAREN" ) {
        if ( real_para -> left -> token_data.token_type == "LEFT-PAREN" // ���X�Ĥ@�h���̫�O���Onon-list�C
             || real_para -> left -> token_data.token_type == "QUOTE" ) {
          true_or_false -> token_data = token_true_data;
        } // if
        else {
          while ( real_para -> isEnd == false ) {
            real_para = real_para -> right;
          } // while

          if ( real_para -> token_data.token_type != "NIL" ) true_or_false -> token_data = token_false_data;
          else true_or_false -> token_data = token_true_data;
        } // else

      } // if
      else {
        true_or_false -> token_data = token_false_data;
      } // else

    } // else if
    else if ( aSaveFunPara.fun_Name == "null?" ) {
      if ( real_para -> token_data.token_type == "NIL" ) true_or_false -> token_data = token_true_data;
      else true_or_false -> token_data = token_false_data;
    } // else if
    else if ( aSaveFunPara.fun_Name == "integer?" ) {
      if ( real_para -> token_data.token_type == "INT" ) true_or_false -> token_data = token_true_data;
      else true_or_false -> token_data = token_false_data;
    } // else if
    else if ( aSaveFunPara.fun_Name == "real?" || aSaveFunPara.fun_Name == "number?" ) {
      if ( real_para -> token_data.token_type == "INT"
           || real_para -> token_data.token_type == "FLOAT" ) true_or_false -> token_data = token_true_data;
      else true_or_false -> token_data = token_false_data;
    } // else if
    else if ( aSaveFunPara.fun_Name == "string?" ) {
      if ( real_para -> token_data.token_type == "STRING" ) true_or_false -> token_data = token_true_data;
      else true_or_false -> token_data = token_false_data;
    } // else if
    else if ( aSaveFunPara.fun_Name == "boolean?" ) {
      if ( real_para -> token_data.token_type == "T"
           || real_para -> token_data.token_type == "NIL" ) true_or_false -> token_data = token_true_data;
      else true_or_false -> token_data = token_false_data;
    } // else if
    else if ( aSaveFunPara.fun_Name == "symbol?" ) {
      if ( real_para -> token_data.token_type == "SYMBOL" ) true_or_false -> token_data = token_true_data;
      else true_or_false -> token_data = token_false_data;
    } // else if

    aSaveFunPara.result_Node = true_or_false;
    return aSaveFunPara.result_Node;
  } // EVAprimitive_predicates()

  TreeNode * EVAnumber_arithmetic( SaveFunctionParameter aSaveFunPara ) {
    TreeNode * number_eva = NULL;
    number_eva = new TreeNode;
    number_eva -> left = NULL;
    number_eva -> right = NULL;
    number_eva -> isEnd = false;
    number_eva -> isStart = false;
    TokenData token_true_data;
    token_true_data.token_name = "";
    token_true_data.token_type = "";
    string parameter_type = "Parameter";
    int i = 0;
    float eva1_float = 0, eva2_float = 0;
    bool alwaysInt = true;
    int para1_pos = aSaveFunPara.parameter_Pos.front();
    bool eva_fun = false;
    TreeNode * number_1_node = DealParameterType( mAllFunPara[para1_pos] -> left, parameter_type, eva_fun );
    if ( eva_fun && CheckNodeIsNULL( number_1_node ) ) {
      UnboundParameterErrorInfo aUnboundParameterErrorInfo;
      aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para1_pos] -> left;
      throw aUnboundParameterErrorInfo;
    } // if

    aSaveFunPara.parameter_Pos.pop();
    i++;
    if ( number_1_node -> token_data.token_type == "INT"
         || number_1_node -> token_data.token_type == "FLOAT" ) {
      if ( number_1_node -> token_data.token_type == "FLOAT" ) {
        eva1_float = atof( number_1_node -> token_data.token_name.c_str() );
        alwaysInt = false;
      } // if
      else {
        eva1_float = atoi( number_1_node -> token_data.token_name.c_str() );
      } // else

    } // if
    else {
      ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
      aArgumentTypeErrorInfo.function_name = aSaveFunPara.fun_Name;
      aArgumentTypeErrorInfo.argumentype_error_node = number_1_node;
      throw aArgumentTypeErrorInfo;
    } // else

    TreeNode * number_2_node = NULL;
    while ( aSaveFunPara.argument_Num > i ) {
      eva_fun = false;
      parameter_type = "Parameter";
      int para2_pos = aSaveFunPara.parameter_Pos.front();
      number_2_node = DealParameterType( mAllFunPara[para2_pos] -> left, parameter_type, eva_fun );
      if ( eva_fun && CheckNodeIsNULL( number_2_node ) ) {
        UnboundParameterErrorInfo aUnboundParameterErrorInfo;
        aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para2_pos] -> left;
        throw aUnboundParameterErrorInfo;
      } // if

      aSaveFunPara.parameter_Pos.pop();
      i++;
      if ( number_2_node -> token_data.token_type == "INT"
           || number_2_node -> token_data.token_type == "FLOAT" ) {
        if ( number_2_node -> token_data.token_type == "FLOAT" ) {
          eva2_float = atof( number_2_node -> token_data.token_name.c_str() );
          alwaysInt = false;
        } // if
        else {
          eva2_float = atoi( number_2_node -> token_data.token_name.c_str() );
        } // else

      } // if
      else {
        ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
        aArgumentTypeErrorInfo.function_name = aSaveFunPara.fun_Name;
        aArgumentTypeErrorInfo.argumentype_error_node = number_2_node;
        throw aArgumentTypeErrorInfo;
      } // else

      // �T�{��̼Ʀr�ҥ��T����A�ন�i�H�B�⪺�榡(int�Bfloat)�C
      if ( aSaveFunPara.fun_Name == "+" ) {
        eva1_float = eva1_float + eva2_float;
      } // if
      else if ( aSaveFunPara.fun_Name == "-" ) {
        eva1_float = eva1_float - eva2_float;
      } // else if
      else if ( aSaveFunPara.fun_Name == "*" ) {
        eva1_float = eva1_float * eva2_float;
      } // else if
      else {  // �ѡi���j
        if ( eva2_float == 0 ) {
          DivisionByZero aDivisionByZero;
          aDivisionByZero.error_name = "ERROR (division by zero) : /";
          throw aDivisionByZero;
        } // if
        else eva1_float = eva1_float / eva2_float;
      } // else

    } // while

    if ( alwaysInt ) {
      stringstream ss;
      eva1_float = ( int ) eva1_float;
      ss << eva1_float;
      ss >> number_eva -> token_data.token_name;
      number_eva -> token_data.token_type = "INT";
    } // if
    else {
      stringstream ss;
      ss << eva1_float;
      ss >> number_eva -> token_data.token_name;
      number_eva -> token_data.token_type = "FLOAT";
    } // else

    aSaveFunPara.result_Node = number_eva;
    return aSaveFunPara.result_Node;
  } // EVAnumber_arithmetic()

  TreeNode * EVAnumber_string_compare( SaveFunctionParameter aSaveFunPara ) {
    TreeNode * true_or_false = NULL;
    true_or_false = new TreeNode;
    true_or_false -> isStart = false;
    true_or_false -> isEnd = false;
    true_or_false -> left = NULL;
    true_or_false -> right = NULL;
    TokenData token_true_data;
    token_true_data.token_name = "#t";
    token_true_data.token_type = "T";
    TokenData token_false_data;
    token_false_data.token_name = "nil";
    token_false_data.token_type = "NIL";
    int i = 0;
    float eva1_float = 0, eva2_float = 0;
    bool eva_fun = false;
    string eva1_string = "", eva2_string = "";
    string parameter_type = "Parameter";
    if ( aSaveFunPara.fun_Type == "Number compare" ) {
      int para1_pos = aSaveFunPara.parameter_Pos.front();
      TreeNode * number_1_node = NULL;
      number_1_node = DealParameterType( mAllFunPara[para1_pos] -> left, parameter_type, eva_fun );
      if ( eva_fun && CheckNodeIsNULL( number_1_node ) ) {
        UnboundParameterErrorInfo aUnboundParameterErrorInfo;
        aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para1_pos] -> left;
        throw aUnboundParameterErrorInfo;
      } // if

      aSaveFunPara.parameter_Pos.pop();
      i++;
      if ( number_1_node -> token_data.token_type == "INT"
           || number_1_node -> token_data.token_type == "FLOAT" ) {
        eva1_float = atof( number_1_node -> token_data.token_name.c_str() );
      } // if
      else {
        ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
        aArgumentTypeErrorInfo.function_name = aSaveFunPara.fun_Name;
        aArgumentTypeErrorInfo.argumentype_error_node = number_1_node;
        throw aArgumentTypeErrorInfo;
      } // else

      bool isUnFinish = true;
      TreeNode * number_2_node = NULL;
      while ( aSaveFunPara.argument_Num > i ) {
        eva_fun = false;
        parameter_type = "Parameter";
        int para2_pos = aSaveFunPara.parameter_Pos.front();
        number_2_node = DealParameterType( mAllFunPara[para2_pos] -> left, parameter_type, eva_fun );
        if ( eva_fun && CheckNodeIsNULL( number_2_node ) ) {
          UnboundParameterErrorInfo aUnboundParameterErrorInfo;
          aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para2_pos] -> left;
          throw aUnboundParameterErrorInfo;
        } // if

        aSaveFunPara.parameter_Pos.pop();
        i++;
        if ( number_2_node -> token_data.token_type == "INT"
             || number_2_node -> token_data.token_type == "FLOAT" ) {
          eva2_float = atof( number_2_node -> token_data.token_name.c_str() );
        } // if
        else {
          ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
          aArgumentTypeErrorInfo.function_name = aSaveFunPara.fun_Name;
          aArgumentTypeErrorInfo.argumentype_error_node = number_2_node;
          throw aArgumentTypeErrorInfo;
        } // else

        if ( aSaveFunPara.fun_Name == ">" && isUnFinish ) {
          if ( eva1_float <= eva2_float ) {
            isUnFinish = false;
          } // if

        } // if
        else if ( aSaveFunPara.fun_Name == ">=" && isUnFinish ) {
          if ( eva1_float < eva2_float ) isUnFinish = false;
        } // else if
        else if ( aSaveFunPara.fun_Name == "<" && isUnFinish ) {
          if ( eva1_float >= eva2_float ) isUnFinish = false;
        } // else if
        else if ( aSaveFunPara.fun_Name == "<=" && isUnFinish ) {
          if ( eva1_float > eva2_float ) isUnFinish = false;
        } // else if
        else if ( aSaveFunPara.fun_Name == "=" && isUnFinish ) {
          if ( eva1_float != eva2_float ) isUnFinish = false;
        } // else if

        eva1_float = eva2_float;
      } // while

      if ( isUnFinish ) {
        true_or_false -> token_data = token_true_data;
        aSaveFunPara.result_Node = true_or_false;
      } // if
      else {
        true_or_false -> token_data = token_false_data;
        aSaveFunPara.result_Node = true_or_false;
      } // else

    } // if
    else {  // fun_Type = "String compare"
      int para1_pos = aSaveFunPara.parameter_Pos.front();
      TreeNode * string_1_node = NULL;
      string_1_node  = DealParameterType( mAllFunPara[para1_pos] -> left, parameter_type, eva_fun );
      if ( eva_fun && CheckNodeIsNULL( string_1_node ) ) {
        UnboundParameterErrorInfo aUnboundParameterErrorInfo;
        aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para1_pos] -> left;
        throw aUnboundParameterErrorInfo;
      } // if

      aSaveFunPara.parameter_Pos.pop();
      i++;
      if ( string_1_node -> token_data.token_type == "STRING" ) {
        eva1_string = string_1_node -> token_data.token_name;
      } // if
      else {
        ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
        aArgumentTypeErrorInfo.function_name = aSaveFunPara.fun_Name;
        aArgumentTypeErrorInfo.argumentype_error_node = string_1_node;
        throw aArgumentTypeErrorInfo;
      } // else

      bool isUnFinish = true;
      TreeNode * string_2_node = NULL;
      while ( aSaveFunPara.argument_Num > i ) {
        parameter_type = "Parameter";
        eva_fun = false;
        int para2_pos = aSaveFunPara.parameter_Pos.front();
        string_2_node = DealParameterType( mAllFunPara[para2_pos] -> left, parameter_type, eva_fun );
        if ( eva_fun && CheckNodeIsNULL( string_2_node ) ) {
          UnboundParameterErrorInfo aUnboundParameterErrorInfo;
          aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para2_pos] -> left;
          throw aUnboundParameterErrorInfo;
        } // if

        aSaveFunPara.parameter_Pos.pop();
        i++;
        if ( string_2_node -> token_data.token_type == "STRING" ) {
          eva2_string = string_2_node -> token_data.token_name;
        } // if
        else {
          ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
          aArgumentTypeErrorInfo.function_name = aSaveFunPara.fun_Name;
          aArgumentTypeErrorInfo.argumentype_error_node = string_2_node;
          throw aArgumentTypeErrorInfo;
        } // else

        if ( aSaveFunPara.fun_Name == "string-append" && isUnFinish ) {
          eva1_string.erase( eva1_string.size() - 1, 1 );
          eva2_string.erase( 0, 1 );
          eva1_string = eva1_string + eva2_string;
        } // if
        else if ( aSaveFunPara.fun_Name == "string>?" && isUnFinish ) {
          if ( eva1_string <= eva2_string ) isUnFinish = false;
          eva1_string = eva2_string;
        } // else if
        else if ( aSaveFunPara.fun_Name == "string<?" && isUnFinish ) {
          if ( eva1_string >= eva2_string ) isUnFinish = false;
          eva1_string = eva2_string;
        } // else if
        else if ( aSaveFunPara.fun_Name == "string=?" && isUnFinish ) {
          if ( eva1_string != eva2_string ) isUnFinish = false;
        } // else if

      } // while

      if ( aSaveFunPara.fun_Name == "string-append" ) {
        TokenData token_string_data;
        token_string_data.token_name = eva1_string;
        token_string_data.token_type = "STRING";
        true_or_false -> token_data = token_string_data;
        aSaveFunPara.result_Node = true_or_false;
      } // if
      else if ( isUnFinish ) {
        true_or_false -> token_data = token_true_data;
        aSaveFunPara.result_Node = true_or_false;
      } // else if
      else {
        true_or_false -> token_data = token_false_data;
        aSaveFunPara.result_Node = true_or_false;
      } // else

    } // else

    return aSaveFunPara.result_Node;
  } // EVAnumber_string_compare()

  TreeNode * EVAlogical( SaveFunctionParameter aSaveFunPara ) {
    TreeNode * logical_node = NULL;
    logical_node = new TreeNode;
    logical_node -> isStart = false;
    logical_node -> isEnd = false;
    logical_node -> left = NULL;
    logical_node -> right = NULL;
    TreeNode * result_node = NULL;
    TreeNode * para_node = NULL;
    TokenData token_true_data;
    token_true_data.token_name = "#t";
    token_true_data.token_type = "T";
    TokenData token_false_data;
    token_false_data.token_name = "nil";
    token_false_data.token_type = "NIL";
    int i = 0;
    bool alwaysTrue = true;
    bool unfinished = true;
    bool eva_fun = false;
    int para_pos = 0;
    string parameter_type = "Parameter";
    while ( aSaveFunPara.argument_Num > i && unfinished ) {
      parameter_type = "Parameter";
      eva_fun = false;
      para_pos = aSaveFunPara.parameter_Pos.front();
      para_node = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type, eva_fun );
      if ( eva_fun && CheckNodeIsNULL( para_node ) ) {
        UnboundConditionErrorInfo aUnboundConditionErrorInfo;
        aUnboundConditionErrorInfo.unbound_Condition_node = mAllFunPara[para_pos] -> left;
        throw aUnboundConditionErrorInfo;
      } // if

      aSaveFunPara.parameter_Pos.pop();
      i++;
      if ( para_node -> token_data.token_type == "NIL" ) {
        alwaysTrue = false;
      } // if
      else {
        alwaysTrue = true;
      } // else

      if ( aSaveFunPara.fun_Name == "or" ) {
        if ( !alwaysTrue ) {
          result_node = para_node;
        } // if
        else {
          result_node = para_node;
          unfinished = false;
        } // else

      } // if
      else if ( aSaveFunPara.fun_Name == "and" ) {
        if ( alwaysTrue ) {
          result_node = para_node;
        } // if
        else {
          result_node = para_node;
          unfinished = false;
        } // else

      } // else if

    } // while

    if ( aSaveFunPara.fun_Name == "not" ) {
      if ( alwaysTrue ) {
        logical_node -> token_data = token_false_data;
      } // if
      else {
        logical_node -> token_data = token_true_data;
      } // else

      aSaveFunPara.result_Node = logical_node;
    } // if
    else {
      aSaveFunPara.result_Node = result_node;
      if ( eva_fun && CheckNodeIsNULL( aSaveFunPara.result_Node ) ) {
        UnboundConditionErrorInfo aUnboundConditionErrorInfo;
        aUnboundConditionErrorInfo.unbound_Condition_node = mAllFunPara[para_pos] -> left;
        throw aUnboundConditionErrorInfo;
      } // if

    } // else

    return aSaveFunPara.result_Node;
  } // EVAlogical()

  TreeNode * EVAequal_eqv( SaveFunctionParameter aSaveFunPara ) {
    TreeNode * equal_eqv_node = NULL;
    equal_eqv_node = new TreeNode;
    equal_eqv_node -> isStart = false;
    equal_eqv_node -> isEnd = false;
    equal_eqv_node -> left = NULL;
    equal_eqv_node -> right = NULL;
    TokenData token_true_data;
    token_true_data.token_name = "#t";
    token_true_data.token_type = "T";
    TokenData token_false_data;
    token_false_data.token_name = "nil";
    token_false_data.token_type = "NIL";
    string parameter_type_1 = "Eq";
    int para1_pos = aSaveFunPara.parameter_Pos.front();
    bool eva_fun = false;
    TreeNode * para1_node = DealParameterType( mAllFunPara[para1_pos] -> left, parameter_type_1, eva_fun );
    if ( eva_fun && CheckNodeIsNULL( para1_node ) ) {
      UnboundParameterErrorInfo aUnboundParameterErrorInfo;
      aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para1_pos] -> left;
      throw aUnboundParameterErrorInfo;
    } // if

    aSaveFunPara.parameter_Pos.pop();
    string parameter_type_2 = "Eq";
    eva_fun = false;
    int para2_pos = aSaveFunPara.parameter_Pos.front();
    TreeNode * para2_node = DealParameterType( mAllFunPara[para2_pos] -> left, parameter_type_2, eva_fun );
    if ( eva_fun && CheckNodeIsNULL( para2_node ) ) {
      UnboundParameterErrorInfo aUnboundParameterErrorInfo;
      aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para2_pos] -> left;
      throw aUnboundParameterErrorInfo;
    } // if

    aSaveFunPara.parameter_Pos.pop();
    if ( aSaveFunPara.fun_Name == "equal?" ) {
      if ( Compare_TwoTree_Data( para1_node, para2_node ) ) {
        equal_eqv_node -> token_data = token_true_data;
      } // if
      else {
        equal_eqv_node -> token_data = token_false_data;
      } // else

    } // if
    else {
      if ( IsAtom( para1_node -> token_data ) && IsAtom( para2_node -> token_data ) ) {
        if ( para1_node -> token_data.token_name == para2_node -> token_data.token_name ) {
          if ( para1_node -> token_data.token_type != "STRING"
               && para2_node -> token_data.token_type != "STRING" ) {
            equal_eqv_node -> token_data = token_true_data;
          } // if
          else {
            if ( para1_node == para2_node ) {
              equal_eqv_node -> token_data = token_true_data;
            } // if
            else {
              equal_eqv_node -> token_data = token_false_data;
            } // else

          } // else

        } // if
        else {
          equal_eqv_node -> token_data = token_false_data;
        } // else

      } // if
      else {  // �p�G��̦��@�̤��OAtom�C
        if ( para1_node == para2_node ) {
          equal_eqv_node -> token_data = token_true_data;
        } // if
        else {
          equal_eqv_node -> token_data = token_false_data;
        } // else

      } // else

    } // else

    aSaveFunPara.result_Node = equal_eqv_node;
    return aSaveFunPara.result_Node;
  } // EVAequal_eqv()

  TreeNode * EVAif( SaveFunctionParameter aSaveFunPara ) {
    TreeNode * result_node = NULL;
    string parameter_type = "Parameter";
    int para1_pos = aSaveFunPara.parameter_Pos.front();
    bool eva_fun = false;
    TreeNode * left_or_right_node = NULL;
    left_or_right_node = DealParameterType( mAllFunPara[para1_pos] -> left, parameter_type, eva_fun );
    aSaveFunPara.parameter_Pos.pop();
    bool isNIL = false;
    if ( eva_fun && CheckNodeIsNULL( left_or_right_node ) ) {
      UnboundTestConditionErrorInfo aUnboundTestConditionErrorInfo;
      aUnboundTestConditionErrorInfo.unbound_Testcondition_node = mAllFunPara[para1_pos] -> left;
      throw aUnboundTestConditionErrorInfo;
    } // if
    else if ( left_or_right_node -> token_data.token_type == "NIL" ) { // true
      isNIL = true;
    } // else if

    // �T�w���Ĥ@�ӰѼƪ����O����A�}�l�i��!!
    if ( isNIL ) {
      if ( aSaveFunPara.argument_Num == 2 ) {
        // NoReturnValueErrorInfo aNoReturnValueErrorInfo;
        // aNoReturnValueErrorInfo.no_return_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
        // throw aNoReturnValueErrorInfo;
        result_node = NULL;
      } // if
      else {
        // �}�l�B�z�ĤT�ӰѼ�
        aSaveFunPara.parameter_Pos.pop(); // (���L�ĤG�ӰѼ�)
        int para3_pos = aSaveFunPara.parameter_Pos.front();
        result_node = DealParameterType( mAllFunPara[para3_pos] -> left, parameter_type, eva_fun );
        aSaveFunPara.parameter_Pos.pop();
      } // else

    } // if
    else {
      // �}�l�B�ztrue�A�i���(���L�ĤT�ӰѼơA)
      int para2_pos = aSaveFunPara.parameter_Pos.front();
      result_node = DealParameterType( mAllFunPara[para2_pos] -> left, parameter_type, eva_fun );
      aSaveFunPara.parameter_Pos.pop();
    } // else

    aSaveFunPara.result_Node = result_node;
    return aSaveFunPara.result_Node;
  } // EVAif()

  TreeNode * EVAcond( SaveFunctionParameter aSaveFunPara ) {
    TreeNode * result_node = NULL;
    string parameter_type = "Cond";
    int i = 0;
    queue<TreeNode*> real_para;
    FormatErrorInfo aFormatErrorInfo;
    aFormatErrorInfo.format_fun_name = "COND";
    aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
    // (�@) ���T�{��ѼƮ榡�O���T���A�A���U�i��B��(���`���A�A�T�{�u��)�C
    // (�G) �B��ɡA�p�Gnil�S�o��ѼơA�N�^��no_return_error�C
    // (�T) �ӦA�i��B��ɡA�Ĥ@�ӭȪ�symnol�Sbound�άOsystem �N�oerror�C
    if ( aSaveFunPara.argument_Num == 0 ) {
      throw aFormatErrorInfo;
    } // if

    while ( aSaveFunPara.argument_Num > i ) { // �Ĥ@�h���n�O()�s�b���Aatom quote����gg�C
      int para_pos = aSaveFunPara.parameter_Pos.front();
      if ( mAllFunPara[para_pos] -> left
           && mAllFunPara[para_pos] -> left -> token_data.token_type == "LEFT-PAREN" ) {
        if ( mAllFunPara[para_pos] -> left -> left
             && mAllFunPara[para_pos] -> left -> left -> token_data.token_type != "QUOTE" ) {
          // �A���̥u��C��@�ӰѼ�
          if ( mAllFunPara[para_pos] -> left -> right
               && mAllFunPara[para_pos] -> left -> right -> token_data.token_type == "LEFT-PAREN" ) {
            real_para.push( mAllFunPara[para_pos] -> left );
          } // if
          else {
            throw aFormatErrorInfo;
          } // else

        } // if
        else {
          throw aFormatErrorInfo;
        } // else

      } // if
      else {
        throw aFormatErrorInfo;
      } // else

      aSaveFunPara.parameter_Pos.pop();
      i++;
    } // while

    queue<TreeNode*> check_dot_queue = real_para;
    i = 0;
    while ( aSaveFunPara.argument_Num > i ) {
      TreeNode * check_dot = check_dot_queue.front();
      while ( check_dot -> isEnd == false ) {
        check_dot = check_dot -> right;
      } // while

      if ( check_dot -> token_data.token_type != "NIL" ) {
        throw aFormatErrorInfo;
      } // if

      check_dot_queue.pop();
      i++;
    } // while

    TreeNode * fake_node = NULL;
    i = 0;
    bool alwaysTrue = false;
    while ( aSaveFunPara.argument_Num > i && !alwaysTrue ) {
      fake_node = real_para.front();
      int start_eva = 0;
      bool thisTrue = false, falseNoRead = false;
      if ( aSaveFunPara.argument_Num == i + 1 ) parameter_type = "Cond-LastElse";
      while ( fake_node -> isEnd == false && !falseNoRead ) {
        bool eva_fun = false;
        TreeNode * real_one_node = DealParameterType( fake_node -> left, parameter_type, eva_fun );
        if ( start_eva == 0 ) {  // �u����Ĥ@�ӰѼƭp�⪺�ɭԡA�~�i�H�M�w��()�����V
          if ( eva_fun && CheckNodeIsNULL( real_one_node ) ) {
            UnboundTestConditionErrorInfo aUnboundTestConditionErrorInfo;
            aUnboundTestConditionErrorInfo.unbound_Testcondition_node = fake_node -> left;
            throw aUnboundTestConditionErrorInfo;
          } // if

          if ( real_one_node -> token_data.token_type != "NIL" ) { // �p�G���O�_�w���ܡC
            thisTrue = true;
          } // if
          else {
            falseNoRead = true;
          } // else

          string procdeure_name = real_one_node -> token_data.token_name;
          string procdeure_type = real_one_node -> token_data.token_type;
          if ( parameter_type == "Cond-LastElse" && real_one_node -> token_data.token_name == "else" ) {

          } // if
          else if ( !alwaysTrue && thisTrue ) {
            result_node = real_one_node;
          } // else if

        } // if
        else {
          if ( !alwaysTrue && thisTrue ) result_node = real_one_node;
        } // else

        fake_node = fake_node -> right;
        parameter_type = "Cond";
        start_eva++;
      } // while

      if ( thisTrue ) alwaysTrue = true;
      real_para.pop();
      i++;
    } // while

    if ( result_node == NULL ) {
      aSaveFunPara.result_Node = NULL;
    } // if
    else {
      aSaveFunPara.result_Node = result_node;
    } // else

    return aSaveFunPara.result_Node;
  } // EVAcond()

  TreeNode * EVAbegin( SaveFunctionParameter aSaveFunPara ) {
    TreeNode * the_last_node = NULL;
    int i = 0;
    bool eva_fun = false;
    while ( aSaveFunPara.argument_Num > i ) {
      string parameter_type = "Parameter";
      int para_pos = aSaveFunPara.parameter_Pos.front();
      the_last_node = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type, eva_fun );
      aSaveFunPara.parameter_Pos.pop();
      i++;
    } // while

    aSaveFunPara.result_Node = the_last_node;
    return aSaveFunPara.result_Node;
  } // EVAbegin()

  TreeNode * EVAlet( SaveFunctionParameter aSaveFunPara ) {
    if ( aSaveFunPara.argument_Num >= 2 ) {
      DefineTable newDefineTable;
      newDefineTable.user_define_name = "";
      newDefineTable.user_define_type = "";
      UserFunctionInfo newUserFunctionInfo;
      newUserFunctionInfo.function_type = "LET";
      TreeNode * para1_node = NULL;
      string parameter_type = "Parameter";
      int para1_pos = aSaveFunPara.parameter_Pos.front();
      para1_node = mAllFunPara[para1_pos] -> left;
      aSaveFunPara.parameter_Pos.pop();
      bool eva_fun = false;
      vector<TreeNode*> pair_node;
      TreeNode * is_Pair_node = NULL;
      // �~�ӫe�H���N�ӡC
      int repeat_fun_pos = -1, repeat_info_pos = -1;
      if ( mUserFunctionInfo.size() > 0 ) {
        newUserFunctionInfo = mUserFunctionInfo[mUserFunctionInfo.size()-1];
        int connect_function = mUserFunctionInfo[mUserFunctionInfo.size()-1].function_pos;
        newDefineTable = gDefineTable[connect_function];
      } // if

      newUserFunctionInfo.function_type = "LET";
      newDefineTable.user_define_type = "LET";
      if ( !IsList( para1_node ) ) {
        if ( para1_node -> token_data.token_type != "NIL" ) {
          FormatErrorInfo aFormatErrorInfo;
          aFormatErrorInfo.format_fun_name = "LET";
          aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
          throw aFormatErrorInfo;
        } // if

      } // if
      else {
        // ���ˬdformat�藍��A���T�w��H��~��i��B��A�Q��
        is_Pair_node = NULL;
        is_Pair_node = para1_node -> left;
        while ( para1_node -> isEnd == false ) {
          eva_fun = false;
          if ( IsPair( is_Pair_node ) ) {
            if ( is_Pair_node -> left -> token_data.token_type == "SYMBOL"
                 && !Is_SystemSymbol( is_Pair_node -> left -> token_data.token_name )
                 && is_Pair_node -> right -> token_data.token_type == "LEFT-PAREN"
                 && is_Pair_node -> right -> right
                 && is_Pair_node -> right -> right -> token_data.token_type == "NIL" ) {
              pair_node.push_back( is_Pair_node );
            } // if
            else {
              FormatErrorInfo aFormatErrorInfo;
              aFormatErrorInfo.format_fun_name = "LET";
              aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
              throw aFormatErrorInfo;
            } // else

          } // if
          else {
            FormatErrorInfo aFormatErrorInfo;
            aFormatErrorInfo.format_fun_name = "LET";
            aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
            throw aFormatErrorInfo;
          } // else

          para1_node = para1_node -> right;
          is_Pair_node = para1_node -> left;
          parameter_type = "Parameter";
        } // while

      } // else

      // �}�l�i��B��C
      int i = 0;
      while ( pair_node.size() > i ) {
        is_Pair_node = pair_node[i];
        int repeat_pos = -1;
        TreeNode * temp_function = NULL;
        if ( IsSameSymbol_In_Argument( is_Pair_node -> left -> token_data,
                                       newDefineTable.fun_argument, repeat_pos ) ) {
          newDefineTable.fun_argument[repeat_pos] = is_Pair_node -> left;
          is_Pair_node = is_Pair_node -> right;
          temp_function = DealParameterType( is_Pair_node -> left,  parameter_type, eva_fun );
          newUserFunctionInfo.real_para[repeat_pos] = temp_function;
        } // if
        else {
          newDefineTable.fun_argument.push_back( is_Pair_node -> left );
          is_Pair_node = is_Pair_node -> right;
          temp_function = DealParameterType( is_Pair_node -> left,  parameter_type, eva_fun );
          newUserFunctionInfo.real_para.push_back( temp_function );
        } // else

        if ( eva_fun && CheckNodeIsNULL( temp_function ) ) {
          NoReturnValueErrorInfo aNoReturnValueErrorInfo;
          aNoReturnValueErrorInfo.no_return_error_node = is_Pair_node -> left;
          throw aNoReturnValueErrorInfo;
        } // if

        i++;
        eva_fun = false;
        parameter_type = "Parameter";
      } // while

      gDefineTable.push_back( newDefineTable );
      int define_table_pos = gDefineTable.size() - 1;
      newUserFunctionInfo.function_pos = define_table_pos;
      mUserFunctionInfo.push_back( newUserFunctionInfo );

      i = 1;
      while ( aSaveFunPara.argument_Num > i ) {
        int some_para_pos = aSaveFunPara.parameter_Pos.front();
        TreeNode * some_node = mAllFunPara[some_para_pos] -> left;
        aSaveFunPara.result_Node = DealParameterType( some_node, parameter_type, eva_fun );
        aSaveFunPara.parameter_Pos.pop();
        parameter_type = "Parameter";
        i++;
      } // while

      mUserFunctionInfo.pop_back();
      gDefineTable.erase( gDefineTable.begin() + define_table_pos );
    } // if
    else {
      FormatErrorInfo aFormatErrorInfo;
      aFormatErrorInfo.format_fun_name = "LET";
      aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
      throw aFormatErrorInfo;
    } // else

    return aSaveFunPara.result_Node;
  } // EVAlet()

  TreeNode * EVAuser_Function( int user_function_pos, SaveFunctionParameter aSaveFunPara ) {
    UserFunctionInfo aUserFunctionInfo;
    TreeNode * real_para_node = NULL;
    TreeNode * real_fun_node = NULL;
    int i = 0;
    string parameter_type = "Parameter";
    bool eva_fun = false;

    // ���q�n�a�J���ѼƤU��(�ˬd)�C
    while ( aSaveFunPara.argument_Num > i ) {
      int para_pos = aSaveFunPara.parameter_Pos.front();
      real_para_node = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type, eva_fun );
      aUserFunctionInfo.real_para.push_back( real_para_node );
      aSaveFunPara.parameter_Pos.pop();
      if ( eva_fun && CheckNodeIsNULL( real_para_node ) ) {
        UnboundParameterErrorInfo aUnboundParameterErrorInfo;
        aUnboundParameterErrorInfo.unbound_Parameter_node = mAllFunPara[para_pos] -> left;
        throw aUnboundParameterErrorInfo;
      } // if

      parameter_type = "Parameter";
      eva_fun = false;
      i++;
    } // while

    // �{�b�}�l�i��function���B��C
    aUserFunctionInfo.function_pos = user_function_pos;
    mUserFunctionInfo.push_back( aUserFunctionInfo );

    i = 0;
    while ( gDefineTable[user_function_pos].eva_function.size() > i ) {
      TreeNode * eva_user_function = gDefineTable[user_function_pos].eva_function[i];
      real_fun_node = Return_NewEval_Tree( eva_user_function, real_fun_node );
      aSaveFunPara.result_Node = DealParameterType( real_fun_node, parameter_type, eva_fun );
      parameter_type = "Parameter";
      eva_fun = false;
      i++;
    } // while

    mUserFunctionInfo.pop_back();
    return aSaveFunPara.result_Node;
  } // EVAuser_Function()

  TreeNode * EVAlambda( SaveFunctionParameter aSaveFunPara ) {
    // ���ˬd�Ĥ@�ӬO���Olist�C
    if ( aSaveFunPara.argument_Num >= 2 ) {
      DefineTable newDefineTable;
      newDefineTable.user_define_name = "";
      newDefineTable.user_define_type = "Lambda-Function";
      int argument_check = 0;
      TreeNode * para1_node = NULL;
      int para1_pos = aSaveFunPara.parameter_Pos.front();
      para1_node = mAllFunPara[para1_pos] -> left;
      aSaveFunPara.parameter_Pos.pop();
      if ( !IsList( para1_node ) ) {
        if ( para1_node -> token_data.token_type != "NIL" ) {
          FormatErrorInfo aFormatErrorInfo;
          aFormatErrorInfo.format_fun_name = "LAMBDA";
          aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
          throw aFormatErrorInfo;
        } // if

      } // if
      else {
        TreeNode * is_Symbol_node = NULL;
        while ( para1_node -> isEnd == false ) {
          is_Symbol_node = para1_node -> left;
          if ( is_Symbol_node -> token_data.token_type == "SYMBOL" ) {
            newDefineTable.fun_argument.push_back( is_Symbol_node );
            argument_check++;
          } // if
          else {
            FormatErrorInfo aFormatErrorInfo;
            aFormatErrorInfo.format_fun_name = "LAMBDA";
            aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
            throw aFormatErrorInfo;
          } // else

          para1_node = para1_node -> right;
        } // while

      } // else

      int i = 1;  //  �ĤG�ѼƤ��ᬰfunction body
      TreeNode * para_many_node = NULL;
      while ( aSaveFunPara.argument_Num > i ) {
        int para_many_pos = aSaveFunPara.parameter_Pos.front();
        para_many_node = mAllFunPara[para_many_pos] -> left;
        newDefineTable.eva_function.push_back( para_many_node );
        aSaveFunPara.parameter_Pos.pop();
        i++;
      } // while

      aSaveFunPara.result_Node = CreateProcedureNode( aSaveFunPara.fun_Name );
      newDefineTable.define_value = aSaveFunPara.result_Node;
      gDefineTable.push_back( newDefineTable );
    } // if
    else {  // aSaveFunPara.argument_Num < 2
      FormatErrorInfo aFormatErrorInfo;
      aFormatErrorInfo.format_fun_name = "LAMBDA";
      aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
      throw aFormatErrorInfo;
    } // else

    return aSaveFunPara.result_Node;
  } // EVAlambda()

  void EVAdefine_Type1( SaveFunctionParameter aSaveFunPara ) {
    DefineTable newDefineTable;
    newDefineTable.user_define_name = "";
    newDefineTable.user_define_type = "";
    newDefineTable.define_value = NULL;
    string check_system_name = "";
    string check_system_type = "";
    int i = 0;
    if ( aSaveFunPara.argument_Num == 2 ) {
      TreeNode * para1_node = NULL;
      int para1_pos = aSaveFunPara.parameter_Pos.front();
      para1_node = mAllFunPara[para1_pos] -> left;
      aSaveFunPara.parameter_Pos.pop();
      check_system_name = para1_node -> token_data.token_name;
      check_system_type = para1_node -> token_data.token_type;
      if ( para1_node -> token_data.token_type != "SYMBOL" ) {
        FormatErrorInfo aFormatErrorInfo;
        aFormatErrorInfo.format_fun_name = "DEFINE";
        aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
        throw aFormatErrorInfo;
      } // if
      else if ( Is_SystemSymbol( check_system_name ) ) {
        FormatErrorInfo aFormatErrorInfo;
        aFormatErrorInfo.format_fun_name = "DEFINE";
        aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
        throw aFormatErrorInfo;
      } // else if

      newDefineTable.user_define_name = para1_node -> token_data.token_name;
      int repeat_para1_pos = -1;
      string repeat_function_type = ""; // ���ˬd���S���Q�w�q�L
      while ( gDefineTable.size() > i ) {
        if ( gDefineTable[i].user_define_name == newDefineTable.user_define_name ) {
          repeat_para1_pos = i;
          if ( gDefineTable[i].user_define_type == "Parameter" ) {
            repeat_function_type = "Parameter";
          } // if
          else if ( gDefineTable[i].user_define_type == "Define-Function" ) {
            repeat_function_type = "Define-Function";
          } // else if
          else if ( gDefineTable[i].user_define_type == "Lambda-Function" ) {
            repeat_function_type = "Lambda-Function";
          } // else if

        } // if

        i++;
      } // while

      TreeNode * para2_node = NULL;
      int para2_pos = aSaveFunPara.parameter_Pos.front();
      aSaveFunPara.parameter_Pos.pop();
      string parameter_type = "Define";
      bool eva_fun = false;
      para2_node = DealParameterType( mAllFunPara[para2_pos] -> left, parameter_type, eva_fun );
      if ( eva_fun && CheckNodeIsNULL( para2_node ) ) {
        NoReturnValueErrorInfo aNoReturnValueErrorInfo;
        aNoReturnValueErrorInfo.no_return_error_node = mAllFunPara[para2_pos] -> left;
        throw aNoReturnValueErrorInfo;
      } // if

      if ( para2_node -> token_data.token_type == "PROCEDURE" ) {
        string change_to_function = "";
        int start = 12;
        int end = para2_node -> token_data.token_name.size() - 13;   // 13
        change_to_function = change_to_function.assign( para2_node -> token_data.token_name, start, end );
        if ( change_to_function == "lambda" ) {
          int lambda_pos = FindNameless_UserFunctionPos( para2_node );
          if ( gDefineTable[lambda_pos].user_define_name == "" ) {
            gDefineTable[lambda_pos].user_define_name = para1_node -> token_data.token_name;
          } // if
          else {
            newDefineTable = gDefineTable[lambda_pos];
            newDefineTable.user_define_name = para1_node -> token_data.token_name;
            gDefineTable.push_back( newDefineTable );
          } // else

          if ( repeat_para1_pos != -1 ) { // �n����������I�C
            gDefineTable.erase( gDefineTable.begin() + repeat_para1_pos );
          } // if

          InsertUserFunctionToSystem( para1_node -> token_data.token_name );
        } // if
        else if ( FindName_UserFunctionPos( change_to_function ) != -1 ) {
          int define_function_pos = FindName_UserFunctionPos( change_to_function );
          newDefineTable = gDefineTable[define_function_pos];
          newDefineTable.user_define_name = para1_node -> token_data.token_name;
          if ( repeat_para1_pos != -1 ) {
            gDefineTable[repeat_para1_pos] = newDefineTable;
          } // if
          else {
            gDefineTable.push_back( newDefineTable );
          } // else

          InsertUserFunctionToSystem( para1_node -> token_data.token_name );
        } // else if
        else {
          newDefineTable.user_define_type = "Parameter";
          if ( parameter_type == "NoCreateMemory" ) {
            newDefineTable.define_value = para2_node;
          } // if
          else {
            TreeNode * new_define_node = NULL;
            new_define_node = Build_Define_Tree( para2_node, new_define_node );
            newDefineTable.define_value = new_define_node;
          } // else

        } // else

      } // if
      else {
        newDefineTable.user_define_type = "Parameter";
        if ( parameter_type == "NoCreateMemory" ) {
          newDefineTable.define_value = para2_node;
        } // if
        else {
          TreeNode * new_define_node = NULL;
          new_define_node = Build_Define_Tree( para2_node, new_define_node );
          newDefineTable.define_value = new_define_node;
        } // else

      } // else

      if ( repeat_para1_pos == -1 ) {
        if ( newDefineTable.user_define_type == "Parameter" ) {
          gDefineTable.push_back( newDefineTable );
        } // if

      } // if
      else {
        if ( newDefineTable.user_define_type == "Parameter" ) {
          gDefineTable[repeat_para1_pos] = newDefineTable;
          if ( repeat_function_type != "Parameter" ) {
            int system_pos = FindSystemSymbol( newDefineTable.user_define_name );
            mSystemSymbolTable.erase( mSystemSymbolTable.begin() + system_pos );
          } // if

        } // if

      } // else

    } // if
    else {
      FormatErrorInfo aFormatErrorInfo;
      aFormatErrorInfo.format_fun_name = "DEFINE";
      aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
      throw aFormatErrorInfo;
    } // else

    cout << newDefineTable.user_define_name << " defined\n";
    mPrint_SExp = false;
    mLevel_num = -1;
  } // EVAdefine_Type1()

  void EVAdefine_Type2( SaveFunctionParameter aSaveFunPara ) {
    DefineTable newDefineTable;
    newDefineTable.user_define_name = "";
    newDefineTable.user_define_type = "";
    newDefineTable.define_value = NULL;
    string check_system_name = "";
    string check_system_type = "";
    int i = 0;
    if ( aSaveFunPara.argument_Num >= 2 ) {
      TreeNode * para1_node = NULL;
      int para1_pos = aSaveFunPara.parameter_Pos.front();
      para1_node = mAllFunPara[para1_pos] -> left;
      aSaveFunPara.parameter_Pos.pop();
      TreeNode * is_Symbol_node = NULL;
      while ( para1_node -> isEnd == false ) {
        is_Symbol_node = para1_node -> left;
        if ( is_Symbol_node -> token_data.token_type == "SYMBOL" ) {
          if ( i == 0 ) {
            if ( Is_SystemSymbol( is_Symbol_node -> token_data.token_name ) ) {
              FormatErrorInfo aFormatErrorInfo;
              aFormatErrorInfo.format_fun_name = "DEFINE";
              aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
              throw aFormatErrorInfo;
            } // if
            else {
              newDefineTable.user_define_name = is_Symbol_node -> token_data.token_name;
            } // else

          } // if
          else {
            newDefineTable.fun_argument.push_back( is_Symbol_node );
          } // else

        } // if
        else {
          FormatErrorInfo aFormatErrorInfo;
          aFormatErrorInfo.format_fun_name = "DEFINE";
          aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
          throw aFormatErrorInfo;
        } // else

        i++;
        para1_node = para1_node -> right;
      } // while

      check_system_name = newDefineTable.user_define_name;
      if ( IsSystemSymbol( check_system_name, check_system_type ) ) {
        if ( check_system_type != "User-Defined-Function" ) {
          FormatErrorInfo aFormatErrorInfo;
          aFormatErrorInfo.format_fun_name = "DEFINE";
          aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
          throw aFormatErrorInfo;
        } // if

      } // if

      i = 1;  //  �ĤG�ѼƤ��ᬰfunction body
      TreeNode * para_many_node = NULL;
      string current_function_type = "";
      while ( aSaveFunPara.argument_Num > i ) {
        int para_many_pos = aSaveFunPara.parameter_Pos.front();
        para_many_node = mAllFunPara[para_many_pos] -> left;
        current_function_type = FindFunctionType( para_many_node );
        newDefineTable.eva_function.push_back( para_many_node );
        aSaveFunPara.parameter_Pos.pop();
        i++;
      } // while

      newDefineTable.user_define_type = current_function_type; // �w�qfunction_type�C
      i = 0;
      int repeat_para1_pos = -1;
      string repeat_function_type = ""; // ���ˬd���S���Q�w�q�L
      while ( gDefineTable.size() > i ) {
        if ( gDefineTable[i].user_define_name == newDefineTable.user_define_name ) {
          repeat_para1_pos = i;
          if ( gDefineTable[i].user_define_type == "Parameter" ) {
            repeat_function_type = "Parameter";
          } // if
          else if ( gDefineTable[i].user_define_type == "Define-Function" ) {
            repeat_function_type = "Define-Function";
          } // else if
          else if ( gDefineTable[i].user_define_type == "Lambda-Function" ) {
            repeat_function_type = "Lambda-Function";
          } // else if

        } // if

        i++;
      } // while

      newDefineTable.define_value = CreateProcedureNode( newDefineTable.user_define_name );
      if ( repeat_para1_pos == -1 ) {
        gDefineTable.push_back( newDefineTable );
        InsertUserFunctionToSystem( newDefineTable.user_define_name );
      } // if
      else {
        if ( repeat_function_type == "Parameter" ) {
          gDefineTable[repeat_para1_pos] = newDefineTable;
          InsertUserFunctionToSystem( newDefineTable.user_define_name );
        } // if
        else {
          gDefineTable[repeat_para1_pos] = newDefineTable;
          if ( newDefineTable.user_define_type == "Lambda-Function" ) {
            gDefineTable[repeat_para1_pos].define_value = CreateProcedureNode( "lambda" );
            TreeNode * insert = gDefineTable[repeat_para1_pos].define_value;
          } // if
          else {
            gDefineTable[repeat_para1_pos] = newDefineTable;
          } // else

        } // else

      } // else

    } // if
    else {
      FormatErrorInfo aFormatErrorInfo;
      aFormatErrorInfo.format_fun_name = "DEFINE";
      aFormatErrorInfo.format_error_node = mAllFunPara[aSaveFunPara.fun_Pos];
      throw aFormatErrorInfo;
    } // else

    cout << newDefineTable.user_define_name << " defined\n";
    mPrint_SExp = false;
    mLevel_num = -1;
  } // EVAdefine_Type2()

  TreeNode * CreateProcedureNode( string system_name ) {
    TreeNode * procedure_error_name = NULL;
    procedure_error_name = new TreeNode;
    procedure_error_name -> left = NULL;
    procedure_error_name -> right = NULL;
    procedure_error_name -> isStart = false;
    procedure_error_name -> isEnd = false;
    int what_fun_type = -1;
    what_fun_type = FindName_UserFunctionPos( system_name ); // �T�{���i�ରlambda���Φ��C
    if ( what_fun_type != -1
         && gDefineTable[what_fun_type].user_define_type == "Lambda-Function" ) {
      system_name = "lambda";
    } // if

    string token_name = "#<procedure " + system_name + ">";
    procedure_error_name -> token_data.token_name = token_name;
    procedure_error_name -> token_data.token_type = "PROCEDURE";
    return procedure_error_name;
  } // CreateProcedureNode()

  bool Compare_TwoTree_Data( TreeNode * tree1, TreeNode * tree2 ) {
    if ( tree1 == NULL && tree2 == NULL ) return true;
    if ( tree1 != NULL && tree2 != NULL
         && tree1 -> token_data.token_name == tree2 -> token_data.token_name
         && Compare_TwoTree_Data( tree1 -> left, tree2 -> left )
         && Compare_TwoTree_Data( tree1 -> right, tree2 -> right ) ) {
      return true;
    } // if

    return false;
  } // Compare_TwoTree_Data()

  TreeNode * Build_Define_Tree( TreeNode * define_tree, TreeNode * new_define_tree ) {
    // �s�ؤ@�ӷs��define�Ŷ��bgDefineTable���C
    if ( define_tree ) {
      new_define_tree = new TreeNode;
      new_define_tree -> token_data = define_tree -> token_data;
      new_define_tree -> isStart = define_tree -> isStart;
      new_define_tree -> isEnd = define_tree -> isEnd;
      new_define_tree -> left = Build_Define_Tree( define_tree -> left, new_define_tree );
      new_define_tree -> right = Build_Define_Tree( define_tree -> right, new_define_tree );
      return new_define_tree;
    } // if

    return NULL;
  } // Build_Define_Tree()

  TreeNode * Return_NewEval_Tree( TreeNode * defined_tree, TreeNode * new_eval_tree ) {
    // return gDefineTable�����@�ӷs��define�Ŷ����p��Ϊ��C
    if ( defined_tree ) {
      new_eval_tree = new TreeNode;
      new_eval_tree -> token_data =  defined_tree -> token_data;
      new_eval_tree -> isStart = defined_tree -> isStart;
      new_eval_tree -> isEnd = defined_tree -> isEnd;
      new_eval_tree -> left = Return_NewEval_Tree( defined_tree -> left, new_eval_tree );
      new_eval_tree -> right = Return_NewEval_Tree( defined_tree -> right, new_eval_tree );
      return new_eval_tree;
    } // if

    return NULL;
  } // Return_NewEval_Tree()

  // �ˬd -> Level�PArgument number
  void ArgumentNum_Check( int user_function_pos, int argument_num, string fun_name, string fun_type ) {
    bool check_get = false;
    if ( fun_name == "cons" ) {
      if ( argument_num == 2 ) check_get = true;
    } // if
    else if ( fun_name == "list" ) {
      if ( argument_num >= 0 ) check_get = true;
    } // else if
    else if ( fun_name == "define" ) {
      if ( mLevel_num != 1 ) {
        LevelErrorInfo aLevelErrorInfo;
        aLevelErrorInfo.level_function_name = "DEFINE";
        throw aLevelErrorInfo;
      } // if

      check_get = true; // define���t�~error���Φ��C (�@) define��level error (�G) define��format error�C
    } // else if
    else if ( fun_type == "Part accessors" ) {
      if ( argument_num == 1 ) check_get = true;
    } // else if
    else if ( fun_type == "Primitive predicates" ) {
      if ( argument_num == 1 ) check_get = true;
    } // else if
    else if ( fun_type == "Number arithmetic" ) {
      if ( argument_num >= 2 ) check_get = true;
    } // else if
    else if ( fun_type == "Logical" ) {
      if ( fun_name == "not" ) {
        if ( argument_num == 1 ) check_get = true;
      } // if
      else {
        if ( argument_num >= 2 ) check_get = true;
      } // else

    } // else if
    else if ( fun_type == "Number compare" || fun_type == "String compare" ) {
      if ( argument_num >= 2 ) check_get = true;
    } // else if
    else if ( fun_type == "Eqivalence tester" ) {
      if ( argument_num == 2 ) check_get = true;
    } // else if
    else if ( fun_type == "Sequencing" ) {
      if ( argument_num >= 1 ) check_get = true;
    } // else if
    else if ( fun_type == "Conditionals" ) {
      if ( fun_name == "cond" ) {
        check_get = true;
      } // if
      else {
        if ( argument_num == 2 || argument_num == 3 ) check_get = true;
      } // else

    } // else if
    else if ( fun_type == "Quote" ) {
      check_get = true;
    } // else if
    else if ( fun_type == "Special level" ) {
      if ( mLevel_num == 1 ) {
        if ( argument_num == 0 ) { // ��@ clean-environment
          check_get = true;
          mPrint_SExp = false;
          if ( fun_name == "exit" ) {
            Finished aFinished;
            aFinished.exit = true;
            throw aFinished;
          } // if
          else {
            vector<DefineTable> newTable;
            gDefineTable = newTable; // ��l��DefineTable�C
            vector<ProcedureTable> anewTable;
            gProcedureTable = anewTable;
            cout << "environment cleaned" << "\n";
            mLevel_num = -1;
          } // else

        } // if
      } // if
      else {        // level�W�L�F�A��Xlevel-error
        LevelErrorInfo aLevelErrorInfo;
        if ( fun_name == "exit" ) {
          aLevelErrorInfo.level_function_name = "EXIT";
        } // if
        else {
          aLevelErrorInfo.level_function_name = "CLEAN-ENVIRONMENT";
        } // else

        throw aLevelErrorInfo;
      } // else

    } // else if
    else if ( fun_type == "Let" ) {
      check_get = true;
    } // else if
    else if ( fun_type == "Lambda" ) {
      check_get = true;
    } // else if
    else if ( fun_type == "User-Defined-Function" ) {
      if ( gDefineTable[user_function_pos].fun_argument.size() == argument_num ) {
        check_get = true;
      } // if
      else {
        if ( gDefineTable[user_function_pos].user_define_type == "Lambda-Function" ) {
          fun_name = "lambda";
        } // if

      } // else

    } // else if

    // throw argument_num error
    if ( check_get == false ) {  // �٦���Lerror���i��ʡC
      AgumentNumberErrorInfo aAgumentNumberErrorInfo;
      aAgumentNumberErrorInfo.fun_name = fun_name;
      throw aAgumentNumberErrorInfo;
    } // if

  } // ArgumentNum_Check()

  // (�@)�ˬd -> ���L��function  (�G) �]�w�n�i�J��function���Ѽ�
  int Function_Check( TreeNode * fun_node, string & fun_name, string & fun_type ) {
    int define_pos = -1;  // �p�Gsymbol���Q�w�q���ܡA��ȷ|�Q���C
    int function_pos = -1;
    bool is_global = true;
    bool is_bound = false;
    if ( mUserFunctionInfo.size() > 0 && fun_node -> token_data.token_type == "SYMBOL" ) {
      UserFunctionInfo aUserFunctionInfo;
      aUserFunctionInfo = mUserFunctionInfo[mUserFunctionInfo.size()-1];
      DefineTable aDefineTable;
      aDefineTable = gDefineTable[aUserFunctionInfo.function_pos];
      int info_pos = -1;
      if ( IsUserFunctionParameter( fun_node -> token_data, aDefineTable, info_pos ) ) {
        is_global = false;
        is_bound = true;
        fun_node = aUserFunctionInfo.real_para[info_pos];
      } // if

    } // if

    if ( is_global && IsBoundSymbol( fun_node -> token_data, define_pos ) ) {  // ���Q�w�q�A���O���ܥN��Onon
      if ( gDefineTable[define_pos].user_define_type == "Parameter" ) {
        is_bound = true;
      } // if

      fun_node = gDefineTable[define_pos].define_value;
    } // if

    fun_name = fun_node -> token_data.token_name;
    fun_type = fun_node -> token_data.token_type;
    if ( fun_type != "PROCEDURE" && IsSystemSymbol( fun_name, fun_type ) ) {
      if ( is_bound ) {
        NonFunctionErrorInfo aNonFunctionErrorInfo;
        aNonFunctionErrorInfo.nonfun_name = gDefineTable[define_pos].define_value;
        throw aNonFunctionErrorInfo;
      } // if

    } // if
    else {
      if ( fun_type == "PROCEDURE" ) {
        function_pos = ConnectProcedure_to_UserFunction( fun_node );
        if ( function_pos == -1 ) {
          string change_to_function = "";
          int start = 12;
          int end = fun_name.size() - 13;   // 13
          change_to_function = change_to_function.assign( fun_name, start, end );
          fun_name = change_to_function;
          IsSystemSymbol( fun_name, fun_type );
        } // if
        else {
          fun_name = gDefineTable[function_pos].user_define_name;
          fun_type = "User-Defined-Function";
        } // else

      } // if
      else if ( define_pos == -1 ) {  // ���O�w�q�A�p�G�Osymbol�Nnon-bound�A�p�G���Osymbol�Nnonfunction
        if ( fun_node -> token_data.token_type == "SYMBOL" ) {
          UnboundSymbolErrorInfo aUnboundSymbolErrorInfo;
          aUnboundSymbolErrorInfo.symbol_name = fun_node -> token_data.token_name;
          throw aUnboundSymbolErrorInfo;
        } // if
        else {
          NonFunctionErrorInfo aNonFunctionErrorInfo;
          aNonFunctionErrorInfo.nonfun_name = fun_node;
          throw aNonFunctionErrorInfo;
        } // else

      } // else if
      else  {
        NonFunctionErrorInfo aNonFunctionErrorInfo;
        aNonFunctionErrorInfo.nonfun_name = gDefineTable[define_pos].define_value;
        throw aNonFunctionErrorInfo;
      } // else

    } // else

    return function_pos;
  } // Function_Check()

  // �ˬd -> ������node�@�w�n�Onil�C
  void NonList_Chcek( TreeNode * cur_node, SaveFunctionParameter aSaveFunPara ) {
    int fun_pos = aSaveFunPara.fun_Pos;
    if ( cur_node -> token_data.token_type != "NIL" ) {  // throw nonlist(�̫�O.������)�C
      NonListErrorInfo aNonListErrorInfo;
      aNonListErrorInfo.nonlist_node = mAllFunPara[fun_pos];
      throw aNonListErrorInfo;
    } // if

  } // NonList_Chcek()

  bool IsBoundSymbol( TokenData tokendata, int & define_pos ) {
    if ( tokendata.token_type == "SYMBOL" ) {
      int i = 0;
      while ( gDefineTable.size() > i ) {
        if ( gDefineTable[i].user_define_name == tokendata.token_name ) {
          define_pos = i;
          return true;
        } // if

        i++;
      } // while

    } // if

    return false;
  } // IsBoundSymbol()

  bool IsAtom( TokenData token_IsAtom ) {
    if ( token_IsAtom.token_type == "PROCEDURE" ) {
      return true;
    } // if
    else if ( token_IsAtom.token_type == "SYMBOL" || token_IsAtom.token_type == "INT"
              || token_IsAtom.token_type == "FLOAT" || token_IsAtom.token_type == "STRING"
              || token_IsAtom.token_type == "NIL" || token_IsAtom.token_type == "LEFT-PAREN RIGHT-PAREN"
              || token_IsAtom.token_type == "T" ) {
      return true;
    } // else if

    return false;
  } // IsAtom()

  bool IsList( TreeNode * real_para ) {
    if ( real_para -> token_data.token_type == "LEFT-PAREN" ) {
      if ( real_para -> left -> token_data.token_type == "LEFT-PAREN" // ���X�Ĥ@�h���̫�O���Onon-list�C
           || real_para -> left -> token_data.token_type == "QUOTE" ) {
        return true;
      } // if
      else {
        while ( real_para -> isEnd == false ) {
          real_para = real_para -> right;
        } // while

        if ( real_para -> token_data.token_type != "NIL" ) return false;
        else return true;
      } // else

    } // if
    else {
      return false;
    } // else

    return false;
  } // IsList()

  bool IsPair( TreeNode * real_para ) {
    bool is_pair = false;
    if ( real_para -> token_data.token_type == "LEFT-PAREN" ) {
      is_pair = true;
      if ( real_para -> left -> token_data.token_type == "QUOTE" ) {
        is_pair = false;
        return is_pair;
      } // if

      else return is_pair;
    } // if
    else {
      return is_pair;
    } // else

    return is_pair;
  } // IsPair()

  int FindName_UserFunctionPos( string function_name ) {
    int i = 0, define_pos = -1;
    while ( gDefineTable.size() > i ) {
      if ( function_name != "" && gDefineTable[i].user_define_name == function_name ) {
        define_pos = i;
        return define_pos;
      } // if

      i++;
    } // while

    return define_pos;
  } // FindName_UserFunctionPos()

  int FindNameless_UserFunctionPos( TreeNode * fun_node ) { // ���P��lambda����m�C
    int i = 0, define_pos = -1;
    while ( gDefineTable.size() > i ) {
      if ( gDefineTable[i].define_value != NULL
           && fun_node != NULL
           && gDefineTable[i].define_value == fun_node ) {
        define_pos = i;
        return define_pos;
      } // if

      i++;
    } // while

    return define_pos;
  } // FindNameless_UserFunctionPos()

  bool IsUserFunctionParameter( TokenData tokendata, DefineTable aDefineTable, int & para_pos ) {
    int i = 0;
    while ( aDefineTable.fun_argument.size() > i ) {
      if ( aDefineTable.fun_argument[i] -> token_data.token_name == tokendata.token_name ) {
        para_pos = i;
        return true;
      } // if

      i++;
    } // while

    return false;
  } // IsUserFunctionParameter()

  // �s�W�s����(���A��)
  TreeNode * ReSetLinkResult( string new_or_link ) {
    TokenData token_LP_data;
    token_LP_data.token_name = "(";
    token_LP_data.token_type = "LEFT-PAREN";
    TreeNode * newnode;
    newnode = new TreeNode;
    newnode -> left = NULL;
    newnode -> right = NULL;
    newnode -> token_data = token_LP_data;
    if ( new_or_link == "LINK" ) {
      newnode -> isEnd = false;
      newnode -> isStart = false;
    } // if
    else {
      newnode -> isEnd = false;
      newnode -> isStart = true;
    } // else

    return newnode;
  } // ReSetLinkResult()

  void ReSetmSave_Table() {
    mSave_Table.fun_Name = "";
    mSave_Table.fun_Type = "";
    mSave_Table.fun_Pos = -1;
    mSave_Table.argument_Num = 0;
    queue<int> newparameter_Pos;
    mSave_Table.parameter_Pos = newparameter_Pos;
    mSave_Table.result_Node = NULL;
  } // ReSetmSave_Table()

  string FindFunctionType( TreeNode * para_node ) {
    string type_1 = "Define-Function";
    string type_2 = "Lambda-Function";
    string eva_string = "";
    TreeNode * eva_node = para_node;
    while ( eva_node -> left != NULL ) {
      eva_node = eva_node -> left;
    } // while

    int define_pos = -1;
    if ( IsBoundSymbol( eva_node -> token_data, define_pos ) ) {
      eva_string = gDefineTable[define_pos].user_define_name;
    } // if
    else {
      eva_string = eva_node -> token_data.token_name;
    } // else

    if ( eva_string == "lambda" ) {
      return type_2;
    } // if

    return type_1;
  } // FindFunctionType()

  void InsertUserFunctionToSystem( string user_fun_name ) {
    int i = 0;
    bool has_repeat = false;
    while ( mSystemSymbolTable.size() > i ) {
      if ( mSystemSymbolTable[i].symbol_name == user_fun_name ) {
        has_repeat = true;
      } // if

      i++;
    } // while

    if ( !has_repeat ) {
      SystemSymbol aSystemSymbol;
      aSystemSymbol.symbol_name = user_fun_name;
      aSystemSymbol.symbol_type = "User-Defined-Function";
      mSystemSymbolTable.push_back( aSystemSymbol );
    } // if

  } // InsertUserFunctionToSystem()

  bool IsSameSymbol_In_Argument( TokenData tokendata, vector<TreeNode*> fun_argument, int & repeat_pos ) {
    int i = 0;
    while ( fun_argument.size() > i ) {
      if ( fun_argument[i] -> token_data.token_name == tokendata.token_name ) {
        repeat_pos = i;
        return true;
      } // if

      i++;
    } // while

    return false;
  } // IsSameSymbol_In_Argument()

  public:
  bool mPrint_SExp;
  Tree( vector<SystemSymbol> aSystemSymbolTable ) {
    mDoingQuote = false;
    mCurrent_pos = 0;
    mStart_ResultSExp = NULL;
    mLevel_num = 0;
    mPrint_SExp = true;
    vector<TreeNode*> aAllFunPara;
    vector<UserFunctionInfo> aUserFunctionInfo;
    mAllFunPara = aAllFunPara;
    mUserFunctionInfo = aUserFunctionInfo;
    mSystemSymbolTable = aSystemSymbolTable;
    ReSetmSave_Table();
  } // Tree()

  bool IsSystemSymbol( string & fun_name, string & fun_type ) {
    int i = 0;
    while ( mSystemSymbolTable.size() > i ) {
      if ( mSystemSymbolTable[i].symbol_name == fun_name ) {
        fun_type = mSystemSymbolTable[i].symbol_type;
        return true;
      } // if

      i++;
    } // while

    return false;
  } // IsSystemSymbol()

  int FindSystemSymbol( string name ) {
    int i = 0, define_pos = -1;
    while ( mSystemSymbolTable.size() > i ) {
      if ( mSystemSymbolTable[i].symbol_name == name ) {
        define_pos = i;
      } // if

      i++;
    } // while

    return define_pos;
  } // FindSystemSymbol()

  bool Is_SystemSymbol( string name ) {
    int i = 0, define_pos = -1;
    while ( mSystemSymbolTable.size() > i ) {
      if ( mSystemSymbolTable[i].symbol_name == name
           && mSystemSymbolTable[i].symbol_type != "User-Defined-Function" ) {
        return true;
      } // if

      i++;
    } // while

    return false;
  } // Is_SystemSymbol()

  // �L��list-like formate
  void PrintSExp( TreeNode * aTreeRoot, string & printed, bool & firstsexp ) {
    if ( aTreeRoot ) {
      if ( aTreeRoot -> isStart ) {
        if ( firstsexp ) {
          cout << aTreeRoot -> token_data.token_name + " ";
          printed = printed + "  ";
        } // if
        else {
          firstsexp = true;
          cout << printed;
          cout << aTreeRoot -> token_data.token_name + " ";
          printed = printed + "  ";
        } // else

      } // if
      else if ( aTreeRoot -> isEnd ) {
        // �p�G�Onil���ܡA��ܸ�sexp�S���I�A��L�h�N���I�C
        if ( aTreeRoot -> token_data.token_name == "nil" ) {
          printed = printed.assign( printed, 0, printed.size() - 2 );
          cout << printed << ")" << "\n";
        } // if
        else if ( aTreeRoot -> token_data.token_type == "LEFT-PAREN" ) {
          cout << printed;
          cout << "." << "\n";
          cout << printed;
          firstsexp = true;
        } // else if
        else {
          cout << printed;
          cout << "." << "\n";
          cout << printed;
          if ( aTreeRoot -> token_data.token_type == "FLOAT" ) {
            stringstream ss;
            ss << aTreeRoot -> token_data.token_name;
            double good = 0;
            ss >> good;
            printf( "%.3f\n", good );
          } // if
          else {
            cout << aTreeRoot -> token_data.token_name << "\n";
          } // else

          printed = printed.assign( printed, 0, printed.size() - 2 );
          cout << printed << ")" << "\n";
        } // else

      } // else if
      else if ( aTreeRoot -> left == NULL && aTreeRoot -> right == NULL ) {

        if ( firstsexp ) {
          firstsexp = false;
        } // if
        else {
          cout << printed;
        } // else

        if ( aTreeRoot -> token_data.token_type == "FLOAT" ) {
          stringstream ss;
          ss << aTreeRoot -> token_data.token_name;
          double good = 0;
          ss >> good;
          printf( "%.3f\n", good );
        } // if
        else {
          cout << aTreeRoot -> token_data.token_name << "\n";
        } // else
      } // else if

      PrintSExp( aTreeRoot -> left, printed, firstsexp );
      PrintSExp( aTreeRoot -> right, printed, firstsexp );
    } // if

  } // PrintSExp()

  void Preorder( TreeNode * aTreeRoot ) {
    if ( aTreeRoot ) {
      cout << aTreeRoot -> token_data.token_name;
      Preorder( aTreeRoot -> left );
      Preorder( aTreeRoot -> right );
    } // if
  } // Preorder()

  bool CheckNodeIsNULL( TreeNode * check_null ) {
    if ( check_null == NULL ) {
      return true;
    } // if

    return false;
  } // CheckNodeIsNULL()

  // catch Error��i�J�B��A�ê�^�B�⧹��reusltSExp��main()
  TreeNode * EvaluateSExp( TreeNode * inputSExp, bool & finished, vector<SystemSymbol> & new_one ) {
    try {
      if ( inputSExp -> left == NULL && inputSExp -> right == NULL ) {
        int define_pos = -1;
        if ( IsBoundSymbol( inputSExp -> token_data, define_pos ) ) {   // ���P�_���O���OSYMBOL
          TreeNode * new_eval_tree = NULL;
          new_eval_tree = Return_NewEval_Tree( gDefineTable[define_pos].define_value, new_eval_tree );
          mStart_ResultSExp = new_eval_tree;
        } // if
        else {
          if ( inputSExp -> token_data.token_type == "SYMBOL" ) {
            if ( IsSystemSymbol( inputSExp -> token_data.token_name, inputSExp -> token_data.token_type ) ) {
              TreeNode * procedure_node = NULL;
              procedure_node = CreateProcedureNode( inputSExp -> token_data.token_name );
              mStart_ResultSExp = procedure_node;
            } // if
            else {
              UnboundSymbolErrorInfo aUnboundSymbolErrorInfo;
              aUnboundSymbolErrorInfo.symbol_name = inputSExp -> token_data.token_name;
              throw aUnboundSymbolErrorInfo;
            } // else

          } // if
          else {
            mStart_ResultSExp = inputSExp;
          } // else

        } // else

      } // if
      else {
        mStart_ResultSExp = EvaluateParameter( inputSExp );  // ���p��ѼơA�A�p��function�C
        new_one = mSystemSymbolTable;
        if ( mLevel_num != -1 && CheckNodeIsNULL( mStart_ResultSExp ) ) {
          NoReturnValueErrorInfo aNoReturnValueErrorInfo;
          aNoReturnValueErrorInfo.no_return_error_node = inputSExp;
          throw aNoReturnValueErrorInfo;
        } // if

      } // else

    } // try
    catch( NonFunctionErrorInfo aNonFunctionErrorInfo ) {
      cout << "ERROR (attempt to apply non-function) : ";
      mStart_ResultSExp = aNonFunctionErrorInfo.nonfun_name;
    } // catch
    catch( UnboundSymbolErrorInfo aUnboundSymbolErrorInfo ) {
      cout << "ERROR (unbound symbol) : " << aUnboundSymbolErrorInfo.symbol_name << "\n";
      mPrint_SExp = false;
    } // catch
    catch( AgumentNumberErrorInfo aAgumentNumberErrorInfo ) {
      cout << "ERROR (incorrect number of arguments) : " << aAgumentNumberErrorInfo.fun_name << "\n";
      mPrint_SExp = false;
    } // catch
    catch( NonListErrorInfo aNonListErrorInfo ) {
      cout << "ERROR (non-list) : ";
      mStart_ResultSExp = aNonListErrorInfo.nonlist_node;
    } // catch
    catch( LevelErrorInfo aLevelErrorInfo ) {
      cout << "ERROR (level of " << aLevelErrorInfo.level_function_name << ")\n";
      mPrint_SExp = false;
    } // catch
    catch( FormatErrorInfo aFormatErrorInfo ) {
      cout << "ERROR (" << aFormatErrorInfo.format_fun_name << " format) : ";
      mStart_ResultSExp = aFormatErrorInfo.format_error_node;
    } // catch
    catch( ArgumentTypeErrorInfo aArgumentTypeErrorInfo ) {
      cout << "ERROR (" << aArgumentTypeErrorInfo.function_name << " with incorrect argument type) : ";
      mStart_ResultSExp = aArgumentTypeErrorInfo.argumentype_error_node;
    } // catch
    catch( DivisionByZero aDivisionByZero ) {
      cout << aDivisionByZero.error_name << "\n";
      mPrint_SExp = false;
    } // catch
    catch( NoReturnValueErrorInfo aNoReturnValueErrorInfo ) {
      cout << "ERROR (no return value) : ";
      mStart_ResultSExp = aNoReturnValueErrorInfo.no_return_error_node;
    } // catch
    catch( Finished aFinished ) {
      finished = aFinished.exit;
      mPrint_SExp = false;
    } // catch
    catch( UnboundParameterErrorInfo aUnboundParameterErrorInfo ) {
      cout << "ERROR (unbound parameter) : ";
      mStart_ResultSExp = aUnboundParameterErrorInfo.unbound_Parameter_node;
    } // catch
    catch ( UnboundConditionErrorInfo aUnboundConditionErrorInfo ) {
      cout << "ERROR (unbound condition) : ";
      mStart_ResultSExp = aUnboundConditionErrorInfo.unbound_Condition_node;
    } // catch
    catch ( UnboundTestConditionErrorInfo aUnboundTestConditionErrorInfo ) {
      cout << "ERROR (unbound test-condition) : ";
      mStart_ResultSExp = aUnboundTestConditionErrorInfo.unbound_Testcondition_node;
    } // catch

    return mStart_ResultSExp;
  } // EvaluateSExp()

}; // Tree

vector<SystemSymbol> BuildSystemSymbol() {
  vector<SystemSymbol> aSystemSymbolTable;
  SystemSymbol aSystemSymbol;
  aSystemSymbol.symbol_name = "cons";
  aSystemSymbol.symbol_type = "Constructors";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "list";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "\'";
  aSystemSymbol.symbol_type = "Quote";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "quote";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "define";
  aSystemSymbol.symbol_type = "Bounding";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "car";
  aSystemSymbol.symbol_type = "Part accessors";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "cdr";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "atom?";
  aSystemSymbol.symbol_type = "Primitive predicates";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "pair?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "list?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "null?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "integer?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "real?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "number?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "string?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "boolean?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "symbol?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "+";
  aSystemSymbol.symbol_type = "Number arithmetic";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "-";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "*";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "/";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "not";
  aSystemSymbol.symbol_type = "Logical";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "and";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "or";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = ">";
  aSystemSymbol.symbol_type = "Number compare";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = ">=";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "<";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "<=";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "=";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "string-append";
  aSystemSymbol.symbol_type = "String compare";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "string>?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "string<?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "string=?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "eqv?";
  aSystemSymbol.symbol_type = "Eqivalence tester";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "equal?";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "begin";
  aSystemSymbol.symbol_type = "Sequencing";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "if";
  aSystemSymbol.symbol_type = "Conditionals";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "cond";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "exit";
  aSystemSymbol.symbol_type = "Special level";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "clean-environment";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "let";
  aSystemSymbol.symbol_type = "Let";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "lambda";
  aSystemSymbol.symbol_type = "Lambda";
  aSystemSymbolTable.push_back( aSystemSymbol );
  return aSystemSymbolTable;
} // BuildSystemSymbol()

void Remove_LET_Element() {
  int i = 0;
  while ( gDefineTable.size() > i ) {
    if ( gDefineTable[i].user_define_type == "LET" ) {
      gDefineTable.erase( gDefineTable.begin() + i );
      i--;
    } // if

    i++;
  } // while

} // Remove_LET_Element()

// Scanner�PParser���۷��q��function�A��Paser�B�z���õL������N�}�l��Tree�C
TreeNode * ReadSExp( TreeNode * aTreeRoot, int & lastspace, bool & finished_exit  ) {
  Scanner aScanner( lastspace );                // �Ф@��aScanner��K�ڭ̡u���v��SExp�C
  Parser aParser;                 // �Ф@��aParser�ˬd�ڭ�aScanner���X�Ӫ�token�A
  TokenData first_Token_Data;    // ��k���S�����T�A���T�Nbulidtree�A���~�N�^�ǿ��~�C
  if ( aParser.IsSexp( first_Token_Data, aScanner ) ) {   // �P�_Sexp���X�k�ʡA�åB�p�G�ӧ�����Sexp�᭱�S�F��Ū���C
    lastspace = aScanner.ReadEndLine();
  } // if
  else {
    NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;          // �p�G�Ĥ@��SExp�N�X���C
    aNoAtomQuoteErrorInfo.token_name = first_Token_Data.token_name;
    aNoAtomQuoteErrorInfo.column = first_Token_Data.token_column;
    aNoAtomQuoteErrorInfo.line = first_Token_Data.token_line;
    aScanner.ReadErrorLine();
    throw aNoAtomQuoteErrorInfo;
  } // else

  vector<TokenData> returnToken_vector = aParser.ReturnToken_vector(); // Sexp�X�k���}�l�C
  if ( returnToken_vector.size() == 3 ) {
    if ( returnToken_vector[0].token_name == "("
         && returnToken_vector[1].token_name == "exit"
         && returnToken_vector[2].token_name == ")" ) {
      finished_exit = true;
      return aTreeRoot;
    } // if

  } // if

  int i = 0, record_pos = -1;
  queue<int> dotforquote;
  aParser.TranslateToken( dotforquote );
  // aParser.PrintVector();
  aTreeRoot = aParser.BuildTree( aTreeRoot, i, dotforquote, record_pos );
  return aTreeRoot;
} // ReadSExp()

int main() {
  char linenter = '\0';
  scanf( "%d%c", &utestNum, &linenter );  // Ū���@��testnum�M�@��line-enter�C
  cout << "Welcome to OurScheme!\n\n";
  cout << "> ";
  bool finished = false;
  int lastspace = 0;
  if ( cin.peek() == EOF ) {            // ���T�w�@�}�l�����ɤ�����ơA�L�N�����L���~�A���N�~�򩹤U���C
    cout << "ERROR (no more input) : END-OF-FILE encountered";
  } // if

  vector<SystemSymbol> systemSymbolTable;
  systemSymbolTable = BuildSystemSymbol();
  while ( !finished ) {
    TreeNode * inputSExp;               // �@���B�z�@��SExp�A�@��SExp�N��@�Ӿ𪺵��c�C
    inputSExp = new TreeNode;
    inputSExp -> left = NULL;
    inputSExp -> right = NULL;
    inputSExp -> isStart = false;
    inputSExp -> isEnd = false;
    try {
      inputSExp = ReadSExp( inputSExp, lastspace, finished );
      // Scanner�PPaser���۷��q��function�A��Paser�B�z���õL������N�}�l��Tree�C userŪ�� (exit)�A�]�wfinished�A�����o�������ɡC
      if ( !finished ) {
        Tree aTree( systemSymbolTable );
        TreeNode * resultSExp;            // �@���B�z�@��SExp�A�@��SExp�N��@�Ӿ𪺵��c�C
        resultSExp = aTree.EvaluateSExp( inputSExp, finished, systemSymbolTable );
        bool firstsexp = true;
        string printed = "";
        // aTree.Preorder( resultSExp );
        if ( aTree.mPrint_SExp ) aTree.PrintSExp( resultSExp, printed, firstsexp );
      } // if

    } // try
    catch( int endoffile ) {         // error1(int): no more input�C
      cout << "ERROR (no more input) : END-OF-FILE encountered";
      finished = true;
      lastspace = 0;
    } // catch
    catch( StringErrorInfo aStringErrorInfo ) {   // error2: scanner�B�zstring���ɭԡA���i��no closing quote
      cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line "
           << aStringErrorInfo.line << " Column " << aStringErrorInfo.column + 1 << "\n";
      lastspace = 0;
    } // catch
    catch( NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo ) {
      cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "
           << aNoAtomQuoteErrorInfo.line;
      cout << " Column " << aNoAtomQuoteErrorInfo.column << " is >>"
           << aNoAtomQuoteErrorInfo.token_name << "<<\n";
      lastspace = 0;
    } // catch
    catch( NoRightParenErrorInfo aNoRightParen ) {
      cout << "ERROR (unexpected token) : ')' expected when token at Line " << aNoRightParen.line;
      cout << " Column " << aNoRightParen.column << " is >>" << aNoRightParen.token_name << "<<\n";
      lastspace = 0;
    } // catch

    Remove_LET_Element();
    if ( !finished ) {
      cout << "\n";
      cout << "> ";
    } // if

  } // while

  cout << "\n";
  cout << "Thanks for using OurScheme!";
} // main()

