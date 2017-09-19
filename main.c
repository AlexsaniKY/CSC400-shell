#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/param.h>
#include <cstdlib>
#include <sys/wait.h>

using namespace std;

//encapsulates all user commands from shell
class Command{
	//executes an execlp statement using template parameter packing to allow any number of arguments
	template<typename... Args>
	static void fork_exec(const char *file, Args... args){
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
	public:
	static char * get_path(){
			char buffer[MAXPATHLEN];
    		return getcwd(buffer, MAXPATHLEN);
	}
	static int cd(string directory){
		if(directory == ""){
			cout << get_path() << endl;
			return 1;
		}
		int status = chdir(directory.c_str());
		fork_exec("/bin/ls","ls");
		if (status<0)
			return 0;
		return 1;
	}
	static void clr(){
		fork_exec("/usr/bin/clear", "clear");
	}
	static void dir(string directory){
		if(directory == ""){
			fork_exec("/bin/ls", "ls");
		}
		else { 
			fork_exec("/bin/ls", "ls", directory);
		}
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
	static void run(string app_string){
	//attempt to extract directory, name, arguments
	stringstream stream(app_string);
	string program, args;
	//stream >> file;
	//stream >> program;
	//getline(stream, args);
	//int end_of_whitespace = args.find_first_not_of(" ");
	//string trimmed_args = args.substr(end_of_whitespace);
	//fork_exec(file.c_str(), program.c_str(), args.c_str());
	fork_exec("git", "", "commit", "-m", "'attempting to use variadic templates to wrap exec statement'");
	}
};

//Helper function to split all commands and arguments provided by user
vector<string> split_string(string input){
	stringstream stream(input);

	vector<string> split_input;
	string token;
	while (stream >> token){
		split_input.push_back(token);
	}
	return split_input;
}


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


