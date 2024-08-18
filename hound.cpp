#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>
#include <vector>
#include "ansi.hpp"
using namespace std;
using namespace filesystem;

int help();
int typeIdentifier(string fsname);
int infos[4] = {
	0,	// chomp
	1,	// angry
	0,	// verbose 
	0	// title
};


int destroyFile(string filename) {
	string angry_lvl = to_string(infos[1]);
	string shred = "shred -n " + angry_lvl + " \"" + filename + "\" > /dev/null"; 
	string eat = "rm " + filename + " > /dev/null";

	// shred
	int shredded = !system(shred.c_str());
	if (!shredded) {
		cout << CRED << "The file couldn't be defaced!" << CDEF << endl;
		return 1;
	} 

	// remove
	if (infos[0] == 0) {
		int eaten = !system(eat.c_str());
		if (!eaten) {
			cout << CRED << "The file couldn't be deleted!" << CDEF << endl;
			return 1;
		}
	}
	
	return 0;
}

int destroyTargets(vector<string> files) {
	// iterate through the list of files and identify their types.
	for (string file : files) {
		typeIdentifier(file);
	}

	return 0;
} 

int destroyFolder(string foldername) {
	// look inside the folder, collect the files, and identifies them.
	// !!! the file isn't detected with it's basename !!!
	vector<string> files;
	for (auto obj_file : directory_iterator(foldername)) {
		string file = obj_file.path().filename().string();
		string filepath = foldername + "/" + file;
		files.push_back(filepath);
	}
	destroyTargets(files);

	// destroy the folder
	if (infos[0] == 0) {
		string eat = "rm -r \"" + foldername + "\" > /dev/null";
		int eaten = !system(eat.c_str());
		if (!eaten) {
			cout << CRED << "The folder '" << foldername << "' couldn't be removed!" << CDEF << endl;
			return 1;
		}
	}
	return 0;	
}

int typeIdentifier(string fsname) {
	if (!exists(fsname)) {
		cout << CRED << "The target '" << fsname << "' doesn't exist!" << CDEF << endl;
		return 1;
	}

	if (is_directory(fsname)) {
		return destroyFolder(fsname);
	} 
	return destroyFile(fsname);
}


int main(int argCount, char* argValue[]) {
  
	vector<string> files;
	if (argCount < 2) {return help();}

	// process all the arguments.
	for (int i=1; i<argCount; i++) {
		
		if (argValue[i][0] != '-') {
			// add the filepath to the list of files to destroy.
			files.push_back(argValue[i]);
			continue;
		}

		// check if the argument call for help.
		if (!strcmp(argValue[i], "-h") || !strcmp(argValue[i], "--help")) {
			return help();
		}

		// check if the argument call for chomp.
		if (!strcmp(argValue[i], "-c") || !strcmp(argValue[i], "--chomp")) {
			infos[0] = 1;
			continue;
		}

		// check if the argument call for angry
		if (!strcmp(argValue[i], "-a") || !strcmp(argValue[i], "--angry")) {
			if (i+1 > argCount) {
				cout << CRED << "Please set a value for the angry value!" << CDEF << endl;
				return 1;
			}

			int value;
			if(sscanf(argValue[i+1], "%d", &value)) {
				infos[1] = value;
				i++;
				continue;
			} 
			else {
				cout << CRED << "Please set a correct angry value!" << CDEF << endl;
				return 1;
	 		}
		}

		cout << CRED << "This flag doesn't exist!" << CDEF << endl;
		cout << "You can check the flags with : " << CHIGH << CBOLD << "hound --help" << CDEF << endl;
		return 1;
	}

	// destroy all the targets in the list.
	destroyTargets(files);
	
	return 0;
}


int help() {
	cout << houndIco << endl;
	cout << "Hound is a software made to destroy data without any recovery chances." << endl;
	cout << "Usage : hound <flag.s> [folder/file]" << endl << endl;
	cout << CBOLD << "<Flags>" << CDEF << endl;
	cout << "-a (--angry)   :\t the file will get defaced [x] more time." << endl;
	cout << "-c (--chomp)   :\t doesn't erase the file at the end." << endl;
	cout << endl;
	cout << CBOLD << CHIGH << "! Disclamer !" << CDEF << "\nPlease, use hound only, on your " << CBOLD << "OWN" << CDEF << " data!" << endl;
	cout << "The author of the program is not responsible for any actions made with this program." << endl;
	return SUCCESS;
}
