


int isAsymbol(symbolTablePointer symTable, char *token) /* checks if token is a symbol. and returns what type of symbol */
{
    int i;
    for(i=0; i<symbolAmount; i++)
        {
            if(!strcmp(symTable[i].name, token))
            {
                if(!strcmp(symTable[i].type, "external"))
                    return 2;
                else
                    return 1;
            }
        }
    return 0;
}


/*
    Function Name: tokenIsImmediate
    Parameters:
        token: Token to check if it is an immediate value.
        symTable: Pointer to the symbol table.
    Return Type: int
    Description:
        Checks if the token represents an immediate value and returns its integer value.
*/
int tokenIsImmediate(char* token, symbolTablePointer symTable)
{
    int val, i;
    char *token1, *imeddiateToken;
    val=0;
    token1 = (char*)malloc(strlen(token) + 1); /* Allocate memory with space for null terminator */
    if(!token1)
    {
        fprintf(stderr, "malloc error\n");
        exit(0);
    }
    strcpy(token1, token);

    imeddiateToken= strchr(token1, '#') + 1; /* point to the character after '#' */

    if(isdigit((*imeddiateToken)) || isdigit((*(imeddiateToken+1)))) /* if its "-2 or +2 or just 2" */
    {
        val= atoi((imeddiateToken));
        free(token1);
        token1=NULL;
        return val;
    }

    else
    {
        for(i=0; i<symbolAmount; i++) /* checking if its a symbol value */
        {
            if(!strcmp(symTable[i].name, imeddiateToken))
            {
                free(token1);
                token1=NULL;
                return symTable[i].value[0];
            }
        }
        if(j==symbolAmount)
        {
            fprintf(stderr, "error! undefined symbol: %s\n", imeddiateToken); /* error */
            errorFlag=1;
        }
    }
    free(token1);
    token1=NULL;
    return 0;
}

/*
    Function Name: tokenIsIndex
    Parameters:
        token: Token to check if it contains an index.
        symTable: Pointer to the symbol table.
    Return Type: int
    Description:
        Checks if the token contains an index and returns its value.
        Returns -1 if no index is found.
*/
int tokenIsIndex(char *token, symbolTablePointer symTable)
{
    int i, indexValue;
    char *token1;
    char *endBracket, *indexToken;
    indexValue=-1;
    token1 = (char*)malloc(strlen(token) + 1); /* Allocate memory with space for null terminator */
    if(!token1)
    {
        fprintf(stderr, "malloc error\n");
        exit(0);
    }
    strcpy(token1, token);
    
    for (i = 0; i < strlen(token); i++)
    {
        if (token[i] == '[')
        {
            endBracket = strchr(token1, ']');
            if (endBracket != NULL)
            {
                *endBracket = '\0'; /* null terminate at ']' */
                indexToken = strchr(token1, '[') + 1; /* point to the character after '[' */
                if (isdigit(indexToken[0]))
                {
                    indexValue = atoi(indexToken);
                } 
                else
                {
                    for(j=0; j<symbolAmount; j++) /* checking if its a symbol value */
                    {
                        if(!strcmp(symTable[j].name, indexToken))
                        {
                            indexValue= symTable[j].value[0];
                            break;
                        }
                    }
                }
                free(token1); /* free memory before returning */
                token1=NULL;
                return indexValue;
            }
        }
    }
    free(token1); /* free memory before returning */
    token1=NULL;
    return -1; /* return -1 if no index is found */
}

/*
    Function Name: tokenIsRegister
    Parameters:
        token: Token to check if it represents a register.
    Return Type: int
    Description:
        Checks if the token represents a register and returns its index.
        Returns -1 if the token is not a register.
*/
int tokenIsRegister(char *token) /* check if token is a register.. */
{
    int i;
    for(i=0; i<REGISTERS_AMOUNT; i++)
    {
        if(!strcmp(token, registers[i]))
            return i;
    }
    return -1;
}

/*
    Function Name: encode
    Parameters:
        x: Integer value to encode.
        s: Buffer to store the encoded string.
    Return Type: char*
    Description:
        Encodes the integer value into a string representation and stores it in the provided buffer.
        The encoded string consists of symbols '*' (binary 00), '#' (binary 01), '% (binary 10), and '!' (binary 11).
        The encoded string is terminated with a newline character '\n'.
        Returns a pointer to the encoded string.
*/
char* encode(int x, char *s)
{
    unsigned int mask= 1<<(14-1);
    int count=0;
    int curr=0, i=0;

    while (mask)
    {
            
        if(count>1)
        {
            switch(curr) /* prints the corresponding sing */
            {
                case 0: 
                    s[i]= '*';
                    break;
                case 1: 
                    s[i]= '#';
                    break;
                case 2: 
                    s[i]= '%';
                    break;
                case 3: 
                    s[i]= '!';
                    break;
            }
            
            count=0;
            curr=0;
            i++;
        }


        if (x&mask) /* if the current bit is 1 */
        {
            

            curr+= count==1? 1 : 2; /* if its the first one add 1 else(the second one) add 2 */
        }

        count++;

        mask >>= 1; /* move to the next bit */
    }
    switch(curr) /* prints the corresponding sing */
    {
        case 0: 
            s[i]= '*';
            break;
        case 1: 
            s[i]= '#';
            break;
        case 2: 
            s[i]= '%';
            break;
        case 3: 
            s[i]= '!';
            break;
    }
    s[++i]= '\n';

    return s;
}