#include <translator.hpp>
#include <prefs.hpp>
#include <errMgr.hpp>


#define PSF1WIDTH     8 // dats constant

int Translator::rawpsf() {

    uint8_t bytesRead;
    char* buffer = new char[4];

    // read the first 4 bytes
    Translator::inputFile.open(Prefs.getInputFile(), std::ifstream::binary);
    Translator::inputFile.read(buffer, 4);
    bytesRead = (uint8_t)Translator::inputFile.gcount(); // that is 4, I hope so

    if (bytesRead < 4) {
        pel(
            "Error! " <<
            Prefs.getInputFile() << " is smaller than 4 bytes!"
        );
        return -99999; // wtaf?!
    }

    int retVal;

    if ((buffer[0] == 0x36 && buffer[1] == 0x04)) {
        retVal = Translator::read_psf1(buffer[2], buffer[3]);
    } else if (buffer[0] == 0x72 && buffer[1] == 0xb5) {
        retVal = Translator::read_psf2(buffer);
    } else {
        pel("Error: " << Prefs.getInputFile() << " is not a valid PSF font file.");
        return -0x999;
    }
    
    return 0;
}