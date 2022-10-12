
    /*********************************************************************
       Name: Brennan Basinger
       Programming Environment: VisualStudio Code - Ubuntu 20.04.2 ARM 64
     *********************************************************************/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

using namespace std;

#include "lexer.h"
#include "productions.h"
#include "parse_tree_nodes.h"

extern "C" {
    // Instantiate global variables
    extern FILE *yyin; // input stream
    extern FILE *yyout; // output stream
    extern int yyleng; // length of current lexeme
    extern char *yytext; // text of current lexeme
    extern int yylineno; // line number for current lexeme
    extern int yylex(); // the generated lexical analyzer
}

// Needed global variables
extern int nextToken; // token returned from yylex
//void printThis();

map<string, int> symbolTable; // Symbol Table

// Formats and Prints required "***" statements
void printThis(int x)
{
    if(x==1)
        cout << endl << "*** In order traversal of parse tree ***" << endl;
    else if(x==2)
        cout << endl << endl << "*** Interpreting the TIPS program ***" << endl;
    else
        cout << endl << "*** Delete the parse tree ***" << endl;    
}

//*****************************************************************************
// The main processing loop
int main(int argc, char* argv[]) 
{
    // Set the input stream
    if (argc > 1) 
    {
        printf("INFO: Using the %s file for input\n", argv[1]);
        yyin = fopen(argv[1], "r"); 
    }
    else 
    {
        printf("INFO: Using the sample.pas file for input\n");
        yyin = fopen("sample.pas", "r");
    }
    if (!yyin) 
    {
        printf("ERROR: input file not found\n");
        return EXIT_FAILURE;
    }

    // Set the output stream
    yyout = stdout;
    nextToken = yylex();

    ProgNode* prog = new ProgNode();
   
    // Fire up the parser!
    try {     
        program(prog);  // Process <program> production

        if (nextToken != TOK_EOF)
            throw "end of file expected, but there is more here!";
        
    } catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near " << yytext << ", error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }

    // Tell the world about our success!!
    cout << endl << "=== SUCCESSFUL PARSE!!! ===" << endl;
    // Print out the symbol table
    cout << endl << "User Defined Symbols:" << endl;

    map<string, int>::iterator it_erator;
    for (it_erator = symbolTable.begin(); it_erator != symbolTable.end(); ++it_erator) 
    {
        cout << it_erator->first << endl;
    }

    printThis(1);
    cout << *prog;
    printThis(2);
    prog->interpret();
    printThis(3);

    delete prog;
    prog = nullptr;

    return EXIT_SUCCESS;
}

