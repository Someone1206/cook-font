#include <iterm.hpp>
#include <fio.hpp>
#include <version>

#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cstdarg>
#include <filesystem>

std::string trim(const std::string& s)
{
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}


void inputToCommands(std::string& input, std::vector<std::string>& commands) {
    commands.clear();
    uint64_t reserveLimit = 10; // 10 commands...
    commands.reserve(reserveLimit);

    size_t i = 0, inpLen = input.length(), fstChar = 0;
    for (; i < inpLen; i++) {
        if (input.at(i) == ' ') {
            commands.push_back(input.substr(fstChar, i - fstChar));
            i++;
            while (input.at(i) == ' ') {
                i++;
            }
            fstChar = i;
            if (commands.size() >= reserveLimit) {
                reserveLimit += reserveLimit;
                commands.reserve(reserveLimit);
            }
        }
    }
    commands.push_back(input.substr(fstChar, i - fstChar));
}


bool matches(std::string& var, int num, ...) {
    va_list choices;
    va_start(choices, num);

    bool retVal = false;

    for (int i = 0; i < num; i++) {
        if (var == va_arg(choices, const char*)) {
            retVal = true;
        }
    }

    va_end(choices);
    return retVal;
}


void iterm(Preferences& prefs) {
    std::string input, prompt = std::filesystem::current_path().string();

    std::vector<std::string> cmds;

    pl("You've discovered the unresponsive, useless and stupid interactive terminal.");
    pl("Type \"help\" to show help prompt or \"quit\" to quit this crap");
    while (true) {
        p(prompt << " >  ");
        getline(std::cin, input);
        input = trim(input);

        if (input == "") {}
        else if (matches(input, 2, "h", "help")) {
            pl("Help prompt:");
            pl("Usage: <commands>");
            pl("h, help          to show this help prompt");
            pl("i, in, input     to specify the input file");
            pl("o, out, output   to specify the output file");
            pl("s, set           to set some attribute.");
            pl("cl, clear        to clear some atttribute.");
            pl("      List of available attributes:");
            pl("  q, quiet       to suppress useless error logs");
            pl("  p, cpu, process");
            pl("                 to make the output more CPU/processor intensive");
            pl("  ufd, use-font-desc");
            pl("                 to use the font description in the font file");
            pl("  c, chi, char-info");
            pl("                 to use character index information in the header file");
            pl("  i, in, input   <input_file>");
            pl("                 the same as \"i\" command(s)");
            pl("  o, out, output <output_file>");
            pl("                 the same as \"o\" command(s)");
            pl("info, reflect    to display all the attributes set and/or errors occurred");
            pl("r, run           to run with all the attribute given");
            pl("q, quit          to quit this crap");
            pl("");
        }
        else if (matches(input, 2, "info", "reflect")) {
            prefs.Reflect();
        }
        else if (matches(input, 2, "r", "run")) {
            return;
        }
        else if (matches(input, 2, "q", "quit")) {
            prefs.setInstaQuit();
            return;
        }
        else if (matches(input, 3, "v", "ver", "-version")) {
            pl("cook-font version: " << VERSION);
        }
        else {
            inputToCommands(input, cmds);
            
            if (matches(cmds[0], 3, "i", "in", "input")) {
            ___INPUTMGR:
                for (auto& name : cmds) {
                    prefs.setInputFile(name);
                }
            }
            else if (matches(cmds[0], 3, "o", "out", "output")) {
            ___OUTPUTMGR:
                for (auto& name : cmds) {
                    prefs.setOutputFile(name);
                }
            }
            else if (matches(cmds[0], 2, "s", "set")) {
                
                if (cmds.size() == 1) {
                ___NOATTR:
                    pel("Error: No attributes specified!");
                }
                else {
                    if (matches(cmds[1], 3, "i", "in", "input")) {
                        goto ___INPUTMGR;
                    }
                    else if (matches(cmds[1], 3, "o", "out", "output")) {
                        goto ___OUTPUTMGR;
                    }
                    else {
                        if (cmds.size() > 2) {
                            goto ___INVALID_ATTR;
                        }
                        if (matches(cmds[1], 2, "q", "quiet")) {
                            prefs.setLogSuppress();
                        }
                        else if (matches(cmds[1], 3, "p", "cpu", "process")) {
                            prefs.setUseCPU();
                        }
                        else if (matches(cmds[1], 2, "ufd", "use-font-desc")) {
                            prefs.setFontComments();
                        }
                        else if(matches(cmds[1], 3, "c", "chi", "char-info")) {
                            prefs.setCharInfo();
                        }
                        else {
                        ___INVALID_ATTR:
                            pel("Error: Invalid Attribute specified!");
                        }
                    }
                }
            }
            else if (matches(cmds[0], 2, "cl", "clear")) {
                if (cmds.size() == 1) {
                    goto ___NOATTR;
                }
                else {
                    if (matches(cmds[1], 3, "i", "in", "input")) {
                        if (cmds.size() > 2) {
                            pl("Warning: will erase input file.");
                        }
                        prefs.clearInputFile();
                    }
                    else if (matches(cmds[1], 3, "o", "out", "output")) {
                        if (cmds.size() > 2) {
                            pl("Warning: will erase output file.");
                        }
                        prefs.clearOutputFile();
                    }
                    else {
                        if (cmds.size() > 2) {
                            goto ___INVALID_ATTR;
                        }
                        if (matches(cmds[1], 2, "q", "quiet")) {
                            prefs.clearLogSuppress();
                        }
                        else if (matches(cmds[1], 3, "p", "cpu", "process")) {
                            prefs.clearUseCPU();
                        }
                        else if (matches(cmds[1], 2, "ufd", "use-font-desc")) {
                            prefs.clearFontComments();
                        }
                        else if (matches(cmds[1], 3, "c", "chi", "char-info")) {
                            prefs.clearCharInfo();
                        }
                        else {
                            goto ___INVALID_ATTR;
                        }
                    }
                }
            }
            else {
            ___INVALID_COMMAND:
                pel("Error: Invalid command!");
            }
        }
    }
}