#include <stdio.h>
#include <stdlib.h>

#include "../bst.h"

int main() {
    struct BST *bst = NULL;
    char *words[] = {
        "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur",
        "adipiscing", "elit,", "sed", "do", "eiusmod", "tempor", "incididunt",
        "ut", "labore", "et", "dolore", "magna", "aliqua.", "Ut", "enim",
        "ad", "minim", "veniam,", "quis", "nostrud", "exercitation",
        "ullamco", "laboris", "nisi", "ut", "aliquip", "ex", "ea", "commodo",
        "consequat.", "Duis", "aute", "irure", "dolor", "in", "reprehenderit",
        "in", "voluptate", "velit", "esse", "cillum", "dolore", "eu",
        "fugiat", "nulla", "pariatur.", "Excepteur", "sint", "occaecat",
        "cupidatat", "non", "proident,", "sunt", "in", "culpa", "qui",
        "officia", "deserunt", "mollit", "anim", "id", "est", "laborum", NULL
    };
    int i;
    for (i = 0; words[i] != NULL; i++) {
        bst = bst_insert(bst, words[i], i);
    }
    bst_print(bst);
    bst_destroy(bst);
    return 0;
}
