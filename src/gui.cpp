#include <QApplication>
#include "mainwin.h"
#include "viewwin.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 4){
    cout << "Usage: ./qt [inputfile] [ad_input file] [bill_output]" << endl;
    return 1;
  }

  QApplication app(argc, argv);  
    
  MainWin window(argv[1], argv[2], argv[3]);
  
  window.setWindowTitle("CS104 Team 123 Search Engine");
  window.show();

  return app.exec();
}

