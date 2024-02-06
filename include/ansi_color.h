#ifndef __ANSI_COLOR_H__
#define __ANSI_COLOR_H__
#include <stdio.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#define _SGR_RESET      "\e[m"
#define _SGR_BOLD       "\e[1m"
#define _SGR_ITALIC     "\e[3m"
//////////////////////////
#define _SGR_REDF       "\e[31m"
#define _SGR_GREENF     "\e[32m"
#define _SGR_YELLOWF    "\e[33m"
#define _SGR_BLUEF      "\e[34m"
#define _SGR_MAGENF     "\e[35m"
#define _SGR_CYANF      "\e[36m"
void prepareANSI(void);

#endif