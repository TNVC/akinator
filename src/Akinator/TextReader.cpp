#include "TextReader.h"

#include <festival/festival.h>

#include <locale.h>
#include <stdio.h>
#include <ctype.h>
#include "Assert.h"

//#include <stddef.h>
#include <wchar.h>
#include <wctype.h>

static int HasVoice = false;
static db::Locale Locale = db::Locale::EN;

static int printAlpha(char *target, const char *source);

void initAudio(const Settings *settings)
{
  assert(settings);

  HasVoice = settings->hasVoice;

  Locale = settings->locale;

  //EST_Wave wave;
  //  int heap_size = 21000;
  //  int load_init_files = 1;

  //  festival_initialize(1, DEFAULT_HEAP_SIZE);

  //  festival_eval_command("(voice_ked_diphone)");
  //festival_say_text("hello world");

  //festival_text_to_wave("hello world",wave);
  //wave.save("/tmp/wave.wav","riff");

  // festival_say_file puts the system in async mode so we better
  // wait for the spooler to reach the last waveform before exiting
  // This isn't necessary if only festival_say_text is being used (and
  // your own wave playing stuff)
  //  festival_wait_for_spooler();
  //  festival_tidy_up();
}

int audioPrintf(const char *format, ...)
{
  static char buffer[MAX_MESSAGE_SIZE] = "";

  va_list args = {};

  va_start(args, format);

  int result = vsprintf(buffer, format, args);

  va_end(args);

  printf("%s", buffer);

  if (HasVoice && Locale != db::Locale::PL)
    {
      static char cmdBuffer[MAX_MESSAGE_SIZE + 80] = "";

      int offset  = sprintf(cmdBuffer, "echo \"");
          offset += printAlpha(cmdBuffer + offset , buffer);


          sprintf(cmdBuffer + offset, "\" | festival --tts --language %s",
                        (Locale == db::Locale::RU ? "russian" : "english"));

          //          printf("\033[31m%s\033[0m\n", cmdBuffer);////

          system(cmdBuffer);
    }

  return result;
}

static int printAlpha(char *target, const char *source)
{
  assert(target);
  assert(source);

  int firstIndex  = 0;
  int secondIndex = 0;

  for ( ; source[secondIndex]; ++secondIndex)
    {
      if (source[secondIndex] == '/')
        target[firstIndex++] = ' ';

      if (ispunct(source[secondIndex]))// && source[secondIndex] != '/')
        continue;

      target[firstIndex++] = source[secondIndex];
    }

  target[firstIndex] = source[secondIndex];

  return firstIndex;
}
