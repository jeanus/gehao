#include<iostream>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<cstdio>
#include<string>
#include<string.h>
#include<vector>
using namespace std;

vector<string> split(string str, const char * d) {
        char * writable = new char[str.size() + 1];
        std::copy(str.begin(), str.end(), writable);
        writable[str.size()] = '\0';
        vector<string> v;
        char *p;
        p = strtok(writable, d);
        while (p)
        {
                string s(p);
                v.push_back(s);
                p = strtok(NULL, d);
        }
        delete[] writable;
        return v;
}


bool start_with(const string &mainstr, const string &substr){
        return mainstr.find(substr)==0 ? 1 : 0;
}

bool check_redirect(vector<string>& vec){
        bool find = false;
        for(vector<string>::iterator a = vec.begin();a<vec.end();a++){
                string str = *a;
                if(str.compare(">")==0){
                        vec.erase(a);
                        string filename = *a;
                        vec.erase(a);
                        a--;
                        freopen(filename.c_str(),"w",stdout);
                        find = true;
                }else if(str.compare("<")==0){
                        vec.erase(a);
                        string filename = *a;
                        vec.erase(a);
                        a--;
                        freopen(filename.c_str(),"r",stdin);
                        find = true;
                }else if (str.compare(">>")==0){
                        vec.erase(a);
                        string filename = *a;
                        vec.erase(a);
                        a--;
                        freopen(filename.c_str(),"a",stdout);
                        find = true;
                }else if(start_with(str,">>")){
                        string filename = (*a).substr(2);
                        vec.erase(a);
                        a--;
                        freopen(filename.c_str(),"a",stdout);
                        find = true;
                }else if(start_with(str,">")){
                        string filename = (*a).substr(1);
                        vec.erase(a);
                        a--;
                        freopen(filename.c_str(),"w",stdout);
                        find = true;
                }else if(start_with(str,"<")){
                        string filename = (*a).substr(1);
                        vec.erase(a);
                        a--;
                        freopen(filename.c_str(),"r",stdin);
                        find = true;
                }

        }
        return find;

}

static void run(char* const argv[], int in, int out) {
        dup2(in, 0);   /* <&in  : child reads from in */
        dup2(out, 1); /* >&out : child writes to out */
        execvp(argv[0], argv);
}

void run_pipeline(const char** command[],int n){
        int i,in=0; /* the first command reads from stdin */
        for ( i=0; i < n; ++i) {
                int status;
                int fd[2]; /* in/out pipe ends */
                pid_t pid; /* child's pid */

                pipe(fd);
                pid = fork();
                if (pid == 0) {
                        /* run command[i] in the child process */
                        if(i!=0)
                                close(fd[0]); /* close unused read end of the pipe */
                        if(i==(n-1))
                                run((char * const*)command[i], in, 1); /* $ command < in */
                        else
                                run((char * const*)command[i], in, fd[1]); /* $ command < in > fd[1] */
                }
                else { /* parent */
                        close(fd[1]); /* close unused write end of the pipe */
                        if(i!=0)
                                close(in);    /* close unused read end of the previous pipe */
                        in = fd[0]; /* the next command reads from here */
                }
                wait(&status);
        }
}


int main(int argc, char* argv[]){
        const char *ls[] = { "ls", "-l", NULL };
        const char *sort[] = { "sort", "-k9", NULL };
        const char *head[] = { "head","-5", NULL };
        const char** command_pipe[] = { ls, sort, head };
        run_pipeline(command_pipe,3);

        int pid, status;
        cout << "$ " ;
        string command;
        getline(cin,command);
        cout<<"dsa"<<endl;
        vector<string> vec;
        char** args_array ;
        while(!cin.eof()){
                int oldin = dup(0);
                int oldout = dup(1);
                vec = split(command," ");
                if(vec.size()>0){
                        if(vec[0].compare("exit")==0)
                                break;
                        bool find = check_redirect(vec);
                        if(vec[0].compare("cd")==0){
                                if(vec.size()>1){
                                        if(vec[1].compare("~")==0)
                                                chdir(getenv("HOME"));
                                        else
                                                chdir(vec[1].c_str());
                                }
                        } else if(vec[0].compare("echo")==0){
                                if(find){
                                        for(vector<string>::iterator a = vec.begin()+1;a<vec.end();a++){
                                                cout<<(*a)<<" ";
                                        }
                                        cout<<endl;}
                                else{
                                        cout<<command.substr(5)<<endl;
                                }
                        }
                        else {
                                args_array = new char*[vec.size()+1];
                                int i=0;
                                for(vector<string>::iterator a = vec.begin();a<vec.end();a++){
                                        args_array[i] =(char*)((*a).data());
                                        i++;
                                }
                                args_array[vec.size()] = NULL;
                                int pid = fork();
                                if(pid ==0){
                                        execvp(args_array[0],args_array);
                                        perror("Unable to execute the command");
                                        exit(EXIT_FAILURE);
                                }
                                pid = wait(&status);
                        }
                }
                dup2(oldin,0);
                dup2(oldout,1);
                cout<<"$ ";
                getline(cin,command);
        }

        return 0;
}
