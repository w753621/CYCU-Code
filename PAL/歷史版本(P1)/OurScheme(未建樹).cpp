# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <vector>
# include <iostream>
using namespace std;
static int testNum = -1;

struct TokenData {                                            // token�r��Btoken�����O�C
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


class Scanner {                                                 // �u�t�d���XGetToken()�A��PeekToken()�A�æ^�Ǹ�Token�r��C
  private:
    int scan_line = 1, scan_column = 0;
    void aSeparatorType( char sexpchar, TokenData & token_data ) {
      if ( sexpchar == '(' ) {
        if ( cin.peek() == ')' ) {                    //  case1:() ���ݤU�@�Ӧr���A�p�G�O')'�NŪ�J�A�ç⦹token���O�令"NIL"�C
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
      else if ( sexpchar == ';' ) {                            // �o�O���ѡA�Q��ReadCommentLine()Ū���A�ðO����escan��C�C
        token_data.token_name = ";";
        token_data.token_type = "COMMENT";                                      
        ReadCommentLine();
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

    } // aSeparatorType()

    bool IsString( string & aString ) {                         // IsString()�|�P�_string����k�A"���᪺��line-eneter���e�n��"�A�S���ܴNthrow StringErrorInfo�A
      bool result = false;                                      // �ç�᭱��line-enter�e����Ū���C
      StringErrorInfo aStringErrorInfo;
      char sexpchar = '\0';
      if ( cin.peek() == '\n' ) {                               // �U�@�ӧY�O����r��gg�A���Oquote�A������᭱����Ƥ]��Ū��(���ݭn)�C
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
        if ( sexpchar == '\"' ) {                   // �J��"������������
          aString = aString + sexpchar;
          result = true;
          break;
        } // if
        else if ( sexpchar == '\\' ) {            // �J������r���A�Ҽ{��@�Ӧr����O���On�Bt�B"�B\�o�|�ر��p�A�O���ܰ��A��B�z�A���O���ܴN�O���q�������r���C
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
          else {                                // ���������L�N�u�O�@�Ӵ��q���r���Ӥw
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
      if ( token[0] == '+' || token[0] == '-' ) {           // '+'�B'-'�B'�Ʀr'�T�ر��p�C
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
      if ( token[0] == '+' || token[0] == '-' || token[0] == '.' ) {           // case1:.�}�Y case2:+�B-�}�Y case3:+�B-�}�Y�ĤG�r�����I 
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
 
      token_data.token_column = scan_column;
      token_data.token_line = scan_line;
      // �o�ɭԮ��쪺sexpchar�O�u�Dwhite space�v�A�i��O�useparator�v�Ρu�Dseparator�v�A�p�G�w�g�O��separator�����B�z�C
      if ( sexpchar == '(' || sexpchar == ')' || sexpchar == '\'' || sexpchar == '\"' || sexpchar == ';' ) {
        aSeparatorType( sexpchar, token_data );
        return token_data;  
      } // if
      else {        
        // �Dseparator�A�B�z��white space(�@��token) or separator(���token)   
        // ��token�i��O1.INT�B2.STRING(")�B3.DOT(.)�B4.FLOAT�B5.NIL(nil�B#f�B())�B6.T(t�B#t)�BQUOTE�BSYMBOL
        string token = "";
        token = token + sexpchar;
        while ( sexpchar != '\n' || sexpchar != ' ' || sexpchar != '\t' ) {
          // �w���U�Ӧr���p�G�Oseparator�A����Ū�J�A�óB�z��separator���e�C
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
 
          if ( nextoken_char == '\n' || nextoken_char == ' ' || nextoken_char == '\t' ) {                                     // interrupt �@��token�B�z
            if ( IsDOT( token, token_data ) );                                                                  
            else if ( IsINT( token, token_data ) );
            else if ( IsFLOAT( token, token_data ) );
            else if ( IsT( token, token_data ) );
            else if ( IsNIL( token, token_data ) );
            else {                                                                                            // �ѤU���N�|�Q�k���bSYMBOL
              token_data.token_name = token;
              token_data.token_type = "SYMBOL";
            } // else 

            break;
          } // if 
          else {                                                                                                   //  �~��token������
            cin.get( sexpchar ); 
            token = token + sexpchar;
            TraceTokenLineColumn( sexpchar );
          } // else 

        } // while

      } // else

      return token_data;
    } // GetOneToken()

    TokenData PeekToken() {              // Parser�w�����D�U�@��token��function�A�n�P�_��k���V�C
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
    bool lookahead = false;                                     // �x��GetOneToken�ɪ��ǫh
  public:
    TokenData aPeekToken;

    TokenData GetNextTokenForParser( Scanner & aScanner ) {      // �^�ǳo��Getoken
      TokenData Get_Token_Data;                         
      if ( lookahead ) {                                      // GetToken�ɡA�T�wlookahead�n�Ofalse�����p�U�~�i�H���Ugetoken�A�_�h�ݥX�wŪ���n�B�z��token�i�hparser
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
          // ���U�~��                                                          
        } // if
        else {
          NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;                                                   // return NoAtomQuoteErrorInfo
          aNoAtomQuoteErrorInfo.token_name = token_IsSexp.token_name;
          aNoAtomQuoteErrorInfo.column = token_IsSexp.token_column;
          aNoAtomQuoteErrorInfo.line = token_IsSexp.token_line;
          aScanner.ReadErrorLine();
          throw aNoAtomQuoteErrorInfo;         
        } // else

        token_IsSexp = aScanner.PeekToken();    // ���ݤU�@��Token { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
        aPeekToken = token_IsSexp;              // �]�waPeekToken
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
            // ���U�~��
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
        
        if ( lookahead == false ) {               // �w��"DOT"���B�z�A�A�~��Peek�U�@�ӬO���O')'�C
          token_IsSexp = aScanner.PeekToken();    // ���ݤU�@��Token RIGHT-PAREN
          aPeekToken = token_IsSexp;              // �]�waPeekToken
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
      // �J��quote�O�o�h�[�@��(..)
      if ( Accurate_token_vector.size() == 1 ) {
        Translated_token_vector.push_back( Accurate_token_vector[0] );   // ATOM
      } // if
      else  {
        while ( Accurate_token_vector.size() > anumber ) {
          if ( Accurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {        // ���O���Ӥl�Τ���sexp�����X�Ӫťե�
            while ( Accurate_token_vector[anumber].token_type == "LEFT-PAREN" ) {
              Translated_token_vector.push_back( Accurate_token_vector[anumber] );
              leftparen++;
              anumber++;
            } // while
            
            if ( Accurate_token_vector[anumber].token_type != "QUOTE" ) {
              Translated_token_vector.push_back( Accurate_token_vector[anumber] );    // �s(����U�@��sexp�i���O(�F�j
              anumber++;
            } // if
      
          } // if
          else if ( Accurate_token_vector[anumber].token_type == "QUOTE" ) {
            Translated_token_vector.push_back( token_LP_data );                       // ���ؤ@��(
            Translated_token_vector.push_back( Accurate_token_vector[anumber] );      // �ç�Quote��J
            extraparen++;                                                             // �M���extraparen
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
          else {                                                                   // �h�W�[�@��.(  
            bool connectnil = false;
            if ( Accurate_token_vector[anumber-1].token_type != "DOT" ) {
              connectnil = true;
              Translated_token_vector.push_back( token_Dot_data );
              Translated_token_vector.push_back( token_LP_data );
              extraparen++;                                                          // �O�o)�[1�A�J��)�ɪ�^�o��)�A(..)�~�ন��C
            } // if

            Translated_token_vector.push_back( Accurate_token_vector[anumber] );
            anumber++;      
            if ( Accurate_token_vector[anumber].token_type == "RIGHT-PAREN" && connectnil ) {    // �s�@.nil�A�b�̫�᭱��nil�C
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

          if ( Translated_token_vector.size() > i + 1 ) {       // �p�G�᭱�٦��F��A�~��P�_�B�~��!!!
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
          newnode -> left = BuildTree( newnode, i, lpnumber );                    // �����@�Ӥlsexp(expr)����
        if ( newnode -> Token_Data.token_name != "(" ) {              
          return newnode;
        } // if
        else if ( Translated_token_vector[i].token_name == "." ) {    // ����DOT
          i++;
        } // if
        else if ( newnode -> Token_Data.token_name == "(" ) {                 // �ѨM�@��sexp���D�A�J��)���e�A�����p��w�g�J��X��(�F
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
    void PrintSExp( TreeNode * TreeRoot ) {                             // �L��list-like ��ormate
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

void ReadSExp( TreeNode * &TreeRoot ) {                         // Scanner�PParser���۷��q��function�A��Paser�B�z���õL������N�}�l��Tree�C
  Scanner aScanner;                                             // �Ф@��aScanner��K�ڭ̡u���v��SExp�C
  Parser aParser;                                               // �Ф@��aParser�ˬd�ڭ�aScanner���X�Ӫ�token�A��k���S�����T�A���T�Nbulidtree�A���~�N�^�ǿ��~�C
  TokenData First_Token_Data;
  string geterrorline = "";
  char linend = '\0';
  if ( aParser.IsSexp( First_Token_Data, aScanner ) ) {                              // �P�_Sexp���X�k�ʡA�åB�p�G�ӧ�����Sexp�᭱�����O����r��Ū���L
    if ( cin.peek() == '\n' ) cin.get( linend );
  } // if
  else {
    NoAtomQuoteErrorInfo aNoAtomQuoteErrorInfo;                                         // �p�G�Ĥ@��SExp�N�X���C
    aNoAtomQuoteErrorInfo.token_name = First_Token_Data.token_name;
    aNoAtomQuoteErrorInfo.column = First_Token_Data.token_column;
    aNoAtomQuoteErrorInfo.line = First_Token_Data.token_line;
    aScanner.ReadErrorLine();
    throw aNoAtomQuoteErrorInfo;
  } // else

  // vector<TokenData> ReturnToken_vector = aParser.ReturnToken_vector();              // Sexp�X�k���}�l�C
  int i = 0, lpnumber = 0;
  aParser.TranslateToken();
  aParser.PrintVector();
  TreeRoot = aParser.BuildTree( TreeRoot, i, lpnumber );
} // ReadSExp()


int main() {
  char linenter = '\0';
  scanf( "%d%c", &testNum, &linenter );                         // Ū���@��testnum�M�@��line-enter�C                        
  cout << "Welcome to OurScheme!\n\n";
  cout << "> ";
  if ( cin.peek() == EOF ) {                                    // ���T�w�@�}�l�����ɤ�����ơA�L�N�����L���~�A���N�~�򩹤U���C
    cout << "ERROR (no more input) : END-OF-FILE encountered";
  } // if

  while ( cin.peek() != EOF ) {                                  
    TreeNode * TreeRoot;                                        // �@���B�z�@��SExp�A�@��SExp�N��@�Ӿ𪺵��c�C
    TreeRoot = new TreeNode;                                     
    try {
      ReadSExp( TreeRoot );                                     // Scanner�PPaser���۷��q��function�A��Paser�B�z���õL������N�}�l��Tree�C
      if ( TreeRoot -> Token_Data.token_name == "(" && TreeRoot -> left -> Token_Data.token_name == "exit" && 
           TreeRoot -> right -> Token_Data.token_name == "nil"  ) {                                               // userŪ�� (exit)�A�����o�������ɡC
           
        break;
      } // if               
      Tree aTree;
      aTree.Preorder( TreeRoot );
      // aTree.PrintSExp( TreeRoot );
      cout << "\n";
      cout << "> ";
    } // try
    catch( int endoffile ) {                              // error1(int): no more input�C
      cout << "ERROR (no more input) : END-OF-FILE encountered\n";
    } // catch
    catch( StringErrorInfo aStringErrorInfo ) {                   // error2: scanner�B�zstring���ɭԡA���i��no closing quote
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
