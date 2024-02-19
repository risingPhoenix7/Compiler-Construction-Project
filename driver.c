#include "Lexical Analyzer/lexical.h"

#include <stdio.h>
#include <time.h>

void remove_comments();
void print_token_list();
void parse_and_print_tree();
void print_timing_info(clock_t start_time, clock_t end_time);

int main(int argc, char *argv[])
{
    int option;
    clock_t start_time, end_time;

    do
    {
        printf("\nOptions:\n");
        printf("0 : Exit\n");
        printf("1 : Remove comments\n");
        printf("2 : Print token list\n");
        printf("3 : Parse and print parse tree\n");
        printf("4 : Print timing information\n");

        printf("Enter option: ");
        scanf("%d", &option);

        switch (option)
        {
        case 0:
            printf("Exiting...\n");
            break;
        case 1:
            remove_comments(argv[1]);
            break;
        case 2:
            print_token_list(argv[1]);
            break;
        case 3:
            parse_and_print_tree(argv[2]);
            break;
        case 4:
            start_time = clock();
            end_time = clock();
            print_timing_info(start_time, end_time);
            break;
        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (option != 0);

    return 0;
}

void remove_comments(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    char buffer[50];
    int prev_char = '\0'; // Store the previous character read

    while (fgets(buffer, 50, file) != NULL)
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (prev_char == '%' && buffer[i] != '\n')
            {
                continue;
            }

            if (prev_char == '%' && buffer[i] == '\n')
            {
                printf("%c", buffer[i]);
            }
            else if (buffer[i] == '\n')
            {
                printf("%c", buffer[i]);
            }
            else if (buffer[i] != '%')
            {
                printf("%c", buffer[i]);
            }
            prev_char = buffer[i];
        }
    }

    fclose(file);
}

void print_token_list(char *filename)
{
    printf("Printing token list...\n");
    insertAllKeywords();

    // printf("KEYWORDS\n");

    // TEST THE TWIN BUFFER
    FILE *file = readTestFile(filename);

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
        // printf("HU");
        printf("(%s : %s)", TokenToString(token->tokenType), token->lexeme);
    }
}

void parse_and_print_tree(char *filename)
{
    // FILE *file = fopen(filename, "w");
    printf("Parsing and printing parse tree...\n");
    // Implement parsing and printing parse tree logic here
}

void print_timing_info(clock_t start_time, clock_t end_time)
{
    double total_CPU_time, total_CPU_time_in_seconds;

    total_CPU_time = (double)(end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

    printf("Total CPU time: %f\n", total_CPU_time);
    printf("Total CPU time in seconds: %f\n", total_CPU_time_in_seconds);
}
