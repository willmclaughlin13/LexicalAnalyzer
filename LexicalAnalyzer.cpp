#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = {	"IDENT_T", "NUMLIT_T", "STRLIT_T", "LISTOP_T", "CONS_T", "IF_T",
                                "COND_T", "ELSE_T", "DISPLAY_T", "NEWLINE_T", "AND_T", "OR_T",
                                "NOT_T", "DEFINE_T", "NUMBERP_T", "LISTP_T", "ZEROP_T", "NULLP_T",
                                "STRINGP_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T", "MODULO_T",
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
}

token_type LexicalAnalyzer::GetToken ()
{
    // This function will find the next lexeme int the input file and return
    // the token_type value associated with that lexeme
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


