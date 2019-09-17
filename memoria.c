/* ***************************************************************************
 *  Este programa implementa um gerenciador de memória hipotético utilizando a linguagem C
 *  ****************************************************************************
 *  Authors: Endrius Ewald <endrius.ewald@acad.purcs.br>
 *  Copyright: GNU General Public License
 *  Description: Trabalho 2
 *  File Name: memoria.c
 *  Version: 1.0 (22/06/2017)
 ****************************************************************************
*/

#define _GNU_SOURCE

/*INCLUDES*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*DEFINES*/
#define MAX_ARG 5
#define MEM_SIZE 5000
#define LIST_SIZE 100
#define DEBUG 1


/*STRUCTS*/
typedef struct {
    char nome[30];
    char* inicio;
    int tipo;
    int x;
    int y;
}Variavel;

/*VARIAVEIS GLOBAIS*/
char memoria[MEM_SIZE];
char* ptrMemoria = memoria;
Variavel listaDeVariaveis[LIST_SIZE];
Variavel* ptrLista = listaDeVariaveis;


/*FUNCOES*/
//strcasestr for MinGW

char *strcasestr(const char *str, const char *pattern) {
    size_t i;

    if (!*pattern)
        return (char*)str;

    for (; *str; str++) {
        if (toupper(*str) == toupper(*pattern)) {
            for (i = 1;; i++) {
                if (!pattern[i])
                    return (char*)str;
                if (toupper(str[i]) != toupper(pattern[i]))
                    break;
            }
        }
    }
    return NULL;
}
 

//Carrega parametros
void loadParam(char* str, char* param[]){
    //Dropa o '\n' carregado pelo getline
    str[strlen(str)-1] = '\0';

    //Subdivide parâmetros
    int paramCount = 0;
    char *ch;
    ch = strtok(str, " ");
    while (ch != NULL) {
//        printf("%s\n", ch);
        param[paramCount] = ch;
        ch = strtok(NULL, " ,");
        paramCount++;
    }
}

//Limpa parametros
void freeParam(char* param[]){
    for (int i = 0; i < MAX_ARG; ++i) {
        param[i] = NULL;
    }
}

//Imprime parametros
void printParam(char* param[]){
    for (int i = 0; i < MAX_ARG; ++i) {
        printf("Param[%i] >%s<\n",i,param[i]);
    }
}

//Imprime mensagem de aguarde
void printAguarde(int choice){
    if(choice > 0){
        printf("COMANDO INVÁLIDO\n");
    }
    printf("Aguardando comando: ...\n");
}

/*Prints do dump*/
void printVetorChar(Variavel* v){
    char* pMem = v->inicio;
    printf("VET: Char - %s - {",v->nome);
    for (int i = 0; i < v->x; ++i) {
        char c = *(pMem+i);
        if(i != v->x-1){
            printf("'%c', ",c);
        }else{
            printf("'%c'",c);
        }
    }
    printf("}\n");
}

void printVetorInt(Variavel* v) {
    char *pMem = v->inicio;
    printf("VET: Int - %s - {", v->nome);
    for (int i = 0; i < v->x; ++i) {
        int* pInt = (int*) (pMem + (i* sizeof(int)));
        int k = *pInt;
        if (i != v->x - 1) {
            printf("%d, ", k);
        } else {
            printf("%d", k);
        }
    }
    printf("}\n");
}

void printVetorFloat(Variavel* v) {
    char *pMem = v->inicio;
    printf("VET: Float - %s - {", v->nome);
    for (int i = 0; i < v->x; ++i) {
        float* pFloat = (float*) (pMem + (i* sizeof(float)));
        float k = *pFloat;
        if (i != v->x - 1) {
            printf("%f, ", k);
        } else {
            printf("%f", k);
        }
    }
    printf("}\n");
}

void printMatChar(Variavel* v){
    char* pMem = v->inicio;
    printf("MAT: Char - %s - \n{",v->nome);
    for (int i = 0; i < v->x; ++i) {
        for (int j = 0; j < v->y; ++j) {
            char c = *(pMem + (i*v->y) + j);
            printf("'%c', ", c);
        }
        if (i != v->x - 1) printf("\n");
    }
    printf("}\n");
}

void printMatInt(Variavel* v){
    char* pMem = v->inicio;
    printf("MAT: Int - %s - \n{",v->nome);
    for (int i = 0; i < v->x; ++i) {
        for (int j = 0; j < v->y; ++j) {
            int* pInt = (int*) (pMem + (i* sizeof(int)*v->y) + j*sizeof(int));
            int k = *pInt;
            printf("%d, ", k);
        }
        if (i != v->x - 1) printf("\n");
    }
    printf("}\n");
}

void printMatFloat(Variavel* v){
    char* pMem = v->inicio;
    printf("MAT: Float - %s - \n{",v->nome);
    for (int i = 0; i < v->x; ++i) {
        for (int j = 0; j < v->y; ++j) {
            float* pFloat = (float*) (pMem + (i* sizeof(float)*v->y) + j*sizeof(float));
            float k = *pFloat;
            printf("%f, ", k);
        }
        if (i != v->x - 1) printf("\n");
    }
    printf("}\n");
}

void printVariavel(Variavel* v){

    int type = v->tipo;
    char* pontoMemoria = v->inicio;
    int* ptrInteiro = (int*)pontoMemoria;
    int inteiro = *ptrInteiro;
    float* pFloat = (float*)pontoMemoria;
    float fl = *pFloat;

    switch (type){
        case 1:
            printf("VAR: Char - %s - %c\n",v->nome,*pontoMemoria);
            break;
        case 2:
            printf("VAR: Int - %s - %d\n",v->nome,inteiro);
            break;
        case 3:
            printf("VAR: Float - %s - %f\n",v->nome,fl);
            break;
        case 4:
            printVetorChar(v);
            break;
        case 5:
            printVetorInt(v);
            break;
        case 6:
            printVetorFloat(v);
            break;
        case 7:
            printMatChar(v);
            break;
        case 8:
            printMatInt(v);
            break;
        case 9:
            printMatFloat(v);
            break;

    }
}

void dump(){
    for (int i = 0; i < LIST_SIZE; ++i) {
        if((listaDeVariaveis+i)->inicio){//Otimizacao
            printVariavel(listaDeVariaveis+i);
        }else{
            break;
        }
    }
}

Variavel* getVarByName(char* name){
    for (int i = 0; i < LIST_SIZE; ++i) {
        if((listaDeVariaveis+i)->nome[0] != NULL){//Otimizacao
            if(!strcmp((listaDeVariaveis+i)->nome, name)) return (listaDeVariaveis+i);
        }else{
            break;
        }
    }
    return NULL;
}

/*SETS*/
void setChar(char* param[],char* ptMemoria){
    *ptMemoria = *param[2];
}

void setInt(char* param[],char* ptMemoria){
    int num = atoi(param[2]);

    int* pInt = &num;
    char* aux = (char*) pInt;

    for (int i = 0; i < sizeof(int); ++i) {
        *(ptMemoria+i) = *(aux+i);
    }
}

void setFloat(char* param[], char* ptMemoria){
    float f = (float) atof(param[2]);

    float* pFloat = &f;
    char* aux = (char*) pFloat;

    for (int i = 0; i < sizeof(float); ++i) {
        *(ptMemoria+i) = *(aux+i);
    }

}

void setVetChar(char* param[],char* ptMemoria){
    int indice = atoi(param[2]);
    *(ptMemoria+indice) = *param[3];
}

void setVetCharFull(char* param[],char* ptMemoria,Variavel* v){
    char* str = param[2];

    const char *c;
    int i = 0;
    for (c = str; *c; ++c, i++) {
        if(i<v->x){
            *(ptMemoria+i) = *(c);
        }else{break;}
    }
}

void setVetInt(char* param[],char* ptMemoria){
    int indice = atoi(param[2]);
    int num = atoi(param[3]);

    int* pInt = &num;
    char* aux = (char*) pInt;

    //Avanca memoria para o indice
    ptMemoria += indice*sizeof(int);

    for (int i = 0; i < sizeof(int); ++i) {
        *(ptMemoria+i) = *(aux+i);
    }
}

void setVetFloat(char* param[],char* ptMemoria){
    //set F 3 69.69
    int indice = atoi(param[2]);
    float num = (float) atof(param[3]);

    float* pFloat = &num;
    char* aux = (char*) pFloat;

    //Avanca memoria para o indice
    ptMemoria += indice*sizeof(float);

    for (int i = 0; i < sizeof(float); ++i) {
        *(ptMemoria+i) = *(aux+i);
    }
}

void setMatChar(char* param[],char* ptMemoria,Variavel* v){
    //set C 3 2 @
    int indiceX = atoi(param[2]);
    int indiceY = atoi(param[3]);
    *(ptMemoria+(indiceX*v->y)+indiceY) = *param[4];
}

void setMatInt(char* param[],char* ptMemoria,Variavel* v){
    //set C 3 2 69
    int indiceX = atoi(param[2]);
    int indiceY = atoi(param[3]);

    int num = atoi(param[4]);

    int* pInt = &num;
    char* aux = (char*) pInt;

    //Avanca memoria para o indice
    ptMemoria += (indiceX*sizeof(int)*v->y)+(indiceY*sizeof(int));

    for (int i = 0; i < sizeof(int); ++i) {
        *(ptMemoria+i) = *(aux+i);
    }
}

void setMatFloat(char* param[],char* ptMemoria,Variavel* v){
    //set C 3 2 69
    int indiceX = atoi(param[2]);
    int indiceY = atoi(param[3]);

    float num = (float) atof(param[4]);

    float* pFloat = &num;
    char* aux = (char*) pFloat;

    //Avanca memoria para o indice
    ptMemoria += (indiceX*sizeof(float)*v->y)+(indiceY*sizeof(float));

    for (int i = 0; i < sizeof(float); ++i) {
        *(ptMemoria+i) = *(aux+i);
    }
}

int set(char* param[]){
    Variavel* var = getVarByName(param[1]);
    if(var == NULL) return 0;
    int type = var->tipo;
    char aux;
    switch (type){
        case 1:
            setChar(param,var->inicio);
            break;
        case 2:
            setInt(param,var->inicio);
            break;
        case 3:
            setFloat(param,var->inicio);
            break;
        case 4:
            aux = *param[2];
            if(isdigit(aux)){
                setVetChar(param,var->inicio);
            }else{
                setVetCharFull(param,var->inicio,var);
            }
            break;
        case 5:
            setVetInt(param,var->inicio);
            break;
        case 6:
            setVetFloat(param,var->inicio);
            break;
        case 7:
            setMatChar(param,var->inicio,var);
            break;
        case 8:
            setMatInt(param,var->inicio,var);
            break;
        case 9:
            setMatFloat(param,var->inicio,var);
            break;
    }
    return 1;
}

/*CREATES*/
void createChar(char* param[]){

    Variavel v;
    v.tipo = 1;
    strcpy(v.nome, param[1]);

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(char);

    *ptrLista = v;
    ptrLista++;
}

void createInt(char* param[]){

    Variavel v;
    v.tipo = 2;
    strcpy(v.nome, param[1]);

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(int);

    *ptrLista = v;
    ptrLista++;
}

void createFloat(char* param[]){

    Variavel v;
    v.tipo = 3;
    strcpy(v.nome, param[1]);

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(float);

    *ptrLista = v;
    ptrLista++;
}

void createVetChar(char* param[]){
    int x = atoi(param[2]);
    Variavel v;
    v.tipo = 4;
    strcpy(v.nome, param[1]);
    v.x = x;

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(char)*x;

    *ptrLista = v;
    ptrLista++;

}

void createVetInt(char* param[]){
    int x = atoi(param[2]);
    Variavel v;
    v.tipo = 5;
    strcpy(v.nome, param[1]);
    v.x = x;

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(int)*x;

    *ptrLista = v;
    ptrLista++;

}

void createVetFloat(char* param[]){
    int x = atoi(param[2]);
    Variavel v;
    v.tipo = 6;
    strcpy(v.nome, param[1]);
    v.x = x;

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(float)*x;

    *ptrLista = v;
    ptrLista++;

}

int createVet(char* param[]){
    if(!strcasecmp(param[0],"create_vet_char")){
        createVetChar(param);
        return 1;
    }else if(!strcasecmp(param[0],"create_vet_int")) {
        createVetInt(param);
        return 1;
    }else if(!strcasecmp(param[0],"create_vet_float")) {
        createVetFloat(param);
        return 1;
    }else{//INVÁLIDO
        return 0;
    }
}

void createMatChar(char* param[]){
    int x = atoi(param[2]);
    int y = atoi(param[3]);
    Variavel v;
    v.tipo = 7;
    strcpy(v.nome, param[1]);
    v.x = x;
    v.y = y;

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(char)*x*y;

    *ptrLista = v;
    ptrLista++;
}

void createMatInt(char* param[]){
    int x = atoi(param[2]);
    int y = atoi(param[3]);
    Variavel v;
    v.tipo = 8;
    strcpy(v.nome, param[1]);
    v.x = x;
    v.y = y;

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(int)*x*y;

    *ptrLista = v;
    ptrLista++;
}

void createMatFloat(char* param[]){
    int x = atoi(param[2]);
    int y = atoi(param[3]);
    Variavel v;
    v.tipo = 9;
    strcpy(v.nome, param[1]);
    v.x = x;
    v.y = y;

    v.inicio = ptrMemoria;
    ptrMemoria += sizeof(float)*x*y;

    *ptrLista = v;
    ptrLista++;
}

int createMat(char* param[]){
    if(!strcasecmp(param[0],"create_mat_char")){
        createMatChar(param);
        return 1;
    }else if(!strcasecmp(param[0],"create_mat_int")) {
        createMatInt(param);
        return 1;
    }else if(!strcasecmp(param[0],"create_mat_float")) {
        createMatFloat(param);
        return 1;
    }else{//INVÁLIDO
        return 0;
    }
}

int create(char* param[]){
    //Verifica a existencia de uma variavel com mesmo nome
    Variavel* v = getVarByName(param[1]);
    if(v != NULL) return -1;
    if(strcasestr(param[0], "create_mat") != NULL){//MATRIZ
        return createMat(param);
    }else if(strcasestr(param[0], "create_vet") != NULL){//ARRAY
        return createVet(param);
    }else{//PRIMITIVA
        if(!strcasecmp(param[0],"create_char")){
            createChar(param);
            return 1;
        }else if(!strcasecmp(param[0],"create_int")) {
            createInt(param);
            return 1;
        }else if(!strcasecmp(param[0],"create_float")) {
            createFloat(param);
            return 1;
        }else{//INVÁLIDO
            return 0;
        }
    }
}

/*Prints individuais*/
void pChar(char* ptMemoria){
    printf("%c\n",*ptMemoria);
}

void pInt(char* ptMemoria){
    int* pInt = (int*) ptMemoria;
    printf("%d\n",*pInt);
}

void pFloat(char* ptMemoria){
    float* pFloat = (float*) ptMemoria;
    printf("%f\n",*pFloat);
}

void pVetChar(char* param[], char* ptMemoria){
    int indice = atoi(param[2]);
    printf("%c\n",*(ptMemoria+indice));
}

void pVetCharFull(char* param[], char* ptMemoria){
    printf("%s\n",ptMemoria);
}

void pVetInt(char* param[], char* ptMemoria){
    int indice = atoi(param[2]);
    int* pInt = (int*) ptMemoria;
    printf("%d\n",*(pInt+indice));
}

void pVetFloat(char* param[], char* ptMemoria){
    int indice = atoi(param[2]);
    float* pFloat = (float*) ptMemoria;
    printf("%f\n",*(pFloat+indice));
}

void pMatChar(char* param[], char* ptMemoria, Variavel* var){
    int x = atoi(param[2]);
    int y = atoi(param[3]);
    printf("%c\n",*(ptMemoria+(x*var->y)+y));
}

void pMatInt(char* param[], char* ptMemoria, Variavel* var){
    int x = atoi(param[2]);
    int y = atoi(param[3]);
    int* pInt = (int*) ptMemoria;
    printf("%d\n",*(pInt+(x*var->y)+y));
}

void pMatFloat(char* param[], char* ptMemoria, Variavel* var){
    int x = atoi(param[2]);
    int y = atoi(param[3]);
    float* pFloat = (float*) ptMemoria;
    printf("%f\n",*(pFloat+(x*var->y)+y));
}

int print(char* param[]){
    Variavel* var = getVarByName(param[1]);
    if(var == NULL) return 0;
    int type = var->tipo;
    switch (type){
        case 1:
            pChar(var->inicio);break;
        case 2:
            pInt(var->inicio);break;
        case 3:
            pFloat(var->inicio);break;
        case 4:
            if(param[2] != NULL){
                pVetChar(param,var->inicio);
            }else{
                pVetCharFull(param,var->inicio);
            }break;
        case 5:
            pVetInt(param,var->inicio);break;
        case 6:
            pVetFloat(param,var->inicio);break;
        case 7:
            pMatChar(param,var->inicio,var);break;
        case 8:
            pMatInt(param,var->inicio,var);break;
        case 9:
            pMatFloat(param,var->inicio,var);break;
    }
    return 1;
}

void process(char* str){

    //Sai se nao ouver nada
    if(!strcmp(str,"\n")){
        printAguarde(0);
        return;
    }

    //Variavel para retorno das funcoes
    int retorno;

    //Variavel para ler parametros
    char* param[MAX_ARG];
    freeParam(param);

    //Carrega parametros
    loadParam(str,param);

    //Verifica EXIT
    if(!strcasecmp(param[0],"exit")) exit(EXIT_SUCCESS);

    //Processa comandos
    if(!strcasecmp(param[0],"set")){
        retorno = set(param);
        if(retorno <= 0){
            printAguarde(1);
            return;
        }
    } else if (!strcasecmp(param[0],"print")){
        retorno = print(param);
        if(retorno <= 0){
            printAguarde(1);
            return;
        }
    }else if(strcasestr(param[0], "create") != NULL){
        retorno = create(param);
        if(retorno == 0){
            printAguarde(1);
            return;
        }else if(retorno==-1){
            printf("VARIAVEL JÁ EXISTENTE\n");
            printAguarde(1);
            return;
        }
    }else if (!strcasecmp(param[0],"dump")) {
        if(DEBUG) printf("DUMP MEMORIA\n");
        dump();
    }else{
        printAguarde(1);
        return;
    }

    //VERIFICA FINAL DA MEMORIA E AVISA
    if(ptrMemoria > memoria+MEM_SIZE){
        printf("OUT OF MEMORY\n");
    }

    //VERIFICA FINAL DAS VARIAVEIS E AVISA
    if(ptrLista > listaDeVariaveis+LIST_SIZE){
        printf("OUT OF LIST\n");
    }

    freeParam(param);
    printAguarde(0);

}

//TODO: AJUSTAR FUNCOES PARA TER MENOS CODIGO???
//char1,int2,float3
//char[]4,int[]5,float[]6
//char[][]7,int[][]8,float[][]9
int main(int argc, char** argv) {

    //VERIFICA ARQUIVO
    if(argc > 1) {

        FILE* fp;
        char* line = NULL;
        size_t len = 0;

        char* fileName = argv[1];

        //Arquivos tem q ter ultima linha em branco
        fp = fopen(fileName, "r");
        if (fp == NULL) {
            printf("Arquivo %s inexistente, o programa irá encerrar.\n", argv[1]);
            exit(EXIT_FAILURE);
        }

        printf("Lendo do arquivo %s\n",argv[1]);

        while (getline(&line, &len, fp) != -1) {
            printf("COMANDO: %s", line);
            process(line);
        }

        fclose(fp);

    }

    //Variavel leitura stdin
    char* str;
    str = (char*) malloc(sizeof(char)*100);

    //??
    size_t  n = 1024;

    printAguarde(0);
    //Repete até "exit"
    while(getline(&str,&n,stdin) > 0 ){
//        printf("%s",str);
        process(str);
    }
    return 0;
}
