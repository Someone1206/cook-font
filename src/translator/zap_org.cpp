#include <translator.hpp>
#include <prefs.hpp>

#define GETLINE() getline(Translator::inputFile, line)

struct BufferStateZAP {
    bool glyphComplete;
};

int parseBuffer(char* buffer, uint64_t len, BufferStateZAP* state);

int Translator::zap_org() {
    //
    Translator::inputFile.open(Prefs.getInputFile(), std::ifstream::binary);

    uint64_t bytesRead, len = 0x1000;
    char* buffer = new char[len];

    //
    BufferStateZAP bufferState;

    while (!Translator::inputFile.eof()) {
        Translator::inputFile.read(buffer, len);
        bytesRead = Translator::inputFile.gcount();

        if (parseBuffer(buffer, len, &bufferState) != 0) {
            // deal with it...
        }

    }


    delete[] buffer;

    std::string line;

    GETLINE();
    if (line[0] == '#') {
        
    }


    while (getline(Translator::inputFile, line)) {
        //
    }

    return 0;
}

int parseBuffer(char* buffer, uint64_t len, BufferStateZAP* state) {
    for (uint64_t i = 0; i < len; i++) {
        //
    }

    return 0;
}