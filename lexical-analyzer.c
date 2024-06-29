#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_PROGRAM,
    TOKEN_VAR,
    TOKEN_INTEGER,
    TOKEN_BOOLEAN,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_BEGIN,
    TOKEN_END,
    TOKEN_READ,
    TOKEN_READLN,
    TOKEN_WRITELN,
    TOKEN_PLUS,
    TOKEN_WRITE,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_ASSIGN,
    TOKEN_MINUS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_LE,
    TOKEN_GT,
    TOKEN_GE,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_PERIOD,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_LESS_EQUAL,
    TOKEN_STRING_VALUE,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_TO,
    TOKEN_REPEAT,
    TOKEN_UNTIL,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[256];
} Token;

typedef struct Variable {
    char name[256];
    struct Variable *next;
} Variable;

FILE *source;
Token current_token;
int error_flag = 0;
Variable *variables = NULL;

void next_token();
void match(TokenType expected);
void error(const char *message);
void declare_variable(const char *name);
int is_declared(const char *name);

void program();
void block();
void variable_declaration();
void statement_list();
void statement();
void identifier();
void identifier_list();
void parameter_list();
void number();
void string_value();
void expression();
void assignment_statement();
void while_statement();
void read_statement();
void readln_statement();
void write_statement();
void writeln_statement();
void if_statement();
void for_statement();
void repeat_statement();
void simple_expression();
void term();
void factor();
void relational_operator();
void optional_assignment_sequence();

void next_token() {
    int c = fgetc(source);

    while (isspace(c)) {
        c = fgetc(source);
    }

    if (c == EOF) {
        current_token.type = TOKEN_EOF;
        return;
    }

    if (isalpha(c)) {
        int i = 0;
        do {
            current_token.lexeme[i++] = c;
            c = fgetc(source);
        } while (isalnum(c));
        ungetc(c, source);
        current_token.lexeme[i] = '\0';

        if (strcmp(current_token.lexeme, "program") == 0) {
            current_token.type = TOKEN_PROGRAM;
        } else if (strcmp(current_token.lexeme, "var") == 0) {
            current_token.type = TOKEN_VAR;
        } else if (strcmp(current_token.lexeme, "integer") == 0 || strcmp(current_token.lexeme, "Integer") == 0) {
            current_token.type = TOKEN_INTEGER;
        } else if (strcmp(current_token.lexeme, "boolean") == 0 || strcmp(current_token.lexeme, "Boolean") == 0) {
            current_token.type = TOKEN_BOOLEAN;
        } else if (strcmp(current_token.lexeme, "string") == 0 || strcmp(current_token.lexeme, "String") == 0) {
            current_token.type = TOKEN_STRING;
        } else if (strcmp(current_token.lexeme, "begin") == 0) {
            current_token.type = TOKEN_BEGIN;
        } else if (strcmp(current_token.lexeme, "end") == 0) {
            current_token.type = TOKEN_END;
        } else if (strcmp(current_token.lexeme, "read") == 0) {
            current_token.type = TOKEN_READ;
        } else if (strcmp(current_token.lexeme, "readln") == 0) {
            current_token.type = TOKEN_READLN;
        } else if (strcmp(current_token.lexeme, "write") == 0) {
            current_token.type = TOKEN_WRITE;
        } else if (strcmp(current_token.lexeme, "writeln") == 0) {
            current_token.type = TOKEN_WRITELN;
        } else if (strcmp(current_token.lexeme, "while") == 0) {
            current_token.type = TOKEN_WHILE;
        } else if (strcmp(current_token.lexeme, "do") == 0) {
            current_token.type = TOKEN_DO;
        } else if (strcmp(current_token.lexeme, "if") == 0) {
            current_token.type = TOKEN_IF;
        } else if (strcmp(current_token.lexeme, "then") == 0) {
            current_token.type = TOKEN_THEN;
        } else if (strcmp(current_token.lexeme, "else") == 0) {
            current_token.type = TOKEN_ELSE;
        } else if (strcmp(current_token.lexeme, "for") == 0) {
            current_token.type = TOKEN_FOR;
        } else if (strcmp(current_token.lexeme, "to") == 0) {
            current_token.type = TOKEN_TO;
        } else if (strcmp(current_token.lexeme, "repeat") == 0) {
            current_token.type = TOKEN_REPEAT;
        } else if (strcmp(current_token.lexeme, "until") == 0) {
            current_token.type = TOKEN_UNTIL;
        } else if (strcmp(current_token.lexeme, "and") == 0 || strcmp(current_token.lexeme, "AND") == 0) {
            current_token.type = TOKEN_AND;
        } else if (strcmp(current_token.lexeme, "or") == 0 || strcmp(current_token.lexeme, "OR") == 0) {
            current_token.type = TOKEN_OR;
        } else if (strcmp(current_token.lexeme, "true") == 0 || strcmp(current_token.lexeme, "True") == 0) {
            current_token.type = TOKEN_TRUE;
        } else if (strcmp(current_token.lexeme, "false") == 0 || strcmp(current_token.lexeme, "False") == 0) {
            current_token.type = TOKEN_FALSE;
        } else if (strcmp(current_token.lexeme, "not") == 0 || strcmp(current_token.lexeme, "NOT") == 0) {
            current_token.type = TOKEN_NOT;
        } else {
            current_token.type = TOKEN_IDENTIFIER;
        }
        return;
    }

    if (isdigit(c)) {
        int i = 0;
        do {
            current_token.lexeme[i++] = c;
            c = fgetc(source);
        } while (isdigit(c));

        ungetc(c, source);
        current_token.lexeme[i] = '\0';
        current_token.type = TOKEN_NUMBER;
        return;
    }

    if (c == '\'') {
        int i = 0;
        current_token.lexeme[i++] = c;
        c = fgetc(source);
        while (c != EOF && c != '\'') {
            current_token.lexeme[i++] = c;
            c = fgetc(source);
        }

        if (c == '\'') {
            current_token.lexeme[i++] = c;
        }
        current_token.lexeme[i] = '\0';
        current_token.type = TOKEN_STRING_VALUE;
        return;
    }

    if (c == '"') {
        int i = 0;
        current_token.lexeme[i++] = c;
        c = fgetc(source);
        while (c != EOF && c != '"') {
            current_token.lexeme[i++] = c;
            c = fgetc(source);
        }

        if (c == '"') {
            current_token.lexeme[i++] = c;
        }
        current_token.lexeme[i] = '\0';
        current_token.type = TOKEN_STRING_VALUE;
        return;
    }
    
    switch (c) {
        case ':':
            c = fgetc(source);
            if (c == '=') {
                current_token.type = TOKEN_ASSIGN;
            } else {
                ungetc(c, source);
                current_token.type = TOKEN_COLON;
            }
            break;
        case '+':
            current_token.type = TOKEN_PLUS;
            break;
        case '-':
            current_token.type = TOKEN_MINUS;
            break;
        case '*':
            current_token.type = TOKEN_MULT;
            break;
        case '/':
            current_token.type = TOKEN_DIV;
            break;
        case '=':
            current_token.type = TOKEN_EQ;
            break;
        case ';':
            current_token.type = TOKEN_SEMICOLON;
            break;
        case '.':
            current_token.type = TOKEN_PERIOD;
            break;
        case '(':
            current_token.type = TOKEN_LPAREN;
            break;
        case ')':
            current_token.type = TOKEN_RPAREN;
            break;
        case ',':
            current_token.type = TOKEN_COMMA;
            break;
        case '<':
            c = fgetc(source);
            if (c == '>') {
                current_token.type = TOKEN_NEQ;
            } else if (c == '=') {
                current_token.type = TOKEN_LE;
            } else {
                ungetc(c, source);
                current_token.type = TOKEN_LT;
            }
            break;
        case '>':
            c = fgetc(source);
            if (c == '=') {
                current_token.type = TOKEN_GE;
            } else {
                ungetc(c, source);
                current_token.type = TOKEN_GT;
            }
            break;
        default:
            current_token.type = TOKEN_UNKNOWN;
            break;
    }

    current_token.lexeme[0] = c;
    current_token.lexeme[1] = '\0';
}

void match(TokenType expected) {
    if (current_token.type == expected) {
        next_token();
    } else {
        // fprintf(stderr, "Syntax error: expected %d, got %d\n", expected, current_token.type);
        error("Unexpected token");
        error_flag = 1;
        next_token();
    }
}

void error(const char *message) {
    fprintf(stderr, "%s\n", message);
    error_flag = 1;
}

void declare_variable(const char *name) {
    Variable *new_var = malloc(sizeof(Variable));
    strcpy(new_var->name, name);
    new_var->next = variables;
    variables = new_var;
}

int is_declared(const char *name) {
    Variable *current = variables;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void program() {
    match(TOKEN_PROGRAM);
    identifier();
    if (current_token.type == TOKEN_LPAREN) {
        match(TOKEN_LPAREN);
        parameter_list();
        match(TOKEN_RPAREN);
    }
    match(TOKEN_SEMICOLON);
    if (current_token.type == TOKEN_VAR) {
        variable_declaration();
    }
    block();
    match(TOKEN_PERIOD);

    if (current_token.type != TOKEN_EOF) {
        // error("Unexpected tokens");
    }

    if (error_flag) {
        printf("Rejeito\n");
    } else {
        printf("Aceito\n");
    }
}

void parameter_list() {
    identifier();
    while (current_token.type == TOKEN_COMMA) {
        match(TOKEN_COMMA);
        identifier();
    }
}

void variable_declaration() {
    match(TOKEN_VAR);
    while (current_token.type == TOKEN_IDENTIFIER) {
        identifier_list();
        match(TOKEN_COLON);
        if (current_token.type == TOKEN_INTEGER || current_token.type == TOKEN_STRING || current_token.type == TOKEN_BOOLEAN) {
            next_token();
        } else {
            // error("Expected type");
        }
        match(TOKEN_SEMICOLON);
    }
}

void identifier_list() {
    if (current_token.type == TOKEN_IDENTIFIER) {
        declare_variable(current_token.lexeme);
        next_token();
    }
    // identifier();
    while (current_token.type == TOKEN_COMMA) {
        match(TOKEN_COMMA);
        if (current_token.type == TOKEN_IDENTIFIER) {
            declare_variable(current_token.lexeme);
            next_token();
        }
        // identifier();
    }
}

void block() {
    match(TOKEN_BEGIN);
    statement_list();
    match(TOKEN_END);
}

void statement_list() {
    statement();
    while (current_token.type == TOKEN_SEMICOLON) {
        match(TOKEN_SEMICOLON);
        if (current_token.type == TOKEN_END) {
            break;
        } else {
            statement();
        }
    }
}

void statement() {
    switch (current_token.type) {
        case TOKEN_IDENTIFIER:
            assignment_statement();
            break;
        case TOKEN_READ:
            // match(TOKEN_READ);
            // match(TOKEN_LPAREN);
            // identifier();
            // match(TOKEN_RPAREN);
            read_statement();
            break;
        case TOKEN_READLN:
            readln_statement();
            break;
        case TOKEN_WRITE:
            write_statement();
            break;
        case TOKEN_WRITELN:
            writeln_statement();
            break;
        case TOKEN_WHILE:
            while_statement();
            break;
        case TOKEN_BEGIN:
            block();
            break;
        case TOKEN_IF:
            if_statement();
            break;
        case TOKEN_FOR:
            for_statement();
            break;
        case TOKEN_REPEAT:
            repeat_statement();
            break;
        default:
            // error("Unexpected statement");
            next_token();
            break;
    }
}

void assignment_statement() {
    if (current_token.type == TOKEN_IDENTIFIER) {
        if (!is_declared(current_token.lexeme)) {
            error("Variable not declared");
        }
        next_token();
    } else {
        error("Expected identifier");
    }
    // identifier();
    match(TOKEN_ASSIGN);
    expression();
    optional_assignment_sequence();
}

void optional_assignment_sequence() {
    while (current_token.type == TOKEN_COMMA) {
        match(TOKEN_COMMA);
        if (current_token.type == TOKEN_IDENTIFIER) {
            if (!is_declared(current_token.lexeme)) {
                error("Variable not declared 2");
            }
            next_token();
        } else {
            error("Expected identifier");
        }
        // identifier();
        match(TOKEN_ASSIGN);
        expression();
    }
}

void if_statement() {
    match(TOKEN_IF);
    expression();
    if (current_token.type == TOKEN_AND || current_token.type == TOKEN_OR || current_token.type == TOKEN_NOT) {
        next_token();
        expression();
    }
    match(TOKEN_THEN);
    statement();
    if (current_token.type == TOKEN_ELSE) {
        match(TOKEN_ELSE);
        statement();
    }
}

void for_statement() {
    match(TOKEN_FOR);
    if (current_token.type == TOKEN_IDENTIFIER) {
        if (!is_declared(current_token.lexeme)) {
            error("Variable not declared 3");
        }
        next_token();
    } else {
        error("Expected identifier");
    }
    // identifier();
    match(TOKEN_ASSIGN);
    expression();
    match(TOKEN_TO);
    expression();
    match(TOKEN_DO);
    statement();
}

void repeat_statement() {
    match(TOKEN_REPEAT);
    statement();
    if (current_token.type == TOKEN_SEMICOLON) {
        match(TOKEN_SEMICOLON);
    }
    match(TOKEN_UNTIL);
    expression();
}

void while_statement() {
    match(TOKEN_WHILE);
    expression();
    match(TOKEN_DO);
    statement();
}

void read_statement() {
    TokenType read_type = current_token.type;
    match(read_type);
    match(TOKEN_LPAREN);
    expression();
    while (current_token.type == TOKEN_COMMA) {
        match(TOKEN_COMMA);
        expression();
    }
    match(TOKEN_RPAREN);
}

void readln_statement() {
    match(TOKEN_READLN);
    match(TOKEN_LPAREN);
    expression();
    while (current_token.type == TOKEN_COMMA) {
        match(TOKEN_COMMA);
        expression();
    }
    match(TOKEN_RPAREN);
}

void write_statement() {
    TokenType write_type = current_token.type;
    match(write_type);
    match(TOKEN_LPAREN);
    expression();
    while (current_token.type == TOKEN_COMMA) {
        match(TOKEN_COMMA);
        expression();
    }
    match(TOKEN_RPAREN);
}

void writeln_statement() {
    match(TOKEN_WRITELN);
    match(TOKEN_LPAREN);
    expression();
    while (current_token.type == TOKEN_COMMA) {
        match(TOKEN_COMMA);
        expression();
    }
    match(TOKEN_RPAREN);
}

void expression() {
    simple_expression();
    if (current_token.type == TOKEN_EQ || current_token.type == TOKEN_NEQ ||
        current_token.type == TOKEN_LT || current_token.type == TOKEN_LE ||
        current_token.type == TOKEN_GT || current_token.type == TOKEN_GE) {
        relational_operator();  
        simple_expression();
    }
}

void simple_expression() {
    term();
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        next_token();
        term();
    }
}

void term() {
    factor();
    while (current_token.type == TOKEN_MULT || current_token.type == TOKEN_DIV) {
        next_token();
        factor();
    }
}

void factor() {
    switch (current_token.type) {
        case TOKEN_IDENTIFIER:
            if (!is_declared(current_token.lexeme)) {
                error("Variable not declared 4");
            }
            identifier();
            break;
        case TOKEN_NUMBER:
            number();
            break;
        case TOKEN_STRING_VALUE:
            string_value();
            break;
        case TOKEN_TRUE:
        case TOKEN_FALSE:
            next_token();
            break;
        case TOKEN_LPAREN:
            match(TOKEN_LPAREN);
            expression();
            match(TOKEN_RPAREN);
            break;
        default:
            // error("Unexpected factor");
            break;
    }
}

void string_value() {
    if (current_token.type == TOKEN_STRING_VALUE) {
        next_token();
    } else {
        // error("Expected string value");
    }
}

void relational_operator() {
    switch (current_token.type) {
        case TOKEN_EQ:
        case TOKEN_NEQ:
        case TOKEN_LT:
        case TOKEN_LE:
        case TOKEN_GT:
        case TOKEN_GE:
            next_token();
            break;
        default:
            // error("Expected relational operator");
            break;
    }
}

void identifier() {
    if (current_token.type == TOKEN_IDENTIFIER) {
        next_token();
    } else {
        // error("Expected identifier");
    }
}

void number() {
    if (current_token.type == TOKEN_NUMBER) {
        next_token();
    } else {
        // error("Expected number");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    source = fopen(argv[1], "r");
    if (!source) {
        perror(argv[1]);
        return 1;
    }

    next_token();
    program();

    fclose(source);
    return 0;
}