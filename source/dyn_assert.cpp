// let emacs know this is a C++ header: -*- C++ -*-
// 345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "dyn_assert.h"

void dynamic_assert(bool assertion, string message) {
  if (assertion) {
    return;
  } else {
    cout << message << "\n";
    cout << "Terminating.\n";
    terminate();
  }

  return;
}
