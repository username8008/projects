    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    static char *fileName, *fileNameAm;
    int status;


    /*
    Function Name: pre_assemble
    Parameters:
        fileName: Name of the file to be pre-assembled.
    Return Type: int
    Description:
        Pre-assembles the specified file and generates a new file with the ".am" extension.
    */
    int pre_assemble(char *fileName);

    /*
    Function Name: assembler
    Parameters:
        fileName: Name of the file to be assembled.
    Return Type: int
    Description:
        Assembles the specified file.
    */
    int assembler(char *fileName);