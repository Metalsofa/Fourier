#pragma once
#include <string>
#include <vector>
#include <fstream>

using namespace std;

const string cipher_base_alphabet = 
"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz;:[]{}<>', .?/!@#$%^&*()-_=+|`~";


string alphabet_shift(char new_firstletter, string base_alphabet) {
	string new_alphabet = base_alphabet;
	char inifinity_preventer = base_alphabet[0];
	bool firsttry = true;
	while (new_alphabet[0] != new_firstletter) {
		if (new_alphabet[0] == inifinity_preventer && !firsttry) {
			break;
		}
		if (new_alphabet[0] == inifinity_preventer) {
			firsttry = false;
		}
		new_alphabet = new_alphabet + new_alphabet[0];
		new_alphabet.erase(new_alphabet.begin());
	}
	return new_alphabet;
}

int letter_index(char letter, string alphabet) {
	unsigned int i = 0;
	while (i < alphabet.size()) {
		if (alphabet[i] == letter)
			return i;
		i++;
	}
	return 0;
}

string encrypt(string message, string keyword) {
	string cipher = "";
	int i = 0;
	if (keyword == "")
		keyword = "0";
	for (char c : message) {
		int letterindex = letter_index(c, cipher_base_alphabet);
		int keyletternum = int(i % keyword.size());
		char keyletter = keyword[keyletternum];
		string shiftebet = alphabet_shift(keyletter, cipher_base_alphabet);
		char newletter = shiftebet[letterindex];
		cipher = cipher + newletter;
		i++;
	}
	return cipher;
}

string multi_encrypt(unsigned int depth, string message, string keyword) {
	unsigned int i = 0;
	if (keyword == "")
		keyword = "0";
	while (i < depth) {
		message = encrypt(message, keyword);
		i++;
	}
	return message;
}

string unencrypt(string cryptogram, string keyword) {
	string message = "";
	if (keyword == "")
		keyword = "0";
	int i = 0;
	for (char c : cryptogram) {
		int keyletternum = int(i % keyword.size());
		char keyletter = keyword[keyletternum];
		string shiftebet = alphabet_shift(keyletter, cipher_base_alphabet);
		int letterindex = letter_index(c, shiftebet);
		char newletter = cipher_base_alphabet[letterindex];
		message = message + newletter;
		i++;
	}
	return message;
}

string multi_unencrypt(unsigned int depth, string cipher, string key) {
	unsigned int i = 0;
	if (key == "")
		key = "0";
	while (i < depth) {
		cipher = unencrypt(cipher, key);
		i++;
	}
	return cipher;
}

//Takes a filename and returns a vector where each line is an uncrypted line from the file.
vector<string> unencrypted_contents(string filename, string keyword) {
	ifstream textfile;
	textfile.open(filename);
	vector<string> contents;
	string line;
	if (textfile.is_open()) {
		while (getline(textfile, line)) {
			line = unencrypt(line, keyword);
			contents.emplace_back(line);
		}
		textfile.close();
	}
	else
		contents.push_back("FAILED TO READ FILE " + filename);
	return contents;
}

void encrypt_and_overwrite(vector<string> contents, string filename, string keyword) {
	ofstream writefile;
	writefile.open(filename, ios::trunc);
	if (writefile.is_open()) {
		for (string printline : contents) {
			writefile << encrypt(printline, keyword) << endl;
		}
		writefile.close();
	}
}