/*
 * Author: Kyle Kloberdanz
 * Project Start Date: 27 Nov 2018
 * License: GNU GPLv3 (see LICENSE.txt)
 *     This file is part of stutter.
 *
 *     stutter is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     stutter is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with stutter.  If not, see <https://www.gnu.org/licenses/>.
 * File: linkedlist.h
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H


typedef struct linkedlist {
    void *value;
    struct linkedlist *next;
} linkedlist;


linkedlist *ll_new(void *value);
linkedlist *ll_append(linkedlist *list, void *value);
linkedlist *ll_concat(linkedlist *dest, linkedlist *src);
linkedlist *ll_insert(linkedlist *list, void *value);
linkedlist *ll_delete_next_node(linkedlist *list);
void ll_free(linkedlist *list);


#endif /* LINKEDLIST_H */
