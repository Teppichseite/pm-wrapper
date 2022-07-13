#define LL_LIST(type, struct_name)                                             \
  struct struct_name {                                                         \
    type *head;                                                                \
    type *tail;                                                                \
  }
#define LL_NODES(type, struct_name)                                            \
  struct struct_name {                                                         \
    type *prev;                                                                \
    type *next;                                                                \
  }

#define LL_INSERT(list, new_node, node_field)                                  \
  do {                                                                         \
    if (!new_node) {                                                           \
      break;                                                                   \
    }                                                                          \
    typeof(new_node) head = (list)->head;                                      \
    (&new_node->node_field)->next = head;                                      \
    (&new_node->node_field)->prev = NULL;                                      \
    (list)->head = new_node;                                                   \
                                                                               \
    if (!head) {                                                               \
      (list)->tail = new_node;                                                 \
    } else {                                                                   \
      (&new_node->node_field)->prev = new_node;                                \
    }                                                                          \
  } while (0)

#define LL_REMOVE_LAST(list, node_field)                                       \
  do {                                                                         \
    typeof((list)->tail) tail = (list)->tail;                                  \
    if (!(&tail->node_field)->prev) {                                          \
      (list)->tail = NULL;                                                     \
      (list)->head = NULL;                                                     \
      pm_free(tail);                                                           \
      break;                                                                   \
    }                                                                          \
                                                                               \
    typeof((list)->tail) new_tail = (&tail->node_field)->prev;                 \
    (&new_tail->node_field)->next = NULL;                                      \
    (list)->tail = new_tail;                                                   \
    pm_free(tail);                                                             \
  } while (0)

#define LL_FOREACH(list, node_field, node_name, callback)                      \
  do {                                                                         \
    typeof((list)->tail) node_name = (list)->head;                             \
    while ((node_name) != NULL) {                                              \
      callback;                                                                \
      (node_name) = (&node_name->node_field)->next;                            \
    }                                                                          \
  } while (0)
