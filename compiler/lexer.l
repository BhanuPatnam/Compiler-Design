%{
#include "ast.h"
#include "y.tab.h"
#include <string.h>
#include <stdlib.h>

extern int current_line;
%}

%option noyywrap

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          ({LETTER}|_)(({LETTER}|{DIGIT}|_)*)
NUMBER      {DIGIT}+

%%

[ \t\r]+    { /* skip whitespace */ }
\n          { current_line++; }

"if"        { return TOK_IF; }
"then"      { return TOK_THEN; }
"else"      { return TOK_ELSE; }
"end"       { return TOK_END; }
"for"       { return TOK_FOR; }
"to"        { return TOK_TO; }
"while"     { return TOK_WHILE; }
"do"        { return TOK_DO; }
"function"  { return TOK_FUNCTION; }
"struct"    { return TOK_STRUCT; }
"print"     { return TOK_PRINT; }
"return"    { return TOK_RETURN; }

"<-"        { return TOK_ASSIGN; }
"->"        { return TOK_ASSIGN; }
"←"         { return TOK_ASSIGN; }

"."         { return TOK_DOT; }
"+"         { return TOK_PLUS; }
"-"         { return TOK_MINUS; }
"*"         { return TOK_STAR; }
"/"         { return TOK_SLASH; }
"("         { return TOK_LPAREN; }
")"         { return TOK_RPAREN; }
","         { return TOK_COMMA; }
"="         { return TOK_EQ; }
"≠"         { return TOK_NEQ; }
"<"         { return TOK_LT; }
">"         { return TOK_GT; }
"≤"         { return TOK_LE; }
"≥"         { return TOK_GE; }

"&"         { return TOK_ADDR_OF; }
"["         { return TOK_LBRACKET; }
"]"         { return TOK_RBRACKET; }
"int"       { return TOK_INT_TYPE; }
"float"     { return TOK_FLOAT_TYPE; }
"char"      { return TOK_CHAR_TYPE; }

{ID}        { yylval.id = strdup(yytext); return TOK_IDENTIFIER; }
{NUMBER}    { yylval.id = strdup(yytext); return TOK_INT_LITERAL; }
{NUMBER}\.{DIGIT}+ { yylval.id = strdup(yytext); return TOK_FLOAT_LITERAL; }
"'"."'"     { yylval.id = strdup(yytext); return TOK_CHAR_LITERAL; }

.           { return TOK_UNKNOWN; }

%%

void lexer_reset() {
    if (yyin) {
        rewind(yyin);
        yyrestart(yyin);
    }
}
