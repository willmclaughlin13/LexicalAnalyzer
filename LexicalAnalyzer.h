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
                STATE_1,
                STATE_2,
                STATE_3,
                STATE_4,
                STATE_5,
                STATE_6,
                STATE_7,
                STATE_8,
                STATE_9,
                STATE_10,
                STATE_11,
                STATE_12,
                STATE_13,
                STATE_14,
                STATE_15,
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
    bool err;
    int linenum;
    int pos;
    int errPos;
    int tState;
    string lexeme;
    int errors;
    char errorChar;
};

#endif

