#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct postfix* Position;
typedef struct postfix {
    double number;
    Position next;
} postfix;

Position CreateElement(double number); //stvara novi cvor
int Push(Position head, double number); //stavlja broj na stog
double Pop(Position head); //uzima sa stoga
int CalculateFromFile(Position head, char* fileName); //cita dat i racuna
int FreeList(Position head); //brise stog iz memorije

int main() {
    postfix head = { .number = 0, .next = NULL };

    char fileName[] = "postfix.txt";

    if (CalculateFromFile(&head, fileName) == 0) //pozivamo fju da procita i izr izraz
        printf("\nRezultat izraza je: %.2lf\n", head.next->number);
    else
        printf("\nDoslo je do pogreske pri racunanju!\n");

    FreeList(&head); //brisu se svi elementi sa stoga i memorija se oslobada
    return 0;
}

Position CreateElement(double number) {
    Position newEl = (Position)malloc(sizeof(postfix)); //alokacija memorije za novi cvor
    if (!newEl) {  //ako nema dovoljno mem
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    newEl->number = number;  //spremi broj u cvor
    newEl->next = NULL;  //pomakni pokazivac na novi element
    return newEl;
}

int Push(Position head, double number) {  //napravi novi cvor s tim brojem
    Position newEl = CreateElement(number);
    if (!newEl)
        return -1;

    newEl->next = head->next; //novi cvor ide na vrh stoga(i pocetak liste)
    head->next = newEl;
    return 0;
}

double Pop(Position head) {
    if (head->next == NULL) {  //nema elementa, stog je prazan
        printf("Stog je prazan!\n");
        return 0;
    }

    Position temp = head->next;   //uzima se prvi el sa stoga
    double number = temp->number; //pamti se njegov broj
    head->next = temp->next;      //prebaci se pokazivac 
    free(temp);                   //i oslobodi mem

    return number;   //vrati se broj koji je bio na vrhu stoga
}

int CalculateFromFile(Position head, char* fileName) {
    FILE* fp = fopen(fileName, "r");    //otvara dat i cita
    if (!fp) {
        printf("Ne mogu otvoriti datoteku!\n");
        return -1;
    }

    char buffer[1024];
    while (fscanf(fp, " %s", buffer) == 1) {

        if (isdigit(buffer[0]) || (buffer[0] == '-' && isdigit(buffer[1]))) {
            double number = atof(buffer); //atof-fja koja pretvara 
                                          //tekst u dec broj
            Push(head, number);    //stavi ga na stog
        }
        else if (strlen(buffer) == 1 && strchr("+-*/", buffer[0])) { //strlen-duljina stringa, strchr-pojavljuje li se odredeni znak u nekom stringu
            double operand2 = Pop(head);
            double operand1 = Pop(head);
            double result = 0;

            switch (buffer[0]) {
            case '+': result = operand1 + operand2; break;
            case '-': result = operand1 - operand2; break;
            case '*': result = operand1 * operand2; break;
            case '/':
                if (operand2 == 0) {
                    printf("Dijeljenje s nulom!\n");
                    fclose(fp);
                    return -3;
                }
                result = operand1 / operand2;
                break;
            default:
                printf("Nepoznat operator!\n");
                fclose(fp);
                return -4;
            }
            Push(head, result); //rezultat vraca nazad na stog
        }
        else {
            printf("Neispravan unos: %s\n", buffer);
            fclose(fp);
            return -5;
        }
    }

    fclose(fp);

    if (head->next && head->next->next == NULL) //provejera da li je na stogu osto tocno jedan broj(rez)
        return 0;
    else {  //ako ima vise ili manje
        printf("Izraz nije ispravan, previse/ premalo operanada.\n");
        return -6;
    }
}

int FreeList(Position head) {  //oslobada se memorija
    Position temp = NULL;
    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    return 0;
}