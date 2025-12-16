#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dir {
    char name[50];        
    struct Dir* parent;  //pok na roditelja
    struct Dir* child;   
    struct Dir* sibling; 
} Dir;

Dir* createDir(char* name, Dir* parent) { //za stvaranje direktorija
    Dir* newDir = (Dir*)malloc(sizeof(Dir));
    strcpy(newDir->name, name);   //kopira ime direktorija u strukturu
    newDir->parent = parent;
    newDir->child = NULL;
    newDir->sibling = NULL; 
    return newDir;  //vraca pokazivac na novi direktorij
}

void makeDir(Dir* current) { //ulazak u direktorij
    char name[50];
    printf("Ime novog direktorija: "); 
    scanf("%s", name);             

    Dir* newDir = createDir(name, current);

    if (current->child == NULL) { 
        current->child = newDir;  //novi direktorij postaje prvo djete
    }
    else {
        Dir* temp = current->child;
        while (temp->sibling != NULL)
            temp = temp->sibling;
        temp->sibling = newDir;        
    }
}

void listDir(Dir* current) {  //ispis sadržaja direktorija
    if (current->child == NULL) {   
        printf("Direktorij je prazan.\n"); 
        return;                          
    }

    Dir* temp = current->child;         
    while (temp != NULL) {             
        printf("%s\n", temp->name);       
        temp = temp->sibling;   
    }
}

Dir* changeDir(Dir* current) {    //ulazak u direktoriji
    char name[50];                        
    printf("Ime direktorija: ");            
    scanf("%s", name);                      

    Dir* temp = current->child;            
    while (temp != NULL) {                  
        if (strcmp(temp->name, name) == 0)   
            return temp;                     
        temp = temp->sibling;                
    }

    printf("Direktorij ne postoji.\n");         
    return current;                   
}


Dir* goBack(Dir* current) { //povratak u prethodni direktorij
    if (current->parent != NULL)           
        return current->parent;           
    return current;                
}

int main() {
    Dir* root = createDir("C:", NULL);     
    Dir* current = root;                    
    int choice;                         

    do {
        printf("\nTrenutni direktorij: %s\n", current->name);
        printf("1 - md\n");  
        printf("2 - cd dir\n"); 
        printf("3 - cd..\n");        
        printf("4 - dir\n");  
        printf("5 - exit\n"); 
        printf("Odabir: ");
        scanf("%d", &choice);              

        switch (choice) {
        case 1:
            makeDir(current);           
            break;
        case 2:
            current = changeDir(current);  
            break;
        case 3:
            current = goBack(current);    
            break;
        case 4:
            listDir(current);         
            break;
        case 5:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Krivi unos\n");  
        }
    } while (choice != 5);                 

    return 0;                                   
}
