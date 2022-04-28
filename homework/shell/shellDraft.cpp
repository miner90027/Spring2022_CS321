
#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <regex>
using std::regex;
using std::ostream;
using std::istream;
using std::string;
using std::vector;
using std::cin;
using std::cout; 



//function declarations
vector<string> lineToTokens(const string& line, char delim); // tokenizer
int myHelpFunc(vector<string> &args);  // buitlin functions
int myCdFunc(vector<string> &args);
int myExitFunc(vector<string> &args);
int myPrintFunc(vector<string> &args);
int myLaunch(vector<string> &args);
int myExecute(vector<string> &args);




//global vars/contants
vector<string> names{"help", "cd", "exit", "print"};
vector<int(*)(vector<string>&)> funcPtrs{&myHelpFunc, &myCdFunc, &myExitFunc, &myPrintFunc};


int main(){

// variables  
    vector<string> tokenGroups;
    vector<string> tokens;
    string line;
    int status;
    const regex pattern("^[a-zA-Z0-9-./_ ]+$");
    char buffer[1000];
    int numCmds; // number of commands in an entered line
    


// promt and response loop
do{
if(getcwd(buffer,sizeof(buffer)) != NULL){cout << buffer;}
else{perror("getcdw() error");}
cout << "% ";

// read in line
    if(std::getline(cin, line)){
// make sure line has something 
        if(line.size() <= 0){ 
            // go back to loop top
        }

        else{ // continue... to process stuff

// make sure line isn't over 100 characters ( if it i it will be truncated and we keep going)
// this is sort of an arbitrary limit since we used getline, but lets do it anyway
        //cout << "number of chars befor: " << line.size();
        if(line.size() > 100){ 
            cout << "Line longer than 100 chars entered\n";
            cout << "Note: it will be truncated to 100 chars\n"; 
            line = line.substr(0,100);   
        }

// process line into token groups
        tokenGroups = lineToTokens(line, '&');

for(int i=0; i< tokenGroups.size(); i++){
    // make sure the line only contains A-Z, a-z, 0-9 -./_ (dash, dot, forward slash, underscore)
        if(!regex_match(tokenGroups.at(i),pattern)){
            cout << "invalid character entered try again\n";
            break; // don't keep processing
        }

    // break the group up into tokens
    tokens = lineToTokens(tokenGroups.at(i),' ');

    // call the coresponding function or execute the specified file
    status = myExecute(tokens); // the only thing is I don't think  exit will exit unless its the last command ented on a line but I believ this is the behavior of the shell in my linux machine so that should be fine
}
        

    }
    }
    
    else{
        cout << "coutln't getline \n";
    }

} while(status);


return 0;
}




// break a single line into tokens
vector<string> lineToTokens(const string& line, char delim) {
	vector<string> vec;
	int len = 0; // the length of the current word
	int pos = 0; // the position we recorded the last space at (+1 to avoid copying the space character)
	auto lastIt = line.begin(); // the last iterator before i

	for (auto i = line.begin(); i < line.end(); i++) {

		if (*lastIt == delim) {
			pos = i - line.begin();
			len = 0;
			// and move on...
		}

		else if(*i == delim){
			string newToken = line.substr(pos, len);
			vec.push_back(newToken);
			len = 0;
			pos = i - line.begin() + 1;
		}

		len++;
		lastIt = i;
	}

	if (line.back() != ' ') {
		vec.push_back(line.substr(pos)); // get the last word 
	}
	

	return vec;
}



// prints help statements
int myHelpFunc(vector<string> &args){
    cout << "CS321 simple shell\n";
    cout << "Type program names and arguments and hit enter\n";
    cout << "The following are builtin\n";
    
    for(auto i: names){
        cout << i << "\n";
    }

return 1; // 1 so we prompt again (keep looping)
}

//changed directory to specifed place 
int myCdFunc(vector<string> &args){

    if(args.size() < 2){
        cout << "shell expected an argument to \"cd\"\n";

    }

    else{
       if(chdir(args.at(1).c_str()) != 0){
            perror("my shell perror");
        }
    }
return 1; // 1 so we'll loop again and ask for another command to run
}

// exits the process...(?)
int myExitFunc(vector<string> &args){
return 0; // zero so the ststus will stop the do while loop int main
}

// prints the current pid
int myPrintFunc(vector<string> &args){
    cout << "current process ID: " << getpid() << "\n";   
    return 1;
}




// run the program or command specified in the file passed
// for non-builtin commands
// must specify program as first argument
// and its run with ./programToRun
int myLaunch(vector<string> &args){
    pid_t pid, wpid;
    int status;

    // turn args into a vector of char* instead of strings 
    // so the execvp function will accept it later  
    // sort of a silly solution but I just hate dealing with c-strings
    // so I'm avoiding it where I can
    char **cArgs = (char**) malloc(sizeof(char*)*(args.size() + 1));
    for(int i=0; i< args.size(); i++){
        cArgs[i] = strdup(args.at(i).c_str());   
    }
    cArgs[args.size()] = nullptr;

    
    pid = fork();
    if(pid == 0){
        // child process does stuff here
        if(execvp(args.at(0).c_str(), cArgs) == -1){
            perror("my shell perror");// if execvp returns somethings wrong      
        }        
        exit(EXIT_FAILURE);
    }
    else if (pid<0){
        // error forking
        perror("my shell perror");    
    }
    else{
        // parent process lands here
        // and must wait for child to finish
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }   while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    // free the memory taken to turn args into a c-style char** 
    for(int i=0; i< args.size(); i++){
        free(cArgs[i]);    
    }
    free(cArgs);
    

    return 1; // 1 lets the calling function know we should promot for input again
}




// run the program from the file or run the specified buitlin functions
int myExecute(vector<string> &args){
    if(args.size() == 0){
        // an empty command was entered
        return 1;        
    }

    else{
        for(int i=0; i< funcPtrs.size(); i++){ // for all of the builtins
            if(args.at(0) == names.at(i)){ // if the first argument is a builtin
                return (funcPtrs.at(i))(args); // run it with its arguments
            }
        }    
    
    return myLaunch(args); // otherwise launch the program with its arguments 
    }
}
































  
