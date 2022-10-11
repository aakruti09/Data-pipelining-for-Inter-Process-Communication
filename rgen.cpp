#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <getopt.h>
#include <unistd.h>
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

void sprint1d(vector<string> v){
  for (auto i=v.begin();i!=v.end();i++){
    cout<<*i<<endl;
  }
}

void print3d(vector<vector<vector<int>>> v){
  for (auto i=v.begin();i!=v.end();i++){
    for(auto j=i->begin();j!=i->end();j++){
      for(auto k=j->begin();k!=j->end();k++){
	cout<<*k<<"\t";
      }
      cout<<endl;
    }
    cout<<endl;
  }
}



bool intersect(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4){
  double x,y,slopel1,slopel2,b11,b12,b21,b22;
  // cout<<"x1:"<<x1<<"\ty1:"<<y1<<endl;
  // cout<<"x2:"<<x2<<"\ty2:"<<y2<<endl;
  // cout<<"x3:"<<x3<<"\ty3:"<<y3<<endl;
  // cout<<"x4:"<<x4<<"\ty4:"<<y4<<endl;

  if( max(x1,x2)<min(x3,x4) ){
    x=max(min(x1,x2),min(x3,x4));
    y=min(min(x1,x2),max(x3,x4));
  }
  else {
    if((x1-x2)!=0)
      slopel1=(double)(y1-y2)/(x1-x2);
    else
      slopel1=0;
    if ((x3-x4)!=0)
      slopel2=(double)(y3-y4)/(x3-x4);
    else
      slopel2=0;
    // cout<<"slopel1: "<<slopel1
    // 	<<"\tslopel2: "<<slopel2<<endl;
    b11=y1-(slopel1*x1);
    b12=y2-(slopel1*x2);
    b21=y3-(slopel2*x3);
    b22=y4-(slopel2*x4);
    // cout<<"b11:"<<b11
    // 	<<"\tb12:"<<b12
    // 	<<"\tb21:"<<b21
    // 	<<"\tb22:"<<b22<<endl;

    if(!(abs(b11-b12)<1e-9)){
      // if(b11!=b12){
      b11=b12=0;
      x=x1=x2;
      y=(slopel2*x)+b21;
    }
    else if(!(abs(b21-b22)<1e-9)){
      //else if(b21!=b22){
      b21=b22=0;
      x=x3=x4;
      y=(slopel1*x)+b11;
    }
    else{
      if(slopel1-slopel2!=0)
	x=(b21-b11)/(slopel1-slopel2);
      else
	x=0;
      y=(slopel1*x)+b11;       
    }
    // cout<<"x:"<<x
    // 	<<"\ty:"<<y<<endl;
  }
  if (x2==x3 && y2==y3){
    if(x==x2 && y==y2)
      return false;
  }
  if ((x<max(min(x1,x2),min(x3,x4))) || (x>min(max(x1,x2),max(x3,x4))))
    return false;
  else if ((y>min(max(y1,y2),max(y3,y4))) || (y<max(min(y1,y2),min(y3,y4))))
    return false;
  else
    return true;
}

vector<int> calls(vector<vector<int>> t){
  //print2d(t);
  vector<vector<int>> lines;
  vector<int> temp;
  int tl=t.size();
  for(int i=0;i<tl-1;i++){
    temp.clear();
    temp.push_back(t[i][0]);
    temp.push_back(t[i][1]);
    temp.push_back(t[i+1][0]);
    temp.push_back(t[i+1][1]);
    lines.push_back(temp);
  }
  //cout<<"Lines"<<endl;
  //print2d(lines);
  vector<int> ans;
  if (lines.size()>1){
    int x1,x2,x3,x4,y1,y2,y3,y4;
    int linesize=lines.size();
    for(int i=0;i<linesize;i++){
      for (int j=i+1;j<linesize;j++){
	x1=lines[i][0],y1=lines[i][1];
	x2=lines[i][2],y2=lines[i][3];
	x3=lines[j][0],y3=lines[j][1];
	x4=lines[j][2],y4=lines[j][3];
	// cout<<"\nx1:"<<x1<<"\ty1:"<<y1;
	// cout<<"\tx2:"<<x2<<"\ty2:"<<y2;
	// cout<<"\tx3:"<<x3<<"\ty3:"<<y3;
	// cout<<"\tx4:"<<x4<<"\ty4:"<<y4<<endl;
	ans.push_back(intersect(x1,y1,x2,y2,x3,y3,x4,y4));
	//cout<<"Intersection:"<<intersect(x1,y1,x2,y2,x3,y3,x4,y4)<<endl;
      }
    }
    //print1d(ans);
    return ans;
  }
  else{
    ans.push_back(0);
    return ans;
  }
}

vector<vector<int>> gen_coor(int n,int r){
  char c1,c2;
  int nr,x,y;
  ifstream urandom("/dev/urandom");
  vector<int> temp;
  vector<vector<int>> vcor;
  set<vector<int>> coor;
  int count,i=0;
  do{
    count=0;
    do{
     
      //for(int i=0;i<=n;i++){
      int ss;
      urandom.read(&c1, 1);
      urandom.read(&c2, 1);
      x=(int)c1;
      x=x % (r+1);
      y=(int)c2;
      y=y % (r+1);
      //cout<<"I:"<<i<<"\tx:"<<x<<"\ty:"<<y<<"\tCount : "<<count<<endl;
      nr=-r;
      //cout<<"Range:[" <<nr<<" , "<<r<<"]"<<endl;
      if( (x>=nr && x<=r) && (y>=nr &&y<=r) ){
	ss=coor.size();
	temp.clear();
	temp.push_back(x);
	temp.push_back(y);
	coor.insert(temp);
	if (ss!=(int)coor.size()){
	  //copy(temp.begin(),temp.end(),back_inserter(vtemp));
	  vcor.push_back(temp);
	  ss=coor.size();
	  i++;
	  count=25;
	}
      }
      count++;
      if(count==25){
	count=26;
	cerr<<"Error: Cannot generate coordinates in 25 attempts"<<endl;
	exit(1);
      }
    }while(count<=24);
  }while(i<=n);
  //print2d(vcor);
  return vcor;
}


vector<string> gen_names(int s){
  int x;
  set<string> names; 
  do{
    string word="";
    for (int j=0;j<3;j++){
      char sc,w;
      ifstream urandom("/dev/urandom");
      for (int i=0;i<26;i++){
	urandom.read(&sc, 1);
	x=(int)sc;
	x=97+(x % 26);
	if((x>=97) && (x<=122)){
	  //cout<<x<<endl;
	  break;
	}
      }
      w=(char)x;
      word=word+w;
    }
    //cout<<"Word:"<<word<<endl;
    names.insert(word);
  }while ((int)names.size()!=s);
  vector<string> name(names.begin(),names.end());
  // for(int i=0;i<name.size();i++){
  //   cout<<name[i]<<endl;
  // }
  return name;
}


vector<string> gen_acmd(int s,vector<string> names,vector<vector<vector<int>>> c){
  vector<string> acmd;
  vector<string> scor;
  string corfor;
  for (auto j=c.begin();j!=c.end();j++){
    corfor="";
    for(auto k=j->begin();k!=j->end();k++){
      auto l=*k;
      string a=to_string(l[0]);
      string b=to_string(l[1]);
      corfor+="("+a+","+b+") ";
      //cout<<"a:"<<a<<"\tb:"<<b<<endl;	
    }
    scor.push_back(corfor);
  }
  //cout<<corfor<<endl;
  //print1d(scor);
  for(int i=0;i<s;i++){
    string tstr="a ";
    tstr+="\""+names[i]+"\" ";
    
    tstr+=scor[i];
    acmd.push_back(tstr);
  }
  //sprint1d(acmd);
  return acmd;
}

vector<string> gen_rcmd(int s,vector<string> name){
  vector<string> rcmd;
  for(int i=0;i<s;i++){
    string rstr="r \"";
    rstr+=name[i]+"\"";
    rcmd.push_back(rstr);
  }
  //sprint1d(rcmd);
  return rcmd;
}


int main(int argc,char *argv[]){
  // cout<<"In rgen printing arguments"<<endl;
  // for(int i=0;i<argc;i++){
  //   cout<<argv[i]<<endl;
  // }
  int first=true;
  vector<string> st_names{to_string(-1)};
  vector<string> old_st;
  int cnt=0;

  int option;
  int s,ks=10,kn=5,l,kl=5,r,kc=20;
  
  while ((option=getopt(argc,argv,"s:n:l:c:")) != -1){
    switch(option){
    case 's':{
      //cout<<"In s"<<endl;
      ks = atoi(optarg);
      if (ks<2){
<<<<<<< HEAD
	cerr<<"Error: Number of street can't be less than 2"<<endl;
	exit(1);
=======
	      cerr<<"Error: Number of street can't be less than 2"<<endl;
	      exit(1);
>>>>>>> 7f43985f3845779d37c0fc00af1d22f557367cc7
      }
      break;
    }
    case 'n':{
      kn = atoi(optarg);
      if (kn<1){
<<<<<<< HEAD
	cerr<<"Error: Number of lines must be greater than or equal to 1"<<endl;
	exit(1);
=======
	      cerr<<"Error: Number of lines must be greater than or equal to 1"<<endl;
	      exit(1);
>>>>>>> 7f43985f3845779d37c0fc00af1d22f557367cc7
      }
      break;
    }
    case 'l': {
      kl = atoi(optarg);
      if (kl<5){
<<<<<<< HEAD
	cerr<<"Error: -l argument must be greater than or equal to 5"<<endl;
	exit(1);
=======
	      cerr<<"Error: -l argument must be greater than or equal to 5"<<endl;
	      exit(1);
>>>>>>> 7f43985f3845779d37c0fc00af1d22f557367cc7
      }
      break;
    }
    case 'c':{
      kc = atoi(optarg);
      if (kc<1){
<<<<<<< HEAD
	cerr<<"Error: -c argument must be greater than or equal to 1"<<endl;
	exit(1);
=======
	      cerr<<"Error: -c argument must be greater than or equal to 1"<<endl;
	      exit(1);
>>>>>>> 7f43985f3845779d37c0fc00af1d22f557367cc7
      }
      break;
    }
      
    }//switch ended
  }//if ended

  //cout<<"s:"<<ks<<"\tn:"<<kn<<"\tl:"<<kl<<"\tc:"<<kc<<endl;
  
  do{
    cnt+=1;
    // for(int i=0;i<argc;i++){
    //   cout<<argv[i]<<endl;
    // }
    
    
    //Random number generator start
    ifstream urandom("/dev/urandom");
    if (urandom.fail()){
      cerr<<"Error: /dev/urandom file is not opening"<<endl;
      return 9;
    }
    //int x,y,nr;
    char c;
    //c1,c2;
    for(int i=0;i<=25;i++){
      urandom.read(&c, 1);
      s=(int)c;
      s=s%(ks+1);
      if(s>=2 && s<=ks){
	//cout<<"Selected s:"<<s<<endl;
	break;
      }
      else
	s=0;      
    }
    //cout<<"Number of streets: "<<s<<endl;

    int n[s];
    for(int j=0;j<s;j++){
      for(int i=0;i<=25;i++){
	urandom.read(&c, 1);
	n[j]=(int)c;
	n[j]=n[j]%(kn+1);
	if(n[j]>=1 && n[j]<=kn){
	  //cout<<"Selected n:"<<s<<endl;
	  break;
	}
	else
	  n[j]=0;      
      }
    }
    /*cout<<"Number of line segments in each street:";
    for(int i=0;i<s;i++) {
      cout<<n[i]<<"\t";
      if(n[i]==0){
	cerr<<"Error: Failed to generate valid number of line segments in each street in 25 attempts"<<endl;
	exit(1);
	break;
      }
    }
    cout<<endl;*/

    for(int i=0;i<=25;i++){
      urandom.read(&c, 1);
      l=(int)c;
      l=l%(kl+1);
      if(l>=5 && l<=kl){
	//cout<<"Selected n:"<<s<<endl;
	break;
      }
      else
	l=5;      
    }
    //cout<<"Before next input wait for :"<<l<<" seconds"<<endl;
    
    for(int i=0;i<=25;i++){
      urandom.read(&c, 1);
      r=(int)c;
      r=r%(kc+1);
      if(r>=1 && r<=kc){
	//cout<<"Selected r:"<<r<<endl;
	break;
      }
      else
	r=0;      
    }
    //cout<<"Range of coordinates is: "<<r<<endl;

    if (s==0){
      cerr<<"Error: Failed to generate valid number of streets in 25 attempts"<<endl;
      exit(1);
    }
    else if (l==0){
      cerr<<"Error: Failed to generate wait seconds before next input in 25 attempts"<<endl;
      exit(1);
    }
    else if(r==0){
      cerr<<"Error: Failed to generate valid range for co-ordinates values in each street in 25 attempts"<<endl;
      exit(1);
    }
    else{
      vector<vector<int>> coor;
      vector<vector<vector<int>>> allstcor;

      //Generating coordinates
      for(int i=0;i<s;i++){	
	int flag=0,cnt=0;
	do{
	  flag=0;
	  coor=gen_coor(n[i],r);
	  
	  //print2d(coor);
	  vector<int> res=calls(coor);
	  //print1d(res);
    int ressize=res.size();
	  for(int i=0;i<ressize;i++){
	    if (res[i]==1){
	      //cout<<res[i]<<"Here flag=1"<<endl;
	      flag=1;
	      break;
	    }
	  }
	  cnt+=1;
	}while (flag==1 and cnt<25);
	if(cnt==25){
	  cerr<<"Error: Coordinates cannot be generated in 25 simultaneous trials"<<endl;
	  exit(1);
	}
	else{
	  allstcor.push_back(coor);
	}
      }


      //Generating unique street names
      old_st.clear();
      copy(st_names.begin(), st_names.end(), back_inserter(old_st));
      st_names.clear();
      st_names=gen_names(s);
      // cout<<"Street:"<<endl;
      // sprint1d(st_names);
      // cout<<"Old Street:"<<endl;
      // sprint1d(old_st);


      
      //Issuing all commands
      vector<string> acmds;
      vector<string> rcmds;
      vector<string> oldst;
      if (first==true){
	//cout<<"In first if"<<endl;
	first=false;
	//Generating a cmd
	acmds=gen_acmd(s,st_names,allstcor);

	//Issue g command
	acmds.push_back("g");
	sprint1d(acmds);
      }
      else{
	//cout<<"In else"<<endl;
	acmds.clear();
	//Generating r cmd
	rcmds=gen_rcmd(old_st.size(),old_st);
	sprint1d(rcmds);
	
	//Generating a cmd
	acmds=gen_acmd(s,st_names,allstcor);

	//Issue g command
	acmds.push_back("g");
	sprint1d(acmds);
      }
    }
    //1 second= 1000000 microsecconds
    //usleep waits for microseconds specified in its arguement
    usleep(l*1000000);
  } while (true);
  return 0;
}
