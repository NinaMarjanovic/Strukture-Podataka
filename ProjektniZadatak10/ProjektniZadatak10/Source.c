#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 64
#define MAXFILE 128

typedef struct cityNode {
    char name[MAX];
    int pop;
    struct cityNode* left;
    struct cityNode* right;
} CityNode;

typedef struct countryList {
    char name[MAX];
    char filename[MAXFILE];
    CityNode* root;
    struct countryList* next;
} CountryList;

typedef struct cityList {
    char name[MAX];
    int pop;
    struct cityList* next;
} CityList;

typedef struct countryTree {
    char name[MAX];
    char filename[MAXFILE];
    CityList* cities;             /* sortirana lista gradova */
    struct countryTree* left;
    struct countryTree* right;
} CountryTree;


void loadCitiesIntoList(CityList** head, const char* filename);
void menu_B(CountryTree* root);
void menu_A(CountryList* head);
void loadAll_B(CountryTree** root, const char* drzaveFile);
void loadAll_A(CountryList** head, const char* drzaveFile);
void loadCitiesIntoBT(CityNode** root, const char* filename);
void freeAllB(CountryTree* root);
void printAllB(CountryTree* root);
CountryTree* findCountryInTree(CountryTree* root, const char* name);
CountryTree* insertCountryBST(CountryTree* root, const char* name, const char* filename);
CountryTree* createCountryTreeNode(const char* name, const char* filename);
void freeCityList(CityList* head);
void printCityListAbove(CityList* head, int threshold);
void printCityList(CityList* head);
void addCitySortedList(CityList** head, const char* name, int pop);
CityList* createCityListNode(const char* name, int pop);
void freeAllA(CountryList* head);
void printAllA(CountryList* head);
CountryList* findCountryInList(CountryList* head, const char* name);
void addCountrySortedList(CountryList** head, CountryList* newNode);
CountryList* createCountryListNode(const char* name, const char* filename);
int compareCity(int popA, const char* nameA, int popB, const char* nameB);
CityNode* createCityNode(const char* name, int pop);
CityNode* insertCityToBT(CityNode* root, const char* name, int pop);
void printCityBT(CityNode* root);
void printCityBSTAbove(CityNode* root, int limit);
void freeCityBST(CityNode* root);

int main() {
    int mode;
    printf("Odaberi dio zadatka:\n");
    printf("1 - A (lista drzava + BST gradova)\n");
    printf("2 - B (BST drzava + lista gradova)\n");
    printf("Odabir: ");
    scanf("%d", &mode);

    if (mode == 1) {
        CountryList* head = NULL;
        loadAll_A(&head, "drzave.txt");
        menu_A(head);
        freeAllA(head);
    }
    else if (mode == 2) {
        CountryTree* root = NULL;
        loadAll_B(&root, "drzave.txt");
        menu_B(root);
        freeAllB(root);
    }
    else {
        printf("Nepostojeca opcija.\n");
    }

    return 0;
}

int compareCity(int popA, const char* nameA, int popB, const char* nameB) {
    if (popA < popB)
        return -1;
    if (popA > popB)
        return 1;
    return strcmp(nameA, nameB);
}

//A)
//novi cvor za grad
CityNode* createCityNode(const char* name, int pop) {
    CityNode* n = (CityNode*)malloc(sizeof(CityNode));
    if (n == NULL) {
        printf("Greska u alokaciji\n");
        return NULL;
    }
    strcpy(n->name, name);
    n->pop = pop;
    n->left = NULL;
    n->right = NULL;
    return n;
}

//ubacuje grad u bibnasrno stablo
CityNode* insertCityToBT(CityNode* root, const char* name, int pop) {
    if (root == NULL) {
        return createCityNode(name, pop);
    }

    int cmp = compareCity(pop, name, root->pop, root->name);

    if (cmp < 0)
        root->left = insertCityToBT(root->left, name, pop);
    else if (cmp > 0)
        root->right = insertCityToBT(root->right, name, pop);
    else {

    }
    return root;
}
//ispis gradova inorder
void printCityBT(CityNode* root) {
    if (root == NULL) return;

    printCityBT(root->left);

    printf("   - %-15s %d\n", root->name, root->pop);

    printCityBT(root->right);
}

void printCityBSTAbove(CityNode* root, int limit) {
    if (root == NULL) return;

    if (root->pop <= limit) {

        printCityBSTAbove(root->right, limit);
    }
    else {

        printCityBSTAbove(root->left, limit);
        printf("   - %-15s %d\n", root->name, root->pop);
        printCityBSTAbove(root->right, limit);
    }
}
void freeCityBST(CityNode* root) {
    if (!root) return;

    freeCityBST(root->left);
    freeCityBST(root->right);
    free(root);
}
//cvor drzave za listu
CountryList* createCountryListNode(const char* name, const char* filename) {
    CountryList* c = (CountryList*)malloc(sizeof(CountryList));
    if (!c) {
        printf("Greska u alokaciji \n");
        return NULL;
    }
    strcpy(c->name, name);
    strcpy(c->filename, filename);
    c->root = NULL;
    c->next = NULL;
    return c;
}

void addCountrySortedList(CountryList** head, CountryList* newNode) {
    if (*head == NULL || strcmp(newNode->name, (*head)->name) < 0) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    CountryList* tmp = *head;
    while (tmp->next != NULL && strcmp(tmp->next->name, newNode->name) <= 0) {
        tmp = tmp->next;
    }
    newNode->next = tmp->next;
    tmp->next = newNode;
}

CountryList* findCountryInList(CountryList* head, const char* name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) return head;
        head = head->next;
    }
    return NULL;
}

void printAllA(CountryList* head) {
    printf("\n=== ISPIS (A) ===\n");
    while (head != NULL) {
        printf("\nDrzava: %s\n", head->name);
        printCityBT(head->root);
        head = head->next;
    }
}
void freeAllA(CountryList* head) {
    while (head != NULL) {
        CountryList* next = head->next;
        freeCityBST(head->root);
        free(head);
        head = next;
    }
}


//B)
CityList* createCityListNode(const char* name, int pop) {
    CityList* n = (CityList*)malloc(sizeof(CityList));
    if (!n) {
        printf("Greska u alokaciji\n");
        return NULL;
    }
    strcpy(n->name, name);
    n->pop = pop;
    n->next = NULL;
    return n;
}

void addCitySortedList(CityList** head, const char* name, int pop) {
    CityList* newNode = createCityListNode(name, pop);
    if (!newNode) return;

    if (*head == NULL || compareCity(pop, name, (*head)->pop, (*head)->name) < 0) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    CityList* tmp = *head;
    while (tmp->next != NULL &&
        compareCity(pop, name, tmp->next->pop, tmp->next->name) > 0) {
        tmp = tmp->next;
    }
    newNode->next = tmp->next;
    tmp->next = newNode;
}

void printCityList(CityList* head) {
    while (head != NULL) {
        printf("   - %-15s %d\n", head->name, head->pop);
        head = head->next;
    }
}

void printCityListAbove(CityList* head, int threshold) {
    while (head != NULL && head->pop <= threshold) head = head->next;
    while (head != NULL) {
        printf("   - %-15s %d\n", head->name, head->pop);
        head = head->next;
    }
}

void freeCityList(CityList* head) {
    while (head != NULL) {
        CityList* next = head->next;
        free(head);
        head = next;
    }
}

CountryTree* createCountryTreeNode(const char* name, const char* filename) {
    CountryTree* n = (CountryTree*)malloc(sizeof(CountryTree));
    if (!n) {
        printf("Greska u alokaciji\n");
        return NULL;
    }
    strcpy(n->name, name);
    strcpy(n->filename, filename);
    n->cities = NULL;
    n->left = n->right = NULL;
    return n;
}
CountryTree* insertCountryBST(CountryTree* root, const char* name, const char* filename) {
    if (root == NULL) return createCountryTreeNode(name, filename);

    int c = strcmp(name, root->name);
    if (c < 0) root->left = insertCountryBST(root->left, name, filename);
    else if (c > 0) root->right = insertCountryBST(root->right, name, filename);
    else {

    }
    return root;
}

CountryTree* findCountryInTree(CountryTree* root, const char* name) {
    while (root != NULL) {
        int c = strcmp(name, root->name);
        if (c == 0) return root;
        if (c < 0) root = root->left;
        else root = root->right;
    }
    return NULL;
}
void printAllB(CountryTree* root) {
    if (root == NULL) return;
    printAllB(root->left);

    printf("\nDrzava: %s\n", root->name);
    printCityList(root->cities);

    printAllB(root->right);
}
void freeAllB(CountryTree* root) {
    if (!root) return;
    freeAllB(root->left);
    freeAllB(root->right);
    freeCityList(root->cities);
    free(root);
}

void loadCitiesIntoBT(CityNode** root, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Greska otvaranja file-a \n");
        return;
    }

    char city[MAX];
    int pop;

    while (fscanf(f, "%63s %d", city, &pop) == 2) {
        *root = insertCityToBT(*root, city, pop);
    }
    fclose(f);
}

void loadCitiesIntoList(CityList** head, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Greska otvaranja file-a \n");
        return;
    }

    char city[MAX];
    int pop;

    while (fscanf(f, "%63s %d", city, &pop) == 2) {
        addCitySortedList(head, city, pop);
    }
    fclose(f);
}

void loadAll_A(CountryList** head, const char* drzaveFile) {
    FILE* f = fopen(drzaveFile, "r");
    if (!f) {
        printf("Greska otvaranja \n");
        return;
    }

    char cname[MAX];
    char cfile[MAXFILE];

    while (fscanf(f, "%63s %127s", cname, cfile) == 2) {
        CountryList* c = createCountryListNode(cname, cfile);
        if (!c) continue;

        loadCitiesIntoBT(&c->root, cfile);
        addCountrySortedList(head, c);
    }

    fclose(f);
}

void loadAll_B(CountryTree** root, const char* drzaveFile) {
    FILE* f = fopen(drzaveFile, "r");
    if (!f) {
        printf("Greska otvaranja \n");
        return;
    }

    char cname[MAX];
    char cfile[MAXFILE];

    while (fscanf(f, "%63s %127s", cname, cfile) == 2) {
        *root = insertCountryBST(*root, cname, cfile);

        CountryTree* node = findCountryInTree(*root, cname);
        if (node) {
            loadCitiesIntoList(&node->cities, cfile);
        }
    }

    fclose(f);
}
void menu_A(CountryList* head) {
    int choice;
    char country[MAX];
    int threshold;

    do {
        printf("\n--- IZBORNIK (A) ---\n");
        printf("1 - Ispis svih drzava i gradova\n");
        printf("2 - Pretrazi gradove drzave po pragu stanovnika (pop > X)\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printAllA(head);
            break;

        case 2:
            printf("Unesi drzavu: ");
            scanf("%63s", country);
            printf("Unesi prag stanovnika: ");
            scanf("%d", &threshold);

            {
                CountryList* c = findCountryInList(head, country);
                if (!c) {
                    printf("Nema drzave '%s'.\n", country);
                }
                else {
                    printf("\nGradovi u %s s pop > %d:\n", c->name, threshold);
                    printCityBSTAbove(c->root, threshold);
                }
            }
            break;

        case 0:
            printf("Izlaz.\n");
            break;

        default:
            printf("Nepostojeca opcija.\n");
        }

    } while (choice != 0);
}
void menu_B(CountryTree* root) {
    int choice;
    char country[MAX];
    int threshold;

    do {
        printf("\n--- IZBORNIK (B) ---\n");
        printf("1 - Ispis svih drzava i gradova\n");
        printf("2 - Pretrazi gradove drzave po pragu stanovnika (pop > X)\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("\n=== ISPIS (B) ===\n");
            printAllB(root);
            break;

        case 2:
            printf("Unesi drzavu: ");
            scanf("%63s", country);
            printf("Unesi prag stanovnika: ");
            scanf("%d", &threshold);

            {
                CountryTree* c = findCountryInTree(root, country);
                if (!c) {
                    printf("Nema drzave '%s'.\n", country);
                }
                else {
                    printf("\nGradovi u %s s pop > %d:\n", c->name, threshold);
                    printCityListAbove(c->cities, threshold);
                }
            }
            break;

        case 0:
            printf("Izlaz.\n");
            break;

        default:
            printf("Nepostojeca opcija.\n");
        }

    } while (choice != 0);
}