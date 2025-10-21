#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Person* Position;  //position -pokazivac na strukturu Persone

typedef struct Person {
    char name[50];
    char lastname[50];
    int year;
    Position next; //pokazivac na iducu osobu u listi
} Person;

Position createPerson() { //funkcija za dodavanje nove osobe
    Position newPerson = (Position)malloc(sizeof(Person)); //dinamicki alociramo memoriju za novu osobu

    if (!newPerson) {
        printf("Greška pri alokaciji memorije!\n");
        return NULL;
    }

    printf("Unesite ime, prezime i godinu rodenja: ");
    scanf(" %49s", newPerson->name);  //upisi ime osobe tocno u polje name
    scanf(" %49s", newPerson->lastname);
    scanf("%d", &newPerson->year);

    newPerson->next = NULL; //postaviti cemo pokazivac na NULL zato sto je lista prazna i nema jos sljedece osobe

    return newPerson; //vracamo pokazivac na novog clana
}

int Beginning(Position head) {
    Position newPerson = createPerson();
    if (!newPerson) return -1;

    newPerson->next = head->next; //nova osoba nam pokazuje na dosadasnji prvi el

    head->next = newPerson; //head(pocetak liste) nam je pokazivac na novu osobu koja je sada prva u listi

    return 0;
}

int EndOfList(Position head) { //funckija za dodavanje osobe na kraju liste
    Position newPerson = createPerson();
    if (!newPerson) return -1;

    Position q = head; //idemo na kraj liste
    while (q->next != NULL) {
        q = q->next;
    }

    q->next = newPerson; //zadnji element sad pokazuje na novu osobu

    return 0;
}

void printList(Position head) { //funkcija za ispis liste
    Position q = head->next;  // prva stvarna osoba (preskaèemo Head)

    if (q == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    printf("\nIspis liste \n");
    while (q != NULL) {
        printf("Ime: %s, Prezime: %s, Godina roðenja: %d\n", q->name, q->lastname, q->year); //ispis svakog clana

        q = q->next; //sljedeca osoba
    }
    printf("\n");
}

Position findByLastName(Position head, const char* targetLastname) { //funkcija za pronalazanje osobe po prezimenu
    Position temp = head->next;  // prva stvarna osoba

    while (temp != NULL) {
        if (strcmp(temp->lastname, targetLastname) == 0) { //usporedujemo uneseno prezime sa svakim u listi
            return temp;  // ako su ista, vracamo pokazivac na tu osobu
        }
        temp = temp->next;  //ako nije, idemo dalje
    }

    return NULL;  //osoba nije pronadena
}

int deleteByLastName(Position head, const char* targetLastname) { //brisanje osobe po prezimenu
    Position q = head;   // q pokazuje na osobu prije one koju možda brišemo
    Position temp = q->next;  // temp pokazuje na trenutnu osobu koju provjeravamo

    while (temp != NULL && strcmp(temp->lastname, targetLastname) != 0) {
        q = temp;    // q ide na trenutnu
        temp = temp->next;  // temp ide na sljedeæu
    }

    if (temp != NULL) {
        q->next = temp->next; //brisemo osobu tako da preskocimo temp???

        free(temp); //oslabadamo memoriju
        temp = NULL; //postavljamo temp na NULL???

        printf("Osoba je uspjesno obrisana.\n");
        return 0;
    }
    else {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", targetLastname);
        return -1;
    }
}

void menu() { //funkcija izbornik kojom upravljamo listom
    Person Head = { .name = "", .lastname = "", .year = 0, .next = NULL }; //head nam je prazna osoba koja sluzi kao pocetak liste

    int choice;
    char searchLastname[50];

    do {
        printf("\nIzaberi opciju: \n");
        printf("1. Dodaj osobu na pocetak\n");
        printf("2. Dodaj osobu na kraj\n");
        printf("3. Ispisi listu\n");
        printf("4. Pronadi osobu po prezimenu\n");
        printf("5. Obrisi osobu po prezimenu\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            insertAtBeginning(&Head);
            break;
        case 2:
            insertAtEnd(&Head);
            break;
        case 3:
            printList(&Head);
            break;
        case 4:
            printf("Unesite prezime koje trazite: ");
            scanf(" %49s", searchLastname);
            Position found = findByLastName(&Head, searchLastname);
            if (found) {
                printf("Pronadena osoba: %s %s, %d\n", found->name, found->lastname, found->year);
            }
            else {
                printf("Osoba nije pronadena.\n");
            }
            break;
        case 5:
            printf("Unesite prezime osobe za brisanje: ");
            scanf(" %49s", searchLastname);
            deleteByLastName(&Head, searchLastname);
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nevažeæi odabir, pokušajte ponovno.\n");
        }

    } while (choice != 0);
}

int main() {
    menu(); //pozivamo funkciju za izbor manipuliranja sa listom
    return 0;
}