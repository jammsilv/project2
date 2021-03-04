// testreader.cpp
// main function for Project 3 Part 1

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

main (int argc, char **argv) {
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	
	Verse verse;
	int b, c, v, n;
	LookupResult result;
	/*cout << "Using Bible from: ";
	webBible.display();*/
	// Prompt for input: get reference to lookup
	// This is good for testing your classes quickly for the first milestone.
	// TODO: your final program should get input from command line arguments instead.
	/*cout << "Enter 3 integers for book, chapter and verse to find: " << flush;
	cin >> b >> c >> v;	*/
	
	switch (argc) {
	case 1:
		cout << "Error: There is no book, chapter, or verse." << endl;
		return EXIT_FAILURE;
	case 2:
		cout << "Error: There is no chapter or verse." << endl;
		return EXIT_FAILURE;
	case 3:
		cout << "Error: There is no verse." << endl;
		return EXIT_FAILURE;
	case 4:
		b = atoi(argv[1]);
		c = atoi(argv[2]);
		v = atoi(argv[3]);
		n = 1;
		break;
	case 5:
		b = atoi(argv[1]);
		c = atoi(argv[2]);
		v = atoi(argv[3]);
		n = atoi(argv[4]);
		break;
	default:
		cout << "Error: There are more arguments than the amount required." << endl;
		return EXIT_FAILURE;
	}
	// Create a reference from the numbers
	Ref ref(b, c, v);

	// Use the Bible object to retrieve the verse by reference
	//cout << "Looking up reference: ";
	//ref.display();
	//cout << endl;

	Ref prev = Ref();
		for (int i = 0; i < n; i++) {
			if (i == 0) {
				verse = webBible.lookup(ref, result);
				//cout << "Result status: " << result << endl;
				if (result == NO_BOOK) {
					cout << "Error: no such book " << ref.getBook() << endl;
					return EXIT_FAILURE;
				}
				else if (result == NO_CHAPTER) {
					cout << "Error: no such chapter " << ref.getChap() << " in " << ref.getBookName() << endl;
					return EXIT_FAILURE;
				}
				else if (result == NO_VERSE) {
					cout << "Error: no such verse " << ref.getVerse() << " in " << ref.getBookName() << " chapter " << ref.getChap() << endl;
					return EXIT_FAILURE;
				}
				ref.display();
				verse.display();
				prev = ref;
			}
			else {
				ref = webBible.next(ref, result);
				if (result == OTHER) {
					cout << "Reached last verse while searching." << endl;
					return EXIT_SUCCESS;
				}
				if (prev.getChap() != ref.getChap()) {
					ref.display();
				}
				verse = webBible.lookup(ref, result);
				verse.display();
				prev = ref;
			}
		}
	cout << endl;
}
