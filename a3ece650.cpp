#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <cstring>
#include <fstream>

using namespace std;

string reader(FILE *istream){
  char buffer[1024];
  string s;
  while (!feof(istream)
	 && !ferror(istream)
	 && fgets(buffer,sizeof(buffer),istream)!=NULL) {
    fputs(buffer,stdout);
    s=s+buffer;
  }
  fclose(istream);
  return s;
}

void writer(const char*  msg,int cnt,FILE *ostream){
  for(;cnt>0;cnt--){
    fprintf(ostream,"%s\n",msg);
    cout<<msg<<endl;
    fflush(ostream);
    sleep(1);
  }
  return;
}

int main(int argc,char* argv[]){
  // int mainid=getpid();
  // cout<<"Main:"<<mainid<<endl;
  argv[argc]=NULL;
  int fds[2];
  int fds1[2];
  pipe (fds);
  pipe (fds1);
  //string str1,str2;
  pid_t pid1,pid2,pid3;

  pid1=fork();
  if(pid1==(pid_t)0){
    // cout<<"First child rgen : "<<getpid()<<"\t"<<getppid()<<endl;
    dup2(fds[1],STDOUT_FILENO);
    dup2(fds[1],STDERR_FILENO);
    close(fds[0]);
    close(fds[1]);
    int p=execv("./rgen",argv);
    if (p==-1){
      cerr<<"Error: cannot open rgen file"<<endl;
    }
  } //rgen done

  else{
    // cout<<"In parent: "<<getpid()<<"\t"<<getppid()<<endl;
    usleep(100000);
    kill(pid1,SIGKILL);
    waitpid(pid1,NULL,0);
    // cout<<"After first child rgen : "<<getpid()<<"\t"<<getppid()<<endl;
    
    pid2=fork();
    if(pid2==(pid_t)0){
      // cout<<"Second child python : "<<getpid()<<"\t"<<getppid()<<endl;
      dup2(fds[0],STDIN_FILENO);
      dup2(fds1[1],STDOUT_FILENO);
      dup2(fds1[1],STDERR_FILENO);
      char* arg_l[]={(char*)"/usr/bin/python3",
	(char*)"a1ece650.py", NULL};
      int p=execv("/usr/bin/python3", arg_l);
      if(p==-1)
	cerr<<"Error: in opening python file: "<<p<<endl;
      close(fds[0]);
      close(fds[1]);
      close(fds1[0]);
      close(fds1[1]);
    }// python done
    waitpid(pid2,NULL,0);
  }  //regen and python complete

  waitpid(pid1,NULL,0);   waitpid(pid2,NULL,0);
  pid3=fork();
  if(pid3==(pid_t)0){
    // cout<<"In second child : "<<getpid()<<"\t"<<getppid()<<endl;
    string i1,i2;
    dup2(fds1[0],STDIN_FILENO);
    getline(cin,i1);
    ofstream f1("graph.txt");
  
    if (i1[0]=='E'){
      //cerr<<i1<<endl;
      f1<<i1<<"\n";
      f1.close();
      exit(0);
    }
    else{
      getline(cin,i2);
      f1<<i1<<"\n";
      f1<<i2<<"\n";
      f1.close();
    }
  }
  else{
    // cout<<"In second parent : "<<getpid()<<"\t"<<getppid()<<endl;
    waitpid(pid3,NULL,0);
    // cout<<"After execution of second child"<<endl;
    string i3,i4;
    ifstream f2("graph.txt");
    // cout<<"From graph.txt"<<endl;    
    getline (f2,i3);
    if (i3[0]=='E'){
      cerr<<i3<<endl;
      f2.close();
      exit(1);
    }
    else{
      getline(f2,i4);
      f2.close();
    }
    
    //cout<<i3<<endl<<i4<<endl;

    char *a=new char[i3.size()+1];
    strcpy(a,i3.c_str());
    char *b=new char[i4.size()+1];
    strcpy(b,i4.c_str());
    char* arg[]={(char*)"./a2ece650",a,b,NULL};
    execv("./a2ece650",arg);
    }// shortest path done
  return 0;
}

