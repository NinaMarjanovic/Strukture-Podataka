#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




typedef struct Osoba {
    char ime[50];
    char prezime[50];
    int godina_rodenja;
    struct Osoba* next;
} Osoba;

// dodaj na po etak
void dodajNaPocetak(Osoba** head) {   //prima adresu pokaziva a na po etak liste
    Osoba* nova = (Osoba*)malloc(sizeof(Osoba)); //rezrvira novi element (u listi) u memoriji
    printf("Unesi ime: ");
    scanf("%49s", nova->ime);
    printf("Unesi prezime: ");
    scanf("%49s", nova->prezime);
    printf("Unesi godinu rodenja: ");
    scanf("%d", &nova->godina_rodenja);

    nova->next = *head; //nova osoba pokazuje na stari prvi element	
    *head = nova;  //nova osoba postaje novi prvi element
}

// ispis liste
void ispisiListu(Osoba* head) {
    if (!head) {
        printf("Lista je prazna.\n");
        return;
    }
    while (head) { //prolazi kroz sve
        printf("%s %s, %d\n", head->ime, head->prezime, head->godina_rodenja);
        head = head->next; //mi e se na sljede i
    }
}

// dodaj na kraj
void dodajNaKraj(Osoba** head) {
    Osoba* nova = (Osoba*)malloc(sizeof(Osoba));
    printf("Unesi ime: ");
    scanf("%49s", nova->ime);
    printf("Unesi prezime: ");
    scanf("%49s", nova->prezime);
    printf("Unesi godinu rodenja: ");
    scanf("%d", &nova->godina_rodenja);
    nova->next = NULL;

    if (!*head) {
        *head = nova;
        return;
    }

    Osoba* temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = nova;
}

// pronadi po prezimenu
Osoba* pronadi(Osoba* head, const char* prezime) {
    while (head) {
        if (strcmp(head->prezime, prezime) == 0) return head; //ako su prezimena ista 0
        head = head->next;
    }
    return NULL;
}

// obrisi po prezimenu
void obrisi(Osoba** head, const char* prezime) {
    Osoba* temp = *head;
    Osoba* prev = NULL;

    while (temp && strcmp(temp->prezime, prezime) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Osoba s prezimenom %s nije pronadena.\n", prezime);
        return;
    }

    if (!prev) *head = temp->next;  // bri e prvi element
    else prev->next = temp->next;

    free(temp);
    printf("Obrisano.\n");
}

//dr i trenutni i prethodni element
//kad ga na e prispoji pokaziva e



int main() {
    Osoba* head = NULL;
    int izbor;
    char trazenoPrezime[50];

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj na pocetak\n");
        printf("2. Dodaj na kraj\n");
        printf("3. Ispisi listu\n");
        printf("4. Pronadi po prezimenu\n");
        printf("5. Obrisi po prezimenu\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1: dodajNaPocetak(&head); break;
        case 2: dodajNaKraj(&head); break;
        case 3: ispisiListu(head); break;
        case 4:
            printf("Unesi prezime za pretragu: ");
            scanf("%49s", trazenoPrezime);
            Osoba* p = pronadi(head, trazenoPrezime);
            if (p) printf("Pronadeno: %s %s, %d\n", p->ime, p->prezime, p->godina_rodenja);
            else printf("Nema takve osobe.\n");
            break;
        case 5:
            printf("Unesi prezime za brisanje: ");
            scanf("%49s", trazenoPrezime);
            obrisi(&head, trazenoPrezime);
            break;
        case 0: printf("Izlaz.\n"); break;
        default: printf("Nepostojeca opcija!\n");
        }
    } while (izbor != 0);

    //petlja dowhile stalno ponavlja izbornik
    //switch bira funkciju
    //case 0 izlazi

    return 0;
}