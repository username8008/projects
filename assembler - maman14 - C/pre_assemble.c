#include "pre_assemble.h"


/*
    Function Name: pre_assemble
    Parameters:
        fileName: Name of the file to be pre-assembled.
    Return Type: int
    Description:
        Pre-assembles the specified file by processing macros and writing the output to a new file with the ".am" extension.
        Returns 0 if successful, or an error code if an error occurs during pre-assembly.
*/
int pre_assemble(char* fileName)
{
    currMacroToWriteInTable= 0;
    macroAmount= 0; /* starting with 1 macro */
    mcrFlag=0;
    lineCount=0;
    tokenCount=0;
    CurrLineInMacro=0;

    macros= (struct macroTable*)malloc(macroAmount*sizeof(struct macroTable));
    if(!macros)
    {
        printf("malloc error!\n");
        exit(0);
    }


    /* copy the name of the file and adding the ".am" and ".as" */
    fileNameAs= (char*)malloc(strlen(fileName)+4); /* +4 for the ".as\0" */
    if(!fileNameAs)
    {
        printf("malloc error!\n");
        exit(0);
    }
    strcpy(fileNameAs, fileName);
    strcat(fileNameAs, ".as\0");

    fileNameAm= (char*)malloc(strlen(fileName)+4);
    if(!fileNameAm)
    {
        printf("malloc error!\n");
        exit(0);
    }
    strcpy(fileNameAm, fileName);
    strcat(fileNameAm, ".am\0");


    if(!(fp= fopen(fileNameAs, "r"))) /* open file to read */
    {
        return READ_ERROR; /*fprintf(stderr, "error. cant open the file!\n"); */
    }
    free(fileNameAs);

    while(!feof(fp)) /* reads until end of file */
    {
        ch=fgetc(fp);
        if(ch=='\n')
        {
            lineSizeFlag= 0; /* to check if the line is too big */
            lineCount++;
        }
        else if(ch==EOF)
        {
            lineCount++;
            break;
        }
        if((lineSizeFlag++) >= LINE_SIZE)
        {
            fprintf(stderr, "error! line %d is too long..\n", lineCount); /* error */\
            return LINE_IS_TOO_LONG;
        }
    }

    rewind(fp);

    if(!(fp2= fopen(fileNameAm, "w"))) /* open file to write a new file.am */
    {
        return WRITE_ERROR; /*fprintf(stderr, "error. cant open the file!\n"); */
    }
    free(fileNameAm);


    while(lineCount--) /* reads until end of file (step 1) */
    {
        lineSizeFlag=0;
        for(i=0; i<LINE_SIZE; i++) /* cleaning the line */
            buff[i]='\0';
        fgets(buff, LINE_SIZE, fp); /* read a line */
        token = strtok(buff, " ");
        tokenCount= 1;
        nameFlag=0;
        while (token != NULL) /* taking word per word to check and write */
        {
            cleanToken /* first cleaning the token (for the macros).. */


            if(mcrFlag==0) /* "mcr" didnt found yet or "endmcr" was found.. */
            {
                if(currMacroToWriteInTable>0 && (macroPointer= isInMacroTable(token2, macroAmount))!=-1) /* checking if the token is a macro name that exist in the table (step 2) */
                    writeMacroToFile /* if its a macro write the macro in the file (not the name of it) */

                else if(!strcmp(token2, "mcr")) /* if its a macro definition (step 3) */
                {
                    mcrFlag= 1; /* (step 4) */
                }

                else
                {
                    while(token!=NULL)
                    {
                        fputs(" ", fp2);
                        fputs(token, fp2); /* not a macro name - just write the token to the file.. */
                        token = strtok(NULL, " \t \n");
                    }
                    fputs("\n", fp2);
                    tokenCount=0;
                }
            }
            else /* mcrFlag is on ("mcr" was found and "endmcr" didnt found yet..) */
            {
                if(currMacroToWriteInTable==macroAmount) /* new macro */
                {
                    macroAmount++;
                    temp= (struct macroTable*)realloc(macros, ((macroAmount)*sizeof(macros->name)*sizeof(macros->content))); /* adding space */
                    macros= temp;

                    if(!macros)
                    {
                        printf("malloc error!\n");
                        exit(0);
                    }
                    
                    strcpy(macros[currMacroToWriteInTable].name, token2); /* copy the name of the macro (step 5) */
                    nameFlag=1;
                    CurrLineInMacro=1;
                }
                if(!strcmp(token2, "endmcr"))
                {
                    currMacroToWriteInTable++;
                    mcrFlag= 0; /* (step 8) */
                    CurrLineInMacro=0;
                }
                else if(!nameFlag)
                {
                    if(token[strlen(token)-1]=='\n')
                        CurrLineInMacro++;
                    if(CurrLineInMacro>MACRO_LINE_AMOUNT)
                        return TOO_LONG_MACRO;
                    if(strcmp(token, "\n"))
                    {
                        strncat(macros[currMacroToWriteInTable].content, token, LINE_SIZE); /* copy the next token (step 6/7) */
                        strncat(macros[currMacroToWriteInTable].content, " ", LINE_SIZE);
                    }
                }
                nameFlag=0;
            }

            free(Mtoken2);
            Mtoken2= NULL;
            token = strtok(NULL, " ");
            tokenCount++;
        }
    }

    fputs("\n", fp2);
    /* (step 9) */
    fclose(fp); /* close the files */
    fclose(fp2);
    free(macros);
    macros=NULL;
    fileNameAs=NULL;
    fileNameAm=NULL;
    return 0;
}

/*
    Function Name: isInMacroTable
    Parameters:
        token: Token to search for in the macro table.
        macroAmount: Number of macros in the table.
    Return Type: int
    Description:
        Checks if the token exists in the macro table.
        Returns the index of the macro in the table if found, or -1 if not found.
*/
int isInMacroTable(char* token, int macroAmount) /* returns pointer to the macro in the table or -1 */
{
    int i;
    for(i=0; i<=macroAmount; i++)
    {
        if(!strcmp(token, macros[i].name)) /* checking if the macro exist.. (step 2)*/
        {
            return i;
        }
    }
    return -1;
}