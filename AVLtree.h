/******************************************
 *               AVLtree.h
 ******************************************
 * Adam Bajguz
 * Michał Kierzkowski
 * Grupa: Ps 7, informatyka III semestr
 ******************************************/

#ifndef PS4_ZAD_3_AVLTREE_H
#define PS4_ZAD_3_AVLTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*******************
 *  AVL tree node
 *******************/
struct t_AVLnode {
    char *word;
    struct t_AVLnode *left, *right;
    uint32_t height;
    int tree_size;
};

/****************************
 * AVL printing aux struct
 ****************************/
struct trunk {
    struct trunk *prev;
    char *str;
};

void AVLDestroy(struct t_AVLnode **const root);

void AVLInsert(struct t_AVLnode **const root, const char *const word);

void AVLRemove(struct t_AVLnode **const root, const char *const word);

bool AVLSearch(const struct t_AVLnode *const root, const char *const search_word);

uint32_t AVLSearchPrefix(const struct t_AVLnode *const root, const char *const prefix);

#define EMPTY_DICT_MESSAGE "   [EMPTY]\n"
#define AVLPrint(root) do{if(root) {printf("\n"); AVLPrintFunction(root, NULL, false);} else printf(EMPTY_DICT_MESSAGE); printf("\nLEGEND: (<word>; <height>, <number of word in subtree>)\n");} while(0)

void AVLPrintFunction(const struct t_AVLnode *const root, struct trunk *const prev, const bool is_right);


#endif //PS4_ZAD_3_AVLTREE_H
