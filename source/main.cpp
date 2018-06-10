// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);  

  
  mainWindow window;

  window.move(700, 100);
  
  // show widget
  window.show();

  return app.exec();
}
