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
		outgoing->addItem(s);	
	}

	for (SmartSet<WebPage*>::iterator it = incomingLinks.begin(); it!=incomingLinks.end(); ++it){
		string my_str = (*it)->filename();
		QString s = my_str.c_str();
		incoming->addItem(s);	
	}
}

void ViewWin::linkClicked(QListWidgetItem* item)
{
	QString s = item->text();
	string str = s.toStdString();
	WebPage* page_ptr = (my_map.find(str))->second;
	clearWin();
	setWindowTitle(s);
	show();
	populate(page_ptr);

}

void ViewWin::grab_map(map<string, WebPage*> m){
	my_map = m;
}

