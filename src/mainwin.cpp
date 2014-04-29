#include "mainwin.h"
#include <string>
#include <sstream>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRadioButton>
#include <QLineEdit>
#include <iostream>
#include "engine.h"
#include "msort.h"

using namespace std;

MainWin::MainWin (char* ifname, char* advert_input) : input_name(ifname)
{
	//PARSE DATA
	const char* filename = (this->input_name).c_str();
	parse_data(filename_to_page, input_to_results, filename);

	//CREATE BUTTONS
	aboutButton = new QPushButton("&About");
	quitButton = new QPushButton("&Quit");
	searchButton = new QPushButton("&Search");
	
	aboutButton->setFixedSize(75, 40);
	quitButton->setFixedSize(75, 40);

	//CREATE LINE EDIT
	queryText = new QLineEdit;

	//CREATE LABEL FOR ADS
	ad_label = new QLabel("Sponsored Ads");

	//CREATE LIST FOR ADS
	ad_list = new QListWidget;
	ad_list->setFixedHeight(60);

	//CREATE LIST FOR RESULTS
	list = new QListWidget;

	//CREATE RADIO BUTTONS FOR SEARCH
	alpha_radio_btn = new QRadioButton("Alphabetical");
	pr_radio_btn = new QRadioButton("Page Rank");
	pr_radio_btn ->setChecked(true);
	
	//CREATE LAYOUTS
	QFormLayout* fLayout = new QFormLayout;
	fLayout->addRow("Search:", queryText);

	QHBoxLayout* toprow = new QHBoxLayout;
	toprow->addLayout(fLayout);
	toprow->addWidget(searchButton);

	QHBoxLayout* radiorow = new QHBoxLayout;
	radiorow->addWidget(alpha_radio_btn);
	radiorow->addWidget(pr_radio_btn);
	
	QHBoxLayout* adrow = new QHBoxLayout;
	adrow->addWidget(ad_list);

	QHBoxLayout* midrow = new QHBoxLayout;
	midrow->addWidget(list);
	
	QHBoxLayout* botrow = new QHBoxLayout;
	botrow->addWidget(quitButton,1, Qt::AlignRight);
	botrow->addWidget(aboutButton,0, Qt::AlignRight);

	//ADD EVERYTHING TO THE MAIN LAYOUT
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(toprow);
	mainLayout->addLayout(radiorow);
	mainLayout->addLayout(midrow);
	mainLayout->addWidget(ad_label);
	mainLayout->addLayout(adrow);
	mainLayout->addLayout(botrow);

	//CONNECT BUTTONS
	QObject::connect(aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));
	QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchClicked()));
	QObject::connect(queryText, SIGNAL(returnPressed()), this, SLOT(searchClicked()));
	QObject::connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));

	//SET THE LAYOUT
	setLayout(mainLayout);

	//GIVE VIEW WINDOW ACCESS TO MAP CONVERTING FILENAMES TO MEM ADDRESSES OF WEBPAGES
	view = new ViewWin;
	view->grab_map(filename_to_page);

	//READ IN THE ADVERTISER FILE
	parseAdvertisers(advert_input);
}

MainWin::~MainWin()
{
	delete aboutButton;
	delete quitButton;
	delete searchButton;
	delete list;
	delete view;
	delete queryText;
}

void MainWin::showAbout()
{
	QMessageBox::information(this, tr("About"), tr("Created by Eshed Margalit, 2014"));
}

void MainWin::searchClicked(){
	list->clear();
	QString query = queryText->text();
	string my_str = query.toStdString();
	set<WebPage*> results =	doSearch(my_str, input_to_results);
	
	SmartSet<WebPage*> expandedSet;
    for (SmartSet<WebPage*>::iterator it=results.begin(); it != results.end(); ++it){

      expandedSet.insert(*it);

      //add all incoming links
      SmartSet<WebPage*> new_incoming = (*it)->allIncomingLinks();
      for (SmartSet<WebPage*>::iterator jt = new_incoming.begin(); jt != new_incoming.end(); ++jt){
        expandedSet.insert(*jt);
      }
      //add all outgoing links
      SmartSet<WebPage*> new_outgoing = (*it)->allOutgoingLinks();
      for (SmartSet<WebPage*>::iterator jt = new_outgoing.begin(); jt != new_outgoing.end(); ++jt){
        expandedSet.insert(*jt);
      }
    }
    if (pr_radio_btn->isChecked()){
      rank_pages(expandedSet);
    }

    vector<WebPage*> final_results;
    for (SmartSet<WebPage*>::iterator it = expandedSet.begin(); it != expandedSet.end(); ++it){
      final_results.push_back(*it);
    }
	
    if (pr_radio_btn->isChecked()){
      PRComp comp;
      mergeSort<WebPage*, PRComp>(final_results, comp);
    }
    else{
      AlphComp comp;
      mergeSort<WebPage*, AlphComp>(final_results, comp);
    }
    for (vector<WebPage*>::iterator it = final_results.begin(); it!= final_results.end(); ++it){
      	string filename = (*(*it)).filename();
       	QString qt_filename(filename.c_str());
       	list->addItem(qt_filename);
    }

	//NOW DO ADVERTISER SEARCH
	vector<Advertiser*> matching_advertisers;
	search_advertisers(matching_advertisers, my_str);
}

void MainWin::itemClicked(QListWidgetItem* item)
{
	view->clearWin();
	QString s = item->text();
	string str = s.toStdString();
	
	WebPage* page_ptr = (filename_to_page.find(str))->second;

	view->setWindowTitle(s);
	view->show();
	view->populate(page_ptr);
}

void MainWin::rank_pages(SmartSet<WebPage*>& s)
{
	//first set all page ranks back to 1/s.size(); create a set of sink nodes
	double num_pages = s.size();
	double starting_pr = 1/num_pages;
	SmartSet<WebPage*> noOutgoing;

	for (SmartSet<WebPage*>::iterator it = s.begin(); it != s.end(); ++it)
	{
		(*it)->set_pr(starting_pr);
		
	        SmartSet<WebPage*> outLinks = (*it)->allOutgoingLinks();
		if( outLinks.empty() || outLinks.size() != s.setIntersection(outLinks).size())
		{
			noOutgoing.insert(*it); //find pages with no outgoing edges, add them to set
		}
	}

	//repeat 30 times-- do the actual page ranking
	for (int iteration = 0; iteration <30; iteration++){
		vector<double> sums;
		
		//for every page in the graph
		for (SmartSet<WebPage*>::iterator it = s.begin(); it != s.end(); ++it)
		{
			double sum = 0;
			SmartSet<WebPage*> i_links = (*it)->allIncomingLinks();
			for (SmartSet<WebPage*>::iterator jt = i_links.begin(); jt != i_links.end(); ++jt)            //this is as before
		        {
				//if the incoming link is not in the webgraph, ignore it
				if (s.find(*jt) == s.end()){
					continue;
				}
	
				
				WebPage curr = *(*jt);
				double my_pr = curr.get_pr();
				
				//find overlap of ourgoing links and webgraph to determine degree
				SmartSet<WebPage*> overlap = s.setIntersection(curr.allOutgoingLinks());
				double num_outgoing = overlap.size();
				sum += (my_pr/num_outgoing);
			}
                        for (SmartSet<WebPage*>::iterator iter = noOutgoing.begin(); iter != noOutgoing.end(); ++iter) //add page rank from pages without outgoing edges
			{
				WebPage curr = *(*iter);
				double my_pr = curr.get_pr();
				sum += (my_pr/num_pages);
			}
		
			sum *= 0.85;
			sum += 0.15/num_pages;
			sums.push_back(sum); //take the sum and add it to a vector so that we do not change existing pagerank values while system is still in iteration

		} //end webpage for
		for (SmartSet<WebPage*>::iterator it = s.begin(); it != s.end(); ++it) //now change values all together in separate loop
		{
			(*it)->set_pr(sums.front());
			sums.erase(sums.begin());
		}
	}// end iterative for

	/*//print out page ranks to console
	cout << "Page Ranks for Most Recent Search" << endl << "---------------------------" << endl;
	for (SmartSet<WebPage*>::iterator it = s.begin(); it != s.end(); ++it){
		cout << (*it)->filename() << " " << (*it)->get_pr() << endl;
	}*/

		
}

void MainWin::parseAdvertisers(char* input)
{
	ifstream ifile (input);
	string temp;

	//read in number of advertisers
	getline(ifile, temp);
	int num_advertisers;
	stringstream ss (temp);
	ss >> num_advertisers;

	for (int i=0; i< num_advertisers; ++i){
		getline(ifile, temp);
		stringstream temp_stream (temp);
		string keyword;
		double bid;
		string temp_name, full_name= "";
		temp_stream >> keyword;
		convert_to_lowercase(keyword);
		temp_stream >> bid;
		temp_stream >> full_name;
		while (temp_stream >> temp_name){
			full_name += (" " + temp_name);
		}
		unordered_map<string, Advertiser*>::iterator it = advertisers.find(full_name);
		if (it != advertisers.end()){
			Advertiser* ptr_to_advertiser = it->second;
			ptr_to_advertiser->add_ad(keyword, bid);	
		}
		else{
		//add name to hash table
			Advertiser* ptr_to_advertiser = new Advertiser(full_name);
			advertisers.emplace(full_name, ptr_to_advertiser);
			ptr_to_advertiser->add_ad(keyword, bid);
			advertisers_vec.push_back(ptr_to_advertiser);
		}
	}

/*	for (unordered_map<string, Advertiser*>::iterator it = advertisers.begin(); it != advertisers.end(); ++it){
	cout << "Name: " << it->second->getName() << endl;
	vector<Ad> vec;
	vec = it->second->getAds();
	for (vector<Ad>::iterator jt = vec.begin(); jt != vec.end(); ++jt){
		cout << jt->keyword << " " << jt->bid << endl;
	}
	cout << endl;

	for (vector<Advertiser*>::iterator it = advertisers_vec.begin(); it != advertisers_vec.end(); ++it){
		cout << (*it)->getName() << endl;
	}
	}*/

	ifile.close();
}

void MainWin::search_advertisers(vector<Advertiser*>& vec, string query)
{
	convert_to_lowercase(query);
	//STEP (1), create a set of keywords
	set<string> keywords;
	if (query[0] == 'o' && query[1] == 'r' && query[3] == '('){
		query.erase(query.begin());	
		query.erase(query.begin());	
		query.erase(query.begin());
		query.erase(query.begin());
		query.erase(query.end()-1);	
	}
	string curr_word = "";
	for (unsigned int i=0; i< query.size(); ++i){
		if (isalnum(query[i])){
			curr_word+=query[i];
		}
		else{
			if (curr_word != ""){
				keywords.insert(curr_word);
				curr_word = "";
			}
		}
	}
	if (curr_word != ""){
		keywords.insert(curr_word);	
	}
	
	//STEP (2), iterate over keywords, looking for them in advertisers bids
	for (set<string>::iterator st = keywords.begin(); st != keywords.end(); ++st){
		string curr_key = *st;
		for (vector<Advertiser*>::iterator it = advertisers_vec.begin(); it != advertisers_vec.end(); ++it){
			Advertiser* curr_advertiser = *it;
			curr_advertiser->setHighestBid(-1);
			for (vector<Ad>::iterator at = (curr_advertiser->getAds()).begin(); at != (curr_advertiser->getAds()).end(); ++at){
				string ad_key = at->keyword;
				double ad_bid = at->bid;
				if (ad_key == curr_key){
					vec.push_back(curr_advertiser);
					curr_advertiser->setHighestBid(ad_bid);
				}
			}
		}
	} 	
}
