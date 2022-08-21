// let emacs know this is a C++ header: -*- C++ -*-
// 345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "dyn_assert.h"

void dynamic_assert(bool assertion, std::string message) {
    if (assertion) {
        return;
    } else {
        std::cout << message << "\n";
        std::cout << "Terminating.\n";
        std::terminate();
    }
}