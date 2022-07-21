#include <fstream>
#include <prefs.hpp>

int translate_core(
    std::ifstream& infile, 
    std::ofstream& outfile, 
    uint32_t&      fontWidth,
    uint32_t&      fontHeight,
    std::string&   line,
    Preferences&   prefs
);