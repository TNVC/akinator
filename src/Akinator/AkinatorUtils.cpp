#include "AkinatorUtils.h"
#include "TreeDump.h"

#include <stdlib.h>
#include <ctype.h>
#include "Settings.h"
#include "ColorOutput.h"
#include "StringsUtils.h"
#include "Assert.h"

#pragma GCC diagnostic ignored "-Wformat-security"

const int MAX_ANSWER_SIZE = 10;

const int MAX_CMD_SIZE    = 64;

static int readAnswer();

static bool checkAnswer(const char *answer);

static void addObject(const Tree *tree, Node *node);

static void printNode(const Node *node, int isRight);

static void printNegative();

void guess(Tree *tree)
{
  assert(tree);

  Node *current = tree->root;

  if (!current)
    {
      printf(FG_BRIGHT_BLUE);
      printf(db::getString(&Bundle, "answer.empty"));
      printf(db::getString(&Bundle, "answer"));
      printf(RESET);

      if (!readAnswer())
        addObject(tree, tree->root);
    }

  while (current->left)
    {
      printf(FG_BRIGHT_CYAN "%s", current->value);
      printf(db::getString(&Bundle, "answer"));
      printf(RESET);

      if (readAnswer())
        current = current->left;
      else
        current = current->right;
    }

  printf(FG_BRIGHT_CYAN);
  printf(db::getString(&Bundle, "answer.final"));
  printf("%s%s" RESET, current->value, db::getString(&Bundle, "answer"));

  if (!readAnswer())
    addObject(tree, current);
  else
    printf(FG_BRIGHT_CYAN "%s\n" RESET, db::getString(&Bundle, "guess.win"));
}

static int readAnswer()
{
  char answer[MAX_ANSWER_SIZE] = "";

  printf(BOLD);

  while (scanf(" %s", answer) != 1 || !checkAnswer(answer))
    {
      while (getchar() != '\n') continue;

      printf(RESET "%s" BOLD, db::getString(&Bundle, "input.incorrect"));
    }

  while (getchar() != '\n') continue;

  printf(RESET);

  return !stricmp(db::getString(&Bundle, "answer.positive"), answer);
}

static bool checkAnswer(const char *answer)
{
  assert(answer);

  return !stricmp(db::getString(&Bundle, "answer.positive"), answer) ||
         !stricmp(db::getString(&Bundle, "answer.negative"), answer);
}

static void addObject(const Tree *tree, Node *node)
{
  assert(tree);
  assert(node);

  printf(FG_BRIGHT_CYAN "%s" RESET BOLD, db::getString(&Bundle, "guess.newObject"));

  element_t buff = "";

  while (scanf(" %[^\n]", buff) != 1)
    {
      while (getchar() != '\n') continue;

      printf(RESET FG_BRIGHT_CYAN "%s" RESET BOLD, db::getString(&Bundle, "input.unkwownData"));
    }

  while (getchar() != '\n') continue;

  printf(RESET);

  if (findElement(tree, buff))
    {
      printf(FG_BRIGHT_CYAN "%s"  ITALIC "%s" RESET FG_BRIGHT_CYAN "!\n" RESET,
             db::getString(&Bundle, "input.copy"), buff);

      return;
    }

  Node *newNode = createNode(buff);

  printf(FG_BRIGHT_CYAN "%s" ITALIC "%s" RESET FG_BRIGHT_CYAN "%s"
         ITALIC "%s" RESET FG_BRIGHT_CYAN "?\n" RESET BOLD,
         db::getString(&Bundle, "guess.difference"), buff,
         db::getString(&Bundle, "guess.and"), node->value);

  while (scanf(" %[^\n]", buff) != 1)
    {
      while (getchar() != '\n') continue;

      printf(RESET FG_BRIGHT_CYAN "%s" RESET BOLD,  db::getString(&Bundle, "input.unkwownData"));
    }

  while (getchar() != '\n') continue;

  printf(RESET);

  Node *oldNode = createNode(node->value);

  strncpy(node->value, buff, MAX_SIZE);
  setParent(newNode, node, true);
  setParent(oldNode, node, false);

  printf(RESET FG_BRIGHT_CYAN "%s\n" RESET, db::getString(&Bundle, "guess.fail"));
}

void determine(const Tree *tree)
{
  printf(FG_BRIGHT_GREEN "%s" RESET BOLD, db::getString(&Bundle, "determine.question"));

  element_t buff = "";

  while (scanf(" %[^\n]", buff) != 1)
    {
      while (getchar() != '\n') continue;

      printf(RESET FG_BRIGHT_GREEN "%s" RESET BOLD,
             db::getString(&Bundle, "input.incorrectData"));
    }

  while (getchar() != '\n') continue;

  printf(RESET);

  const Node *node = findElement(tree, buff, true);

  if (node)
    printNode(node, false);
  else
    printf(FG_BRIGHT_GREEN "%s" RESET,
           db::getString(&Bundle, "determine.unknownObject"));
}

static void printNode(const Node *node, int isRight)
{
  assert(node);

  if (node->parent)
    {
      printNode(node->parent, node->parent->right == node);

      printf(FG_BRIGHT_GREEN "%s" RESET,/////////////////
             node->left ?
             (((isRight ? node->right : node->left) ->left) ? ", " :
              db::getString(&Bundle, "determinate.last")) :
             "\n");
    }

  if (!node->left || !node->right) return;

  if (isRight)
    printNegative();

  printf(FG_BRIGHT_GREEN "%s" RESET, node->value);
}

static void printNegative()
{
  const char *negative = nullptr;

  switch (rand() % 3)
    {
    case  0: negative = db::getString(&Bundle, "negative.first");  break;
    case  1: negative = db::getString(&Bundle, "negative.second"); break;
    case  2:
    default: negative = db::getString(&Bundle, "negative.third");  break;
    }

  printf(FG_BRIGHT_GREEN "%s ", negative);
}

void difference(const Tree *tree)
{
  printf(ITALIC FG_BRIGHT_RED "diffrence\n" RESET);
}

void show(const Tree *tree)
{
  Settings settings = {};

  getSettings(&settings);

  if (settings.hasTxt)
    {
      printf(ITALIC FG_YELLOW);

      saveTree(tree, stdout);

      printf(RESET);
    }

  if (settings.hasViz)
    {
      const char *image = createImage(tree, false);

      char buffer[MAX_CMD_SIZE] = "";

      sprintf(buffer, "xdg-open %s", image);

      system(buffer);
    }

  printf(RESET);
}
