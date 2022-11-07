#include "Akinator.h"
#include "AkinatorUtils.h"

#include "Settings.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "StringsUtils.h"
#include "TextReader.h"
#include "ColorOutput.h"
#include "ErrorHandler.h"
#include "Assert.h"

#pragma GCC diagnostic ignored "-Wformat-security"

enum Choices {
  GUESS,
  DETERMINE,
  DIFFERENCE,
  SHOW,
  QUIT,
  CHANGE_SAVE,
};

db::ResourceBundle Bundle = {};

const int MAX_ANSWER_SIZE = 10;

static void destroy();

static bool loadTree(Tree *tree);

static bool saveTree(const Tree *tree);

static int menu(bool needSave);

static bool checkMenuAnswer(const char *answer, bool needSave);

static int translateAnswer(const char *answer);

int init()
{
  Settings settings = {};

  getSettings(&settings);

  initAudio(&settings);

  int errorCode = 0;

  db::getBundle(&Bundle, "messages", settings.locale, &errorCode);

  srand(static_cast<unsigned>(time(0)));

  atexit(destroy);

  return errorCode;
}

static void destroy()
{
  db::destroyBundle(&Bundle);
}

void start()
{
  Tree tree = {};

  createTree(&tree);

  if (!loadTree(&tree))
    {
      destroyTree(&tree);

      return;
    }

  bool needSave = true;

  int keepLoop = true;

  while (keepLoop)
    {
      switch (menu(needSave))
        {
        case GUESS:       guess(&tree);          break;
        case DETERMINE:   determine(&tree);      break;
        case DIFFERENCE:  difference(&tree);     break;
        case SHOW:        show(&tree);           break;
        case CHANGE_SAVE: needSave = !needSave;  break;
        case QUIT:
          {
            audioPrintf("%s\n", db::getString(&Bundle, "goodbye"));

            keepLoop = false;

            break;
          }
        default:
          {
            handleError(db::getString(&Bundle, "error.unexpected"));

            keepLoop = false;
            needSave = false;

            break;
          }
        }
    }

  if (needSave)
    saveTree(&tree);

  destroyTree(&tree);
}

static bool loadTree(Tree *tree)
{
  assert(tree);

  Settings settings = {};

  getSettings(&settings);

  FILE *source = fopen(settings.source, "r");

  if (!source)
    return false;

  int error = 0;

  loadTree(tree, source, &error);

  fclose(source);

  if (error)
    return false;

  return true;
}

static bool saveTree(const Tree *tree)
{
  assert(tree);

  Settings settings = {};

  getSettings(&settings);

  FILE *target = fopen(settings.target, "w");

  if (!target)
    return false;

  int error = 0;

  saveTree(tree, target, &error);

  fclose(target);

  if (error)
    return false;

  return true;
}

static int menu(bool needSave)
{
  printf(ITALIC);
  audioPrintf("%s\n", db::getString(&Bundle, needSave ? "menu.statusPositive" :
                                                   "menu.statusNegative"));
  printf(RESET);
  audioPrintf(db::getString(&Bundle, "menu.choice"));
  audioPrintf("%s\n", db::getString(&Bundle, needSave ? "menu.choiceSavePositive" :
                                                        "menu.choiceSaveNegative"));
  audioPrintf(db::getString(&Bundle, "menu.input"));
  printf(BOLD);

  char answer[10] = "";

  while (scanf(" %s", answer) != 1 ||
         !checkMenuAnswer(answer, needSave))
    {
      while (getchar() != '\n')
        continue;

      printf(RESET);
      audioPrintf(db::getString(&Bundle, "input.incorrect"));
      printf(BOLD);
    }

  while (getchar() != '\n')
    continue;

  printf(RESET);

  return translateAnswer(answer);
}

static bool checkMenuAnswer(const char *answer, bool needSave)
{
  assert(answer);

  return !stricmp(db::getString(&Bundle, "action.guess"),      answer) ||
         !stricmp(db::getString(&Bundle, "action.determine"),  answer) ||
         !stricmp(db::getString(&Bundle, "action.difference"), answer) ||
         !stricmp(db::getString(&Bundle, "action.show"),       answer) ||
         !stricmp(db::getString(&Bundle, "action.quit"),       answer) ||
     (
        (!needSave && !stricmp(db::getString(&Bundle, "action.save"),   answer)) ||
        ( needSave && !stricmp(db::getString(&Bundle, "action.unsave"), answer))
     );
}

static int translateAnswer(const char *answer)
{
  if (!stricmp(db::getString(&Bundle, "action.guess"), answer))
    return GUESS;
  else if (!stricmp(db::getString(&Bundle, "action.determine"), answer))
    return DETERMINE;
  else if (!stricmp(db::getString(&Bundle, "action.difference"), answer))
    return DIFFERENCE;
  else if (!stricmp(db::getString(&Bundle, "action.show"), answer))
    return SHOW;
  else if (!stricmp(db::getString(&Bundle, "action.quit"), answer))
    return QUIT;
  else if (!stricmp(db::getString(&Bundle, "action.save"),   answer) ||
           !stricmp(db::getString(&Bundle, "action.unsave"), answer))
    return CHANGE_SAVE;
  else
    {
      handleError(db::getString(&Bundle, "error.unexpected"));

      return QUIT;
    }
}
