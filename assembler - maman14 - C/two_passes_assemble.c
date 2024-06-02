#include "databases.h"
#include "two_passes_assemble.h"
#include "functions.h"

/*
    Function Name: assembler
    Parameters:
        fileName: Name of the file to be assembled.
    Return Type: int
    Description:
        Assembles the specified file by performing two-pass assembly.
        Returns 0 if successful, or an error code if an error occurs during assembly.
*/
int assembler(char *fileName)
{
    symbolTablePointer symTable;
    dataTablePointer dataTable;
    stringDataTablePointer stringDataTable;
    binaryTablePointer binTable;

    /* initalizing variables */
    int status;
    currLine= 0;
    totalLinesCount= lineCount;
    currInBinTable=0;
    currSymbol=0;
    currData=0;
    currStringData=0;
    errorFlag=0;
    tokenCount=0;
    i=j=k=0;
    totalBinTableAmount=0;
    symbolAmount=1;

    symTable = (symbolTablePointer)malloc(sizeof(struct symbolTable)*symbolAmount);
    dataTable = (dataTablePointer)malloc(sizeof(struct dataTable)*(currData+1));
    stringDataTable = (stringDataTablePointer)malloc(sizeof(struct stringDataTable)*(currStringData+1));
    binTable = (binaryTablePointer)malloc(sizeof(struct binaryTable)*1);

    cleanTabels

    /* initalizing some static variables */
    lineCount=0, tokenCount=0, symbolFlag=0;
    ch='\0', token=NULL, fileNameAm=NULL;


    status= pass1(fileName, symTable, dataTable, stringDataTable, binTable);


    /* initalizing some static variables */
    lineCount=0, tokenCount=0, symbolFlag=0;
    ch='\0', token=NULL, fileNameAm=NULL;

    symTable= globalSymTable;
    dataTable= globalDataTable;
    stringDataTable= globalStringDataTable;
    binTable= globalBinTable;


    if(!status)
        status= pass2(fileName, symTable, dataTable, stringDataTable, binTable);

    cleanTabels

    free(globalSymTable);
    symTable=NULL;
    globalSymTable=NULL;
    free(globalDataTable);
    dataTable=NULL;
    globalDataTable=NULL;
    free(globalStringDataTable);
    stringDataTable=NULL;
    globalStringDataTable=NULL;
    free(globalBinTable);
    binTable=NULL;
    globalBinTable=NULL;

    if(status)
        return STOP_ERROR;

    return 0;
}

/*
    Function Name: pass1
    Parameters:
        fileName: Name of the file to be processed in pass 1.
        symTable: Pointer to the symbol table.
        dataTable: Pointer to the data table.
        stringDataTable: Pointer to the string data table.
        binTable: Pointer to the binary table.
    Return Type: int
    Description:
        Performs the first pass of the assembler, processing the input file and generating the symbol table, data table, and binary table.
        Returns 0 if successful, or an error code if an error occurs during pass 1.
*/
int pass1(char *fileName, symbolTablePointer symTable, dataTablePointer dataTable, stringDataTablePointer stringDataTable, binaryTablePointer binTable)
{
    /* copy the name of the file and adding the ".am" */
    fileNameAm= (char*)malloc(strlen(fileName)+4);
    if(!fileNameAm)
    {
        printf("malloc error!\n");
        exit(0);
    }
    strcpy(fileNameAm, fileName);
    strcat(fileNameAm, ".am\0");

    if(!(fp= fopen(fileNameAm, "r"))) /* open file to read */
    {
        return READ_ERROR; /*fprintf(stderr, "error. cant open the file!\n"); */
    }
    free(fileNameAm);
    fileNameAm=NULL;

    lineCount=0;
    while(!feof(fp)) /* reads until end of file */
    {
        ch=fgetc(fp);
        if(ch=='\n')
        {
            lineCount++;
        }
    }
    rewind(fp);

    DC=IC=L=0; /* (step 1) */
    currLine= 0;
    totalLinesCount= lineCount;
    currInBinTable=0;
    currSymbol=0;
    currData=0;
    currStringData=0;
    errorFlag=0;
    tokenCount=0;
    i=j=k=0;
    totalBinTableAmount=0;

    
    while(lineCount--) /* reads until end of file (step 2) */
    {
        for(i=0; i<LINE_SIZE; i++) /* cleaning the line */
        {
            buff[i]='\0';
            buff2[i]='\0';
        }
        fgets(buff, LINE_SIZE, fp); /* read a line */
        currLine++;
        checkCommas /* check if there are multiple commas together */
        cleanBuff /* clean the buffer */
        token = strtok(buff2, " \t");
        tokenCount= 1; 
        symbolFlag=0;
        while (token != NULL) /* taking word per word to check */
        {
            /* checking if its not just a new line */
            newLineCheck

            if(!strcmp(token, ".define")) /* if its a define token (step 3) */
            writeDefineInSymbolTable /* enter to the symbols table (step 4) */

            else if(tokenCount==1 && tokenIsASymbolDefinition) /* if the first word in the line is a symbol definition (step 5) */
            {
                strcpy(name, token); /* taking the name of the symbol */
                name[strlen(token)-1]='\0';
                token = strtok(NULL, " \t");
                tokenCount++;
                symbolFlag=1; /* (step 6) */
            }

            else if(tokenIsData) /* if the token is a data instruction (step 7) */
            {
                if(symbolFlag==1) /* if the symbol flag is on (step 8) */
                {
                    writeInSymbolTable
                }
                
                writeDataInTable /* write the data in the table (step 9) */

                while(token!=NULL)
                    token = strtok(NULL, "\t \n");
            }

            else if(!strcmp(token, ".extern") || !strcmp(token, ".entry")) /* if its an extern or entry instruction (step 10) */
            {
                writeInSymbolTable
            }

            else if(symbolFlag==1) /* if the symbol flag is on (step 12) */
            {
                writeInSymbolTable
                symbolFlag=0;
            }

            else
            {
                checkForCommand /* search and analyze the command in the line (step 13-14) */
                writeInBinTable /* add the binary data to the table */
            }

            IC+=L; /* update the IC (step 15) */
            L=0;
        }
    }


    if(errorFlag>0) /* if there was an error stop here.. (step 16) */
        return STOP_ERROR;

    updateDataInSymbolTable /* update every symbol in the table with the value of IC+100 (step 17) */

    addDataToBinTable /* add the data from the data tables to the binary tables. */

    updateEntries /* dealing with entries */

    if(errorFlag>0) /* if there was an error stop here.. (step 16) */
        return STOP_ERROR;


    fclose(fp); /* close the file */
    fp=NULL;

    globalSymTable= symTable;
    globalDataTable= dataTable;
    globalStringDataTable= stringDataTable;
    globalBinTable= binTable;

    totalBinTableAmount= currInBinTable;
    
    return 0; /* return 0 (good status) and continue to pass 2.. (step 18) */
}

/*
    Function Name: pass2
    Parameters:
        fileName: Name of the file to be processed in pass 2.
        symTable: Pointer to the symbol table.
        dataTable: Pointer to the data table.
        stringDataTable: Pointer to the string data table.
        binTable: Pointer to the binary table.
    Return Type: int
    Description:
        Performs the second pass of the assembler, generating the object file from the generated tables.
        Returns 0 if successful, or an error code if an error occurs during pass 2.
*/
int pass2(char *fileName, symbolTablePointer symTable, dataTablePointer dataTable, stringDataTablePointer stringDataTable, binaryTablePointer binTable)
{
    currInBinTable=0;
    i=0;
    fp=NULL;
    fp2=NULL;
    fp3=NULL;
    fileNameExt= NULL;
    fileNameEnt= NULL;
    fileNameOb= NULL;
    entFlag= 0;
    extFlag= 0;

    fileNameExt= (char*)malloc(strlen(fileName)+5); /* +5 for the ".ext\0" */
    fileNameEnt= (char*)malloc(strlen(fileName)+5); /* +5 for the ".ent\0" */
    fileNameOb= (char*)malloc(strlen(fileName)+4); /* +4 for the ".ob\0" */
    
    if(!fileNameExt)
    {
        printf("malloc error!\n");
        exit(0);
    }
    strcpy(fileNameExt, fileName);
    strcat(fileNameExt, ".ext\0");

    if(!fileNameEnt)
    {
        printf("malloc error!\n");
        exit(0);
    }
    strcpy(fileNameEnt, fileName);
    strcat(fileNameEnt, ".ent\0");

    if(!fileNameOb)
    {
        printf("malloc error!\n");
        exit(0);
    }
    strcpy(fileNameOb, fileName);
    strcat(fileNameOb, ".ob\0");

    if(!(fp3= fopen(fileNameOb, "w"))) /* open file to write */
        return WRITE_ERROR;

    /* adding the number of the IC and DC to the ob file */
    fprintf(fp3, "  %d ", IC);
    fprintf(fp3, "%d\n", DC);
    
    while (totalBinTableAmount!=currInBinTable) /* checking the binary table for miising info */
    {
            token = strtok(binTable[currInBinTable].source, "\t , [ \n");
            if(binTable[currInBinTable].adress==0 && token!=NULL) /* if there is missing info */
            {
                    for(i=0; i<symbolAmount; i++) /* search for it in the symbol table */
                    {
                        if(!strcmp(symTable[i].name, token)) /* if its found in the symbol table.. */
                        {
                            binTable[currInBinTable].adress= currInBinTable+100; /* update the adress */
                            binTable[currInBinTable].bin=*symTable[i].value; /* add it to the binary table */
                            binTable[currInBinTable].bin= binTable[currInBinTable].bin<<2; /* move it so its not on the A,R,E */
                            binTable[currInBinTable].bin+=2; /* and add the value 2 to the A,R,E - internal */
                            if(!strcmp(symTable[i].type , "external")) /* if its external substruct 1 so it bacome 1 - external and add to external file */
                            {
                                binTable[currInBinTable].bin-=1;
                                if(!extFlag)
                                {
                                    if(!(fp= fopen(fileNameExt, "w"))) /* open file to write */
                                        return WRITE_ERROR;
                                    extFlag=1;
                                }
                                fputs(symTable[i].name, fp);
                                fputs(" ", fp);
                                fprintf(fp, "%d", binTable[currInBinTable].adress);
                                fputs("\n", fp);
                            }
                            else if(!strcmp(symTable[i].type , "entry")) /* add to entry file */
                            {
                                if(!entFlag)
                                {
                                    if(!(fp2= fopen(fileNameEnt, "w"))) /* open file to write */
                                        return WRITE_ERROR;
                                    entFlag=1;
                                }
                                fputs(symTable[i].name, fp2);
                                fputs(" ", fp2);
                                fprintf(fp2, "%d", *symTable[i].value);
                                fputs("\n", fp2);
                            }


                            break;
                        }
                    }
                    if(i==symbolAmount) /* symbol was not found */
                    {
                        fprintf(stderr, "error! undefined symbol: %s\n", token); /* error */
                        errorFlag=1;
                    }
            }

            /* write the ob file! */
            s= malloc(8); /* the string that will return from the function */
            for(i=0; i<8; i++)
                s[i]='0';
            s[i]='\0';
            fputs("0", fp3);
            fprintf(fp3, "%d", binTable[currInBinTable].adress);
            fputs(" ", fp3);
            pointer= binTable[currInBinTable].bin;
            fputs(encode(pointer, s), fp3);
            free(s);


            currInBinTable++; /* to the next one.. */

    }

    if(fp!=NULL)
        fclose(fp); /* close the files */
    if(fp2!=NULL)
        fclose(fp2);
    if(fp3!=NULL)
        fclose(fp3);

    if(errorFlag>0) /* if there was an error stop here.. (step 16) */
    {
        remove(fileNameEnt);
        remove(fileNameExt);
        remove(fileNameOb);
    }

    /* free some memory */
    if(fileNameExt!=NULL)
        free(fileNameExt);
    if(fileNameEnt!=NULL)
        free(fileNameEnt);
    if(fileNameOb!=NULL)
        free(fileNameOb);

    if(errorFlag>0) /* if there was an error stop here.. (step 16) */
    {
        return STOP_ERROR;
    }

    globalSymTable= symTable;
    globalDataTable= dataTable;
    globalStringDataTable= stringDataTable;
    globalBinTable= binTable;


    return 0;
}
