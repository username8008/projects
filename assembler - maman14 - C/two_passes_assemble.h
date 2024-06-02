#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define COMANDS_AMOUNT 16
#define REGISTERS_AMOUNT 8
#define READ_ERROR -1
#define WRITE_ERROR -2
#define LINE_IS_TOO_LONG -3
#define STOP_ERROR -4


#define writeDefineInSymbolTable /* if its a define token (step 3) */\
{\
    {\
        /* enter values to the symbols table (step 4) */\
        token = strtok(NULL, "=");\
        for(i=strlen(token); i>0; i--)\
        {\
            if(token[i]==' ')\
                token[i]='\0';\
        }\
        \
        /* if symbol already exist in the table */\
        for(i=0; i<symbolAmount; i++)\
        {\
            if(!strcmp(symTable[i].name, token))\
            {\
                fprintf(stderr, "error! in line: %d, symbol %s already exist.\n",currLine, token); /* error */\
                errorFlag=1;\
            }\
        }\
        \
        strcpy(symTable[currSymbol].name, token);\
        strcpy(symTable[currSymbol].type, "mdefine");\
        i=0;\
        while (token != NULL)\
        {\
            token = strtok(NULL, "\n");\
            if(token!=NULL)\
            {\
                symTable[currSymbol].value[i]= atoi(token);\
                i++;\
                symTable[currSymbol].value[i]=0; /* initalizing the next one */\
                symbolAmount++;\
                currSymbol++;\
                reallocSymTable\
            }\
        }\
    }\
}

#define reallocSymTable \
{\
    symbolTablePointer temp;\
    temp= (symbolTablePointer) realloc(symTable, (sizeof(symTable->name)*sizeof(symTable->type)*sizeof(symTable->value)*symbolAmount));\
    if(!temp)\
    {\
        fprintf(stderr, "malloc error\n");\
        exit(0);\
    }\
    symTable= temp;\
}

#define tokenIsASymbolDefinition token[strlen(token)-1]==':'
#define tokenIsData (!strcmp(token, ".data") || !strcmp(token, ".string"))

#define writeInSymbolTable \
{\
    /* enter values to the symbols table (step 4) */\
                            \
    if(tokenIsData)\
    {\
        for(i=0; i<symbolAmount; i++)\
        {\
            if(!strcmp(symTable[i].name, name))\
            {\
                fprintf(stderr, "error! in line: %d, symbol %s already exist.\n",currLine, name); /* error */\
                errorFlag=1;\
            }\
        }\
                        \
        strcpy(symTable[currSymbol].name, name);\
        strcpy(symTable[currSymbol].type, "data");\
        symTable[currSymbol].value[0]= DC;\
        symbolAmount++;\
        currSymbol++;\
        reallocSymTable\
    }\
    else if(!strcmp(token, ".extern"))\
    {\
        token = strtok(NULL, ", \t \n");\
        while(token!=NULL)\
        {\
            for(i=0; i<symbolAmount; i++)\
            {\
                if(!strcmp(symTable[i].name, token))\
                {\
                    fprintf(stderr, "error! in line: %d, symbol %s already exist.\n",currLine, token); /* error */\
                    errorFlag=1;\
                }\
            }\
                            \
            strcpy(symTable[currSymbol].name, token);\
            strcpy(symTable[currSymbol].type, "external");\
            symTable[currSymbol].value[0]= 0;\
            token = strtok(NULL, ", \t \n");\
            symbolAmount++;\
            currSymbol++;\
            reallocSymTable\
        }\
    }\
    else if(!strcmp(token, ".entry"))\
    {\
        while(token!=NULL) /* entry */\
                    token = strtok(NULL, "\t  \n");\
    }\
    else\
    {\
        strcpy(symTable[currSymbol].name, name);\
        strcpy(symTable[currSymbol].type, "code");\
        symTable[currSymbol].value[0]= (IC+100);\
        symbolAmount++;\
        currSymbol++;\
        reallocSymTable\
    }\
\
}

#define writeDataInTable \
{\
    token = strtok(NULL, ",\n");\
    for(k=0; k<strlen(token); k++)\
    {\
        if(token[k]==',')\
        {\
            token[k]='\0';\
        }\
    }\
    if(isdigit(token[0]))\
    {\
        i=0;\
        strcpy(dataTable[currData].name, symTable[currSymbol-1].name);\
        while (token != NULL)\
        {\
                if(token!=NULL)\
                {\
                    for(j=0; j<symbolAmount; j++) /* checking if its a symbol value */\
                    {\
                        if(!strcmp(symTable[j].name, token))\
                        {\
                            dataTable[currData].value[i]= symTable[j].value[0];\
                            break;\
                        }\
                    }\
                    if(j==symbolAmount) /* symbol not found */\
                        dataTable[currData].value[i]= atoi(token);\
\
                    DC++;\
                    i++;\
                }\
            token = strtok(NULL, ", \n");\
        }\
        currData++;\
        reallocDataTable\
    }\
\
    else\
    {\
        i=0;\
        strcpy(stringDataTable[currStringData].name, symTable[currSymbol-1].name);\
        while (token[i] != '\0')\
        {\
            if(token[i]!='\0')\
            {\
                if(token[i]!='\"')\
                {\
                    stringDataTable[currStringData].value[i]= token[i];\
                    DC++;\
                }\
                i++;\
            }\
        }\
        stringDataTable[currStringData].value[i]= '\0';\
        DC++;\
        currStringData++;\
        reallocStringDataTable\
    }\
}


#define checkForCommand \
{\
        indexOn1=0;\
        regOn1=0;\
        indexOn2=0;\
        regOn2=0;\
    strcpy(currCommand, token);\
    \
                L=0;\
                commandValue.a= 0;\
                commandValue.opcode= 0;\
                commandValue.operand1= 0;\
                commandValue.operand2= 0;\
                commandValue.ARE= 0;\
\
                commandValueOperand1= -1;\
                commandValueOperand2= -1;\
\
                opperandValue1.operand= 0;\
                opperandValue1.ARE= 0;\
\
                opperandValue2.operand= 0;\
                opperandValue2.ARE= 0;\
\
                opperandValue3.operand= 0;\
                opperandValue3.ARE= 0;\
\
                opperandValue4.operand= 0;\
                opperandValue4.ARE= 0;\
\
                for(i=0; i<COMANDS_AMOUNT; i++) /* searching command.. */\
                {\
                    if(!strcmp(commands[i], token))\
                    {\
                        commandValue.opcode= i;\
                        L++;\
                        i--; /* in case its the last command */\
                        break;\
                    }\
                }\
\
                    if(i==COMANDS_AMOUNT) /* command not found.. */\
                    {\
                        fprintf(stderr, "error! in line: %d, the command %s does not exist..\n",currLine, token); /* error */\
                        errorFlag=1;\
                        while(token!=NULL)\
                            token = strtok(NULL, "\t , \n");\
                    }\
\
                    else\
                    {\
                        /* first opperand */\
                        token = strtok(NULL, ", \t\n");\
                        if(token!=NULL)\
                        {\
                            if (token[0]=='#') /* if(token is immediate) */\
                            {\
                                commandValue.operand1= 0;\
                                opperandValue1.operand= tokenIsImmediate(token, symTable);\
                            }\
                        \
                            else if((i=tokenIsIndex(token, symTable))!=-1)\
                            {\
                                commandValue.operand1= 2;\
                                opperandValue1.operand=0 ;\
                                opperandValue2.operand=i ;\
                                strcpy(sym1, token);\
                                \
                                L++;\
                                indexOn1=1;\
                            }\
                \
                            else if((i=tokenIsRegister(token))!=-1)\
                            {\
                                commandValue.operand1= 3;\
                                reg1.operand1= i;\
                                regOn1=1;\
                            }\
                \
                            else /* if(token is direct) */\
                            {\
                                commandValue.operand1= 1;\
                                strcpy(sym1, token);\
                            }\
                \
                            L++;\
                            commandValueOperand1= commandValue.operand1;\
                \
                            /* second opperand */\
                            token = strtok(NULL, ", \t\n");\
                            if(token!=NULL)\
                            {\
                                if (token[0]=='#') /* if(token is immediate) */\
                                {\
                                commandValue.operand2= 0;\
                                opperandValue3.operand= tokenIsImmediate(token, symTable);\
                                }\
                    \
                                else if((i=tokenIsIndex(token, symTable))!=-1)\
                                {\
                                    commandValue.operand2= 2;\
                                    opperandValue3.operand=0 ;\
                                    opperandValue4.operand=i ;\
                                    strcpy(sym2, token);\
                                    \
                                    L++;\
                                    indexOn2=1;\
                                }\
                    \
                                else if((i=tokenIsRegister(token))!=-1)\
                                {\
                                    commandValue.operand2= 3;\
                                    reg2.operand2= i;\
                                    regOn2=1;\
                                }\
                    \
                                else /* if(token is direct) */\
                                {\
                                    commandValue.operand2= 1;\
                                    strcpy(sym2, token);\
                                }\
                                        \
                                        \
                                if((commandValue.operand1== 3) && (commandValue.operand2== 3)) /* if the two opperands are registers */\
                                {\
                                    L--;\
                                    reg1.operand2= reg2.operand2;\
                                    reg2.operand2=0;\
                                }\
\
                                L++;\
\
                                commandValueOperand2= commandValue.operand2;\
                            }\
                            else\
                            {\
                                commandValue.operand2= commandValue.operand1;\
                                commandValue.operand1=0;\
\
                                commandValueOperand2= commandValueOperand1;\
                                commandValueOperand1= -1;\
\
                \
                                reg1.operand2= reg1.operand1;\
                                reg1.operand1=0;\
                            }\
                        }\
\
                        commandValue.ARE= 0;\
                        while(token!=NULL)\
                            token = strtok(NULL, "\t \n");\
\
                        for(i=0; i<COMANDS_AMOUNT; i++) /* checking if the command is valid (operands amount) */\
                        {\
                            if(!strcmp(commands[i], currCommand))\
                            {\
                                switch(i)\
                                {\
                                    /* case mov||cmp||add||sub, they can have it all so not need to check.. */\
                                    \
                                    case not:\
                                    case clr:\
                                    case inc:\
                                    case dec:\
                                    case jmp:\
                                    case bne:\
                                    case red:\
                                    case prn:\
                                    case jsr:\
                                    case rts:\
                                    case hlt:\
                                        if(commandValueOperand1!=-1)\
                                        {\
                                            fprintf(stderr, "error! in line: %d, bad operand for command: %s\n", currLine, currCommand); /* error */\
                                            errorFlag=1;\
                                        }\
                                        break;\
\
                                    case lea:\
                                        if(commandValueOperand1==0 || commandValueOperand1==3)\
                                            {\
                                            fprintf(stderr, "error! in line: %d, bad operand for command: %s\n", currLine, currCommand); /* error */\
                                            errorFlag=1;\
                                            }\
                                        break;\
                                    \
                                }\
                                switch(i)\
                                {\
                                    /* case cmp||prn, they can have it all so not need to check.. */\
                                    \
                                    case rts:\
                                    case hlt:\
                                        if(commandValueOperand2!=-1)\
                                            {\
                                            fprintf(stderr, "error! in line: %d, bad operand for command: %s\n", currLine, currCommand); /* error */\
                                            errorFlag=1;\
                                            }\
                                        break;\
\
                                    case mov:\
                                    case add:\
                                    case sub:\
                                    case lea:\
                                    case not:\
                                    case clr:\
                                    case inc:\
                                    case dec:\
                                    case red:\
                                        if(commandValueOperand2==0)\
                                            {\
                                            fprintf(stderr, "error! in line: %d, bad operand for command: %s\n", currLine, currCommand); /* error */\
                                            errorFlag=1;\
                                            }\
                                        break;\
                                    case jsr:\
                                    case bne:\
                                    case jmp:\
                                        if(commandValueOperand2==0 || commandValueOperand2==2)\
                                            {\
                                            fprintf(stderr, "error! in line: %d, bad operand for command: %s\n", currLine, currCommand); /* error */\
                                            errorFlag=1;\
                                            }\
                                        break;\
                                    \
                                }\
                            }\
                        }\
                        if(commandValueOperand1==-1)\
                        {\
                            commandValue.operand1=0;\
                        }\
                        if(commandValueOperand2==-1)\
                        {\
                            commandValue.operand2=0;\
                        }\
                    }\
}

#define writeInBinTable \
{\
    binTable[currInBinTable].adress= IC+100;\
    memcpy(binTable[currInBinTable].source, buff, LINE_SIZE);\
    p =(unsigned int*)&commandValue;\
    binTable[currInBinTable].bin= *p;\
\
    currInBinTable+=L;\
    reallocBinTable\
    /* enter data to the new ones */\
    for(i=currInBinTable-L+1, j=1; i<currInBinTable; i++, j++)\
    {\
        binTable[i].source[0]='\0';\
        if(regOn1 || indexOn1 || (*(p = (unsigned int*)&opperandValue1) != 0) || commandValue.operand1==1)\
        {\
                if(*(p =(unsigned int*)&reg1)>=0 && regOn1>0)\
            {\
                binTable[i].adress= IC+100+j;\
                binTable[i].bin=*p;\
                reg1.operand1=0;\
                reg1.operand2=0;\
                reg1.ARE=0;\
                regOn1=0;\
            }\
            else if(*(p =(unsigned int*)&opperandValue2)>=0 && indexOn1>0) /* this is the index of a symbol */\
            {\
                binTable[temp=i++].adress= 0;\
                strcpy(binTable[temp].source, sym1);\
                binTable[i].adress= (IC+100+(++j));\
                binTable[i].bin=*p;\
                opperandValue2.operand=0;\
                opperandValue2.operand=0;\
                opperandValue2.ARE=0;\
                indexOn1=0;\
            }\
            else if(*(p =(unsigned int*)&opperandValue1)!=0)\
            {\
                binTable[i].adress= IC+100+j;\
                binTable[i].bin=*p;\
                opperandValue1.operand=0;\
                opperandValue1.operand=0;\
                opperandValue1.ARE=0;\
            }\
            else\
            {\
                binTable[i].adress= 0;\
                binTable[i].bin=0 ;\
                strcpy(binTable[i].source, sym1);\
                commandValue.operand1=0;\
            }\
        }\
        else if(regOn2 || indexOn2 || (*(p = (unsigned int*)&opperandValue3) != 0) || commandValue.operand2==1)\
        {\
            if(*(p =(unsigned int*)&reg2)>=0 && regOn2>0)\
            {\
                binTable[i].adress= IC+100+j;\
                binTable[i].bin=*p;\
                reg2.operand1=0;\
                reg2.operand2=0;\
                reg2.ARE=0;\
                regOn2=0;\
            }\
            else if(*(p =(unsigned int*)&opperandValue4)>=0 && indexOn2>0) /* this is the index of a symbol */\
            {\
                binTable[temp=i++].adress= 0;\
                strcpy(binTable[temp].source, sym2);\
                binTable[i].adress= (IC+100+(++j));\
                binTable[i].bin=*p;\
                opperandValue4.operand=0;\
                opperandValue4.operand=0;\
                opperandValue4.ARE=0;\
                indexOn2=0;\
            }\
            else if(*(p =(unsigned int*)&opperandValue3)!=0)\
            {\
                binTable[i].adress= IC+100+j;\
                binTable[i].bin=*p;\
                opperandValue3.operand=0;\
                opperandValue3.operand=0;\
                opperandValue3.ARE=0;\
            }\
            else\
            {\
                binTable[i].adress= 0;\
                binTable[i].bin=0 ;\
                strcpy(binTable[i].source, sym1);\
                commandValue.operand2=0;\
            }\
        }\
    }\
}

#define updateDataInSymbolTable \
{\
    for(i=0; i<symbolAmount; i++)\
    {\
        if(!strcmp(symTable[i].type, "data"))\
            symTable[i].value[0]+= (IC+100);\
            \
    }\
}

#define addDataToBinTable \
{\
        for(i=0; i<symbolAmount; i++)\
    {\
        if(!strcmp(symTable[i].type, "data"))\
        {\
            dataOpperand1.operand= 0;\
            binTable[currInBinTable].adress= symTable[i].value[0];\
            memcpy(binTable[currInBinTable].source, symTable[i].name, LINE_SIZE);\
            for(j=0; j<symbolAmount; j++)\
            {\
                if(!strcmp(symTable[i].name, dataTable[j].name))\
                {\
                    for(k=0; dataTable[j].value[k]!='\0'; k++)\
                    {\
                        if(k>0)\
                            strcpy(binTable[currInBinTable].source, "\0");\
                        dataOpperand1.operand=dataTable[j].value[k];\
                        p =(unsigned int*)&dataOpperand1;\
                        binTable[currInBinTable].adress= currInBinTable+100;\
                        binTable[currInBinTable++].bin= *p;\
                        reallocBinTable\
                    }\
                }\
                else if(!strcmp(symTable[i].name, stringDataTable[j].name))\
                {\
                    for(k=1; stringDataTable[j].value[k]!='\0'; k++)\
                    {\
                        if(k>1)\
                            strcpy(binTable[currInBinTable].source, "\0");\
                        dataOpperand1.operand=stringDataTable[j].value[k];\
                        p =(unsigned int*)&dataOpperand1;\
                        binTable[currInBinTable].adress= currInBinTable+100;\
                        binTable[currInBinTable++].bin= *p;\
                        reallocBinTable\
                    }\
                    strcpy(binTable[currInBinTable].source, "\0");\
                    dataOpperand1.operand=0;\
                    p =(unsigned int*)&dataOpperand1;\
                    binTable[currInBinTable].adress= currInBinTable+100;\
                    binTable[currInBinTable++].bin= *p;\
                    reallocBinTable\
                }\
            }\
        }\
    }\
}

#define newLineCheck \
{\
    charCount=0;\
            i=0;\
            while(token[i]!='\0' && token[i]!='\n')\
            {\
                if(token[i]!=' ' && token[i]!='\t')\
                    charCount++;\
                i++;\
            }\
            if(charCount==0)\
            {\
                token = strtok(NULL, " ");\
                break;\
            }\
}

#define cleanBuff \
{\
    i=0;\
    while(currLine<=totalLinesCount && buff[i]!='\n')\
    {\
        for(i=0; i<LINE_SIZE; i++) /* cleaning the line */\
        {\
            buff2[i]='\0';\
        }\
        len =strlen(buff);\
        i=0;\
\
        while(buff[i]==' ' || buff[i]=='\t')\
        {\
            i++;\
        }\
\
        if(buff[i]==';' || buff[i]=='\n')\
        {\
            for(i=0; i<LINE_SIZE; i++) /* cleaning the line */\
            {\
                buff[i]='\0';\
                buff2[i]='\0';\
            }\
            fgets(buff, LINE_SIZE, fp); /* continue to another line */\
            currLine++;\
        }\
\
        else\
        {\
            /* iterate through each character */\
            for (i = 0, j=0; i < len; i++)\
            {\
                if(buff[i]=='\n')\
                {\
                    break;\
                }\
                if ((buff[i] != ' ' && buff[i] != '\t') || (i > 0 && (buff[i - 1] != ' ' && buff[i - 1] != '\t')))\
                    /* copy the character to the new position */\
                    buff2[j++] = buff[i];\
            }\
        }\
    }\
}

#define updateEntries \
{\
    rewind(fp);\
    currLine= 0;\
    lineCount= totalLinesCount;\
    \
    while(lineCount--) /* reads until end of file (step 2) */\
    {\
        for(i=0; i<LINE_SIZE; i++) /* cleaning the line */\
            buff[i]='\0';\
        fgets(buff, LINE_SIZE, fp); /* read a line */\
        currLine++;\
        cleanBuff /* clean the buffer */\
        token = strtok(buff2, " \t");\
        while (token != NULL) /* taking word per word to check */\
        {\
            if(!strcmp(token, ".entry")) /* if its an entry instruction */\
            {\
                token = strtok(NULL, ", \t \n");\
                while(token!=NULL)\
                {\
                    for(i=0; i<symbolAmount; i++)\
                    {\
                        if(!strcmp(symTable[i].name, token))\
                        {\
                            strcpy(symTable[i].type, "entry");\
                            break;\
                        }\
                    }\
                    if(i==symbolAmount)\
                    {\
                        fprintf(stderr, "error! in line: %d, symbol %s undefined.\n",currLine, token); /* error */\
                        errorFlag=1;\
                    }\
                    token = strtok(NULL, ", \t \n");\
                }\
            }\
            token = strtok(NULL, ", \t \n");\
        }\
    }\
}


#define reallocDataTable \
{\
    dataTablePointer temp;\
    temp= (dataTablePointer) realloc(dataTable, (sizeof(dataTable->name)*sizeof(dataTable->value)*(currData+1)));\
    if(!temp)\
    {\
        fprintf(stderr, "malloc error\n");\
        exit(0);\
    }\
    dataTable= temp;\
}

#define reallocStringDataTable \
{\
    stringDataTablePointer temp;\
    temp= (stringDataTablePointer) realloc(stringDataTable, (sizeof(stringDataTable->name)*sizeof(stringDataTable->value)*(currStringData+1)));\
    if(!temp)\
    {\
        fprintf(stderr, "malloc error\n");\
        exit(0);\
    }\
    stringDataTable= temp;\
}

#define reallocBinTable \
{\
    binaryTablePointer temp;\
    temp= (binaryTablePointer) realloc(binTable, (sizeof(binTable->adress)*sizeof(binTable->source)*sizeof(binTable->bin)*(currInBinTable+1))); /* realloc acording to the amount of "words" (L) */\
    if(!temp)\
    {\
        fprintf(stderr, "malloc error\n");\
        exit(0);\
    }\
    binTable= temp;\
}\

#define cleanTabels \
{\
    /* cleaning tables */\
    for (i = 0; i < symbolAmount; i++)\
    {\
        if(symTable!=NULL)\
        {\
            memset(symTable[i].name, '\0', LINE_SIZE);\
            memset(symTable[i].type, '\0', SYMBOL_TYPE_SIZE);\
            for (j = 0; j < LINE_SIZE; j++)\
                symTable[i].value[j] = 0;\
        }\
    }\
\
    for (i = 0; i <= currData; i++)\
    {\
        if(dataTable!=NULL)\
        {\
            memset(dataTable[i].name, '\0', LINE_SIZE);\
            for (j = 0; j < (LINE_SIZE*sizeof(int)); j++)\
                dataTable[i].value[j] = 0;\
        }\
    }\
\
    for (i = 0; i <= currStringData; i++)\
    {\
        if(stringDataTable!=NULL)\
        {\
            memset(stringDataTable[i].name, '\0', LINE_SIZE);\
            memset(stringDataTable[i].value, '\0', LINE_SIZE);\
        }\
    }\
\
    for (i = 0; i < totalBinTableAmount; i++)\
    {\
        if(binTable!=NULL)\
        {\
            binTable[i].adress = 0;\
            binTable[i].bin = 0;\
            memset(binTable[i].source, '\0', LINE_SIZE);\
        }\
    }\
}

#define checkCommas \
{\
        j=0;\
        commaCounter=0;\
        for (i=0; i<LINE_SIZE && buff[i]!='\n'; i++) /* comma check */\
        {\
            if (buff[i]==',')\
            {\
                commaCounter++;\
                j++;\
            }\
            else if(buff[i]==' ' || buff[i]=='\t')\
                continue;\
            else\
                j=0;\
            if (j>1)\
            {\
                fprintf(stderr, "error! in line: %d, multiple commas.\n", currLine); /* error */\
                errorFlag=1;\
            }\
        }\
        j=0;\
}



char commands[COMANDS_AMOUNT][4]= {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt"};
char registers[REGISTERS_AMOUNT][3]= {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
typedef enum {mov,cmp,add,sub,not,clr,lea,inc,dec,jmp,bne,red,prn,jsr,rts,hlt} commandsNum;

int pass1(char *fileName, symbolTablePointer symTable, dataTablePointer dataTable,stringDataTablePointer stringDataTable, binaryTablePointer binTable);
int pass2(char *fileName, symbolTablePointer symTable, dataTablePointer dataTable, stringDataTablePointer stringDataTable, binaryTablePointer binTable);
int isAsymbol(symbolTablePointer, char*); /* checks if token is a symbol. and returns what type of symbol */
int tokenIsIndex(char *token, symbolTablePointer symTable); /* check if token is a index address */
int tokenIsRegister(char*); /* check if token is a register.. */
int tokenIsImmediate(char* token, symbolTablePointer symTable);
char* encode(int x, char *s);


int symbolAmount;
int currSymbol, currData, currStringData;
int errorFlag;
int IC, DC, L;
static int lineCount, tokenCount, symbolFlag, i, j, k;
static FILE *fp, *fp2, *fp3; /* file pointers */
static char ch, *token, *fileNameAm;
static char buff[LINE_SIZE]; /* a buffer with the size of line */
char buff2[LINE_SIZE];
char name[LINE_SIZE]; /* name of data */
int currInBinTable;
unsigned int* p;
int len;
int currLine, totalLinesCount;
static int temp, totalBinTableAmount;
char sym1[20], sym2[20];
char *fileNameExt, *fileNameEnt, *fileNameOb;
unsigned int pointer;
char *s;
int charCount;
int entFlag, extFlag;
char currCommand[4];
int commandValueOperand1, commandValueOperand2;
int commaCounter;
int indexOn1;
int regOn1;
int indexOn2;
int regOn2;

symbolTablePointer globalSymTable;
dataTablePointer globalDataTable;
stringDataTablePointer globalStringDataTable;
binaryTablePointer globalBinTable;
