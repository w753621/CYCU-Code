# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>
# include <iostream>
using namespace std;
static int testNum = -1;

struct TokenData {                                            // token字串、token的類別。
  int token_line = 0;
  int token_column = 0;
  string token_name = "";
  string token_type = "";
}; // TokenData

struct TreeNode {
  TokenData Token_Data;
  TreeNode * left = NULL;
  TreeNode * right = NULL;
}; // TreeNode

struct StringErrorInfo {
  int line = 0;
  int column = 0;
}; // StringErrorInfo

struct NoAtomQuoteErrorInfo {
  string token_name = "";
  int line = 0;
  int column = 0;
}; // NoAtomQuoteErrorInfo

struct NoRightParen {
  string token_name = "";
  int line = 0;
  int column = 0;
}; // NoRightParen


class Scanner {                                                 // 只負責切出GetToken()，跟PeekToken()，並回傳該Token字串。
  private:
    int scan_line = 1, scan_column = 0;
    void aSeparatorType( char sexpchar, TokenData & token_data ) {
      if ( sexpchar == '(' ) {
        if ( cin.peek() == ')' ) {                    //  case1:() 偷看下一個字元，如果是')'就讀入，並把此token類別改成"NIL"。
          char sexpchar = '\0';
          cin.get( sexpchar );
          TraceTokenLineColumn( sexpchar );
          token_data.token_name = "nil";
          token_data.token_type = "NIL";
        } // if 
        else if ( cin.peek() == ' ' ) {               //  case2: ( ) LEFT-PAREN RIGHT-PAREN
          char sexpchar = '\0';
          cin.get( sexpchar );
          TraceTokenLineColumn( sexpchar );
          if ( cin.peek() == ')' ) {
            cin.get( sexpchar );
            TraceTokenLineColumn( sexpchar );
            token_data.token_name = "( )";
            token_data.token_type = "LEFT-PAREN RIGHT-PAREN";
          } // if
          else {
            token_data.token_name = "(";                   // case3: ( LEFT-PAREN
            token_data.token_type = "LEFT-PAREN";
          } // else

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

    } // aSeparatorType()

    bool IsString( string & aString ) {                         // IsString()會判斷string的文法，"之後的到line-eneter之前要有"，沒的話就throw StringErrorInfo，
      bool result = false;                                      // 並把後面到line-enter前全部讀掉。
      StringErrorInfo aStringErrorInfo;
      char sexpchar = '\0';
      if ( cin.peek() == '\n' ) {                               // 下一個即是換行字元gg，不是quote，直接把後面的資料也給讀掉(不需要)。
        cin.get( sexpchar );
        aStringErrorInfo.column = scan_column + 1;
        aStringErrorInfo.line = scan_line;
        TraceTokenLineColumn( sexpchar );
        ReadErrorLine();
        throw aStringErrorInfo;
      } // if

      cin.get( sexpchar );
      TraceTokenLineColumn( sexpchar );
      while ( sexpchar != '\n' ) {
        if ( sexpchar == '\"' ) {                   // 遇到"直接結束完成
          aString = aString + sexpchar;
          result = true;
          break;
        } // if
        else if ( sexpchar == '\\' ) {            // 遇到分號字元，考慮後一個字元後是不是n、t、"、\這四種情況，是的話做適當處理，不是的話就是普通的分號字元。
          if ( cin.peek() == 'n' ) {
            cin.get( sexpchar );
            aString = aString + "\n";
            scan_column = scan_column + 2;  
          } // if
          else if ( cin.peek() == 't' ) {
            cin.get( sexpchar );
            aString = aString + "\t";
            scan_column = scan_column + 8;  
          } // else if      
          else if ( cin.peek() == '\"' ) {
            cin.get( sexpchar );
            aString = aString + "\"";
            scan_column = scan_column + 2;  
          } // else if
          else if ( cin.peek() == '\\' ) {      
            cin.get( sexpchar );
            aString = aString + "\\";
            scan_column = scan_column + 2;  
          } // else if
          else {                                // 分號本身他就只是一個普通的字元而已
            aString = aString + sexpchar;  
          } // else

        } // else if
        else {
          aString = aString + sexpchar;
        } // else 

        cin.get( sexpchar );
        if ( sexpchar == '\n' ) {
          aStringErrorInfo.column = scan_column + 1;
          aStringErrorInfo.line = scan_line;
          TraceTokenLineColumn( sexpchar );
          ReadErrorLine();
          throw aStringErrorInfo;
        } // if
        else {
          TraceTokenLineColumn( sexpchar );
        } // else

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

      while ( token[i] ) {    
        number = token[i];
        if ( number >= 48 && number <= 57 ) {
          intstring = intstring + token[i];
          result = true;
        } // if
        else {
          result = false;
          break;
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
      if ( token[0] == '+' || token[0] == '-' || token[0] == '.' ) {           // case1:.開頭 case2:+、-開頭 case3:+、-開頭第二字元為點 
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

      while ( token[i] ) {    
        number = token[i];
        if ( number >= 48 && number <= 57 ) {
          floatstring = floatstring + token[i];
          result = true;
        } // if
        else if ( token[i] == '.' && !(hasDotOnce) ) {
          floatstring = floatstring + token[i];
          result = true;
          hasDotOnce = true;
        } // else if
        else {
          hasDotOnce = false;
          result = false;
          break;
        } // else if

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
        scan_line++;
        scan_column = 0;
      } // if
      else if ( sexpchar == '\t' ) {
        scan_column = scan_column + 8;
      } // else if      
      else {
        scan_column++;
      } // else

    } // TraceTokenLineColumn()

  public:
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
 
      token_data.token_column = scan_column;
      token_data.token_line = scan_line;
      // 這時候拿到的sexpchar是「非white space」，可能是「separator」或「非separator」，如果已經是個separator直接處理。
      if ( sexpchar == '(' || sexpchar == ')' || sexpchar == '\'' || sexpchar == '\"' || sexpchar == ';' ) {
        aSeparatorType( sexpchar, token_data );
        return token_data;  
      } // if
      else {        
        // 非separator，處理到white space(一個token) or separator(兩個token)   
        // 此token可能是1.INT、2.STRING(")、3.DOT(.)、4.FLOAT、5.NIL(nil、#f、())、6.T(t、#t)、QUOTE、SYMBOL
        string token = "";
        token = token + sexpchar;
        while ( sexpchar != '\n' || sexpchar != ' ' || sexpchar != '\t' ) {
          // 預測下個字元如果是separator，停止讀入，並處理到separator之前。
          char nextoken_char = '\0';
          nextoken_char = cin.peek();
          if ( nextoken_char == '(' || nextoken_char == ')' || nextoken_char == '\'' || nextoken_char == '\"' || nextoken_char == ';' ) {   
            if ( IsDOT( token, token_data ) );                                                                  
            else if ( IsINT( token, token_data ) );
            else if ( IsFLOAT( token, token_data ) );
            else if ( IsT( token, token_data ) );
            else if ( IsNIL( token, token_data ) );
            else {                                                                                                 
              token_data.token_name = token;
              token_data.token_type = "SYMBOL";  
            } // else 

            break;
          } // if
 
          if ( nextoken_char == '\n' || nextoken_char == ' ' || nextoken_char == '\t' ) {                                     // interrupt 一種token處理
            if ( IsDOT( token, token_data ) );                                                                  
            else if ( IsINT( token, token_data ) );
            else if ( IsFLOAT( token, token_data ) );
            else if ( IsT( token, token_data ) );
            else if ( IsNIL( token, token_data ) );
            else {                                                                                            // 剩下的就會被歸類在SYMBOL
              token_data.token_name = token;
              token_data.token_type = "SYMBOL";
            } // else 

            break;
          } // if 
          else {                                                                                                   //  繼續此token的探索
            cin.get( sexpchar ); 
            token = token + sexpchar;
            TraceTokenLineColumn( sexpchar );
          } // else 

        } // while

      } // else

      return token_data;
    } // GetOneToken()

    TokenData PeekToken() {              // Parser預先知道下一個token的function，好判斷文法走向。
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
      if ( scan_column != 0 ) {
        cin.get( getnext );
        while ( getnext != '\n' ) {
          cin.get( getnext );
        } // while

      } // if

    } // ReadErrorLine()

    void ReadCommentLine() {
      char getnext = '\0';
      if ( scan_column != 0 ) {
        cin.get( getnext );
        TraceTokenLineColumn( getnext );
        while ( getnext != '\n' ) {
          cin.get( getnext );
          TraceTokenLineColumn( getnext );
        } // while

      } // if

    } // ReadCommentLine()

    int CurColumn() {
      return scan_column;
    } // CurColumn()

    int CurLine() {
      return scan_line;
    } // CurLine()


}; // Scanner

class Parser {
  private:
    vector<TokenData> Accurate_token_vector;
    vector<TokenData> Translated_token_vector;
    bool lookahead = false;                                     // 掌管GetOneToken時的準則
  public:
    TokenData aPeekToken;

    TokenData GetNextTokenForParser( Scanner & aScanner ) {      // 回傳這次Getoken
      TokenData Get_Token_Data;                         
      if ( lookahead ) {                                      // GetToken時，確定lookahead要是false的情況下才可以往下getoken，否則端出已讀做好處理的token進去parser
        Get_Token_Data = aPeekToken;
        lookahead = false;    
      } // if
      else {
        Get_Token_Data = aScanner.GetOneToken();
        while ( Get_Token_Data.token_type == "COMMENT" ) {
          Get_Token_Data = aScanner.GetOneToken();
        } // while

      } // else

      // cout << Get_Token_Data.token_name << Get_Token_Data.token_type << "\n";
      Accurate_token_vector.push_back( Get_Token_Data );
      return Get_Token_Data;
    } // GetNextTokenForParser()

    bool IsSexp( TokenData & token_IsSexp, Scanner & aScanner ) {
      string geterrorline = "";
      token_IsSexp = GetNextTokenForParser( aScanner );
      if ( IsAtom( token_IsSexp ) ) {                             // <ATOM>
        return true;
      } // if
      else if ( token_IsSexp.token_type == "LEFT-PAREN" ) {       // LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
        
        if ( IsSexp( token_IsSexp, aScanner ) ) {                 // <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
          // 往下繼續做                                                          
        } // if
        else {
          NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;                                                   // return NoAtomQuoteErrorInfo
          aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
          aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
          aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line;
          aScanner.ReadErrorLine();
          throw aNoAtomQuoteErrorInfo;         
        } // else

        token_IsSexp = aScanner.PeekToken();    // 偷看下一個Token { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
        aPeekToken = token_IsSexp;              // 設定aPeekToken
        lookahead = true;
        if ( token_IsSexp.token_type != "RIGHT-PAREN" && token_IsSexp.token_type != "DOT" ) {
          bool loop = true;
          while ( loop ) {    // { <S-exp> }
            if ( IsSexp( token_IsSexp, aScanner ) ) {
              token_IsSexp = aScanner.PeekToken();
              aPeekToken = token_IsSexp;
              lookahead = true;
              if ( token_IsSexp.token_type == "RIGHT-PAREN" || token_IsSexp.token_type == "DOT" ) {
                loop = false;
                break;
              } // if
            } // if
            else {
              NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;                                               // return NoAtomQuoteErrorInfo
              aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
              aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
              aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line;
              getline( cin, geterrorline );
              throw aNoAtomQuoteErrorInfo; 
            } // else

          } // while

        } // if                

        if ( token_IsSexp.token_type == "DOT" ) {
          Accurate_token_vector.push_back( token_IsSexp );
          lookahead = false;
          if ( IsSexp( token_IsSexp, aScanner ) ) {
            // 往下繼續做
          } // if
          else {
            NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;                                               // return NoAtomQuoteErrorInfo
            aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
            aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
            aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line;
            aScanner.ReadErrorLine();
            throw aNoAtomQuoteErrorInfo; 
          } // else

        } // if
        
        if ( lookahead == false ) {               // 針對"DOT"的處理，再繼續Peek下一個是不是')'。
          token_IsSexp = aScanner.PeekToken();    // 偷看下一個Token RIGHT-PAREN
          aPeekToken = token_IsSexp;              // 設定aPeekToken
          lookahead = true;
        } // if

        if ( token_IsSexp.token_type == "RIGHT-PAREN" ) { 
          Accurate_token_vector.push_back( token_IsSexp );
          lookahead = false;
          return true;
        } // if
        else {
          NoRightParen aNoRightParen;                                                                 // return NoRightParen
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
          getline( cin, geterrorline );
          throw aNoAtomQuoteErrorInfo;
        } // else

      } // else if
      else {
        return false;
      } // else 
      
      return true;
    } // IsSexp()

    bool IsAtom( TokenData token_IsAtom ) {
      if ( token_IsAtom.token_type == "SYMBOL" || token_IsAtom.token_type == "INT" || token_IsAtom.token_type == "FLOAT" || 
           token_IsAtom.token_type == "STRING" || token_IsAtom.token_type == "NIL" || token_IsAtom.token_type == "LEFT-PAREN RIGHT-PAREN" ||
           token_IsAtom.token_type == "T" ) {
        return true;
      } // if

      return false;
    } // IsAtom()

    void TranslateToken() {
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
      int anumber = 0;
      int leftparen = 0, rightparen = 0, extraparen = 0;
      // 遇到quote記得多加一對(..)
      if ( Accurate_token_vector.size() == 1 ) {
        Translated_token_vector.push_back( Accurate_token_vector[0] );   // ATOM
      } // if
      else  {
        while ( Accurate_token_vector.size() > anumber ) {
          if ( Accurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {        // 先記錄該子或父的sexp當中有幾個空白先
            while ( Accurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {
              Translated_token_vector.push_back( Accurate_token_vector[anumber] );
              leftparen++;
              anumber++;
            } // while
            
            if ( Accurate_token_vector[anumber].token_type != "QUOTE" ) {
              Translated_token_vector.push_back( Accurate_token_vector[anumber] );    // 存(之後下一個sexp【不是(了】
              anumber++;
            } // if
      
          } // if
          else if ( Accurate_token_vector[anumber].token_type == "QUOTE" ) {
            Translated_token_vector.push_back( token_LP_data );                       // 先建一個(
            Translated_token_vector.push_back( Accurate_token_vector[anumber] );      // 並把Quote丟入
            extraparen++;                                                             // 然後把extraparen
            anumber++;
          } // else if
          else if ( Accurate_token_vector[anumber].token_type == "RIGHT-PAREN" ) {
            rightparen++;

            while ( extraparen > 0 ) {
              Translated_token_vector.push_back( token_RP_data );
              extraparen--;
            } // while

            Translated_token_vector.push_back( Accurate_token_vector[anumber] );
            anumber++;
          } // else if
          else if ( Accurate_token_vector[anumber].token_type == "DOT" ) {
            Translated_token_vector.push_back( Accurate_token_vector[anumber] );
            anumber++;
          } // else if
          else {                                                                   // 多增加一對.(  
            bool connectnil = false;
            if ( Accurate_token_vector[anumber-1].token_type != "DOT" ) {
              connectnil = true;
              Translated_token_vector.push_back( token_Dot_data );
              Translated_token_vector.push_back( token_LP_data );
              extraparen++;                                                          // 記得)加1，遇到)時返回這些)，(..)才能成對。
            } // if

            Translated_token_vector.push_back( Accurate_token_vector[anumber] );
            anumber++;      
            if ( Accurate_token_vector[anumber].token_type == "RIGHT-PAREN" && connectnil ) {    // 製作.nil，在最後後面接nil。
              Translated_token_vector.push_back( token_Dot_data );
              Translated_token_vector.push_back( token_NIL_data );
            } // if                  

          } // else  

        } // while

        while ( extraparen > 0 ) {
          Translated_token_vector.push_back( token_RP_data );
          extraparen--;
        } // while

      } // else

    } // TranslateToken()

    TreeNode * BuildTree( TreeNode * TreeRoot, int & i, int & lpnumber ) {
      TreeNode * newnode;
      if ( Translated_token_vector[i].token_name == "." || Translated_token_vector[i].token_name == ")" ) {

        if ( Translated_token_vector[i].token_name == "." ) {
          i++;
          return NULL;
        } // if
        else {
          while ( lpnumber > 0 ) {
            i++;
            lpnumber--;
          } // while

          if ( Translated_token_vector.size() > i + 1 ) {       // 如果後面還有東西，繼續判斷、繼續做!!!
            i++;
          } // if

          return NULL;
        } // else

      } // if
      else {
        newnode = new TreeNode;
        newnode -> Token_Data = Translated_token_vector[i];
        i++;
        if ( Translated_token_vector.size() == 1 ) return newnode; 
          newnode -> left = BuildTree( newnode, i, lpnumber );                    // 做完一個子sexp(expr)之後
        if ( newnode -> Token_Data.token_name != "(" ) {              
          return newnode;
        } // if
        else if ( Translated_token_vector[i].token_name == "." ) {    // 忽略DOT
          i++;
        } // if
        else if ( newnode -> Token_Data.token_name == "(" ) {                 // 解決一對sexp問題，遇到)之前，必須計算已經遇到幾次(了
          lpnumber++;
        } // else if

        newnode -> right = BuildTree( newnode, i, lpnumber );
        return newnode;
      } // else

    
    } // BuildTree()

    void PrintVector() {
      int i = 0;
      while ( Accurate_token_vector.size() > i ) {
        cout << Accurate_token_vector[i].token_name;
        i++;
      } // while 

      i = 0;
      cout << "\n";
      while ( Translated_token_vector.size() > i ) {
        cout << Translated_token_vector[i].token_name;
        i++;
      } // while

      cout << "\n";
    } // PrintVector()

    vector<TokenData> ReturnToken_vector() {
      return Accurate_token_vector;
    } // ReturnToken_vector()
    

}; // Parser

class Tree {
  private:
  public:
    void PrintSExp( TreeNode * TreeRoot ) {                             // 印成list-like ㄑormate
      if ( TreeRoot -> right == NULL && TreeRoot -> left == NULL ) {    // 
      
          
      } // if

      PrintSExp( TreeRoot -> left );
      PrintSExp( TreeRoot -> right );
    } // PrintSExp()

    void Preorder( TreeNode * TreeRoot ) {
      if ( TreeRoot ) {
        cout << TreeRoot -> Token_Data.token_name << "\n";
        Preorder( TreeRoot -> left );
        Preorder( TreeRoot -> right );
      } // if

    } // Preorder()

}; // Tree

void ReadSExp( TreeNode * &TreeRoot ) {                         // Scanner與Parser互相溝通的function，等Paser處理完並無錯之後就開始建Tree。
  Scanner aScanner;                                             // 創一個aScanner方便我們「切」此SExp。
  Parser aParser;                                               // 創一個aParser檢查我們aScanner切出來的token，文法有沒有正確，正確就bulidtree，錯誤就回傳錯誤。
  TokenData First_Token_Data;
  string geterrorline = "";
  char linend = '\0';
  if ( aParser.IsSexp( First_Token_Data, aScanner ) ) {                              // 判斷Sexp的合法性，並且如果該完成的Sexp後面接的是換行字元讀掉他
    if ( cin.peek() == '\n' ) cin.get( linend );
  } // if
  else {
    NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;                                         // 如果第一個SExp就出錯。
    aNoAtomQuoteErrorInfo.token_name = First_Token_Data.token_name;
    aNoAtomQuoteErrorInfo.column = First_Token_Data.token_column;
    aNoAtomQuoteErrorInfo.line = First_Token_Data.token_line;
    aScanner.ReadErrorLine();
    throw aNoAtomQuoteErrorInfo;
  } // else

  // vector<TokenData> ReturnToken_vector = aParser.ReturnToken_vector();              // Sexp合法的開始。
  int i = 0, lpnumber = 0;
  aParser.TranslateToken();
  aParser.PrintVector();
  TreeRoot = aParser.BuildTree( TreeRoot, i, lpnumber );
} // ReadSExp()


int main() {
  char linenter = '\0';
  scanf( "%d%c", &testNum, &linenter );                         // 讀取一個testnum和一個line-enter。                        
  cout << "Welcome to OurScheme!\n\n";
  cout << "> ";
  if ( cin.peek() == EOF ) {                                    // 先確定一開始測試檔中有資料，無就直接印錯誤，有就繼續往下走。
    cout << "ERROR (no more input) : END-OF-FILE encountered";
  } // if

  while ( cin.peek() != EOF ) {                                  
    TreeNode * TreeRoot;                                        // 一次處理一個SExp，一個SExp代表一個樹的結構。
    TreeRoot = new TreeNode;                                     
    try {
      ReadSExp( TreeRoot );                                     // Scanner與Paser互相溝通的function，等Paser處理完並無錯之後就開始建Tree。
      if ( TreeRoot -> Token_Data.token_name == "(" && TreeRoot -> left -> Token_Data.token_name == "exit" && 
           TreeRoot -> right -> Token_Data.token_name == "nil"  ) {                                               // user讀到 (exit)，結束這次測試檔。
           
        break;
      } // if               
      Tree aTree;
      aTree.Preorder( TreeRoot );
      // aTree.PrintSExp( TreeRoot );
      cout << "\n";
      cout << "> ";
    } // try
    catch( int endoffile ) {                              // error1(int): no more input。
      cout << "ERROR (no more input) : END-OF-FILE encountered\n";
    } // catch
    catch( StringErrorInfo aStringErrorInfo ) {                   // error2: scanner處理string的時候，有可能no closing quote
      cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line "<< aStringErrorInfo.line << " Column " << aStringErrorInfo.column << "\n";
    } // catch
    catch( NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo ) {
      cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " << aNoAtomQuoteErrorInfo.line;
      cout << " Column " << aNoAtomQuoteErrorInfo.column << " is >>" << aNoAtomQuoteErrorInfo.token_name << "<<\n";
    } // catch
    catch( NoRightParen aNoRightParen ) {
      cout << "ERROR (unexpected token) : ')' expected when token at Line " << aNoRightParen.line;
      cout << " Column " << aNoRightParen.column << " is >>" << aNoRightParen.token_name << "<<\n";      
    } // atch

    
  } // while

  cout << "\n";
  cout << "Thanks for using OurScheme!\n";

} // main()
