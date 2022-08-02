#include <translator_main.hpp>
#include <fio.hpp>
#include <filesystem>
#include <fstream>
#include <regex>
#include <algorithm>

#include <string>

#include <cstdlib>

#include <errMgr.hpp>

#include <translator.hpp>


void Translator::translate_main() {
    if (std::filesystem::exists(Prefs.getInputFile())) {
        if (std::filesystem::is_directory(Prefs.getInputFile())) {
            pel("Error: \"" <<
                Prefs.getInputFile() <<
                "\"is a directory."
            );
            Prefs.addError("inp-dir");
            return;
        }
    }
    else {
        pel("Error: \"" <<
            Prefs.getInputFile() <<
            "\" does not exist or is invalid or is in accessible."
        );
        Prefs.addError("inp-non");
        return;
    }

    // 

    Translator::inputFile.open(Prefs.getInputFile());
    Translator::outputFile.open(Prefs.getOutputFile());

    pl("Here!");

    if (
        std::filesystem::path(Prefs.getInputFile())
        .extension() == ".sfl"
    ) {
        Prefs.setFormat(Preferences::sfl);
        Translator::sfl();
    }
    else if (
        std::filesystem::path(Prefs.getInputFile())
        .extension() == ".zap"
    ) {
        Prefs.setFormat(Preferences::Format::zap_org_out);
    }
    else if (
        std::filesystem::path(Prefs.getInputFile())
        .extension() == ".psf"
    ) {
        Prefs.setFormat(Preferences::Format::raw_psf);
    }
    else {
        pel("Unrecognized input format!");
        return;
    }
}