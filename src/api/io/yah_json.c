#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum 
{
    TOKEN_LBRACE,    // {
    TOKEN_RBRACE,    // }
    TOKEN_LBRACKET,  // [
    TOKEN_RBRACKET,  // ]
    TOKEN_COLON,     // :
    TOKEN_COMMA,     // ,
    TOKEN_STRING,    // "hola"
    TOKEN_NUMBER,    // 123.45
    TOKEN_TRUE,      // true
    TOKEN_FALSE,     // false
    TOKEN_NULL,      // null
    TOKEN_EOF,       // Fin del archivo/string
    TOKEN_ERROR      // Algo salió mal
}JSON_TOKEN_TYPE;

typedef struct 
{
    JSON_TOKEN_TYPE type;
    const char *start;
    int length;        
}JSON_TOKEN;

typedef struct 
{
    const char *cursor; // El puntero que va avanzando por el JSON
    int line;           // Para decir donde esta el error del archivo   
}LEXER;

static char *yahReadFile(const char *path) 
{
    FILE *file = fopen(path, "rb");
    if (!file) 
        return NULL;

    // Saltamos al final del archivo para obtener su tamaño en bytes
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET); // Regresamos el cursor al inicio

    // Reservamos memoria exacta (+1 para el terminador nulo de string)
    char *buffer = (char *)malloc(length + 1);
    if (!buffer) 
    {
        fclose(file);
        return NULL;
    }

    // Leemos el archivo entero directo a la RAM (literalmente un bufferedReader de java o bueno, mas o menos)
    fread(buffer, 1, length, file);
    buffer[length] = '\0'; // Sellamos el arreglo para que sea un string válido en C    

    fclose(file);
    return buffer;
}

static LEXER *createLexer(const char *cursor)
{
    LEXER new = (LEXER*)malloc(sizeof(LEXER));

    if(!new)
        return NULL;

    new->cursor = cursor;
    new->line = 1;

    return new;
}

static void skipWhitespace(LEXER *lex)
{
    while (*lex->cursor == ' ' || *lex->cursor == '\t' || *lex->cursor == '\r' || *lex->cursor == '\n')
    {
        if (*lex->cursor == '\n') 
        {
            lex->line++;
        }
        lex->cursor++;
    }
}

static JSON_TOKEN nextToken(LEXER *lex)
{   
    JSON_TOKEN token;

    skipWhiteSpace(lex);

    if(!lex)
    {
        token.type = TOKEN_ERROR;
        token.start = NULL;
        token.length = 0;
        return token;
    }

    token.start = lex->cursor;
    token.length = 1;

    char currentToken = *(lex->cursor);

    switch(currentToken)
    {
        case '\0':
            token.type = TOKEN_EOF;
            break;
        case '{':
            token.type = TOKEN_LBRACE;
            break;
        case '}':
            token.type = TOKEN_RBRACE;
            break;
        case '[':
            token.type = TOKEN_LBRACKET;
            break;
        case ']':
            token.type = TOKEN_RBRACKET;
            break;
        case ':':
            token.type = TOKEN_COLON;
            break;
        case ',':
            token.type = TOKEN_COMMA;
            break;
        case '\"':
            token.type = TOKEN_STRING;
            lex->cursor++;
            token.start = lex->cursor;
            while(*lex->cursor != '\"' && *lex->cursor != '\0') 
            {
                lex->cursor++;
            }
            token.length = (int)(lex->cursor - token.start);

            if (*lex->cursor == '\"') 
            {
                lex->cursor++; 
            }
            return token;
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            token.type = TOKEN_NUMBER;

            while (isdigit(*lex->cursor) || *lex->cursor == '.' || *lex->cursor == '-')
            {
                lex->cursor++;
            }

            token.length = (int)(lex->cursor - token.start);
            return token;
        
        default:
            token.type = TOKEN_ERROR;
            lex->cursor++;
            return token;
        }

    lex->cursor++; // preparing the cursor for the next call
    return token;
}

