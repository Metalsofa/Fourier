#include "crypt.h"

string alphabetShift(char& newFirstletter, const string baseAlphabet) {
	string newAlphabet = baseAlphabet;
	char infinityPreventer = baseAlphabet[0];
	bool firsttry = true;
	while (newAlphabet[0] != newFirstletter) {
		if (newAlphabet[0] == infinityPreventer && !firsttry) {
			break;
		}
		if (newAlphabet[0] == infinityPreventer) {
			firsttry = false;
		}
		newAlphabet = newAlphabet + newAlphabet[0];
		newAlphabet.erase(newAlphabet.begin());
	}
	return newAlphabet;
}

int letterIndex(char& letter, const string& alphabet) {
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
		int letterindex = letterIndex(c, cipherBaseAlphabet);
		int keyletternum = int(i % keyword.size());
		char keyletter = keyword[keyletternum];
		string shiftebet = alphabetShift(keyletter, cipherBaseAlphabet);
		char newletter = shiftebet[letterindex];
		cipher = cipher + newletter;
		i++;
	}
	return cipher;
}

string multiEncrypt(unsigned int depth, string message, string keyword) {
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
		string shiftebet = alphabetShift(keyletter, cipherBaseAlphabet);
		int letterindex = letterIndex(c, shiftebet);
		char newletter = cipherBaseAlphabet[letterindex];
		message = message + newletter;
		i++;
	}
	return message;
}

string multiUnencrypt(unsigned int depth, string cipher, string key) {
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
vector<string> unencryptedContents(string filename, string keyword) {
	ifstream textfile;
	textfile.open(filename);
	if (!textfile.good()) {
		cerr << "ERROR | crypt.h | Could not open file | " << filename << endl;
	}
	vector<string> contents;
	string line;
	if (textfile.is_open()) {
		while (getline(textfile, line)) {
			line = unencrypt(line, keyword);
			contents.emplace_back(line);
		}
		textfile.close();
	} else
		contents.push_back("FAILED TO READ FILE " + filename);
	return contents;
}

void encryptAndOverwrite(vector<string>& contents, string& filename, string keyword) {
	ofstream writefile;
	writefile.open(filename, ios::trunc);
	if (writefile.is_open()) {
		for (string printline : contents) {
			writefile << encrypt(printline, keyword) << endl;
		}
		writefile.close();
	}
}