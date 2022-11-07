#include "TextReader.h"

#include <festival/festival.h>

#include <locale.h>
#include <stdio.h>
#include <ctype.h>
#include "Assert.h"

//#include <stddef.h>
#include <wchar.h>
#include <wctype.h>

#pragma GCC diagnostic ignored "-Wcast-qual"

const int CMD_SIZE = 80;

static int HasVoice = false;
static db::Locale Locale = db::Locale::EN;

static int sayText(const char *format, va_list args);

static void saySentence(const char *sentence);

static char *strchrs(const char *string, const char *chars, int number = 1);

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
  assert(format);

  va_list args = {};

  va_start(args, format);

  int result = 0;

  if (HasVoice)
    result = sayText(format, args);
  else
    result = vprintf(format, args);

  va_end(args);

  return result;
}

static int sayText(const char *format, va_list args)
{
  if (Locale == db::Locale::PL)
    return vprintf(format, args);

  static char buffer   [MAX_MESSAGE_SIZE           ] = "";

  int result = vsprintf(buffer, format, args);

  char *wordStart = buffer;
  char *wordEnd   = strchrs(wordStart, " \t\n/", 2);

  for ( ; wordEnd; wordEnd = strchrs(wordStart, " \t\n/", 2))
    {
      char temp = wordEnd[1];
      wordEnd[1] = '\0';

      printf("%s", wordStart);

      fflush(stdout);

      saySentence(wordStart);

      wordEnd[1] = temp;

      wordStart = wordEnd + 1;
    }

  if (*wordStart != '\0')
    {
      printf("%s", wordStart);

      fflush(stdout);

      saySentence(wordStart);
    }

  return result;
}

static void saySentence(const char *sentence)
{
  assert(sentence);

  static char cmdBuffer[MAX_MESSAGE_SIZE + CMD_SIZE] = "";

  int offset  = sprintf(cmdBuffer, "echo \"");
  offset += printAlpha(cmdBuffer + offset, sentence);


  sprintf(cmdBuffer + offset, "\" | festival --tts --language %s",
          (Locale == db::Locale::RU ? "russian" : "english"));

  //          printf("\033[31m%s\033[0m\n", cmdBuffer);////

  system(cmdBuffer);
}

static char *strchrs(const char *string, const char *chars, int number)
{
  assert(string);
  assert(chars);
  assert(number > 0);

  int i = 0;
  for ( ; string[i]; ++i)
      if (strchr(chars, string[i]))
        {
          if (!--number)
            return (char *)(string + i);
        }

  return nullptr;
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

      if (ispunct(source[secondIndex]))
        continue;

      target[firstIndex++] = source[secondIndex];
    }

  target[firstIndex] = source[secondIndex];

  return firstIndex;
}
