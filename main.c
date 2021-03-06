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

bool VERBOSE = false;

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
			//expand packed parameters to execlp
			execlp(file, args..., NULL);
			//failure state if execlp returns
			exit(EXIT_FAILURE);
		}
		else{  		/* parent process */
			waitpid(pid, NULL, 0);		
		}
	}

	//executes an execvp statement with a vector of program arguments
	static void fork_execvp(string file, vector<string> argv){
		//pointer for vector to char** conversion
		char** ptr; 
		// char pointer array for execvp arguments
		const char* arg_arr[argv.size() + 1]; 
		//point all char* to cstring representations of strings in vector
		for (unsigned int i = 0; i < argv.size(); i++){
			arg_arr[i] = argv[i].c_str();
		}
		//null terminated argument list
		arg_arr[argv.size()] = NULL;
	
		pid_t	pid;
		pid = fork();
		if (pid < 0){	//error
			return;
		} 
		else if (pid == 0){	/* Child Process */
			ptr = (char**)arg_arr;
			execvp(file.c_str(), ptr);
			//failure state if execvp returns
			exit(EXIT_FAILURE);
		}
		else{  		/* parent process */
			//wait for status change of child
			waitpid(pid, NULL, 0);
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
	static void cd(string directory){
		if(directory == ""){
			cout << get_path() << endl;
			return;
		}
		int status = chdir(directory.c_str());
		if (status == -1){
			cout << "error in opening directory \"" << directory << "\"" << endl; 			
			return;		
		}
		fork_execlp("/bin/ls","ls");
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
		//if there is not a '/' in the path, it is interpreted as a program and needs an empty second argument.
		if(filename.find("/") == string::npos){
			vec.push_back("");
		}
		split_string(app_string.substr(filename.size()), vec);
		fork_execvp(filename, vec);
	}
};


//decision making function to take input and apply known commands, 
//or fall through to execute with exec statement
void cmd(string input){
	vector<string> split = split_string(input);

	string selection;
	string options;

	if(split.size()>0)
		{selection = split[0];}
	if(split.size()>1)
		{options = split[1];}
	else{options = "";}

	//change directory
	if (selection == "cd"){
		Command::cd(options);
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
	//exit program
	else if(selection == "quit"){
		exit(0);	
	}
	else{
		if(VERBOSE) cout << "attempting to run program: " << input << endl;
		Command::run(input);	
	}
}

//fork process to use cmd function in a child process
void forked_cmd(string input){
	pid_t	pid;
	pid = fork();
	if (pid < 0){	//error
		cout << "An error occurred in creating child process" << endl;
	} 
	else if (pid == 0){	/* Child Process */
		cmd(input);
		if(VERBOSE) cout << "exiting child process" << endl;
		exit(0);
	}
	else{  		/* parent process */
		if(VERBOSE) cout << "Child process in pid = " << pid << endl;
		return;		
	}

}

//Entry point
int main(int argc, char *argv[]){
	if(argc > 1)
		if(strcmp(argv[1], "-v") == 0)
			VERBOSE = true;

	string input;
	string whitespace = " \t\f\v\n\r";
	char last_char = ' ';
	size_t last_char_index;

	while(true){
		//prompt
		cout << Command::get_path() << ": ";
		//get input
		getline(cin, input);
		//keep last non-whitespace character for comparison
		last_char_index = input.find_last_not_of(whitespace);
		if(last_char_index != string::npos)
			last_char = input[last_char_index];
		else last_char = ' ';
		//ampersand is a fork instruction for the execution
		if(last_char == '&'){
			if(VERBOSE) cout << "forking a command" << endl;
			forked_cmd(input.substr(0, last_char_index));
		}
		//otherwise execute the string inline
		else cmd(input);
		//check for zombie processes waiting for removal
		waitpid(-1, NULL, WNOHANG);
	}
	return 0;
}


