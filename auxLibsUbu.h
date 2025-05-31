#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>

int setEchoMode(bool enable)
    {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~ICANON;
        if (enable)
            newt.c_lflag |= ECHO;
        else
            newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }

    int getch()
    {
        return setEchoMode(false);
    }

void cls() {
    system("clear");
}