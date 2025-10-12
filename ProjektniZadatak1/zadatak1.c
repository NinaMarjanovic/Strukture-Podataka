#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#define MAX 100 //definiram maksimalan broj bodova na kolokviju

typedef struct studenti {
	char ime[10];
	char prezime[20];
	int bodovi;
}Student;

FILE* otvoriDat(const char* popis);
int prebroji(FILE* fp);
Student* alocirajMemoriju(int brojStudenata);
void ucitaj(FILE* fp, Student* studenti, int brojStudenata);
void ispis(Student* studenti, int brojStudenata);


FILE* otvoriDat(const char* popis){
	FILE* fp = fopen("popis.txt", "r"); //otvaram datoteku za èitanje(read)
	if (fp==NULL) { //provjeravam je li datoteka uspješno otvorena
		printf("Greska pri otvaranju datoteke"); //ako nije ispisuje se greška
		return NULL;  
	}
	return fp;
}
int prebroji(FILE* fp) {
	int brojac = 0;
	char buffer[50]; //pomoæna varijabla za èitanje redaka 
	while (fgets(buffer, sizeof(buffer), fp) != NULL) //pomocu fgets èitam redak po redak
		brojac++; //brojimo koliko redaka ima
	return brojac; //vraæamo ukupan broj redaka 
}
Student* alocirajMemoriju(int brojStudenata){
	Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student)); //racunamo koliko memorije trebamo zauzeti
	if (studenti == NULL) { //provjera uspiješnosti alociranja memorije(ako slucajno nema dovoljno mjesta u memoriji)
		printf("Greska"); 
		return NULL;
	}
	return studenti;
}
void ucitaj(FILE* fp, Student* studenti, int brojStudenata) {
	rewind(fp); //funckija koja postavlja pokazivaè na pocetak datoteke fp
	for (int i = 0; i < brojStudenata; i++)
		fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi); //koristimo fscanf zato sto on prima datoteku kao argument
}
void ispis(Student* studenti, int brojStudenata) {
	printf("Ime     Prezime     Bodovi     Relativni bodovi(%%)\n");
	printf("---------------------------------------------------\n");
	for (int i = 0; i < brojStudenata; i++) { //ispisujemo popis studenata i bodova
		float relativni = (studenti[i].bodovi / (float)MAX) * 100; //racunamo relativne bodove tj. postotak uspiješnosti na kolokviju
		printf("%-10s %-10s %-10d %-10.2f%%\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativni);
	}
}
int main() {

	FILE* fp=otvoriDat("popis.txt");
	int brojStudenata=prebroji(fp);
	printf("\nDatoteka ima %d redaka\n\n", brojStudenata); //ispisujemo brod redaka datoteke
	Student* studenti=alocirajMemoriju(brojStudenata); //pozivamo funkcije
	ucitaj(fp, studenti, brojStudenata);
	ispis(studenti, brojStudenata);

	free(studenti); //oslobaðamo memoriju
	fclose(fp); //zatvaramo datoteku

	return 0;
}