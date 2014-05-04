#ifndef WEBPAGE_H_
#define WEBPAGE_H_

#include <set>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "smartset.h"
#include <ctime>

using namespace std;

class WebPage {
  public:
    //CONSTRUCTORS
    WebPage ();
    WebPage (string filename);

    //DESTRUCTOR
    ~WebPage ();

    //OTHER PUBLIC FUNCTIONS
    set<string> allWords () const;
    friend ostream & operator<< (ostream & os, const WebPage & page);
    bool operator==(const WebPage& other) const;	
    void parse(map<string, WebPage*>& global_map); 	
    void addIncomingLink(WebPage* start);
    void addOutgoingLink(WebPage* target);
    
    //ACCESSOR FUNCTIONS
    string filename();	
    SmartSet<WebPage*> allIncomingLinks() {return incomingLinks;}
    SmartSet<WebPage*> allOutgoingLinks() {return outgoingLinks;}
    double get_pr();
    void set_pr(double pr);
    void set_start(clock_t begin);
    void set_finish(clock_t closing);
    void set_time_on_page();
    double get_time_fraction();

  private:
   //PRIVATE VARIABLES
    static double total_time;
    time_t start, finish;
   double time_fraction;
   double cumulative_time_on_page;
   set<string> wordBank;
   string _filename;
   SmartSet<WebPage*> incomingLinks;
   SmartSet<WebPage*> outgoingLinks;
   double page_rank;
};

#endif
