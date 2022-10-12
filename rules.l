
    /*********************************************************************
       Name: Brennan Basinger
       Programming Environment: VisualStudio Code - MacOS
     *********************************************************************/

%option yylineno
%option noyywrap
%{
#include "lexer.h"

int num_lines = 1;
%}


%%

    /* Keyword Lexemes */
"BEGIN"             {return TOK_BEGIN;}
"BREAK"             {return TOK_BREAK;}
"CONTINUE"          {return TOK_CONTINUE;}
"DOWNTO"            {return TOK_DOWNTO;}
"ELSE"              {return TOK_ELSE;}  
"END"               {return TOK_END;}
"FOR"               {return TOK_FOR;}
"IF"                {return TOK_IF;} 
"LET"               {return TOK_LET;}  
"PROGRAM"           {return TOK_PROGRAM;}
"READ"              {return TOK_READ;}
"THEN"              {return TOK_THEN;}
"TO"                {return TOK_TO;}   
"VAR"               {return TOK_VAR;}
"WHILE"             {return TOK_WHILE;}
"WRITE"             {return TOK_WRITE;} 

    /* Datatype Specifier Lexemes */
"REAL"              {return TOK_REAL;}
"INTEGER"           {return TOK_INTEGER;}

    /* Operator Lexemes */
"+"                 {return TOK_PLUS;}
"-"                 {return TOK_MINUS;}
"*"                 {return TOK_MULTIPLY;}
"/"                 {return TOK_DIVIDE;}
":="                {return TOK_ASSIGN;}
"="                 {return TOK_EQUALTO;}
"<"                 {return TOK_LESSTHAN;}
">"                 {return TOK_GREATERTHAN;}
"<>"                {return TOK_NOTEQUALTO;}
"MOD"               {return TOK_MOD;}
"NOT"               {return TOK_NOT;}
"OR"                {return TOK_OR;}
"AND"               {return TOK_AND;}

    /* Punctuation Lexemes */
";"                 {return TOK_SEMICOLON;}
":"                 {return TOK_COLON;}
"("                 {return TOK_OPENPAREN;}
")"                 {return TOK_CLOSEPAREN;}	
"{"                 {return TOK_OPENBRACE;}
"}"                 {return TOK_CLOSEBRACE;}

    /* Useful Abstraction Lexemes */
[0-9]+              { return TOK_INTLIT; }      
[0-9]+"."[0-9]+     { return TOK_FLOATLIT; }
[A-Z][0-9A-Z]{0,7}  { return TOK_IDENT; } 
'[^'^\n]{0,78}'     { return TOK_STRINGLIT; }
[\n]                { num_lines++; }
[ \t\r]+

    /* Found an unknown character */
.                   { return TOK_UNKNOWN; }
[A-Z][0-9A-Z]{8,}   { return TOK_UNKNOWN; }
'[^']{79,}'         { return TOK_UNKNOWN; }

    /* Recognize end of file */
<<EOF>>   { return TOK_EOF; }




