// let emacs know this is a C++ header: -*- C++ -*-
// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

// forward declarations, included in .cpp from <QtWidgets>
class QDialogButtonBox;
class QGroupBox;

class mainWindow : public QWidget {
  Q_OBJECT
public:
  explicit mainWindow(QWidget* parent = 0);

private:
  QDialogButtonBox* dialog_button_widget;
};

#endif  // MAINWINDOW_H
