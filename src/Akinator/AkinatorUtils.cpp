#include "AkinatorUtils.h"
#include "TreeDump.h"

#include <stdlib.h>
#include <ctype.h>
#include "TextReader.h"
#include "Settings.h"
#include "ColorOutput.h"
#include "StringsUtils.h"
#include "Assert.h"

#pragma GCC diagnostic ignored "-Wformat-security"

const int MAX_ANSWER_SIZE   = 10;

const int MAX_CMD_SIZE      = 64;

const int COUNT_OF_NEGATIVE = 3;

static int readAnswer();

static bool checkAnswer(const char *answer);

static void addObject(Tree *tree, Node *node);

static void printNode(const Node *node, int isRight);

static void printNegative();

static const Node *getNode(const Tree *tree);

static char *getPath(const Node *node);

void guess(Tree *tree)
{
  assert(tree);

  Node *current = tree->root;

  if (!current)
    {
      printf(FG_BRIGHT_CYAN);
      audioPrintf("%s%s", db::getString(&Bundle, "answer.empty"),
                          db::getString(&Bundle, "answer"));
      printf(RESET);

      if (!readAnswer())
        addObject(tree, tree->root);

      return;
    }

  while (current->left)
    {
      printf(FG_BRIGHT_CYAN);
      audioPrintf("%s%s", current->value,
                  db::getString(&Bundle, "answer"));
      printf(RESET);

      if (readAnswer())
        current = current->left;
      else
        current = current->right;
    }

  printf(FG_BRIGHT_CYAN);
  audioPrintf("%s%s%s", db::getString(&Bundle, "answer.final"),
              current->value, db::getString(&Bundle, "answer"));
  printf(RESET);

  if (!readAnswer())
    addObject(tree, current);
  else
    {
      printf(FG_BRIGHT_CYAN);
      audioPrintf("%s\n", db::getString(&Bundle, "guess.win"));
      printf(RESET);
    }
}

static int readAnswer()
{
  char answer[MAX_ANSWER_SIZE] = "";

  printf(BOLD);

  while (scanf(" %s", answer) != 1 || !checkAnswer(answer))
    {
      while (getchar() != '\n') continue;

      printf(RESET);
      audioPrintf("%s", db::getString(&Bundle, "input.incorrect"));
      printf(BOLD);
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

static void addObject(Tree *tree, Node *node)
{
  assert(tree);

  printf(FG_BRIGHT_CYAN);
  audioPrintf("%s\n", db::getString(&Bundle, "guess.newObject"));
  printf(RESET BOLD);

  element_t buff = "";

  while (scanf(" %[^\n]", buff) != 1)
    {
      while (getchar() != '\n') continue;

      printf(RESET FG_BRIGHT_CYAN);
      audioPrintf("%s", db::getString(&Bundle, "input.unkwownData"));
      printf(RESET BOLD);
    }

  while (getchar() != '\n') continue;

  printf(RESET);

  if (findElement(tree, buff))
    {
      printf(FG_BRIGHT_CYAN);
      audioPrintf("%s", db::getString(&Bundle, "input.copy"));
      printf(ITALIC);
      audioPrintf("%s", buff);
      printf(RESET FG_BRIGHT_CYAN"!\n" RESET);

      return;
    }

  Node *newNode = createNode(buff);

  if (!node)
    {
      tree->root = newNode;

      return;
    }

  printf(FG_BRIGHT_CYAN);
  audioPrintf("%s", db::getString(&Bundle, "guess.difference"));
  printf(ITALIC);
  audioPrintf("%s", buff);
  printf(RESET FG_BRIGHT_CYAN);
  audioPrintf("%s", db::getString(&Bundle, "guess.and"));
  printf(ITALIC);
  audioPrintf("%s", node->value);
  printf(RESET FG_BRIGHT_CYAN "?\n" RESET BOLD);

  while (scanf(" %[^\n]", buff) != 1)
    {
      while (getchar() != '\n') continue;

      printf(RESET FG_BRIGHT_CYAN);
      audioPrintf("%s",  db::getString(&Bundle, "input.unkwownData"));
      printf(RESET BOLD);
    }

  while (getchar() != '\n') continue;

  printf(RESET);

  Node *oldNode = createNode(node->value);

  strncpy(node->value, buff, MAX_SIZE);
  setParent(newNode, node, true);
  setParent(oldNode, node, false);

  printf(RESET FG_BRIGHT_CYAN);
  audioPrintf("%s\n", db::getString(&Bundle, "guess.fail"));
  printf(RESET);
}

void determine(const Tree *tree)
{
  printf(FG_BRIGHT_GREEN);
  audioPrintf("%s", db::getString(&Bundle, "determine.question"));
  printf(RESET BOLD);

  element_t buff = "";

  while (scanf(" %[^\n]", buff) != 1)
    {
      while (getchar() != '\n') continue;

      printf(RESET FG_BRIGHT_GREEN);
      audioPrintf("%s", db::getString(&Bundle, "input.incorrectData"));
      printf(RESET BOLD);
    }

  while (getchar() != '\n') continue;

  printf(RESET);

  const Node *node = findElement(tree, buff, true);

  if (node)
    printNode(node, false);
  else
    {
      printf(FG_BRIGHT_GREEN);
      audioPrintf("%s\n", db::getString(&Bundle, "determine.unknownObject"));
      printf(RESET);
    }
}

static void printNode(const Node *node, int isRight)
{
  assert(node);

  if (node->parent)
    {
      printNode(node->parent, node->parent->right == node);

      printf(FG_BRIGHT_GREEN);
      audioPrintf("%s", node->left ?
                  (((isRight ? node->right : node->left) ->left) ? ", " :
                   db::getString(&Bundle, "determinate.last")) :
                  "\n");/////////////////////////
      printf(RESET);
    }

  if (!node->left || !node->right) return;

  printf(FG_BRIGHT_GREEN);

  if (isRight)
    printNegative();

  printf(FG_BRIGHT_GREEN);
  audioPrintf("%s", node->value);
  printf(RESET);
}

static void printNegative()
{
  const char *negative = nullptr;

  switch (rand() % COUNT_OF_NEGATIVE)
    {
    case  0: negative = db::getString(&Bundle, "negative.first");  break;
    case  1: negative = db::getString(&Bundle, "negative.second"); break;
    case  2:
    default: negative = db::getString(&Bundle, "negative.third");  break;
    }

  audioPrintf("%s ", negative);
}

void difference(const Tree *tree)
{
  assert(tree);

  printf(RESET FG_BRIGHT_RED);
  audioPrintf("%s", db::getString(&Bundle, "difference.firstQuestion"));
  printf(RESET BOLD);

  const Node *firstNode = getNode(tree);

  if (!firstNode)  return;

  printf(RESET FG_BRIGHT_RED);
  audioPrintf("%s", db::getString(&Bundle, "difference.secondQuestion"));
  printf(RESET BOLD);

  const Node *secondNode = getNode(tree);

  if (!secondNode) return;

  char *firstPath  = getPath(firstNode);
  char *secondPath = getPath(secondNode);

  firstNode  = tree->root;
  secondNode = tree->root;

  int i = 0;

  for ( ; firstPath[i] && secondPath[i] && firstPath[i] == secondPath[i]; ++i)
    {
      printf(FG_BRIGHT_RED);
      audioPrintf("%s", db::getString(&Bundle, "difference.same"));

      if (firstPath[i] == 'r')
        printNegative();

      audioPrintf("%s\n", firstNode->value);

      printf(RESET);

      if (firstPath[i] == 'l')
        firstNode = secondNode = secondNode->left;
      else
        firstNode = secondNode = secondNode->right;
    }

  if (firstPath[i])
    {
      printf(FG_BRIGHT_RED ITALIC);
      audioPrintf("%s\n", db::getString(&Bundle, "difference.firstDiff"));
      printf(RESET FG_BRIGHT_RED);
    }

  for (int j = 0; firstPath[i + j]; ++j)
    {
      if (firstPath[i + j] == 'r')
        printNegative();

      audioPrintf("%s\n", firstNode->value);

      if (firstPath[i + j] == 'l')
        firstNode = firstNode->left;
      else
        firstNode = firstNode->right;
    }

  if (secondPath[i])
    {
      printf(FG_BRIGHT_RED ITALIC);
      audioPrintf("%s\n", db::getString(&Bundle, "difference.secondDiff"));
      printf(RESET FG_BRIGHT_RED);
    }

  for (int j = 0; secondPath[i + j]; ++j)
    {
      if (secondPath[i + j] == 'r')
        printNegative();

      audioPrintf("%s\n", secondNode->value);

      if (secondPath[i + j] == 'l')
        secondNode = secondNode->left;
      else
        secondNode = secondNode->right;
    }

  free(firstPath);
  free(secondPath);

  printf(RESET);
}

static const Node *getNode(const Tree *tree)
{
  element_t buff = "";

  while (scanf(" %[^\n]", buff) != 1)
    {
      while (getchar() != '\n') continue;

      printf(RESET FG_BRIGHT_RED);
      audioPrintf("%s", db::getString(&Bundle, "input.incorrectData"));
      printf(RESET BOLD);
    }

  while (getchar() != '\n') continue;

  printf(RESET);

  const Node *node = findElement(tree, buff, true);

  if (!node)
    {
      printf(FG_BRIGHT_RED);
      audioPrintf("%s\n", db::getString(&Bundle, "determine.unknownObject"));
      printf(RESET);

      return nullptr;
    }

  return node;
}

static char *getPath(const Node *node)
{
  size_t depth = 0;

  for (const Node *temp = node; temp->parent; temp = temp->parent)
    ++depth;

  char *path = (char *)calloc(depth + 1, sizeof(char));

  if (!path) return nullptr;

  for (const Node *temp = node; temp->parent; temp = temp->parent)
    path[--depth] = ((temp->parent->left == temp) ? 'l' : 'r');

  return path;
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
