#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int exp;
    int coef;
} Term;

typedef struct {
    Term* terms; //lista svih clanova polinova
    int n;
} Polynomial;

void addTerm(Polynomial* p, int coef, int exp) {  //dodavanje clana u polinom (i spajamo iste eksponente)
    for (int i = 0; i < p->n; i++) {  //ako postoji clan s istim ekps, promjeni koef
        if (p->terms[i].exp == exp) {
            p->terms[i].coef += coef;
            return;
        }
    }
    p->terms = (Term*)realloc(p->terms, (p->n + 1) * sizeof(Term)); //ako ne postoji prosiri niz i dodaj clan
    p->terms[p->n].coef = coef;
    p->terms[p->n].exp = exp;
    p->n++;
}

Polynomial readPolyLine(char* line) {   //citanje jednog polinoma iz retka
    Polynomial p = { NULL, 0 };
    float coef;
    int exp;
    char* token = strtok(line, " \t\n");
    while (token != NULL) {
        coef = atof(token);  //ascii to float, pretvara string u float
        token = strtok(NULL, " \t\n");  //string token, razbija string na dijelove
        if (token == NULL) break; // neparan broj vrijednosti
        exp = atoi(token); //asci to integer, pretvara string u int
        addTerm(&p, coef, exp);
        token = strtok(NULL, " \t\n");
    }
    return p;
}

Polynomial addPoly(Polynomial a, Polynomial b) {  //funkcija za zbrajanje polinoma
    Polynomial res = { NULL, 0 };
    for (int i = 0; i < a.n; i++)  //prolazimo kroz sve clanove A i B poloinoma
        addTerm(&res, a.terms[i].coef, a.terms[i].exp);
    for (int j = 0; j < b.n; j++)
        addTerm(&res, b.terms[j].coef, b.terms[j].exp);
    return res;
}


Polynomial multPoly(Polynomial a, Polynomial b) {  //funkcija za mnozenje polinoma
    Polynomial res = { NULL, 0 };
    for (int i = 0; i < a.n; i++) {  
        for (int j = 0; j < b.n; j++) {
            addTerm(&res,
                a.terms[i].coef * b.terms[j].coef,
                a.terms[i].exp + b.terms[j].exp);
        }
    }
    return res;
}

void sortPoly(Polynomial* p) {  //funkcija za sortiranje polinoma po eksponentu (od najveceg do najmanjeg)
    for (int i = 0; i < p->n - 1; i++)
        for (int j = i + 1; j < p->n; j++)
            if (p->terms[j].exp > p->terms[i].exp) {
                Term t = p->terms[i];
                p->terms[i] = p->terms[j];
                p->terms[j] = t;
            }
}

void printPoly(Polynomial p) {  //ispis polinoma
    sortPoly(&p);
    for (int i = 0; i < p.n; i++) {
        if (i > 0 && p.terms[i].coef >= 0)
            printf(" + ");
        printf("%dx^%d", p.terms[i].coef, p.terms[i].exp);
    }
    printf("\n");
}

int main() {
    FILE* f = fopen("polinomi.txt", "r");
    if (!f) {
        printf("Greška pri otvaranju datoteke!\n");
        return 1;
    }

    char line[256];

    
    fgets(line, sizeof(line), f);     //prvi red
    Polynomial p1 = readPolyLine(line);

    
    fgets(line, sizeof(line), f);   //drugi red
    Polynomial p2 = readPolyLine(line);

    fclose(f);

    printf("P1(x) = ");
    printPoly(p1);

    printf("P2(x) = ");
    printPoly(p2);

    Polynomial sum = addPoly(p1, p2);
    Polynomial prod = multPoly(p1, p2);

    printf("\nZbroj:\n");
    printPoly(sum);

    printf("Umnožak:\n");
    printPoly(prod);

    free(p1.terms); //oslobada dinamicki niz clanova polinoma
    free(p2.terms);
    free(sum.terms);  //oslobadamo memoriju za zbr
    free(prod.terms); //-||- za mnozenje

    return 0;
}
