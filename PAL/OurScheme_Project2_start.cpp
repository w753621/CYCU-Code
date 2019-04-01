# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>
# include <stack>
# include <queue>
# include <iomanip>  // 控制輸出小數位數
# include <iostream>
using namespace std;
static int utestNum = -1;

struct TokenData {  // token字串、token的類別。
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

struct ANilCheck {  // 用來給sexp，nil。
  int start;
}; // ANilCheck

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
  int function_Pos;   // Is_Start=true存入左邊的node position，等到Is_END成對可以計算時才pop出。
  int quote_Num;      // bypass第一個quote用。
  int startFun_Num;  // 紀錄該function的start數目，start=end 數目才知道該function的參數找到完。
  queue<int> parameter_Pos; // 計算參數時所存的參數。
}; // SaveFunctionParameter

struct UnboundSymbolErrorInfo {
  TreeNode * symbol_name;
}; // UnboundSymbolErrorInfo

struct NonFunctionErrorInfo {
  TreeNode * nonfun_name;
}; // NonFunctionErrorInfo 

struct AgumentNumberErrorInfo {
  string eval_name;
}; // AgumentNumberErrorInfo

// 接下存的是本身文法不合法的
struct ConsListErrorInfo {
  TreeNode * error_node;
}; // ConsListErrorInfo

vector<DefineTable*> gDefineTable;

class Scanner {         // 只負責切出GetToken()，跟PeekToken()，並回傳該Token字串。
  private:
  int mscan_line, mscan_column;
  void ASeparatorType( char sexpchar, TokenData & token_data ) {
    if ( sexpchar == '(' ) {
      if ( cin.peek() == ')' ) {                    //  case1:() 偷看下一個字元，如果是')'就讀入，並把此token類別改成"NIL"。
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
    else if ( sexpchar == ';' ) {                            // 這是註解，利用ReadCommentLine()讀掉，並記錄當前scan行列。
      token_data.token_name = ";";
      token_data.token_type = "COMMENT";                                      
      ReadCommentLine();
      mCommentLine++;
    } // else if
    else if ( sexpchar == '\"' ) {                           // 這是string，小心5種case(wait deal)
      string aString = "";
      aString = aString + sexpchar;
      if ( IsString( aString ) ) {
        token_data.token_name = aString;
        token_data.token_type = "STRING";
      } // if 
      else {
        // IsString()如果false會直接throw一個StringErrorInfo回main()中。
      } // else
        
    } // else if
  } // ASeparatorType()

  // IsString()會判斷string的文法，"之後的到line-eneter之前要有"，沒的話就throw StringErrorInfo，
  bool IsString( string & aString ) { 
    bool result = false;                    // 並把後面到line-enter前全部讀掉。
    StringErrorInfo aStringErrorInfo;
    char sexpchar = '\0';
    if ( cin.peek() == '\n' ) {             // 下一個即是換行字元gg，不是quote，直接把後面的資料也給讀掉(不需要)。
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
      if ( sexpchar == '\"' ) {                   // 遇到"直接結束完成
        aString = aString + sexpchar;
        result = true;
        finished = true;
      } // if
      else if ( sexpchar == '\\' ) {            // 遇到分號字元，考慮後一個字元後是不是n、t、"、\這四種情況，是的話做適當處理，不是的話就是普通的分號字元。
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
        else {                                // 分號本身他就只是一個普通的字元而已
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
    if ( token[0] == '+' || token[0] == '-' ) {           // '+'、'-'、'數字'三種情況。
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
    // case1:.開頭 case2:+、-開頭 case3:+、-開頭第二字元為點  
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

  TokenData GetOneToken() {         // 抓下一個token
    char sexpchar = '\0';
    if ( cin.peek() == EOF ) {                                // 先確定一開始讀的不是EOF
      throw -1;  
    } // if

    // 使用一個字元下去讀，(一) 先跳過所有white space直到EOF或非White space
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
    // 這時候拿到的sexpchar是「非white space」，可能是「separator」或「非separator」，separator字元也      
    // 非separator，處理到white space(一個token) or separator(兩個token)   
    // 此token可能是1.INT、2.STRING(")、3.DOT(.)、4.FLOAT、5.NIL(nil、#f、())、6.T(t、#t)、QUOTE、SYMBOL
    if ( sexpchar == '(' || sexpchar == ')' || sexpchar == '\'' || sexpchar == '\"' || sexpchar == ';' ) {
      ASeparatorType( sexpchar, token_data );
      return token_data;
    } // if
    else { 
      string token = "";
      token = token + sexpchar;
      bool finished = false;
      while ( ( sexpchar != '\n' || sexpchar != ' ' || sexpchar != '\t' ) && !finished ) {
        // 預測下個字元如果是separator或white space，停止讀入，並處理到separator之前。
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
        else if ( !finished ) { // 繼續此Token之探索。
          cin.get( sexpchar );
          token = token + sexpchar;
          TraceTokenLineColumn( sexpchar );
        } // else if

      } // while

    } // else

    return token_data;
  } // GetOneToken()

  TokenData PeekToken() {           // Parser預先知道下一個token的function，好判斷文法走向。
    // 而在GetNextTokenForParser()中，必須把lookahead改成true，才不會又讀了一個新的token進來。     
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
  bool mlookahead;                                     // 掌管GetOneToken時的準則
  public:
  TokenData maPeekToken;
  Parser() {
    mlookahead = false;
  } // Parser()

  TokenData GetNextTokenForParser( Scanner & aScanner ) {      // 回傳這次Getoken
    TokenData get_token_data;                         
    if ( mlookahead ) {    // GetToken時，確定lookahead要是false的情況下才可以往下getoken，否則端出已讀做好處理的token進去parser
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
        // <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN 往下繼續做    
      } // if
      else {
        NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;        // return NoAtomQuoteErrorInfo
        aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
        aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
        aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line - aScanner.mCommentLine;
        aScanner.ReadErrorLine();
        throw aNoAtomQuoteErrorInfo;         
      } // else

      token_IsSexp = aScanner.PeekToken();    // 偷看下一個Token { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
      maPeekToken = token_IsSexp;              // 設定aPeekToken
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
          // 往下繼續做
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
        
      if ( mlookahead == false ) {               // 針對"DOT"的處理，再繼續Peek下一個是不是')'。
        token_IsSexp = aScanner.PeekToken();    // 偷看下一個Token RIGHT-PAREN
        maPeekToken = token_IsSexp;              // 設定aPeekToken
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

  void TranslateToken() {      // 把輸入的sexp轉成dotted pair的形式。
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
    int rpfordot = 0;
    stack<int> rpforlp;
    // 遇到quote記得多加一對(..)
    if ( mAccurate_token_vector.size() == 1 ) {
      mTranslated_token_vector.push_back( mAccurate_token_vector[0] );   // ATOM
      return;
    } // if
    else  {
      while ( mAccurate_token_vector.size() > anumber ) {
        if ( mAccurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {    // 先記錄該子或父的sexp當中有幾個空白先
          int cur = mTranslated_token_vector.size() - 1;
          if ( cur != -1 ) {
            if ( mTranslated_token_vector[cur].token_type != "DOT" ) {
              mTranslated_token_vector.push_back( token_Dot_data );
              mTranslated_token_vector.push_back( token_LP_data );
              rpfordot++;
            } // if
          } // if

          while ( mAccurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {
            mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
            anumber++;
          } // while
            
          if ( mAccurate_token_vector[anumber].token_type != "QUOTE" ) {
            mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );    // 存(之後下一個sexp【不是(了】
            anumber++;
          } // if
          else {
            mTranslated_token_vector.push_back( token_LP_data );                       // 先建一個(
            mTranslated_token_vector.push_back( token_QUOTE_data );
            anumber++;
            rpforlp.push( 1 ); 
          } // else
      
        } // if
        else if ( mAccurate_token_vector[anumber].token_type == "QUOTE" ) {
          int cur = mTranslated_token_vector.size() - 1;
          if ( cur != -1 ) {
            if ( mTranslated_token_vector[cur].token_type != "DOT" ) {
              mTranslated_token_vector.push_back( token_Dot_data );
              mTranslated_token_vector.push_back( token_LP_data );
              rpfordot++;
            } // if

          } // if

          mTranslated_token_vector.push_back( token_LP_data );                       // 先建一個(
          mTranslated_token_vector.push_back( token_QUOTE_data );                     // 並把Quote丟入。
          rpforlp.push( 1 );                                         
          anumber++;    

        } // else if
        else if ( mAccurate_token_vector[anumber].token_type == "RIGHT-PAREN" ) {
          
          if ( rpforlp.size() > 0 ) {
            mTranslated_token_vector.push_back( token_RP_data );
            rpforlp.pop();
          } // if

          while ( rpfordot > 0 ) {
            mTranslated_token_vector.push_back( token_RP_data );
            rpfordot--;
          } // while

          mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          anumber++;
        } // else if
        else if ( mAccurate_token_vector[anumber].token_type == "DOT" ) {
          mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          anumber++;
        } // else if
        else {                   // 多增加一對.(  
          int cur = mTranslated_token_vector.size() - 1; 
          bool alreadypush = false;
          if ( mTranslated_token_vector[cur].token_type != "DOT" 
               && mTranslated_token_vector[cur].token_type != "QUOTE" ) {
            mTranslated_token_vector.push_back( token_Dot_data );
            mTranslated_token_vector.push_back( token_LP_data );
            rpfordot++;      // 記得)加1，遇到)時返回這些)，(..)才能成對。
          } // if  
          else if ( mTranslated_token_vector[cur].token_type == "QUOTE" ) { 
            if ( mAccurate_token_vector.size() > anumber ) {
              mTranslated_token_vector.push_back( token_Dot_data );
              mTranslated_token_vector.push_back( token_LP_data );
              rpfordot++;
              mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
              mTranslated_token_vector.push_back( token_RP_data );
              alreadypush = true;
            } // if

          } // else if
          
          if ( !alreadypush ) mTranslated_token_vector.push_back( mAccurate_token_vector[anumber] );
          anumber++;               
        } // else  

      } // while

      while ( rpforlp.size() > 0 ) {
        mTranslated_token_vector.push_back( token_RP_data );
        rpforlp.pop();
      } // while

      while ( rpfordot > 0 ) {
        mTranslated_token_vector.push_back( token_RP_data );
        rpfordot--;
      } // while

    } // else

    // 讓可以有尾巴 . nil or nil (Build樹時才會完整，不會陷入沒有因沒nil所以return數量與
    // mTranslated_token_vector 右誇號數量不對等情況，陷入無窮迴圈)。 nil用來當end。
    // casa1 = 在)前，前兩個如果是(，沒有.，新增.nil。 case2 = 在(前如果沒.，最後要在相應的)後面給nil，方便建樹新增IsEnd給他。
    int i = 0;
    stack<ANilCheck> aNilCheck;
    bool doingDot = true;
    while ( mTranslated_token_vector.size() > i ) {
      if ( mTranslated_token_vector[i].token_type == "LEFT-PAREN" ) {  
        if ( i != 0 ) {
          ANilCheck check;
          // 先判斷是否需新增，新增條件為(前為.，如果沒的話確認stack有沒有東西，有的話pop出來++。
          if ( mTranslated_token_vector[i-1].token_type != "DOT" ) {
            check.start = 1;
            aNilCheck.push( check );  
          } // if

        } // if
   
      } // if
      else if ( mTranslated_token_vector[i].token_type == "RIGHT-PAREN" ) {
        
        if ( mTranslated_token_vector[i-2].token_type == "LEFT-PAREN" 
             || mTranslated_token_vector[i-2].token_type == "QUOTE" ) {
                 
          doingDot = true;
          mTranslated_token_vector.insert( mTranslated_token_vector.begin() + i, token_NIL_data );
          mTranslated_token_vector.insert( mTranslated_token_vector.begin() + i, token_Dot_data );
          i = i + 2;
          dotnumber++;
        } // if

        if ( dotnumber == 0 ) doingDot = false;
        if ( aNilCheck.size() > 0 && !doingDot ) {    
          mTranslated_token_vector.insert( mTranslated_token_vector.begin() + i, token_NIL_data );
          mTranslated_token_vector.insert( mTranslated_token_vector.begin() + i, token_Dot_data );
          i = i + 2;
          aNilCheck.pop();
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

    // cout << dotnumber;
    // cout << aNilCheck.size();

  } // TranslateToken()

  TreeNode * BuildTree( TreeNode * aTreeRoot, int & i ) {
    TreeNode * newnode;
    newnode = new TreeNode;
    if ( mTranslated_token_vector.size() <= i ) {
      return NULL;
    } // if
    else if ( mTranslated_token_vector[i].token_type == "DOT" ) {
      // 樹返回並往右邊走。
      return NULL;
    } // else if
    else {
      newnode -> right = NULL;
      newnode -> left = NULL;
      newnode -> isStart = false;
      newnode -> isEnd = false;
      newnode -> token_data = mTranslated_token_vector[i];
      if ( mTranslated_token_vector.size() == 1 ) return newnode;
      // 設定Sexp開始，(前一個沒.代表Start
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
      // 預測下一個是不是)，是的話直接設為IsEnd並讀完所有)
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

      newnode -> left = BuildTree( newnode, i );      // 做完一個子sexp(expr)之後
      if ( mTranslated_token_vector.size() <= i ) return newnode;
      if ( mTranslated_token_vector[i].token_type == "DOT" ) {
        i++;
        return newnode; // 遇到DOT中斷
      } // if
      
      newnode -> right = BuildTree( newnode, i );
      return newnode;
    } // else
    
    return newnode;
  } // BuildTree()

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
    vector<TreeNode*> mAllFunctionParameter;  // 計算參數時，所有存入的function & parameter。
    stack<SaveFunctionParameter> mSave_Table; // 紀錄Function位置(Function_Pos)，Function計算參數何時結束(startFun_Num)
                                              // 紀錄Parameter位置(Parameter_Pos)。
    TreeNode * mResultSExp;   // 一次處理一個SExp，一個SExp代表一個樹的結構。
    bool mOnlyQuote;          // 該sexp只有quote，計算多餘的第一個quote用。
    bool mPrintSExp;          // 控制需不需要印出sexp用。
    bool mOneEvaluate;
  // KEY:遇到QUOTE就是一個List，不然下一個Is_Start左邊接的就是function。


  
  void EvaluateParameter( TreeNode * inputSExp, int & current_pos ) { 
    if ( inputSExp ) {
      mAllFunctionParameter.push_back( inputSExp );  
      SaveFunctionParameter aSaveFunPara;
      if ( mSave_Table.size() > 0 ) aSaveFunPara = mSave_Table.top();
      bool pushfunpara = true;

      if ( inputSExp -> isStart ) {
        DealmStartFun_Number( "ADD", aSaveFunPara );
        if ( inputSExp -> left -> token_data.token_name == "quote"//整個quote裡面存的就是一個參數需繼續走訪。
             || inputSExp -> left -> token_data.token_type == "quote" ) {
          if ( aSaveFunPara.quote_Num == 1 ) {                 // 第一個已經bypass了，設定第二個start!。
            aSaveFunPara.parameter_Pos.push( current_pos );   // 這時候存的參數是(.....
          } // if

          aSaveFunPara.quote_Num++;
        } // if
        else { 
          if ( aSaveFunPara.quote_Num == 0 ) {             // 新的function，左邊接的node是function ^0^。
            SaveFunctionParameter newSaveFunPara;
            newSaveFunPara.function_Pos = current_pos + 1; // 下一個即是function。    
            newSaveFunPara.startFun_Num = 1;               // 此function層開始往下計算層數xd。
            aSaveFunPara.quote_Num = 0;                    // 剛開始quote = 0
            mSave_Table.push( newSaveFunPara );            // 完成一個function位置的傳遞。
            pushfunpara = false;                           // 因為是新增一個function
          } // if
          else if ( aSaveFunPara.quote_Num == 1 ) {        // 第一個已經bypass了，設定參數!。
            aSaveFunPara.parameter_Pos.push( current_pos );   // 這時候存的參數是(.....
          } // else if

          mOnlyQuote = false;  
        } // else

      } // if  
      else if ( inputSExp -> isEnd ) {   
        DealmStartFun_Number( "SUB", aSaveFunPara );
        // >>.結尾<< 這種形式(也一樣要帶入參數)
        if ( inputSExp -> token_data.token_type != "NIL" ) aSaveFunPara.parameter_Pos.push( current_pos );
        int finish = aSaveFunPara.startFun_Num;
        if ( finish == 0 ) {
          EvaluateFunction( aSaveFunPara );
          mSave_Table.pop(); // 確定會有push的情況下(start與end成對)。
          pushfunpara = false;
        } // if
        else if ( finish == 1 ) {   // 要回到第一層了，quote_Num值記得初始化。
          aSaveFunPara.quote_Num = 0;
        } // else if

      } // else if
      else if ( inputSExp -> left == NULL && inputSExp -> right == NULL ) {  // 存參數
      // 兩種情況才可以加入，(一)不能存到funtion (二) 到quote結束之前 皆不存
        if ( aSaveFunPara.startFun_Num == 1 
             && mAllFunctionParameter[current_pos-1] -> isStart == false ) {
          aSaveFunPara.parameter_Pos.push( current_pos );
        } // if

      } // else if

      if ( mSave_Table.size() > 0 && pushfunpara ) {
        mSave_Table.pop();
        mSave_Table.push( aSaveFunPara );
      } // if  

      current_pos++;
      EvaluateParameter( inputSExp -> left, current_pos );
      EvaluateParameter( inputSExp -> right, current_pos );
    } // if

  } // EvaluateParameter()

  void DealmStartFun_Number( string addorsub, SaveFunctionParameter & aSaveFunPara ) {
    if ( aSaveFunPara.startFun_Num > 0 ) {
      if ( addorsub == "ADD" ) {
        aSaveFunPara.startFun_Num++;
      } // if
      else if ( addorsub == "SUB" ) {
        aSaveFunPara.startFun_Num--;
      } // else if

    } // if
  } // DealmStartFun_Number()


  // (一)檢查有無此function  (二) 設定要進入此function的參數
  void EvaluateFunction( SaveFunctionParameter aSaveFunPara ) {
    if ( mOnlyQuote ) {
      mResultSExp = mAllFunctionParameter[aSaveFunPara.parameter_Pos.front()]; // 只會有一個參數。
    } // if
    else {
      int fun_lastpos = aSaveFunPara.function_Pos;
      int defineError = -1;
      string fun_type = ""; 
      string fun_name = "";
      if ( IsFunction( fun_lastpos, fun_name, fun_type, defineError ) ) {


      } // if
      else {
        NonFunctionErrorInfo aNonFunctionErrorInfo;
        if ( defineError == -1 ) aNonFunctionErrorInfo.nonfun_name = mAllFunctionParameter[fun_lastpos];
        else aNonFunctionErrorInfo.nonfun_name = gDefineTable[defineError] -> define_value;
        throw aNonFunctionErrorInfo;
      } // else

    } // else
    
    mOneEvaluate = false;
  } // EvaluateFunction()

  bool IsFunction( int fun_lastpos, string & fun_name, string & fun_type, int & defineError ) {
    fun_name = mAllFunctionParameter[fun_lastpos] -> token_data.token_name;
    int i = 0;
    while ( gDefineTable.size() > i ) {
      if ( gDefineTable[i] -> define_name == fun_name ) {
        fun_name = gDefineTable[i] -> define_value -> token_data.token_name;
        defineError = i;
      } // if

      i++;
    } // while

    if ( fun_name == "cons" || fun_name == "list" ) {
      fun_type = "Constructors";
    } // if
    else if ( fun_name == "define" ) {
      fun_type = "Bounding";
    } // else if  
    else if ( fun_name == "car" || fun_name == "cdr" ) {
      fun_type = "Part accessors";
    } // else if  
    else if ( fun_name == "atom?" || fun_name == "pair?" || fun_name == "list?" || fun_name == "null?"
              || fun_name == "integer?" || fun_name == "real?" || fun_name == "number?" 
              || fun_name == "string?" || fun_name == "boolean" || fun_name == "symbol" ) {
      fun_type = "Primitive predicates";          
    } // else if
    else if ( fun_name == "+" || fun_name == "-" || fun_name == "*" || fun_name == "/" ) {
      fun_type = "Number arithmetic";
    } // else if
    else if ( fun_name == "not" || fun_name == "and" || fun_name == "or" ) {
      fun_type = "Logical";
    } // else if
    else if ( fun_name == ">" || fun_name == ">=" || fun_name == "<" || fun_name == "<=" 
              || fun_name == "=" ) {
      fun_type = "Number compare";
    } // else if            
    else if ( fun_name == "string-append" || fun_name == "string>?" || fun_name == "string<?" 
              || fun_name == "string=?" ) {
      fun_type = "String compare";
    } // else if
    else if ( fun_name == "eqv" || fun_name == "equal" || ) {
      fun_type = "Eqivalence tester";
    } // else if
    else if ( fun_name == "begin" ) {
      fun_type = "Sequencing";
    } // else if
    else if ( fun_name == "if" || fun_name == "cond" ) {
      fun_type = "Conditionals";
    } // else if
    else {
      return false;
    } // else

    return true;
  } // IsFunction()

  bool IsUnBoundSymbol( TokenData tokendata, int & define_pos ) {
    if ( tokendata.token_type == "SYMBOL" ) {
      int i  = 0;
      while ( gDefineTable.size() > i ) {
        if ( gDefineTable[i] -> define_name == tokendata.token_name ) {
          tokendata.token_name = gDefineTable[i] -> define_value -> token_data.token_name;
          define_pos = i;
        } // if

        i++;
      } // while

    } // if

    return false;
  } // IsUnBoundSymbol()

  public:
  Tree() {
    mOnlyQuote = true;
    mPrintSExp = true;
    mOneEvaluate = true;
  } // Tree()

  // 印成list-like formate
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
        // 如果是nil的話，表示該sexp沒有點，其他則代表有點。
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

  bool IsAtom( TokenData token_IsAtom ) {
    if ( token_IsAtom.token_type == "SYMBOL" || token_IsAtom.token_type == "INT" 
         || token_IsAtom.token_type == "FLOAT" || token_IsAtom.token_type == "STRING" 
         || token_IsAtom.token_type == "NIL" || token_IsAtom.token_type == "LEFT-PAREN RIGHT-PAREN" 
         || token_IsAtom.token_type == "T" ) {
      return true;
    } // if

    return false;
  } // IsAtom()

  // catch Error跟進入運算，並返回運算完的reusltSExp給main()  
  TreeNode * EvaluateSExp( TreeNode * inputSExp, bool & printSExp ) {
    try {
      int current_pos = 0;
      if ( inputSExp -> left == NULL && inputSExp -> right == NULL ) {
        if ( IsUnBoundSymbol( inputSExp -> token_data, ) )      
        mResultSExp = inputSExp;
      } // if
      else {
        EvaluateParameter( inputSExp, current_pos );  // 先計算參數，再計算function。
      } // else 
    
    } // try
    catch( NonFunctionErrorInfo aNonFunctionErrorInfo ) {
      cout << "ERROR (attempt to apply non-function) : ";
      mResultSExp = aNonFunctionErrorInfo.nonfun_name;
    } // catch

    return mResultSExp;
  } // EvaluateSExp()

}; // Tree

// Scanner與Parser互相溝通的function，等Paser處理完並無錯之後就開始建Tree。
TreeNode * ReadSExp( TreeNode * aTreeRoot, int & lastspace, bool & finished_exit  ) {  
  Scanner aScanner( lastspace );                // 創一個aScanner方便我們「切」此SExp。
  Parser aParser;                 // 創一個aParser檢查我們aScanner切出來的token，
  TokenData first_Token_Data;    // 文法有沒有正確，正確就bulidtree，錯誤就回傳錯誤。
  string geterrorline = "";
  if ( aParser.IsSexp( first_Token_Data, aScanner ) ) {   // 判斷Sexp的合法性，並且如果該完成的Sexp後面沒東西讀掉。
    lastspace = aScanner.ReadEndLine();
  } // if
  else {
    NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;          // 如果第一個SExp就出錯。
    aNoAtomQuoteErrorInfo.token_name = first_Token_Data.token_name;
    aNoAtomQuoteErrorInfo.column = first_Token_Data.token_column;
    aNoAtomQuoteErrorInfo.line = first_Token_Data.token_line;
    aScanner.ReadErrorLine();
    throw aNoAtomQuoteErrorInfo;
  } // else

  vector<TokenData> returnToken_vector = aParser.ReturnToken_vector(); // Sexp合法的開始。
  if ( returnToken_vector.size() == 3 ) {
    if ( returnToken_vector[0].token_name == "(" 
         && returnToken_vector[1].token_name == "exit"
         && returnToken_vector[2].token_name == ")" ) {
      finished_exit = true;
      return aTreeRoot;
    } // if

  } // if
        
  int i = 0;
  aParser.TranslateToken();
  // aParser.PrintVector();
  aTreeRoot = aParser.BuildTree( aTreeRoot, i );
  return aTreeRoot;
} // ReadSExp()

int main() {
  char linenter = '\0';
  scanf( "%d%c", &utestNum, &linenter );  // 讀取一個testnum和一個line-enter。                        
  cout << "Welcome to OurScheme!\n\n";
  cout << "> ";
  bool finished = false;
  int lastspace = 0;
  if ( cin.peek() == EOF ) {            // 先確定一開始測試檔中有資料，無就直接印錯誤，有就繼續往下走。
    cout << "ERROR (no more input) : END-OF-FILE encountered";
  } // if

  while ( !finished ) {                                  
    TreeNode * inputSExp;               // 一次處理一個SExp，一個SExp代表一個樹的結構。
    inputSExp = new TreeNode;          
    inputSExp -> left = NULL;
    inputSExp -> right = NULL;   
    inputSExp -> isStart = false;
    inputSExp -> isEnd = false;                    
    try {
      inputSExp = ReadSExp( inputSExp, lastspace, finished );  
      // aTree.Preorder( aTreeRoot );
      // Scanner與Paser互相溝通的function，等Paser處理完並無錯之後就開始建Tree。 user讀到 (exit)，設定finished，結束這次測試檔。      
      if ( !finished ) {
        Tree aTree;
        TreeNode * resultSExp;               // 一次處理一個SExp，一個SExp代表一個樹的結構。
        bool printSExp = true;
        resultSExp = aTree.EvaluateSExp( inputSExp, printSExp );
        bool firstsexp = true;
        string printed = "";
        if ( printSExp ) aTree.PrintSExp( resultSExp, printed, firstsexp );
      } // if           

    } // try
    catch( int endoffile ) {         // error1(int): no more input。
      cout << "ERROR (no more input) : END-OF-FILE encountered";
      finished = true;
      lastspace = 0;
    } // catch
    catch( StringErrorInfo aStringErrorInfo ) {   // error2: scanner處理string的時候，有可能no closing quote
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
