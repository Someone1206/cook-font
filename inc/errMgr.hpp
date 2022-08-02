#pragma once

#include <fio.hpp>

#define _ERR(line, xtra) \
{\
    pel("Error: in file " << Prefs.getInputFile()); \
    pe("Line " << line << " : "); \
    xtra; \
}