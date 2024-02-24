#include <stdio.h>
#include <stdlib.h>


//Same as interface provided for project.
void removeComments(char* testcaseFile, char* cleanFile) {
    const int BUFFER_SIZE = 200;
    FILE *src_file, *dest_file;
    char buffer[BUFFER_SIZE];

    // Opening files
    src_file = fopen(testcaseFile, "r");
    dest_file = fopen(cleanFile, "w");

    if (!src_file || !dest_file) {
        printf("Error in Opening Files!\n");
        printf("Comment Removal Aborted!\n");
        return;
    }

    while (fgets(buffer, BUFFER_SIZE, src_file)) {
        int i = 0;
        while (i < BUFFER_SIZE && buffer[i] != '%' && buffer[i] != '\n')
            i++;
        fprintf(dest_file, "%.*s\n", i, buffer);
    }

    fclose(src_file);
    fclose(dest_file);
    return;
}