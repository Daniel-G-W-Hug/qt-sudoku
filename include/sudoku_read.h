// let emacs know this is a C++ header: -*- C++ -*-

#pragma once

#include <iostream>

using namespace std;

void skip_to_int(istream* is)  // modified routine from "Programming, Principles and
                               // Practice", p. 355
{
  if (is->fail()) {  // found something that was not an integer
    is->clear();     // clear flags, we'd like to look at the characters
    char ch;
    while (*is >> ch) {  // throw away non-digits and comment lines
      if (ch == '#') {
        is->unget();  // ignore comments starting with # to end of line
        string line;
        getline(*is, line);
      }
      if (isdigit(ch)) {
        is->unget();  // put back the digit so that it can be read via is
        return;
      }
    }
    cout << "ERROR: bad input (e.g. EOF or file corrupt)\n\n";  // eof or bad:
                                                                // give up
  }

  return;
}

int read_int(istream* is) {
  int int_var = 0;
  if (!(*is >> int_var)) {
    skip_to_int(is);
    *is >> int_var;
  }

  return int_var;
}
