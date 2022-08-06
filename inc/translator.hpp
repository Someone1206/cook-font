#pragma once
#include <fstream>

namespace Translator
{
    void translate_main();
    
    int sfl();

    int zap_org();

    int rawpsf();

    int read_psf1(uint16_t font_mode, uint16_t font_height);
    int read_psf2(char* buffer);

    extern std::ifstream inputFile;
    extern std::ofstream outputFile;
} // namespace Translator
