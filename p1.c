#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define X 20    //definoval som si velkost pomocnych poli

//----------------------------------------------------------------------------------------------------------------------------//
//alokuje dyn polia ak bol .txt otvoreny
void function_n( FILE **text, int kontrola_v,int alokovane ,int *pocet_zaznamov, long long **ID_cislo_osoby, char ***mer_modul,
char ***typ_veliciny, float **hodnota, char ***cas_merania, long long **datum){

    char znak;
    int temp=1;
    char string_modul[X], string_typ[X], string_cas[X];

    if(*text == NULL){
        printf("Neotvoreny subor\n");
    }

    //uvolnenie ak boli alokovane a nasledne nastavene na NULL
    if(alokovane == 1) {
        for (int i = 0; i < *pocet_zaznamov; i++) {
            free((*mer_modul)[i]);
            free((*typ_veliciny)[i]);
            free((*cas_merania)[i]);
        }
        free(*mer_modul);
        free(*typ_veliciny);
        free(*hodnota);
        free(*cas_merania);
        free(*datum);

        *ID_cislo_osoby = NULL;
        *mer_modul = NULL;
        *typ_veliciny = NULL;
        *hodnota = NULL;
        *cas_merania = NULL;
        *datum = NULL;
    }

    if(kontrola_v == 1){

        //zisti pocet zaznamov
        rewind(*text);
        while((znak = fgetc(*text)) != EOF){
            if(znak == '\n'){
                temp+=1;
            }
        }

        *pocet_zaznamov = temp/7;

        //alokuje polia * pocet_zaznamov cize temp(57 prvkov)
        *ID_cislo_osoby = (long long*)malloc( (*pocet_zaznamov) * sizeof(long long ));
        *mer_modul = (char **)malloc((*pocet_zaznamov) * sizeof(char*));
        *typ_veliciny = (char **)malloc( (*pocet_zaznamov) * sizeof(char*));
        *hodnota = (float *)malloc( (*pocet_zaznamov) * sizeof(float));
        *cas_merania = (char **)malloc( (*pocet_zaznamov) * sizeof(char*));
        *datum = (long long *)malloc((*pocet_zaznamov) * sizeof(long long));

        //alokuje miesto pre stringy
        for(int i=0; i < (*pocet_zaznamov);i++) {
            (*mer_modul)[i] = (char *)malloc(sizeof(char) * X);
            (*typ_veliciny)[i] = (char *)malloc(sizeof(char) * X);
            (*cas_merania)[i] = (char *)malloc(sizeof(char) * X);
        }

        rewind(*text);

        for(int i=0; i < (*pocet_zaznamov);i++){

            fscanf(*text, "%lld\n", &(*ID_cislo_osoby)[i]); //1. riadok

            fgets(string_modul,X ,*text);       //2. riadok
            string_modul[strlen(string_modul)-1] = '\0';

            fgets(string_typ,X ,*text);         //3.riadok
            string_typ[strlen(string_typ)-1] = '\0';

            fscanf(*text, "%f\n", &(*hodnota)[i]);      //4. riadok

            fgets(string_cas,X ,*text);          //5. riadok
            string_cas[strlen(string_cas)-1] = '\0';

            fscanf(*text, "%lld\n\n", &(*datum)[i]);    //6. riadok, prazdny to skipne -> navyse '\n'

            //z pomocnych poli to kopirujem na adresu i pre char polia
            strcpy((*mer_modul)[i], string_modul);
            strcpy((*typ_veliciny)[i], string_typ);
            strcpy((*cas_merania)[i], string_cas);

        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------//
//nacita mer. modul a typ, zoradi podla datumu a casu
void function_o(FILE **text,int pocet_zaznamov, char ***mer_modul,
                char ***typ_veliciny, float **hodnota, char ***cas_merania, long long **datum){

    char mer_modul_buf[5],typ_veliciny_buf[5];
    long long x;
    float pomocna_float;
    int splnene=0, c;

    //"Neotvoreny subor" vyprintuje z 'n' lebo ho volam pred touto funkciou

    scanf("%s %s",mer_modul_buf, typ_veliciny_buf);
    rewind(*text);          //vrati text na zaciatok

    //(pocet zaznamov) lebo to je max co moze nastat, kebyze dame menej tak to nemusi stacit pri inych typoch .txt
    long long *pomocna_datum = (long long*)malloc( pocet_zaznamov * sizeof(long long )) ;
    int *pomocna_cas = (int*)malloc( pocet_zaznamov * sizeof(int ));
    float *pomocna_hodnota = (float *)malloc( pocet_zaznamov * sizeof(float )) ;


    for (int i = 0; i < pocet_zaznamov; i++) {
        // negoval som to lebo strcmp vrati opacnu hodnotu
        if(!(strcmp((*mer_modul)[i],mer_modul_buf)) && !(strcmp((*typ_veliciny)[i],typ_veliciny_buf))){

            pomocna_datum[splnene] = (*datum)[i];
            pomocna_cas[splnene] = atoi((*cas_merania)[i]);
            pomocna_hodnota[splnene] = (*hodnota)[i];

            splnene++;      //pohyb v pomocnych poliach
        }
    }

    //upravenym bubble sortom usporiadam prvky, ktore vyhovuju podmienke a su v novom poli
    for (int j = 0; j < splnene ; j++) {
        for (int k = j+1; k < splnene; k++) {
            if (pomocna_datum[j] > pomocna_datum[k]) {

                x = pomocna_datum[k];
                pomocna_datum[k] = pomocna_datum[j];
                pomocna_datum[j] = x;

                pomocna_float = pomocna_hodnota[k];
                pomocna_hodnota[k] = pomocna_hodnota[j];
                pomocna_hodnota[j] = pomocna_float;

                c = pomocna_cas[k];
                pomocna_cas[k] = pomocna_cas[j];
                pomocna_cas[j] = c;
            }
            else if (pomocna_datum[j] == pomocna_datum[k] ){  //ak sa datumy rovnaju tak prehodime cas a hodnotu ak nutne

                if (pomocna_cas[j] > pomocna_cas[k]){
                    pomocna_float = pomocna_hodnota[k];
                    pomocna_hodnota[k] = pomocna_hodnota[j];
                    pomocna_hodnota[j] = pomocna_float;

                    c = pomocna_cas[k];
                    pomocna_cas[k] = pomocna_cas[j];
                    pomocna_cas[j] = c;
                }
            }
        }
    }

    for (int i = 0; i < splnene; i++) {
        printf("%s\t%s\t%lld\t",mer_modul_buf, typ_veliciny_buf, pomocna_datum[i]);

        //kedze je to vo formate cisiel tak 0905 prvu 0 nevypisuje tak ju tam treba pridat
        if(pomocna_cas[i] < 1000){
            printf("0");
            if(pomocna_cas[i] < 100){
                printf("0");
                if(pomocna_cas[i] < 10){
                    printf("0");
                }
            }
        }

        printf("%d\t%f\n", pomocna_cas[i], pomocna_hodnota[i]);
    }

    //uvolnim pomocne polia
    free(pomocna_datum);
    free(pomocna_cas);
    free(pomocna_hodnota);

    pomocna_datum = NULL;
    pomocna_cas = NULL;
    pomocna_hodnota = NULL;

}

//----------------------------------------------------------------------------------------------------------------------------//
//otvori subor, kontrola ci su dyn. polia, vypise zaznamy
void function_v(FILE **text,int kontrola_n,int pocet_zaznamov, long long **ID_cislo_osoby, char ***mer_modul,
                char ***typ_veliciny, float **hodnota, char ***cas_merania, long long **datum){

    if(text == NULL){
        *text = fopen("C:\\Users\\riso\\CLionProjects\\school\\dataloger.txt","r");

        if(text == NULL){
            printf("Neotvoreny subor\n");
        }
    }
    else if(text){

        *text = fopen("C:\\Users\\riso\\CLionProjects\\school\\dataloger.txt","r");

        //vypis z dyn. poli
        if(kontrola_n){

            for (int i = 0; i < pocet_zaznamov; i++) {

                printf("ID cislo mer. osoby: %lld\n", (*ID_cislo_osoby)[i]);
                printf("Mer. modul: %s\n",(*mer_modul)[i]);
                printf("Typ mer. veliciny: %s\n",(*typ_veliciny)[i]);
                printf("Hodnota: %f\n",(*hodnota)[i]);
                printf("Cas merania: %s\n",(*cas_merania)[i]);
                printf("Datum: %lld\n",(*datum)[i]);
                printf("\n");
            }
        }

        //vypis ak dyn. polia neboli alokovane
        if(!kontrola_n){

            int znak;
            while((znak= getc(*text)) != EOF){

                printf("ID cislo mer. osoby: ");
                //lebo to nepisalo prvy znak kvoli getc()
                if(znak != '\n'){
                    printf("%c",znak);
                }
                while((znak = getc(*text)) != '\n'){
                    printf("%c",znak);
                }
                printf("\n");

                printf("Mer. modul: ");
                while((znak = getc(*text)) != '\n'){
                    printf("%c",znak);
                }
                printf("\n");

                printf("Typ mer. veliciny: ");
                while((znak = getc(*text)) != '\n'){
                    printf("%c",znak);
                }
                printf("\n");

                printf("Hodnota: ");
                while((znak = getc(*text)) != '\n'){
                    printf("%c",znak);
                }
                printf("\n");

                printf("Cas merania: ");
                while((znak = getc(*text)) != '\n'){
                    printf("%c",znak);
                }
                printf("\n");

                printf("Datum: ");
                while((znak = getc(*text)) != '\n'){
                    printf("%c",znak);
                }
                printf("\n");

                printf("\n");

            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------//
//skontroluje zaznamy
void function_c(FILE **text){

    char mer_modul[X], typ_veliciny[X], ID_cislo_osoby[X],hodnota[X],cas_merania[X], datum[X],kos,znak;
    int temp = 0;

    rewind(*text);

    if(*text == NULL){
        printf("Neotvoreny subor\n");
    }

    else {
        while ((znak = fgetc(*text)) != EOF) {
            if (znak == '\n') {
                temp += 1;
            }
        }

        rewind(*text);

        for (int i = 0; i < temp; i++) {

            //hodil som vsetko do %s kvoli vypisovaniu nekorektnych zaznamov
            fscanf(*text, "%s", ID_cislo_osoby);
            fscanf(*text, "%s", mer_modul);
            fscanf(*text, "%s", typ_veliciny);
            fscanf(*text, "%s", hodnota);
            fscanf(*text, "%s", cas_merania);
            fscanf(*text, "%s", datum);
            fscanf(*text, "%c", &kos);

            int dlzka_modulu = strlen(mer_modul);
            int dlzka_veliciny = strlen(typ_veliciny);

            //jednotlive podmienky pre kazdy zaznam
            if (atoll(ID_cislo_osoby) < 1000000000 || atoll(ID_cislo_osoby) > 9999999999 ||
                atoll(ID_cislo_osoby) % 11 != 0) {
                printf("Nekorektne zadany vstup: %s\n", ID_cislo_osoby);
            }

            if (dlzka_modulu != 3 || islower(mer_modul[0]) || 57 < (int) mer_modul[1] ||
                (int) mer_modul[1] < 48 || 57 < (int) mer_modul[2] || (int) mer_modul[2] < 48) {
                printf("Nekorektne zadany vstup: %s\n", mer_modul);
            }

            if (dlzka_veliciny != 2 || (typ_veliciny[0] != 'R' && typ_veliciny[0] != 'U' && typ_veliciny[0] != 'A')
                || (typ_veliciny[1] != '1' && typ_veliciny[1] != '2' && typ_veliciny[1] != '3')) {
                printf("Nekorektne zadany vstup: %s\n", typ_veliciny);
            }

            //kontrola ci je realne cislo
            int kontrola = 0;

            for (int j = 0; j < strlen(hodnota); j++) {
                if(hodnota[j] == '.'){
                    continue;
                }
                else if(isdigit(hodnota[j])){
                    continue;
                }
                else{
                    kontrola = 1;
                }
            }

            if (kontrola == 1){
                printf("Nekorektne zadany vstup: %s\n",hodnota);
            }

            if (atoi(cas_merania) > 2359 || atoi(cas_merania) < 0 || strlen(cas_merania) != 4) {
                printf("Nekorektne zadany vstup: %s\n", cas_merania);
            }

            if (atoi(datum) < 10000000 || atoi(datum) > 99999999) {
                printf("Nekorektne zadany vstup: %s\n", datum);
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------//
//zoradi datum + cas a vypise k nim ich hodnotu
void function_s(FILE **text, int kontrola_n,int *pocet_zaznamov,char ***mer_modul,char ***typ_veliciny,
                float **hodnota, char ***cas_merania, long long **datum){

    char mer_modul_buf[5],typ_veliciny_buf[5],datum_temp[20];
    long long x;
    float pomocna_float;

    if(kontrola_n == 0){
        printf("Polia nie su vytvorene\n");
    }

    else {
        int j=0;
        scanf("%s %s", mer_modul_buf, typ_veliciny_buf);

        //alokovanie pomocnych poli
        char **pomocne_stringov = (char**)malloc(*pocet_zaznamov * sizeof(char*));
        for (int i = 0; i < *pocet_zaznamov; i++) {
            pomocne_stringov[i] = (char*)malloc(X * sizeof(char));
        }

        float *pomocne_hodnot = (float*) malloc((*pocet_zaznamov) * sizeof(float ));

        //prebehne vsetky zaznamy a hlada zhodu
        for (int i = 0; i < *pocet_zaznamov; i++) {

            if(!(strcmp((*mer_modul)[i],mer_modul_buf)) && !(strcmp((*typ_veliciny)[i],typ_veliciny_buf))){

                itoa((*datum)[i],datum_temp,10);        //datum premenime na str a nahra do datum_temp
                strcat(datum_temp,(*cas_merania)[i]);        //spojenie 2 stringov datumu+cas
                pomocne_hodnot[j] = (*hodnota)[i];                      //nacitanie do novych poli ktore mozme zoradit
                strcpy(pomocne_stringov[j],datum_temp);     //nakopiruje do pomocneho pola
                j++;
            }
        }

        long long *datum_cas = (long long *)malloc(j * sizeof(long long));
        
        //nacitanie do alokovaneho pola cisla
        for (int i = 0; i < j; ++i) {
            datum_cas[i] = atoll(pomocne_stringov[i]);
        }

        //bubble sortom si ulozime casy a k nim hodnoty
        for (int i = 0; i < j; i++) {
            for (int y = i+1; y < j; y++) {
                if (datum_cas[i] > datum_cas[y]) {

                    x = datum_cas[y];
                    datum_cas[y] = datum_cas[i];
                    datum_cas[i] = x;

                    pomocna_float = pomocne_hodnot[y];
                    pomocne_hodnot[y] = pomocne_hodnot[i];
                    pomocne_hodnot[i] = pomocna_float;

                }
            }
        }

        FILE *p_novy = fopen("C:\\Users\\riso\\CLionProjects\\school\\vystup_S.txt","w");

        //zapisanie hodnot do vystup_S.txt
        for (int i = 0; i < j; ++i) {
            fprintf(p_novy, "%lld\t%.7f\n",datum_cas[i],pomocne_hodnot[i]);
        }

        if(j > 0){
            printf("Pre dany vstup je vytvoreny txt subor\n");
        }

        else{
            printf("Pre dany vstup neexistuju zaznamy\n");
        }


        for (int i = 0; i < *pocet_zaznamov; i++) {
            free(pomocne_stringov[i]);
        }

        //dealokovali sme polia a nastavili ich hodnotu na NULL
        free(datum_cas);
        free(pomocne_hodnot);
        free(pomocne_stringov);

        datum_cas = NULL;
        pomocne_hodnot = NULL;
        pomocne_stringov = NULL;

        //zavrel som vystup_S, v 'k' staci zavriet iba vstupny subor
        fclose(p_novy);
    }
}

//----------------------------------------------------------------------------------------------------------------------------//
//vypisuje kolko hodnot je z daneho intervalu
void function_h(FILE **text, int kontrola_n, int pocet_zaznamov, char ***typ_veliciny, float **hodnota){

    if(kontrola_n == 0){
        printf("Polia nie su vytvorene\n");
    }
    else if(kontrola_n){
        int a=0, b=0, c=0, d=0, e=0, f=0, g=0;

        char typ_veliciny_buf[5];

        scanf("%s", typ_veliciny_buf);

        //zistenie poctu z intervalu
        for (int i = 0; i < pocet_zaznamov; i++) {

            if(!(strcmp((*typ_veliciny)[i],typ_veliciny_buf))){

                if( 0 < (*hodnota)[i] && (*hodnota)[i] <= 5.0 ){
                    a++;
                }

                if( 5.0 < (*hodnota)[i] && (*hodnota)[i] <= 10.0 ) {
                    b++;
                }

                if( 15.0 < (*hodnota)[i] && (*hodnota)[i] <= 20.0 ){
                    c++;
                }

                if( 40.0 < (*hodnota)[i] && (*hodnota)[i] <= 45.0 ){
                    d++;
                }

                if( 45.0 < (*hodnota)[i] && (*hodnota)[i] <= 50.0 ){
                    e++;
                }

                if( 50.0 < (*hodnota)[i] && (*hodnota)[i] <= 55.0 ){
                    f++;
                }

                if( 150.0 < (*hodnota)[i] && (*hodnota)[i] <= 155.0 ){
                    g++;
                }
            }
        }

        //vypisanie
        printf("      %s\t     pocetnost\n",typ_veliciny_buf);
        printf("(  0.0 -   5.0>\t\t%d\n",a);
        printf("(  5.0 -  10.0>\t\t%d\n",b);
        printf("( 15.0 -  20.0>\t\t%d\n",c);
        printf("( 40.0 -  45.0>\t\t%d\n",d);
        printf("( 45.0 -  50.0>\t\t%d\n",e);
        printf("( 50.0 -  55.0>\t\t%d\n",f);
        printf("(150.0 - 155.0>\t\t%d\n",g);

    }
}
//----------------------------------------------------------------------------------------------------------------------------//
//vypisuje hodiny a k nim ich casy zoradene
void function_r(int kontrola_n, char ***cas_merania,int pocet_zaznamov){


    if(kontrola_n == 0){
        scanf("Polia nie su vytvorene\n");
    }
    else if(kontrola_n){

        int x = 0;

        //pomocne
        int *temp = malloc((pocet_zaznamov) * sizeof(int *));

        //premenenie na cisla kvoli porovnavaniu
        for (int i = 0; i < pocet_zaznamov; ++i) {
            temp[i] = atoi((*cas_merania)[i]);
        }

        //bubble sort
        for (int i = 0; i < pocet_zaznamov; i++) {
            for (int y = i+1; y < pocet_zaznamov; y++) {
                if (temp[i] > temp[y]) {

                    x = temp[y];
                    temp[y] = temp[i];
                    temp[i] = x;
                }
            }
        }

        //vymazanie dulikatov
        for (int i = 0; i < pocet_zaznamov; i++) {
            for (int j = i+1; j < pocet_zaznamov; j++) {
                if(temp[i] == temp[j]){
                    for (int k = j; k < pocet_zaznamov ; k++) {
                        temp[k] = temp[k + 1];
                    }
                    j--;
                    pocet_zaznamov--;
                }
            }
        }

        //alokovanie novych poli, bez duplikatov
        int **casy = (int**) malloc((pocet_zaznamov) * sizeof(int *));

        for (int i = 0; i < pocet_zaznamov; i++) {
            casy[i] = (int *)malloc(sizeof(int) * (pocet_zaznamov));
        }

        int predosly_cas=-1;    //-1 kontrola kebyze sa nemeni hodnota

        //printovanie zaznamov s tym ze zisti pocet cifier a podla toho bude printovat rozne,
        // lebo mame int a nie stringy tak tam chybaju 0, dalej zistuje prve cifry a posledne vypise,
        // ak prve cifry uz boli vypisane vypisuje iba posledne dve

        for (int i = 0; i < pocet_zaznamov; i++) {

            int cifry = temp[i];
            int cifry2 = temp[i];
            int counter = 0;

            while (cifry2 != 0){
                cifry2 /= 10;
                counter++;
            }

            if(counter == 3) {

                while (cifry >= 10) {
                    cifry /= 10;
                }

                int temp_cifry = temp[i] % 100;

                if (cifry == predosly_cas){
                    printf(",%02d",temp_cifry);
                }
                else{
                    predosly_cas = cifry;
                    printf("\n0%d:%02d",cifry, temp_cifry);
                }
            }
            else if(counter == 4){

                while (cifry >= 100) {
                    cifry /= 10;
                }

                int temp_cifry = temp[i] % 100;

                if (cifry == predosly_cas){
                    printf(",%02d", temp_cifry);
                }
                else{
                    predosly_cas = cifry;
                    printf("\n%d:%02d",cifry, temp_cifry);
                }
            }

            else if(counter == 2){
                cifry = 0;

                int temp_cifry = temp[i] % 100;

                if (cifry == predosly_cas){
                    printf(",%02d", temp_cifry);
                }
                else{
                    predosly_cas = cifry;
                    printf("\n00:%02d", temp_cifry);
                }
            }
            else if(counter == 1){
                cifry = 0;

                int temp_cifry = temp[i] % 10;

                if (cifry == predosly_cas){
                    printf(",0%d", temp_cifry);
                }
                else{
                    predosly_cas = cifry;
                    printf("\n00:0%d", temp_cifry);
                }
            }
            else if(counter == 0){
                printf("00:00");
            }

        }

        //uvolnenie pomocnych poli
        for (int i = 0; i < pocet_zaznamov; ++i) {
            free(casy[i]);
        }
        free(temp);
        free(casy);

    }
}
//----------------------------------------------------------------------------------------------------------------------------//
//vymaze vsetky prvky kt. sa zhoduju so zadanym ID
void function_z(int kontrola_n, long long **ID_cislo_osoby, int pocet_zaznamov){

    long long ID;
    int vymazane = 0;

    scanf("%lld",&ID);

    if(kontrola_n == 0){
        printf("Polia nie su vytvorene\n");
    }

    else {

        for (int i = 0; i < pocet_zaznamov; i++) {
            if (ID == (*ID_cislo_osoby)[i]) {

                long long *temp = malloc((pocet_zaznamov - 1) * sizeof(long long*));

                if (i != 0)         //ak je to 1. zaznam
                    memcpy(temp, *ID_cislo_osoby, i * sizeof(long long)); // kopiruje pred vymazanym

                if (i != (pocet_zaznamov - 1))      //ak je to posledny zaznam
                    memcpy(temp+i, *ID_cislo_osoby+i+1, (pocet_zaznamov - i - 1) * sizeof(long long)); // kopiruje po vymazanom

                free(*ID_cislo_osoby);      //uvoli sa povodne a nahradi sa za temp
                *ID_cislo_osoby = temp;

                vymazane ++;    //kvoli printf
                pocet_zaznamov--; //zmensi sa velkost pola ked mazeme
                i--; //ak su zaznamy hned za sebou lebo posuvam cele pole o 1 a ptm to bude na rovnakom indexe
            }

        }

        printf("Vymazalo sa: %d zaznamov !\n", vymazane);
    }
}
//----------------------------------------------------------------------------------------------------------------------------//
int main(){

    char vstup;
    int kontrola_n=0,kontrola_v=0,alokovane=0;

    //nastavim si polia na NULL
    FILE *p_text = NULL;
    long long *ID_cislo_osoby = NULL;
    char **mer_modul = NULL;
    char **typ_veliciny = NULL;
    float *hodnota = NULL;
    char **cas_merania = NULL;
    long long *datum = NULL;

    int pocet_zaznamov=0;

    //jednotlive prikazy, kontrola ci boli stlacene
    while(vstup != 'k'){
        switch(vstup=getchar()) {
            case 'n':
                function_n(&p_text,kontrola_v,alokovane,&pocet_zaznamov, &ID_cislo_osoby, &mer_modul,
                           &typ_veliciny, &hodnota, &cas_merania, &datum);
                kontrola_n=1;
                alokovane=1;
                break;

            case 'v':
                function_v(&p_text,kontrola_n,pocet_zaznamov,&ID_cislo_osoby, &mer_modul,
                           &typ_veliciny, &hodnota, &cas_merania, &datum);
                kontrola_v=1;
                break;

            case 'o':

                //prikaz 'n' nebol aktivovany iba som zavolal funkciu, aby som mohol robit s poliami
                function_n(&p_text,kontrola_v,alokovane,&pocet_zaznamov, &ID_cislo_osoby, &mer_modul,
                           &typ_veliciny, &hodnota, &cas_merania, &datum);
                alokovane=1;

                function_o(&p_text,pocet_zaznamov, &mer_modul, &typ_veliciny, &hodnota, &cas_merania, &datum);

                break;

            case 'k':
                fclose(p_text);

                //uvolnim polia z 'n' pomocne polia som uvolnoval hned v jednotlivych funkciach
                //nechapal som ci sa tu mali dealokovat iba polia z 'n' alebo vsetky ktore boli pouzite

                if(alokovane == 1) {
                    for (int i = 0; i < pocet_zaznamov; i++) {
                        free((mer_modul)[i]);
                        free((typ_veliciny)[i]);
                        free((cas_merania)[i]);
                    }
                    free(mer_modul);
                    free(typ_veliciny);
                    free(hodnota);
                    free(cas_merania);
                    free(datum);

                    ID_cislo_osoby = NULL;
                    mer_modul = NULL;
                    typ_veliciny = NULL;
                    hodnota = NULL;
                    cas_merania = NULL;
                    datum = NULL;
                    p_text = NULL;
                }
                return 0;

            case 'c':
                function_c(&p_text);
                break;

            case 's':
                function_s(&p_text, kontrola_n, &pocet_zaznamov,&mer_modul,
                           &typ_veliciny, &hodnota, &cas_merania, &datum);
                break;

            case 'h':
                function_h(&p_text, kontrola_n, pocet_zaznamov, &typ_veliciny, &hodnota);
                break;

            case 'r':
                function_r(kontrola_n,&cas_merania, pocet_zaznamov);
                break;

            case 'z':
                function_z(kontrola_n, &ID_cislo_osoby, pocet_zaznamov);
                break;
        }
    }
    return 0;
}