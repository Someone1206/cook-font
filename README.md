# cook-font
`cook-font` is a font file to C header converter.  
Written in C++ 20, the source code tries to provide readability with speed and efficiency.  
Currently this program supports reading files of only [one syntax](#the-file-syntax) but more may be added later.

## The Constitution
The constituion of this repository is the [CONTRIBUTING.md](./CONTRIBUTING.md).

## Usage
```
cook-font --help
cook-font font.txt -o font.h
```
**List of Flags**
|Flag|Purpose|
|----|-------|
|`-h` OR `--help`|Shows a help prompt with all the available flags|
|`-v` OR `--ver` OR `--version`|Shows the version number|
|`-o` OR `--output=`|Specify the output file name|
|`-q` OR `--quiet`|Suppress logs|
|`-Q` OR `--no-quiet`| Show Logs. Negate the effects of `-q`|
|`-p` OR `--cpu-i`|Generate Output which will be CPU/Processor Intensive to interpret and use.<br>**Not Yet Implemented**|
|`-P` OR `--no-cpu-i`|Generate Output which will be memory intensive to use.<br>**Implemented and is the DEFAULT Output mode**|
|`-d` OR `--font-desc`|to include the font comments after the "-"|
|`-D` OR `--no-font-d`|to exclude the font comments after the "-"<br>**DEFULT**|
|`-c` OR `--char-info`|to include information about character index in the output header file|
|`-C` OR `--no-char-i`|to exclude information about character index in the output header file<br>**DEFAULT**|
|**Any Other Arguement**|Is interpreted as the input file|


## For Interested Developers
* The program has its starting point in file the [cook-font.cpp](./cook-font.cpp) in the `int main(int argc, const char* argv[])` function.
* [inc/prefs.hpp](./inc/prefs.hpp) and [src/prefs.cpp](./src/prefs.cpp) contain `Preferences` class which defines and stores the user preferences. It has methods to set, clear and access them.
* [inc/fio.hpp](./inc/fio.hpp) has some basic IO macros.
* [inc/version](./inc/version) stores the version.
* [inc/errMgr.hpp](./inc/errMgr.hpp) has the macro for error handling.
* [src/translator_core.cpp](./src/translator_core.cpp) has the core translation unit.
* [src/translator_main.cpp](./src/translator_main.cpp) has the font identification, macro creation units and calls translate_core internally.




## The file syntax
The program currently reads files with the following syntax.  
The extension or name does not matter but show have the following syntax.
1. The file **MUST** have the following line as its **FIRST LINE**.
   ```
   font-<font_width>x<font_height>-<font_type>
   ```
   Some Valid examples are:
   ```
   font-8x18-psf1
   ```
   ```
   font-6x10-psf1
   ```
2. The second line **MUST** start with `-`. `-` also marks the start/end of a glyph and also acts as a comment.
   Some Examples:
   ```
   -
   ```
   ```
   - This is a comment but will be written in the header
   ```
   But this comment **WILL BE IGNORED**
3. Write 1's and 0's according to the width and height.
   ```
   00011000
   01100110
   01100110
   01111110
   01100110
   01100110
   - This is an A ;)
   ```
   Will be valid for a font whose width is specified as 8 and height as 6, otherwise it is regarded as a syntax error.
A Sample may be:
```
font-8x6-psf1
-
00011000
01100110
01100110
01111110
01100110
01100110
- There will be many more glyphs
```

## Tests & Examples
The tests are stored in the [tests](./tests/) folder. The tests also act as examples to the files this program can currently read. They are arranged in sets of two, one having `.sfl` extension and the other having `.h` extension.  
Command used:
```
cook-font <filename>.sfl -o <filename>.h -d -c
```