#include "CArgument.h"

vector split_arg(char* Arg) {
    vector vposition; //int
    vector_init(&vposition, sizeof (int), 10);

    vector v_str; //string
    vector_init(&v_str, sizeof (char*), 10);

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

    int add = strlen(Arg) + 1;
    vector_push_back(&vposition, &add); // ajoute espace après le dernier mot

    for (i = 0; i < vector_size(&vposition); i++) {
        if (pos == 0) { // Pas besoin de prendre en compte l'espace avant la chaine de caractère à copier
            int nextPos; // Position du séparateur suivant
            vector_get(&vposition, i, &nextPos);
            char *target = Arg + pos; //Adresse à partir de laquelle on doit copier la chaine de caractère
            char* tmp; // Variable temporaire pour copier la chaine de caractère
            int nb = nextPos - pos; // Nombre de caractères à copier
            memcpy(tmp, target, nb * sizeof (char)); // Copie dans tmp de nb char à partir de l'adresse target
            vector_push_back(&v_str, tmp); // Insertion de la chaine de caractères tmp dans le vecteur
            free(target);
        } else { // Il ne faut pas copier l'espace : on copie à partir de pos+1
            int nextPos; // Position de l'espace suivant
            vector_get(&vposition, i, &nextPos);
            char *target = Arg + pos + 1; //Adresse à partir de laquelle on doit copier la chaine de caractère
            char* tmp; // Variable temporaire pour copier la chaine de caractère
            int nb = nextPos - (pos + 1); // Nombre de caractères à copier
            memcpy(tmp, target, nb * sizeof (char)); // Copie dans tmp de nb char à partir de l'adresse target
            vector_push_back(&v_str, tmp); // Insertion de la chaine de caractères tmp dans le vecteur 
        }
        vector_get(&vposition, i, &pos); // Position du séparateur suivant
    }

    for (i = 0; i < vector_size(&v_str); i++) {
        char *tmp;
        vector_get(&v_str, i, tmp);
        ag = atoi(tmp);
        vector_push_back(&varg, &ag);
    }

    return (varg);
}