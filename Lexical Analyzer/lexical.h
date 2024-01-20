// Enumerated token types
typedef enum Tokentype {
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_OP,
    TK_CL,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE
};

// Corresponding keywords or symbols
char* keywordList[] = {
    "=",
    "%",
    "with",
    "parameters",
    "end",
    "while",
    "union",
    "endunion",
    "definetype",
    "as",
    "type",
    "_main",
    "global",
    "parameter",
    "list",
    "input",
    "output",
    "int",
    "real",
    "endwhile",
    "if",
    "then",
    "endif",
    "read",
    "write",
    "return",
    "call",
    "record",
    "endrecord",
    "else",
    ",",
    ";",
    ":",
    ".",
    "(",
    ")",
    "+",
    "-",
    "*",
    "/",
    "&&&",
    "@@@",
    "~",
    "<",
    "<=",
    "==",
    ">",
    ">=",
    "!="
};

