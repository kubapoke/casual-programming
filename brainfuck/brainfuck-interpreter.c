#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#define CELLS_SIZE 32768
#define BUF_SIZE 32768
#define COMMAND_SIZE 32768

struct stackNode{
    struct stackNode* prev;
    int value;
};

int isOneElem_S(struct stackNode* S)
{
    if(S->prev == NULL)
        return 1;
    return 0;
}

void push_S(struct stackNode** S, int val)
{
    struct stackNode* newS;
    newS = malloc(sizeof(struct stackNode));
    newS->value = val;
    newS->prev = *S;
    *S = newS;
}

void pop_S(struct stackNode** S)
{
    struct stackNode* tempS;
    tempS = *S;

    if ((*S)->prev)
        *S = (*S)->prev;
    
    free(tempS);
}

void ERR(char* message, int number)
{
    switch (number)
    {
    case -1:
        fprintf(stdout, "ERROR: %s\n", message);
        break; 
    default:
        fprintf(stdout, "ERROR: %s %d", message, number);
        break;
    }

    exit(EXIT_FAILURE);
}

void readfile(char* filename, char* instructions, int* brackets)
{
    FILE* fd;
    char buffer[BUF_SIZE];

    if ((fd = fopen(filename, "r")) == NULL)
        ERR("The file could not be opened", -1);
        
    struct stackNode* top = malloc(sizeof(struct stackNode));
    top->prev = NULL;
    top->value=-1;

    while(fgets(buffer, BUF_SIZE, fd))
    {
        for(int i = 0; i < strlen(buffer); ++i)
            if(buffer[i] == '+' || buffer[i] == '-' || buffer[i] == '<' || buffer[i] == '>' || buffer[i] == '[' || buffer[i] == ']' || buffer[i] == '.' || buffer[i] == ',')
            {   
                strncat(instructions, &buffer[i], ((size_t)1));

                if(buffer[i] == '[')
                    push_S(&top, strlen(instructions) - 1);
                else if(buffer[i] == ']')
                {
                    if(isOneElem_S(top))
                    {
                        free(top);
                        ERR("Incorrect bracketing", -1);
                    }

                    int prevBracket = top->value;
                    pop_S(&top);

                    brackets[strlen(instructions) - 1] = prevBracket + 1;
                    brackets[prevBracket] = strlen(instructions);
                }
                else
                {
                    brackets[strlen(instructions) - 1] = -1;
                }
            }                
    }

    if(!isOneElem_S(top))
    {
        while(!isOneElem_S(top))
            pop_S(&top);
        free(top);
        ERR("Incorrect bracketing", -1);
    }

    free(top);

    if(fclose(fd))
        ERR("The file could not be closed correctly", -1);

    return;
}

void interpret(char* instructions, int* brackets)
{
    char cells[CELLS_SIZE];

    char* ip = instructions;
    char* cp = cells;

    for(int i=0; i < CELLS_SIZE; i++)
    {
        cells[i] = (char)0;
    }

    while(*ip)
    {
        switch (*ip)
        {
        case '+':
            ++(*cp);
            if((*cp) > 127)
                ERR("Value out of range (too big) in position", (int)(ip - instructions));
            break;
        case '-':
            --(*cp);
            if((*cp) < -128)
                ERR("Value out of range (too small) in position", (int)(ip - instructions));
            break;
        case '<':
            --cp;
            if(cp < cells)
                ERR("Cell pointer out of range (too small) in position", (int)(ip - instructions));
            break;  
        case '>':
            ++cp;
            if(cp >= cells + CELLS_SIZE)
                ERR("Cell pointer out of range (too small) in position", (int)(ip - instructions));
            break;
        case '[':
            if((*cp) == 0)
                ip = instructions + brackets[(int)(ip - instructions)] - 1;
            break;     
        case ']':
            if((*cp) != 0)
                ip = instructions + brackets[(int)(ip - instructions)] - 1;
            break;   
        case '.':
            putchar(*cp);
            break;    
        case ',':
            (*cp) = getchar();
            break;      
        default:
            ERR("Incorrect symbol in position", (int)(ip - instructions));
            break;
        }

        ++ip;
    }
}

int main(int argc, char** argv)
{
    char instructions[COMMAND_SIZE];
    int brackets[COMMAND_SIZE];

    if(argc != 2)
        ERR("The program takes exactly 1 argument (path to the brainfuck file to interpret)", -1);

    readfile(argv[1], instructions, brackets);

    interpret(instructions, brackets);

    return EXIT_SUCCESS;
}