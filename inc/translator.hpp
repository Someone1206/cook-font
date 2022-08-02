#pragma once
#include <fstream>

namespace Translator
{
    void translate_main();
    
    int sfl();

    extern std::ifstream inputFile;
    extern std::ofstream outputFile;
} // namespace Translator
