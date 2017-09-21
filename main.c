#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/param.h>
#include <cstdlib>
#include <sys/wait.h>

#include "utility.h"
using namespace std;

//encapsulates all user commands from shell
class Command{
	//executes an execlp statement using template parameter packing to allow any number of arguments
	template<typename... Args>
	static void fork_execlp(const char *file, Args... args){
		pid_t	pid;
		pid = fork();
		if (pid < 0){	//error
			return;
		} 
		else if (pid == 0){	/* Child Process */
			execlp(file, args..., NULL);
		}
		else{  		/* parent process */
			wait(NULL);		
		}
	}

	//executes an execvp statement with 
	static void fork_execvp(string file, vector<string> argv){
		char** ptr;
		const char* arg_arr[argv.size() + 1];
		for (unsigned int i = 0; i < argv.size(); i++){
			arg_arr[i] = argv[i].c_str();
		}
		arg_arr[argv.size()] = NULL;
	
		pid_t	pid;
		pid = fork();
		if (pid < 0){	//error
			return;
		} 
		else if (pid == 0){	/* Child Process */
			ptr = (char**)arg_arr;
			execvp(file.c_str(), ptr);
		}
		else{  		/* parent process */
			wait(NULL);		
		}
	}

	public:
	//Returns the current working directory
	static char * get_path(){
			char buffer[MAXPATHLEN];
    		return getcwd(buffer, MAXPATHLEN);
	}

	//Changes the directory to the supplied location.
	//If no location is supplied it simply prints the current one
	static int cd(string directory){
		if(directory == ""){
			cout << get_path() << endl;
			return 1;
		}
		int status = chdir(directory.c_str());
		fork_execlp("/bin/ls","ls");
		if (status<0)
			return 0;
		return 1;
	}

	//clears screen
	static void clr(){
		fork_execlp("/usr/bin/clear", "clear");
	}

	//prints the current working directory contents or the supplied directory's contents
	static void dir(string directory){
		if(directory == ""){
			fork_execlp("/bin/ls", "ls");
		}
		else { 
			fork_execlp("/bin/ls", "ls", directory);
		}
	}

	//prints the supplied string back to the user
	static void echo(string comment){
		cout << comment << endl;
	}

	//echo with vector and start index functionality
	static void echo(vector<string> comment, int start_index = 0){
		for(unsigned int i=start_index; i < comment.size(); i++){
			cout << comment[i] << " ";		
		}
		cout << endl;
	}

	//executes the string as a command line file and arguments
	static void run(string app_string){
	//attempt to extract name, arguments
	vector<string> vec;
	stringstream stream(app_string);
	string filename, program;

	stream >> filename;
	if(filename.find("/") == string::npos){
		vec.push_back("");
	}
	split_string(app_string.substr(filename.size()), vec);
	fork_execvp(filename, vec);
	}
};



void cmd(string input){
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
	else if(selection == "quit"){
		exit(0);	
	}
	else{
		Command::run(input);
		//cout << "unknown request" << endl;	
	}
}

int main(){
	//get input
	string input;
	while(true){
		//prompt
		cout << Command::get_path() << ": ";
		//get input
		getline(cin, input);
		//attempt to run command
		cmd(input);
	}
	return 0;
}


