// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	buildTextIndex();
}

// Constructor – pass bible filename
Bible::Bible(const string s) { 
	infile = s; 
	buildTextIndex();
}

// REQUIRED: lookup finds a given verse in this Bible
const Verse Bible::lookup(Ref ref, LookupResult& status) { 
	ifstream instream;
	Verse none;
	if (!index.count(ref)) {
		Ref q = Ref(ref.getBook(), 1, 1);
		if (index.count(q)) {
			Ref r = Ref(ref.getBook(), ref.getChap(), 1);
			if (index.count(r)) {
				status = NO_VERSE;
			}
			else {
				status = NO_CHAPTER;
			}
		}
		else {
			status = NO_BOOK;
		}
	}
	else {
		string s; 
		instream.open(infile);
		instream.seekg(index[ref]);
		getline(instream, s);
		Verse v = Verse(s);
		instream.close();
		status = SUCCESS;
		return v;
	}
	//ifstream in;
	//string s;
	//bool bookFound = false;
	//bool chapterFound = false;
	//in.open(infile);
	//if (in.is_open()) {
	//	while (getline(in, s)) {
	//		Ref r = Ref(s);
	//		if (r.getBook() == ref.getBook()) {
	//			bookFound = true;
	//			if (r.getChap() == ref.getChap()) {
	//				chapterFound = true;
	//				if (r.getVerse() == ref.getVerse()) {
	//					in.close();
	//					status = SUCCESS;
	//					Verse v = Verse(s);
	//					return v;
	//				}
	//			}
	//		}
	//	}
	//	in.close();
	//}
	//if (!bookFound && !chapterFound) {
	//	status = NO_BOOK;
	//}
	//if (bookFound && !chapterFound) {
	//	status = NO_CHAPTER;
	//}
	//if (bookFound && chapterFound) {
	//	status = NO_VERSE;
	//}
    // TODO: scan the file to retrieve the line with ref ...
    // update the status variable
	// create and return the verse object
    return none;
}
//make helper to take over infile stuff

void Bible::buildTextIndex() {
	ifstream instream;
	int position, refcount;
	string line;
	instream.open(infile);
	if (!instream) {
		cerr << "Error - can't open input file: " << infile << endl;
	}
	else {
		while (!instream.fail()) {
			position = instream.tellg();
			getline(instream, line);
			if (!instream.fail()) {
				Ref r = Ref(line);
				refcount++;
				index[r] = position;
			}
		}
		cout << "References: " << refcount << " Unique: " << index.size() << " Byte Offset: " << position << endl;
		cout << endl;
	}
}


// REQUIRED: Return the reference after the given ref
const Ref Bible::next(const Ref ref, LookupResult& status) {
	map<Ref, int> ::iterator it;
	Ref r;
	it = index.find(ref);
	if (it == index.end()) {
		status = OTHER;
		return r;
	}
	else {
		it++;
		if (it != index.end()) {
			r = it->first;
			status = SUCCESS;
			return r;
		}
		else {
			status = OTHER;
			return r;
		}
	}
	/*ifstream in;
	string s;
	in.open(infile);
	if (in.is_open()) {
		while (getline(in, s)) {
			Ref r = Ref(s);
			if (r == ref) {
				getline(in, s);
				in.close();
				status = SUCCESS;
				Ref p = Ref(s);
				return p;
			}
		}
		in.close();
	}
	status = NO_BOOK;
	Ref aRef;
	return(aRef);*/
};

// OPTIONAL: Return the reference before the given ref
const Ref prev(const Ref ref, LookupResult& status) {};

const string Bible::getInfile() {
	return infile;
}

// Return an error message string to describe status
const string Bible::error(LookupResult status) {};

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
