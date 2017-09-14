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
			cout << path << endl;
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
	static void echo(vector<string> comment, int start_index){
		for(unsigned int i=start_index; i < comment.size(); i++){
			cout << comment[i] << " ";		
		}
		cout << endl;
			
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
	string selection;
	string options;

	if(split.size()>0)
		{selection = split[0];}
	if(split.size()>1)
		{options = split[1];}
	else{options = "";}
	
	//change directory
	if (selection == "cd"){
		status = Command::cd(options);
	}
	//clear screen
	else if (selection == "clr"){
		Command::clr();
	}
	//set directory or report current
	else if (selection == "dir"){
		Command::dir(options);
	}
	//print the given argument on the screen
	else if(selection == "echo"){
		Command::echo(split, 1);
	}


	return 0;
}


