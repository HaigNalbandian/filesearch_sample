#include <QApplication>
#include "mainwin.h"
#include "viewwin.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 2){
    cout << "Usage: ./qt [inputfile]" << endl;
    return 0;
  }

  QApplication app(argc, argv);  
    
  MainWin window(argv[1]);
  
  window.setWindowTitle("Eshed's Fabulous Search Engine");
  window.show();

  return app.exec();
}

