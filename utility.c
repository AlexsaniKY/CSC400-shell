#include "utility.h"

//Helper functions to split all commands and arguments provided by user
void split_string(string input, vector<string> &target_vec){
	stringstream stream(input);
	string token;
	while (stream >> token){
		target_vec.push_back(token);
	}
	return ;
}
vector<string> split_string(string input){
	vector<string> split_input;
	split_string(input, split_input);
	return split_input;
}

