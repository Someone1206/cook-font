#include <translator.hpp>
#include <prefs.hpp>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <iomanip>

#include <errMgr.hpp>


#include <cstdlib>


int sfl_core(uint32_t font_width, uint32_t font_height, std::string& line);

int cless8(uint32_t font_width, std::string& line);

int Translator::sfl() {
    //
    Translator::outputFile.open(Prefs.getOutputFile());
    Translator::inputFile.open(Prefs.getInputFile());


    std::string line, tmp, fontType;
    uint32_t    font_width, font_height;
    getline(Translator::inputFile, line);

    {
        if (!line.starts_with("font-")) {
            _ERR(
                1, pel(" File not recognised.");
            );
            return -10;
        }

        uint32_t xpos = line.find('x', 4);
        tmp = line.substr(5, xpos - 5);
        char* check;
        font_width = strtol(tmp.c_str(), &check, 10);

        if (*check) {
            _ERR(
                1,
                pel("Invalid Width!");
            );
            return -11;
        }

        tmp = line.substr(xpos + 1, line.find('-', xpos) - xpos - 1);
        font_height = strtol(tmp.c_str(), &check, 10);

        if (*check) {
            _ERR(
                1,
                pel("Invalid Height!");
            );
            return -12;
        }

        uint64_t lastI = line.find_last_of('-');

        if (line.ends_with('\r')) {
            fontType = line.substr(lastI + 1, line.length() - lastI - 2);
        } else {
            fontType = line.substr(lastI + 1);
        }
    }

    {
        // write macros to file
        std::vector<std::string> definedMacros;
        definedMacros.reserve(6);

        // line = "____" + std::regex_replace(line, std::regex("-"), "_");
        line = "____" + fontType + "_" + std::to_string(font_width) + "x"
            + std::to_string(font_height) + "_";
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);

        definedMacros.push_back(line);

        line = "TYPE_PSF1_8X18";
        definedMacros.push_back(line);

        line = "PSF1_SIGNATURE";
        definedMacros.push_back(line);

        line = fontType + "_" + std::to_string(font_width) + "x"
            + std::to_string(font_height) + "_height";
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        definedMacros.push_back(line);

        line = fontType + "_" + std::to_string(font_width) + "x"
            + std::to_string(font_height) + "_width";
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        definedMacros.push_back(line);

        line = "decl_" + fontType + "_" + std::to_string(font_width)
            + "x" + std::to_string(font_height);
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        definedMacros.push_back(line);


        if (fontType == "psf1") {
            line = "     0x3604";
        } else {
            line = "";
        }


        Translator::outputFile << "#ifndef " << definedMacros[0]
            << "\n#define " << definedMacros[0]
            << "\n\n\n#define " << definedMacros[1] << "     0"
            << "\n\n#define " << definedMacros[2]   << line
            << "\n\n#define " << definedMacros[3]   << "     " << std::to_string(font_height)
            << "\n\n#define " << definedMacros[4]   << "      " << std::to_string(font_width)
            << "\n\n#define " << definedMacros[5]
            << "   \\\n{\\\n\t";


        if (!Prefs.isLogSuppressed()) {
            Prefs.Reflect();
            pl("Font type(detected): " << fontType);
            pl("Font Width:   " << font_width);
            pl("Font Height:  " << font_height);

            pl("Defined Macros:");
            for (auto& tmp0 : definedMacros) {
                pl("    " << tmp0);
            }
        }
    }

    int retVal = sfl_core(font_width, font_height, line);

    Translator::outputFile << "\\\n}\n\n\n#endif";

    if (retVal == -1) {
        pel(Prefs.getInputFile() << " has syntax errors!");
    } else if (retVal == -2) {
        pl("Feature has not been implemented yet.");
    } else if (retVal == -3) {
        pel(
            Prefs.getInputFile() 
            << " ended unexpectedly. Reached the END OF FILE before finishing."
        );
    }

    return retVal;
}



int sfl_core(uint32_t font_width, uint32_t font_height, std::string& line) {
    uint32_t heightC = 0, charCount = 0, inLineCount = 2;
    // - comment/divider

    uint8_t byteCount = 0;

    getline(Translator::inputFile, line);

    if (!line.starts_with("-")) {
        _ERR(
            inLineCount, 
            pel("File should have - at the second line");
        );
        return -1;
    }


    while (getline(Translator::inputFile, line)) {
        inLineCount++;
        if (line.empty()) {
            continue;
        }
        
        if (line.back() == '\r') {
            line = line.substr(0, line.length() - 1);
        }
        
        if (line.starts_with("-")) {
            if (heightC != font_height) {
                _ERR(
                    inLineCount,
                    pel("Font Height does not match glyph height. Font Height: "
                        << font_height << ", Glyph Height: " << heightC);
                );
                return -1;
            }
            heightC = 0;

            
            // commenting system
            Translator::outputFile << "\\\n\t/* ";
            if (Prefs.isFontComments()) {
                Translator::outputFile << line.substr(1);
            }
            if (Prefs.isCharInfo()) {
                Translator::outputFile << " (charater: " << charCount << ") ";
            }
            Translator::outputFile << " */ \\\n\t\\\n\t";

            charCount++;
            byteCount = 0;
            continue;
        }


        if (line.length() == font_width) {
            _PROCESS_LINE:
            if (font_width <= 8) {
                if (cless8(font_width, line) == -1) {
                    _ERR(
                        inLineCount,
                        pel("Syntax Error! Number is non-binary!")
                    );
                    return -1;
                }
                byteCount++;
            } else {
                //
                return -2;
            }
        } else {
            _ERR(
                inLineCount,
                pel("Length of line [" << line.length() 
                    << "] is not same as font width ["
                    << font_width << "]");
            );
        }

        if (byteCount >= 8) {
            Translator::outputFile << "  \\\n\t";
            byteCount = 0;
        }
        heightC++;
    }

    if (heightC != 0 && heightC != font_height) {
        return -3;
    }

    return 0;
}

int cless8(uint32_t font_width, std::string& line) {
    line = line + std::string(8 - font_width, '0');

    char* errH;

    int number = strtol(line.c_str(), &errH, 2);

    if (*errH) {
        return -1;
    }

    Translator::outputFile << std::hex 
        << "0x"
        << std::setw(2) 
        << std::setfill('0') 
        << number << ", ";

    return 1;
}