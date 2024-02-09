// SCHEME: TWO-INPUT BUFFERS EACH OF SIZE N WITH SENTINEL CHARACTER "EOF" MARKING THE END OF EACH BUFFER

// INCLUDE LIBRARIES
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// PARAMETER N = NUMBER OF CHARACTERS READ WITH EACH RELOAD
#define BUFFER_SIZE 2569
#define FINAL_STATE_OFFSET NUM_NON_ACCEPT_STATES + 1 // this is same as NON ACCEPT STATES + 1
// TYPICALLY 256 BYTES

// EOF CHARACTERS AT END OF BOTH BUFFERS ALWAYS

// FUNCTIONS TO READ "RELOAD" INTO BUFFER FROM A FILE


// CODE TO READ FROM THE BUFFERS CHARACTER BY CHARACTER
struct TwinBuffer
{
    // 0 -> READING SECOND BUFFER, 1 -> READING FIRST BUFFER
    int readingFirst;
    FILE *file;
    char buffer[BUFFER_SIZE * 2 + 2];
};

void incrementLineNo(struct LexicalAnalyzer *LA)
{
    LA->lineNo += 1;
    printf("\n >> line no : %d\n", LA->lineNo);
    return;
}

FILE *readTestFile(char *file_path)
{
    FILE *file = fopen(file_path, "r");

    // UNABLE TO READ FILE
    if (file == NULL)
    {
        printf("Error opening file %s\n", file_path);
        return NULL;
    }

    return file;
}
int readIntoBuffer(struct TwinBuffer *twinBuffer)
{
    char *buffer;
    if (twinBuffer->readingFirst)
    {
        buffer = twinBuffer->buffer;
    }
    else
    {
        buffer = twinBuffer->buffer + BUFFER_SIZE + 1;
    }
    // READING ALTERNATE BUFFER
    twinBuffer->readingFirst = 1 - twinBuffer->readingFirst;
    size_t read_bytes = fread(buffer, sizeof(char), BUFFER_SIZE, twinBuffer->file);

    // MARK END OF INPUT
    if (read_bytes < BUFFER_SIZE)
    {
        // MARK WITH ANOTHER EOF
        buffer[read_bytes] = EOF;
    }

    // FOR LA TO RECOGNISE END OF INPUT FROM END OF BUFFER
    return read_bytes;
}

// CREATE THE BUFFER AND RETURN IT
struct TwinBuffer *initialiseTwinBuffer(FILE *file)
{
    // INITIALISE BUFFER
    struct TwinBuffer *twinBuffer = (struct TwinBuffer *)malloc(sizeof(struct TwinBuffer));

    twinBuffer->readingFirst = 1;
    twinBuffer->buffer[BUFFER_SIZE] = EOF;
    twinBuffer->buffer[2 * BUFFER_SIZE + 1] = EOF;
    twinBuffer->file = file;

    return twinBuffer;
}
void resetBegin(struct LexicalAnalyzer *LA)
{
    LA->begin = LA->forward;
}

void returnToStart(struct LexicalAnalyzer *LA)
{
    LA->state = 0;
    resetBegin(LA);
}

char *lexicalError(struct SymbolTableEntry *token)
{
    char *errorMessage = (char *)malloc(sizeof(char) * 100);

    if (token->tokenType == TK_ID)
    {
        if (strlen(token->lexeme) > MAX_ID_SIZE)
        {
            // char errorMessage[100];

            sprintf(errorMessage, "Lexical Error: Identifier %s greater than maximum size", token->lexeme);
            return errorMessage;
            // return "Lexical Error: Identifier %s of length %d greater than maximum size";
        }
    }

    if (token->tokenType == TK_FUNID)
    {
        if (strlen(token->lexeme) > MAX_FUNID_SIZE)
        {
            return "Lexical Error: Function Identifier %s of length %d greater than maximum size";
        }
    }

    return "";
}

// FUNCTION TO GET CORRESPONDING NUMBER
void equivalentNumber(struct LexicalAnalyzer *lex, int flag, struct SymbolTableEntry *token)
{
    // printf("EQV NUMS %s\n", token->lexeme);
    if (flag == TK_NUM1 || flag == TK_NUM2)
    {
        // INTEGER
        token->intValue = atoi(token->lexeme);
    }
    else
    {
        // DOUBLE
        token->doubleValue = atof(token->lexeme);
    }
}
void changeForward(struct LexicalAnalyzer *LA, int flag)
{
    // FLAG IS 1 FOR INCREMENT AND -1 FOR DECREMENT
    LA->forward = (LA->forward + flag) % (BUFFER_SIZE * 2 + 2);
}

void changeBegin(struct LexicalAnalyzer *LA, int flag)
{
    // FLAG IS 1 FOR INCREMENT AND -1 FOR DECREMENT
    LA->begin = (LA->begin + flag) % (BUFFER_SIZE * 2 + 2);
}

// TAKE ACTIONS BASED ON THE FINAL STATE AND RETURN A TOKEN
struct SymbolTableEntry *takeActions(struct LexicalAnalyzer *LA, struct SymbolTableEntry *token)
{
    // NON FINAL STATE32
    int state = LA->state;
    if (state < FINAL_STATE_OFFSET)
    {
        return token;
    }

    state -= FINAL_STATE_OFFSET;

    // DONT SET TOKEN WHEN DELIMITER
    if (state == CARRIAGE_RETURN || state == DELIMITER)
    {
        // RETURN TO START STATE
        returnToStart(LA);

        // DONT SET STATE OR LEXEME AND RETURN
        
        if (state == CARRIAGE_RETURN){
            incrementLineNo(LA);

            //RESET BEGIN AND INCREMENT FORWARD
            changeBegin(LA, 1);
        }

        if (state == DELIMITER){
            changeForward(LA, -1);
        }
        
        // printf("TOOK ACTIONS");
        return token;
    }

    // SET TOKEN TYPE [SET FOR TOKEN, NOT SET FOR CARRIAGE_RETURN OR DELIMITER]
    token->tokenType = state;

    // printf("SET STATE\n");

    // DOUBLE STAR STATES
    if (state == TK_NUM2 || state == TK_LT2)
    {
        // DECREMENT FORWARD POINTER
        changeForward(LA, -1);
    }

    // SET LEXEME
    strncpy(token->lexeme, LA->twinBuffer->buffer + LA->begin, LA->forward - LA->begin);

    // EQUIVALENT NUMBER
    if (state == TK_RNUM1 || state == TK_RNUM2 || state == TK_NUM1 || state == TK_NUM2)
    {

        // COMPUTE NUMBER
        equivalentNumber(LA, state, token);
    }

    // GET THE SYMBOL TABLE ENTRY
    else if (state == TK_FIELDID || state == TK_RUID || state == TK_ID || state == TK_FUNID)
    {
        token = getToken(token);
    }

    // FINAL STATE WITHOUT ANY OTHER ACTIONS
    else if (state != TK_COMMENT && state != TK_LT2 && state != TK_NUM2)
    {
        // INCREMENT FORWARD
        changeForward(LA, +1);

        // SET LEXEME
        strncpy(token->lexeme, LA->twinBuffer->buffer + LA->begin, LA->forward - LA->begin);
    }

    return token;
}

struct SymbolTableEntry *initialiseToken()
{

    struct SymbolTableEntry *token = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));

    token->lexeme = (char *)malloc(sizeof(char) * 100);
    token->intValue = 0;
    token->doubleValue = 0;
    token->lineNo = 0;
    token->tokenType = 0;
    return token;
}

struct SymbolTableEntry *scanToken(struct LexicalAnalyzer *LA)
{
    // INITIALSE TOKEN
    struct SymbolTableEntry *token = initialiseToken();

    // RESET TO START STATE
    LA->state = 0;

    char character;


    //while(1)
    while(1){
        // GET CHARACTER CURRENTLY BEING READ

        character = LA->twinBuffer->buffer[LA->forward];


        // TODO: DIFFERENTIATE BETWEEN END OF INPUT AND END OF BUFFER
        if (character == EOF)
        {

            // END OF BUFFER
            if (LA->forward == BUFFER_SIZE || LA->forward == (2 * BUFFER_SIZE + 1))
            {

                // RELOAD OTHER BUFER
                readIntoBuffer(LA->twinBuffer);

                // DONT NEED TO CHECK FOR INPUT END [FORWARD WOULD BE SOMETHING ELSE]
                // printf("RELOADED BUFFER\n");
            }

            // END OF INPUT
            else
            {
                // CHANGE STATE
                LA->state = getNextState(LA->state, 128); // 128 for end of file

                // TAKE ACTIONS FOR THE STATE
                token = takeActions(LA, token);

                // HAVE TO RETURN
                if (token->tokenType != 0)
                {
                    // RESET BEGIN
                    resetBegin(LA);
                    return token;
                }

                // ALL INPUT READ AND PROCESSED
                // printf("END OF INPUT. FINISHING SCANNING\n");
                return NULL;
            }
        }
        // printf("\nSTATE %d CHARACTER %d (%c)\n", LA->state, character, character);

        // CHANGE STATE
        LA->state = getNextState(LA->state, (int)character);

        // printf("STATE HERE\n");
        // TAKE ACTIONS FOR THE STATE
        token = takeActions(LA, token);

        // printf("TOOK ACTIONS!\n");
        // HAVE TO RETURN
        if (token->tokenType != 0)
        {
            resetBegin(LA);
            return token;
        }

        // INCREMENT FORWARD
        changeForward(LA, 1);
    }

    return NULL;
}

struct LexicalAnalyzer *initialiseLA(struct TwinBuffer *twinBuffer)
{
    struct LexicalAnalyzer *LA;
    LA = (struct LexicalAnalyzer *)malloc(sizeof(struct LexicalAnalyzer));

    LA->lineNo = 0;
    LA->begin = 0;
    LA->forward = 0;
    LA->state = 0;
    LA->twinBuffer = twinBuffer;
    return LA;
}

int main()
{

    // printf("ENTERED MAIN\n");
    
    // INITIALISE THE SYMBOL TABLE
    insertAllKeywords();

    // printf("KEYWORDS\n");

    // TEST THE TWIN BUFFER
    FILE *file = readTestFile("test_program.txt");

    // INITIALISE A TWIN BUFFER
    struct TwinBuffer *twinBuffer = initialiseTwinBuffer(file);

    // INITIALISE LA
    struct LexicalAnalyzer *LA = initialiseLA(twinBuffer);

    // printf("LA INITIALISED\n");

    // START SCANNING
    readIntoBuffer(twinBuffer);

    // printf("READ INPUT\n");

    // THE TOKEN
    struct SymbolTableEntry *token;

    // printf("STARTING SCANNING\n");

    while ((token = scanToken(LA)))
    {
        printf("(%s) %s  ", TokenToString(token->tokenType), token->lexeme);
    }

    printSymbolTable();
}
