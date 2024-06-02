#define LINE_SIZE 81
#define SYMBOL_TYPE_SIZE 10



struct symbolTable
{
    char name[LINE_SIZE]; /* symbol name */
    char type[SYMBOL_TYPE_SIZE]; /* the type of the symbol */
    int value[LINE_SIZE]; /* the value of the symbol */
};
typedef struct symbolTable *symbolTablePointer;


struct binaryTable
{
    int adress;
    char source[LINE_SIZE]; /*source code  */
    int bin; /* binary code */
};
typedef struct binaryTable *binaryTablePointer;


struct command /* for exemple: cmp r3 , #2 */
{
    unsigned int ARE : 2; /* 2 bits for the A,R,E */
    unsigned int operand2 : 2; /* 2 bits.. */
    unsigned int operand1 : 2;
    unsigned int opcode : 4; /* 4 bits for the opcode */
    unsigned int a : 4; /* 4 unused bits.. */
}commandValue;

struct dataOpperand /* 1, 2, 3, 4 */
{
    signed int operand : 14; /* 14 bits for operand 1 */
}dataOpperand1;

struct opperand /* r3 , #2 */
{
    unsigned int ARE : 2; /* 2 bits for the A,R,E */
    signed int operand : 12; /* 12 bits for operand 1 */
}opperandValue1, opperandValue2, opperandValue3, opperandValue4;

struct Regopperand /* r3 , r4 */
{
    unsigned int ARE : 2; /* 2 bits for the A,R,E */
    signed int operand2 : 3; /* 3 bits for operand 2 */
    signed int operand1 : 3; /* 3 bits for operand 1 */
    unsigned int a : 6; /* 5 unused bits.. */
}reg1, reg2;

struct dataTable
{
    char name[LINE_SIZE];
    int value[LINE_SIZE*sizeof(int)];
};
typedef struct dataTable *dataTablePointer;

struct stringDataTable
{
    char name[LINE_SIZE];
    char value[LINE_SIZE];
};
typedef struct stringDataTable *stringDataTablePointer;