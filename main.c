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
 * File: main.c
 */

#include <stdio.h>
#include <stdlib.h>


#include "stutter.h"


int main(int argc, char **argv) {
    char *output_filename = NULL;
    FILE *output;
    int exit_code;
    if (argc != 2) {
        fprintf(stderr, "usage: %s FILENAME\n", argv[0]);
        return 1;
    } else {
        ASTNode *tree = parse();

        if (tree == NULL) {
            fprintf(stderr, "%s\n", "failed to parse input");
            exit(EXIT_FAILURE);
        }

        output_filename = argv[1];
        output = fopen(output_filename, "w");

        if (output == NULL) {
            fprintf(stderr, "%s\n", "failed to open output file");
            exit(EXIT_FAILURE);
        }

        exit_code = emit(output, tree);
        if (fclose(output) != 0) {
            fprintf(stderr, "%s\n", "failed to close output file");
            exit(EXIT_FAILURE);
        }
        destroy_ast_node(tree);
        return exit_code;
    }
}
