# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>
# include <stack>
# include <iomanip>  // �����X�p�Ʀ��
# include <iostream>
using namespace std;
static int utestNum = -1;

struct TokenData {                                            // token�r��Btoken�����O�C
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

struct StringErrorInfo {
  int line;
  int column;
}; // StringErrorInfo

struct NoAtomQuoteErrorInfo {
  string token_name;
  int line;
  int column;
}; // NoAtomQuoteErrorInfo

struct NoRightParen {
  string token_name;
  int line;
  int column;
}; // NoRightParen

struct ANilCheck {  // �Ψӵ�sexp�Anil�C
  int start;
}; // ANilCheck


class Scanner {                                        // �u�t�d���XGetToken()�A��PeekToken()�A�æ^�Ǹ�Token�r��C
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

    cin.get( sexpchar );
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
        cin.get( sexpchar );
        if ( sexpchar == '\n' ) {
          aStringErrorInfo.column = mscan_column + 1;
          aStringErrorInfo.line = mscan_line;
          TraceTokenLineColumn( sexpchar );
          ReadErrorLine();
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
               || nextoken_char == '(' || nextoken_char == ')' ) && !finished ) {   
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
      cin.get( getnext );
      while ( getnext != '\n' ) {
        cin.get( getnext );
      } // while

    } // if
  } // ReadErrorLine()

  int ReadEndLine() {
    char getnext = '\0';
    int i = 0;
    while ( cin.peek() == ' ' || cin.peek() == ';' ) {
      cin.get( getnext );
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
      cin.get( getnext );
      TraceTokenLineColumn( getnext );
      while ( getnext != '\n' ) {
        cin.get( getnext );
        TraceTokenLineColumn( getnext );
      } // while

    } // if
  } // ReadCommentLine()

  int CurColumn() {
    return mscan_column;
  } // CurColumn()

  int CurLine() {
    return mscan_line;
  } // CurLine()


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

    // cout <<Get_token_data.token_name << Get_token_data.token_type << "\n";
    mAccurate_token_vector.push_back( get_token_data );
    return get_token_data;
  } // GetNextTokenForParser()

  bool IsSexp( TokenData & token_IsSexp, Scanner & aScanner ) {
    string geterrorline = "";
    token_IsSexp = GetNextTokenForParser( aScanner );
    if ( IsAtom( token_IsSexp ) ) {                          // <ATOM>
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
        NoRightParen aNoRightParen;            // return NoRightParen
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
      
    return true;
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

  void TranslateToken() {      // ���J��sexp�নdotted pair���Φ��C
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
    int dotnumber = 0;
    stack<int> rpforlp;
    // �J��quote�O�o�h�[�@��(..)
    if ( mAccurate_token_vector.size() == 1 ) {
      mTranslated_token_vector.push_back( mAccurate_token_vector[0] );   // ATOM
    } // if
    else  {
      while ( mAccurate_token_vector.size() > anumber ) {
        if ( mAccurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {    // ���O���Ӥl�Τ���sexp�����X�Ӫťե�
          while ( mAccurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {
            mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
            anumber++;
          } // while
            
          if ( mAccurate_token_vector[anumber].token_type != "QUOTE" ) {
            mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );    // �s(����U�@��sexp�i���O(�F�j
            anumber++;
          } // if
      
        } // if
        else if ( mAccurate_token_vector[anumber].token_type == "QUOTE" ) {
          mTranslated_token_vector.push_back( token_LP_data );                       // ���ؤ@��(
          mTranslated_token_vector.push_back( token_QUOTE_data );                     // �ç�Quote��J�C
          rpforlp.push( 1 );                                         
          anumber++;
        } // else if
        else if ( mAccurate_token_vector[anumber].token_type == "RIGHT-PAREN" ) {
          
          if ( rpforlp.size() > 0 ) {
            mTranslated_token_vector.push_back( token_RP_data );
            rpforlp.pop();
          } // if

          mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          anumber++;
        } // else if
        else if ( mAccurate_token_vector[anumber].token_type == "DOT" ) {
          mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          anumber++;
        } // else if
        else {                   // �h�W�[�@��.(  
          int cur = mTranslated_token_vector.size() - 1; 
          bool alreadypush = false;
          if ( mTranslated_token_vector[cur].token_type != "DOT" ) {
            if ( mTranslated_token_vector[cur].token_type != "QUOTE" ) {
              mTranslated_token_vector.push_back( token_Dot_data );
              mTranslated_token_vector.push_back( token_LP_data );
              rpforlp.push( 1 );       // �O�o)�[1�A�J��)�ɪ�^�o��)�A(..)�~�ন��C
            } // if
            else if ( mTranslated_token_vector[cur].token_type == "QUOTE" ) { 
              if ( mAccurate_token_vector.size() > anumber + 1 ) {
                alreadypush = true;
                mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
                mTranslated_token_vector.push_back( token_Dot_data );
                mTranslated_token_vector.push_back( token_NIL_data );
                mTranslated_token_vector.push_back( token_RP_data );
                if ( rpforlp.size() > 0 ) rpforlp.pop();
              } // if
              else {
                mTranslated_token_vector.push_back( token_Dot_data );
                mTranslated_token_vector.push_back( token_LP_data );
                rpforlp.push( 1 );
              } // else

            } // else if
            else {
              mTranslated_token_vector.push_back( token_Dot_data );
              mTranslated_token_vector.push_back( token_LP_data );
              rpforlp.push( 1 );
              // �O�o)�[1�A�J��)�ɪ�^�o��)�A(..)�~�ন��C
            } // else
          
          } // if

          if ( !alreadypush ) mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          anumber++;               
        } // else  

      } // while
      
      while ( rpforlp.size() > 0 ) {
        mTranslated_token_vector.push_back( token_RP_data );
        rpforlp.pop();
      } // while


    } // else

    // ���i�H������ . nil or nil (Build��ɤ~�|����A���|���J�S���]�Snil�ҥHreturn�ƶq�P
    // mTranslated_token_vector �k�ظ��ƶq���ﵥ���p�A���J�L�a�j��)�C nil�Ψӷ�end�C
    // casa1 = �b)�e�A�e��Ӧp�G�O(�A�S��.�A�s�W.nil�C case2 = �b(�e�p�G�S.�A�̫�n�b������)�᭱��nil�A��K�ؾ�s�WIsEnd���L�C
    int i = 0;
    stack<ANilCheck> aNilCheck;
    bool doingDot = true;
    while ( mTranslated_token_vector.size() > i ) {
      if ( mTranslated_token_vector[i].token_type == "LEFT-PAREN" ) {  
        if ( i != 0 ) {
          ANilCheck check;
          // ���P�_�O�_�ݷs�W�A�s�W����(�e��.�A�p�G�S���ܽT�{stack���S���F��A������pop�X��++�C
          if ( mTranslated_token_vector[i-1].token_type != "DOT" ) {
            check.start = 1;
            aNilCheck.push( check );  
          } // if

        } // if
   
      } // if
      else if ( mTranslated_token_vector[i].token_type == "RIGHT-PAREN" ) {
        
        if ( mTranslated_token_vector[i-2].token_type == "LEFT-PAREN" ) {
          doingDot = true;
          mTranslated_token_vector.insert( mTranslated_token_vector.begin() + i, token_NIL_data );
          mTranslated_token_vector.insert( mTranslated_token_vector.begin() + i, token_Dot_data );
          i = i + 2;
          dotnumber++;
        } // if

        if ( dotnumber == 0 ) doingDot = false;
        if ( aNilCheck.size() > 0 && !doingDot ) {
          ANilCheck check; 
          check = aNilCheck.top();
          check.start--;
          if ( check.start == 0 ) {
            mTranslated_token_vector.insert( mTranslated_token_vector.begin() + i, token_NIL_data );
            mTranslated_token_vector.insert( mTranslated_token_vector.begin() + i, token_Dot_data );
            i = i + 2;
            aNilCheck.pop();
          } // if
          else {
            aNilCheck.pop();
            aNilCheck.push( check );           
          } // else
        } // if

        if ( doingDot ) {
          dotnumber--;
        } // if

      } // else if
      else if ( mTranslated_token_vector[i].token_type == "DOT" ) {
        doingDot = true;
        dotnumber++;       
      } // else if    

      i++;
    } // while  

  } // TranslateToken()

  TreeNode * BuildTree( TreeNode * aTreeRoot, int & i ) {
    TreeNode * newnode;
    newnode = new TreeNode;
    if ( mTranslated_token_vector[i].token_type == "DOT" ) {
      // ���^�é��k�䨫�C
      return NULL;
    } // if
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

      newnode -> left = BuildTree( newnode, i );      // �����@�Ӥlsexp(expr)����
      if ( mTranslated_token_vector.size() <= i ) return newnode;
      if ( mTranslated_token_vector[i].token_type == "DOT" ) {
        i++;
        return newnode; // �J��DOT���_
      } // if

      newnode -> right = BuildTree( newnode, i );
      return newnode;
    } // else
    
    return newnode;
  } // BuildTree()

  bool IsQuoteStart( int i, TreeNode * node ) {  
    if ( node -> token_data.token_name == "(" ) {
      if ( i - 2 != 0 ) {
        if ( mTranslated_token_vector[i-2].token_type == "QUOTE" ) {
          return true;
        } // if
      } // if

    } // if
    else {
      return false;
    } // else
      
    return false;
  } // IsQuoteStart()

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

  vector<TokenData> ReturnToken_vector() {
    return mAccurate_token_vector;
  } // ReturnToken_vector()

  void Preorder( TreeNode * aTreeRoot ) {
    if ( aTreeRoot ) {
      cout << aTreeRoot -> token_data.token_name << "\n";
      Preorder( aTreeRoot -> left );
      Preorder( aTreeRoot -> right );
    } // if

  } // Preorder()
    
}; // Parser

class Tree {
  private:
  public:
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
            
        } // if
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

        } // else

        printed = printed.assign( printed, 0, printed.size() - 2 );
        cout << printed << ")" << "\n";
      } // else if
      else if ( ( IsAtom( aTreeRoot -> token_data ) ) || aTreeRoot -> token_data.token_type == "QUOTE" ) {

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

  bool IsAtom( TokenData token_IsAtom ) {
    if ( token_IsAtom.token_type == "SYMBOL" || token_IsAtom.token_type == "INT" 
         || token_IsAtom.token_type == "FLOAT" || token_IsAtom.token_type == "STRING" 
         || token_IsAtom.token_type == "NIL" || token_IsAtom.token_type == "LEFT-PAREN RIGHT-PAREN" 
         || token_IsAtom.token_type == "T" ) {
      return true;
    } // if

    return false;
  } // IsAtom()

}; // Tree

// Scanner�PParser���۷��q��function�A��Paser�B�z���õL������N�}�l��Tree�C
TreeNode * ReadSExp( TreeNode * aTreeRoot, int & lastspace ) {  
  Scanner aScanner( lastspace );                // �Ф@��aScanner��K�ڭ̡u���v��SExp�C
  Parser aParser;                 // �Ф@��aParser�ˬd�ڭ�aScanner���X�Ӫ�token�A
  TokenData first_Token_Data;    // ��k���S�����T�A���T�Nbulidtree�A���~�N�^�ǿ��~�C
  string geterrorline = "";
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

  // vector<TokenData> ReturnToken_vector = aParser.ReturnToken_vector(); // Sexp�X�k���}�l�C
  int i = 0;
  aParser.TranslateToken();
  // aParser.PrintVector();
  aTreeRoot = aParser.BuildTree( aTreeRoot, i );
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

  while ( !finished ) {                                  
    TreeNode * aTreeRoot;               // �@���B�z�@��SExp�A�@��SExp�N��@�Ӿ𪺵��c�C
    aTreeRoot = new TreeNode;          
    aTreeRoot -> left = NULL;
    aTreeRoot -> right = NULL;   
    aTreeRoot -> isStart = false;
    aTreeRoot -> isEnd = false;                    
    try {
      aTreeRoot = ReadSExp( aTreeRoot, lastspace );  // Scanner�PPaser���۷��q��function�A��Paser�B�z���õL������N�}�l��Tree�C
      if ( aTreeRoot -> token_data.token_name == "(" && 
           aTreeRoot -> left -> token_data.token_name == "exit" && 
           aTreeRoot -> right -> token_data.token_name == "nil"  ) {   // userŪ�� (exit)�A�����o�������ɡC
        
        finished = true;
      } // if  
      
      if ( !finished ) {
        Tree aTree;
        string printed = "";
        bool firstsexp = true;
        // aTree.Preorder( aTreeRoot );
        aTree.PrintSExp( aTreeRoot, printed, firstsexp );
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
    catch( NoRightParen aNoRightParen ) {
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
