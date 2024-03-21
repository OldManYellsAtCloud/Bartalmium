#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>

#define DECIMAL0 0x1
#define DECIMAL1 0x2
#define DECIMAL2 0x4
#define DECIMAL3 0x8
#define SEMICOLON 0x10
#define APOSTROPHE 0x20

#define CLEAR_DISPLAY 0x76
#define SET_BRIGHTNESS 0x7a
#define SET_DECIMAL 0x77

void clearDisplay(int terminalFd){
    const char c[] = {CLEAR_DISPLAY};
    write(terminalFd, c, 1);
}

void setBrightness(int terminalFd, std::string brightness){
    int i;
    try {
        i = std::stoi(brightness);
    } catch (std::exception e) {
        fprintf(stderr, "Could not convert brightness to number: %s\n", e.what());
        exit(1);
    }

    const char c[] = {SET_BRIGHTNESS, (char)i};
    write(terminalFd, c, 2);
}

void setDecimal(int terminalFd, int decimalPositions){
    const char c[] = {SET_DECIMAL, (char)decimalPositions};
    write(terminalFd, c, 2);
}

void setTerminal(int terminalFd){
    struct termios settings;
    int res;

    res = tcgetattr(terminalFd, &settings);
    if (res != 0) {
        std::fprintf(stderr, "Could not get terminal settings!\n");
        exit(1);
    }

    res = cfsetspeed(&settings, B9600);
    if (res != 0) {
        std::fprintf(stderr, "Could not set terminal speed!\n");
        exit(1);
    }

    res = tcsetattr(terminalFd, TCSANOW, &settings);
    if (res != 0) {
        std::fprintf(stderr, "Could not apply new terminal settings!\n");
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    int opt;
    std::string serialPath, textToSet, s_brightness;
    bool doTerminalConfig = true;
    bool shouldClearDisplay = false;
    uint8_t decimalPoint = 0;
    /*
     * -s serialpath
     * -t text
     * -k (skip terminal config)
     * -b brightness
     * -c (clear display)
     *
     * -d activate decimalpoint, 0-3, can be repeated
     * -e (activate semicolon)
     * -a (activate apostrophe)
     *
     */
    while ((opt = getopt(argc, argv, "s:t:b:kcd:ae")) != -1){
        switch (opt) {
        case 's':
            serialPath = optarg;
            break;
        case 't':
            textToSet = optarg;
            break;
        case 'k':
            doTerminalConfig = false;
            break;
        case 'b':
            s_brightness = optarg;
            break;
        case 'c':
            shouldClearDisplay = true;
            break;
        case 'e':
            decimalPoint |= SEMICOLON;
            break;
        case 'a':
            decimalPoint |= APOSTROPHE;
            break;
        case 'd':
            switch(optarg[0]){
            case '0':
                decimalPoint |= DECIMAL0;
                break;
            case '1':
                decimalPoint |= DECIMAL1;
                break;
            case '2':
                decimalPoint |= DECIMAL2;
                break;
            case '3':
                decimalPoint |= DECIMAL3;
                break;
            }
        }
    }

    if (serialPath.empty()){
        std::fprintf(stderr, "-s serial path and -t text arguments are mandatory\n");
        exit(1);
    }

    if (textToSet.length() > 4){
        std::fprintf(stderr, "Text should be maximum 4 characters long");
        exit(1);
    }

    int displayFd = open(serialPath.c_str(), O_WRONLY);
    if (!isatty(displayFd)){
        std::fprintf(stderr, "%s is not a serial terminal!\n", serialPath.c_str());
        exit(1);
    }

    if (doTerminalConfig)
        setTerminal(displayFd);

    if (shouldClearDisplay){
        clearDisplay(displayFd);
    }

    if (!s_brightness.empty()){
        setBrightness(displayFd, s_brightness);
    }

    if (!textToSet.empty()){
        write(displayFd, textToSet.c_str(), textToSet.size());
    }

    setDecimal(displayFd, decimalPoint);

    return 0;
}
