// let emacs know this is a C++ header: -*- C++ -*-
// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#ifndef W_SUDOKU_VIEW_H
#define W_SUDOKU_VIEW_H

#include "PlainTextEditIODevice.h"
#include "sudoku_class.h"
#include "sudoku_solve.h"
#include "w_sudoku.h"

#include <QTextStream>
#include <QWidget>

// forward declarations, included in .cpp from <QtWidgets>
class QDialogButtonBox;
class QPlainTextEdit;
class QGroupBox;
class QLabel;
class QPushButton;

class w_Sudoku_view : public QWidget {
  Q_OBJECT
public:
  explicit w_Sudoku_view(Sudoku& t_s, QWidget* parent = 0);

private:
  void createTextConsoleWidget();
  void createStatusWidget();
  void createSolverWidget();
  void createOptionsWidget();

  void update_status_values();
  void update_number_solution_pattern_values();

  void update_active_widgets();

signals:
  void text_msg(QString msg);  // send messages intended for text console

private slots:
  void remove_naked_singles();
  void remove_hidden_singles();
  void remove_naked_twins();
  void remove_hidden_twins();
  void remove_naked_triples();
  void remove_hidden_triples();
  void remove_naked_quadruples();

private:
  Sudoku& s;

  w_Sudoku* sudoku_widget;

  QPlainTextEdit* textconsole_widget;
  PlainTextEditIODevice* textDevice;

  // status labels and widget
  QLabel* valid_value;
  QLabel* num_entry_value;
  QLabel* num_candidates_value;
  QGroupBox* status_widget;

  // solver labels, push buttons and widget
  QLabel* num_naked_singles;
  QLabel* num_hidden_singles;
  QLabel* num_naked_twins;
  QLabel* num_hidden_twins;
  QLabel* num_naked_triples;
  QLabel* num_hidden_triples;
  QLabel* num_naked_quadruples;
  QPushButton* remove_naked_singles_button;
  QPushButton* remove_hidden_singles_button;
  QPushButton* remove_naked_twins_button;
  QPushButton* remove_hidden_twins_button;
  QPushButton* remove_naked_triples_button;
  QPushButton* remove_hidden_triples_button;
  QPushButton* remove_naked_quadruples_button;
  QGroupBox* solver_widget;

  QGroupBox* options_widget;

  QGroupBox* w1;  // sudoku_w + textconsole_widget (vertical)
  QGroupBox* w2;  // status + solver + options widgets (vertical)
  QGroupBox* w3;  // w1 + w2 (horizontal)

  QDialogButtonBox* dialog_button_widget;
};

#endif  // W_SUDOKU_VIEW_H
