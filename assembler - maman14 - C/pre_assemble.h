#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_SIZE 81
#define MACRO_LINE_AMOUNT 20
#define READ_ERROR -1
#define WRITE_ERROR -2
#define LINE_IS_TOO_LONG -3
#define TOO_LONG_MACRO -4

/* clean the token (use only when its a macro) */
#define cleanToken \
{\
    Mtoken2= (char*) malloc(strlen(token)+1);\
    if(Mtoken2!=NULL)\
        for(i=0; i<=strlen(token); i++)\
            Mtoken2[i] = '\0';\
    else\
    {\
        printf("malloc error!\n");\
        exit(0);\
    }\
    token2=Mtoken2;\
    strcpy(token2, token);\
    /* more of cleaning the token.. */\
    while(token2[0]=='\t')\
        token2++;\
    for(i=0; i<=(strlen(token2)-1); i++)\
        if(token2[i]=='\t')\
            token2[i]='\0';\
    if(token2[strlen(token2)-1]=='\n')\
        token2[strlen(token2)-1]='\0';\
}

#define writeMacroToFile fputs(macros[macroPointer].content, fp2); /* write the content of the macro to the file */

struct macroTable /* a struct of macros */
{
    char name[LINE_SIZE-4]; /* macro name (size of line -4 because the "mcr") */
    char content[MACRO_LINE_AMOUNT * LINE_SIZE]; /* the content of the macro */
};

struct macroTable *macros, *temp;

int isInMacroTable(char* token, int macroAmount);


FILE *fp, *fp2; /* file pointers */
char *token;
char buff[LINE_SIZE]; /* a buffer with the size of line */
int macroPointer, mcrFlag; /* macroPointer is for pointing to a macro in the table, mcrFlag is on when there is a definition of macro until the end of it. */
int macroAmount;
int tokenCount;
int lineCount, CurrLineInMacro;
char ch, *fileNameAs, *fileNameAm;
int nameFlag, lineSizeFlag;
int currMacroToWriteInTable, i;
char *token2, *Mtoken2;