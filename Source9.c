#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10 

typedef struct Node {
    int value;         
    struct Node* left; 
    struct Node* right; 
} Node;


int printTree(Node* root, int level);     
Node* createNode(int value);               
Node* insert(Node* root, int x);          
int replace(Node* root);                
int ToFile(Node* root, FILE* f);          
int freeTree(Node* root);                 

int main() {
    srand((unsigned)time(NULL));  //inicijalizacija generatora slučajnih brojeva
    int a[N];              

    printf("Generirani Brojevi: ");
    for (int i = 0; i < N; i++) {
        a[i] = (rand() % 81) + 10;  //generiranje brojeva između 10 i 90
        printf("%d ", a[i]);   
    }
    printf("\n");

    Node* root = NULL; 

    for (int i = 0; i < N; i++) {
        root = insert(root, a[i]);
    }

    FILE* f = fopen("inorder.txt", "w");  //otvaranje dat za upis inorder
    if (f == NULL) {
        printf("Greska pri otvaranju datoteke\n");
        freeTree(root);
        return 1;
    }

    printf("\nStablo nakon (a) - insert:\n");
    printTree(root, 0);  //ispis stabla na ekran

    fprintf(f, "INORDER nakon (a) - nakon insert:\n");
    ToFile(root, f);     //upis stabla u datoteku
    fprintf(f, "\n\n");

    replace(root); //zamjena vrijednosti čvorova sumom djece

    printf("\nStablo nakon (b) - replace:\n");
    printTree(root, 0); 

    fprintf(f, "INORDER nakon (b) - nakon replace:\n");
    ToFile(root, f);
    fprintf(f, "\n");

    fclose(f);          
    printf("\nUpisano u datoteku: inorder.txt\n");

    freeTree(root);     
    root = NULL;

    return 0;
}


Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node)); 
    if (newNode == NULL) {
        printf("Greska u alokaciji!\n");
        return NULL;
    }
    newNode->value = value;   
    newNode->left = NULL;     
    newNode->right = NULL;    
    return newNode;        
}

Node* insert(Node* root, int x) {
    if (root == NULL) {
        return createNode(x);   //ako nema korijena, stvori novi cvor
    }
    if (x >= root->value) {
        root->left = insert(root->left, x); 
    }
    else {
        root->right = insert(root->right, x); 
    }
    return root;  
}

int replace(Node* root) {
    if (root == NULL) return 0;   

    int prevValue = root->value;  //sprema originalnu vrijednost

    int leftSum = replace(root->left);   //rekurzija lijevo
    int rightSum = replace(root->right); //rekurz. desno

    root->value = leftSum + rightSum; //zamjena vrijednosti čvora sumom djece

    return root->value + prevValue;      //vraca novu vrijednost+original
}

int ToFile(Node* root, FILE* f) {
    if (root == NULL) return 0;

    ToFile(root->left, f);             
    fprintf(f, "%d ", root->value);    
    ToFile(root->right, f);           
    return 0;
}

int freeTree(Node* root) {
    if (root == NULL) return 0;

    freeTree(root->left); 
    freeTree(root->right);  
    free(root);            
    return 0;
}

int printTree(Node* root, int level) {
    if (root == NULL) return 0;

    printTree(root->right, level + 1); //rekurzija desno (ispis desnog podstabla prvo)

    for (int i = 0; i < level; i++) printf("    ");
    printf("%d\n", root->value);           

    printTree(root->left, level + 1);  
    return 0;
}
