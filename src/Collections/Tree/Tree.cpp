#include "Tree.h"
#include "TreeDump.h"

#include "Assert.h"

static const Node *findNode(const Node *node, const element_t value);

unsigned validateTree(const Tree *tree)
{
  if (!tree)
    return TREE_NULLPTR;

  unsigned error = 0;

  return error;
}

void createTree(Tree *tree, int *error)
{
  if (!tree)
    ERROR();

  tree->root = nullptr;
  tree->size = 0;

  CHECK_VALID(tree, error);
}

void destroyTree(Tree *tree, int *error)
{
  CHECK_VALID(tree, error);

  if (tree->root)
    removeNode(tree->root, error);

  tree->root = nullptr;
  tree->size = 0;
}

const Node *findElement(const Tree *tree, element_t value, int isList, int *error)
{
  CHECK_VALID(tree, error, 0);

  if (!validateValue(value))
    ERROR(0);

  const Node *result = nullptr;

  if (tree->root)
    result = findNode(tree->root, value);

  if (!result)
    return nullptr;
  else
    {
      if (isList)
        return (!(result->left || result->right) ?  result : nullptr);
      else
        return result;
    }
}

static const Node *findNode(const Node *node, const element_t value)
{
  assert(node);
  assert(validateValue(value));

  const Node *result = nullptr;

  if (node->left)
    {
      result = findNode(node->left,  value);

      if (result)
        return result;
    }

  if (node->right)
    {
      result = findNode(node->right,  value);

      if (result)
        return result;
    }

  return (!compareValues(node->value, value) ? node : nullptr);
}
