#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lock.h"

int main(void)
{
    Display *display;
    Window window;
    XEvent event;
    XSetWindowAttributes windowAttributes;
    char *msg = "Hello, World!";
    int s;

    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    s = DefaultScreen(display);
    windowAttributes.background_pixel = WhitePixel(display, s);
    windowAttributes.border_pixel = BlackPixel(display, s);
    windowAttributes.override_redirect = 1;
    Window rootWindow = RootWindow(display, s);
    unsigned long valueMask = CWBackPixel | CWBorderPixel | CWOverrideRedirect;
    window = XCreateWindow(display, // display
                           rootWindow, // parent
                           50, //x
                           59, // y
                           1000, // w
                           1000, // h
                           1, //bw
                           DefaultDepth(display, s), //d
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
    handleEvent(display);
    XSelectInput(display, window, inputsToListenTo);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
int isKeyNotRelevant(KeySym sym){
    return IsCursorKey(sym) ||
           IsFunctionKey(sym) ||
           IsKeypadKey(sym) ||
           IsModifierKey(sym) ||
           IsPFKey(sym) ||
           IsPrivateKeypadKey(sym);
}


int handleKeyPress(XKeyEvent *keyEvent) {
    char keyBuffer[16];
    KeySym keySymbol;
    int returnValue = XLookupString(keyEvent,
            keyBuffer,
            sizeof(keyBuffer),
            &keySymbol, 0);
    if(isKeyNotRelevant(keySymbol)){
        return 0;
    }
    int enterPressed = 0;
    switch (keySymbol) {
        case XK_Return:
            printf("HI\n");
            enterPressed = 1;
            break;
    }
    return enterPressed;
}


void handleEvent(Display *display){
    XEvent event;
    int running = 1;
    int enterPressed = 0;
    while(running) {
        XNextEvent(display, &event);
        enterPressed = 0;
        switch (event.type) {
            case KeyPress:
                enterPressed = handleKeyPress(&event.xkey);
                break;
            case ButtonPress:
                running = 0;
                break;
        }
        if(enterPressed){
            printf("Enter Pressed\n");
        }
    }
}

