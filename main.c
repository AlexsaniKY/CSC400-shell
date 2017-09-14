#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/param.h>
#include <cstdlib>

using namespace std;

//encapsulates all user commands from shell
class Command{
	public:
	static int cd(string directory){
		if(directory == ""){
			char buffer[MAXPATHLEN];
    		char *path = getcwd(buffer, MAXPATHLEN);
			cout << path;
			return 1;
		}
		int status = chdir(directory.c_str());
		execlp("/bin/ls","ls",NULL);
		if (status<0)
			return 0;
		return 1;
	}
	static void clr(){
		execlp("/usr/bin/clear","clear",NULL);
	}
	static void dir(string directory){
		if(directory == "")
			execlp("/bin/ls","ls", NULL);
		else execlp("/bin/ls","ls", directory.c_str(), NULL);
	}
	static void echo(string comment){
		cout << comment << endl;
	}
};

//Helper function to split all commands and arguments provided by user
vector<string> split_string(string input){
	stringstream stream(input);

	vector<string> split_input;
	string token;
	while (stream >> token)
		split_input.push_back(token);
	return split_input;
}


int main(){
//get input
string input;
getline(cin, input);

vector<string> split = split_string(input);


int status = -1;

if(split.size()>0){
	//change directory
	if (split[0] == "cd"){
		if (split.size()>1)
			status = Command::cd(split[1]);
		else status = Command::cd("");
	}
	//clear screen
	else if (split[0] == "clr"){
		Command::clr();
	}
	//set directory or report current
	else if (split[0] == "dir"){
		if(split.size()>1)
			Command::dir(split[1]);
		else Command::dir("");
	}
	//print the given argument on the screen
	else if(split[0] == "echo"){
		if(split.size()>1)
			Command::echo(split[1]);
		else Command::echo("");
	}
}


return 0;
}


