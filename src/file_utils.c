#include <stdio.h>

int count_lines(FILE* file) {
    int lines = 0;
    char buffer[1024];
    while (fgets(buffer, 1024, file)) lines++;
    rewind(file);
    return lines;
}
