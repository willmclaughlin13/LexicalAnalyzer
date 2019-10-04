#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = {	"IDENT_T", "NUMLIT_T", "STRLIT_T", "LISTOP_T", "CONS_T", "IF_T",
                                "COND_T", "ELSE_T", "DISPLAY_T", "NEWLINE_T", "AND_T", "OR_T",
                                "NOT_T", "DEFINE_T", "NUMBERP_T", "LISTP_T", "ZEROP_T", "NULLP_T",
                                "STRLIT_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T", "MODULO_T",
                                "ROUND_T", "EQUALTO_T", "GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T",
                                "RPAREN_T", "SQUOTE_T", "ERROR_T", "EOF_T"};

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
    // This function will initialize the lexical analyzer class

    string rootFileName = filename;
    for (int i = 0; i < 3; ++i) {
        rootFileName.pop_back();
    }
    this->input.open(filename);
    this->listingFile.open(rootFileName + ".lst", ofstream::out | ofstream::trunc);
    this->debugFile.open(rootFileName + ".dbg", ofstream::out | ofstream::trunc);
    this->tokenFile.open(rootFileName + ".p1", ofstream::out | ofstream::trunc);
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
    // This function will complete the execution of the lexical analyzer class
    this->input.close();
    this->listingFile.close();
    this->debugFile.close();
    this->tokenFile.close();
}

token_type LexicalAnalyzer::GetToken ()
{
    // This function will find the next lexeme int the input file and return
    // the token_type value associated with that lexeme
    int state = 1;
    int table[][21] = {
            {3, 2, 4, DIV_T, MULT_T, 7, 8, EQUALTO_T, LPAREN_T, RPAREN_T , SQUOTE_T, 9, 12, 12, 12, 12, 5, 1, ERROR_T, 15, ERROR_T},
            {MINUS_T, MINUS_T, 6, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, 2, MINUS_T, MINUS_T, MINUS_T, MINUS_T},
            {PLUS_T, PLUS_T, 6, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, 3, PLUS_T, PLUS_T, PLUS_T, PLUS_T},
            {ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, 6, ERROR_T, ERROR_T, ERROR_T, ERROR_T},
            {NUMLIT_T, NUMLIT_T, 6, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T},
            {NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, 6, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T},
            {GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GTE_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T},
            {LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LTE_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, 12, 10, 11, 12, 12, 12, IDENT_T, 12, IDENT_T, IDENT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, 12, 12, 10, 14, 12, 12, IDENT_T, 12, IDENT_T, IDENT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, 12, 12, 11, 14, 12, 12, IDENT_T, 12, IDENT_T, IDENT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, 12, 12, 12, 12, 12, 13, IDENT_T, 13, IDENT_T, IDENT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, 13, 13, 13, 13, 13, 13, IDENT_T, 13, IDENT_T, IDENT_T},
            {LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, 12, 12, 12, 12, 12, 12, LISTOP_T, 12, LISTOP_T, LISTOP_T},
            {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, STRLIT_T, 15}
    };

    //while(!this->input.eof()) {
    char c;
    int input = 0;
    string word;
    while(this->input.get(c)) {
        if (c == '\n')
            this->input.get(c);
        state--; // Because our states start at 1, but C++ starts at 0
        word += c;
        bool other = false;
        //cout << c;
        switch(c) {
            case '+':
                input = 0;break;
            case '-':
                input = 1;break;
            case '.':
                input = 2;break;
            case '/':
                input = 3;break;
            case '*':
                input = 4;break;
            case '>':
                input = 5;break;
            case '<':
                input = 6;break;
            case '=':
                input = 7;break;
            case '(':
                input = 8;break;
            case ')':
                input = 9;break;
            case '\'':
                input = 10;break;
            case 'c':
                input = 11;break;
            case 'a':
                input = 12;break;
            case 'd':
                input = 13;break;
            case 'r':
                input = 14;break;
            case ' ':
                input = 17;break;
            case '_':
                input = 18;break;
            case '"':
                input = 19;break;
            default:
                other = true;break;
        }
        if (other) {
            if (isalpha(c))
                input = 15;
            else if (isdigit(c))
                input = 16;
            else
                input = 20;
        }
        state = table[state][input];
        if (state <= 0) {
            cout << token_names[state+35] << '\t' << word << endl;
            state = 1;
            word.clear();
        }
    }
    //token = state;
    //cout << "State: " << state << endl;

    //token = table[this->linenum][this->pos];
    return token;
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
    // The GetTokenName function returns a string containing the name of the
    // token passed to it.
    return "";
}

string LexicalAnalyzer::GetLexeme () const
{
    // This function will return the lexeme found by the most recent call to
    // the get_token function
    return "";
}

void LexicalAnalyzer::ReportError (const string & msg)
{
    // This function will be called to write an error message to a file
}


