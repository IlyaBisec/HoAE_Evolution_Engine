#include "StdAfx.h"
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

static char FXML_message[1024];
int FXML_ERROR(char *fmt, ...)
{
   va_list argptr;
   int cnt;

   va_start(argptr, fmt);
   cnt = vsprintf(FXML_message, fmt, argptr);
   va_end(argptr);

   printf(FXML_message);

   return cnt;
}

