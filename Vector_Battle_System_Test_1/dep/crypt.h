#pragma once
#include <string>
#include <vector>
#include <fstream>

using namespace std;

/*The first character in this alphabet being the numeral '0' actually isn't great. You see, when
storing floats (in, say, graphical data) many of these floats will take on integral values. This
means (you guessed it) a LOT of zeroes appear in the decrypted text file. And in this type of ci
pher, the first character in the alphabet shows up as one of the characters in the cipher key. S
o if we use zeroes next to eachother a lot, the key begins to show up in the cryptogram, and then
they'll be onto us. Simple solution: move 0 to the end of the numerals in the cipher's base alpha
bet. Unfortunately this means that any graphics or cryptograms from before 1/22/19 at 10:45 PM wi
ll be rendered unreadable. If you really want to read some of those, use version of 'crypt.h' fro
m before then, or just move the 0 back to the beginning for a bit. I'm moving the 0 now. '1' will
be the new first numeral, which should not cause the same problem.*/
/*1/22/2019 10:56 PM: Actually, I gave it some more thought, and I see no reason that the alphabe
t should be ordered at all. It'll be WAY tougher to decrypt then! I bet there won't be a single
keyed vigenere cipher codebreaker program on the internet that could possible handle it then!*/
//Scrambling the alphabet now. Here's the old one: 
//"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz;:[]{}<>', .?/!@#$%^&*()-_=+|`~";
//Now for the new one, which includes the characters \ and " :
const string cipher_old_alphabet =
"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz;:[]{}<>', .?/!@#$%^&*()-_=+|`~";
const string cipher_base_alphabet = 
"+L~A0|J`F9Ir8?/O!t@E#u$NhiP7nQRS^(K)-_s1v[B]{M}<C>', .%wG&Hxyz;:=T*U6VWXY\\ZD5abcde4f\"gj3klmo2pq";


string alphabet_shift(char& new_firstletter, const string base_alphabet) {
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

int letter_index(char& letter, const string& alphabet) {
	unsigned int i = 0;
	while (i < alphabet.size()) {
		if (alphabet[i] == letter)
			return i;
		i++;
	}
	return 0;
}

string encrypt(const string& message, const string& keyword) {
	string cipher = "";
	int i = 0;
	if (keyword == "")
		return message;
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

string multi_encrypt(unsigned int depth,  string message, string keyword) {
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
