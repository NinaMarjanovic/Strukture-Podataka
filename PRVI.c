#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME 50

typedef struct {
    char ime[MAX_NAME];
    char prezime[MAX_NAME];
    int bodovi;
    float relativni;
} Student;

int brojStud(const char* datoteka) {
    FILE* f = fopen(datoteka, "r");
    if (!f) {
        printf("Ne mogu otvoriti datoteku.\n");
        return -1;
    }

    int br = 0;
    char ime[MAX_NAME], prezime[MAX_NAME];
    int bodovi;

    while (!feof(f)) {
        if (fscanf(f, "%s %s %d", ime, prezime, &bodovi) == 3) {
            br++;
        }
    }

    fclose(f);
    return br;
}

int main() {
    const char* datoteka = "STUDENT.txt";

    int broj = brojStud(datoteka);
    if (broj <= 0) {
        printf("Datoteka je prazna.\n");
        return 1;
    }

    Student* studenti = (Student*)malloc(broj * sizeof(Student));
    if (!studenti) {
        printf("Greška pri alokaciji memorije.\n");
        return 1;
    }

    FILE* f = fopen(datoteka, "r");
    if (!f) {
        printf("Ne mogu otvoriti datoteku.\n");
        free(studenti);
        return 1;
    }

    for (int i = 0; i < broj; i++) {
        fscanf(f, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
    }

    fclose(f);

    int maxBodovi;
    printf("Unesite maksimalan broj bodova: ");
    scanf("%d", &maxBodovi);

    printf("\nIme  \tPrezime   \tBodovi   \tRelativni bodovi (%%)\n");
    for (int i = 0; i < broj; i++) {
        studenti[i].relativni = (float)studenti[i].bodovi / maxBodovi * 100;
        printf("%-10s %-10s %-10d %-10.2f%%\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, studenti[i].relativni);
    }

    free(studenti);
    return 0;
}
