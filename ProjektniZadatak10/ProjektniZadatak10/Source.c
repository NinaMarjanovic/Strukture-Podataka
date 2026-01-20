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
    CityList* cities;
    struct countryTree* left;
    struct countryTree* right;
} CountryTree;

int loadCitiesIntoList(CityList** head, const char* filename);
int menu_B(CountryTree* root);
int menu_A(CountryList* head);
int loadAll_B(CountryTree** root, const char* drzaveFile);
int loadAll_A(CountryList** head, const char* drzaveFile);
int loadCitiesIntoBT(CityNode** root, const char* filename);
int freeAllB(CountryTree* root);
int printAllB(CountryTree* root);
CountryTree* findCountryInTree(CountryTree* root, const char* name);
CountryTree* insertCountryBST(CountryTree* root, const char* name, const char* filename);
CountryTree* createCountryTreeNode(const char* name, const char* filename);
int freeCityList(CityList* head);
int printCityListAbove(CityList* head, int threshold);
int printCityList(CityList* head);
int addCitySortedList(CityList** head, const char* name, int pop);
CityList* createCityListNode(const char* name, int pop);
int freeAllA(CountryList* head);
int printAllA(CountryList* head);
CountryList* findCountryInList(CountryList* head, const char* name);
int addCountrySortedList(CountryList** head, CountryList* newNode);
CountryList* createCountryListNode(const char* name, const char* filename);
int compareCity(int popA, const char* nameA, int popB, const char* nameB);
CityNode* createCityNode(const char* name, int pop);
CityNode* insertCityToBT(CityNode* root, const char* name, int pop);
int printCityBT(CityNode* root);
int printCityBSTAbove(CityNode* root, int limit);
int freeCityBST(CityNode* root);

int main() {
    int mode;
    printf("1 - A\n2 - B\nOdabir: ");
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
    return 0;
}


int compareCity(int popA, const char* nameA, int popB, const char* nameB) {
    if (popA < popB) return -1;
    if (popA > popB) return 1;
    return strcmp(nameA, nameB);
}


CityNode* createCityNode(const char* name, int pop) {
    CityNode* n = malloc(sizeof(CityNode));
    if (!n) return NULL;
    strcpy(n->name, name);
    n->pop = pop;
    n->left = n->right = NULL;
    return n;
}

CityNode* insertCityToBT(CityNode* root, const char* name, int pop) {
    if (!root) return createCityNode(name, pop);

    int c = compareCity(pop, name, root->pop, root->name);
    if (c < 0) root->left = insertCityToBT(root->left, name, pop);
    else if (c > 0) root->right = insertCityToBT(root->right, name, pop);

    return root;
}

int printCityBT(CityNode* root) {
    if (!root) return 0;
    printCityBT(root->left);
    printf("   - %-15s %d\n", root->name, root->pop);
    printCityBT(root->right);
    return 0;
}

int printCityBSTAbove(CityNode* root, int limit) {
    if (!root) return 0;

    if (root->pop > limit) {
        printCityBSTAbove(root->left, limit);
        printf("   - %-15s %d\n", root->name, root->pop);
        printCityBSTAbove(root->right, limit);
    }
    else {
        printCityBSTAbove(root->right, limit);
    }
    return 0;
}

int freeCityBST(CityNode* root) {
    if (!root) return 0;
    freeCityBST(root->left);
    freeCityBST(root->right);
    free(root);
    return 0;
}

CountryList* createCountryListNode(const char* name, const char* filename) {
    CountryList* c = malloc(sizeof(CountryList));
    if (!c) return NULL;
    strcpy(c->name, name);
    strcpy(c->filename, filename);
    c->root = NULL;
    c->next = NULL;
    return c;
}

int addCountrySortedList(CountryList** head, CountryList* newNode) {
    if (!*head || strcmp(newNode->name, (*head)->name) < 0) {
        newNode->next = *head;
        *head = newNode;
        return 0;
    }

    CountryList* tmp = *head;
    while (tmp->next && strcmp(tmp->next->name, newNode->name) <= 0)
        tmp = tmp->next;

    newNode->next = tmp->next;
    tmp->next = newNode;
    return 0;
}

CountryList* findCountryInList(CountryList* head, const char* name) {
    while (head) {
        if (!strcmp(head->name, name)) return head;
        head = head->next;
    }
    return NULL;
}

int printAllA(CountryList* head) {
    while (head) {
        printf("\nDrzava: %s\n", head->name);
        printCityBT(head->root);
        head = head->next;
    }
    return 0;
}

int freeAllA(CountryList* head) {
    while (head) {
        CountryList* next = head->next;
        freeCityBST(head->root);
        free(head);
        head = next;
    }
    return 0;
}

CityList* createCityListNode(const char* name, int pop) {
    CityList* n = malloc(sizeof(CityList));
    if (!n) return NULL;
    strcpy(n->name, name);
    n->pop = pop;
    n->next = NULL;
    return n;
}

int addCitySortedList(CityList** head, const char* name, int pop) {
    CityList* n = createCityListNode(name, pop);
    if (!n) return -1;

    if (!*head || compareCity(pop, name, (*head)->pop, (*head)->name) < 0) {
        n->next = *head;
        *head = n;
        return 0;
    }

    CityList* tmp = *head;
    while (tmp->next &&
        compareCity(pop, name, tmp->next->pop, tmp->next->name) > 0)
        tmp = tmp->next;

    n->next = tmp->next;
    tmp->next = n;
    return 0;
}

int printCityList(CityList* head) {
    while (head) {
        printf("   - %-15s %d\n", head->name, head->pop);
        head = head->next;
    }
    return 0;
}

int printCityListAbove(CityList* head, int threshold) {
    while (head && head->pop <= threshold) head = head->next;
    printCityList(head);
    return 0;
}

int freeCityList(CityList* head) {
    while (head) {
        CityList* next = head->next;
        free(head);
        head = next;
    }
    return 0;
}

CountryTree* createCountryTreeNode(const char* name, const char* filename) {
    CountryTree* n = malloc(sizeof(CountryTree));
    if (!n) return NULL;
    strcpy(n->name, name);
    strcpy(n->filename, filename);
    n->cities = NULL;
    n->left = n->right = NULL;
    return n;
}

CountryTree* insertCountryBST(CountryTree* root, const char* name, const char* filename) {
    if (!root) return createCountryTreeNode(name, filename);

    int c = strcmp(name, root->name);
    if (c < 0) root->left = insertCountryBST(root->left, name, filename);
    else if (c > 0) root->right = insertCountryBST(root->right, name, filename);
    return root;
}

CountryTree* findCountryInTree(CountryTree* root, const char* name) {
    if (!root) return NULL;
    int c = strcmp(name, root->name);
    if (!c) return root;
    if (c < 0) return findCountryInTree(root->left, name);
    return findCountryInTree(root->right, name);
}

int printAllB(CountryTree* root) {
    if (!root) return 0;
    printAllB(root->left);
    printf("\nDrzava: %s\n", root->name);
    printCityList(root->cities);
    printAllB(root->right);
    return 0;
}

int freeAllB(CountryTree* root) {
    if (!root) return 0;
    freeAllB(root->left);
    freeAllB(root->right);
    freeCityList(root->cities);
    free(root);
    return 0;
}

int loadCitiesIntoBT(CityNode** root, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return -1;

    char city[MAX];
    int pop;
    while (fscanf(f, "%63s %d", city, &pop) == 2)
        *root = insertCityToBT(*root, city, pop);

    fclose(f);
    return 0;
}

int loadCitiesIntoList(CityList** head, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return -1;

    char city[MAX];
    int pop;
    while (fscanf(f, "%63s %d", city, &pop) == 2)
        addCitySortedList(head, city, pop);

    fclose(f);
    return 0;
}

int loadAll_A(CountryList** head, const char* file) {
    FILE* f = fopen(file, "r");
    if (!f) return -1;

    char name[MAX], fname[MAXFILE];
    while (fscanf(f, "%63s %127s", name, fname) == 2) {
        CountryList* c = createCountryListNode(name, fname);
        loadCitiesIntoBT(&c->root, fname);
        addCountrySortedList(head, c);
    }
    fclose(f);
    return 0;
}

int loadAll_B(CountryTree** root, const char* file) {
    FILE* f = fopen(file, "r");
    if (!f) return -1;

    char name[MAX], fname[MAXFILE];
    while (fscanf(f, "%63s %127s", name, fname) == 2) {
        *root = insertCountryBST(*root, name, fname);
        CountryTree* c = findCountryInTree(*root, name);
        loadCitiesIntoList(&c->cities, fname);
    }
    fclose(f);
    return 0;
}

int menu_A(CountryList* head) {
    int ch, x;
    char name[MAX];
    do {
        scanf("%d", &ch);
        if (ch == 1) printAllA(head);
        if (ch == 2) {
            scanf("%s %d", name, &x);
            CountryList* c = findCountryInList(head, name);
            if (c) printCityBSTAbove(c->root, x);
        }
    } while (ch);
    return 0;
}

int menu_B(CountryTree* root) {
    int ch, x;
    char name[MAX];
    do {
        scanf("%d", &ch);
        if (ch == 1) printAllB(root);
        if (ch == 2) {
            scanf("%s %d", name, &x);
            CountryTree* c = findCountryInTree(root, name);
            if (c) printCityListAbove(c->cities, x);
        }
    } while (ch);
    return 0;
}
