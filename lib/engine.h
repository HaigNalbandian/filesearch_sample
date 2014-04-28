#ifndef ENGINE_H_
#define ENGINE_H_

#include <iostream>
#include "webpage.h"
#include "smartset.h"
#include <map>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

void convert_to_lowercase(string& s);
void initialize_map(map<string, WebPage*>&, map<string, SmartSet<WebPage*> >&);
set<WebPage*> standard_protocol(const map<string, SmartSet<WebPage*> >& m, string input);
set<WebPage*> other_protocol(const map<string, SmartSet<WebPage*> >& m, string input);


void parse_data(map<string, WebPage*>& all_pages, map<string, SmartSet<WebPage*> >& m, const char* master_filename)
{	
	//open the main data file
	ifstream master_directory (master_filename);
	
	//temporary string to hold the name of the file to plug into the webpage
	string filename;
	
	//for every webpage
	while (getline(master_directory, filename)){
	
		//stick in then extract from stream to remove newlines
		stringstream ss (filename);
		ss >> filename;
		
		//iterate through the webpages already created
		map<string, WebPage*>::iterator it = all_pages.find(filename);
		if (it == all_pages.end()){
			WebPage* p = new WebPage(filename);
			p->parse(all_pages);
			all_pages.insert(pair<string, WebPage*>(filename, p));
		}
		else{ //else, still parse it even though it shouldn't be created
			WebPage* p = it->second;
			p->parse(all_pages);
		}
	}
	master_directory.close();
	//CREATE MAP of words to webpages, INTIALIZE ALL OF THE PAIRS
	
	//match each word with a set of webpage pointers
	initialize_map(all_pages, m);
	
}
set<WebPage*> doSearch(string qt_query, map<string, SmartSet<WebPage*> >& m)
{
	convert_to_lowercase(qt_query);
	stringstream ss (qt_query);
	string input;
	ss >> input;
	convert_to_lowercase(input);

	set<WebPage*> results;
	
        if (input == "and" || input == "or"){
		results = other_protocol(m, qt_query);
	}
	else{
		results = standard_protocol(m, qt_query);
	}
	
	//delete data
/*	for (map<string, WebPage*>::iterator it=all_pages.begin();it!=all_pages.end();++it){
	  delete it->second;
	}*/
	
	return results;
}

void initialize_map(map<string, WebPage*>& all_pages, map<string, SmartSet<WebPage*> >& m)
{	
		SmartSet<string> all_words_on_every_page;

		for (map<string, WebPage*>::iterator it = all_pages.begin(); it != all_pages.end(); ++it){
			WebPage nextPage = *(it->second);
			all_words_on_every_page = all_words_on_every_page.setUnion(nextPage.allWords());
		}
 				
		//walk down each word, then add matching pages to a results set
		for (set<string>::iterator it = all_words_on_every_page.begin(); it != all_words_on_every_page.end(); ++it){
			string target = *it;
			SmartSet<WebPage*> results;

			//for every webpage
			for (map<string, WebPage*>::iterator it = all_pages.begin(); it != all_pages.end(); ++it){
				WebPage nextPage = *(it->second);
				set<string> all_w = nextPage.allWords();
				if (all_w.find(target) != all_w.end()){
					results.insert(it->second);
				}
			}
			m.insert( pair<string, SmartSet<WebPage*> >(target, results) );
		}
}

set<WebPage*> standard_protocol(const map<string, SmartSet<WebPage*> >& m, string input)
{
	set<WebPage*> results;
	
	//see if there's anything to be found
	map<string, SmartSet<WebPage*> >::const_iterator it = m.find(input);
	if (it == m.end()){
		return results;
	}
	else{
		results = it->second;	
	}
	return results;
	
}

set<WebPage*> other_protocol(const map<string, SmartSet<WebPage*> >& m, string input)
{
	set<WebPage*> results;
	SmartSet<string> set_of_keys;
	string buffer, word_buf, next_buffer = "";
	int comma_count = 0;
	
	stringstream ss (input);

	//store the operator
	string op;
	ss >> op;
	
	while (ss >> word_buf){
		//start at pos 0, walk up until something isn't alphanumeric, grab the substring from that first pos to second pos
		int idx = 0, len =-1;
		//move len up a notch as long as I'm alphanumeric
		while(static_cast<unsigned int>(idx-1) != word_buf.size()){
			//check for correct number of commas
			if (word_buf[len] == ','){ 
				comma_count++;
			}
			
			len++; //advance the "pointer"
			while (isalnum(word_buf[len])){
				len++;
			} 
			string val_word = word_buf.substr(idx, (len-idx));
			idx = len+1;
			if (val_word != ""){
				convert_to_lowercase(val_word);
				set_of_keys.insert(val_word);
			}
		} //end inner while
	} //end while
	
	//check that there are any keys to read
	int numKeys = set_of_keys.size();
	if (numKeys == 0){
		standard_protocol(m, input);
		return results;
	}
	
	if (comma_count < numKeys-1){
		cout << "Invalid input. Please separate queries with commas." << endl;
		return results;
	}

	//create results set
	SmartSet<WebPage*> all_results;
	for (set<string>::iterator it = set_of_keys.begin(); it != set_of_keys.end(); ++it){
		SmartSet<WebPage*> single_results;
		string current_key = *it;
		map<string, SmartSet<WebPage*> >::const_iterator jt = m.find(current_key);
		if (op == "and"){
			if (jt == m.end()){
			  //				cout << "0 results found." << endl;
				return results;
			}
			single_results = jt->second;
			if (it == set_of_keys.begin()){
				all_results = single_results;
			}
			//cout << "Number of single hits for " << current_key << " ... " << single_results.size() << endl;
			all_results = single_results.setIntersection(all_results);
		}
		else{
			if (jt == m.end()){
				continue;
			}
			single_results = jt->second;
			all_results = all_results.setUnion(single_results);
		}
	}
	results = all_results;
	return results;
}

#endif
