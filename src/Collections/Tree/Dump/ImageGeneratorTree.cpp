#include "Tree.h"
#include "TreeDump.h"

#include <ctype.h>
#include <string.h>
#include <time.h>
#include "Assert.h"

const char *const ELEMENT_COLOR = "\"#b59d65\"";

static char *generateDotFile(const Tree *tree, int isDump);

static char *generateNewFileName();

static void openDigraph(FILE *file);

static void setDefaultNodeParameters(FILE *file);

static void generateNode(const Node *node, int isDump, FILE *file);

static void generateMainSequence(const Node *node, FILE *file);

static void closeDigraph(FILE *file);

char *createImage(const Tree *tree, int isDump)
{
  assert(tree);

  char *dotFileName = generateDotFile(tree, isDump);

  size_t size = strlen(dotFileName);

  char *command = (char *)calloc(2*size + 16, sizeof(char));

  if (!command)
    return nullptr;

  int offset = sprintf(command, "dot %s -T png ", dotFileName);

  sprintf(dotFileName + size - 3, "png");

  sprintf(command + offset, "-o %s", dotFileName);

  system(command);

  free(command);

  return dotFileName;
}

static char *generateDotFile(const Tree *tree, int isDump)
{
  assert(tree);

  char *fileName = generateNewFileName();

  if (!fileName)
    return nullptr;

  FILE *file = fopen(fileName, "w");

  if (!file)
    return nullptr;

  openDigraph(file);

  setDefaultNodeParameters(file);

  if (tree->root)
    {
      generateNode(tree->root, isDump, file);

      generateMainSequence(tree->root, file);
    }

  closeDigraph(file);

  fclose(file);

  return fileName;
}

static char *generateNewFileName()
{
  time_t now = 0;
  time(&now);
  char *dataString = ctime(&now);

  int i = 0;

  for ( ; dataString[i]; ++i)
    if (isspace(dataString[i]) || ispunct(dataString[i]))
      dataString[i] = '_';

  dataString[i - 1] = '\0';

  static char buff[64] = "";

  static int imageCount = 0;

  sprintf(buff, ".log/image_%s_%10.10d.dot", dataString, imageCount++);

  return buff;
}

static void openDigraph(FILE *file)
{
  fprintf(file, "digraph {\n");
}

static void setDefaultNodeParameters(FILE *file)
{
  //  fprintf(file, "\tsplines=ortho;\n");
  fprintf(file, "\tnode[shape=Mrecord];\n");
  fprintf(file, "\tYES [shape=Mrecord,style=filled,color=RED,label=\"Yes\"];\n");
  fprintf(file, "\tNo [shape=Mrecord,style=filled,color=BLUE,label=\"No\"];\n");
}

static void generateNode(const Node *node, int isDump, FILE *file)
{
  assert(node);
  if (isDump)
    fprintf(
            file,
            "\t\tNODE_%p [ style=filled,color=%s,label=\""
            "{ %p |"
            " Elem: %s |"
            " <p> Parent: %p |"
            " <l> Left  : %p |"
            " <r> Right : %p }"
            "\" ];\n",
            (const void *)node,
            ELEMENT_COLOR,
            (const void *)node,
            toString(node->value),
            (const void *)node->parent,
            (const void *)node->left,
            (const void *)node->right
            );
  else
    fprintf(
            file,
            "\t\tNODE_%p [ style=filled,color=%s,label=\""
            "{ %s }"
            "\" ];\n",
            (const void *)node,
            ELEMENT_COLOR,
            toString(node->value)
            );

  if (node->left)
    generateNode(node->left, isDump, file);

  if (node->right)
    generateNode(node->right, isDump, file);
}

static void generateMainSequence(const Node *node, FILE *file)
{
  assert(node);

  if (node->left)
    {
      fprintf(file, "\tNODE_%p->NODE_%p [color=RED,label=\"Yes\"];\n", (const void *)node, (const void *)node->left);

      generateMainSequence(node->left, file);
    }

  if (node->right)
    {
      fprintf(file, "\tNODE_%p->NODE_%p [color=BLUE,label=\"No\"];\n", (const void *)node, (const void *)node->right);
      generateMainSequence(node->right, file);
    }
}

static void closeDigraph(FILE *file)
{
  fprintf(file, "}");
}
