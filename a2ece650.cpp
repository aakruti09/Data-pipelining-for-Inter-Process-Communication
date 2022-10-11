// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include <regex>

using namespace std;

void print1d(vector<int> v){
  int l=v.size();
  for(int i=0;i<l;i++){
    cout<<v[i]<<"\t";
  }
  cout<<endl;
}

void print2d(vector<vector<int>> vec){
  int gi=vec.size();
  for (int i=0;i<gi;i++){
    int gii=vec[i].size();
    for (int j=0;j<gii;j++){
      cout<<vec[i][j]<<"\t";
    }
    cout<<endl;
  }
}

vector<int> bfs(vector<vector<int>> G,int &s,int &r,int &path_len){
  int sg=G.size();

  vector<int> color; //0 white, 1 grey, 2 black
  vector<double> dist; //double because of initial infinity
  vector<int> parent;
  double infi = std::numeric_limits<double>::infinity();

  for(int i=0;i<sg;i++){
    color.push_back(0);
    dist.push_back(infi);
    parent.push_back(-1);
  }
  if (s>=sg || r>=sg){
    cerr<<"Error: Source or destination vertex is out of index"<<endl;
    return parent;
  }
    
  color[s]=1;
  dist[s]=(int)0;
  
  vector<int> quevec;
  quevec.push_back(s);
  while (!quevec.empty()){
    int u=quevec[0];
    quevec.erase(quevec.begin());
    int del_u=G[u].size();
    for(int j=0;j<del_u;j++){
      int item=G[u][j];
      if(color[item]==0){
	color[item]=1;
	dist[item]=(int)dist[u]+1;
	parent[item]=u;
	quevec.push_back(item);
      }
    }
    color[u]=2;
  }
  if(parent[r]!=-1)
    path_len=(int)dist[r];
  return parent;
}


void pathstor(vector<int> p,int pl,int &s,int &r){
  int temp=(int)r;
  vector<int> path;
  for (int i=0;i<pl;i++){
    temp=p[temp];
    path.push_back(temp);
  }
  reverse(path.begin(),path.end());
  path.push_back(r);

  int l=path.size();
  for(int i=0;i<l-1;i++){
    cout<<path[i]<<"-";
  }
  cout<<path[l-1]<<endl;
}

vector<vector<int>> make_G(int v,vector<vector<int>> e){
  int el=e.size();
  vector<vector<int>> g;
  for(int i=0;i<v;i++){
    vector<int> temp;
    for(int j=0;j<el;j++){
      if(e[j][0]==i)
	temp.push_back(e[j][1]);
      else if(e[j][1]==i)
	temp.push_back(e[j][0]);
    }
    g.push_back(temp);
  }
  return g;
}



int main(int argc,char* argv[]){
  //cout<<"In a2:"<<endl;
  //string l1="V 5",l2="E {<1,2>,<2,3>,<0,4>}";
  string l1,l2;
  //cout<<argc<<endl;
  for(int i=1;i<argc;i++){
    //cout<<argv[i]<<endl;
    if (i==1){
      l1=argv[i];
    }
    if (i==2){
      l2=argv[i];
    }
  }
  
  int v;
  vector<vector<int>> e;
  cout<<l1<<endl;
  cout<<l2<<endl;
  istringstream i1(l1);  //Replace input with i1
  istringstream i2(l2);  //Replace input with i2
  char c1,c2;
  i1>>c1;
  i2>>c2;
  //cout<<"C1:"<<c1<<"\tC2:"<<c2<<endl;
  if (c1=='V'){
    string v1;
    istringstream vin(l1);
    ws(vin);
    char vt;
    vin>>vt;  //For V
    vin>>v1;  //For number
      
    regex vpat("([0-9]+)");
    if (regex_match (v1,vpat)){
      //cout<<"Matched"<<endl;
      if(v!=0)	e.clear();
      ws(i1);
      i1>>v;
      //cout<<"Vertex:"<<v<<endl;
      if (!i1.eof())  cerr<<"Error: Invalid V command"<<endl;
    }
    else{
      cout<<"Error: Enter valid integer number in V command"<<endl;
      v=0;
      e.clear();
    }
  }
  
  if (c2=='E'){
    string e1;
    istringstream ein(l2);
    ws(ein);
    char ct;
    ein>>ct;  // E
    ein>>e1;  // {<1,2>,<2,0>,<-1,6>}
    //cout<<"Line 1:"<<l1<<endl;
      
    regex pattern("^(\\{)((<)([0-9]+)(,)([0-9]+)(>))?((,)(<)([0-9]+)(,)([0-9]+)(>))*(})");
    if (regex_match (e1,pattern)){
      //cout<<"Matched"<<endl;
      
      e.clear(); 
      char charac;
      int n1,n2;
      ws(i2);
      while(!i2.eof()){
	vector<int> temp;
	i2>>charac;  //For { , comma , }
	if (charac=='}' || charac=='>')   break;
	else if(charac==',' || charac=='{'){
	  i2>>charac;  //For < char
	  if (charac=='}')   break;
	}
	else {
	  cerr<<"Error: Invalid character : "<<charac;
	  break;
	}
	i2>>n1;  //For first vertex of edge
	i2>>charac;  //For comma
	i2>>n2;  //For second vertex of edge
	i2>>charac;  //For > char
      
	if(n2>=v || n1>=v || n2<0 || n1<0){
	  cerr<<"Error: Edges to unknown vertex"<<endl;
	  e.clear();
	  break;
	}
	else if (n1==n2){
	  cerr<<"Error: Edge to own vertex is invalid in undirected graph"<<endl;
	}
	else{
	  temp.push_back(n1);
	  temp.push_back(n2);
	  e.push_back(temp);
	  //print1d(temp);
	}
      }
    }
    else {
      cerr<<"Error: Invalid E command"<<endl;
    }
  }
  
  string line;;
  int s,r;
  while(getline(cin,line))
  {
    istringstream input(line);
    ws(input);
    char cmd;
    input>>cmd;

    if (cmd=='s'){
      string s1,s2;
      char just;
      istringstream sin(line);
      ws(sin);
      sin>>just; //For s cmd
      sin>>s1;
      sin>>s2;
      //cout<<"S1 : "<<s1<<"\tS2 : "<<s2<<endl;
      
      regex spat("([0-9]+)");
      if ((regex_match (s1,spat)) && (regex_match (s2,spat))){
	//cout<<"Matched"<<endl;
	ws(input);
	input>>s;
	input>>r;
	if(s>=v || r>=v || s<0 || r<0){
	  cerr<<"Error: Vertex does not exist"<<endl;
	}
	else if (s==r) {
	  cerr<<s<<endl;
	}
	else{
	  vector<vector<int>> g=make_G(v,e);
	  int path_len=0;
	  vector<int> par;
	  par=bfs(g,s,r,path_len);
	  if (path_len==0)
	    cerr<<"Error: Path does not exist"<<endl;
	  else{
	    pathstor(par,path_len,s,r);
	  }
	}
      }
      else{
	cout<<"Error: Invalid s command"<<endl;
      }
	
    }
    else{
      cerr<<"Error: Invalid command"<<endl;
    }
  }
  return 0;
}




    /* if (cmd=='V'){
      string v1,v2;
      v2=line;
      istringstream vin(v2);
      ws(vin);
      char vt;
      vin>>vt;  //For V
      vin>>v1;  //For number
      
      regex vpat("([0-9]+)");
      if (regex_match (v1,vpat)){
	//cout<<"Matched"<<endl;
        if(v!=0){
	  e.clear();
	}
	ws(input);
	input>>v;
	if (!input.eof()){
	  cerr<<"Error: Invalid V command"<<endl;
	}
      }
      else{
	cout<<"Error: Enter valid integer number in V command"<<endl;
	v=0;
	e.clear();
      }
    }
    else if (cmd=='E'){
      string l1,l2;
      l2=line;
      istringstream ein(l2);
      ws(ein);
      char ct;
      ein>>ct;
      ein>>l1;
      //cout<<"Line 1:"<<l1<<endl;
      
      regex pattern("^(\\{)((<)([0-9]+)(,)([0-9]+)(>))?((,)(<)([0-9]+)(,)([0-9]+)(>))*(})");
      if (regex_match (l1,pattern)){
	//cout<<"Matched"<<endl;
      
	e.clear(); 
	char charac;
	int n1,n2;
	ws(input);
	while(!input.eof()){
	  vector<int> temp;
	  input>>charac;  //For { , comma , }
	  if (charac=='}' || charac=='>'){   //exit
	    break;
	  }
	  else if(charac==',' || charac=='{'){
	    input>>charac;  //For < char
	    if (charac=='}')
	      break;
	  }
	  else {
	    cerr<<"Error: Invalid character : "<<charac;
	    break;
	  }
	  input>>n1;  //For first vertex of edge
	  input>>charac;  //For comma
	  input>>n2;  //For second vertex of edge
	  input>>charac;  //For > char
      
	  if(n2>=v || n1>=v || n2<0 || n1<0){
	    cerr<<"Error: Edges to unknown vertex"<<endl;
	    e.clear();
	    break;
	  }
	  else if (n1==n2){
	    cerr<<"Error: Edge to own vertex is invalid in undirected graph"<<endl;
	  }
	  else{
	    temp.push_back(n1);
	    temp.push_back(n2);
	    e.push_back(temp);
	    //print1d(temp);
	  }
	}
      }
      else {
	cerr<<"Error: Invalid E command"<<endl;
      }
      }*/
