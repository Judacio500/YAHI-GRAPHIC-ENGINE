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
    TOKEN_STRING,    // "hello"
    TOKEN_NUMBER,    // 123.45
    TOKEN_TRUE,      // true
    TOKEN_FALSE,     // false
    TOKEN_NULL,      // null
    TOKEN_EOF,       // End Of File/String
    TOKEN_ERROR      // Error marker
}JSON_TOKEN_TYPE;

typedef struct 
{
    JSON_TOKEN_TYPE type;
    const char *start;
    int length;        
}JSON_TOKEN;

typedef struct 
{
    const char *cursor; // The character being analyzed
    int line;           // To print accurate errors  
}LEXER;

static char *yahReadFile(const char *path) 
{
    FILE *file = fopen(path, "rb");
    if (!file) 
        return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET); 

    char *buffer = (char *)malloc(length + 1);
    if (!buffer) 
    {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

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

    skipWhitespace(lex);

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

YAH_DICT *yahParseJson(const char *path)
{
    char *doc = yahReadFile(path);
    if (!doc) return NULL;

    LEXER lex;
    lex.cursor = doc;
    lex.line = 1;

    TOKEN t = nextToken(&lex);

    YAH_DICT *rootDict = NULL;

    if (t.type == TOKEN_LBRACE)
    {
        rootDict = parseDict(&lex);
    } 
    else 
    {
        printf("ERROR AT LINE %d: EXPECTED '{', GOT '%.*s'\n", lex.line, t.length, t.start);
    }

    free(doc);
    return rootDict;
}

static YAH_DICT *parseDict(LEXER *lex)
{
    YAH_DICT *dict = createDict(10);
    if(!dict)
        return NULL;

    TOKEN t;

    while((t = nextToken(lex)).type != TOKEN_EOF)
    {
        if(t.type == TOKEN_RBRACE)
            return dict;

    
        if(t.type != TOKEN_STRING)
        {
            printf("ERROR AT LINE %d: EXPECTED STRING GOT '%.*s'\n", lex.line, t.length, t.start);
            return NULL;
        }

        uint32_t keyHash = yahHashStringLength(t.start, t.length);

        t = nextToken(lex);
        if(t.type != TOKEN_COLON)
        {
            printf("ERROR AT LINE %d: EXPECTED ':' GOT '%.*s'\n", lex.line, t.length, t.start);
            return NULL;
        }

        YAH_VALUE val = parseValue(lex);

        yahDictSet(dict, keyHash, val);

        t = nextToken(lex);
        if(t.type == TOKEN_RBRACE)
        {
            return dict;
        }
        else if(t.type != TOKEN_COMMA)
        {
            printf("ERROR AT LINE %d: EXPECTED ',' GOT '%.*s'\n", lex.line, t.length, t.start);
            return NULL;
        }
    }

    return dict;
}

static YAH_VALUE parseValue(LEXER *lex)
{
    TOKEN t = nextToken(lex);
    YAH_VALUE val;

    switch(t.type)
    {
        case TOKEN_NUMBER:
            val = yahValueFromFloat(strtof(t.start,NULL));
            break;
        case TOKEN_STRING:
            val = yahValueFromHash(yahHashStringLength(t.start,t.length));
            break;
        case TOKEN_LBRACE:
            YAH_DICT *subDict = parseDict(lex);
            val = yahValueFromDict(subDict);
        case TOKEN_LBRACKET:
            val = parseArray(lex);
            break;
        default:
            printf("ERROR AT LINE %d: UNEXPECTED VALUE TOKEN '%.*s'\n", lex.line, t.length, t.start);
            val.type = YAH_TYPE_NIL;
            break;
    }

    return val;
}

static YAH_VALUE parseArray(LEXER *lex)
{
    YAH_VALUE stackBuffer[256];
    int count = 0;

    while (1)
    {
        const char *savedCursor = lex->cursor;
        int savedLine = lex->line;
        
        TOKEN t = nextToken(lex);

        if (t.type == TOKEN_RBRACKET) 
        {
            break; 
        }
        else if (t.type == TOKEN_EOF)
        {
            printf("ERROR AT LINE %d: UNEXPECTED END OF FILE IN ARRAY\n", lex->line);
            break;
        }

        lex->cursor = savedCursor;
        lex->line = savedLine;

        YAH_VALUE val = parseValue(lex);
        
        if (count < 256) 
        {
            stackBuffer[count++] = val;
        }
        else
        {
            printf("WARNING AT LINE %d: ARRAY EXCEEDS STACK LIMIT (256)\n", lex->line);
        }

        t = nextToken(lex);
        if (t.type == TOKEN_RBRACKET)
        {
            break;
        }
        else if (t.type != TOKEN_COMMA)
        {
            printf("ERROR AT LINE %d: EXPECTED ',' OR ']', GOT '%.*s'\n", lex->line, t.length, t.start);
            break;
        }
    }
    if (count == 4 &&
        (stackBuffer[0].type == YAH_TYPE_FLOAT || stackBuffer[0].type == YAH_TYPE_INT) &&
        (stackBuffer[1].type == YAH_TYPE_FLOAT || stackBuffer[1].type == YAH_TYPE_INT) &&
        (stackBuffer[2].type == YAH_TYPE_FLOAT || stackBuffer[2].type == YAH_TYPE_INT) &&
        (stackBuffer[3].type == YAH_TYPE_FLOAT || stackBuffer[3].type == YAH_TYPE_INT))
    {
        YAH_VALUE vecVal;
        vecVal.type = YAH_TYPE_VEC4;
        
        vecVal.data.vec[0] = (stackBuffer[0].type == YAH_TYPE_FLOAT) ? stackBuffer[0].data.f : (float)stackBuffer[0].data.i;
        vecVal.data.vec[1] = (stackBuffer[1].type == YAH_TYPE_FLOAT) ? stackBuffer[1].data.f : (float)stackBuffer[1].data.i;
        vecVal.data.vec[2] = (stackBuffer[2].type == YAH_TYPE_FLOAT) ? stackBuffer[2].data.f : (float)stackBuffer[2].data.i;
        vecVal.data.vec[3] = (stackBuffer[3].type == YAH_TYPE_FLOAT) ? stackBuffer[3].data.f : (float)stackBuffer[3].data.i;
        
        return vecVal;
    }

    YAH_VALUE *finalItems = NULL;
    if (count > 0)
    {
        finalItems = (YAH_VALUE *)malloc(sizeof(YAH_VALUE) * count);
        memcpy(finalItems, stackBuffer, sizeof(YAH_VALUE) * count);
    }

    YAH_VALUE arrayVal;
    arrayVal.type = YAH_TYPE_ARRAY; 
    arrayVal.data.array.items = finalItems; // Tu puntero a la union/struct interno
    arrayVal.data.array.count = count;

    return arrayVal;
}