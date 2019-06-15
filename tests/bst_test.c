#include <stdio.h>
#include <stdlib.h>

#include "../bst.h"

int main() {
    struct BST *bst = NULL;
    char *words[] = {"these", "are", "some", "words", NULL};
    int i;
    for (i = 0; words[i] != NULL; i++) {
        bst = bst_insert(bst, words[i], i);
    }
    bst_print(bst);
    bst_destroy(bst);
    return 0;
}
