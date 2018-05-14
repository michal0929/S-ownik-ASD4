/******************************************
 *               AVLtree.c
 ******************************************
 * Adam Bajguz
 * Michał Kierzkowski
 * Grupa: Ps 7, informatyka III semestr
 ******************************************/

#include "AVLtree.h"

#define HEIGHT(node)    ((node) ? (node)->height : 0)
#define BALANCE(node)   ((node) ? HEIGHT((node)->left) - HEIGHT((node)->right) : 0)
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

#define GET_SUB_SIZE(node) ((node) ?(node)->tree_size : 0)

void AVLDestroy(struct t_AVLnode **const root) {
	if (*root) {
		if (!(*root)->right && !(*root)->left) {
			if ((*root)->word)
				free((*root)->word);
			free((*root));
			return;
		}
		AVLDestroy(&((*root)->right));
		AVLDestroy(&((*root)->left));
	}
	*root = NULL;
}

void RotateLeft(struct t_AVLnode **const subtree) __attribute__((always_inline));

void RotateRight(struct t_AVLnode **const subtree) __attribute__((always_inline));

void RotateLeft(struct t_AVLnode **const A) {
	register struct t_AVLnode *B = (*A)->right, *T2 = B->left;

	B->left = *A;
	(*A)->right = T2;

	(*A)->height = MAX(HEIGHT((*A)->left), HEIGHT((*A)->right)) + 1;
	B->height = MAX(HEIGHT(B->left), HEIGHT(B->right)) + 1;

	(*A)->tree_size = GET_SUB_SIZE((*A)->left) + GET_SUB_SIZE((*A)->right) + 1;
	B->tree_size = GET_SUB_SIZE(B->left) + GET_SUB_SIZE(B->right) + 1;

	*A = B;
}

void RotateRight(struct t_AVLnode **const A) {
	register struct t_AVLnode *B = (*A)->left, *T2 = B->right;

	B->right = *A;
	(*A)->left = T2;

	(*A)->height = MAX(HEIGHT((*A)->left), HEIGHT((*A)->right)) + 1;
	B->height = MAX(HEIGHT(B->left), HEIGHT(B->right)) + 1;

	(*A)->tree_size = GET_SUB_SIZE((*A)->left) + GET_SUB_SIZE((*A)->right) + 1;
	B->tree_size = GET_SUB_SIZE(B->left) + GET_SUB_SIZE(B->right) + 1;

	*A = B;
}

void AVLInsert(struct t_AVLnode **const root, const char *const word) {
	register struct t_AVLnode *node = *root;

	/*********************
	 * 1)  BST addition
	 *********************/

	if (!node) {
		struct t_AVLnode *new_node = malloc(sizeof(*new_node));
		char *tmp = malloc(sizeof(*tmp) * (strlen(word) + 1));

		*new_node = (struct t_AVLnode) {tmp, NULL, NULL, 1, 1}; // height = 1 => new node is initially added at leaf
		strcpy(new_node->word, word);
		*root = new_node;
		return;
	}

	if (strcmp(word, node->word) < 0)       //word < node->word
		AVLInsert(&node->left, word);
	else if (strcmp(word, node->word) > 0)  //word > node->word
		AVLInsert(&node->right, word);
	else {                                  //word == node->word
		*root = node;
		return;
	}
	++(node->tree_size);

	/**********************
	 * 2)  AVL rotations
	 **********************/
	node->height = 1 + MAX(HEIGHT(node->left), HEIGHT(node->right));

	register int balance_factor = BALANCE(node);

	if (balance_factor > 1 && strcmp(word, node->left->word) < 0) {           // LL rotation: word < node->left->word
		RotateRight(root);
		return;
	} else if (balance_factor < -1 && strcmp(word, node->right->word) > 0) {  // RR rotation: word > node->right->word
		RotateLeft(root);
		return;
	} else if (balance_factor > 1 && strcmp(word, node->left->word) > 0) {  // LR rotation: word < node->right->word
		RotateLeft(&node->left);
		RotateRight(root);
		return;
	} else if (balance_factor < -1 &&  strcmp(word, node->right->word) < 0) {    // RL rotation: word > node->left->word
		RotateRight(&node->right);
		RotateLeft(root);
		return;
	}
}

void AVLRemove(struct t_AVLnode **const root, const char *const word) {
	register struct t_AVLnode *node = *root;

	/*********************
	 * 1)  BST delete
	 *********************/
	if (!node)
		return;

	if (strcmp(word, node->word) < 0)       // word < node->word => left subtree
		AVLRemove(&node->left, word);
	else if (strcmp(word, node->word) > 0)  // word > node->word => right subtree
		AVLRemove(&node->right, word);
	else {                                  // word == node->word => delete node
		if ((!node->left) || (!node->right)) { // node with 0 or 1 child
			struct t_AVLnode *temp = node->left ? node->left : node->right;

			free(node->word);

			if (!temp) {    // if 0 child
				temp = node;
				node = NULL;
			} else          // if 1 child
				*node = *temp; // copy non-empty child

			free(temp);
		} else {            // node with 2 children
			struct t_AVLnode *temp = node->right; // get the right leaf

			/* get the last left leaf (inorder successor) */
			while (temp->left)
				temp = temp->left;

			strcpy(node->word, temp->word);

			AVLRemove(&node->right, temp->word); // delete inorder successor
		}
	}

	/**********************
	 * 2)  AVL rotations
	 **********************/
	if (!node) {
		*root = node;
		return;
	}

	node->height = 1 + MAX(HEIGHT(node->left), HEIGHT(node->right));
	--(node->tree_size);

	register int balance_factor = BALANCE(node);
	register int balance_factor_L = BALANCE(node->left), balance_factor_R = BALANCE(node->right);

	if (balance_factor > 1 && balance_factor_L >= 0) {              // LL rotation
		RotateRight(root);
		return;
	} else if (balance_factor < -1 && balance_factor_R <= 0) {      // RR rotation
		RotateLeft(root);
		return;
	} else if (balance_factor > 1 && balance_factor_L < 0) {        // LR rotation
		RotateLeft(&node->left);
		RotateRight(root);
		return;
	} else if (balance_factor < -1 && balance_factor_R > 0) {       // RL rotation
		RotateRight(&node->right);
		RotateLeft(root);
		return;
	}

	*root = node;
}

bool AVLSearch(const struct t_AVLnode *const root, const char *const search_word) {
	if (!root)
		return false;

	if (strcmp(search_word, root->word) < 0)        // search_word < root->word
		return AVLSearch(root->left, search_word);
	else if (strcmp(search_word, root->word) > 0)   // search_word > root->word
		return AVLSearch(root->right, search_word);

	return true;
}

const struct t_AVLnode *const AVLSearchPrefixTop(const struct t_AVLnode *const root, const char *const prefix) {
	if (!root)
		return NULL;

	if (strncmp(prefix, root->word, strlen(prefix)) < 0)        // prefix < root->word
		return AVLSearchPrefixTop(root->left, prefix);
	else if (strncmp(prefix, root->word, strlen(prefix)) > 0)   // prefix > root->word
		return AVLSearchPrefixTop(root->right, prefix);

	return root;
}

uint32_t AVLSearchPrefix(const struct t_AVLnode *const root, const char *const prefix) {
	if (!root)
		return 0;

	const struct t_AVLnode *const top = AVLSearchPrefixTop(root, prefix);

	if (!top)
		return 0;

	register uint32_t num = 1;

    const struct t_AVLnode * path = top;

    while(path && (path = path->left)) {
        if (strncmp(prefix, path->word, strlen(prefix)) != 0) {
			if(path)
				num += AVLSearchPrefix(path->right, prefix);
		}
		else
			num += 1 + (path->right ? (path->right->height == 1 ? 1 : path->right->tree_size) : 0);
	}

	path = top;

    while(path && (path = path->right)) {
        if (strncmp(prefix, path->word, strlen(prefix)) != 0) {
			if(path)
				num += AVLSearchPrefix(path->left, prefix);
		}
		else
			num += 1 + (path->left ? (path->left->height == 1 ? 1 : path->left->tree_size) : 0);
	}

	return num;
}

void print_branches(const struct trunk *const ptr) {
	if (!ptr)
		return;

	print_branches(ptr->prev);
	printf("%s", ptr->str);
}

void AVLPrintFunction(const struct t_AVLnode *const root, struct trunk *const prev, const bool is_right) {
	if (!root)
		return;

	struct trunk this_disp = {prev, "    "};
	char *prev_str = this_disp.str;
	AVLPrintFunction(root->right, &this_disp, true);

	if (!prev)
		this_disp.str = "\xC4\xC4\xC4";
	else if (is_right) {
		this_disp.str = "\xDA\xC4\xC4";
		prev_str = "    \xB3";
	} else {
		this_disp.str = "\xC0\xC4\xC4";
		prev->str = prev_str;
	}

	print_branches(&this_disp);
	printf("(%s; %u, %u)\n", root->word, root->height, root->tree_size);

	if (prev)
		prev->str = prev_str;
	this_disp.str = "    \xB3";

	AVLPrintFunction(root->left, &this_disp, false);
}