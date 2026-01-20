#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person* Position;  //position -pokazivac na strukturu Persone kako nebi
//stalno morali pisati struct Person*, pisemo samo Position

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
    scanf("%d", &newPerson->year); //u strukturi na koju pokazuje newPersone uzmi polje godina i tamo spremi upisanu godinu od korisnika

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

    if (q == NULL) { //ako je lista prazna
        printf("Lista je prazna.\n");
        return;
    }

    printf("\nIspis liste \n");
    while (q != NULL) {
        printf("Ime: %s, Prezime: %s, Godina rodenja: %d\n", q->name, q->lastname, q->year); //ispis svakog clana

        q = q->next; //sljedeca osoba
    }
    printf("\n");
}

Position findByLastName(Position head, const char* targetLastname) { //funkcija za pronalazanje osobe po prezimenu
    Position temp = head->next;  // prva stvarna osoba (preskacemo head koji je cvor na pocetku liste)

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
        q->next = temp->next; //brisemo osobu tako da preskocimo temp

        free(temp); //oslabadamo memoriju
        temp = NULL; //postavljamo temp na NULL

        printf("Osoba je uspjesno obrisana.\n");
        return 0;
    }
    else {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", targetLastname);
        return -1;
    }
}
int addBefore(Position head, const char* targetLastname) { //dodavanje osobe iza odredene osobe
    Position target = findByLastName(head, targetLastname); //pronalazimo osobu iza koje ocemo dodat novu
    if (!target) { //ako ne postoji to prezime
        printf("Osoba nije pronadena.\n");
        return -1;
    }

    Position newPerson = createPerson(); //radimo novu osobu
    if (!newPerson) return -1;

    newPerson->next = target->next; //nova osoba pokazuje na onu nakon ciljane osobe
    target->next = newPerson; //ciljana osoba sada pokazuje na novu osobu

    printf("Osoba dodana iza %s.\n", targetLastname);
    return 0;
}
int addAfter(Position head, const char* targetLastname) { //dodavanje osobe ispred odredene osobe
    Position prev = head;  //prethodna osoba
    Position curr = head->next; //trenutna osoba

    while (curr != NULL && strcmp(curr->lastname, targetLastname) != 0) { //prolazimo kroz listu dok ne pronademo ciljanu osobu
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) { //ako je nema ide poruka da ne postoji ta osoba(prezime)
        printf("Osoba nije pronadena.\n");
        return -1;
    }

    Position newPerson = createPerson(); //ako ima onda pravimo novu osobu koja ce ici ispred te ciljane osobe koju smo nasli sa while petljom
    if (!newPerson) return -1;

    newPerson->next = curr; //nova osoba pokazuje na ciljanu
    prev->next = newPerson; //prethodna osoba pokazuje na novu

    printf("Osoba dodana ispred %s.\n", targetLastname);
    return 0;
}
void sortList(Position head) { //sortiranje liste po prezimenu
    if (head->next == NULL || head->next->next == NULL) { //ako ima manje od dvije osobe
        printf("Lista je prekratka za sortiranje.\n");
        return;
    }

    Position end = NULL; //granica sortiranja tj dio koji je vec sortiran
    Position prev, curr, next;

    while (head->next != end) {
        prev = head;          //bubble sortiranje
        curr = head->next;
        next = curr->next;

        while (next != end) {
            if (strcmp(curr->lastname, next->lastname) > 0) { //ako su prezimena u pogresnom redosljedu
                curr->next = next->next;  //zamjena mjesta curr i next
                next->next = curr;
                prev->next = next;

                Position temp = curr; //pomicanje pokazivaca
                curr = next;
                next = temp;
            }
            prev = curr;
            curr = curr->next;
            next = curr->next;
        }
        end = curr;
    }

    printf("Lista je sortirana po prezimenima.\n");
}
int writeToFile(Position head, const char* filename) {  //funkcija za upisivanje(spremanje) liste u datoteku
    FILE* fp = fopen(filename, "w");  //otvaramo datoteku za pisanje
    if (!fp) {
        printf("Greska pri otvaranju datoteke.\n"); //greska ako se ne moze otvoriti ili upisat
        return -1;
    }

    Position q = head->next;  //postavljamo pokazivac q na prvu osobu
    while (q != NULL) {  //vrti se sve dok ne dodemo do kraja liste
        fprintf(fp, "%s %s %d\n", q->name, q->lastname, q->year);  //upisujemo podatke u datoteku
        q = q->next; //iduca osoba, iduca, ...
    }

    fclose(fp); //zatvaramo datoteku
    printf("Lista je uspjesno upisana u datoteku '%s'.\n", filename); //poruka da je sve proslo u redu
    return 0;
}
int readFromFile(Position head, const char* filename) { //funckija za citanje iz datoteke i spremanje u povezanu listu
    FILE* fp = fopen(filename, "r");  //otvaramo datoteku samo za citanje (r-read)
    if (!fp) { //ako se ne moze otvoriti
        printf("Greska pri otvaranju datoteke.\n"); //ispisuje se poruka o grešci
        return -1;
    }

    while (!feof(fp)) { //while petlja se vrti dok nismo dosli do kraja datoteke
        Position newPerson = (Position)malloc(sizeof(Person)); //alokacija memorije za novu osobu
        if (fscanf(fp, " %49s %49s %d", newPerson->name, newPerson->lastname, &newPerson->year) == 3) {
            newPerson->next = head->next;  //novi el pokazuje na trenutnog prvog clana liste
            head->next = newPerson; //head pokazuje na novi el (ubacujemo ga na pocetak liste)
        }
        else { //ako se ne mogu ucitati sva tri podatka (ime, prezime, godiste)
            free(newPerson); //oslobodi predhodno alociranu memoriju zato sto se ne koristi
            break; //prekidamo citanje iz datoteke
        }
    }

    fclose(fp); //zatvaramo datoteku
    printf("Lista je uspjesno ucitana iz datoteke '%s'.\n", filename); //poruka da je sve ok
    return 0;
}

void menu() { //funkcija izbornik kojom upravljamo listom
    Person Head = { .name = "", .lastname = "", .year = 0, .next = NULL }; //head nam je prazna osoba koja sluzi kao pocetak liste

    int choice;
    char searchLastname[50];
    char filename[50];

    do {
        printf("\nIzaberi naredbu koju zelis izvrsiti: \n");
        printf("1. Dodaj osobu na pocetak\n");
        printf("2. Dodaj osobu na kraj\n");
        printf("3. Ispisi listu\n");
        printf("4. Pronadi osobu po prezimenu\n");
        printf("5. Obrisi osobu po prezimenu\n");
        printf("6. Dodaj osobu ispred odredene osobe\n");
        printf("7. Dodaj osobu iza odredene osobe\n");
        printf("8. Sortiraj listu po prezimenima\n");
        printf("9. Upisi listu u datoteku\n");
        printf("10. Ucitaj listu iz datoteke\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            Beginning(&Head);
            break;
        case 2:
            EndOfList(&Head);
            break;
        case 3:
            printList(&Head);
            break;
        case 4:
            printf("Unesite prezime koje trazite: ");
            scanf(" %49s", searchLastname);
            Position found = findByLastName(&Head, searchLastname);
            if (found)
                printf("Pronadena osoba: %s %s, %d\n", found->name, found->lastname, found->year);
            else
                printf("Osoba nije pronadena.\n");
            break;
        case 5:
            printf("Unesite prezime osobe za brisanje: ");
            scanf(" %49s", searchLastname);
            deleteByLastName(&Head, searchLastname);
            break;
        case 6:
            printf("Iza koje osobe zelis dodati novi element (prezime): ");
            scanf(" %49s", searchLastname);
            addAfter(&Head, searchLastname);
            break;
        case 7:
            printf("Ispred koje osobe zelis dodati novi element (prezime): ");
            scanf(" %49s", searchLastname);
            addBefore(&Head, searchLastname);
            break;
        case 8:
            sortList(&Head);
            break;
        case 9:
            printf("Unesite ime datoteke za upis: ");
            scanf(" %99s", filename);
            writeToFile(&Head, filename);
            break;
        case 10:
            printf("Unesite ime datoteke za citanje: ");
            scanf(" %99s", filename);
            readFromFile(&Head, filename);
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Pogresan odabir, pokusajte ponovno.\n");
        }

    } while (choice != 0);
}


int main() {
    menu(); //pozivamo funkciju za izbor manipuliranja sa listom
    return 0;
}