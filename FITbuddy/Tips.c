#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Tips.h"
#include "console_color.h"

void display_random_tip(const char* filename) {
    FILE* file;
    fopen_s(&file, filename, "r");
    if (file == NULL) {
        set_color(COLOR_ERROR);
        printf("Could not open tips file.\n");
        set_color(COLOR_DEFAULT);
        return;
    }

    int total = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        total++;
    }

    int chosen = rand() % total;

    rewind(file);
    int current = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (current == chosen) {
            set_color(COLOR_TIP);
            printf("Tip of the Day: %s\n", line);
            set_color(COLOR_DEFAULT);
            break;
        }
        current++;
    }

    fclose(file);
}