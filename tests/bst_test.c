/*
 * Author: Kyle Kloberdanz
 * Project Start Date: 27 Nov 2018
 * License: GNU GPLv3 (see LICENSE.txt)
 *     This file is part of minic.
 *
 *     minic is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     minic is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with minic.  If not, see <https://www.gnu.org/licenses/>.
 * File: bst_test.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "../bst.h"

int main() {
    struct BST *bst = NULL;
    struct BST *node = NULL;
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

    node = bst_find(bst, "non");
    bst_print_node(node);
    if (node->value != 56) {
        fprintf(stderr, "%s\n", "bst_find returned wrong node");
        exit(EXIT_FAILURE);
    }

    bst_destroy(bst);
    return 0;
}
