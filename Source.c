//8 zadatak
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>             
#include <stdlib.h>           

struct treeNode;

typedef struct treeNode Node;       
typedef struct treeNode* Tree;   
typedef struct treeNode* Position; 

struct treeNode {
    int value;   
    Tree left;   
    Tree right; 
};

int Menu(Tree* tree);         
Tree DeleteNode(int x, Tree tree); 
int PrintInorder(Tree tree);    
int PrintPreorder(Tree tree);    
int PrintPostorder(Tree tree);   
int PrintLevelOrder(Tree tree);  
int PrintLevel(Tree tree, int level); 
int Height(Tree tree);           
Tree InsertNode(int x, Tree tree); 
Tree CreateNode(int x);       
Position FindNode(int x, Tree tree); 
Position FindMin(Tree tree);     
Position FindMax(Tree tree);      
Tree CreateEmptyTree(Tree tree);  
int DeleteAll(Tree tree);    

int main() {
    Tree tree = CreateEmptyTree(NULL); 
    Menu(&tree);                    
    DeleteAll(tree);                 
    return 0;                       
}

Tree CreateEmptyTree(Tree tree) {
    (void)tree;     
    return NULL;      
}

Position FindMin(Tree tree) {
    if (tree == NULL) return NULL;   
    while (tree->left != NULL)       
        tree = tree->left;               
    return tree;                         
}

Position FindMax(Tree tree) {
    if (tree == NULL) return NULL;       
    while (tree->right != NULL)          
        tree = tree->right;              
    return tree;                         
}

Position FindNode(int x, Tree tree) {
    while (tree != NULL) {              
        if (x == tree->value) return tree;
        if (x < tree->value) tree = tree->left;
        else tree = tree->right;        
    }
    return NULL; 
}

Tree CreateNode(int x) {
    Tree node = (Tree)malloc(sizeof(Node)); 
    if (node == NULL) printf("greska u alokaciji mem");
    node->value = x;     
    node->left = node->right = NULL;
    return node;       
}

Tree InsertNode(int x, Tree tree) {
    if (tree == NULL) return CreateNode(x); //ako nema mjesta, dodaj novi čvor
    if (x < tree->value) tree->left = InsertNode(x, tree->left);  //ako manji → lijevo
    else if (x > tree->value) tree->right = InsertNode(x, tree->right); //ako veći → desno
    return tree;
}

Tree DeleteNode(int x, Tree tree) {
    if (tree == NULL) return NULL; 
    if (x < tree->value) tree->left = DeleteNode(x, tree->left); //trazi lijevo
    else if (x > tree->value) tree->right = DeleteNode(x, tree->right); 
    else { 
        if (tree->left == NULL) { 
            Tree right = tree->right;
            free(tree);             // oslobadamo memoriju
            return right;          
        }
        if (tree->right == NULL) {  //ako nema desno dijete
            Tree left = tree->left;
            free(tree);           
            return left;            //vracamo lijevo dijete kao novi cvor
        }
        //ako ima oba djeteta
        Position minRight = FindMin(tree->right); //najmanji iz desnog podstabla
        tree->value = minRight->value;            //zamjena vrijednosti
        tree->right = DeleteNode(minRight->value, tree->right); //briše taj cvor
    }
    return tree;
}

int PrintInorder(Tree tree) { //inorder lijevo -> root -> desno
    if (tree == NULL) return 0; 
    PrintInorder(tree->left);     //ispisi lijevo podstablo
    printf("%d ", tree->value);
    PrintInorder(tree->right);    //ispisi desno podstablo
    return 0;
}

int PrintPreorder(Tree tree) { //preorder root -> lijevo -> desno
    if (tree == NULL) return 0;
    printf("%d ", tree->value);   //root
    PrintPreorder(tree->left);
    PrintPreorder(tree->right);
    return 0;
}

int PrintPostorder(Tree tree) { //postorder lijevo -> desno -> root
    if (tree == NULL) return 0;
    PrintPostorder(tree->left);
    PrintPostorder(tree->right);
    printf("%d ", tree->value);
    return 0;
}

int Height(Tree tree) {
    if (tree == NULL) return 0;
    int leftH = Height(tree->left);
    int rightH = Height(tree->right);
    return (leftH > rightH ? leftH : rightH) + 1; //max(lijevo, desno) + 1
}

int PrintLevel(Tree tree, int level) {
    if (tree == NULL) return 0;
    if (level == 1) printf("%d ", tree->value); //ako smo na trazenoj razini
    PrintLevel(tree->left, level - 1);  //rekurzija lijevo
    PrintLevel(tree->right, level - 1);
    return 0;
}

int PrintLevelOrder(Tree tree) {
    int h = Height(tree);  //koliko razina ima stablo
    for (int i = 1; i <= h; i++)
        PrintLevel(tree, i); 
    return 0;
}

int DeleteAll(Tree tree) {
    if (!tree) return 0;
    DeleteAll(tree->left); 
    DeleteAll(tree->right); //oslobađanje desnog podstabla
    free(tree);             //oslobađanje korijena
    return 0;
}

int Menu(Tree* tree) {
    int izbor, x;     
    do {
        printf("\nIzbor: \n");
        printf("1 Unesi novi el\n");
        printf("2 Ispis inorder\n");
        printf("3 Ispis preorder\n");
        printf("4 Ispis postorder\n");
        printf("5 Ispis level order\n");
        printf("6 Pronadi element\n");
        printf("7 Brisi element\n");
        printf("0 Izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &izbor) != 1) return 0;

        switch (izbor) {
        case 1:
            printf("Unesi broj: ");
            if (scanf("%d", &x) == 1) *tree = InsertNode(x, *tree);
            break;
        case 2:
            printf("Inorder: ");
            PrintInorder(*tree); printf("\n");
            break;
        case 3:
            printf("Preorder: ");
            PrintPreorder(*tree); printf("\n");
            break;
        case 4:
            printf("Postorder: ");
            PrintPostorder(*tree); printf("\n");
            break;
        case 5:
            printf("Level order: ");
            PrintLevelOrder(*tree); printf("\n");
            break;
        case 6:
            printf("Trazi broj: ");
            if (scanf("%d", &x) == 1) {
                Position p = FindNode(x, *tree);
                printf(p ? "Pronaden\n" : "Nije pronaden\n");
            }
            break;
        case 7:
            printf("Brisi broj: ");
            if (scanf("%d", &x) == 1) *tree = DeleteNode(x, *tree); 
            break;
        case 0:
            break;
        default:
            printf("Ponovo odaberi\n");
        }
    } while (izbor != 0);
    return 0;
}
