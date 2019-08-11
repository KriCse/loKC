
#include <security/pam_appl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lock.h"
#include "pam_manager.h"
#include <unistd.h>
char* passwordBuffer;

int main(void) {
    pam_handle_t *handle = NULL;
    passwordBuffer = (char *) malloc(PASSWORD_BUFFER_SIZE);
    if(passwordBuffer == NULL){
        fprintf(stderr, "Cannot allocate passwordBuffer\n");
        return EXIT_FAILURE;
    }
    char username[32];
    int error  = getlogin_r(username, 32);
    if(error){
        return  0;
    }
    if (!startPam(&handle, username)) {
        fprintf(stderr, "Cannot start Pam");
        return EXIT_FAILURE;
    }

    Display *display;
    Window window;
    XSetWindowAttributes windowAttributes;
    int screenNumber;
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screenNumber = DefaultScreen(display);
    windowAttributes.background_pixel = WhitePixel(display, screenNumber);
    windowAttributes.border_pixel = BlackPixel(display, screenNumber);
    windowAttributes.override_redirect = 1;
    Window rootWindow = RootWindow(display, screenNumber);
    unsigned long valueMask = CWBackPixel | CWBorderPixel | CWOverrideRedirect;
    window = XCreateWindow(display, // display
                           rootWindow, // parent
                           50, //x
                           59, // y
                           1000, // w
                           1000, // h
                           1, //bw
                           DefaultDepth(display, screenNumber), //d
                           CopyFromParent, // class
                           DefaultVisual(display, rootWindow),
                           valueMask,
                           &windowAttributes);
    XMapRaised(display, window);
    long inputsToListenTo = ExposureMask | KeyPressMask | ButtonPressMask;
    XGrabKeyboard(display,
                  window,
                  0,
                  GrabModeAsync, GrabModeAsync, CurrentTime
    );
    printf("username %s\n", username);

    handleEvent(display, &handle);
    XSelectInput(display, window, inputsToListenTo);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}

int isKeyNotRelevant(KeySym sym) {
    return IsCursorKey(sym) ||
           IsFunctionKey(sym) ||
           IsKeypadKey(sym) ||
           IsModifierKey(sym) ||
           IsPFKey(sym) ||
           IsPrivateKeypadKey(sym);
}


int handleKeyPress(XKeyEvent *keyEvent,
                   int *cursor
) {
    char keyBuffer[16];
    KeySym keySymbol;
    int numberOfChars = XLookupString(keyEvent,
                                      keyBuffer,
                                      sizeof(keyBuffer),
                                      &keySymbol, 0);
    if (isKeyNotRelevant(keySymbol)) {
        return 0;
    }
    int enterPressed = 0;
    switch (keySymbol) {
        case XK_Return:
            passwordBuffer[*cursor] = '\0';
            enterPressed = 1;
            break;
        case XK_BackSpace:
            *cursor = *cursor - 1;
            break;
        default:
            // PASSWORD_BUFFER_SIZE - 1 -> Room for \0
            if ((numberOfChars + *cursor) > PASSWORD_BUFFER_SIZE - 1) {
                printf("Waring: No more room in PasswordBuffer!\n");
            } else {
                strncpy(passwordBuffer + *cursor,
                        keyBuffer,
                        numberOfChars);
                *cursor = (*cursor + numberOfChars);
            }

    }
    return enterPressed;
}

void handleEvent(Display *display, pam_handle_t** handle) {
    XEvent event;
    int running = 1;
    int cursor = 0;
    int enterPressed = 0;

    while (running) {
        enterPressed = 0;
        XNextEvent(display, &event);
        switch (event.type) {
            case KeyPress:
                enterPressed = handleKeyPress(&event.xkey,
                                              &cursor);
                break;
            case ButtonPress:
                running = 0;
                break;
        }
        if (enterPressed) {
            if (isPasswordCorrect(handle)) {
                printf("Password Correct \n");
                running = 0;
            } else {
                printf("Password Incorrect \n");
            }
            cursor = 0;
        }
    }
}


