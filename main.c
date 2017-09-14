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
	//executes an execlp statement
	static void fork_exec(const char *file, const char *program, string args = ""){
		pid_t	pid;
		pid = fork();
		if (pid < 0){	//error
			return;
		} 
		else if (pid == 0){	/* Child Process */
			if(args.size()>0)
				execlp(file, program, args.c_str(), NULL);
			execlp(file, program, NULL);
		}
		else{  		/* parent process */
			wait(NULL);		
		}
		
	}
	public:
	static int cd(string directory){
		if(directory == ""){
			char buffer[MAXPATHLEN];
    		char *path = getcwd(buffer, MAXPATHLEN);
			cout << path << endl;
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
		cout << "unknown request" << endl;	
	}
}

int main(){
	//get input
	string input;
	while(true){
		getline(cin, input);
		cmd(input);
	}
	return 0;
}


