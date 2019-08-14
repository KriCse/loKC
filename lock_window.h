//
// Created by kcserpes on 8/14/19.
//

#ifndef LOKC_LOCK_WINDOW_H
#define LOKC_LOCK_WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
Display* getDisplay();
Window createWindow(Display *display, int screenNumber);
#endif //LOKC_LOCK_WINDOW_H
