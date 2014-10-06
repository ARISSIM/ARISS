#include "CArgument.h"

vector split_arg(char* Arg) {
    vector vposition; //int
    vector_init(&vposition, sizeof (int), 10);
    
    vector varg; //int
    vector_init(&varg, sizeof (int), 10);

    int pos = 0;
    int ag;
    int i;
    //recherche des positions des espaces
    for (i = 0; i < strlen(Arg); i++) {
        if (Arg[i] == ' ') {
            vector_push_back(&vposition, &i);
        }
    }

    int add = strlen(Arg);
    vector_push_back(&vposition, &add); // ajoute espace après le dernier mot

    for (i = 0; i < (int) vposition.size; i++) {
        if (pos == 0) { // Pas besoin de prendre en compte l'espace avant la chaine de caractère à copier
            int nextPos; // Posi(int)vposition.sizetion du séparateur suivant
            vector_get(&vposition, i, &nextPos);
            const char *target = Arg; //Adresse à partir de laquelle on doit copier la chaine de caractère
            int nb = nextPos; // Nombre de caractères à copier
            char tmp[nb + 1]; // Variable temporaire pour copier la chaine de caractère
            strncpy(tmp, target, nb); //Copie de nb caractères
            tmp[sizeof (tmp) - 1] = '\0'; //Ajout du caractere terminant
            ag = atoi(tmp); //Transforme la chaine de caractère en in
            vector_push_back(&varg, &ag);      
        } else { // Il ne faut pas copier l'espace : on copie à partir de pos+1
            int nextPos; // Position de l'espace suivant
            vector_get(&vposition, i, &nextPos);
            const char *target = Arg + pos + 1; //Adresse à partir de laquelle on doit copier la chaine de caractère
            int nb = nextPos - (pos + 1); // Nombre de caractères à copier
            char tmp[nb + 1]; // Variable temporaire pour copier la chaine de caractère
            strncpy(tmp, target, nb);
            tmp[sizeof (tmp) - 1] = '\0';
            ag = atoi(tmp);
            vector_push_back(&varg, &ag);
        }
        vector_get(&vposition, i, &pos); // Position du séparateur suivant
    }
    return (varg);
}