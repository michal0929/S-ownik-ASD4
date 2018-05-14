/******************************************
 *               script.c
 ******************************************
 * Adam Bajguz
 * Michał Kierzkowski
 * Grupa: Ps 7, informatyka III semestr
 ******************************************/

#include "script.h"

#define SCRIPT_PRINT_DEBUG 0

void AVLScript(struct t_AVLnode **const root, const char *const filename) {
    clock_t exec_begin = clock();
    char filename_cpy[260];

    strcpy(filename_cpy, filename);
    FILE *input = fopen(strcat(filename_cpy, ".txt"), "rt");
    if(!input) {
        printf("Error reading file!\n");
        return;
    }

    strcpy(filename_cpy, filename);
    FILE *output = fopen(strcat(filename_cpy, "_out.txt"), "wt");

    uint32_t cmd_num;
    while(fscanf(input, "%u", &cmd_num) != 1);
#if SCRIPT_PRINT_DEBUG == 1
    printf("%u\n", cmd_num);
#endif

    register char command;
    char parameter[31];
    for (register uint32_t i = 0; i < cmd_num; ++i) {
#if SCRIPT_PRINT_DEBUG == 1
        printf("-----\n");
#endif

        do {
            command = fgetc(input);
        } while (!isprint(command));

        while(fscanf(input, "%s", &parameter) != 1);
#if SCRIPT_PRINT_DEBUG == 1
        printf("%c %s\n", command, parameter);
#endif
        switch (command) {
            case 'W':
                AVLInsert(root, parameter);
                break;

            case 'U':
                AVLRemove(root, parameter);
                break;

            case 'S':
                fprintf(output, "%s\n", AVLSearch(*root, parameter) ? "TAK" : "NIE");
                break;

            case 'L':
                fprintf(output, "%u\n", AVLSearchPrefix(*root, parameter));
                break;

            default:
                printf("An error occured while reading '%s.txt'! Unknown command '%c' (line: %u)\n", filename, command, i);
                break;
        }
    };

    fclose(input);
    fclose(output);

    clock_t exec_end = clock();
    double exec_time = ((double) (exec_end - exec_begin)) / CLOCKS_PER_SEC;
    printf("   Execution time: %f seconds.\n", exec_time);
}