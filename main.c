#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/param.h>
#include <cstdlib>

using namespace std;

class Command{
	public:
	int cd(string directory){
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
	void clr(){
		execlp("/usr/bin/clear","clear",NULL);
	}
	void dir(string directory){}
	void echo(string comment){}
};

vector<string> split_string(string input){
	stringstream stream(input);

	vector<string> split_input;
	string token;
	while (stream >> token)
		split_input.push_back(token);
	return split_input;
}

int main(){
//cout << MAXPATHLEN;
string input;
getline(cin, input);

vector<string> split = split_string(input);

int status = -1;
Command cmd;
if(split.size()>0){
	if (split[0] == "cd"){
		if (split.size()>1)
			status = cmd.cd(split[1]);
		else status = cmd.cd("");
	}
	else if (split[0] == "clr")
		cmd.clr();

	if (status == 0)
		cout << "failed" << endl;
	else cout << "success" << endl;
}

//for (unsigned int i=0; i<split.size(); i++)
//	cout << split[i] << endl;


return 0;
}


