//
// Created by Will on 9/28/2019.
//

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>

using namespace std;

enum token_type {NONE = -18,
                IDENT_T,
                NUMLIT_T,
                STRLIT_T,
                LISTOP_T,
                PLUS_T,
                MINUS_T,
                DIV_T,
                MULT_T,
                EQUALTO_T,
                EOF_T,
                GT_T,
                LT_T,
                GTE_T,
                LTE_T,
                LPAREN_T,
                RPAREN_T,
                SQUOTE_T,
                ERROR_T,
                NUM_TOKENS};

class LexicalAnalyzer
{
public:
    LexicalAnalyzer (char * filename);
    ~LexicalAnalyzer ();
    token_type GetToken ();
    string GetTokenName (token_type t) const;
    string GetLexeme () const;
    void ReportError (const string & msg);
private:
    ifstream input;
    ofstream listingFile;
    ofstream tokenFile;
    ofstream debugFile;
    token_type token;
    string line;
    int linenum;
    int pos;
    string lexeme;
    int errors;
    char errorChar;
};

#endif

