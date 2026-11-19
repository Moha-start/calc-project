
#include "calc.h"
#include "math.h"

Calc * getCalc(void) {
    uint capacity=32;
    uint length=0;
    printf("Enter your calculation: ");

    char * buffer=malloc(capacity*sizeof(char));
    if (buffer==NULL){
        fprintf(stderr,"Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    int inp;
    while ((inp = getchar()) != '\n' && inp != EOF){
        if(length+1>=capacity){
            capacity*=2;
            char * temp=realloc(buffer,capacity);
            if(temp==NULL){
                free(buffer);
                fprintf(stderr,"Not enough memory\n");
                exit(EXIT_FAILURE);
            }
            buffer=temp;
        }
        if(inp!=32)buffer[length++]=(char)inp;
    }
    if (length==0 && inp==EOF){
        free(buffer);
        fprintf(stderr,"reached the end of file\n");
        exit(EXIT_FAILURE);
    }
    buffer[length]='\0';
    Calc * calcu=malloc(sizeof(Calc));
    if (calcu==NULL){
        free(buffer);
        fprintf(stderr,"Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    calcu->word=malloc(2*sizeof(*calcu->word));
    if (calcu->word==NULL){
        free(buffer);
        free(calcu);
        fprintf(stderr,"Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    calcu->word[0]=buffer;
    calcu->word[1]=NULL;
    return calcu;
}

void freeCalc(Calc * calc){
    for (uint i=0;calc->word[i]!=NULL;i++) {
        free(calc->word[i]);
    }
    free(calc->word);
    free(calc);
}

void printCalc(Calc * calc){
    printf("word ->");
    for (uint i=0;calc->word[i]!=NULL;i++){
        trimTrailingZeros(calc->word[i]);
        printf("|%s|",calc->word[i]);
    }
    printf("\n");
}

uint getCalcSize(Calc *calc){
    uint size=0;
    for (;calc->word[size]!=NULL;size++);
    return size+1;
}

void replaceRangeWithResult(Calc * calc, uint first, uint last, char * result){
    for(uint i = first; i <= last; i++) {
        if(calc->word[i] != NULL) {
            free(calc->word[i]);
        }
    }
    
    calc->word[first] = result;
    
    uint tempF = first + 1;
    uint tempE = last + 1; 
    while(calc->word[tempE] != NULL){
        calc->word[tempF] = calc->word[tempE];
        tempF++;
        tempE++;
    }
    
    calc->word[tempF] = NULL;
    
    size_t new_size = tempF + 1;
    char **temp = realloc(calc->word, new_size * sizeof(char *));
    if (temp == NULL) {
        fprintf(stderr, "Error while shrinking the allocation\n");
    } else {
        calc->word = temp;
    }
}