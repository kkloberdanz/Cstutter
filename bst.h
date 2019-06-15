#ifndef BST_H 
#define BST_H

struct BST {
    char *key;
    int value;
    struct BST *left;
    struct BST *right;
};

struct BST *bst_new(char *key, int value);
struct BST *bst_insert(struct BST *node, char *key, int value);
void bst_print(struct BST *node);
struct BST *bst_find(struct BST *node, char *key);
void bst_destroy(struct BST *bst);

#endif
