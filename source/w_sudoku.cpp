// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "w_sudoku.h"

//#include "sudoku_print.h"  // debugging only

#include <QtWidgets>

///////////////////////////////////////////////////////////////////////////////////////////
// w_Sudoku_cell
///////////////////////////////////////////////////////////////////////////////////////////
w_Sudoku_cell::w_Sudoku_cell(Sudoku& t_s, int t_cnt, w_Sudoku& t_w_Sudoku,
                             QWidget* parent) :
    QWidget(parent),
    s(t_s), cnt(t_cnt), w_s(t_w_Sudoku) {

  // resize to and define minimum size w_Sudoku_cell
  resize(cell_size, cell_size);
  setMinimumSize(cell_size, cell_size);
  setMaximumSize(cell_size, cell_size);

  // set white as background color
  QPalette pal = palette();
  pal.setColor(QPalette::Background, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);

  return;
}

void w_Sudoku_cell::paintEvent(QPaintEvent* e) {
  Q_UNUSED(e);
  QPainter qp(this);
  draw_w_Sudoku_cell(&qp);

  return;
}

void w_Sudoku_cell::draw_w_Sudoku_cell(QPainter* qp) {

  // define logical coordinates (start with (0,0) in upper left corner)
  qp->setWindow(QRect(0, 0, cell_size, cell_size));

  // set fixed viewport size (to avoid scaling of drawing when manually resizing the
  // window)
  qp->setViewport(QRect(0, 0, cell_size, cell_size));

  qp->save();

  // draw cell frame in logical coordinates
  const QRect cell_view(0, 0, cell_size, cell_size);

  qp->setPen(QPen(Qt::black, 1, Qt::SolidLine));
  if (w_s.prop.show_block_background && (s(cnt).bi % 2 != 0)) {
    // qp->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    qp->setBrush(QBrush(QColor(240, 240, 240), Qt::SolidPattern));
  } else {
    qp->setBrush(Qt::NoBrush);
  }
  qp->drawRect(cell_view);

  // draw cell value centered in cell (only if cell is not empty)
  if (s(cnt).val > 0) {
    qp->setFont(QFont("Helvetica", 24));
    if (w_s.cell_prop_default[cnt].cell_marked_as_initially_given) {
      // initially given cell values in black
      qp->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    } else {
      // newly added cell values in blue
      qp->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    }
    qp->drawText(cell_view, Qt::AlignCenter, QString::number(s(cnt).val));
  }

  // draw all candidates of cell (if enabled)
  if (w_s.prop.show_candidate_lists) {

    qp->setFont(QFont("Helvetica", 12));
    qp->setPen(QPen(Qt::black, 1, Qt::SolidLine));

    for (const auto& c : s(cnt).cand) {

      // ATTENTION: all drawing commands manually adjusted for cell_size == 50
      //            if cell_size is changed these values must be adjusted as well

      qp->setFont(QFont("Helvetica", 12));

      // mark candidate with bold font, if it is a naked single
      if (w_s.prop.show_naked_singles && w_s.cell_prop[cnt].cell_marked_as_naked_single) {

        for (const auto& e : w_s.naked_singles) {
          const auto& [cnt1, region, subregion, val1] = e;
          if (cnt == cnt1 && c == val1) {  // find value of hidden single and mark bold
            qp->setFont(QFont("Helvetica", 12, QFont::Bold));
            break;
          }
        }
      }

      // mark candidate with bold font, if it is a hidden single
      if (w_s.prop.show_hidden_singles &&
          w_s.cell_prop[cnt].cell_marked_as_hidden_single) {

        for (const auto& e : w_s.hidden_singles) {
          const auto& [cnt1, region, subregion, val1] = e;
          if (cnt == cnt1 && c == val1) {  // find value of hidden single and mark bold
            qp->setFont(QFont("Helvetica", 12, QFont::Bold));
            break;
          }
        }
      }

      // mark candidate with bold font, if it is a naked twin
      if (w_s.prop.show_naked_twins && w_s.cell_prop[cnt].cell_marked_as_naked_twin) {

        for (const auto& e : w_s.naked_twins) {
          const auto& [cnt1, cnt2, region, subregion, val1, val2] = e;
          if ((cnt == cnt1 && (c == val1 || c == val2)) ||
              (cnt == cnt2 && (c == val1 || c == val2))) {
            // find value of naked twin and mark bold
            qp->setFont(QFont("Helvetica", 12, QFont::Bold));
            break;
          }
        }
      }

      // mark candidate with bold font, if it is a hidden twin
      if (w_s.prop.show_hidden_twins && w_s.cell_prop[cnt].cell_marked_as_hidden_twin) {

        for (const auto& e : w_s.hidden_twins) {
          const auto& [cnt1, cnt2, region, subregion, val1, val2] = e;
          if ((cnt == cnt1 && (c == val1 || c == val2)) ||
              (cnt == cnt2 && (c == val1 || c == val2))) {
            // find value of hidden twin and mark bold
            qp->setFont(QFont("Helvetica", 12, QFont::Bold));
            break;
          }
        }
      }

      // mark candidate with bold font, if it is a naked triple
      if (w_s.prop.show_naked_triples && w_s.cell_prop[cnt].cell_marked_as_naked_triple) {

        for (const auto& e : w_s.naked_triples) {
          const auto& [cnt1, cnt2, cnt3, region, subregion, val1, val2, val3] = e;
          if ((cnt == cnt1 && (c == val1 || c == val2 || c == val3)) ||
              (cnt == cnt2 && (c == val1 || c == val2 || c == val3)) ||
              (cnt == cnt3 && (c == val1 || c == val2 || c == val3))) {
            // find values of naked triple and mark bold
            qp->setFont(QFont("Helvetica", 12, QFont::Bold));
            break;
          }
        }
      }

      // mark candidate with bold font, if it is a hidden triple
      if (w_s.prop.show_hidden_triples && w_s.cell_prop[cnt].cell_marked_as_hidden_triple) {

        for (const auto& e : w_s.hidden_triples) {
          const auto& [cnt1, cnt2, cnt3, region, subregion, val1, val2, val3] = e;
          if ((cnt == cnt1 && (c == val1 || c == val2 || c == val3)) ||
              (cnt == cnt2 && (c == val1 || c == val2 || c == val3)) ||
              (cnt == cnt3 && (c == val1 || c == val2 || c == val3))) {
            // find values of hidden triple and mark bold
            qp->setFont(QFont("Helvetica", 12, QFont::Bold));
            break;
          }
        }
      }

      // mark candidate with bold font, if it is a naked quadruple
      if (w_s.prop.show_naked_quadruples &&
          w_s.cell_prop[cnt].cell_marked_as_naked_quadruple) {

        for (const auto& e : w_s.naked_quadruples) {
          const auto& [cnt1, cnt2, cnt3, cnt4, region, subregion, val1, val2, val3,
                       val4] = e;
          if ((cnt == cnt1 && (c == val1 || c == val2 || c == val3 || c == val4)) ||
              (cnt == cnt2 && (c == val1 || c == val2 || c == val3 || c == val4)) ||
              (cnt == cnt3 && (c == val1 || c == val2 || c == val3 || c == val4)) ||
              (cnt == cnt4 && (c == val1 || c == val2 || c == val3 || c == val4))) {
            // find values of naked quadruple and mark bold
            qp->setFont(QFont("Helvetica", 12, QFont::Bold));
            break;
          }
        }
      }

      switch (c) {
        case 1:
          qp->drawText(QRect(5, 6, 40, 12), Qt::AlignLeft, QString::number(c));
          break;
        case 2:
          qp->drawText(QRect(5, 6, 40, 12), Qt::AlignHCenter, QString::number(c));
          break;
        case 3:
          qp->drawText(QRect(5, 6, 40, 12), Qt::AlignRight, QString::number(c));
          break;
        case 4:
          qp->drawText(QRect(5, 21, 40, 12), Qt::AlignLeft, QString::number(c));
          break;
        case 5:
          qp->drawText(QRect(5, 21, 40, 12), Qt::AlignHCenter, QString::number(c));
          break;
        case 6:
          qp->drawText(QRect(5, 21, 40, 12), Qt::AlignRight, QString::number(c));
          break;
        case 7:
          qp->drawText(QRect(6, 36, 40, 12), Qt::AlignLeft, QString::number(c));
          break;
        case 8:
          qp->drawText(QRect(6, 36, 40, 12), Qt::AlignHCenter, QString::number(c));
          break;
        case 9:
          qp->drawText(QRect(6, 36, 40, 12), Qt::AlignRight, QString::number(c));
          break;
        default: break;
      }
    }
  }

  // show cells marked as part of selected region
  if (w_s.prop.show_affected_regions) {
    if (w_s.dyn_cell_prop[cnt].cell_marked_as_part_of_region) {
      qp->setBrush(QColor(0, 0, 255, 30));  // transparent blue
      qp->drawRect(0, 0, cell_size, cell_size);
    }
  }

  // mark cell as active cell
  if (w_s.prop.show_active_cell) {
    if (w_s.dyn_cell_prop[cnt].cell_marked_as_selected) {
      qp->setBrush(QColor(255, 0, 0, 30));  // transparent red
      qp->drawRect(0, 0, cell_size, cell_size);
    }
  }

  // mark cell if it contains a naked single
  if (w_s.prop.show_naked_singles && w_s.cell_prop[cnt].cell_marked_as_naked_single) {
    qp->setBrush(QColor(255, 255, 0, 30));  // transparent yellow
    qp->drawRect(0, 0, cell_size, cell_size);
  }

  // mark cell if it contains a hidden single
  if (w_s.prop.show_hidden_singles && w_s.cell_prop[cnt].cell_marked_as_hidden_single) {
    qp->setBrush(QColor(0, 255, 0, 30));  // transparent green
    qp->drawRect(0, 0, cell_size, cell_size);
  }

  // bringt keinen Mehrwert, wenn zu viele Zellen markiert werden (unübersichtlich)
  // // mark cell if it is in a region that determines the hidden single
  // if (w_s.prop.show_hidden_singles &&
  // w_s.cell_prop[cnt].cell_marked_as_hidden_single_region) {
  //   qp->setBrush(QColor(0, 255, 0, 10));  // light transparent green
  //   qp->drawRect(0, 0, cell_size, cell_size);
  // }

  if (w_s.prop.show_naked_twins && w_s.cell_prop[cnt].cell_marked_as_naked_twin) {
    qp->setBrush(QColor(255, 128, 0, 30));  // transparent orange
    qp->drawRect(0, 0, cell_size, cell_size);
  }

  if (w_s.prop.show_hidden_twins && w_s.cell_prop[cnt].cell_marked_as_hidden_twin) {
    qp->setBrush(QColor(255, 0, 255, 30));  // transparent magenta
    qp->drawRect(0, 0, cell_size, cell_size);
  }

  if (w_s.prop.show_naked_triples && w_s.cell_prop[cnt].cell_marked_as_naked_triple) {
    qp->setBrush(QColor(128, 0, 255, 30));  // transparent violet
    qp->drawRect(0, 0, cell_size, cell_size);
  }

  if (w_s.prop.show_hidden_triples && w_s.cell_prop[cnt].cell_marked_as_hidden_triple) {
    qp->setBrush(QColor(0, 128, 255, 30));  // transparent light blue
    qp->drawRect(0, 0, cell_size, cell_size);
  }

  if (w_s.prop.show_naked_quadruples &&
      w_s.cell_prop[cnt].cell_marked_as_naked_quadruple) {
    qp->setBrush(QColor(128, 128, 0, 30));  // transparent olive
    qp->drawRect(0, 0, cell_size, cell_size);
  }

  qp->restore();

  return;
}

void w_Sudoku_cell::enterEvent(QEvent* e) {

  // emit text_msg(QString("Enter Event in cell ") + QString::number(cnt));

  // mark cell
  w_s.dyn_cell_prop[cnt].cell_marked_as_selected = true;

  // mark cells in current row
  int curr_row = s(cnt).ri;
  for (int j = 0; j < s.region_size; ++j) {
    w_s.dyn_cell_prop[s.row_to_cnt(curr_row, j)].cell_marked_as_part_of_region = true;
  }

  // mark cells in current col
  int curr_col = s(cnt).ci;
  for (int j = 0; j < s.region_size; ++j) {
    w_s.dyn_cell_prop[s.col_to_cnt(curr_col, j)].cell_marked_as_part_of_region = true;
  }

  // mark cells in current block
  int curr_block = s(cnt).bi;
  for (int j = 0; j < s.region_size; ++j) {
    w_s.dyn_cell_prop[s.block_to_cnt(curr_block, j)].cell_marked_as_part_of_region = true;
  }

  emit update_parent(cnt);
  //  emit text_msg(QString("Update_parent() request in cell ") + QString::number(cnt));

  return;
}

void w_Sudoku_cell::leaveEvent(QEvent* e) {

  // emit text_msg(QString("Leave Event in cell ") + QString::number(cnt));
  // unmark all cells to default value (in case of fast mouse movements)
  w_s.dyn_cell_prop = w_s.dyn_cell_prop_default;

  emit update_parent(cnt);
  //  emit text_msg(QString("Update_parent() request from cell ") + QString::number(cnt));

  return;
}

// void w_Sudoku_cell::mark_cell_as_selected(const int cell_idx) {

// }

///////////////////////////////////////////////////////////////////////////////////////////
// w_Sudoku
///////////////////////////////////////////////////////////////////////////////////////////
w_Sudoku::w_Sudoku(Sudoku& t_s, QPlainTextEdit* t_textConsole, QWidget* parent) :
    QWidget(parent), s(t_s), textConsole(t_textConsole) {

  w_Sudoku_dyn_cell_properties sudoku_dyn_cell_prop_default;
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    dyn_cell_prop_default.push_back(sudoku_dyn_cell_prop_default);
  }
  dyn_cell_prop = dyn_cell_prop_default;

  w_Sudoku_cell_properties sudoku_cell_prop_default;
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    cell_prop_default.push_back(sudoku_cell_prop_default);
    // mark cells initially given as input
    if (s(cnt).val != 0) cell_prop_default[cnt].cell_marked_as_initially_given = true;
  }
  cell_prop = cell_prop_default;

  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    // for (int cnt = 0; cnt < 1; ++cnt) {
    cell_widgets.push_back(new w_Sudoku_cell(s, cnt, *this));
  }

  // resize to and define minimum size of w_Sudoku
  // int widget_size = s.region_size * cell_widgets[0]->size();
  // resize(widget_size, widget_size);
  // setMinimumSize(widget_size, widget_size);
  // setMaximumSize(widget_size, widget_size);

  resize(450, 450);
  setMinimumSize(450, 450);
  setMaximumSize(450, 450);

  // activate w_Sudoku layout
  QVBoxLayout* layout = new QVBoxLayout;

  // begin at upper left corner and don't use any spacing between cells
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  int cnt = 0;
  for (int i = 0; i < s.region_size; ++i) {
    // for (int i = 0; i < 1; ++i) {
    row_widgets.push_back(new QHBoxLayout);
    for (int j = 0; j < s.region_size; ++j, ++cnt) {
      // for (int j = 0; j < 1; ++j, ++cnt) {
      row_widgets.back()->addWidget(cell_widgets[cnt]);
    }
    layout->addLayout(row_widgets.back());
    // layout->addStretch(1);
  }

  setLayout(layout);

  // connect text_msg from cells with console widget in Sudoku_view
  for (int i = 0; i < s.total_size; ++i) {
    connect(cell_widgets[i], SIGNAL(text_msg(QString)), textConsole,
            SLOT(appendPlainText(QString)));
  }

  // connect text_msg from w_Sudoku with console widget in Sudoku_view
  connect(this, SIGNAL(text_msg(QString)), textConsole, SLOT(appendPlainText(QString)));

  // connect update_parent() from w_Sudoku with on_update_requested_by child
  // in Sudoku_view
  connect(this, SIGNAL(update_parent()),
	  parent, SLOT(on_update_requested_by_child()));

  // connect update_parent() msg from cells with on_update_request_by_child() in
  // Sudoku_view
  for (int i = 0; i < s.total_size; ++i) {
    connect(cell_widgets[i], SIGNAL(update_parent(int)), this,
            SLOT(on_update_request_by_child(int)));
  }

  // intialize solution types (do once for w_sudoku, use for all cells)
  update_sudoku_solution_type_vectors();

  return;
}

void w_Sudoku::update_all_cells() {

  // update all sudoku cells
  for (int i = 0; i < s.total_size; ++i) { cell_widgets[i]->update(); }

  return;
}

void w_Sudoku::update_sudoku_solution_type_vectors() {

  naked_singles    = sudoku_naked_singles(s);
  hidden_singles   = sudoku_hidden_singles(s);
  naked_twins      = sudoku_naked_twins(s);
  hidden_twins     = sudoku_hidden_twins(s);
  naked_triples    = sudoku_naked_triples(s);
  hidden_triples   = sudoku_hidden_triples(s);
  naked_quadruples = sudoku_naked_quadruples(s);

  mark_cells_as_solution_regions();

  return;
}

void w_Sudoku::on_toggle_show_candidate_lists(int) {
  // toggle value and schedule update
  prop.show_candidate_lists = !prop.show_candidate_lists;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_active_cell(int) {
  // toggle value and schedule update
  prop.show_active_cell = !prop.show_active_cell;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_affected_regions(int) {
  // toggle value and schedule update
  prop.show_affected_regions = !prop.show_affected_regions;
  // requires update of all cells
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_block_background(int) {
  // toggle value and schedule update
  prop.show_block_background = !prop.show_block_background;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_naked_singles(int) {
  // toggle value and schedule update
  prop.show_naked_singles = !prop.show_naked_singles;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_hidden_singles(int) {
  // toggle value and schedule update
  prop.show_hidden_singles = !prop.show_hidden_singles;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_naked_twins(int) {
  // toggle value and schedule update
  prop.show_naked_twins = !prop.show_naked_twins;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_hidden_twins(int) {
  // toggle value and schedule update
  prop.show_hidden_twins = !prop.show_hidden_twins;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_naked_triples(int) {
  // toggle value and schedule update
  prop.show_naked_triples = !prop.show_naked_triples;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_hidden_triples(int) {
  // toggle value and schedule update
  prop.show_hidden_triples = !prop.show_hidden_triples;
  update_all_cells();

  return;
}

void w_Sudoku::on_toggle_show_naked_quadruples(int) {
  // toggle value and schedule update
  prop.show_naked_quadruples = !prop.show_naked_quadruples;
  update_all_cells();

  return;
}

void w_Sudoku::on_update_request_by_child(int from_child) {

  // emit text_msg(QString("w_Sudoku update request by child: ") +
  // QString::number(from_child)); normally called when changes induced by child require
  // widget update
  update_all_cells();

  return;
}

void w_Sudoku::remove_naked_singles() {

  store_sudoku_for_undo(s);
  int num_removed = sudoku_remove_naked_singles(s);
  emit text_msg(QString::number(num_removed) + QString(" naked single(s) entfernt."));
  update_sudoku_solution_type_vectors();
  update_all_cells();
  
  emit update_parent();

  return;
}

void w_Sudoku::remove_hidden_singles() {

  store_sudoku_for_undo(s);
  int num_removed = sudoku_remove_hidden_singles(s);
  emit text_msg(QString::number(num_removed) + QString(" hidden single(s) entfernt."));
  update_sudoku_solution_type_vectors();
  update_all_cells();
  
  emit update_parent();

  return;
}

void w_Sudoku::remove_naked_twins() {

  store_sudoku_for_undo(s);
  int num_removed = sudoku_remove_naked_twins(s);
  emit text_msg(QString::number(num_removed) + QString(" naked twin(s) entfernt."));
  update_sudoku_solution_type_vectors();
  update_all_cells();

  emit update_parent();

  return;
}

void w_Sudoku::remove_hidden_twins() {

  store_sudoku_for_undo(s);
  int num_removed = sudoku_remove_hidden_twins(s);
  emit text_msg(QString::number(num_removed) + QString(" hidden twin(s) entfernt."));
  update_sudoku_solution_type_vectors();
  update_all_cells();

  emit update_parent();

  return;
}

void w_Sudoku::remove_naked_triples() {

  store_sudoku_for_undo(s);
  int num_removed = sudoku_remove_naked_triples(s);
  emit text_msg(QString::number(num_removed) + QString(" naked triple(s) entfernt."));
  update_sudoku_solution_type_vectors();
  update_all_cells();

  emit update_parent();

  return;
}

void w_Sudoku::remove_hidden_triples() {

  store_sudoku_for_undo(s);
  int num_removed = sudoku_remove_hidden_triples(s);
  emit text_msg(QString::number(num_removed) + QString(" hidden triple(s) entfernt."));
  update_sudoku_solution_type_vectors();
  update_all_cells();

  emit update_parent();

  return;
}

void w_Sudoku::remove_naked_quadruples() {

  store_sudoku_for_undo(s);
  int num_removed = sudoku_remove_naked_quadruples(s);
  emit text_msg(QString::number(num_removed) + QString(" naked quadruple(s) entfernt."));
  update_sudoku_solution_type_vectors();
  update_all_cells();

  emit update_parent();

  return;
}

void w_Sudoku::undo_requested() {

  if (undo.size() > 0) {
    s = retrieve_from_undo();
    update_sudoku_solution_type_vectors();
    update_all_cells();
  } else {
    emit text_msg(QString("No further undo information available."));
  }
  
  emit update_parent();
  
  return;
}

void w_Sudoku::mark_cells_as_solution_regions() {

  // initialize with default value
  cell_prop = cell_prop_default;

  // mark naked singles
  for (const auto& e : naked_singles) {
    const auto& [cnt, region, subregion, val]  = e;
    cell_prop[cnt].cell_marked_as_naked_single = true;
  }

  // mark hidden singles
  for (const auto& e : hidden_singles) {
    const auto& [cnt, region, subregion, val]   = e;
    cell_prop[cnt].cell_marked_as_hidden_single = true;
  }

  // bringt keinen Mehrwert, wenn zu viele Zellen markiert werden (unübersichtlich)
  // // mark regions of hidden singles
  // for (const auto& e : hidden_singles) {
  //   const auto& [cnt,region,subregion,val] = e;
  //   for (int i = 0; i < s.region_size; ++i) {
  //     cell_prop[s.region_to_cnt(region, subregion,
  //     i)].cell_marked_as_hidden_single_region =
  //         true;
  //   }
  // }

  // mark naked twins
  for (const auto& e : naked_twins) {
    const auto& [cnt1, cnt2, region, subregion, val1, val2] = e;
    cell_prop[cnt1].cell_marked_as_naked_twin               = true;
    cell_prop[cnt2].cell_marked_as_naked_twin               = true;
  }

  // mark hidden twins
  for (const auto& e : hidden_twins) {
    const auto& [cnt1, cnt2, region, subregion, val1, val2] = e;
    cell_prop[cnt1].cell_marked_as_hidden_twin              = true;
    cell_prop[cnt2].cell_marked_as_hidden_twin              = true;
  }

  // mark naked triples
  for (const auto& e : naked_triples) {
    const auto& [cnt1, cnt2, cnt3, region, subregion, val1, val2, val3] = e;
    cell_prop[cnt1].cell_marked_as_naked_triple                         = true;
    cell_prop[cnt2].cell_marked_as_naked_triple                         = true;
    cell_prop[cnt3].cell_marked_as_naked_triple                         = true;
  }

  // mark hidden triples
  for (const auto& e : hidden_triples) {
    const auto& [cnt1, cnt2, cnt3, region, subregion, val1, val2, val3] = e;
    cell_prop[cnt1].cell_marked_as_hidden_triple                        = true;
    cell_prop[cnt2].cell_marked_as_hidden_triple                        = true;
    cell_prop[cnt3].cell_marked_as_hidden_triple                        = true;
  }

  // mark naked quadruples
  for (const auto& e : naked_quadruples) {
    const auto& [cnt1, cnt2, cnt3, cnt4, region, subregion, val1, val2, val3, val4] = e;
    cell_prop[cnt1].cell_marked_as_naked_quadruple = true;
    cell_prop[cnt2].cell_marked_as_naked_quadruple = true;
    cell_prop[cnt3].cell_marked_as_naked_quadruple = true;
    cell_prop[cnt4].cell_marked_as_naked_quadruple = true;
  }

  return;
}

void w_Sudoku::store_sudoku_for_undo(Sudoku sh) {
  
  undo.push_back(sh);

  return;
}

Sudoku w_Sudoku::retrieve_from_undo() {

  Sudoku sh(s.region_size, s.blocks_per_row, s.blocks_per_col);
  
  if (undo.size() > 0) {
    sh = undo.back();
    undo.pop_back();
    emit text_msg(QString("Undo performed. Number of undo steps remaining: ") +
                  QString::number(undo.size()));
  }

  return sh;
}
