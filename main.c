/******************************************
 *               main.c
 ******************************************
 * Adam Bajguz
 * Michał Kierzkowski
 * Grupa: Ps 7, informatyka III semestr
 ******************************************/

// nie ma duplikatow
// przy usuwaniu wybierac nastpenika (na prawo i na lewo ile sie da)
// nie obliczac wag dla calego drzewa tylko dla sciezki
// podpunkt d) tez ma miec pezymistyczna zlozonosc logarytmiczna

#include <stdio.h>
#include "AVLtree.h"
#include "script.h"

void preOrder(FILE * out, struct t_AVLnode *root)
{
    if(root != NULL)
    {
        fprintf(out, "(%s; %u, %u)\n", root->word, root->height, root->tree_size);
        printf("(%s; %u, %u)\n", root->word, root->height, root->tree_size);
        preOrder(out, root->left);
        preOrder(out, root->right);
    }
}

int main() {
    struct t_AVLnode *root = NULL;

    enum {
        exit, add, remove, search, prefix_search, print, script
    } input;
    char buffer[260];
    do {
        printf("\n\n");
        printf("=====Dictionary=====\n");
        printf("1) Add word \n");
        printf("2) Remove word\n");
        printf("3) Search\n");
        printf("4) Prefix search\n");
        printf("5) Print dictionary structure\n");
        printf("6) Execute script\n");
        printf("\n");
        printf("0) Exit dictionary\n");
        printf("=====================\n");
        printf("Select option (0-6): ");
        scanf("%d", &input);
        printf("=====================\n\n");
        system("CLS");
        switch (input) {
            case add:
                printf("Enter word to add (30 char. max): ");
                if (scanf("%s", &buffer) == 1)
                    if(strlen(buffer) <= 30)
                        AVLInsert(&root, buffer);
                    else
                        printf("Word '%s' is too long!\n", buffer);
                else
                    printf("Unexpected error!\n");
                break;

            case remove:
                printf("Enter word to remove (30 char. max): ");
                if (scanf("%s", &buffer) == 1)
                    if(strlen(buffer) <= 30)
                        AVLRemove(&root, buffer);
                    else
                        printf("Word '%s' is too long!\n", buffer);
                else
                    printf("Unexpected error!\n");
                break;

            case search:
                printf("Enter word to search (30 char. max): ");
                if (scanf("%s", &buffer) == 1)
                    if(strlen(buffer) <= 30)
                        printf("'%s' does %sexist in dictionary.", buffer, AVLSearch(root, buffer) ? "" : "not ");
                    else
                        printf("Word '%s' is too long!\n", buffer);
                else
                    printf("Unexpected error!\n");
                break;

            case prefix_search:
                printf("Enter prefix to search (30 char. max): ");
                if (scanf("%s", &buffer) == 1)
                    if(strlen(buffer) <= 30) {
                        uint64_t num = AVLSearchPrefix(root, buffer);
                        printf("There %s %u word%s starting with '%s'.", (num > 1 || num == 0) ? "are" : "is", num, (num > 1 || num == 0) ? "s" : "", buffer);
                    }
                    else
                        printf("Word '%s' is too long!\n", buffer);
                else
                    printf("Unexpected error!\n");
                break;

            case print:
                printf("Dictionary structure:\n");
                AVLPrint(root);
                break;

            case script:
                printf("Enter script filename (without *.txt): ");
                if (scanf("%s", &buffer) == 1)
                    if(strlen(buffer) <= 255) {
                        printf("Output will be saved to: '%s_out.txt'\n", buffer);
                        printf("Executing '%s.txt'...\n\n", buffer);
                        AVLScript(&root, buffer);
                    }
                    else
                        printf("Filename is to long!\n");
                else
                    printf("Unexpected error!\n");

//                FILE * out = fopen("test.txt", "wr");
//                preOrder(out, root);
//                fclose(out);
                break;
        }
    } while (input != exit);

    AVLDestroy(&root);

    return 0;
}