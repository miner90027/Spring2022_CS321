

#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
using std::ostream;
using std::istream;
using std::string;
using std::vector;
using std::cin;
using std::cout; 



//function declarations
vector<string> lineToTokens(const string& line); // tokenizer
int myHelpFunc(vector<string> &args);  // buitlin functions
int myCdFunc(vector<string> &args);
int myExitFunc(vector<string> &args);
int myLaunch(vector<string> &args);
int myExecute(vector<string> &args);




//global vars/contants
vector<string> names{"help", "cd", "exit"};
vector<int(*)(vector<string>&)> funcPtrs{&myHelpFunc, &myCdFunc, &myExitFunc};
//funcPtrs.push_back(&myHelpFunc);
//funcPtrs.push_back(&myCdFunc);
//funcPtrs.push_back(&myExitFunc);










int main(){

// variables  
    vector<string> tokens;
    string line;
    int status;


// promt and response loop
do{
cout << "<";
//string helpName = "help";
//string cdName = "cd";
//string exitName = "exit";

//auto helpFuncPtr = &myHelpFunc;
//auto cdFuncPtr = &myCdFunc;
//auto exitFuncPtr = &myExitFunc;


// read in line... (?) just stdin for now but I think I need it for a file too?
    if(std::getline(cin, line)){
 
// process line 
    tokens = lineToTokens(line);



// test we got tokens ... .JUST FOR TESTING
//for(int i=0; i<tokens.size(); i++){ 
//    cout << tokens.at(i) << ":";
//}
//cout << "\n";


// call the coresponding function or execute the specified file
//int success = myLaunch(tokens);
//if(success){cout << "success";}
//else{ cout << "returned but no success";}
//if(myHelpFunc(tokens)==0){cout << "returned from help\n";} // FOR TESTING
//if(myCdFunc(tokens)==0){cout << "returned form cd\n";} // FOR TESTING
//if(myExitFunc(tokens)==0){cout << "return from exit\n";} // FOR TESTING
    status = myExecute(tokens);


    }

    else{
        cout << "coutln't getline \n";
    }
} while(status);


return 0;
}




// break a single line into tokens
vector<string> lineToTokens(const string& line) {
	vector<string> vec;
	int len = 0; // the length of the current word
	int pos = 0; // the position we recorded the last space at (+1 to avoid copying the space character)
	auto lastIt = line.begin(); // the last iterator before i

	for (auto i = line.begin(); i < line.end(); i++) {

		if (*lastIt == ' ') {
			pos = i - line.begin();
			len = 0;
			// and move on...
		}

		else if(*i == ' '){
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
                cout << "running my " << names.at(i) << "function\n";
                return (funcPtrs.at(i))(args); // run it 
            }
        }    
    }
return myLaunch(args);
}









