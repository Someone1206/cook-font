#include <translator_main.hpp>
#include <fio.hpp>
#include <filesystem>
#include <fstream>
#include <regex>
#include <algorithm>

#include <string>

#include <cstdlib>

#include <errMgr.hpp>
#include <translator_core.hpp>

void translate_main(Preferences& prefs) {
    if (std::filesystem::exists(prefs.getInputFile())) {
        if (std::filesystem::is_directory(prefs.getInputFile())) {
            pel("Error: \"" << prefs.getInputFile() << "\" is a directory.");
            prefs.addError("inp-dir");
            return;
        }
    }
    else {
        pel("Error: \"" << prefs.getInputFile() << "\" does not exist or is invalid.");
        prefs.addError("inp-not");
        return;
    }


    // if (!std::filesystem::create_directories(std::filesystem::path(prefs.getOutputFile()).parent_path())) {
    //     pel("Error: Output path is invalid!");
    //     prefs.addError("out-not");
    //     return;
    // }


    std::ofstream outfile(prefs.getOutputFile());
    std::ifstream infile(prefs.getInputFile());
    std::string fontType;
    fontType.reserve(15);

    uint32_t fontWidth = 0, fontHeight = 0;
    std::string line;
    getline(infile, line);

    {
        std::string tmp;
        tmp.reserve(30);
        // process 1st line
        if (!line.starts_with("font-")) {
            ___LINE1_ERR:
            ERR(
                prefs, 1,
                pel(" File not recognised.");
            );
            return;
        }

        uint32_t xpos = line.find('x', 4);
        tmp = line.substr(5, xpos - 5);
        char *check;
        fontWidth = strtol(tmp.c_str(), &check, 10);

        if (*check) {
            goto ___LINE1_ERR;
        }

        tmp = line.substr(xpos + 1, line.find('-', xpos) - xpos - 1);
        fontHeight = strtol(tmp.c_str(), &check, 10);

        if (*check) {
            goto ___LINE1_ERR;
        }

        fontType = line.substr(line.find('-', xpos) + 1);
    }


    if (prefs.isCPU()) {
        pl("The feature has not yet been implemented");
        return;
    }

    {
        // write macros to file

        std::vector<std::string> definedMacros;
        std::string tmp;
        definedMacros.reserve(5);

        // line.replace('-', '_');
        line = "____" + std::regex_replace(line, std::regex("-"), "_");
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);

        definedMacros.push_back(line);

        outfile << "#ifndef " << line << "\n";
        outfile << "#define " << line << "\n\n\n";

        line = "_" + fontType + "_" + std::to_string(fontWidth) + "x" + std::to_string(fontHeight);
        tmp = line + "_width";
        line.append("_height");

        std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);

        definedMacros.push_back(line);
        definedMacros.push_back(tmp);

        outfile << "#define " << line << "        " << fontHeight << "\n\n";
        outfile << "#define " << tmp << "         " << fontWidth << "\n\n\n";


        line = "decl_" + fontType + "_" + std::to_string(fontWidth) + "x" + std::to_string(fontHeight);
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        definedMacros.push_back(line);

        outfile << "#define " << line << "    \\\n{\\\n\t";




        if (!prefs.isLogSuppressed()) {
            prefs.Reflect();
            pl("Font type(detected): " << fontType);
            pl("Font Width:   " << fontWidth);
            pl("Font Height:  " << fontHeight);

            pl("Defined Macros:");
            for (auto& tmp0 : definedMacros) {
                pl("    " << tmp0);
            }
        }
    }

    int retVal = translate_core(infile, outfile, fontWidth, fontHeight, line, prefs);

    outfile << "\\\n}\n\n\n#endif";

    if (retVal == -1) {
        pel(prefs.getInputFile() << " has syntax errors!");
    } else if (retVal == -2) {
        pl("Feature has not been implemented yet.");
    } else if (retVal == -3) {
        pel(prefs.getInputFile() << " ended unexpectedly. Reached the END OF FILE before finishing.");
    }
}