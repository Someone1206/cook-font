#pragma once

#include <string>
#include <bitset>


class Preferences
{
public:
    enum Format {
        sfl,
        zap_org_out,
        raw_psf
    };

private:
    std::bitset<8> boolPreferences;
    /*********
     * 0 - instant quit
     * 1 - -q, -Q - log suppress
     * 2 - -p, -P - use cpu
     * 3 - -d, -D - use font file comments
     * 4 - -c, -C - use char info in comments
     * *******/


    std::string inputFileName, outputFileName;

    std::string errors;

    enum Format format;

public:
    Preferences();
    /************
     * Public functions
     * 
     * *****/

    bool isInstaQuit();
    void setInstaQuit();
    void clearInstaQuit();

    bool isLogSuppressed();
    void setLogSuppress();
    void clearLogSuppress();

    bool isCPU();
    void setUseCPU();
    void clearUseCPU();

    bool isFontComments();
    void setFontComments();
    void clearFontComments();

    bool isCharInfo();
    void setCharInfo();
    void clearCharInfo();

    std::string getInputFile();
    void setInputFile(std::string& fileName);
    void setInputFile(const char* fileName);
    void clearInputFile();


    std::string getOutputFile();
    void setOutputFile(std::string& fileName);
    void setOutputFile(const char* fileName);
    void clearOutputFile();

    std::string getErrors();
    void setError(std::string errorMsg);
    void addError(std::string errorMsg);
    void clearErrors();

    void setFormat(enum Preferences::Format fmt);
    enum Preferences::Format getFormat();

    void ParseArgs(const char** args, int len);


    void Reflect();

    ~Preferences();
};


extern Preferences Prefs;