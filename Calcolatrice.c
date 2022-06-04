#include <stdio.h>
void main()
{
    float op1, op2, tot;
char operazione;
unsigned int errore = 0;

    printf ("Operazione (+,-,*,/): ");
    scanf ("%c", &operazione);
    printf ("Primo operando: ");
    scanf ("%f", &op1);
    printf ("Secondo operando: ");
    scanf ("%f", &op2);
switch (operazione)
    {
    case '+': tot = op1 + op2;
    break;
    case '-': tot = op1 - op2;
    break;
    case '*': tot = op1 * op2;
    break;
    case '/': if (op2 != 0)
    {
    tot = op1 / op2;
    }
    else
    {
    errore = 1;
    }
    break;
    default: printf ("Operazione?\n");
    errore = 1;
    }
if (errore == 0)
{
    printf ("Risultato: %f\n", tot);
}
else
{
    printf ("Errore!\n");
}
}