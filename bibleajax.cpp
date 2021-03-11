/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

//Stuff for pipes                                                               
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "fifo.h"

#define logging // enable log file
#define LOG_FILENAME "/tmp/scowarton-project3-2.log"
#include "logfile.h"

string receive_pipe = "reply";
string send_pipe = "request";


int main() {
	/* A CGI program must send a response header with content type
	 * back to the web client before any other output.
	 * For an AJAX request, our response is not a complete HTML document,
	 * so the response type is just plain text to insert into the web page.
	 */
	cout << "Content-Type: text/plain\n\n";

	Cgicc cgi;  // create object used to access CGI request data

	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);

#ifdef logging
	logFile.open(logFilename.c_str(), ios::out);
#endif

	// GET THE INPUT DATA
	// browser sends us a string of field name/value pairs from HTML form
	// retrieve the value for each appropriate field name
	form_iterator st = cgi.getElement("search_type");
	form_iterator book = cgi.getElement("book");
	form_iterator chapter = cgi.getElement("chapter");
	form_iterator verse = cgi.getElement("verse");
	form_iterator nv = cgi.getElement("num_verse");
	int bookn, versen;
	bookn = book->getIntegerValue();
	versen = verse->getIntegerValue();
	int chapterNum = chapter->getIntegerValue();
	string chapterNumText = chapter->getValue();
	string verseNumText = verse->getValue();

	// Convert and check input data
	bool validInput = true;
	bool chapterValid = true;
	bool verseValid = true;
	bool blankChapter = false;
	bool blankVerse = false;
	int chapterV = 0;
	int verseV = 0;
	if (chapter != cgi.getElements().end()) {
		if (!(chapter->getValue().empty()) && !(verse->getValue().empty())) { //checks if empty
			if (chapterNum > 150) {
				chapterValid = false;
				validInput = false;
				chapterV = 1;
			}
			else if (chapterNum <= 0) {
				chapterValid = false;
				validInput = false;
				chapterV = -1;
			}
			if (versen <= 0)
			{
				verseValid = false;
				validInput = false;
				verseV = -1;
			}
		}
		else { // runs this if it is
			validInput = false;
			if (chapter->getValue().empty()) {
				blankChapter = true;
			}
			if (verse->getValue().empty()) {
				blankVerse = true;
			}
		}
	}
	/* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */

	/* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
	 *        TO LOOK UP THE REQUESTED VERSES
	 */
	 //Bible bible = Bible("/home/class/csc3004/Bibles/web-complete");
	LookupResult result = OTHER;
	Ref ref = Ref(bookn, chapterNum, versen);
	int n = nv->getIntegerValue();
	//Verse v;
	//Ref prev;

	/* SEND BACK THE RESULTS
	 * Finally we send the result back to the client on the standard output stream
	 * in HTML text format.
	 * This string will be inserted as is inside a container on the web page,
	 * so we must include HTML formatting commands to make things look presentable!
	 */
	int a, d, g;
	if (validInput) {
		cout << "Search Type: <b>" << **st << "</b>" << endl;
		cout << "<p>Your result: ";
		string message = "lookup " + to_string(bookn) + ":" + to_string(chapterNum) + ":" + to_string(versen);
		sendfifo.openwrite();
		log("Open Request Pipe");

		// Call server to get results                                                 
		sendfifo.send(message);
		log("Request: " + message);

		recfifo.openread();
		log("open reply fifo");

		int count = 0;

		while (count < n) {
			string reference = recfifo.recv();
			string t = GetNextToken(reference, " ");
			if (t != "SUCCESS") {
				if (t == "NO_VERSE") {
					result = NO_VERSE;
					break;
				}
				else if (t == "NO_CHAPTER") {
					result = NO_CHAPTER;
					break;
				}
			}
			else {
				result = SUCCESS;
			}
			if (result == SUCCESS) {
				cout << reference << endl;
				count++;
			}
			if (count < n) {
				message = "next " + to_string(bookn) + ":" + to_string(chapterNum) + ":" + to_string(versen);
				sendfifo.send(message);
				log("Request: " + message);
			}
		}
		/*for (int i = 0; i < n; i++) {
			if (i == 0) {
				v = bible.lookup(ref, result);
				a = ref.getBook();
				string bookName = ref.getBookName();
				d = ref.getChap();
				g = ref.getVerse();
				if (result != SUCCESS) {
					break;
				}
				cout << "<h2>" << bookName << " " << d << "</h2>";
					cout << "<p><em> ";
				v.display();
				prev = ref;
				cout << "</em></p>";
			}
			else {
				ref = bible.next(ref, result);
				a = ref.getBook();
				string bookName = ref.getBookName();
				d = ref.getChap();
				g = ref.getVerse();
				if (d != prev.getChap() || a != prev.getBook()) {
					cout << "<h2>" << bookName << " " << d << "</h2>";
				}
				cout << "<p><em> ";
				v = bible.lookup(ref, result);
				v.display();
				prev = ref;
				cout << "</em></p>";
			}
		}*/
		//The " << **nv
		//	 //<< " actual verse(s) retreived from the server should go here!</em></p>" << endl;
	}
	else if (!validInput && !chapterValid && verseValid) {
		if (chapterV == 1) {
			cout << "<p>Error: the chapter " << chapterNum << " is greater than the largest chapter in a book.</p>" << endl;
		}
		else {
			cout << "<p>Error: the chapter " << chapterNum << " is lower than the smallest chapter in a book.</p>" << endl;
		}
	}
	else if (!validInput && chapterValid && !verseValid) {
		cout << "<p>Error: the verse " << versen << " is lower than the smallest verse in a chapter.</p>" << endl;
	}
	else if (!validInput && !chapterValid && !verseValid) {
		if (chapterV == 1) {
			cout << "<p>Error: the chapter " << chapterNum << " is too large to be a chapter and the verse " << versen << " is less than the smallest possible verse.</p>" << endl;
		}
		else {
			cout << "<p>Error: the chapter " << chapterNum << " and the verse " << versen << " are both lower than the smallest possible values for chapters and verses.</p>" << endl;
		}
	}
	else if (!validInput && (blankChapter || blankVerse)) {
		if (blankChapter && !blankVerse) {
			cout << "<p>Error: the chapter input box has been left blank, please enter a value into the box to continue.</p>" << endl;
		}
		else if (!blankChapter && blankVerse) {
			cout << "<p>Error: the verse input box has been left blank, please enter a value into the box to continue.</p>" << endl;
		}
		else {
			cout << "<p>Error: the chapter and verse input boxes have been left blank, please enter values into the boxes to continue.</p>" << endl;
		}
	}
	else {
		cout << "<p>Error: An unexpected error has occurred, one that you shouldn't be seeing. Notify someone if you see this message.</p>" << endl;
	}
	if (result == NO_CHAPTER) {
		cout << "<p>Error: no such chapter " << ref.getChap() << " in " << ref.getBookName() << "</p>" << endl;
	}
	else if (result == NO_VERSE) {
		cout << "<p>Error: no such verse " << ref.getVerse() << " in " << ref.getBookName() << " chapter " << ref.getChap() << "</p>" << endl;
	}

	cout << endl; // flush output when done
	recfifo.fifoclose();
	log("close reply fifo");
	sendfifo.fifoclose();
	log("close request fifo");
	return 0;
}
