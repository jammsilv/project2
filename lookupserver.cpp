
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include "Ref.h"
#include "fifo.h"
#include "Bible.h"
#include "Verse.h"

string receive_pipe = "request";
string send_pipe = "reply";

int main() {
	Bible bible = Bible();
	string message = "";
	string line;
	LookupResult status = OTHER;
	string reference;
	ifstream textFile;
	string fileName = bible.getInfile();

	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);

	//textFile.open(fileName);
	//if (textFile.bad()) {
	//	cout << "File: " << fileName << " cannot be opened" << endl;
	//	exit(1);
	//}
	recfifo.openread();
	Ref prev;

	cout << "Ready to recieve requests" << endl;
	while (1) {
		reference = recfifo.recv();
		string t = GetNextToken(reference, " ");
		string prefix = t;
		Ref ref = Ref(reference);
		cout << "Message: | " << reference << endl;
		sendfifo.openwrite();
		if (prefix == "lookup") {
			Verse v = bible.lookup(ref, status);
			if (status == SUCCESS) {
				prev = ref;
				message = "SUCCESS ";
				message += "<h2>" + ref.getBookName() + " " + to_string(ref.getChap()) + "</h2>";
				message += "<p><em>" + to_string(ref.getVerse()) + " " + v.getVerse() + "</em></p>";
				/*if (times > 1) {
					for (int i = 1; i < times; i++) {
						ref = bible.next(ref, status);
						if ((ref.getChap() != prev.getChap()) || (ref.getBook() != prev.getBook())) {
							message += "<h2>" + ref.getBookName() + " " + to_string(ref.getChap()) + "</h2>";
						}
						v = bible.lookup(ref, status);
						message += "<p><em>" + to_string(ref.getVerse()) + " " + v.getVerse() + "</em></p>";
					}
				}*/
				sendfifo.send(message);
			}
			else {
				switch (status) {
				case NO_VERSE:
					message = "NO_VERSE ";
					break;
				case NO_CHAPTER:
					message = "NO_CHAPTER ";
					break;
				}
				sendfifo.send(message);
			}
		}
		else if (prefix == "next") {
			ref = bible.next(prev, status);
			if (status != OTHER) {
				Verse v = bible.lookup(ref, status);
				if (status == SUCCESS) {
					message = "0 ";
					if ((ref.getChap() != prev.getChap()) || (ref.getBook() != prev.getBook())) {
						message += "<h2>" + ref.getBookName() + " " + to_string(ref.getChap()) + "</h2>";
					}
					message += "<p><em>" + to_string(ref.getVerse()) + " " + v.getVerse() + "</em></p>";
					prev = ref;
					sendfifo.send(message);
				}
				else {
					switch (status) {
					case NO_VERSE:
						message = "NO_VERSE ";
						break;
					case NO_CHAPTER:
						message = "NO_CHAPTER ";
						break;
					}
					sendfifo.send(message);
				}
			}
		}
		else if (prefix == "prev") {

		}
	}
	message = "$end";
	sendfifo.send(message);
	recfifo.fifoclose();
	sendfifo.fifoclose();
}

