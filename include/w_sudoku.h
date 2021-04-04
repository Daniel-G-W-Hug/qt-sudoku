// let emacs know this is a C++ header: -*- C++ -*-
// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#ifndef W_SUDOKU_H
#define W_SUDOKU_H

#include "sudoku_class.h"
#include "sudoku_solve.h"

#include <QPainter>
#include <QWidget>
#include <vector>

// forward declarations, included in .cpp from <QtWidgets>
class QHBoxLayout;
class QPlainTextEdit;

struct w_Sudoku_dyn_cell_properties {  // dynamic = linked to enter & leave
                                       // events in cell

  bool cell_marked_as_selected{false};  // mark background in tranparend red
  bool cell_marked_as_part_of_region{
      false};  // mark backgound in transparent blue
  // bool cell_has_input_focus{false};              // if true cell accepts user
  // input
};

struct w_Sudoku_cell_properties {  // depend only on status of Sudoku

  bool cell_marked_as_initially_given{false};
  bool cell_marked_as_naked_single{false};
  bool cell_marked_as_hidden_single{false};
  bool cell_marked_as_naked_twin{false};
  bool cell_marked_as_hidden_twin{false};
  bool cell_marked_as_naked_triple{false};
  bool cell_marked_as_hidden_triple{false};
  bool cell_marked_as_naked_quadruple{false};

  // bringt keinen Mehrwert, wenn zu viele Zellen markiert werden
  // (unübersichtlich) bool cell_marked_as_hidden_single_region{false};  // mark
  // if part of hidden single region
};

struct w_Sudoku_properties {
  // display properties for Sudoku as a whole
  bool show_candidate_lists{false};
  bool show_active_cell{true};  // active cell when mouse is located above cell
  bool show_affected_regions{
      false};  // show cells affected by current active cell
  bool show_block_background{
      true};  // show gray blackground in cells of uneven blocks
  bool show_naked_singles{false};
  bool show_hidden_singles{false};
  bool show_naked_twins{false};
  bool show_hidden_twins{false};
  bool show_naked_triples{false};
  bool show_hidden_triples{false};
  bool show_naked_quadruples{false};
};

class w_Sudoku;

class w_Sudoku_cell : public QWidget {
  Q_OBJECT

 public:
  w_Sudoku_cell(Sudoku& t_s,
                int t_cnt,
                w_Sudoku& t_w_Sudoku,
                QWidget* parent = 0);

  int size() const { return cell_size; };

 protected:
  void paintEvent(QPaintEvent* event) override;
  void draw_w_Sudoku_cell(QPainter* qp);

  // Use enter and leave event to mark a cell
  void enterEvent(QEnterEvent* e) override;
  void leaveEvent(QEvent* e) override;

  void keyPressEvent(QKeyEvent* e) override;

  // private slots:

 signals:
  void text_msg(QString msg);  // send messages intended for text console
  void update_parent(
      int from_child);  // signal parent widget that it needs to update
                        // because of changes in this child widget
  void value_changed_by_user(int from_child, int value);  // signal user input

 private:
  const int cell_size{50};  // side length of one cell in pixels
  Sudoku& s;
  const int cnt;
  w_Sudoku& w_s;
};

class w_Sudoku : public QWidget {
  friend class w_Sudoku_cell;

  Q_OBJECT
 public:
  explicit w_Sudoku(Sudoku& t_s,
                    QPlainTextEdit* t_textConsole,
                    QWidget* parent = 0);

  // currently public, should be transferred to property types with public
  // get-/set-func@tions
  w_Sudoku_properties prop;

  void update_all_cells();
  void update_sudoku_solution_type_vectors();

 signals:
  void text_msg(QString msg);  // send messages intended for text console
  void update_parent();

 private slots:

  void on_toggle_show_candidate_lists(int);
  void on_toggle_show_active_cell(int);
  void on_toggle_show_affected_regions(int);
  void on_toggle_show_block_background(int);

  void on_toggle_show_naked_singles(int);
  void on_toggle_show_hidden_singles(int);
  void on_toggle_show_naked_twins(int);
  void on_toggle_show_hidden_twins(int);
  void on_toggle_show_naked_triples(int);
  void on_toggle_show_hidden_triples(int);
  void on_toggle_show_naked_quadruples(int);

  void on_update_request_by_child(int);

  void on_value_changed_by_user(int from_child, int value);

  void remove_naked_singles();
  void remove_hidden_singles();
  void remove_naked_twins();
  void remove_hidden_twins();
  void remove_naked_triples();
  void remove_hidden_triples();
  void remove_naked_quadruples();
  void remove_recursive();
  void remove_algo_all();
  void remove_recursive_algo_all_mixed();

  void undo_requested();

 private:
  void mark_cells_as_solution_regions();
  void store_sudoku_for_undo(Sudoku sh);
  Sudoku retrieve_from_undo();

 private:
  Sudoku& s;  // Sudoku memory managed by caller
  QPlainTextEdit* textConsole;

  vector<Sudoku> undo;

  vector<w_Sudoku_cell*> cell_widgets;
  vector<QHBoxLayout*> row_widgets;

  vector<w_Sudoku_dyn_cell_properties> dyn_cell_prop_default;
  vector<w_Sudoku_dyn_cell_properties> dyn_cell_prop;

  vector<w_Sudoku_cell_properties> cell_prop_default;
  vector<w_Sudoku_cell_properties> cell_prop;

  single_vec naked_singles;
  single_vec hidden_singles;
  twin_vec naked_twins;
  twin_vec hidden_twins;
  triple_vec naked_triples;
  triple_vec hidden_triples;
  quad_vec naked_quadruples;
};

#endif  // W_SUDOKU_H
