#include <prefs.hpp>
#include <fio.hpp>
#include <version>

#include <cstring>

#define out_len 9

#define HEREPU() \
pl("Help prompt:"); \
pl("Usage: make-font [<flags>] <input file>"); \
pl("-h, --help        to show this help prompt"); \
pl("-o, --output=<file>"); \
pl("                  to specify the output file"); \
pl("-v, --ver, --version"); \
pl("                  to show the version number"); \
pl("-q, --quiet       to suppress useless log messages"); \
pl("-Q, --no-quiet    to show useless log messages"); \
pl("-p, --cpu-i       to make the output CPU/process intensive"); \
pl("-P, --no-cpu-i    to make the output memory intensive (default)"); \
pl("-d, --font-desc   to include the font comments after the \"-\""); \
pl("                  in comments of the header"); \
pl("-D, --no-font-d   negate the previous one (DEFAULT)"); \
pl("-c, --char-info   to include information about character index"); \
pl("-C, --no-char-i   to negate the previous one (DEFAULT)"); \
pl("                  and recommended"); \
pl("Any other arguement will be interpreted as an input file."); \


Preferences::Preferences()
{
    boolPreferences.reset();
    inputFileName.clear();
    outputFileName.clear();
}



bool Preferences::isInstaQuit() {
    return boolPreferences[0];
}
void Preferences::setInstaQuit() {
    boolPreferences.set(0);
}
void Preferences::clearInstaQuit() {
    boolPreferences.reset(0);
}



bool Preferences::isLogSuppressed() {
    return boolPreferences[1];
}
void Preferences::setLogSuppress() {
    boolPreferences.set(1);
}
void Preferences::clearLogSuppress() {
    boolPreferences.reset(1);
}



bool Preferences::isCPU() {
    return boolPreferences[2];
}
void Preferences::setUseCPU() {
    boolPreferences.set(2);
    pl("Warning: This feature has not been implemented yet. Cuz I'm lazy");
}
void Preferences::clearUseCPU() {
    boolPreferences.reset(2);
}



bool Preferences::isFontComments() {
    return boolPreferences[3];
}
void Preferences::setFontComments() {
    boolPreferences.set(3);
}
void Preferences::clearFontComments() {
    boolPreferences.reset(3);
}



bool Preferences::isCharInfo() {
    return boolPreferences[4];
}
void Preferences::setCharInfo() {
    boolPreferences.set(4);
}
void Preferences::clearCharInfo() {
    boolPreferences.reset(4);
}



std::string Preferences::getInputFile() {
    return inputFileName;
}

void Preferences::setInputFile(std::string& fileName) {
    inputFileName = fileName;
}

void Preferences::setInputFile(const char* fileName) {
    inputFileName = fileName;
}

void Preferences::clearInputFile() {
    inputFileName.clear();
}



std::string Preferences::getOutputFile() {
    return outputFileName;
}

void Preferences::setOutputFile(std::string& fileName) {
    outputFileName = fileName;
}

void Preferences::setOutputFile(const char* fileName) {
    outputFileName = fileName;
}

void Preferences::clearOutputFile() {
    outputFileName.clear();
}



void Preferences::Reflect() {
    pl(std::boolalpha << "Suppress Logs:           " << isLogSuppressed());
    pl(std::boolalpha << "Output is CPU intensive: " << isCPU());
    pl(std::boolalpha << "Use font file comments:  " << isFontComments());
    pl(std::boolalpha << "Use char index in header:" << isCharInfo());

    pl("Input file name:         " << getInputFile());
    pl("Output file name:        " << getOutputFile());

    if (getErrors() != "") {
        pel("Errors Occurred:\n" << getErrors());
    }
}



std::string Preferences::getErrors() {
    return errors;
}

void Preferences::setError(std::string errorMsg) {
    errors = errorMsg;
}

void Preferences::addError(std::string errorMsg) {
    errors += "\n" + errorMsg;
}

void Preferences::clearErrors() {
    errors.clear();
}

void Preferences::ParseArgs(const char** args, int len) {
    const char* arg;
    // constexpr int out_len = strlen("--output=");


    for (int i = 1; i < len; i++) {
        arg = *(args + i);

        if (strcmp(arg, "-o") == 0)  {
            if ((i + 1) < len) {
                i++;
                setOutputFile(*(args + i));
            } else {
                __NOOUT:
                pel("Error: No output file specified!");
                setInstaQuit();
                return;
            }
        }
        else if (strncmp(arg, "--output=", out_len) == 0) {
            // patch 1 of ver 1.1
            arg += out_len;

            if (strcmp(arg, "") == 0) {
                goto __NOOUT; //
            } else {
                setOutputFile(arg);
            }
        }
        else if (
            strcmp(arg, "-h") == 0 ||
            strcmp(arg, "--help") == 0
        ) {
            HEREPU();
        }
        else if (
            strcmp(arg, "-v") == 0       ||
            strcmp(arg, "--ver") == 0    ||
            strcmp(arg, "--version") == 0
        ) {
            pl("cook-font: convert font bitmaps to C Headers");
            pl("Version: " << VERSION);
        }
        else if (
            strcmp(arg, "-d") == 0  ||
            strcmp(arg, "--font-desc") == 0
        ) {
            setFontComments();
        }
        else if (
            strcmp(arg, "-D") == 0   ||
            strcmp(arg, "--no-font-d") == 0
        ) {
            clearFontComments();
        }
        else if (
            strcmp(arg, "-q") == 0 ||
            strcmp(arg, "--quiet") == 0
        ) {
            setLogSuppress();
        }
        else if (
            strcmp(arg, "-Q") == 0 ||
            strcmp(arg, "--no-quiet") == 0
        ) {
            clearLogSuppress();
        }
        else if (
            strcmp(arg, "-p") == 0 ||
            strcmp(arg, "--cpu-i") == 0
        ) {
            setUseCPU();
        }
        else if (
            strcmp(arg, "-P") == 0 ||
            strcmp(arg, "--no-cpu-i") == 0
        ) {
            clearUseCPU();
        }
        else if (
            strcmp(arg, "-c") == 0    ||
            strcmp(arg, "--char-info") == 0
        ) {
            setCharInfo();
        }
        else if (
            strcmp(arg, "-C") == 0    ||
            strcmp(arg, "--no-char-i") == 0
        ) {
            clearCharInfo();
        }
        else {
            setInputFile(arg);
        }
    }

    if (getInputFile() == "" && getOutputFile() == "") {
        setInstaQuit();
    }
}




Preferences::~Preferences()
{
}
