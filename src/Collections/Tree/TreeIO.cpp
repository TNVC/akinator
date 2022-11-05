#include "Tree.h"
#include "TreeDump.h"

#include "Assert.h"

const int TAB_SIZE = 4;

static void printNode(const Node *node, FILE *file);

static void printSpaces(int count, FILE *file);

static Node *scanNode(FILE *file);

void saveTree(const Tree *tree, FILE *file, int *error)
{
  CHECK_VALID(tree, error);

  if (!file)
    ERROR();

  printNode(tree->root, file);

  CHECK_VALID(tree, error);
}

void loadTree(Tree *tree, FILE *file, int *error)
{
  CHECK_VALID(tree, error);

  if (!file)
    ERROR();

  tree->root = scanNode(file);

  CHECK_VALID(tree, error);
}

static void printNode(const Node *node, FILE *file)
{
  assert(node);
  assert(file);

  static int offset = 0;

  printSpaces(offset, file);

  fprintf(file, "{ \"%s\" ", node->value);

  offset += TAB_SIZE;

  if (node->left || node->right)
    putc('\n', file);

  if (node->left)
    printNode(node->left, file);


  if (node->right)
      printNode(node->right, file);

  offset -= TAB_SIZE;

  if (node->left || node->right)
    printSpaces(offset, file);

  fprintf(file, "}\n");
}

static void printSpaces(int count, FILE *file)
{
  assert(file);
  assert(count >= 0);

  fprintf(file, "%*s", count, "");
}

static Node *scanNode(FILE *file)
{
  assert(file);

  char ch = '\0';

  if (!fscanf(file, " %c", &ch) || ch != '{')
    return nullptr;

  Node *newNode = nullptr;

  char buff[MAX_SIZE] = "";

  if (fscanf(file, " \"%[^\"]\"", buff))
    {
      newNode = createNode(buff);

      if (!newNode)
        return nullptr;

      ch = '\0';

      fscanf(file, " %c", &ch);

      ungetc(ch, file);

      if (ch == '{')
        {
          newNode->left  = scanNode(file);
          newNode->right = scanNode(file);

          if (!newNode->left || !newNode->right)
            {
              removeNode(newNode);

              return nullptr;
            }

          newNode->left ->parent = newNode;
          newNode->right->parent = newNode;
        }
    }

  if (!fscanf(file, " %c", &ch) || ch != '}')
    {
      if (newNode) removeNode(newNode);

      return nullptr;
    }

  return newNode;
}
