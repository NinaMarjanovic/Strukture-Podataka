#include<stdio.h>

int main() {
	int brojac = 0;

	FILE* file = fopen("popis.txt", "r");

	if (file == NULL);
	return 0;

	while ((fgetc(file)) != EOF){
			brojac++;
	}

	fclose(file);
	printf("Broj redaka u file-u: %d", brojac);
	typedef struct student{
		char ime[20];
		char prezime[20];
		int bodovi;

	}Stud;

}