#include "calc.h"
#include "math.h"

char * solveString(char *c){
    char ** list=malloc(2*sizeof(char *));
    if (list == NULL) {
        fprintf(stderr, "not enough space\n");
        return NULL;
    }
    list[0]=c;
    list[1]=NULL;
    char * res=solve(list);
    free(list);
    return res;
}

bool hasMixedOperations(Calc * calc) {
    bool found[2] = {false, false};
    
    for (uint i = 0; calc->word[i] != NULL; i++) {
        if(getTokenType(calc->word[i]) == OPERATION || getTokenType(calc->word[i]) == SINGLEOPERATION) {
            if(calc->word[i][0] == '-' || calc->word[i][0] == '+') found[0] = true;
            else if(calc->word[i][0] == '*' || calc->word[i][0] == '/') found[1] = true;
        }
    }
    return found[0] && found[1]; 
}

double applyBinaryOperator(double first,char operation,double last,Calc * calc){
    double result;
        if(operation=='*'){
            result =first*last;
        }
        else if (operation=='/'){
            if(last==0){
                fprintf(stderr,"error division by 0\n");
                freeCalc(calc);
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
        else if(operation=='^'){
            result=power(first,last);
        }else if(operation=='%'){
            result = mod(first, last);
        }
        return result;
}

bool findHighestPrecedenceOp(Calc *calc, uint * index) {
    char *last = NULL;
    bool found = false;

    for (uint i = 0; calc->word[i] != NULL; i++) {
        if (getTokenType(calc->word[i]) == OPERATION || getTokenType(calc->word[i]) == SINGLEOPERATION) {
            if (last == NULL || hasHigherPrecedence(calc->word[i], last)) {
                last = calc->word[i];
                *index = i;
                found = true;
            }
        }
    }
    return found;
}

uint getOperatorPrecedence(char * c) {
    uint prio=0;
    if (isUnaryOperation(c)){
        return prio;
    }
    prio++;
    if (c[0] == '^') {
        return prio;
    }
    prio++;
    if (c[0] == '*' || c[0] == '/' || c[0] == '%'){
        return prio;
    }
    prio++;
    if (c[0] == '+' || c[0] == '-') {
        return prio;
        }
    return 99;
}

bool hasHigherPrecedence(char * c1, char * c2) {
    return getOperatorPrecedence(c1) < getOperatorPrecedence(c2);
}

char * evalUnaryOperation(char * opearation, char * number){
    char ln[] = "ln";
    char log[] = "log";
    char sin[] = "sin";
    char cos[] = "cos";
    char tan[] = "tan";
    bool is_deg = true;
    double result = 0.0;

    // Use calloc so values[0] and values[1] are initialized to NULL
    char ** values = calloc(2, sizeof(char *));
    if (values == NULL){
        // TODO : impliment
        fprintf(stderr, "Not enagh space\n");
        exit(EXIT_FAILURE);
    }

    parseAndEvalArgs(number, values);
    double nb = stringToDouble(values[0]);

    if (strcmp(opearation, log) == 0){
        free(values[0]);
        free(values[1]);
        free(values);
        fprintf(stderr, "Not yet added -> log\n");
        exit(EXIT_FAILURE);
    } else if (strcmp(opearation, ln) == 0){
        if (nb <= 0) {
            free(values[0]);
            free(values[1]);
            free(values);
            fprintf(stderr, "there you have given to ln a negative or 0 number\n");
            exit(EXIT_FAILURE);
        }

        double y = (nb - 1.0) / (nb + 1.0);
        double y2 = y * y; 
        double current_y = y;

        for (uint i = 1; i < (PRECISION * 2) + 5; i = i + 2) {
            result = result + (current_y / i);
            current_y = current_y * y2; 
        }
        result = result * 2.0;
    } else if (strcmp(opearation, sin) == 0){
        result = computeTrig(nb, true, is_deg);
    } else if (strcmp(opearation, cos) == 0){
        result = computeTrig(nb, false, is_deg);
    } else if (strcmp(opearation, tan) == 0){ 
        result = computeTrig(nb, true, is_deg) / computeTrig(nb, false, is_deg);
    }

    free(values[0]);
    free(values[1]);
    free(values);
    return doubleToString(result);
}

char * solveUnaryOperation(Calc * calc, uint index){
    char * operation = calc->word[index];
    char ** calculation = malloc(2 * sizeof(char *));
    if (calculation == NULL) {
        fprintf(stderr, "there no space left\n");
        exit(EXIT_FAILURE);
    }

    calculation[0] = malloc(strlen(calc->word[index + 1]) + 1);
    if (calculation[0] == NULL) {
        fprintf(stderr, "there no space left\n");
        free(calculation);
        exit(EXIT_FAILURE);
    }

    strcpy(calculation[0], calc->word[index + 1]);
    calculation[1] = NULL;

    char * number = solve(calculation);
    free(calculation[0]);
    free(calculation);

    if (number == NULL) {
        return NULL;
    }

    // Wrap number in parentheses so parseAndEvalArgs accepts it
    char * wrapped = malloc(strlen(number) + 3); // '(' + number + ')' + '\0'
    if (wrapped == NULL) {
        free(number);
        fprintf(stderr, "there no space left\n");
        exit(EXIT_FAILURE);
    }
    sprintf(wrapped, "(%s)", number);
    free(number);

    char * result = evalUnaryOperation(operation, wrapped);
    free(wrapped);
    return result;
}

char * solveFlatExpression(Calc * calc) {
    uint index;
    
    while (findHighestPrecedenceOp(calc, &index)) {
        char * result;
        double first = stringToDouble(calc->word[index - 1]);
        char operation = calc->word[index][0];
        double last = stringToDouble(calc->word[index + 1]);
        result = doubleToString(applyBinaryOperator(first, operation, last, calc));
        
        replaceRangeWithResult(calc, index - 1, index + 1, result);
    }
    
    double final_result = stringToDouble(calc->word[0]);
    return doubleToString(final_result);
}

void evalDelimitedExpression(Calc * calc){
    uint index=0;
    for(;calc->word[index]!=NULL && getTokenType(calc->word[index])!=DELIMITER;index++);
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
        freeCalc(calc);
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
    replaceRangeWithResult(calc,first-1,last+1,result);
    for (uint x=0;par[x]!=NULL;x++) free(par[x]);
    free(par);
}

void evalUnaryOperations(Calc *calc){
    if (calc->word == NULL || calc->word[0] == NULL) return;

    for (uint index = 1; calc->word[index] != NULL; index++){
        char first = calc->word[index][0];
        if (first == '(' && isUnaryOperation(calc->word[index - 1])){
            char * result = evalUnaryOperation(calc->word[index - 1], calc->word[index]);
            replaceRangeWithResult(calc, index - 1, index, result);
            index--; // Adjust for the shifted/shrunk array
        }
    }
}

char * solve(char ** words) {
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
            freeCalc(calc);
            fprintf(stderr, "not enough space\n");
            return NULL;
        }
        memcpy(calc->word[i], words[i], length);
    }

    if (!areDelimitersValid(calc)) {
        freeCalc(calc);
        fprintf(stderr, "a parenthesis is not closed\n");
        return NULL;
    }
    
    combineDigits(calc);
    evalUnaryOperations(calc);
    uint openingDelimiters=countOpeningDelimiters(calc);
    if (openingDelimiters>0){
        evalDelimitedExpression(calc);
    }
    
    char * solution=solveFlatExpression(calc);
    freeCalc(calc);
    return solution;
}