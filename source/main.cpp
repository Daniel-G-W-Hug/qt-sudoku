// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_read.h"
#include "sudoku_solve.h"
#include "sudoku_print.h"    // for debugging only
#include "w_sudoku_view.h"

#include <QApplication>
#include <fstream>
#include <iostream>
#include <istream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // check for arguments, if there is one, it is assumed that it is a file name for
    // input

    std::cout << '\n' << "argc: " << argc << '\n';
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "]: " << argv[i] << '\n';
    }
    std::cout << '\n';

    std::istream* input_stream;
    std::ifstream input_file;

    if (argc == 1) {    // assume user provides input via std::cin

        input_stream = &std::cin;

    } else {    // assume user provides input file name as 1st argument

        input_file.open(argv[1]);
        if (!input_file) {
            std::cout << "\nCan't open input file " << argv[1] << "!\n";
            return 1;
        }
        input_stream = &input_file;
    }
    //****************************************************************************
    // read and initialize sudoku from input_stream
    //****************************************************************************
    // read sudoku size and create sudoku object
    int region_size = read_int(input_stream);
    int bpr         = read_int(input_stream);
    int bpc         = read_int(input_stream);
    Sudoku s(region_size, bpr, bpc);

    // initialize sudoku with input values and initialize candidate values
    for (int cnt = 0; cnt < s.total_size; ++cnt) { s(cnt).val = read_int(input_stream); }
    sudoku_update_candidates_all_cells(s);

    // for (int cnt = 0; cnt < s.total_size; ++cnt) {
    //     fmt::print("{}\n", s(cnt));
    // }
    // fmt::print("Row_test:\n{}\n", &(s.row));

    w_Sudoku_view Sudoku_view(s);
    Sudoku_view.move(700, 100);

    // show widget
    Sudoku_view.show();

    return app.exec();
}
