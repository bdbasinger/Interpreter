    /*********************************************************************
       Name: Brennan Basinger
       Programming Environment: VisualStudio Code - Ubuntu 20.04.2 ARM 64
       Purpose of File: Includes necessary functions for parsing the .pas files
     *********************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H
#include "parse_tree_nodes.h"
#include <iostream>

extern map<string, int> symbolTable;
int indentationLevel = 0;  // used to indent outputLexeme to approximate parse tree
int nextToken = 0;  // token returned from yylex

extern "C"
{
	// Instantiate global variables used by flex
	extern char* yytext;       // text of current lexeme
	extern int yylex();      // the generated lexical analyzer
}

// Forward declarations of production parsing functions
void program(ProgNode *&prog);
bool is_a_statement(Statement *&s);
bool readStatement(Statement *&s);
bool writeStatement(Statement *&s);
bool whileStatement(Statement *&s);
bool isIfStatement(Statement *&s);
bool simpleExpression(SimpleExpressionNode *&simple);
bool is_a_compound(Statement *&c);
bool term(TermNode *&t);
bool is_a_factor(FactorNode *&factor);
bool is_an_assignment(Statement *&s);
bool is_an_expression(ExprNode *&expression);
bool is_a_block(BlockNode *&b);

// Forward declarations of functions that check whether the current token is
// in the first set of a production rule
bool first_of_program();
bool firstFactor();


inline void indent()
{
    for (int x=0; x<indentationLevel; x++)
    {
        cout << ("    ");
    }
}


void printEnterAndType(string type) 
{
    indent();
    cout << "enter <" << type << ">"  << "\n";
    ++indentationLevel;
}


void outputLexeme()
{
    indent();
    cout << "-->found " << yytext << "\n";
    nextToken = yylex();
}


void printExitAndType(string type) 
{
    --indentationLevel;
    indent();
    cout << "exit <" << type << ">" << "\n";
}


bool firstFactor() 
{
    return 
        nextToken == TOK_IDENT || nextToken == TOK_INTLIT || 
        nextToken == TOK_FLOATLIT || nextToken == TOK_OPENPAREN || 
        nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

bool first_of_program(void) 
{
    return nextToken == TOK_PROGRAM;
}

// Program Method
void program(ProgNode *&p) 
{
    if (!first_of_program()) // Check for PROGRAM
    {
        throw "3: 'PROGRAM' expected";
    }
    indent();
    cout << "enter <program>" << "\n";
    delete p;
    ProgNode* program = new ProgNode();
    ++indentationLevel;
    // TODO: finish this parsing function...
    outputLexeme();
    if (nextToken == TOK_IDENT) 
    {
        program->n = new string(yytext);
        outputLexeme();
        if(nextToken == TOK_SEMICOLON) 
        {
            outputLexeme();

            if(!is_a_block(program->blockNode))
            {
                return;
            }
        }
        else
        {
            throw "14: ';' expected";
        }
    }
    else
    {
        throw "2: identifier expected";
    }
    // Remaining Starting Point Code for program() 
    --indentationLevel;
    indent();
    cout << "exit <program>" << "\n";
    p = program;
}


// Method to check statement
bool is_a_statement(Statement *&s) 
{
    printEnterAndType("statement");
    bool is_correct;
    switch(nextToken) 
    {
        case TOK_IDENT:
            is_correct = is_an_assignment(s);
            break;
        case TOK_READ:
            is_correct = readStatement(s);
            break;
        case TOK_WRITE:
            is_correct = writeStatement(s);
            break;
        case TOK_WHILE:
            is_correct = whileStatement(s);
            break;
        case TOK_IF:
            is_correct = isIfStatement(s);
            break;
        case TOK_BEGIN:
            is_correct = is_a_compound(s);
            break;
        default:
            throw "900: illegal type of statement";
            break;
    }
    if(is_correct == false)
    {
        return false;
    }
    printExitAndType("statement");
    return is_correct;
}


// Read Statement Method
bool readStatement(Statement *&s) 
{
    delete s;
    Read* readObject = new Read();
    bool val;
    if(nextToken == TOK_READ) 
    {
        printEnterAndType("read");
        outputLexeme();
        if(nextToken == TOK_OPENPAREN) 
        {
            outputLexeme();
            if(nextToken == TOK_IDENT) 
            {
                readObject->id = new string(yytext);
                outputLexeme();
                if(nextToken == TOK_CLOSEPAREN) 
                {
                    outputLexeme();
                    val = true;
                }
                else
                {
                    throw "4: ')' expected";
                }
            }
            else
            {
                throw "2: identifier expected";
            }
        }
        else
        {
            throw "9: '(' expected";
        }  
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("read");
    s = readObject;
    return val;
}


// Write Statement Method
bool writeStatement(Statement *&s) 
{
    delete s;
    Write* writeObject = new Write();
    bool val;
    if(nextToken == TOK_WRITE) 
    {
        printEnterAndType("write");
        outputLexeme();
        if(nextToken == TOK_OPENPAREN) 
        {
            outputLexeme();
            if(nextToken == TOK_STRINGLIT ||nextToken == TOK_IDENT) 
            {
                writeObject->id = new string(yytext);
                if(nextToken == TOK_IDENT)
                {
                    writeObject->tru = false;
                }
                else
                {
                    writeObject->tru = true;
                }
                outputLexeme();
                
                if(nextToken == TOK_CLOSEPAREN) 
                {
                    outputLexeme();
                    val = true;
                }
                else
                {
                    throw "4: ')' expected";
                }
            }
            else
            {
                throw "134: illegal type of operand(s)";
            }
        }
        else
        {
            throw "9: '(' expected";
        }
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("write");
    s = writeObject;
    return val;
}


// While Statement Method
bool whileStatement(Statement *&s) 
{
    delete s;
    While* _while = new While();
    ExprNode* expression = new ExprNode();
    Statement* bodyOfLoop = nullptr;
    bool val;

    if(nextToken == TOK_WHILE) 
    {
        printEnterAndType("while statement");
        outputLexeme();

        if(is_an_expression(expression)) 
        {
            if(is_a_statement(bodyOfLoop))
            {
                val = true;
            }
            else
            {
                val = false;
            }
        }
        else
        {
            val = false;
        }
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("while statement");
    _while->bodyOfLoop = bodyOfLoop;
    _while->expression = expression;
    s = _while;
    return val;
}

// If Statement Method
bool isIfStatement(Statement *&s) 
{
    bool val;
    delete s;
    If* if_s = new If();
    ExprNode* expression = new ExprNode();
	Statement* _else = nullptr;
	Statement* _then = nullptr;

    if(nextToken == TOK_IF) 
    {
        printEnterAndType("if statement");
        outputLexeme();
        if(is_an_expression(expression)) 
        {
            if(nextToken == TOK_THEN) 
            {
                outputLexeme();
                if(is_a_statement(_then)) 
                {
                    if(nextToken == TOK_ELSE) 
                    {
                        outputLexeme();
                        if(is_a_statement(_else))
                        {
                            if_s->_else_is_here = true;
                            val = true;
                        }
                        else
                        {
                            if_s->_else_is_here = false;
                        }
                    }
                    else
                    {
                        val = true;
                    }
                }
                else
                {
                    val = false;
                }
            }
            else
            {
                throw "52: 'THEN' expected";
            }     
        }
        else
        {
            val = false;
        }
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("if statement");
    if_s->expression = expression;
    if_s->_else = _else;
    if_s->_then = _then;
    s = if_s;
    return val;
}


// Simple Expression Method
bool simpleExpression(SimpleExpressionNode *&simple) 
{
    bool val;
    printEnterAndType("simple expression");
    delete simple;
    simple = new SimpleExpressionNode();
    if(!firstFactor())
    {
        throw "901: illegal type of simple expression";
    }
    if(term(simple->_1stTerm)) 
    {
        if(nextToken == TOK_OR || nextToken == TOK_PLUS || nextToken == TOK_MINUS) 
        {
            simple->restTermOps.push_back(nextToken);
            outputLexeme();
            simple->restTerms.push_back(new TermNode());
            if(term(simple->restTerms.back()))
            {
                val = true;
            }
            else
            {
                val = false;
            }
        }
        else
        {
            val = true;
        }
    }
    else
    {
        val = false;
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("simple expression");
    return val;
}


// Compound Statement Method
bool is_a_compound(Statement *&s) 
{
    bool val;
    delete s;
    CS_Node* compound = new CS_Node();
    if(nextToken == TOK_BEGIN) 
    {
        printEnterAndType("compound_statement");
        outputLexeme();
        compound->_s.push_back(nullptr);

        if(is_a_statement(compound->_s.back())) 
        {
            while(nextToken == TOK_SEMICOLON) 
            {
                outputLexeme();
                compound->_s.push_back(nullptr);

                if(!is_a_statement(compound->_s.back())) 
                {
                    val = false;
                    break;
                }
            }
            if(nextToken == TOK_END) 
            {
                outputLexeme();
                val = true;
            }
            else
            {
                throw "13: 'END' expected";
            }        
        }
        else
        {
            val = false;
        }      
    }
    else
    {
        throw "17: 'BEGIN' expected";
    }
    if (val == false)
    {
        return false;
    }
    printExitAndType("compound_statement");
    s = compound;
    return val;
}


// Term Method
bool term(TermNode *&t) 
{
    bool val;
    printEnterAndType("term");
    delete t;
    t = new TermNode();
    if(!firstFactor())
    {
        throw "902: illegal type of term";
    }
    if(is_a_factor(t->_1stFactor)) 
    {
        if(nextToken == TOK_AND || nextToken == TOK_DIVIDE || nextToken == TOK_MULTIPLY) 
        {
            do 
            {
                t->restFactorOps.push_back(nextToken);
                outputLexeme();
                t->restFactors.push_back(nullptr);

                if(is_a_factor(t->restFactors.back())) 
                {
                    val = true;
                }
                else
                {
                    val = false;
                }
            } 
            while(nextToken == TOK_AND || nextToken == TOK_DIVIDE || nextToken == TOK_MULTIPLY);
        }
        else
        {
            val = true;
        }
    }
    else
    {
        val = false;
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("term");
    return val;
}


// Factor Method
bool is_a_factor(FactorNode *&factor) 
{
    printEnterAndType("factor");
    //delete factor;
    bool val;
    if(!firstFactor())
    {
        throw "903: illegal type of factor";
    }
    if(nextToken == TOK_INTLIT)
    {
        factor = new IntLitNode(yytext);
        outputLexeme();
        val = true;
    }
    else if(nextToken == TOK_FLOATLIT) 
    {
        factor = new FloatLitNode(yytext);
        outputLexeme();
        val = true;
    }
    else if(symbolTable.find(yytext) != symbolTable.end() && nextToken == TOK_IDENT) 
    {
        factor = new IdNode(yytext);
        outputLexeme();
        val = true;
    }
    else if(nextToken == TOK_IDENT) 
    {
        throw "104: identifier not declared";
    }
    else if(nextToken == TOK_OPENPAREN) 
    {
        outputLexeme();
        ExprNode* expr = new ExprNode();
        if(is_an_expression(expr)) 
        {
            NestedExprNode* nestedNode = new NestedExprNode(expr);
            factor = nestedNode;
            if(nextToken == TOK_CLOSEPAREN) 
            {
                outputLexeme();
                val = true;
            }
            else
            {
                throw "4: ')' expected";
            }
        }
        else
        {
            val = false;
        }
    }
    else if(nextToken == TOK_MINUS || nextToken == TOK_NOT) 
    {
        bool t = nextToken == TOK_MINUS;
        outputLexeme();
        FactorNode* fact = new IdNode("x");
        if(is_a_factor(fact))
        {
            factor = new NoFactor(fact, t);
            val = true;
        }
        else
        {
            val = false;
        }
    }
    else
    {
        val = false;
    }

    if(val == false)
    {
        return false;
    }
    printExitAndType("factor");
    return val;
}


// Assignment Method
bool is_an_assignment(Statement *&s) 
{
    bool val;
    delete s;
    Assignment_S* assign = new Assignment_S();
    if(symbolTable.find(yytext) != symbolTable.end() && nextToken == TOK_IDENT) 
    {
        printEnterAndType("assignment");
        assign->n = new string(yytext);
        outputLexeme();
        if(nextToken == TOK_ASSIGN) 
        {
            outputLexeme();
            if(is_an_expression(assign->expression))
            {
                val = true;
            }
            else
            {
                val = false;
            }
        }
        else
        {
            throw "51: ':=' expected";
        }   
    }
    else if(nextToken == TOK_IDENT)
    {
        printEnterAndType("assignment");
        throw "104: identifier not declared";
    }
    else
    {
        throw "2: identifier expected";
    }
    if(val == false)
    {
        return false;
    }
    printExitAndType("assignment");
    s = assign;
    return val;
}


// Expression Method
bool is_an_expression(ExprNode *&expression) 
{
    printEnterAndType("expression");
    delete expression;
    expression = new ExprNode();
    if(!firstFactor())
    {
        throw "144: illegal type of expression";
    }
    bool val;
    if(simpleExpression(expression->_1stSimple))
    {
        if(nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_EQUALTO || nextToken == TOK_NOTEQUALTO) 
        {
            expression->restTermOps.push_back(nextToken);
            outputLexeme();
            expression->restOfSimpleTerms.push_back(new SimpleExpressionNode());
            if(simpleExpression(expression->restOfSimpleTerms.back())) 
            {
                val = true;
            }
            else
            {
                val = false;
            }
        }
        else
        {
            val = true;
        }
    }
    else
    {
        val = false;
    }

    if(val == false)
    {
        return false;
    }
    printExitAndType("expression");
    return val;
}


// Block Method
bool is_a_block(BlockNode *&block) 
{
    printEnterAndType("block");
    bool val;
    delete block;
    block = new BlockNode();
    if(nextToken != TOK_BEGIN && nextToken != TOK_VAR)
    {
        throw "18: error in declaration part OR 17: 'BEGIN' expected";
    }
    if(nextToken == TOK_VAR) 
    {
        outputLexeme();
        if(nextToken == TOK_IDENT) 
        {
            do 
            {
                if(symbolTable.end() == symbolTable.find(yytext))
                {
                    symbolTable.insert({yytext, 0});
                }
                else
                {
                    throw "101: identifier declared twice";
                }
                outputLexeme();
                if(nextToken == TOK_COLON) 
                {
                    outputLexeme();
                    if(nextToken == TOK_REAL || nextToken == TOK_INTEGER) 
                    {
                        outputLexeme();
                        if(nextToken == TOK_SEMICOLON) 
                        {
                            outputLexeme();
                        }
                        else 
                        {
                            throw "14: ';' expected";
                        }
                    }
                    else 
                    {
                        throw "10: error in type";
                    }
                }
                else 
                {
                    throw "5: ':' expected";
                    break;
                }
                cout << endl;
            } while(nextToken == TOK_IDENT);
        }
        else
        {
            throw "2: identifier expected";
        }
    }
    Statement* compound = nullptr;
    if(is_a_compound(compound)) 
    {
        val = true;
    }
    else
    {
        val = false;
    }

    if(val == false)
    {
        return false;
    }
    printExitAndType("block");
    block->compound = compound;
    return true;
}




#endif




