#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst.h"

struct BST *bst_new(char *key, int value) {
    struct BST *node = malloc(sizeof(struct BST));
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct BST *bst_insert(struct BST *root, char *key, int value) {
    int comparison;
    struct BST *node = root;
    if (node == NULL) {
        return bst_new(key, value);
    }
_tail_insert:
    comparison = strcmp(node->key, key);
    if (comparison < 0) {
        if (node->right == NULL) {
            node->right = bst_new(key, value);
        } else {
            node = node->right;
            goto _tail_insert;
        }
    } else if (comparison > 0) {
        if (node->left == NULL) {
            node->left = bst_new(key, value);
        } else {
            node = node->left;
            goto _tail_insert;
        }
    }
    return root;
}

void bst_print_node(struct BST* node) {
    printf("{\"%s\": %d}\n", node->key, node->value);
}

void bst_print(struct BST* node) {
    if (node != NULL) {
        bst_print(node->left);
        bst_print_node(node);
        bst_print(node->right);
    }
}

struct BST *bst_find(struct BST* node, char *key) {
    int comparison;
_tail_find:
    comparison = strcmp(node->key, key);
    if (comparison < 0) {
        if (node->right == NULL) {
            return NULL;
        } else {
            node = node->right;
            goto _tail_find;
        }
    } else if (comparison > 0) {
        if (node->left == NULL) {
            return NULL;
        } else {
            node = node->left;
            goto _tail_find;
        }
    } else {
        return node;
    }
}

void bst_destroy(struct BST* node) {
    if (node != NULL) {
        bst_destroy(node->left);
        bst_destroy(node->right);
        free(node);
    }
}
