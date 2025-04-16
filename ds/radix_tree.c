#include "radix_tree.h"

#include <string.h>

#include "util.h"

static _RadixTreeNode* _form_node(char *prefix, void *data, size_t data_size)
{
  _RadixTreeNode* node = malloc(sizeof(*node));

  node->prefix   = cpystr(prefix);
  node->data     = cpymem(data, data_size);
  node->next     = NULL;
  node->children = NULL;
  node->parent   = NULL;

  return node;
}

static void _add(_RadixTreeNode *node, char *key, void *data, size_t data_size)
{
  if (NULL == node)
    return;
  char *kstart = key;
  char *prefix = node->prefix;

  while (*prefix && *prefix == *key)
    prefix++, key++;

  if (!prefix) {
    if (!key) {
      free(node->data);
      node->data = cpymem(data, data_size);
      return;
    } else {
      if (node->children) {
        _add(node->children, key, data, data_size);
      } else {
        node->children = _form_node(key, data, data_size);
        node->children->parent = node;
      }
      return;
    }
  } else {
    if (key == kstart) {
      // no common prefix
    }
    if (!key) {
      int len = kstart - key;
      // update node prefix to contain only differing part
      memmove(node->prefix, prefix, len + 1);
      // split prefix into two parts and add extra transit node without data

      // 1) parent with common prefix - transit node without data
      _RadixTreeNode *parent = _form_node(key, NULL, 0);
      parent->data[key - kstart] = '\0';

      // 2) sibling with differing postfix - leaf node with data
      _RadixTreeNode *child = _form_node(key + len, data, data_size);

      parent->children = child;
      child->next = node;

      node->parent->children = parent;
      node->parent = parent;

      return;
    }

    // TODO: some balanced tree as optimization both performance and code mess
    //       would be nice
    if (!node->next) {
      node->next = _form_node(key + len, data + len, data_size);
      node->next->parent = node;

      return;
    }

    if (key == kstart) {
      // no common prefix. try out next node
    }


  }
}

void add(RadixTree *self, char *key, void *data, size_t data_size)
{
  if (NULL == self->head) {
    self->head = _form_node(NULL, key, data, data_size);
    return;
  }

}

static void* get(RadixTree *self, char *key)
{
  // TODO: impl
}

static void remove(RadixTree *self, char *key)
{
  // TODO: impl
}

static void release(RadixTree **pself)
{
  // TODO: impl
}

static RadixTree* form(void)
{
  // TODO: impl
}
