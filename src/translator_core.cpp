#include <translator_core.hpp>

#include <errMgr.hpp>

#include <iomanip>
#include <cstdlib>

#include <fio.hpp>

int less8(
    std::ofstream&  outfile,
    uint32_t&       fontWidth,
    std::string&    line
) {
    std::string tmp;
    line = line + std::string(8 - fontWidth, '0');

    char* errH;

    int number = strtol(line.c_str(), &errH, 2);

    if (*errH) {
        return -1;
    }

    outfile << std::hex 
        << "0x"
        << std::setw(2) 
        << std::setfill('0') 
        << number << ", ";

    return 1;
}


int translate_core(
    std::ifstream& infile, 
    std::ofstream& outfile, 
    uint32_t&      fontWidth,
    uint32_t&      fontHeight,
    std::string&   line,
    Preferences&   prefs
) {
    uint32_t heightC = 0, charCount = 0, inLineCount = 2;
    // - comment/divider

    uint8_t byteCount = 0;

    getline(infile, line);

    if (!line.starts_with("-")) {
        ERR(prefs, inLineCount, 
            pel("File should have - at the second line");
        );
        return -1;
    }


    while (getline(infile, line)) {
        inLineCount++;
        if (line.empty()) {
            continue;
        }
        
        if (line.back() == '\r') {
            line = line.substr(0, line.length() - 1);
        }
        
        if (line.starts_with("-")) {
            if (heightC != fontHeight) {
                ERR(
                    prefs, inLineCount,
                    pel("Font Height does not match glyph height. Font Height: "
                        << fontHeight << ", Glyph Height: " << heightC);
                );
                return -1;
            }
            heightC = 0;
            outfile << "\\\n\t/* " 
                << line.substr(1) 
                << " (done " 
                << charCount 
                << ") */ \\\n\t\\\n\t";
            charCount++;
            byteCount = 0;
            continue;
        }


        if (line.length() == fontWidth) {
            _PROCESS_LINE:
            if (fontWidth <= 8) {
                if (less8(outfile, fontWidth, line) == -1) {
                    ERR(
                        prefs, inLineCount,
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
            ERR(
                prefs, inLineCount,
                pel("Length of line [" << line.length() 
                    << "] is not same as font width ["
                    << fontWidth << "]");
            );
        }

        if (byteCount >= 8) {
            outfile << "  \\\n\t";
            byteCount = 0;
        }
        heightC++;
    }

    if (heightC != 0 && heightC != fontHeight) {
        return -3;
    }

    return 0;
}