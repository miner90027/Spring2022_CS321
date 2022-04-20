#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
using std::ostream;
using std::istream;
using std::string;
using std::vector;
using std::cin;
using std::cout;

//function declarations
vector<string> lineToTokens(const string& line);


int main(){

// variables  
vector<string> tokens;
string line;



// read in line... (?) just stdin for now but I think I need it for a file too
if(std::getline(cin, line)){
 
// process line 
tokens = lineToTokens(line);



// test we got tokens
for(int i=0; i<tokens.size(); i++){
    cout << tokens.at(i) << ":";
}
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


