// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "w_sudoku_view.h"

#include "sudoku_solve.h"

#include <QtWidgets>

w_Sudoku_view::w_Sudoku_view(Sudoku& t_s, QWidget* parent) : QWidget(parent), s(t_s) {

  setWindowTitle("Sudoku View");

  createTextConsoleWidget();  // needed before cells are created to connect signals
  sudoku_widget = new w_Sudoku(s, textconsole_widget, this);

  dialog_button_widget = new QDialogButtonBox(QDialogButtonBox::Close);

  // these widgets refer to sudoku_widget and thus must be initialized after sudoku_widget

  // status_widget = new w_Sudoku_status(s);
  createStatusWidget();

  createSolverWidget();
  createOptionsWidget();

  // level1 layout
  w1 = new QGroupBox;
  w1->setFlat(true);

  QVBoxLayout* layout1 = new QVBoxLayout;
  layout1->addWidget(sudoku_widget);
  // layout->addStretch(1);
  layout1->addWidget(textconsole_widget);
  w1->setLayout(layout1);

  // level2 layout
  w2 = new QGroupBox;
  // w2->setFlat(true);
  QVBoxLayout* layout2 = new QVBoxLayout;
  layout2->addWidget(status_widget);
  layout2->addStretch(1);
  layout2->addWidget(solver_widget);
  layout2->addStretch(1);
  layout2->addWidget(options_widget);
  w2->setLayout(layout2);

  // level3 layout
  w3 = new QGroupBox;
  w3->setFlat(true);
  QHBoxLayout* layout3 = new QHBoxLayout;
  layout3->addWidget(w1);
  layout3->addWidget(w2);
  w3->setLayout(layout3);

  // top level layout
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(w3);
  layout->addWidget(dialog_button_widget);
  setLayout(layout);

  QTextStream qout(textDevice);
  qout << "Hallo Sudoku-Freunde!\n";

  /////////////////////////////////////////////////////////////////////////////////////////
  // connect signals
  /////////////////////////////////////////////////////////////////////////////////////////
  connect(dialog_button_widget, SIGNAL(rejected()), QApplication::instance(),
          SLOT(quit()));

  connect(this, SIGNAL(text_msg(QString)), textconsole_widget,
          SLOT(appendPlainText(QString)));
  return;
}

void w_Sudoku_view::createTextConsoleWidget() {
  textconsole_widget = new QPlainTextEdit;

  textconsole_widget->setLineWrapMode(QPlainTextEdit::WidgetWidth);
  // textconsole_widget->setLineWrapMode(QPlainTextEdit::NoWrap);

  textconsole_widget->setWordWrapMode(QTextOption::WrapAnywhere);
  textconsole_widget->setReadOnly(true);
  textconsole_widget->moveCursor(QTextCursor::End);  // now insert can be used
  textconsole_widget->verticalScrollBar()->setValue(
  textconsole_widget->verticalScrollBar()->maximum());

  // textconsole_widget->setPlainText("Welcome to ...");
  // textconsole_widget->moveCursor(QTextCursor::End);
  // textconsole_widget->insertPlainText("This text inserted.\n");
  // textconsole_widget->appendPlainText("This text appended.");

  // Link textDevice to textconsole_widget
  textDevice = new PlainTextEditIODevice(textconsole_widget, this);

  // use for output:
  // QTextStream qout(textdevice);
  // qout << "Text";
  return;
}

void w_Sudoku_view::createStatusWidget() {

  status_widget = new QGroupBox("Status:");
  status_widget->setFlat(true);

  valid_value          = new QLabel;
  num_entry_value      = new QLabel;
  num_candidates_value = new QLabel;
  update_status_values();

  QFormLayout* formLayout = new QFormLayout;
  formLayout->addRow("Status: ", valid_value);
  formLayout->addRow("# Einträge: ", num_entry_value);
  formLayout->addRow("# Kandidaten: ", num_candidates_value);
  status_widget->setLayout(formLayout);

  return;
}

void w_Sudoku_view::createSolverWidget() {

  solver_widget = new QGroupBox("Löser:");
  solver_widget->setFlat(true);

  num_naked_singles    = new QLabel;
  num_hidden_singles   = new QLabel;
  num_naked_twins      = new QLabel;
  num_hidden_twins     = new QLabel;
  num_naked_triples    = new QLabel;
  num_hidden_triples   = new QLabel;
  num_naked_quadruples = new QLabel;
  update_number_solution_pattern_values();

  remove_naked_singles_button = new QPushButton("Entfernen");
  connect(remove_naked_singles_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_naked_singles()));

  remove_hidden_singles_button = new QPushButton("Entfernen");
  connect(remove_hidden_singles_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_hidden_singles()));

  remove_naked_twins_button = new QPushButton("Entfernen");
  connect(remove_naked_twins_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_naked_twins()));

  remove_hidden_twins_button = new QPushButton("Entfernen");
  connect(remove_hidden_twins_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_hidden_twins()));

  remove_naked_triples_button = new QPushButton("Entfernen");
  connect(remove_naked_triples_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_naked_triples()));

  remove_hidden_triples_button = new QPushButton("Entfernen");
  connect(remove_hidden_triples_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_hidden_triples()));

  remove_naked_quadruples_button = new QPushButton("Entfernen");
  connect(remove_naked_quadruples_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_naked_quadruples()));

  remove_try_recursive_button = new QPushButton("Entferne rekursiv");
  connect(remove_try_recursive_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_try_recursive()));

  remove_automatic_button = new QPushButton("Entferne alle");
  connect(remove_automatic_button, SIGNAL(clicked()), sudoku_widget,
          SLOT(remove_all()));

  undo_button = new QPushButton("Rückgängig");
  connect(undo_button, SIGNAL(clicked()), sudoku_widget, SLOT(undo_requested()));

  QCheckBox* naked_singles_toggle = new QCheckBox(" Naked Singles  ", this);
  naked_singles_toggle->setChecked(sudoku_widget->prop.show_naked_singles);
  connect(naked_singles_toggle, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_naked_singles(int)));

  QCheckBox* hidden_singles_toggle = new QCheckBox(" Hidden Singles  ", this);
  hidden_singles_toggle->setChecked(sudoku_widget->prop.show_hidden_singles);
  connect(hidden_singles_toggle, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_hidden_singles(int)));

  QCheckBox* naked_twins_toggle = new QCheckBox(" Naked Twins  ", this);
  naked_twins_toggle->setChecked(sudoku_widget->prop.show_naked_twins);
  connect(naked_twins_toggle, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_naked_twins(int)));

  QCheckBox* hidden_twins_toggle = new QCheckBox(" Hidden Twins  ", this);
  hidden_twins_toggle->setChecked(sudoku_widget->prop.show_hidden_twins);
  connect(hidden_twins_toggle, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_hidden_twins(int)));

  QCheckBox* naked_triples_toggle = new QCheckBox(" Naked Triples  ", this);
  naked_triples_toggle->setChecked(sudoku_widget->prop.show_naked_triples);
  connect(naked_triples_toggle, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_naked_triples(int)));

  QCheckBox* hidden_triples_toggle = new QCheckBox(" Hidden Triples  ", this);
  hidden_triples_toggle->setChecked(sudoku_widget->prop.show_hidden_triples);
  connect(hidden_triples_toggle, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_hidden_triples(int)));

  QCheckBox* naked_quadruples_toggle = new QCheckBox(" Naked Quads  ", this);
  naked_quadruples_toggle->setChecked(sudoku_widget->prop.show_naked_quadruples);
  connect(naked_quadruples_toggle, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_naked_quadruples(int)));

  QGridLayout* gridlayout = new QGridLayout;

  gridlayout->addWidget(naked_singles_toggle, 0, 0);
  gridlayout->addWidget(num_naked_singles, 0, 1);
  gridlayout->addWidget(remove_naked_singles_button, 0, 2);
  gridlayout->addWidget(hidden_singles_toggle, 1, 0);
  gridlayout->addWidget(num_hidden_singles, 1, 1);
  gridlayout->addWidget(remove_hidden_singles_button, 1, 2);

  gridlayout->addWidget(naked_twins_toggle, 2, 0);
  gridlayout->addWidget(num_naked_twins, 2, 1);
  gridlayout->addWidget(remove_naked_twins_button, 2, 2);
  gridlayout->addWidget(hidden_twins_toggle, 3, 0);
  gridlayout->addWidget(num_hidden_twins, 3, 1);
  gridlayout->addWidget(remove_hidden_twins_button, 3, 2);

  gridlayout->addWidget(naked_triples_toggle, 4, 0);
  gridlayout->addWidget(num_naked_triples, 4, 1);
  gridlayout->addWidget(remove_naked_triples_button, 4, 2);
  gridlayout->addWidget(hidden_triples_toggle, 5, 0);
  gridlayout->addWidget(num_hidden_triples, 5, 1);
  gridlayout->addWidget(remove_hidden_triples_button, 5, 2);

  gridlayout->addWidget(naked_quadruples_toggle, 6, 0);
  gridlayout->addWidget(num_naked_quadruples, 6, 1);
  gridlayout->addWidget(remove_naked_quadruples_button, 6, 2);

  gridlayout->addWidget(remove_try_recursive_button, 7, 2);
  gridlayout->addWidget(remove_automatic_button, 8, 2);
  gridlayout->addWidget(undo_button, 9, 2);

  solver_widget->setLayout(gridlayout);

  return;
}

void w_Sudoku_view::createOptionsWidget() {

  options_widget = new QGroupBox("Optionen:");
  options_widget->setFlat(true);

  QCheckBox* candidate_toggle = new QCheckBox("Kandidaten", this);
  candidate_toggle->setChecked(sudoku_widget->prop.show_candidate_lists);
  connect(candidate_toggle, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_candidate_lists(int)));

  QCheckBox* active_cell = new QCheckBox("Aktive Zelle", this);
  active_cell->setChecked(sudoku_widget->prop.show_active_cell);
  connect(active_cell, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_active_cell(int)));

  QCheckBox* affected_regions = new QCheckBox("Aktive Regionen", this);
  affected_regions->setChecked(sudoku_widget->prop.show_affected_regions);
  connect(affected_regions, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_affected_regions(int)));

  QCheckBox* block_background = new QCheckBox("Blockhintergrund", this);
  block_background->setChecked(sudoku_widget->prop.show_block_background);
  connect(block_background, SIGNAL(stateChanged(int)), sudoku_widget,
          SLOT(on_toggle_show_block_background(int)));

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(candidate_toggle);
  layout->addWidget(active_cell);
  layout->addWidget(affected_regions);
  layout->addWidget(block_background);
  options_widget->setLayout(layout);

  return;
}

void w_Sudoku_view::update_status_values() {

  if (sudoku_is_valid(s)) {
    valid_value->setText("OK");
  } else {
    valid_value->setText("Unzulässig!");
  }

  num_entry_value->setText(QString::number(sudoku_num_entries(s)) + QString(" von ") +
                           QString::number(s.total_size) + QString("  (") +
                           QString::number(sudoku_num_empty(s)) + QString(" leer)"));

  num_candidates_value->setText(QString::number(sudoku_num_candidates(s)));

  status_widget->repaint();

  return;
}

void w_Sudoku_view::update_number_solution_pattern_values() {

  num_naked_singles->setText(QString::number(sudoku_num_naked_singles(s)));
  num_hidden_singles->setText(QString::number(sudoku_num_hidden_singles(s)));
  num_naked_twins->setText(QString::number(sudoku_num_naked_twins(s)));
  num_hidden_twins->setText(QString::number(sudoku_num_hidden_twins(s)));
  num_naked_triples->setText(QString::number(sudoku_num_naked_triples(s)));
  num_hidden_triples->setText(QString::number(sudoku_num_hidden_triples(s)));
  num_naked_quadruples->setText(QString::number(sudoku_num_naked_quadruples(s)));

  solver_widget->repaint();

  return;
}

void w_Sudoku_view::on_update_requested_by_child() {

  update_status_values();
  update_number_solution_pattern_values();

  return;
}
