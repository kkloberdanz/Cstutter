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
 * File: linkedlist.c
 */

#include <stdio.h>
#include <stdlib.h>


#include "linkedlist.h"


linkedlist *ll_new(void *value) {
    linkedlist *new_list = (linkedlist *)malloc(sizeof(linkedlist));
    if (new_list == NULL) {
        fprintf(stderr, "%s\n", "linkedlist out of memory");
        exit(EXIT_FAILURE);
    }
    new_list->value = value;
    new_list->next = NULL;
    return new_list;
}


linkedlist *ll_append(linkedlist *list, void *value) {
    linkedlist *new_node = (linkedlist *)malloc(sizeof(linkedlist));
    if (new_node == NULL) {
        fprintf(stderr, "%s\n", "linkedlist out of memory");
        exit(EXIT_FAILURE);
    }
    while (list->next) {
        list = list->next;
    }
    new_node->value = value;
    new_node->next = NULL;
    list->next = new_node;
    return new_node;
}


linkedlist *ll_insert(linkedlist *list, void *value) {
    linkedlist *new_node = (linkedlist *)malloc(sizeof(linkedlist));
    if (new_node == NULL) {
        fprintf(stderr, "%s\n", "linkedlist out of memory");
        exit(EXIT_FAILURE);
    }
    new_node->value = value;
    new_node->next = list->next;
    list->next = new_node;
    return new_node;
}


linkedlist *ll_delete_next_node(linkedlist *list) {
    linkedlist *tmp = list->next;
    list->next = list->next->next;

    if (tmp->value) {
        free(tmp->value);
    }
    tmp->value = NULL;
    free(tmp);
    return list;
}


linkedlist *ll_concat(linkedlist *dest, linkedlist *src) {
    linkedlist *tmp = dest;
    if (dest == NULL) {
        return src;
    }
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = src;
    return dest;
}


void ll_free(linkedlist *list) {
    /* TODO: pass destructor as function pointer */
    while (list) {
        linkedlist *tmp = list;
        list = list->next;
        tmp->next = NULL;

        free(tmp->value);
        tmp->value = NULL;
        free(tmp);
    }
}
