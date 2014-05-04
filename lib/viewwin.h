#ifndef VIEW_WIN_H_
#define VIEW_WIN_H_

#include <QWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <string>
#include <map>
#include "webpage.h"

using namespace std;

class ViewWin : public QWidget
{
	Q_OBJECT

	public:
		ViewWin(QWidget* parent = NULL);
		void clearWin();
		void populate(WebPage*);
		void grab_map(map<string, WebPage*>);
		void grab_visited_pages(set<WebPage*>*);
		void closeEvent(QCloseEvent*);
		~ViewWin();
	private slots:
		void linkClicked(QListWidgetItem*);
			void writeOutTime();

	private:
		QTextEdit* display;
		QListWidget* incoming;
		QListWidget* outgoing;
		QLabel* incomingLabel;
		QLabel* outgoingLabel;
		map<string, WebPage*> my_map;

		//deal with visited pages
		set<WebPage*>* visited_pages;
};

#endif
