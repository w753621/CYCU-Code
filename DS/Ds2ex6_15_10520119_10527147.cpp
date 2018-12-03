/* ��T�G�� 10520119���ժl  10527147 ��Ӵ� 

*/
# include<stdio.h>
# include<stdlib.h>
# include<math.h>  // �I�s pow, sqrt �����Y�� 
# include<time.h>  // �I�s�ɶ������禡
# include<fstream> 
# include<istream>
# include<string>
# include<string.h>
# include <sstream>
# include <queue>
# include<list> 
# include<vector> 
# include <iomanip> // ����p���I��� 
# include<iostream>
using namespace std; 

struct Pair {
	char sid1[10];	
	char sid2[10];
	float weight;
};

struct Node {
	char sid[10];
	float weight;
	Node*next;	
};

struct Edge {
	int from, to;
	float weight;
};

struct CCNode {
	char sid[10];
	CCNode*next;
	int size;
	vector<Edge> edge;
	vector<Edge> IncreaseWeight;
	vector<Edge> edgeMST;	
}; 


class Graph {
	private:
	  int vertex;
	  vector<Node*> adjacencylist; 
	  vector<CCNode*> CClist; 
	  int * color; // 0�N��զ�(�S�Q�o�{) 1�N��(�Q�o�{)������ 2�N��w�g���� 
	  int * predecessor;
	  
	public:
		
    	 AdjacencyList(  ) {
   		   vertex = 0;
		 } //  AdjacencyList()
		
		 void weightBigToSmall( int position1, int position2, float toweight ) {
		   int i = 0;
		   int insertpostion = 0;
		   Node * node_insert =  adjacencylist[position1];
		   Node * before;
		   Node * newnode;
		   newnode = new Node;
		   newnode -> next = NULL; 
		   
		   if ( node_insert -> next == NULL ) {   
		     strcpy( newnode -> sid, adjacencylist[position2] -> sid );  
		   	 newnode -> weight = toweight;
		   	 node_insert -> next = newnode;
		   } // if
		   else {
		     while ( node_insert -> next != NULL )	{
		       before = node_insert;	
		       node_insert = node_insert -> next;	
		       if ( toweight >= node_insert -> weight ) {
		         if ( insertpostion == 0 ) {
		           // ���Y
				   strcpy( newnode -> sid, adjacencylist[position2] -> sid );
			       newnode -> weight = toweight; 
			       before = adjacencylist[position1] -> next; 
			       adjacencylist[position1] -> next = newnode;
				   newnode -> next = before;  
				   break;	
				 } // if
				 else {
				   //������	
				   strcpy( newnode -> sid, adjacencylist[position2] -> sid );
			       newnode -> weight = toweight; 
				   before -> next = newnode;  
				   newnode -> next = node_insert;
				   break;	
				 } // else         
			   	
			   } // if
			   else {
			   	 // ���J���� 
			     if ( node_insert -> next == NULL ) {	
			       strcpy( newnode -> sid, adjacencylist[position2] -> sid );
			       newnode -> weight = toweight;
			   	   node_insert -> next = newnode; 
				   break;  	
				 } // if 
			   	
			   } // else
			  
			   insertpostion++;  	
		     } // while
		   
		   	
		   } // else
		   
		 	
		 } // weightBigToSmall()
		 
		 void sid1SmallToBig( Node * node ) {
		   
		   if ( adjacencylist.size() == 0 ) {
		     adjacencylist.push_back( node );	
		   } // if
		   else {
		     int i = 0;
		     bool back = false;
			 while( adjacencylist.size() > i ) {
			   if ( strcmp( adjacencylist[i] -> sid, node -> sid ) > 0 ) {
			     adjacencylist.insert( adjacencylist.begin() + i, node ); 
			     back = true;
			     break;
			   } // if
			   
			   i++; 	
			 } // while
			 
			 if ( !back ) {
			   adjacencylist.push_back( node );
			 } // if
			 
		   } // else
		 	
		 } // sid1SmallToBig()
		 
		 void OnePairToAL( Pair pair ) {
		 // Step1: �C���۾F��C�ˬd���Lsid1�A�A�ˬdsid2�A�Y�L�s�ؤ@����C�A�s�خ� 
		 	int i  = 0;
		 	bool findid_1 = false;
		 	bool findid_2 = false;
		    int id_1position = -1, id_2position = -1;
			while ( adjacencylist.size() > i ) {
			  if ( strcmp( pair.sid1, adjacencylist[i] -> sid ) == 0  )  {
			    findid_1 = true;
			  } // if
			  
			  if ( strcmp( pair.sid2, adjacencylist[i] -> sid ) == 0  )  {
			    findid_2 = true;	
			  } // if
		     
	          i++;	
			}  // while	
				
			
			if ( !findid_1 ) {
			  Node * node;
			  node = new Node;
			  node -> next = NULL; 
			  node -> weight = -1;
			  strcpy( node -> sid, pair.sid1 );
			  sid1SmallToBig( node );
			} // if
			
			if ( !findid_2 ) {
		      Node * node;
			  node = new Node;
			  node -> next = NULL; 
			  node -> weight = -1;
			  strcpy( node -> sid, pair.sid2 );
			  sid1SmallToBig( node );
			} // if 
			  

		   // Step2: �⦹weight�v���]�w(�s��)�b�����node�W(��C����u�Ѥj��p�Ƨ�)
		   i = 0;
		   while ( adjacencylist.size() > i ) {
		      if ( strcmp( pair.sid1, adjacencylist[i] -> sid ) == 0  )  {
		        id_1position = i;
			  } // if
			  
			  if ( strcmp( pair.sid2, adjacencylist[i] -> sid ) == 0  )  {
			    id_2position = i;	
			  } // if
		     
	          i++;	
			}  // while
			
			weightBigToSmall( id_1position, id_2position, pair.weight );		 		  
		    weightBigToSmall( id_2position, id_1position, pair.weight );
		 	
		 } // OnePairToAl
		 
		 void Insert( vector<Pair> pair_vector ) {
		   int i = 0;
		   while ( pair_vector.size() > i ) {
		   	 OnePairToAL( pair_vector[i] );
		     i++;	
		   } // while
		   
		   i = 0;
		 
		 } // Insert()
		 
		 
		 void SetVertex() {
		   vertex = adjacencylist.size();   
		 } // SetVertex()
		 
		 int FindPosiion( char sid[10] ) {
		   int i = 0;
		   while ( i < adjacencylist.size() ) {
		   	 if ( strcmp( sid, adjacencylist[i] -> sid  ) == 0 )  return i;
		     i++;	
		   } // while	
		 	
		   return -1;
		 } // FindPosiion()
		
  	     void DFS() {
  	     	int i = 0;
  	     	color = new int[vertex];
			predecessor = new int[vertex]; 
			while ( i < vertex ) {
			  color[i] = 0;
			  predecessor[i] = -1;	
			  i++;	
			} // while
			
			i = 0;
			int j = 0;
			int time = 0;
			// �u�n���I(vertex)�O�զ�N�i��Visit 
			int CC_size = 0, CC_num = 0;	
			while ( i < vertex ) {
			  CC_size = 0;	
			  if ( color[i] == 0 )  {
			  	CCNode * newnode = NULL;
			    newnode = new CCNode;
			    newnode -> next = NULL;
			    CClist.push_back( newnode );
			    CCNode * node = newnode;
			  	CC_size++;
			  	CCInsert( node, i, CC_num );
			  	cout << adjacencylist[i] -> sid << "\n"; 
			    VisitDFS( i, node, CC_size, CC_num );
			    CClist[CC_num] -> size = CC_size;
				CC_num++;	
			  } // if 
			  
			  i++;	
			} // while
			

		 } // DFS() 
		 
		 void VisitDFS( int aVertex, CCNode * node, int & CC_size, int CC_num  ) {
		   Node * curnode = adjacencylist[aVertex];
		   curnode = curnode -> next;
		   color[aVertex] = 1; // ���令�Ǧ�  
		   int i = 0;
		   
 		   // �}�l���o�����|  �H���j���覡�e�{�ustack���|�v 
 		   // predecessor�s���O�s���o���I���W�@�Ӧ�m�C 
		   while ( curnode != NULL ) {
		   	 int NewVertex = FindPosiion( curnode -> sid );
		     if ( color[NewVertex] == 0 ) {
		       CCInsert( node, NewVertex, CC_num );
		       CC_size++; 
		       predecessor[NewVertex] = aVertex;
		       VisitDFS( NewVertex, node, CC_size, CC_num );
			 } // if
			 
			 curnode = curnode -> next;
		   } // while
		    
		   
		   // �ӱ����|����	
		   color[aVertex] = 2;
			 	
		 	
		 } // VisitDFS()
		 
		 void CCInsert( CCNode * node_insert, int position, int CCposition ) {
		   int i = 0;
		   int insertpostion = 0;
		   CCNode * before;
		   CCNode * newnode;
		   newnode = new CCNode;
		   newnode -> next = NULL; 	
		   if ( node_insert -> next == NULL ) {   
		     strcpy( newnode -> sid, adjacencylist[position] -> sid );  
		   	 node_insert -> next = newnode;
	
		   } // if	
	       else {
	       	 
		  	 while ( node_insert -> next != NULL )	{
		       before = node_insert;	
		       node_insert = node_insert -> next;	
		       if ( strcmp( node_insert -> sid, adjacencylist[position] -> sid ) >= 0 ) {
		         if ( insertpostion == 0 ) {
		           // ���Y
				   strcpy( newnode -> sid, adjacencylist[position] -> sid );
			       before = CClist[CCposition] -> next;
			       CClist[CCposition] -> next = newnode;
				   newnode -> next = before;
				   break;	
				 } // if
				 else {
				   //������	
				   strcpy( newnode -> sid, adjacencylist[position] -> sid );
				   before -> next = newnode;  
				   newnode -> next = node_insert;		   	 
				   break;	
				 } // else         
			   	
			   } // if
			   else {
			   	 // ���J���� 
			   	 
			     if ( node_insert -> next == NULL ) {	
			       strcpy( newnode -> sid, adjacencylist[position] -> sid );
			   	   node_insert -> next = newnode; 
				   break;  	
				 } // if 
			   	
			   } // else
			  
			   insertpostion++;  	
		     } // while  		       	
	       	
		   } // else
	      
		   	
		 } // CCInsert()
	
		 
		 void OutputCC( ) {
		   cout <<  "~~~ Connected Components ~~~\n";	
		   int i =  0;
		   while ( i < CClist.size() ) {
		   	 cout << "{ " << i + 1 << " }" << " Connected Component: size = " << CClist[i] -> size << "\n\t";
		   	 int j = 0;
		   	 CCNode * node = CClist[i] -> next; 
		   	 while ( node != NULL ) {
		   	   j++;
			   cout << "( " << j << " )" << "\t" << node -> sid << "\t"; 
			   if ( j % 5 == 0 ) cout << "\n\t";
			   node = node -> next;
			 } // while
			 
			 cout << "\n\n";
		     i++; 	
		   } // while	
		 	
		 } // OutputCC()
		 
		 void CCWeightSmallToBig( int CClist_position, Edge edge ) {
		 	CCNode * node = CClist[CClist_position];
		    if ( node -> IncreaseWeight.size() == 0 ) {	    
		     node -> IncreaseWeight.push_back ( edge );
		    } // if
			else {
			  int i = 0;
			  while ( i < node -> IncreaseWeight.size() ) {
			  	
			  	if ( edge.weight < node -> IncreaseWeight[i].weight ) {
			  	  node -> IncreaseWeight.insert( node -> IncreaseWeight.begin() + i, edge ); 
				  break;	
				} // if 
				else if ( i == node -> IncreaseWeight.size() - 1 ){
				  node -> IncreaseWeight.push_back ( edge );
				  break;	
				} // else if
			  	
			    i++;	
			  } // while
				
			} // else
		    
		     	
		 } // WeightSmallToBig()
		 
		 void AddEdgeForOneCC( CCNode * node, CCNode * node_start, int CClist_position, int CCNode_position ) {
		   int position = FindPosiion( node -> sid );  // ��즹�I�b�۾F��C����m�C
		   Node * curnode = adjacencylist[position] -> next;   	
		   while ( curnode != NULL ) { 
		     int to_position = 0;
		   	 bool findCC = false; 
		   	 CCNode * ccnode = node_start;
		   	 while ( ccnode != NULL ) {   							  // ���T�w�۾F��C�����I�bCCNode���L�s�b 
		   	   if ( strcmp( ccnode -> sid, curnode -> sid  ) == 0 ) {
		   	     findCC = true;	
			   	 break;	  	
			   }  // if
			   
			   to_position++;
			   ccnode =  ccnode -> next;  	
			 } // while
			 
			 if ( findCC ) {   // �s�WEdge�A���Ӥp��j�ƦC(Kruskal's�t��k)�C 
			   Edge edge;
			   edge.from = CCNode_position;
			   edge.to = to_position;   
			   edge.weight = curnode -> weight;
			   CCWeightSmallToBig( CClist_position, edge );	 	
			 } // if
		   	 
		     curnode = curnode -> next;	
		   } // while
		 	
		 } // AddOneEdge()
		 
		 void FindCCEdge() {
		   //  �إ�Edge�b�C��CC�̡C 
		   int i = 0;  
           CCNode * node_start = CClist[i] -> next;
           CCNode * node = CClist[i] -> next;
		   while ( i < CClist.size() ) {
		     int CC_position = 0;
		     while ( node != NULL )  {  // �}�l�B�z�o��CC 
		       AddEdgeForOneCC( node, node_start, i, CC_position );		 	
		       node = node -> next;   	
		       CC_position++;
			 } // while
		   
		     i++;
		     if ( i == CClist.size() ) break;
			 node = CClist[i] -> next;    
			 node_start = CClist[i] -> next;
		   }  // while
		   	
		 } // FindCCEdge()
		 
		 void FindCCposition( int & vertex, int & position, int & filepo, char aSid[10]  ) {
		   int i = 0;
		   vertex = 0, position = 0, filepo = 0;
		   bool find = false;
           while ( i < CClist.size() ) {
		    CCNode * node = CClist[i] -> next;
		    while ( node != NULL ) {
		      vertex++;		
		      if ( strcmp( node -> sid, aSid ) == 0 ) {
		      	filepo = vertex - 1;
			    find = true;
			  } // if 
		      node = node -> next;	
			} // while
		   
		    if ( find ) break;
		    vertex = 0;
		    position++;
		    i++; 
		    
		   } // while
		   
		  	
		 	
		 } // FindCCposition()
		 
		 int FindFather( vector<int> father, int x ) {
		   int a = x;
		   while ( x != father[x] ) x = father[x];
		   while ( a != father[a] ) {
		     int z = a;
			 a = father[a];
			 father[z] = x;		 
		   } // while 
		   
		   return x;
		 } // FindFather()
		 
		 
		 void KruskalMST( ) {
		 	// STEP2.�ϥ�Kruskal's�t��k�إ�MST�b�C��CC�̡C	
		 	int i = 0;
		 	while ( i < CClist.size() ) {	
		 	  int j = 0, NumEdge = 0, n = 0;
		 	  float ans = 0;
		 	  CCNode * node = CClist[i];
		 	  CCNode * node_num = CClist[i] -> next;
		 	  while ( node_num != NULL ) {
		 	    n++;
			    node_num = node_num -> next;
			  } // while
	
		 	  vector<int> father;
		 	  for ( j = 0; j < n; j++ )
		 	    father.push_back( j );
		 	  j = 0;  
		 	  int k = 0;
		 	  bool can_add = true;
		 	  while ( node -> IncreaseWeight.size() > j ) {   // j���n�[�J��Increaseedge,k���M�XedgeMST���S�����ƩΧΦ�cycle�A��while���P�_�ण��i�JedgeMST�C 
		 	    while ( node -> edgeMST.size() > k ) {
		 	      if ( node -> IncreaseWeight[j].from == node -> edgeMST[k].to && node -> IncreaseWeight[j].to == node -> edgeMST[k].from ) {
		 	        can_add = false;
		 	        break;
			      } // if
				  k++;	
				} // while
			   if ( can_add ) {
			   	  node -> edge.push_back( node -> IncreaseWeight[j] );
				  int faFrom = FindFather( father, node -> IncreaseWeight[j].from );
		 	  	  int faTo = FindFather( father, node -> IncreaseWeight[j].to );
		 	  	  if ( faFrom != faTo ) {    // Union it
		 	  	    father[faTo] = faFrom;
				    ans = ans + node -> IncreaseWeight[j].weight;
				    node -> edgeMST.push_back ( node -> IncreaseWeight[j] ); 
				    NumEdge++; 	
		 	  	    if ( NumEdge == n - 1 ) break; 
				  } // if  
				 
				}  // if 
				can_add = true;
				j++;
				k = 0;
		 	  } // while
             
			  cout << "The MST cost of connected component { " << i + 1 << " } = " << ans << "\n";
			  i++;	
			} // while
			
			
		 } // KruskalMST()
		 
		 float findweight ( int from, int to, vector<Edge> edge ) {
		   float weight = 0;
		   int i = 0;
		   while ( i < edge.size() ) {
		   	 if ( edge[i].from = from && edge[i].to == to ) {
		   	   weight = edge[i].weight;
			   return weight;		
			 } // if
			 
		     i++;	
		   } // while
		   	
		 	
		 } // findweight()
		 
		 void DijkstraSD( int CC_position, int vertex, int filepo, fstream & outputfile ) {
		   float d[vertex];  // �����U�I�oshorest path 
		   int parent[vertex]; // �^�� 
		   bool visited[vertex];  // �w�Q�X�ݤF�� 
		   int i = 0;
		   vector<Edge> edge = CClist[CC_position] -> edge;
		   CCNode * node_2 = CClist[CC_position] -> next; 
		   CCNode * node = CClist[CC_position] -> next; 
		   while ( i != filepo ) {
		     node = node -> next;
			 i++;	
		   } // while 
		   
		   // initialize
		   for ( i = 0; i < vertex ; i++ ) visited[i] = false;	
		   for ( i = 0; i < vertex ; i++ ) d[i] = 1e9;	
		   d[filepo] = 0;
		   parent[filepo] = filepo;
		   int k = 0;
		   while ( k < vertex ) {
		     int a = -1, b = -1, min = 1e9;
		   	 i = 0;
		   	 while( i < vertex ) {                          //�T�{a���Ȭ�source 
			   if ( !visited[i] && d[i] < min )  {
			  	 a = i;
			  	 min = d[i];
		
			   }  // if
		   	  
		   	   i++;
		     } // while
		    
		     if ( a == -1 ) break;
		     visited[a] = true;
		     i = 0;
		     while ( i < edge.size() ) {
		      if ( edge[i].from == a) {
		        int b = edge[i].to;
				float w = edge[i].weight;		
		      	if ( !visited[b] && d[a] + w < d[b] ) {	
		      	 d[b] = d[a] + w;	
		      	 parent[b] = a;	
				} // if
			  } // if
			  else if ( edge[i].to == a) {
		        int b = edge[i].from;
				float w = edge[i].weight;		
		      	if ( !visited[b] && d[a] + w < d[b] ) {	
		      	 d[b] = d[a] + w;	
		      	 parent[b] = a;	
				} // if
			  } // if
			  
			  
			  i++;	
			 } // while
			
		   	k++;
		   } // while
		   
		   i = 0;
		   outputfile << "origin: " << node -> sid << "\n"; 
	       while ( node_2 != NULL )  {
	       	 if ( i != filepo ) { 
			   if ( i % 5 == 0 ) cout << "\n\t";	  	 	
	       	   outputfile << "( " << i + 1 << " )" << node_2 -> sid << "\t" << d[i] << "\t"; 
			 } // if
	    
	         node_2 = node_2 -> next; 
			 i++;
		   } // while
			 

			 
		   	
		 } // DijkstraSD()
		 
		 void PrintVertex() {
           int i = 0;
		   while ( i < adjacencylist.size() ) {		   	
		     if ( i % 5 == 0 ) cout << "\n\t";
		     cout << adjacencylist[i] -> sid << "\t";
		     i++;	
		   } // while
		 	
		   cout << "\n";
		 } // PrintVertex()
		 
  	
	
};

void ReadBinary( fstream & inputfile, vector<Pair> & pair_vector  ) {
  int i = 0;
  while ( inputfile.peek() != EOF ) {
  	Pair pair;
    inputfile.read( (char *) & pair ,sizeof( pair ) );  	
    pair_vector.push_back( pair );
    i++;
  }	 // while   
  
  cout << "\n";	
  
} // WriteBinary()




void Ui() { 
  cout << "\n*******************************************\n"
          "  On-machine Exercise                     *\n"
 		  "  Mission 1: Minimum Spanning Tree        *\n"
		  "  Mission 2: Shortest Distances           *\n"
		  "*******************************************\n\n";
		  	  
  cout << "\nInput the file number: 601, 602, ... [0]Quit\n";	
  string inputstring, quit;
  cin >> inputstring; 
  if ( inputstring == "0" ) return;
  string input = "pairs";
  string outputfile2 = input + inputstring + ".dis";
  input = input + inputstring + ".bin";
  fstream inputfile;   // �ŧi�@��fstream����A�Ψӿ�J�C
  inputfile.open( input.c_str(), ios::in | ios::binary );  // Ū���G�i���� 
  while ( !inputfile ) {
    cout << "\n### " << input <<" does not exist! ###\n";
    cout << "\nInput the file number: 501, 502, ... [0]Quit\n";
	cin >> inputstring;
	if ( inputstring == "0" ) return;
	input = "pairs";
    input = input + inputstring + ".bin";
    inputfile.open( input.c_str(), ios::in | ios::binary );	
  } // while
  
  // �e�m���Ȥ@  Ū�ɫإ߬۾F��C
  vector<Pair> pair_vector;
  ReadBinary( inputfile, pair_vector );
  Graph aGraph; 
  aGraph.Insert( pair_vector );
  aGraph.SetVertex();
  // �e�m���ȤG  Traverse a graph to find connected components (CC)
  aGraph.DFS();
  aGraph.OutputCC();
  inputfile.close();
  // ���Ȥ@  ��X�C�ӳs�q�������̤p�ͦ���A�ϥ� Kruskal's�t��k�C 
  cout << "########################################################\n"
		  "Mission 1: Minimum Spanning Trees                      #\n"
		  "########################################################\n\n";	
  aGraph.FindCCEdge();
  aGraph.KruskalMST();
  
  // ���ȤG �D�X���w�ǥͩM��L�ǥͪ��̵u�Z���A�ϥ� Dijkstra's�t��k ��X���w�ǥͦܦP�@�s�q��������L�ǥͦb�ϤW���̵u�Z���C 
  cout << "\n########################################################\n"
		  "Mission 2: Shortest Distances                          #\n"
		  "########################################################\n\n";
  		  
  aGraph.PrintVertex();		  
  quit = "NO";
  char aSid[10];
  cout << "Input a student id from the above:\n\n";
  cin >> aSid; 
  cin.clear();
  cin.ignore();
  fstream outputSD;
  outputSD.open( outputfile2.c_str(), ios::out ); 
  while ( quit != "0" ) {
  	int vertex = 0, position = 0, filepo = 0; 
    aGraph.FindCCposition( vertex, position, filepo, aSid );	
  	aGraph.DijkstraSD( position, vertex, filepo,outputSD );
  	cout << "[0]Quit or [Any other]continue?\n";
    cin >> quit;
    if ( quit == "0" ) break;
	else {
	  outputSD << "\n";	
	  cout << "Input a student id from the above:\n\n";
	  cin >> aSid; 
  	  cin.clear();
  	  cin.ignore();	
	} // else
	
  } // while
  
  
} // Ui()

int main() {

  Ui();	
  
} // main()
