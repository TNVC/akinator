#pragma once

#include <stdarg.h>
#include "Settings.h"

#define MAX_MESSAGE_SIZE 256

void initAudio(const Settings *settings);

int audioPrintf(const char *format, ...);
