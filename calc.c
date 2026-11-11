#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "calc.h"
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

bool IsSameOpen(char open,char close){
    if(open=='[') return close==']';
    else if(open=='(') return close==')';
    else if(open=='{') return close=='}';
    else return false;
}

void FreeCalc(Calc * calc){
    for (uint i=0;calc->word[i]!=NULL;i++) {
        free(calc->word[i]);
    }
    //printf("finished split\n");

    free(calc->word);
    free(calc);
}

bool AreDelimitersValid(Calc * calc){
    Stack * stack=StackCreateEmpty();
    for (uint arg=0;calc->word[arg]!=NULL;arg++){
        for (uint i=0;calc->word[arg][i]!='\0';i++){
            char current=calc->word[arg][i];
            if(current=='(' || current=='[' || current=='{') stack=StackPush(stack,current);
            else if(current==')' || current==']' || current=='}') {
                if(StackIsEmpty(stack)) return false;

                char c = StackGetValue(stack);
                if(!IsSameOpen(c, current)){
                    return false;
                }
                stack = StackPop(stack);
            }
        }
    }
    bool result=StackIsEmpty(stack);
    StackFree(stack);
    return result;
}

void trim_trailing_zeros(char *str) {
    // Only trim if the string contains a decimal point
    if (strchr(str, '.') != NULL) {
        int len = strlen(str);
        
        // Start from the last character before the null terminator
        for (int i = len - 1; i >= 0; i--) {
            if (str[i] == '0') {
                str[i] = '\0'; // Remove the zero
            } else if (str[i] == '.') {
                str[i] = '\0'; // Remove the decimal point and stop
                break;
            } else {
                break; // Found a non-zero digit, stop trimming
            }
        }
    }
}

uint countOpeningDelimiters(Calc *calc){
    uint count=0;
    for (uint arg=0;calc->word[arg]!=NULL;arg++){
        for (uint i=0;calc->word[arg][i]!='\0';i++){
            char current=calc->word[arg][i];
            if(current=='(' || current=='[' || current=='{') count++;
        }
    }
    return count;   
}

void printCalc(Calc * calc){
    printf("word ->");
    for (uint i=0;calc->word[i]!=NULL;i++){
        trim_trailing_zeros(calc->word[i]);
        printf("|%s|",calc->word[i]);
    }
    printf("\n");
}

static void split(Calc * calc){
    size_t size=0;
    size_t index=0;
    for (uint i=0;calc->word[i]!=NULL;i++){
        size+=strlen(calc->word[i]);   
    }
    char **word=malloc((size+1)*sizeof(*word));
    if (word==NULL){
        FreeCalc(calc);
        fprintf(stderr,"Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    
    for (uint i=0;calc->word[i]!=NULL;i++){
        for (uint j=0;calc->word[i][j]!='\0';j++,index++){
            word[index]=malloc(2*sizeof(char));
            if (word[index]==NULL){
                for (size_t k=0;k<index;k++){
                    free(word[k]);
                }
                free(word);
                FreeCalc(calc);
                fprintf(stderr,"Not enough memory\n");
                exit(EXIT_FAILURE);
            }
            word[index][0]=calc->word[i][j];
            word[index][1]='\0';
        }
    }
    
    char ** old=calc->word;
    calc->word=word;
    for (uint i=0;old[i]!=NULL;i++){
        free(old[i]);
    }
    free(old);
    calc->word[index]=NULL;
}

uint calctype(char c){
    if ((c >= '0' && c <= '9') || (c == '.' || c== ',')) {
        return NUMBER;
    } 
    else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
        return DELIMITER;
    } 
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
        return OPERATION;
    } 
    else if (isalpha(c)){
        return VARIABLE;
    } else return 99;
} 

uint getCalctype(char *c){
    if(strlen(c)==1) return calctype(c[0]);
    else return NUMBER;

}

static void combineDigits(Calc * calc){
    split(calc);

    size_t count=0;
    while (calc->word[count]!=NULL) count++;

    char **tokens=calloc(count+1, sizeof(*tokens));
    if(tokens==NULL){
        FreeCalc(calc);
        fprintf(stderr,"Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    size_t token_count=0;
    for (size_t i=0;i<count;){
        size_t length=1;
        if(calctype(calc->word[i][0])==NUMBER){
            while(i+length<count &&
                  calctype(calc->word[i+length][0])==NUMBER){
                length++;
            }
        }
        if(calc->word[i][0]=='-' && (i==0 || calctype(calc->word[i-1][0])==OPERATION)){
            while(i+length < count && calctype(calc->word[i+length][0])==NUMBER){
                length++;
            }
        }
        tokens[token_count]=malloc(length+1);
        if(tokens[token_count]==NULL){
            for(size_t j=0;j<token_count;j++) free(tokens[j]);
            free(tokens);
            FreeCalc(calc);
            fprintf(stderr,"Not enough memory\n");
            exit(EXIT_FAILURE);
        }
        for(size_t j=0;j<length;j++){
            tokens[token_count][j]=calc->word[i+j][0];
        }
        tokens[token_count][length]='\0';
        token_count++;
        i+=length;
    }

    for(size_t i=0;i<count;i++) free(calc->word[i]);
    free(calc->word);
    calc->word=tokens;

}

uint sizeCalc(Calc *calc){
    uint size=0;
    for (;calc->word[size]!=NULL;size++);
    return size+1;
}

void moveResult(Calc * calc, uint first, uint last, char * result){
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

double fromStringToDouble(char * c) {
    bool point=false;
    bool start=0;
    double ent = 0;
    double dic = 0 ;
    uint size=0;
    if (c[0]=='-')start=1;
    for (uint i = start; c[i] != '\0'; i++) {
            if (c[i]!='.' && c[i]!=','){
                if(point){
                    dic=dic * 10 + (c[i] - '0');
                    size++;
                }
                else ent = ent * 10 + (c[i] - '0');
            }
            else {
                point=true;
            }
        }
    return (start?-1:1)*(ent + dic/pow(10,size));
    
}

char *fromDoobleToString(double number) {
    // Increased buffer size to 64 to safely hold a double
    char *result = malloc(64 * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "not enough space\n");
        return NULL; 
    }
    
    // Changed %d to %f for double
    snprintf(result, 64, "%f", number);

    return result;
}

bool isMixOperation(Calc * calc) {
    bool found[2] = {false, false}; // 0: +-, 1: */
    
    for (uint i = 0; calc->word[i] != NULL; i++) {
        if(getCalctype(calc->word[i]) == OPERATION) {
            if(calc->word[i][0] == '-' || calc->word[i][0] == '+') found[0] = true;
            else if(calc->word[i][0] == '*' || calc->word[i][0] == '/') found[1] = true;
        }
    }
    return found[0] && found[1]; 
}

double calculateSimple(double first,char operation,double last,Calc * calc){
    
    double result;
        if(operation=='*'){
            result =first*last;
        }
        else if (operation=='/'){
            if(last==0){
                fprintf(stderr,"error division by 0\n");
                FreeCalc(calc);
                exit(EXIT_FAILURE);
            }
            result =first/last;
        }
        else if (operation=='+'){
            result =first+last;
        }
        else if (operation=='-'){
            result =first-last;
        }
        return result;
}

bool isPrio(Calc *calc,uint * index){
    for(uint i=0;calc->word[i]!=NULL;i++){
        if(calc->word[i][0]=='*'||calc->word[i][0]=='/'){
            *index=i;
            return true;
        }
    }
    return false;
}

char * solveSimple(Calc * calc){
    uint size=sizeCalc(calc);
    if (size==3){
        double first=fromStringToDouble(calc->word[0]);
        double last=fromStringToDouble(calc->word[3]);
        char operation=calc->word[1][0];
        double result=calculateSimple(first,operation,last,calc);
        return fromDoobleToString(result);
    }else if (isMixOperation(calc)){
        
        uint index;
        bool prio=isPrio(calc,&index);
        
        while (prio){
            double first=fromStringToDouble(calc->word[index-1]);
            char operation=calc->word[index][0];
            double last=fromStringToDouble(calc->word[index+1]);
            char * result=fromDoobleToString(calculateSimple(first,operation,last,calc));
            moveResult(calc, index-1, index+1,result);
            prio=isPrio(calc,&index);
        }
        double first=fromStringToDouble(calc->word[0]);
        char operation=calc->word[1][0];
        double last=fromStringToDouble(calc->word[2]);
        first=calculateSimple(first,operation,last,calc);
        uint size=sizeCalc(calc);
        index=3;
        while (size>index+1){
            operation=calc->word[index][0];
            last=fromStringToDouble(calc->word[index+1]);
            first=calculateSimple(first,operation,last,calc);
            index+=2;
        }
        return fromDoobleToString(first);
    }else{  
        double first=fromStringToDouble(calc->word[0]);
        char operation=calc->word[1][0];
        double last=fromStringToDouble(calc->word[2]);
        first=calculateSimple(first,operation,last,calc);
        uint size=sizeCalc(calc);
        uint index=3;
        while (size>index+1){
            operation=calc->word[index][0];
            last=fromStringToDouble(calc->word[index+1]);
            first=calculateSimple(first,operation,last,calc);
            index+=2;
        }
        return fromDoobleToString(first);
    }
}

void evaluateParenthesizedExpression(Calc * calc){
    uint index=0;
    for(;calc->word[index]!=NULL && getCalctype(calc->word[index])!=DELIMITER;index++);
    uint first=index+1;
    uint depth=1;
    for(index=first;calc->word[index]!=NULL && depth>0;index++){
        char current=calc->word[index][0];
        if(current=='(' || current=='[' || current=='{'){
            depth++;
        } else if(current==')' || current==']' || current=='}'){
            depth--;

        }
    }
    uint last=index-2;
    
    char ** par=malloc((last-first+2)*sizeof(char *));
    if (par==NULL){
        FreeCalc(calc);
        fprintf(stderr, "A delimiter is not closed\n");
        return ;
    }
    for (uint i=0;i+first<last+1;i++){
        size_t len = strlen(calc->word[i + first]);
    
        par[i] = malloc(len + 1);
        
        if (par[i] != NULL) {
            strcpy(par[i], calc->word[i + first]);
        }
    }
    par[last-first+1]=NULL;
    char * result=solve(par);
    moveResult(calc,first-1,last+1,result);
    for (uint x=0;par[x]!=NULL;x++) free(par[x]);
    free(par);
}

char *solve(char **words) {
    if (words == NULL || words[0] == NULL) {
        fprintf(stderr, "no calculation provided\n");
        return NULL;
    }

    size_t count = 0;
    size_t input_length = 0;
    while (words[count] != NULL) {
        input_length += strlen(words[count]);
        count++;
    }
    if (input_length == 0) {
        fprintf(stderr, "no calculation provided\n");
        return NULL;
    }

    Calc *calc = calloc(1, sizeof(*calc));
    if (calc == NULL) {
        fprintf(stderr, "not enough space\n");
        return NULL;
    }
    calc->word = calloc(count + 1, sizeof(*calc->word));
    if (calc->word == NULL) {
        free(calc);
        fprintf(stderr, "not enough space\n");
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        size_t length = strlen(words[i]) + 1;
        calc->word[i] = malloc(length);
        if (calc->word[i] == NULL) {
            FreeCalc(calc);
            fprintf(stderr, "not enough space\n");
            return NULL;
        }
        memcpy(calc->word[i], words[i], length);
    }

    if (!AreDelimitersValid(calc)) {
        FreeCalc(calc);
        fprintf(stderr, "a parenthesis is not closed\n");
        return NULL;
    }
    uint openingDelimiters=countOpeningDelimiters(calc);
    combineDigits(calc);
    if (openingDelimiters>0){
        evaluateParenthesizedExpression(calc);
    }
    
    char * solution=solveSimple(calc);
    FreeCalc(calc);
    return solution;
}