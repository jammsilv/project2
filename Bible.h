// Class Bible
// Computer Science, MVNU
//
// A Bible object represents a particular version of the Bible
// A Bible object is constructed by giving it a file reference containing 
// the entire text of the version.

#ifndef Bible_H
#define Bible_H

#include "Ref.h"
#include "Verse.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// status codes to be returned when looking up a reference
enum LookupResult { SUCCESS, NO_BOOK, NO_CHAPTER, NO_VERSE, OTHER };

class Bible {	// A class to represent a version of the bible
 private:
   string infile;		// file path name
   ifstream instream;	// input stream, used when file is open
   bool isOpen;			// true if file is open
   // OPTIONAL: you may add variables to keep track of
   // the current line and/or reference when scanning the file

 public:
   Bible();	// Default constructor
   Bible(const string s); // Constructor – pass name of bible file
   
   // REQUIRED: Find and return a verse in this Bible, given a reference
   const Verse lookup(Ref ref, LookupResult& status); 
   // REQUIRED: Return the reference after the given ref
   const Ref next(const Ref ref, LookupResult& status);
   // OPTIONAL: Return the reference before the given ref
   const Ref prev(const Ref ref, LookupResult& status);
   
   // Information functions (REQUIRED)
   // Return an error message string to describe status
   const string error(LookupResult status);
   // Show the name of the bible file on cout
   void display();
};
#endif //Bible_H
