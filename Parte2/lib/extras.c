#include <stdio.h>
#include <stdlib.h>

void reporteTWC(int max_width,
                int linesOnly, 
                int wordsOnly,
                int lines,
                int words,
                int bytes,
                char *filename) {
    if (linesOnly && wordsOnly) {
        fprintf(stdout, "%*d %*d %*d %s\n", max_width, lines, max_width, words, max_width, bytes, filename);
    } else if (linesOnly) {
        fprintf(stdout, "%*d %*d %s\n", max_width, lines, max_width, bytes, filename);  
    } else if (wordsOnly) {
        fprintf(stdout, "%*d %*d %s\n", max_width, words, max_width, bytes, filename);
    } else {
        fprintf(stdout, "%*d %*d %*d %s\n", max_width, lines, max_width, words, max_width, bytes, filename);
    }
    return;
}