// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; }

// REQUIRED: lookup finds a given verse in this Bible
const Verse Bible::lookup(Ref ref, LookupResult& status) { 
	ifstream in;
	string s;
	bool bookFound = false;
	bool chapterFound = false;
	in.open(infile);
	if (in.is_open()) {
		while (getline(in, s)) {
			Ref r = Ref(s);
			if (r.getBook() == ref.getBook()) {
				bookFound = true;
				if (r.getChap() == ref.getChap()) {
					chapterFound = true;
					if (r.getVerse() == ref.getVerse()) {
						in.close();
						status = SUCCESS;
						Verse v = Verse(s);
						return v;
					}
				}
			}
		}
		in.close();
	}
	if (!bookFound && !chapterFound) {
		status = NO_BOOK;
	}
	if (bookFound && !chapterFound) {
		status = NO_CHAPTER;
	}
	if (bookFound && chapterFound) {
		status = NO_VERSE;
	}
    // TODO: scan the file to retrieve the line with ref ...
    // update the status variable
	// create and return the verse object
	Verse aVerse;
    return(aVerse);
}
//make helper to take over infile stuff


// REQUIRED: Return the reference after the given ref
const Ref Bible::next(const Ref ref, LookupResult& status) {
	ifstream in;
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
	return(aRef);
};

// OPTIONAL: Return the reference before the given ref
const Ref prev(const Ref ref, LookupResult& status) {};

// Return an error message string to describe status
const string Bible::error(LookupResult status) {};

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
