# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>
# include <stack>
# include <queue>
# include <iomanip>  // �����X�p�Ʀ��
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
}; // LPforRP

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

struct DefineTable {
  string define_name;
  TreeNode * define_value;
}; // DefineTable

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

struct ProcedureErrorInfo {
  string procedure_error_name;
}; // ProcedureErrorInfo

struct ArgumentTypeErrorInfo {
  string function_name;
  string argumentype_error_name;
}; // ArgumentTypeErrorInfo


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
    if ( mAccurate_token_vector.size() == 1 ) {
      mTranslated_token_vector.push_back( mAccurate_token_vector[0] );   // ATOM
      return;
    } // if
    else {
      while ( mAccurate_token_vector.size() > anumber ) {
        if ( mAccurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {    // ���O���Ӥl�Τ���sexp�����X�Ӫťե�

          int cur = mTranslated_token_vector.size() - 1;
          if ( cur != -1 ) {
            if ( mTranslated_token_vector[cur].token_type != "DOT"
                 && mTranslated_token_vector[cur].token_type != "LEFT-PAREN" ) {
              mTranslated_token_vector.push_back( token_Dot_data );
              mTranslated_token_vector.push_back( token_LP_data );
            } // if
          } // if

          mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          LPforRP newLPforRP;
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
          } // else

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
          else if ( mTranslated_token_vector[cur].token_type == "RIGHT-PAREN" ) {
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

}; // Parser

class Tree {
  private:
  vector<TreeNode*> mAllFunPara;  // �Ҧ��s�J��function & parameter�A(�s���A���s���I)�C
  vector<SystemSymbol> mSystemSymbolTable;
  SaveFunctionParameter mSave_Table;
  TreeNode * mStart_ResultSExp;   // �}�l���Y�A�������G�C
  TreeNode * mCur_ResultSExp;    //  �{�b�i��쪺��m�A�hFunction�P�_�ΡC
  bool mDoingQuote;          // ��sexp�u��quote�A�p��h�l���Ĥ@��quote�ΡC
  int mCurrent_pos;         // �C�@���I�s���W�[1�A�]���n�Φ��Ӵ��JmAllFunPara�C
  int mLevel_num;
  // KEY:�J��QUOTE�N�O�@��List�A���M�U�@��Is_Start���䱵���N�Ofunction�C
  TreeNode * EvaluateParameter( TreeNode * inputSExp ) {
    if ( inputSExp ) {
      if ( inputSExp -> isStart ) {  // �Ӽh�}�l�C�L�����n���������䪺function��b�k���䪺�Ҧ��ѼơC
        // �x�s��function node isStart����m�C
        mLevel_num++;
        ReSetmSave_Table();
        mAllFunPara.push_back( inputSExp );         // *�n���ק�(����node)*
        mSave_Table.fun_Pos = mCurrent_pos;         // *�n���ק�(����fun��m)*
        mCurrent_pos++;
        EvaluateParameter( inputSExp -> right );
        //
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
    if ( fun_node -> isStart ) {        // �p�G�S�J�쥪�A���A�N�}�l���j���ȡA����ڭn��function node�C
      fun_node = EvaluateParameter( fun_node );
    } // if

    // �A���ˬdFunction�A�̫��ˬdArgument Number�C
    Function_Check( fun_node, aSaveFunPara.fun_Name, aSaveFunPara.fun_Type );
    ArgumentNum_Check( argu_num, aSaveFunPara.fun_Name, aSaveFunPara.fun_Type );
    return EvaluateFunction( aSaveFunPara );
  } // CheckAllErrors_EVAFunction()

  TreeNode * EvaluateFunction( SaveFunctionParameter aSaveFunPara ) {
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
      if ( fun_name == "car" ) return EVAcar_cdr( "car", aSaveFunPara );
      else return EVAcar_cdr( "cdr", aSaveFunPara );
    } // else if
    else if ( fun_type == "Bounding" ) {
      EVAdefine( aSaveFunPara );
    } // else if
    else if ( fun_type == "Primitive predicates" ) {

    } // else if
    else if ( fun_type == "Number arithmetic" ) {

    } // else if
    else if ( fun_type == "Logical" ) {

    } // else if
    else if ( fun_type == "Number compare" ) {

    } // else if
    else if ( fun_type == "String compare" ) {

    } // else if
    else if ( fun_type == "Eqivalence tester" ) {

    } // else if
    else if ( fun_type == "Sequencing" ) {

    } // else if
    else if ( fun_type == "Conditionals" ) {

    } // else if

    return NULL;
  } // EvaluateFunction()

  // �ѼƦ��|�ت��i��A1.function 2.quote(�ܺ�)�Ѽ�  3.sybol(�ܺ�define)�Ѽ� 4.�@��Ѽ�
  // �p�G�Ofunction�|�^��false�A�}�l�i�J�s��function�p��C(��e�᭱���ѼƼȰ��p��)�C
  TreeNode * DealParameterType( TreeNode * para_node, string & parameter_type ) {
    int define_pos = -1;
    if ( IsBoundSymbol( para_node -> token_data, define_pos ) ) {   // ���P�_���O���OSYMBOL
      TreeNode * new_eval_tree = NULL;
      para_node = Return_NewEval_Tree( gDefineTable[define_pos].define_value, new_eval_tree );
    } // if
    else {
      if ( para_node -> token_data.token_type == "SYMBOL" ) {
        UnboundSymbolErrorInfo aUnboundSymbolErrorInfo;
        aUnboundSymbolErrorInfo.symbol_name = para_node -> token_data.token_name;
        throw aUnboundSymbolErrorInfo;
      } // if
    } // else

    //  �p�Gnode���ѼƬO�}�l�B�Ounbound�~�i�H�i�J�s��function�B��C
    if ( para_node -> isStart && define_pos == -1 ) {
      para_node = EvaluateParameter( para_node );
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
    if ( mStart_ResultSExp == NULL ) {
      SetStartResult();
      aSaveFunPara.result_Node = mStart_ResultSExp;
      mCur_ResultSExp = aSaveFunPara.result_Node;
    } // if
    else {
      aSaveFunPara.result_Node = ReSetLinkResult();
    } // else

    while ( aSaveFunPara.argument_Num > i ) {
      int para_pos = aSaveFunPara.parameter_Pos.front();
      TreeNode * real_para = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type );
      if ( i == 0 ) {
        aSaveFunPara.result_Node -> left = real_para;
        mCur_ResultSExp = aSaveFunPara.result_Node;
      } // if
      else {
        if ( parameter_type == "Parameter" ) {
          if ( real_para -> token_data.token_type == "LEFT-PAREN" ) {
            real_para -> isStart = false;
            real_para -> isEnd = false;
          } // if
          else {
            real_para -> isStart = false;
            real_para -> isEnd = true;
          } // else
        } // if
        else if ( parameter_type == "DoingQuote" ){

          if ( real_para -> left -> left -> token_data.token_type != "QUOTE" ){ //�ܦ��P���h�A�]���A���e��.
            real_para = real_para -> left;
            real_para -> isStart = false;
            real_para -> isEnd = false;
          } // if
          else {
            real_para -> isStart = false;
            real_para -> isEnd = true;
          } // else

        } // else if

        aSaveFunPara.result_Node -> right = real_para;
        mCur_ResultSExp = aSaveFunPara.result_Node -> right;
      } // else

      parameter_type = "Parameter";
      aSaveFunPara.parameter_Pos.pop();
      i++;
    } // while

    return aSaveFunPara.result_Node;
  } // EVAcons()

  TreeNode * EVAlist( SaveFunctionParameter aSaveFunPara ) {
    if ( aSaveFunPara.argument_Num == 0 ) {   // (list) �|return nil_node�C
      mAllFunPara[aSaveFunPara.fun_Pos] -> right -> isEnd = false;
      return mAllFunPara[aSaveFunPara.fun_Pos] -> right;
    } // if

    int i = 0;
    string parameter_type = "Parameter";
    if ( mStart_ResultSExp == NULL ) {
      SetStartResult();
      aSaveFunPara.result_Node = mStart_ResultSExp;
      mCur_ResultSExp = aSaveFunPara.result_Node;
    } // if
    else {
      aSaveFunPara.result_Node = ReSetLinkResult();
    } // else

    TreeNode * cur_node = aSaveFunPara.result_Node;
    TreeNode * link_node;
    while ( aSaveFunPara.argument_Num > i ) {
      int para_pos = aSaveFunPara.parameter_Pos.front();
      TreeNode * real_para = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type );
      if ( i == 0 ) {
        cur_node -> left = real_para;
        if ( aSaveFunPara.argument_Num == 1 ) cur_node -> right = mAllFunPara[para_pos] -> right;
      } // if
      else if ( aSaveFunPara.argument_Num == i + 1 ) {   // �����쩳�F�C
        if ( parameter_type == "Parameter" ) {
          link_node = ReSetLinkResult();
          cur_node -> right = link_node;
          cur_node = cur_node -> right;
          cur_node -> left = real_para;
          cur_node -> right = mAllFunPara[para_pos] -> right;
        } // if
        else {
          cur_node -> right = real_para;
        } // else

      } // else if
      else {    // ������ó]�w�s��node
        if ( parameter_type == "Parameter" ) {
          link_node = ReSetLinkResult();
          cur_node -> right = link_node;
          cur_node = cur_node -> right;
          cur_node -> left = real_para;
        } // if
        else {
          cur_node -> right = real_para;
          cur_node = cur_node -> right;
          cur_node -> right = NULL;
        } // else

      } // else

      mCur_ResultSExp = cur_node;
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
    if (  mAllFunPara[para_pos] -> left -> token_data.token_type != "LEFT-PAREN" ) {
      aSaveFunPara.result_Node = mAllFunPara[para_pos] -> left;
    } // if
    else if ( mCur_ResultSExp == NULL ) {
      aSaveFunPara.result_Node = mAllFunPara[para_pos] -> left;
      mDoingQuote = true;
    } // else if
    else if ( mCur_ResultSExp -> left == NULL && mCur_ResultSExp -> right == NULL ) {
      aSaveFunPara.result_Node = mAllFunPara[para_pos] -> left;
      mDoingQuote = true;
    } // else if
    else if ( mCur_ResultSExp -> left == NULL ) {
      aSaveFunPara.result_Node = mAllFunPara[para_pos] -> left;
      mDoingQuote = true;
    } // if
    else {
      aSaveFunPara.result_Node = mAllFunPara[para_pos];
      mDoingQuote = true;
    } // else

    return aSaveFunPara.result_Node;
  } // EVAQuote()

  void EVAdefine( SaveFunctionParameter aSaveFunPara ) {
    // ���ˬdargument_num�C
    bool define_error = false;
    int repeat_pos = -1, i = 0;
    int para_1_pos = aSaveFunPara.parameter_Pos.front();
    int para_2_pos = aSaveFunPara.parameter_Pos.back();
    TreeNode * para_1_node = mAllFunPara[para_1_pos] -> left;
    TreeNode * para_2_node = NULL;
    string symbol_name = para_1_node -> token_data.token_name;
    string symbol_type = para_1_node -> token_data.token_type;
    if ( aSaveFunPara.argument_Num != 2 ) {                         // ���ˬd�ѼƼƥءC
      define_error = true;
    } // if
    else if ( para_1_node -> token_data.token_type != "SYMBOL" ) {  // �A���ˬd�O���O�t�Ϊ�function�C
      define_error = true;
    } // else if
    else if ( IsSystemSymbol( symbol_name, symbol_type ) ) {
      define_error = true;
    } // else if
    else {                                                          // �̫��ˬd��ѼƤ��������Y�C
      while ( gDefineTable.size() > i ) {         // ���ˬd���S���Q�w�q�L�C
        if ( gDefineTable[i].define_name == symbol_name ) {
          repeat_pos = i;
        } // if

        i++;
      } // while

      string parameter_type = "Parameter";
      para_2_node = DealParameterType( mAllFunPara[para_2_pos] -> left, parameter_type ); //�ݦ��S���Q�w�q�L
    } // else

    if ( define_error ) {
      FormatErrorInfo aFormatErrorInfo;
      int fun_pos = aSaveFunPara.fun_Pos;
      aFormatErrorInfo.format_error_node = mAllFunPara[fun_pos];
      aFormatErrorInfo.format_fun_name = "DEFINE";
      throw aFormatErrorInfo;
    } // if
    else {
      if ( repeat_pos == -1 ) {
        DefineTable aDefineTable;
        aDefineTable.define_name = symbol_name;
        TreeNode * new_define_node = NULL;
        new_define_node = Build_Define_Tree( para_2_node, new_define_node );
        aDefineTable.define_value =new_define_node;
        gDefineTable.push_back( aDefineTable );
      } // if
      else {
        TreeNode * new_define_node = NULL;
        new_define_node = Build_Define_Tree( para_2_node, new_define_node );
        gDefineTable[repeat_pos].define_value = new_define_node;
      } // else

      cout << symbol_name << " defined\n";
      mPrint_SExp = false;
    } // else

  } // EVAdefine()

  TreeNode * EVAcar_cdr( string fun_name, SaveFunctionParameter aSaveFunPara ) {
    string parameter_type = "Parameter";
    int para_pos = aSaveFunPara.parameter_Pos.front();
    TreeNode * part_access = DealParameterType( mAllFunPara[para_pos] -> left, parameter_type );

    // �ˬd��part���L���~�A���~�Nthrow incorrect argument type��error�C
    if ( part_access -> token_data.token_type != "LEFT-PAREN" ) {
      ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
      aArgumentTypeErrorInfo.function_name = fun_name;
      aArgumentTypeErrorInfo.argumentype_error_name = part_access -> token_data.token_name;
      throw aArgumentTypeErrorInfo;
    } // if
    else if ( part_access -> left -> token_data.token_type == "QUOTE" ){
      ArgumentTypeErrorInfo aArgumentTypeErrorInfo;
      aArgumentTypeErrorInfo.function_name = fun_name;
      aArgumentTypeErrorInfo.argumentype_error_name = "\'";
      throw aArgumentTypeErrorInfo;
    } // else if

    TreeNode * left_part = part_access;
    TreeNode * right_part = part_access;
    // �}�l���p��A�T�{�O�_���F�Ӽh���ѼƤ~��B��C
    // (�@) ���T�w�h�ơA�A�T�w�ѼơC
    if ( fun_name == "car" ) {
      left_part = left_part -> left;
      aSaveFunPara.result_Node = left_part;
    } // if
    else {
      if ( right_part -> left -> token_data.token_type == "LEFT-PAREN" ) {

      } // if
      else {    // �p�G�O�Ĥ@�h�����p�C
        right_part = right_part -> right;
        if ( right_part -> isEnd ) {
          right_part -> isEnd = false;
          right_part -> isStart = false;
        } // if
        else {
          right_part -> isEnd = false;
          right_part -> isStart = true;
        } // else

      } // else

      aSaveFunPara.result_Node = right_part;
    } // else

    return aSaveFunPara.result_Node;
  } // EVAcar_cdr()


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
  void ArgumentNum_Check( int argument_num, string fun_name, string fun_type ) {
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
        if ( argument_num >= 1 ) check_get = true;
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
          vector<DefineTable> newTable;
          gDefineTable = newTable; // ��l��DefineTable�C
          cout << "environment cleaned" << "\n";
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

    // throw argument_num error
    if ( check_get == false ) {  // �٦���Lerror���i��ʡC
      AgumentNumberErrorInfo aAgumentNumberErrorInfo;
      aAgumentNumberErrorInfo.fun_name = fun_name;
      throw aAgumentNumberErrorInfo;
    } // if

  } // ArgumentNum_Check()

  // (�@)�ˬd -> ���L��function  (�G) �]�w�n�i�J��function���Ѽ�
  void Function_Check( TreeNode * fun_node, string & fun_name, string & fun_type ) {
    int define_pos = -1;  // �p�Gsymbol���Q�w�q���ܡA��ȷ|�Q���C
    if ( IsBoundSymbol( fun_node -> token_data, define_pos ) ) {  // ���Q�w�q�A���O���ܥN��Onon
      fun_name = gDefineTable[define_pos].define_value -> token_data.token_name;
    } // if
    else {   // �S�Q�w�q�������|�O�H�U�������A���O����1.symbol�Ounbound�A2.���Osymbol�Ononfunction�C
      fun_name = fun_node -> token_data.token_name;
    } // else

    if ( IsSystemSymbol( fun_name, fun_type ) ) {
      // ���fun_name�ó]�wfun_type�A��^�C
    } // if
    else {
      if ( define_pos == -1 ) { // ���O�w�q�A�p�G�Osymbol�Nnon-bound�A�p�G���Osymbol�Nnonfunction
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

      } // if
      else  {
        NonFunctionErrorInfo aNonFunctionErrorInfo;
        aNonFunctionErrorInfo.nonfun_name = gDefineTable[define_pos].define_value;
        throw aNonFunctionErrorInfo;
      } // else

    } // else

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
        if ( gDefineTable[i].define_name == tokendata.token_name ) {
          define_pos = i;
          return true;
        } // if

        i++;
      } // while

    } // if

    return false;
  } // IsBoundSymbol

  // �@�}�l�]�wSTART_Resutlt���Y�A
  void SetStartResult() {
    TokenData token_LP_data;
    token_LP_data.token_name = "(";
    token_LP_data.token_type = "LEFT-PAREN";
    mStart_ResultSExp = new TreeNode;
    mStart_ResultSExp -> token_data = token_LP_data;
    mStart_ResultSExp -> isStart = true;
    mStart_ResultSExp -> isEnd = false;
    mStart_ResultSExp -> left = NULL;
    mStart_ResultSExp -> right = NULL;
  } // SetStartResult()

  // �s�W�s����(���A��)
  TreeNode * ReSetLinkResult() {
    TokenData token_LP_data;
    token_LP_data.token_name = "(";
    token_LP_data.token_type = "LEFT-PAREN";
    TreeNode * newnode;
    newnode = new TreeNode;
    newnode -> left = NULL;
    newnode -> right = NULL;
    if ( mCur_ResultSExp -> left == NULL ) {
      newnode -> token_data = token_LP_data;
      newnode -> isEnd = false;
      newnode -> isStart = true;
    } // if
    else if ( mCur_ResultSExp -> right == NULL ) {
      newnode -> token_data = token_LP_data;
      newnode -> isEnd = false;
      newnode -> isStart = false;
    } // else if

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

  public:
  bool mPrint_SExp;
  Tree( vector<SystemSymbol> aSystemSymbolTable ) {
    mDoingQuote = false;
    mCurrent_pos = 0;
    mStart_ResultSExp = NULL;
    mCur_ResultSExp = NULL;
    mLevel_num = 0;
    mPrint_SExp = true;
    vector<TreeNode*> aAllFunPara;
    mAllFunPara = aAllFunPara;
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
            float pointnumber;
            pointnumber = atof( aTreeRoot -> token_data.token_name.c_str() );
            cout << fixed << setprecision( 3 ) << pointnumber << "\n";
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
          float pointnumber;
          pointnumber = atof( aTreeRoot -> token_data.token_name.c_str() );
          cout << fixed << setprecision( 3 ) << pointnumber << "\n";
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

  // catch Error��i�J�B��A�ê�^�B�⧹��reusltSExp��main()
  TreeNode * EvaluateSExp( TreeNode * inputSExp ) {
    try {
      if ( inputSExp -> left == NULL && inputSExp -> right == NULL ) {
        int define_pos = -1;
        if ( IsBoundSymbol( inputSExp -> token_data, define_pos ) ) {   // ���P�_���O���OSYMBOL
          TreeNode * new_eval_tree = NULL;
          mStart_ResultSExp = Return_NewEval_Tree( gDefineTable[define_pos].define_value, new_eval_tree );
        } // if
        else {
          if ( inputSExp -> token_data.token_type == "SYMBOL" ) {
            string none = "";
            if ( IsSystemSymbol( inputSExp -> token_data.token_name, none ) ) {
              ProcedureErrorInfo aProcedureErrorInfo;
              aProcedureErrorInfo.procedure_error_name = inputSExp -> token_data.token_name;
              throw aProcedureErrorInfo;
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
        return EvaluateParameter( inputSExp );  // ���p��ѼơA�A�p��function�C
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
    catch( ProcedureErrorInfo aProcedureErrorInfo ) {
      cout << "#<procedure " << aProcedureErrorInfo.procedure_error_name << ">\n";
      mPrint_SExp = false;
    } // catch
    catch( ArgumentTypeErrorInfo aArgumentTypeErrorInfo ) {
      cout << "ERROR (" << aArgumentTypeErrorInfo.function_name << " with incorrect argument type) : ";
      cout << aArgumentTypeErrorInfo.argumentype_error_name << "\n";
      mPrint_SExp = false;
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
  aSystemSymbol.symbol_name = "eqv";
  aSystemSymbol.symbol_type = "Eqivalence tester";
  aSystemSymbolTable.push_back( aSystemSymbol );
  aSystemSymbol.symbol_name = "equal";
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
  return aSystemSymbolTable;
} // BuildSystemSymbol()

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
      // aTree.Preorder( aTreeRoot );
      // Scanner�PPaser���۷��q��function�A��Paser�B�z���õL������N�}�l��Tree�C userŪ�� (exit)�A�]�wfinished�A�����o�������ɡC
      if ( !finished ) {
        Tree aTree( systemSymbolTable );
        TreeNode * resultSExp;            // �@���B�z�@��SExp�A�@��SExp�N��@�Ӿ𪺵��c�C
        resultSExp = aTree.EvaluateSExp( inputSExp );
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
           << aStringErrorInfo.line << " Column " << aStringErrorInfo.column << "\n";
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

    if ( !finished ) {
      cout << "\n";
      cout << "> ";
    } // if

  } // while

  cout << "\n";
  cout << "Thanks for using OurScheme!";
} // main()
