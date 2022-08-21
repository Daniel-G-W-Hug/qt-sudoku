// let emacs know this is a C++ header: -*- C++ -*-
// 345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <exception>    // terminate()
#include <iostream>
#include <string>

void dynamic_assert(bool assertion, std::string message);
