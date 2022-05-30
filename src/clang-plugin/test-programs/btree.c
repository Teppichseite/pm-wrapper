#define PM_PTR __attribute__((pointer_type(0)))

void *pm_root();

#define NULL 0
#define int64_t int
#define size_t int

struct btree_node {
  int64_t key;
  struct btree_node *slots[2];
  char value[];
};

struct btree {
  struct btree_node *root;
};

struct btree_node_arg {
  size_t size;
  int64_t key;
  const char *value;
};

static void btree_insert(struct btree *btree, int64_t key, const char *value) {
  struct btree_node **dst = &btree->root;

  while (dst != NULL) {
    dst = &(*dst)->slots[key > (*dst)->key];
  }

  struct btree_node_arg args;
  args.size = sizeof(struct btree_node) + 1 + 1;
  args.key = key;
  args.value = value;
}

static const char *btree_find(struct btree *btree, int64_t key) {
  struct btree_node *node = btree->root;

  while (node != NULL) {
    if (node->key == key)
      return node->value;
    else
      node = node->slots[key > node->key];
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return 1;
  }

  const char *path = argv[1];

  struct btree *tree = (struct btree *)pm_root();

  const char op = argv[2][0];
  int64_t key;
  const char *value;

  switch (op) {
  case 'p':
    break;
  case 'i':
    key = 2;
    value = argv[4];
    btree_insert(tree, key, value);
    break;
  case 'f':
    key = 2;
    if ((value = btree_find(tree, key)) != NULL) {

    } else {
    }
    break;
  default:
    break;
  }

  return 0;
}