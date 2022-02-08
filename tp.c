#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct action{
    char jour[2];
    char heure[3];
    char nom[11];
    struct action * action_suiv;
}action_t;

typedef struct semaine{
    char annee[5];
    char num_semaine[3];
    action_t * action;
    struct semaine * semaine_suiv;
}semaine_t;

void fgets2(char * ligne, int n, FILE * file){
    fgets(ligne,n,file);
    ligne[strlen(ligne)-1] = '\0';
}

void writeYear(semaine_t * psemaine, char * ligne){
    for (int i = 0; i < 4; i++){
        psemaine->annee[i] = ligne[i];
    }
    psemaine->annee[4] = '\0';
    //printf("year : %s \n", psemaine->annee);
}

void writeWeek(semaine_t * psemaine, char * ligne){
    for (int i = 4; i < 6; i++){
        psemaine->num_semaine[i-4] = ligne[i];
    }
    psemaine->num_semaine[2] = '\0';
    //printf("num semaineaine : %s \n", psemaine->num_semaine);
}

void writeDay(action_t * paction, char * ligne){
    paction->jour[0] = ligne[6];
    paction->jour[1] = '\0';
    //printf("num jour : %s \n", paction->jour);
}

void writeHour(action_t * paction, char * ligne){
    paction->heure[0] = ligne[7];
    paction->heure[1] = ligne[8];
    paction->heure[2] = '\0';
    //printf("hour : %s \n", paction->heure);
}

void writeName(action_t * paction, char * ligne){
    for (int i = 9; i < 19; i++){
        paction->nom[i-9] = ligne[i];
    }
    paction->nom[11] = '\0';
    //printf("nom : %s \n", paction->nom);
}

int compareDates(action_t * paction, action_t * cour){
    int jour_comp = strcmp(paction->jour, cour->jour);
    int heure_comp = strcmp(paction->heure, cour->heure);
    if (jour_comp == 0){
        if (heure_comp == 0){
            return 1; //SAME DATE
        }
        else{
            if (heure_comp > 0){
                return 2; //DATE1 > DATE2
            }
            else{
                return 3; //DATE 1 < DATE2
            }
        }
    }
    else {
        if (jour_comp > 0){
            return 2; //DATE1 > DATE2
        }
        else {
            return 3; //DATE 1 < DATE2
        }
    }
}

int compareSem(semaine_t * psemaine, semaine_t * cour){
    int annee_comp = strcmp(psemaine->annee, cour->annee);
    int numsemaine_comp = strcmp(psemaine->num_semaine, cour->num_semaine);
    if (annee_comp == 0){
        if (numsemaine_comp == 0){
            return 1; //SAME DATE
        }
        else{
            if (numsemaine_comp > 0){
                return 2; //DATE1 > DATE2
            }
            else{
                return 3; //DATE 1 < DATE2
            }
        }
    }
    else {
        if (annee_comp > 0){
            return 2; //DATE1 > DATE2
        }
        else {
            return 3; //DATE 1 < DATE2
        }
    }
}

void printActionList(action_t * action_tete){
    action_t * cour = action_tete;
    while (cour != NULL){
        printf("%s\n", cour->nom);
        cour = cour->action_suiv;
    }
}

void addActionToList(action_t * action_tete, action_t * paction){
    printf("-----adding : %s ----- \n", paction->nom);
    //printActionList(action_tete);
    int comp_dates;
    int ok = 1;
    if (action_tete->action_suiv == NULL){
        action_tete->action_suiv = paction;
    }
    else {
        action_t * cour = action_tete->action_suiv;
        action_t * prec = action_tete;
        while (cour != NULL && ok){
            comp_dates = compareDates(paction, cour);
            if (comp_dates == 3){
                ok = 0;
            }
            if (ok){
                prec = cour;
                cour = cour->action_suiv;
            }
        }
        prec->action_suiv = paction;
        paction->action_suiv = cour;
    }
}

semaine_t * addSemaineToList(semaine_t * semaine_tete, semaine_t * semaine_to_add){
    printf("-----adding : %s ----- \n", semaine_to_add->num_semaine);
    int comp_sem;
    int ok = 1;
    if (semaine_tete->semaine_suiv == NULL){
        semaine_tete->semaine_suiv = semaine_to_add;
    }
    else {
        semaine_t * cour = semaine_tete->semaine_suiv;
        semaine_t * prec = semaine_tete;
        while (cour != NULL){
            prec = cour;
            cour = cour->semaine_suiv;
        }
        prec->semaine_suiv = semaine_to_add;
        semaine_to_add->semaine_suiv = cour;
    }
}

semaine_t * getSemainePtr(semaine_t * semaine_tete, semaine_t * semaine_cour){
    semaine_t * cour = semaine_tete->semaine_suiv;
    while (cour)
    {
        if (!strcmp(cour->annee, semaine_cour->annee) && !strcmp(cour->num_semaine, semaine_cour->num_semaine)){
            return cour;
        }
        cour = cour->semaine_suiv;
    }
    return NULL;
}

int main(){
    char ligne[21];
    FILE * file = fopen("data.txt", "r");
    semaine_t semaine_tete;
    semaine_tete.semaine_suiv = NULL;    
    if (file){
        while (!feof(file) && fgets(ligne,21,file) != NULL){
            action_t * action_cour = (action_t *)malloc(sizeof(action_t));
            semaine_t * semaine_tmp = (semaine_t *)malloc(sizeof(semaine_t));

            //gestion de la ligne
            writeYear(semaine_tmp, ligne);
            writeWeek(semaine_tmp, ligne);
            writeDay(action_cour, ligne);
            writeHour(action_cour, ligne);
            writeName(action_cour, ligne);
            //fin gestion ligne

            semaine_t * semaine_cour = getSemainePtr(&semaine_tete, semaine_tmp);
            printf("%p \n", semaine_cour);
            fflush(stdout);
            if (semaine_cour == NULL){
                semaine_cour = semaine_tmp;
                semaine_cour->action = action_cour;
                addSemaineToList(&semaine_tete, semaine_cour);
                printActionList(action_cour);
            }
            else {
                addActionToList(semaine_cour->action, action_cour);
                printActionList(semaine_cour->action);
            }
        }
        //printActionList(&action_tete);
    }
}