// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "mainwindow.h"

#include <QtWidgets>

mainWindow::mainWindow(QWidget* parent) : QWidget(parent) {

  dialog_button_widget = new QDialogButtonBox(QDialogButtonBox::Close);
  connect(dialog_button_widget, SIGNAL(rejected()), QApplication::instance(),
          SLOT(quit()));

  // activate top level layout
  QVBoxLayout* mainLayout = new QVBoxLayout;
  // mainLayout->addWidget(horizontalGroupBox);
  mainLayout->addStretch(1);
  mainLayout->addWidget(dialog_button_widget);
  setLayout(mainLayout);

  setWindowTitle("qt basic");
}
