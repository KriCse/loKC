//
// Created by kcserpes on 8/14/19.
//

#include <stdio.h>
#include <stdlib.h>
#include "lock_window.h"

Display* getDisplay() {
    Display* display = NULL;
    display = XOpenDisplay(NULL);
    if(display == NULL){
        fprintf(stderr, "Cannot Open display");
        exit(1);
    }
    return display;
}

Window createWindow(Display *display, int screenNumber) {
    XSetWindowAttributes windowAttributes;
    screenNumber = DefaultScreen(display);
    windowAttributes.background_pixel = WhitePixel(display, screenNumber);
    windowAttributes.border_pixel = BlackPixel(display, screenNumber);
    windowAttributes.override_redirect = 1;
    Window rootWindow = RootWindow(display, screenNumber);
    unsigned long valueMask = CWBackPixel | CWBorderPixel | CWOverrideRedirect;
    return XCreateWindow(display, // display
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
}
