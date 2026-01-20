#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 64
#define MAXFILE 128
#define TABLE_SIZE 11

typedef struct CityNode {
    char name[MAX];
    int pop;
    struct CityNode* left;
    struct CityNode* right;
} CityNode;

typedef struct CountryNode {
    char name[MAX];
    char filename[MAXFILE];
    CityNode* cities;
    struct CountryNode* next;
} CountryNode;

typedef struct HashTable {
    int size;
    CountryNode* buckets[TABLE_SIZE];
} HashTable;

int hashKey(char* countryName, int size);
int initHash(HashTable* ht);
int compareCity(int pop1, char* name1, int pop2, char* name2);
CityNode* createCity(char* name, int pop);
CityNode* insertCity(CityNode* root, char* name, int pop);
int printCityBST(CityNode* root);
int printCityAbove(CityNode* root, int limit);
int freeCityBST(CityNode* root);
CountryNode* createCountry(char* name, char* filename);
int insertCountrySorted(CountryNode** head, CountryNode* newNode);
CountryNode* findCountry(HashTable* ht, char* countryName);
int freeCountryList(CountryNode* head);
int loadCities(CityNode** root, char* filename);
int loadCountries(HashTable* ht, char* drzaveFile);
int printAll(HashTable* ht);
int menu(HashTable* ht);

int main() {
    HashTable ht;
    int i;

    initHash(&ht);
    loadCountries(&ht, "drzave.txt");
    menu(&ht);

    for (i = 0; i < ht.size; i++) {
        freeCountryList(ht.buckets[i]);
    }

    return 0;
}

int hashKey(char* countryName, int size) {
    int sum = 0;
    for (int i = 0; i < 5 && countryName[i] != '\0'; i++) {
        sum += countryName[i];
    }
    return sum % size;
}

int initHash(HashTable* ht) {
    ht->size = TABLE_SIZE;
    for (int i = 0; i < ht->size; i++)
        ht->buckets[i] = NULL;
    return 0;
}

int compareCity(int pop1, char* name1, int pop2, char* name2) {
    if (pop1 < pop2) return -1;
    if (pop1 > pop2) return 1;
    return strcmp(name1, name2);
}

CityNode* createCity(char* name, int pop) {
    CityNode* n = malloc(sizeof(CityNode));
    if (!n) return NULL;

    strcpy(n->name, name);
    n->pop = pop;
    n->left = n->right = NULL;
    return n;
}

CityNode* insertCity(CityNode* root, char* name, int pop) {
    if (!root) return createCity(name, pop);

    int c = compareCity(pop, name, root->pop, root->name);
    if (c < 0) root->left = insertCity(root->left, name, pop);
    else if (c > 0) root->right = insertCity(root->right, name, pop);

    return root;
}

int printCityBST(CityNode* root) {
    if (!root) return 0;
    printCityBST(root->left);
    printf("%s %d\n", root->name, root->pop);
    printCityBST(root->right);
    return 0;
}

int printCityAbove(CityNode* root, int limit) {
    if (!root) return 0;

    if (root->pop > limit) {
        printCityAbove(root->left, limit);
        printf("%s %d\n", root->name, root->pop);
        printCityAbove(root->right, limit);
    }
    else {
        printCityAbove(root->right, limit);
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

CountryNode* createCountry(char* name, char* filename) {
    CountryNode* c = malloc(sizeof(CountryNode));
    if (!c) return NULL;

    strcpy(c->name, name);
    strcpy(c->filename, filename);
    c->cities = NULL;
    c->next = NULL;
    return c;
}

int insertCountrySorted(CountryNode** head, CountryNode* newNode) {
    if (!*head || strcmp(newNode->name, (*head)->name) < 0) {
        newNode->next = *head;
        *head = newNode;
        return 0;
    }

    CountryNode* cur = *head;
    while (cur->next && strcmp(cur->next->name, newNode->name) < 0)
        cur = cur->next;

    if (cur->next && strcmp(cur->next->name, newNode->name) == 0) {
        freeCityBST(newNode->cities);
        free(newNode);
        return -1;
    }

    newNode->next = cur->next;
    cur->next = newNode;
    return 0;
}

CountryNode* findCountry(HashTable* ht, char* countryName) {
    int index = hashKey(countryName, ht->size);
    CountryNode* cur = ht->buckets[index];

    while (cur) {
        int cmp = strcmp(cur->name, countryName);
        if (cmp == 0) return cur;
        if (cmp > 0) return NULL;
        cur = cur->next;
    }
    return NULL;
}

int freeCountryList(CountryNode* head) {
    while (head) {
        CountryNode* next = head->next;
        freeCityBST(head->cities);
        free(head);
        head = next;
    }
    return 0;
}

int loadCities(CityNode** root, char* filename) {
    FILE* f = fopen(filename, "r");
    char city[MAX];
    int pop;

    if (!f) return -1;

    while (fscanf(f, "%63s %d", city, &pop) == 2)
        *root = insertCity(*root, city, pop);

    fclose(f);
    return 0;
}

int loadCountries(HashTable* ht, char* drzaveFile) {
    FILE* f = fopen(drzaveFile, "r");
    char cname[MAX], cfile[MAXFILE];

    if (!f) return -1;

    while (fscanf(f, "%63s %127s", cname, cfile) == 2) {
        CountryNode* c = createCountry(cname, cfile);
        int idx;

        loadCities(&c->cities, cfile);
        idx = hashKey(cname, ht->size);
        insertCountrySorted(&ht->buckets[idx], c);
    }

    fclose(f);
    return 0;
}

int printAll(HashTable* ht) {
    printf("\nISPIS HASH TABLICE\n");
    for (int i = 0; i < ht->size; i++) {
        CountryNode* cur = ht->buckets[i];
        if (!cur) continue;

        printf("\nBucket %d:\n", i);
        while (cur) {
            printf("Drzava: %s\n", cur->name);
            printCityBST(cur->cities);
            cur = cur->next;
            printf("\n");
        }
    }
    return 0;
}

int menu(HashTable* ht) {
    int choice, limit;
    char country[MAX];
    CountryNode* c;

    do {
        printf("\nMeni: \n");
        printf("1 - Ispis svih drzava i gradova\n");
        printf("2 - Pretraga gradova (pop > X)\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printAll(ht);
        }
        else if (choice == 2) {
            printf("Unesi drzavu: ");
            scanf("%63s", country);

            printf("Unesi prag X: ");
            scanf("%d", &limit);

            c = findCountry(ht, country);
            if (!c)
                printf("Nema drzave '%s'\n", country);
            else
                printCityAbove(c->cities, limit);
        }
    } while (choice != 0);

    return 0;
}