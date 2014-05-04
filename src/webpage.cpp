#include "webpage.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <stack>
#include <iostream>
#include <map>
#include <ctime>

using namespace std;
void convert_to_lowercase(string& s);

//CONSTRUCTORS
WebPage::WebPage()
{
  cumulative_time_on_page=0;
}

WebPage::WebPage (string filename) : _filename(filename)
{
  cumulative_time_on_page=0;
}

//DESTRUCTOR
WebPage::~WebPage(){
}

//PUBLIC FUNCTIONS
set<string> WebPage::allWords () const
{
	return wordBank;
}

void WebPage::parse(map<string, WebPage*>& global_map) {
	const char* c_string_filename = _filename.c_str();
	ifstream ifile(c_string_filename);
	string word = "";
	while (true){
		string anchor = "";
		string url = "";
		char c = ifile.get();

		//if I'm at the end of the file, break
		if (ifile.eof()) break;
		
		//is the character the start of a link?
		if (c == '['){
		  char link_char = ifile.get();
			while (link_char != ']'){
				if (isalnum(link_char)){
					anchor += link_char;
				}
				else{
					convert_to_lowercase(anchor);
					wordBank.insert(anchor);
					anchor = "";
				}
				link_char = ifile.get();
			}

			convert_to_lowercase(anchor);
			wordBank.insert(anchor);
			char url_char;
			link_char = ifile.get(); //grab the open paren
			while (true){
				url_char = ifile.get();
				if (url_char == ')'){
					break;
				}
				else{
					url += url_char;
				}
			} //end url while loop

		} //end link protocol
		else{
			if (isalnum(c)){
				word += c;
			}
			else {
				if (word != ""){
					convert_to_lowercase(word);
					wordBank.insert(word);
				}
				word = "";
			}
		}

		if (anchor != ""){
			//cout << "Anchor: " << anchor << endl << "URL: " << url << endl;
			WebPage* p;
			map<string, WebPage*>::iterator it = global_map.find(url);
			if (it == global_map.end()){
				p = new WebPage(url);
				global_map.insert(pair<string, WebPage*>(url, p));
			}
			else{
				p = it->second; 
			}
			this->addOutgoingLink(p);
			p->addIncomingLink(this);
		}

		

	}//end main while loop


}

string WebPage::filename()
{
	return this-> _filename;
}

void WebPage::addIncomingLink(WebPage* start)
{
	incomingLinks.insert(start);
}

void WebPage::addOutgoingLink(WebPage* target)
{
	outgoingLinks.insert(target);
}

bool WebPage::operator==(const WebPage& other) const{
	return (wordBank == other.wordBank);
}

double WebPage::get_pr()
{
  time_fraction = cumulative_time_on_page/total_time;
  return page_rank;
}

void WebPage::set_pr(double pr)
{
  page_rank = pr;
}

double WebPage::total_time=0.00000001;

void WebPage::set_start(time_t begin)
{
  start = begin;
} 

void WebPage::set_time_on_page()
{
  double time_on_page = (double)(finish-start);
  cumulative_time_on_page += time_on_page;
  
  //add this page's time to the total time
  WebPage::total_time+=time_on_page;
  time_fraction = cumulative_time_on_page/total_time;
 /* cout << "Hi, my name is " << filename() << " and I was just opened for " << time_on_page << " seconds." << endl << "That brings my total view time to " << cumulative_time_on_page << " seconds!" << endl;
  cout << "That's " << time_fraction*100 << " percent of the time spent browsing." << endl;*/
} 

void WebPage::set_finish(time_t closing)
{
  finish = closing;
  set_time_on_page();
}

double WebPage::get_time_fraction(){
	return time_fraction;
}

ostream& operator<<(ostream& os, const WebPage& page)
{
	const char* input_file = page._filename.c_str();
	ifstream ifile (input_file);
	string temp;
	while (getline(ifile, temp)){
		int found = temp.find('[');
		while (found > -1){
		  temp.erase(temp.begin()+found);
		  temp.erase(temp.begin()+temp.find(']'));
		  int found2 = temp.find('(');
		  temp.erase(found2, temp.find(')')-found2+1);
		  found = temp.find('[');
		}
		   
		os << temp << endl;
	}
	ifile.close();
	
	return os;
}

void convert_to_lowercase(string& s)
{
	for (unsigned int i=0; i<s.size(); i++){
		if (isupper(s[i])){
			s[i] = tolower(s[i]);
		}
	}
}

