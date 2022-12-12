#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define SIZE 20
extern int errno;
int enableLogging = 0;
struct variableNode *variableHeadNode; 
struct gateNode *gateHeadNode; 

typedef enum {INPUT,OUTPUT,AND,OR,NAND,NOR,XOR,NOT,PASS,DECODER,MULTIPLEXER} kind_t;

struct multiplexerInput{
    int type;
    int valueOrIndex;
};

struct gateNode{
    kind_t kind;
    int size; 
    int outputSize;
    int constantSize;
    int selectorSize;
    int *parameters;
    int *outputParameters;
    int *constantParameters;
    struct multiplexerInput *selectorParameters;
    struct multiplexerInput *multiplexerInputParameters;
    struct gateNode *next;
};

struct variableNode{
    int index; 
    char *name;
    struct variableNode *next; 
};

long twoPow(int power){
    return (1L << power);
}

void printVariableValuesList(int *variableValues, int variableLinkedListSize){
    printf("variable values are\n");
    for(int i = 0; i < variableLinkedListSize; i ++){
        printf("%d ", variableValues[i] );
    }
     printf("\n");
}

int checkIfCharacter(char *c){
    for(int i = 0; i < strlen(c); i ++){
        if(!isdigit(c[i])){
            return 1;
        }
    }
    return 0;
}

void printVariableLinkedList(){
    printf("Printing variable linked list\n");
    struct variableNode *current = variableHeadNode;
    while(current != NULL){
        printf("%d %s \n", current->index,current->name);
        current = current->next;
    }
    printf("\n");
    printf("\n------- \n");
}

void printGateLinkedList(){
    printf("Printing gate linked list\n");
    struct gateNode *current = gateHeadNode;
    while(current != NULL){
        printf("g--> %d %d ", current->kind,current->size);
        if(current->kind != MULTIPLEXER){
            for(int i = 0; i < current->size; i ++){
                printf("%d ", current->parameters[i]);
            }
            printf("\n");
            if(current->outputSize != 0){
                printf("o -->");
                for(int j = 0; j < current->outputSize; j ++){
                    printf("%d ",current->outputParameters[j]);
                }
                printf("\n");
            }
            if(current->constantSize != 0){
                printf("c--> ");
                for(int x = 0; x < current->constantSize; x ++){
                    printf("%d ",current->constantParameters[x]);
                }
            }
            printf("\n");
        }

        if(current->kind == MULTIPLEXER){
            printf("Multiplexer input values are \n");
            for(int i = 0; i < current->size; i ++){
                printf("t->%d vi->%d ", current->multiplexerInputParameters[i].type,current->multiplexerInputParameters[i].valueOrIndex);
            }
            printf("\nMultiplexer selector values are \n");
            for(int i = 0; i < current->selectorSize; i ++){
                printf("t->%d vi->%d ",current->selectorParameters[i].type,current->selectorParameters[i].valueOrIndex);
            }
            printf("\n");
        }
        current = current->next;
    }
    printf("\n------- \n");
}

void insertVariableNode(int index, char *name){
    if(enableLogging){
        printf("in insert function variable inserting %d %s \n",index,name);
    }
    struct variableNode *variableToBeInserted = malloc(sizeof(struct variableNode));
    variableToBeInserted->index = index;
    variableToBeInserted->name = (char *)malloc(sizeof(name)*sizeof(char));
    strcpy(variableToBeInserted->name,name);
    variableToBeInserted->next = NULL;
    struct variableNode *current = variableHeadNode;
    if(current != NULL){
        if(enableLogging){
            printf("variable head node data is %d %s \n", current->index,current->name);
        }
    }
    
    if(variableHeadNode == NULL){
        if(enableLogging){
            printf("inserted variable head \n");
        }
        variableHeadNode = variableToBeInserted;
        return;
    }

    while(current->next != NULL){
        current = current->next;
    }
    if(enableLogging){
        printf("inserted variable value \n");
    }
    current->next = variableToBeInserted;
}

void insertGateNode(kind_t kind, int size, int *parameters,int *outputParameters, int sizeOfOutputParameters, int *constantParameters, int sizeOfConstantParameters, struct multiplexerInput *selectorParameters, int selectorSize, struct multiplexerInput *multiplexerInputs){
    if(enableLogging){
        printf("in insert function gate inserting %d ",size);
        printf("OUTPUT LIST SIZE IS %d\n",sizeOfOutputParameters);
    }

    struct gateNode *gateToBeInserted = malloc(sizeof(struct gateNode));
    
    gateToBeInserted->kind = kind;
    gateToBeInserted->size = size;
    gateToBeInserted->outputSize = sizeOfOutputParameters;
    gateToBeInserted->parameters = parameters;
    gateToBeInserted->outputParameters = outputParameters;
    gateToBeInserted->constantSize = sizeOfConstantParameters;
    gateToBeInserted->constantParameters = constantParameters;
    gateToBeInserted->selectorSize = selectorSize;
    gateToBeInserted->selectorParameters = selectorParameters;
    gateToBeInserted->multiplexerInputParameters = multiplexerInputs;
    gateToBeInserted->next = NULL;
    struct gateNode *current = gateHeadNode;
    
    if(gateHeadNode == NULL){
        gateHeadNode = gateToBeInserted;
        return;
    }

    while(current->next != NULL){
        current = current->next;
    }
    if(enableLogging){
        printf("inserted gate value of kind %d and address %p\n", kind,gateToBeInserted);
    }
    current->next = gateToBeInserted;
}

struct variableNode* searchVariableNode(char *name){
    struct variableNode *current = variableHeadNode;
    while(current != NULL){
        if(current->name == name){
            return current; 
        }
        current = current->next;
    }
    return NULL;
}

int NOTGATE(int IN1){
    if(IN1 == 1){
        return 0;
    }
    else{
        return 1;
    }
}

int ANDGATE(int IN1, int IN2){
    if(IN1 == 1 && IN2 == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int ORGATE(int IN1, int IN2){
    if(IN1 == 1 || IN2 == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int NANDGATE(int IN1, int IN2){
    if(IN1 == 1 && IN2 == 1){
        return 0;
    }
    else{
        return 1;
    }
}

int NORGATE(int IN1, int IN2){
    if(IN1 == 0 && IN2 == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int XORGATE(int IN1, int IN2){
    if(IN1 == 1 && IN2 == 1){
        return 0;
    }
    else if(IN1 == 0 && IN2 == 0){
        return 0;
    }
    else{
        return 1;
    }
}

void DECODERGATE(struct gateNode *givenNode, int *variableList){
    int totalValueOfInput = 0;
    int *reversedValueList = malloc(givenNode->size *sizeof(int) );
    int reversedCounter = 0;

    if(enableLogging){
        printf("variable list values are \n");
        for(int i = 0; i < givenNode->size; i ++){
            printf("%d ",variableList[givenNode->parameters[i]]);
        }
        printf("\n");
    }

    for(int i = givenNode->size-1; i >= 0; i --){
        reversedValueList[reversedCounter] = variableList[givenNode->parameters[i]];
        reversedCounter ++;
    }

    if(enableLogging){    
        printf("reversed variable list values are \n");
        for(int i = 0; i < givenNode->size; i ++){
            printf("%d ",reversedValueList[i]);
        }
        printf("\n");
    }

        for(int i = 0; i < givenNode->size; i ++){
            if(reversedValueList[i] == 1){
                totalValueOfInput += twoPow(i);
                if(enableLogging){
                    printf("val of s is %d\n", totalValueOfInput);
                }
            }
        }

        if(enableLogging){
            printf("s is %d\n ",totalValueOfInput);
        }

    for(int i = 0; i < givenNode->outputSize; i ++){
        variableList[givenNode->outputParameters[i]] = 0;
    }

    variableList[givenNode->outputParameters[totalValueOfInput]] = 1;

    if(enableLogging){
        printVariableLinkedList();
        printVariableValuesList(variableList,25);
        printGateLinkedList();
    }
    free(reversedValueList);
}

void MULTIPLEXERGATE(struct gateNode *givenNode, int *variableList){
    int selectorValue = 0;
    int *reversedVariableList = malloc(givenNode->selectorSize * sizeof(int));
    int *copiedList = malloc(givenNode->selectorSize * sizeof(int));
    int reverseCount = 0;

    for(int i = 0; i < givenNode->selectorSize; i ++){
        if(givenNode->selectorParameters[i].type == 1){
            copiedList[i] = givenNode->selectorParameters[i].valueOrIndex;
        }
        else if(givenNode->selectorParameters[i].type == 0){
            copiedList[i] = variableList[givenNode->selectorParameters[i].valueOrIndex];
        }
    }

    for(int i = givenNode->selectorSize-1; i >=0; i --){
        reversedVariableList[reverseCount] = copiedList[i];
        reverseCount ++;
    }

    for(int i = 0; i < givenNode->selectorSize; i ++){
        if(reversedVariableList[i] == 1){
            selectorValue += twoPow(i);
        }
    }
    if(enableLogging){
        printf("s for multiplexer is %d", selectorValue);
    }

    if(givenNode->multiplexerInputParameters[selectorValue].type == 1){
        variableList[givenNode->outputParameters[0]] = givenNode->multiplexerInputParameters[selectorValue].valueOrIndex;
    }
    else{
        variableList[givenNode->outputParameters[0]] = variableList[givenNode->multiplexerInputParameters[selectorValue].valueOrIndex];
    }
    free(reversedVariableList);
    free(copiedList);
}

int PASSGATE(int IN1){
    return IN1;
}

int checkIfVariableExists(char *var){
    struct variableNode *current = variableHeadNode;
    while(current != NULL){
        if(strcmp(var,current->name) == 0){
            return current->index;
        }
        current = current->next;
    }
    return -1;
}

int createGateData(kind_t kind, int size, int outputSize,int *parameters, int *outputParameters,FILE *ptr, char buf[], int index, int *constantParameters, int sizeOfConstantParameters){
    int potentialIndex = 0;
    int constantIndex = 0;

    for(int i = 0; i < size; i ++){
        fscanf(ptr," %16s", buf);
        if(checkIfCharacter(buf) == 1){
            potentialIndex = checkIfVariableExists(buf);
            if(potentialIndex == -1){
                insertVariableNode(index,buf);
                parameters[i] = index;
                index ++;
            }
            else{
                parameters[i] = potentialIndex;
            }
        }
        else{
            int val = atoi(buf);
            constantParameters[constantIndex] = val;
            constantIndex ++;
        }
    }
    for(int i = 0; i < outputSize; i ++){
        fscanf(ptr," %16s", buf);
        potentialIndex = checkIfVariableExists(buf);
        if(potentialIndex == -1){
            insertVariableNode(index,buf);
            outputParameters[i] = index;
            index ++;
        }
        else{
            outputParameters[i] = potentialIndex;
        }
    }
    insertGateNode(kind,size,parameters,outputParameters,outputSize,constantParameters,constantIndex,NULL,0,NULL);
    return index;
}

int createGateDataMultiplexer(kind_t kind, int outputSize, int *outputParameters,FILE *ptr, char buf[], int index, struct multiplexerInput *selectorParameters, int selectorSize, struct multiplexerInput *multiplexerInputs, int sizeOfMultiplexerInputs){
    int potentialIndex = 0;

    for(int i = 0; i < sizeOfMultiplexerInputs; i ++){
        fscanf(ptr," %16s", buf);
        if(checkIfCharacter(buf) == 1){
            potentialIndex = checkIfVariableExists(buf);
            if(potentialIndex == -1){
                insertVariableNode(index,buf);
                multiplexerInputs[i].type = 0;
                multiplexerInputs[i].valueOrIndex = index;
                index ++;
            }
            else{
                multiplexerInputs[i].type = 0;
                multiplexerInputs[i].valueOrIndex = potentialIndex;
            }
        }
        else{
            int val = atoi(buf);
            multiplexerInputs[i].type = 1;
            multiplexerInputs[i].valueOrIndex = val;
        }
    }

    for(int i = 0; i < selectorSize; i ++){
        fscanf(ptr," %16s", buf);
        if(checkIfCharacter(buf) == 1){
            potentialIndex = checkIfVariableExists(buf);
            if(potentialIndex == -1){
                insertVariableNode(index,buf);
                selectorParameters[i].type = 0;
                selectorParameters[i].valueOrIndex = index;
                index ++;
            }
            else{
                selectorParameters[i].type = 0;
                selectorParameters[i].valueOrIndex = potentialIndex;
            }
        }
        else{
            int val = atoi(buf);
            selectorParameters[i].type = 1;
            selectorParameters[i].valueOrIndex = val;
        }
    }

    for(int i = 0; i < outputSize; i ++){
        fscanf(ptr," %16s", buf);
        potentialIndex = checkIfVariableExists(buf);
        if(potentialIndex == -1){
            insertVariableNode(index,buf);
            outputParameters[i] = index;
            index ++;
        }
        else{
            outputParameters[i] = potentialIndex;
        }
    }
    insertGateNode(MULTIPLEXER,sizeOfMultiplexerInputs,NULL,outputParameters,1,NULL,0,selectorParameters,selectorSize,multiplexerInputs);
    return index;
}

int createInputOutputGateDate(kind_t kind, FILE *ptr, char buf[], int index){
    fscanf(ptr, " %s", buf);
    int size = atoi(buf);
    int *parameters = malloc(size * sizeof(int));
    for(int i = 0; i < size; i ++){
        fscanf(ptr," %16s", buf);
        insertVariableNode(index,buf);
        parameters[i] = index;
        index ++;
    }
    insertGateNode(kind,size,parameters,NULL,0,NULL,0,NULL,0,NULL);
    return index;
}

int invokeGateFunctionWithTwoParams(kind_t kind, int param1, int param2){
    int returnVal = 0;
    switch(kind){
        case NAND:
            returnVal = NANDGATE(param1,param2);
            break;
        case AND:
            returnVal = ANDGATE(param1,param2);
            break;
        case OR:
            returnVal = ORGATE(param1,param2);
            break;
        case XOR:
            returnVal = XORGATE(param1,param2);
            break;
        case NOR:
            returnVal = NORGATE(param1,param2);
            break;
        default:
            printf("invalid gate type\n");
            exit(0);
            break;
    }
    return returnVal;
}

void runCicuitWithTwoParams(kind_t kind, struct gateNode *currentGate, int *variableValues){
    if(currentGate->constantSize == 0){
        variableValues[currentGate->outputParameters[0]] = invokeGateFunctionWithTwoParams(kind,variableValues[currentGate->parameters[0]],variableValues[currentGate->parameters[1]]);
    }
    else{
        if(currentGate->constantSize == 1){
            variableValues[currentGate->outputParameters[0]] = invokeGateFunctionWithTwoParams(kind,variableValues[currentGate->parameters[0]],currentGate->constantParameters[0]);
        }
        else{
            variableValues[currentGate->outputParameters[0]] = invokeGateFunctionWithTwoParams(kind,currentGate->constantParameters[0],currentGate->constantParameters[1]);
        }
    }
}

void runCircuit(int *permutations, int permutationSize){
    int variableLinkedListSize = 0;
    struct variableNode *current = variableHeadNode;
    struct gateNode *outputNode;
    while(current != NULL){
        variableLinkedListSize ++;
        current = current->next;
    }
    int *variableValues = malloc(variableLinkedListSize * sizeof(int));

    struct gateNode *currentGate = gateHeadNode;

    while(currentGate != NULL){
        if(currentGate->kind == INPUT){
            for(int i = 0; i < permutationSize; i ++){
                variableValues[i] = permutations[i];
            }
        }
        else if(currentGate->kind == NOT){
            if(currentGate->constantSize == 0){
                variableValues[currentGate->outputParameters[0]] = NOTGATE(variableValues[currentGate->parameters[0]]);
            }
            else{
                if(currentGate->constantSize == 1){
                    variableValues[currentGate->outputParameters[0]] = NOTGATE(currentGate->constantParameters[0]);
                }
            }
        }
        else if(currentGate->kind == AND){
            runCicuitWithTwoParams(AND,currentGate,variableValues);
        }
        else if(currentGate->kind == OR){
            runCicuitWithTwoParams(OR,currentGate,variableValues);
        }
        else if(currentGate->kind == NAND){
            runCicuitWithTwoParams(NAND,currentGate,variableValues);
        }
        else if(currentGate->kind == NOR){
            runCicuitWithTwoParams(NOR,currentGate,variableValues);
        }
        else if(currentGate->kind == XOR){
            runCicuitWithTwoParams(XOR,currentGate,variableValues);
        }
        else if(currentGate->kind == DECODER){
            DECODERGATE(currentGate,variableValues);
        }
        else if(currentGate->kind == MULTIPLEXER){
            MULTIPLEXERGATE(currentGate,variableValues);
        }
        else if(currentGate->kind == PASS){
            if(currentGate->constantSize == 0){
                variableValues[currentGate->outputParameters[0]] = PASSGATE(variableValues[currentGate->parameters[0]]);
            }
            else{
                 if(currentGate->constantSize == 1){
                    variableValues[currentGate->outputParameters[0]] = PASSGATE(currentGate->constantParameters[0]);
                }
            }
        }
        else if (currentGate->kind == OUTPUT)
        {
            outputNode = currentGate;
        }
        currentGate = currentGate->next;
    }

    for(int i = 0; i < permutationSize; i ++){
        printf("%d ",permutations[i]);
    }
    printf("| ");
    if(enableLogging){
        printf("\nVariable list values are: \n");
        for(int i = 0; i < variableLinkedListSize; i ++){
            printf("%d ",variableValues[i]);
        }
        printf("\n");
    }
    if(enableLogging){
        printf("output size is %d ", outputNode->size);
        printf("\nprinting the output node values\n");
        printf("output val is %d\n", outputNode->parameters[0]);
        for(int i = 0; i < outputNode->size; i ++){
            printf("%d ",outputNode->parameters[i]);
        }
    }
    for(int i = 0; i < outputNode->size; i ++){
        if(i == outputNode->size-1){
            printf("%d",variableValues[outputNode->parameters[i]]);
        }
        else{
            printf("%d ",variableValues[outputNode->parameters[i]]);
        }
    }
    printf("\n");

    if(enableLogging){
        printf("\n printing output node parameters \n");
        for(int i = 0; i < outputNode->size; i ++){
            printf("%d ", outputNode->parameters[i]);
        }    
        printf("\n variable values are \n");

        for(int i = 0; i < variableLinkedListSize; i ++){
            printf("%d ",variableValues[i]);
        }
        printf("\n");
    }

    free(variableValues);
}

void freeGateNode(struct gateNode *givenGateNode){
    if(givenGateNode->parameters != NULL)
        free(givenGateNode->parameters);
    if(givenGateNode->outputParameters != NULL)
        free(givenGateNode->outputParameters);
    if(givenGateNode->constantParameters != NULL)
        free(givenGateNode->constantParameters);
    if(givenGateNode->multiplexerInputParameters != NULL)
        free(givenGateNode->multiplexerInputParameters);
    if(givenGateNode->selectorParameters != NULL)
        free(givenGateNode->selectorParameters);
}

void freeGateLinkedList(){
    struct gateNode *current = gateHeadNode;
    while(current != NULL){
        struct gateNode *temp = current; 
        freeGateNode(current);
        current = current->next;
        free(temp);
    }
}

void freeVariableNode(struct variableNode *givenVariableNode){
    free(givenVariableNode->name);
}

void freeVariableLinkedList(){
    struct variableNode *current = variableHeadNode;
    while(current != NULL){
        struct variableNode *temp = current;
        freeVariableNode(current);
        current = current->next;
        free(temp);
    }
}

int main(int argc, char **argv){
    FILE *ptr = fopen(argv[1],"r");
    char buf[100];
    int index = 0;
    
    if(ptr == NULL){
        printf("file does not exist");
        return 0;
    }
    while(fscanf(ptr," %16s",buf) != EOF){
        if(enableLogging){
            printf("reading line \n");
            printf("buf is %s ",buf);
        }
        if(strcmp(buf,"INPUT") == 0){
            if(enableLogging){
                printf("in INPUT gate file read\n");
            }
            index = createInputOutputGateDate(INPUT,ptr,buf,index);
        }
        else if(strcmp(buf,"OUTPUT") == 0){
            if(enableLogging){
                printf("in OUTPUT gate file read\n");
            }
            index = createInputOutputGateDate(OUTPUT,ptr,buf,index);
        }
        else if(strcmp(buf,"AND") == 0){
            if(enableLogging){
                printf("in AND gate file read\n");
            }
            int *parameters = malloc(2 * sizeof(int));
            int *outputParameters = malloc(1 * sizeof(int));
            int *constantParameters = malloc(2 * sizeof(int));
            index = createGateData(AND,2,1,parameters,outputParameters,ptr,buf,index,constantParameters,2);
        }
        else if(strcmp(buf,"OR") == 0){
            if(enableLogging){
                printf("in OR gate file read\n");
            }
            int *parameters = malloc(2 * sizeof(int));
            int *outputParameters = malloc(1 * sizeof(int));
            int *constantParameters = malloc(2 * sizeof(int));
            index = createGateData(OR,2,1,parameters,outputParameters,ptr,buf,index,constantParameters,2);
        }
        else if(strcmp(buf,"NAND") == 0){
            if(enableLogging){
                printf("in NAND gate file read\n");
            }
            int *parameters = malloc(2 * sizeof(int));
            int *outputParameters = malloc(1 * sizeof(int));
            int *constantParameters = malloc(2 * sizeof(int));
            index = createGateData(NAND,2,1,parameters,outputParameters,ptr,buf,index,constantParameters,2);
        }
        else if(strcmp(buf,"NOR") == 0){
            if(enableLogging){
                printf("in NOR gate file read\n");
            }
            int *parameters = malloc(2 * sizeof(int));
            int *outputParameters = malloc(1 * sizeof(int));
            int *constantParameters = malloc(2 * sizeof(int));
            index = createGateData(NOR,2,1,parameters,outputParameters,ptr,buf,index,constantParameters,2);
        }
        else if(strcmp(buf,"XOR") == 0){
            if(enableLogging){
                printf("in XOR gate file read\n");
            }
            int *parameters = malloc(2 * sizeof(int));
            int *outputParameters = malloc(1 * sizeof(int));
            int *constantParameters = malloc(2 * sizeof(int));
            index = createGateData(XOR,2,1,parameters,outputParameters,ptr,buf,index,constantParameters,2);
        }
        else if(strcmp(buf,"NOT") == 0){
            if(enableLogging){
                printf("in AND gate file read\n");
            }
            int *parameters = malloc(1 * sizeof(int));
            int *outputParameters = malloc(1 * sizeof(int));
            int *constantParameters = malloc(1 * sizeof(int));
            index = createGateData(NOT,1,1,parameters,outputParameters,ptr,buf,index,constantParameters,1);
        }
        else if(strcmp(buf,"PASS") == 0){
            if(enableLogging){
                printf("in PASS gate file read\n");
            }
            int *parameters = malloc(1 * sizeof(int));
            int *outputParameters = malloc(1 * sizeof(int));
            int *constantParameters = malloc(1 * sizeof(int));
            index = createGateData(PASS,1,1,parameters,outputParameters,ptr,buf,index,constantParameters,1);
        }
        else if(strcmp(buf,"DECODER") == 0){
            if(enableLogging){
                printf("in DECODER gate file read\n");
            }
            fscanf(ptr," %16s", buf);   
            int inputSize = atoi(buf);
            int *parameters = malloc(inputSize * sizeof(int));
            int outputSize = twoPow(inputSize);
            int *outputParameters = malloc(outputSize * sizeof(int));
            int *constantParameters = malloc(inputSize * sizeof(int));
            index = createGateData(DECODER,inputSize,outputSize,parameters,outputParameters,ptr,buf,index,constantParameters,inputSize);
        }
        else if(strcmp(buf,"MULTIPLEXER") == 0){
            if(enableLogging){
                printf("in MULTIPLEXER gate file read\n");
            }
            fscanf(ptr," %16s", buf);  
            int selectorSize = atoi(buf);
            int parametersSize = twoPow(selectorSize);

            struct multiplexerInput *multiplexerInputs = malloc(parametersSize*sizeof(struct multiplexerInput));
            struct multiplexerInput *selectorParameters = malloc(selectorSize*sizeof(struct multiplexerInput));

            int *outputParameters = malloc(1 * sizeof(int));

            index = createGateDataMultiplexer(MULTIPLEXER,1,outputParameters,ptr,buf,index,selectorParameters,selectorSize,multiplexerInputs,parametersSize);
        }
        if(enableLogging){
            printf("%s ",buf);
        }
    }
    
    if(enableLogging){
        printGateLinkedList();
        printf("\n");
        printVariableLinkedList();
        printf("\n");
    }

    struct gateNode *inputNode = gateHeadNode;
    int n = inputNode->size;
    int *permutations = malloc(n * sizeof(int));
    int *tempArray = malloc(n * sizeof(int));
    unsigned long long numberOfPermutations = 1ULL << n;
    for(unsigned long long i = 0; i < numberOfPermutations; i ++){
        for(unsigned int j = 0; j < n; j ++){
            int val = (i >> j)&1;
            tempArray[j] = val;
            if(enableLogging){
                printf("\n%d\n ", val);
            }
        }
        int tempLooper = n-1;
        for(int i = 0; i < n; i ++){
            permutations[i] = tempArray[tempLooper];
            tempLooper --;
        }
        runCircuit(permutations,n);
    }

    free(tempArray);
    free(permutations);
    freeVariableLinkedList();
    freeGateLinkedList();
}
