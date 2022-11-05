#include "TextReader.h"

#include <stdio.h>
#include "Assert.h"

static int hasVoice = false;

void initAudio(const Settings *settings)
{
  assert(settings);

  hasVoice = settings->hasVoice;
}

int audioPrintf(const char *format, ...)
{
  static char buffer[MAX_MESSAGE_SIZE] = "";

  //static char cmdBuffer[MAX_MESSAGE_SIZE + 80] = "";

  va_list args = {};

  va_start(args, format);

  int result = vsprintf(buffer, format, args);

  va_end(args);

  printf("%s", buffer);

  //sprintf(cmdBuffer, "echo \"%s\" | festival --tts", buffer);

  //system(cmdBuffer);

  return result;
}
