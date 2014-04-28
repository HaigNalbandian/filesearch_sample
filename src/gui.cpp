#include <QApplication>
#include "mainwin.h"
#include "viewwin.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 3){
    cout << "Usage: ./qt [inputfile] [ad_input file]" << endl;
    return 1;
  }

  QApplication app(argc, argv);  
    
  MainWin window(argv[1], argv[2]);
  
  window.setWindowTitle("Eshed's Fabulous Search Engine");
  window.show();

  return app.exec();
}

