#include <stdio.h>
#include <stdlib.h>
#include "Tips.h"

void display_random_tip(const char* filename) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        printf("Could not open tips file.\n");
        return;
    }

    /* Count total tips */
    int total = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        total++;
    }

    /* Pick a random one */
    int chosen = rand() % total;

    /* Go back to start and read to chosen line */
    rewind(file);
    int current = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (current == chosen) {
            printf("Tip of the Day: %s\n", line);
            break;
        }
        current++;
    }

    fclose(file);
}