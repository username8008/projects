#include "main.h"

int main (int argc, char* argv[])
{
    if (argc<2)
    {
        printf("no arguments - enter files name.\n");
        exit(0);
    }

    while(--argc)
    {
        fileName=argv[argc]; /* taking files name from the line arguments */

        printf("file: %s.\n", fileName);

        status= pre_assemble(fileName); /* sending the fileName to the pre assemble procces, and making file.am (the new file after the procces..) */

        /* making the name of file.am just in case we need to remove it */
        fileNameAm= (char*)malloc(strlen(fileName)+4);
        if(!fileNameAm)
        {
            printf("malloc error!\n");
            exit(0);
        }
        strcpy(fileNameAm, fileName);
        strcat(fileNameAm, ".am\0");


        if(status==0) /* no errors */
        {
            status= assembler(fileName); /* the assemble proccess.. */
        }

        if(status!=0) /* errors in pass 1 or 2 */
        {
            printf("STATUS: an error ocurred! cant build the files in file %s.\n", fileName);
            remove(fileNameAm);
        }
        else
            printf("STATUS: all the files have been built in file %s.\n", fileName); /* no errors */
            


        free(fileNameAm);
        
    }
    return 0;
}