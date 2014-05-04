#include "viewwin.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>
#include <sstream>

using namespace std;

ViewWin::ViewWin(QWidget *parent) : QWidget(parent)
{
	display = new QTextEdit;
	incoming = new QListWidget;
	outgoing = new QListWidget;
	incomingLabel= new QLabel("Incoming");
	outgoingLabel= new QLabel("Outgoing");

	QVBoxLayout* left_layout = new QVBoxLayout;
	left_layout->addWidget(display);

	display->setReadOnly(true);

	QVBoxLayout* top_half = new QVBoxLayout;
	QVBoxLayout* bottom_half = new QVBoxLayout;
	top_half->addWidget(incomingLabel);
	top_half->addWidget(incoming);
	bottom_half->addWidget(outgoingLabel);
	bottom_half->addWidget(outgoing);

	QVBoxLayout* right_layout = new QVBoxLayout;
	right_layout->addLayout(top_half);
	right_layout->addLayout(bottom_half);

	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addLayout(left_layout, 7);
	mainLayout->addLayout(right_layout, 2);


	QObject::connect(outgoing, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(linkClicked(QListWidgetItem*)));
	QObject::connect(incoming, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(linkClicked(QListWidgetItem*)));

	setLayout(mainLayout);
}

ViewWin::~ViewWin()
{
	delete display;
	delete incoming;
	delete outgoing;
	delete incomingLabel;
	delete outgoingLabel;
}

void ViewWin::clearWin(){
	display->clear();
	incoming->clear();
	outgoing->clear();
}

void ViewWin::populate(WebPage* page){
	stringstream ss;
	ss << *page;
	string page_string = ss.str();
	QString Qpage_string = page_string.c_str();

	display->setText(Qpage_string);

	SmartSet<WebPage*> outgoingLinks = page->allOutgoingLinks();
	SmartSet<WebPage*> incomingLinks = page->allIncomingLinks();

	for (SmartSet<WebPage*>::iterator it = outgoingLinks.begin(); it!=outgoingLinks.end(); ++it){
		string my_str = (*it)->filename();
		QString s = my_str.c_str();
		QListWidgetItem* new_item = new QListWidgetItem(s);
		if (visited_pages->find(*it) != visited_pages->end()){
			QColor color(187, 190, 191, 255);
			new_item->setForeground(color);
		}
		outgoing->addItem(new_item);	
	}

	for (SmartSet<WebPage*>::iterator it = incomingLinks.begin(); it!=incomingLinks.end(); ++it){
		string my_str = (*it)->filename();
		QString s = my_str.c_str();
		QListWidgetItem* new_item = new QListWidgetItem(s);
		if (visited_pages->find(*it) != visited_pages->end()){
			QColor color(187, 190, 191, 255);
			new_item->setForeground(color);
		}
		incoming->addItem(new_item);	
	}
}

void ViewWin::linkClicked(QListWidgetItem* item)
{
	QString window_title = this->windowTitle();
	string win_title = window_title.toStdString();
	WebPage* old_page = my_map.find(win_title)->second;
	old_page->set_finish(clock());

	QString s = item->text();
	string str = s.toStdString();
	WebPage* page_ptr = (my_map.find(str))->second;
	page_ptr->set_start(clock());
	visited_pages->insert(page_ptr);

	clearWin();
	setWindowTitle(s);
	show();
	populate(page_ptr);

}

void ViewWin::grab_map(map<string, WebPage*> m){
	my_map = m;
}

void ViewWin::grab_visited_pages(set<WebPage*>* s){
	visited_pages = s;
}
