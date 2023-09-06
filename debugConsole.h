#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define AC_RED "\x1b[32m"
/*
	Example: printf("%s", AC_RED); //https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
*/

void dcCreateConsole();
void dcFreeConsole();