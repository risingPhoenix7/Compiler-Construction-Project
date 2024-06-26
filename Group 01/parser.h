// GROUP 01
// NAMES	                 BITS ID
// ------------------------------------------
// Yash Pandey	             2021A7PS0661P
// Achinthya Hebbar S	     2021A7PS1457P
// Ujjwal Aggarwal	         2021A7PS2427P
// Hardik Gupta 	         2021A7PS2421P
// Agrawal Vansh Anil        2021A7PS2215P

#include "lexer.h"
#include "parserDef.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PARSER_H
#define PARSER_H

const char *TokenToString(int token);
char *NonTerminalToString(enum NonTerminals nonTerminal);
struct stack *initialiseStack();
struct Variable *createCopy(struct Variable var);
void printStack(struct stack *st, FILE *errors);
bool isEmptyStack(struct stack *st);
struct Variable *peek(struct stack *st);
bool onlyContainsEOF(struct stack *st);
bool isFull(struct stack *st);
void push(struct stack *st, struct Variable *data);
struct Variable *pop(struct stack *st);
int parseInputSourceCode(struct SymbolTableEntry *token,
                         struct ParsingTable *pt, struct stack *st,
                         lexicalAnalyser LA, struct tree_node *parent,
                         bool skipError, struct tree_node **parentpointer,
                         FILE *errors, int toPrint);
struct stack *initialiseStack();
void computeFirstSet(FirstAndFollow *sets_for_all, struct GrammarRule *G);
void computeFollowSet(FirstAndFollow *sets_for_all, struct GrammarRule *G);
void createParseTable(struct ParsingTable *PT, struct GrammarRule *productions,
                      struct Sets **sets_for_all, int *synchSet,
                      int defaultSynchSetSize);
void printParsingTable(struct ParsingTable *PT);
void printFFSetsTable(FILE *file, struct Sets **sets_for_all);
void insertAllKeywords();
FILE *readTestFile(const char *filename);
void printTree(struct tree_node *root, int depth);
void printParseTree(parseTree *PT, char *outfile);
void serialize_tree(struct tree_node *root);
void print_and_parse_tree(char *testfile, char *outputfile,
                          FirstAndFollow *sets, struct ParsingTable *PT,
                          Grammar G, int toPrint);
struct tree_node *create_tree_node(struct Variable *data);
FirstAndFollow *computeFirstAndFollow(struct GrammarRule *productions);
void addSyn(struct ParsingTable *PT, struct Sets **sets_for_all,
            int nonTerminal, int *synchSet, int defaultSynchSetSize);

#endif