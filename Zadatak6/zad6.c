#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Artikal {
    char naziv[100];
    int kolicina;
    float cijena;
    struct Artikal* next;  //pokazivac na sljedeci proizvod
} Artikal;

typedef struct Racun {  //struktura za pojedini racun
    char datum[11];
    Artikal* artikli;  //pokazivac na veznu listu artikala za taj racun
    struct Racun* next;  //pokazivac na iduci racun
} Racun;


int compareDates(const char* d1, const char* d2) {
    return strcmp(d1, d2);  //usporeduje dva stringa, sortiranje datuma
}

void insertArtikal(Artikal** head, Artikal* novi) {  //f-ja dodaje novi proizvod u listu tako da lista ostane sortirana po nazivu
    if (*head == NULL || strcmp(novi->naziv, (*head)->naziv) < 0) {
        novi->next = *head;
        *head = novi;
        return;
    }

    Artikal* curr = *head;
    while (curr->next && strcmp(novi->naziv, curr->next->naziv) > 0)
        curr = curr->next;

    novi->next = curr->next;  //ubacivanje tj neka novi cvor pokazuje na sljedeci cvor
    curr->next = novi;  //spajamo prethodni cvor s novim cvorom
}

void insertRacun(Racun** head, Racun* novi) {  //ubacivanje racuna u vezanu listu
    if (*head == NULL || compareDates(novi->datum, (*head)->datum) < 0) { //sortiraj po datumima
        novi->next = *head;
        *head = novi;
        return;
    }

    Racun* curr = *head;
    while (curr->next && compareDates(novi->datum, curr->next->datum) > 0)
        curr = curr->next;

    novi->next = curr->next;
    curr->next = novi;
}

Racun* readRacunFile(const char* filename) { //ucitavanje racuna iz datoteke
    FILE* f = fopen(filename, "r");  //otvara datoteku za citanje
    if (!f) {
        printf("Greska, ne mogu otvoriti %s\n", filename); //ispis za gresku
        return NULL;
    }

    Racun* r = malloc(sizeof(Racun));  //dinamicki alocira novi racun
    r->artikli = NULL;
    r->next = NULL;

    fgets(r->datum, 11, f); //cita datum
    fgetc(f); //cita i uklanja \n

    char line[256];
    while (fgets(line, sizeof(line), f)) {  //citamo liniju po liniju dok ne dodemo do kraja fajla
        Artikal* a = malloc(sizeof(Artikal));  //din al novi proizvod
        a->next = NULL;

        sscanf(line, "%s %d %f", a->naziv, &a->kolicina, &a->cijena);  //razbijamo na tri podatka iz jedne linije

        insertArtikal(&r->artikli, a);  //ubacuje u sortiranu listu po nazivu
    }

    fclose(f); //zatvaramo dat
    return r;
}

void Sum(Racun* head, const char* artikl, const char* odDatuma, const char* doDatuma) {  //sumiranje proizvoda
    int ukupnaKolicina = 0;
    float ukupnoNovca = 0;

    Racun* r = head;
    while (r) {  //prolazimo kroz sve racune
        if (compareDates(r->datum, odDatuma) >= 0 && compareDates(r->datum, doDatuma) <= 0) { //provjera je li datum unutar intervala

            Artikal* a = r->artikli;
            while (a) {
                if (strcmp(a->naziv, artikl) == 0) {  //provjera je li racun prisutan u racunu
                    ukupnaKolicina += a->kolicina;
                    ukupnoNovca += a->kolicina * a->cijena;
                }
                a = a->next;
            }
        }
        r = r->next;
    }

    printf("\nRezultat:\n");
    printf("Artikl: %s\n", artikl);
    printf("Ukupno kupljeno: %d kom\n", ukupnaKolicina);
    printf("Ukupno potroseno: %.2f EUR\n", ukupnoNovca);
}
void freeArtikli(Artikal* head) {
    Artikal* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeRacuni(Racun* head) {
    Racun* temp;
    while (head) {
        temp = head;
        head = head->next;

        freeArtikli(temp->artikli);
        free(temp);
    }
}

int main() {
    FILE* f = fopen("racuni.txt", "r");  //otvara file za citanje
    if (!f) {
        printf("Greska, racuni.txt nije pronaden!\n");
        return 1;
    }

    Racun* listaRacuna = NULL;
    char filename[100];

    while (fgets(filename, sizeof(filename), f)) { //cita red po red
        filename[strcspn(filename, "\n")] = 0;

        Racun* r = readRacunFile(filename); //ucitava sadrzaj racuna
        if (r)
            insertRacun(&listaRacuna, r);  //ubacuje racun u vezanu listu sortiranu po datumu
    }

    fclose(f);

    char artikl[100];
    char odDatuma[11], doDatuma[11];

    printf("Unesite naziv artikla: ");
    scanf("%s", artikl);
    printf("Unesite pocetni datum (YYYY-MM-DD): ");
    scanf("%s", odDatuma);
    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    scanf("%s", doDatuma);

    Sum(listaRacuna, artikl, odDatuma, doDatuma);

    freeRacuni(listaRacuna);

    return 0;
}