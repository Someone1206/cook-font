#pragma once

#include <fio.hpp>

#define ERR(pref, line, xtra) \
{\
    pel("Error: in file " << pref.getInputFile()); \
    pe("Line " << line << " : "); \
    xtra; \
}