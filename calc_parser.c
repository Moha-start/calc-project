#include "calc.h"
#include "math.h"

bool isMatchingDelimiter(char open,char close){
    if(open=='[') return close==']';
    else if(open=='(') return close==')';
    else if(open=='{') return close=='}';
    else return false;
}

bool areDelimitersValid(Calc * calc){
    Stack * stack=StackCreateEmpty();
    for (uint arg=0;calc->word[arg]!=NULL;arg++){
        for (uint i=0;calc->word[arg][i]!='\0';i++){
            char current=calc->word[arg][i];
            if(current=='(' || current=='[' || current=='{'){
                stack=StackPush(stack,current);
            }
            else if(current==')' || current==']' || current=='}') {
                if(StackIsEmpty(stack)){
                    StackFree(stack);
                    return false;
                }

                char c = StackGetValue(stack);
                if(!isMatchingDelimiter(c, current)){
                    StackFree(stack);
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

void trimTrailingZeros(char *str) {
    if (strchr(str, '.') != NULL) {
        int len = strlen(str);
        
        for (int i = len - 1; i >= 0; i--) {
            if (str[i] == '0') {
                str[i] = '\0'; 
            } else if (str[i] == '.') {
                str[i] = '\0';
                break;
            } else {
                break;
            }
        }
    }
}

uint countOpeningDelimiters(Calc * calc){
    uint count=0;
    for (uint index=0;calc->word[index]!=NULL;index++){
        char first=calc->word[index][0];
        if(first=='(' || first=='[' || first=='{'){
            if (index==0 || !isUnaryOperation(calc->word[index-1])) count++;
        }
    }
    return count;
}

uint getCharType(char c){
    if ((c >= '0' && c <= '9') || (c == '.' || c== ',')) {
        return NUMBER;
    } 
    else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
        return DELIMITER;
    } 
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c =='^' || c == '%') {
        return OPERATION;
    } 
    else if (isalpha(c)){
        return VARIABLE;
    } else return 99;
} 

bool isUnaryOperation(char *c){
    char ln[]="ln";
    char log[]="log";
    char sin[]="sin";
    char cos[]="cos";
    char tan[]="tan";
    if (strcmp(c,ln)==0) return true;
    else if (strcmp(c,log)==0) return true;
    else if (strcmp(c,sin)==0) return true;
    else if (strcmp(c,cos)==0) return true;
    else if (strcmp(c,tan)==0) return true;
    else return false;
}

uint getTokenType(char *c){
    if(strlen(c)==1) return getCharType(c[0]);
    else if(isUnaryOperation(c)) return SINGLEOPERATION;
    else return NUMBER;

}

double stringToDouble(char * c) {
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
    return (start?-1:1)*(ent + dic/power(10,size));
    
}

char * doubleToString(double number) {
    char *result = malloc(64 * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "not enough space\n");
        return NULL; 
    }
    
    snprintf(result, 64, "%f", number);

    return result;
}

void parseAndEvalArgs(char * number,char ** value){
    if(number[0]=='('){
        uint size1=0;
        uint size2=0;
        bool isfirst=true;
        for (uint index=1;number[index]!='\0';index++){
            if(number[index]==','){
                isfirst=false;
            }else{
                if(isfirst) size1++;
                else size2++;
            }
        }
        size2>0 ? size2--:size1--;
        char * nbFirst=malloc(size1 *sizeof(char)+1);
        if (nbFirst==NULL){
            
            fprintf(stderr,"Not enagh space\n");
            exit(EXIT_FAILURE);
        }
        char * nbSecond=NULL;
        if (size2!=0){
            nbSecond=malloc(size2 *sizeof(char)+1);
            if (nbSecond==NULL){
                free(nbFirst);
                fprintf(stderr,"Not enagh space\n");
                exit(EXIT_FAILURE);
            }
        }
        uint index=1;
        for (;index<size1+1;index++)nbFirst[index-1]=number[index];
        nbFirst[index-1]='\0';
        index++;
        for (;index<size2+size1+2;index++)nbSecond[index-size1]=number[index];
        if(!isfirst) nbSecond[size2]='\0';
        value[0]=solveString(nbFirst);
        if(value[0]==NULL){
            free(nbFirst);
            free(nbSecond);
        }
        if (size2!=0) value[1]=solveString(nbSecond);
        free(nbFirst);
        free(nbSecond);
    }else{
        fprintf(stderr,"a Single Opearation must start with (\n");
        exit(EXIT_FAILURE);
    }
}

static void split(Calc * calc){
    size_t size=0;
    size_t index=0;
    for (uint i=0;calc->word[i]!=NULL;i++){
        size+=strlen(calc->word[i]);   
    }
    char **word=malloc((size+1)*sizeof(*word));
    if (word==NULL){
        freeCalc(calc);
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
                freeCalc(calc);
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

void combineDigits(Calc * calc){
    // TODO : add the replacement of e and PI
    split(calc);

    size_t count=0;
    while (calc->word[count]!=NULL) count++;

    char **tokens=calloc(count+1, sizeof(*tokens));
    if(tokens==NULL){
        freeCalc(calc);
        fprintf(stderr,"Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    size_t token_count=0;
    for (size_t i=0; i<count;) {
        size_t length=1;
        
        if(getCharType(calc->word[i][0])==NUMBER){
            while(i+length<count && getCharType(calc->word[i+length][0])==NUMBER){
                length++;
            }
        }
        else if(calc->word[i][0]=='-' && (i==0 || getCharType(calc->word[i-1][0])==OPERATION)){
            while(i+length < count && getCharType(calc->word[i+length][0])==NUMBER){
                length++;
            }
        }
        else if(getCharType(calc->word[i][0])==VARIABLE){
            while(i+length < count && getCharType(calc->word[i+length][0])==VARIABLE){
                length++;
            }
        }
        else if(getCharType(calc->word[i][0])==DELIMITER && 
                (calc->word[i][0]=='(' || calc->word[i][0]=='[' || calc->word[i][0]=='{')) {
            
            if (token_count > 0 && isUnaryOperation(tokens[token_count-1])) {
                int depth = 1;
                while(i+length < count && depth > 0) {
                    char c = calc->word[i+length][0];
                    if (c == '(' || c == '[' || c == '{') depth++;
                    else if (c == ')' || c == ']' || c == '}') depth--;
                    length++;
                }
            }
        }

        tokens[token_count]=malloc(length+1);
        if(tokens[token_count]==NULL){
            for(size_t j=0;j<token_count;j++) free(tokens[j]);
            free(tokens);
            freeCalc(calc);
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