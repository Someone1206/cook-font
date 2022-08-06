#include <translator.hpp>
#include <prefs.hpp>
#include <errMgr.hpp>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>

#define PSF1_FONT_WIDTH      8

#define FONT_REG font_height * PSF1_FONT_WIDTH

#define hope a lie that u tell to urself

int Translator::read_psf1(uint16_t font_mode, uint16_t font_height) {
    //
    // hope that buffer is verified properly

    uint16_t length = font_height * PSF1_FONT_WIDTH;
    bool has_unicode = (font_mode & 0x02) != 0,
        has_unicode_seq = (font_mode & 0x04) != 0;


    if ((font_mode & (~0x07)) != 0) {
        pel("Error: unknown PSF1 font mode!");
        return -0x998;
    }

    uint16_t glyph_count = ((font_mode & 0x1) != 0) ? 512 : 256;

    if (has_unicode_seq && !has_unicode) {
        pel("Font specifies sequences without Unicode table.");
        return -0x997;
    }




    /**********************
     * 
     * 
     * Write data to output file
     * 
     * 
     * *************/


    {
        std::vector<std::string> definedMacros;
        std::string tmp;
        definedMacros.reserve(6);

        tmp = "____PSF1_8X" + std::to_string(font_height) + "_";

        definedMacros.push_back(tmp);

        tmp = "TYPE_PSF1_8X18     0";
        definedMacros.push_back(tmp);
        tmp = "PSF1_SIGNATURE     0x3604";
        definedMacros.push_back(tmp);

        tmp = "PSF1_8X" +
            std::to_string(font_height) + "_HEIGHT";
        definedMacros.push_back(tmp);
        tmp = "PSF1_8X" +
            std::to_string(font_height) + "_WIDTH";
        definedMacros.push_back(tmp);

        tmp = "DECL_PSF1_8X" +
            std::to_string(font_height);
        definedMacros.push_back(tmp);

        Translator::outputFile << "#ifndef " << definedMacros[0]
            << "\n#define " << definedMacros[0]
            << "\n\n\n#define " << definedMacros[1] << "     0"
            << "\n\n#define " << definedMacros[2]   << "     0x3604"
            << "\n\n#define " << definedMacros[3]   << "     " << std::to_string(font_height)
            << "\n\n#define " << definedMacros[4]   << "     " << std::to_string(PSF1_FONT_WIDTH)
            << "\n\n#define " << definedMacros[5]
            << "   \\\n{\\\n\t";

        if (!Prefs.isLogSuppressed()) {
            Prefs.Reflect();
            pl("Font Width:     " << PSF1_FONT_WIDTH);
            pl("Font Height:    " << font_height);

            pl("Defined Macros:");
            for (auto& tmp0 : definedMacros) {
                pl("\t" << tmp0);
            }
        }
    }

    /*******************
     * 
     * 
     * Oh My! Finally done with it!
     * 
     * 
     * 
     * ****************/



    char* fontBuffer, * unicodeBuffer = nullptr;
    uint64_t fontBufferLen = font_height * glyph_count, unicodeBufferLen;
    fontBuffer = new char[fontBufferLen];
    Translator::inputFile.read(fontBuffer, fontBufferLen);

    uint64_t readB = Translator::inputFile.gcount();

    if (readB != fontBufferLen) {
        pel("Unable to read font file!");
        return -0x990;
    }

    if (has_unicode) {
        //  TODO:
        // implement this letaa...
    }

    Translator::outputFile << std::hex << std::setw(2) << std::setfill('0');

    for (uint32_t i = 0; i < glyph_count; i++) {
        for (uint32_t j = 0; j < font_height; j++) {
            Translator::outputFile << "0x"
                << (uint32_t)((uint8_t)fontBuffer[(i * font_height) + j]) << ", ";

            if ((j + 1) % 8 == 0) {
                Translator::outputFile << "\\\n\t";
            }
        }

        Translator::outputFile << "\\\n\t/* ";

        if (Prefs.isCharInfo()) {
            Translator::outputFile << "(Character Index: "
                << std::dec << i
                << " (0x" << std::hex << i << ")";
        }

        Translator::outputFile << " */\\\n\t";

    }

    Translator::outputFile << "\\\n\t\\\n}\n\n#endif";

    return 0;
}
