#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
#include <unistd.h>

using std::ostream;
using std::istream;
using std::string;
using std::vector;
using std::cin;
using std::cout;

//function declarations
vector<string> lineToTokens(const string& line); // tokenizer
int myHelpFunc(vector<string> &args); 
int myCdFunc(vector<string> &args);
int myExitFunc(vector<string> &args);


//global vars/contants
vector<string> names{"help", "cd", "exit"};
vector<int(*)(vector<string>&)> funcPtrs{&myHelpFunc, &myCdFunc, &myExitFunc};
//funcPtrs.push_back(&myHelpFunc);
//funcPtrs.push_back(&myCdFunc);
//funcPtrs.push_back(&myExitFunc);










int main(){

//string helpName = "help";
//string cdName = "cd";
//string exitName = "exit";

//auto helpFuncPtr = &myHelpFunc;
//auto cdFuncPtr = &myCdFunc;
//auto exitFuncPtr = &myExitFunc;


// variables  
vector<string> tokens;
string line;



// read in line... (?) just stdin for now but I think I need it for a file too?
if(std::getline(cin, line)){
 
// process line 
tokens = lineToTokens(line);



// test we got tokens ... .JUST FOR TESTING
for(int i=0; i<tokens.size(); i++){ 
    cout << tokens.at(i) << ":";
}
cout << "\n";


// call the coresponding function 
if(myHelpFunc(tokens)==0){cout << "returned from help\n";}
if(myCdFunc(tokens)==0){cout << "returned form cd\n";}
if(myExitFunc(tokens)==0){cout << "return from exit\n";}



}

else{
    cout << "coutln't getline \n";
}

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
    cout << "The followling are builtin\n";
    
    for(auto i: names){
        cout << i << "\n";
    }

return 0;
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
return 0;
}

// exits the process...(?)
int myExitFunc(vector<string> &args){
return 0;
}









