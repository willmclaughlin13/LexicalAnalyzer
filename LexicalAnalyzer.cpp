#include <iomanip>
#include "LexicalAnalyzer.h"
#include <map>
#include <algorithm>

using namespace std;

static string token_names[] = {	"IDENT_T", "NUMLIT_T", "STRLIT_T", "LISTOP_T", "PLUS_T",
                                   "MINUS_T", "DIV_T", "MULT_T", "EQUALTO_T", "EOF_T",
                                   "GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T", "RPAREN_T",
                                   "SQUOTE_T", "ERROR_T"};

map<string, string> keyNames;

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
    // This function will initialize the lexical analyzer class
    token = NONE;
    linenum = 0;
    pos = 0;
    errors = 0;
    tState = 0;
    err = false;


    string rootFileName = filename;
    for (int i = 0; i < 3; ++i) {
        rootFileName.pop_back();
    }
    input.open(filename);
    listingFile.open(rootFileName + ".lst", ofstream::out | ofstream::trunc);
    debugFile.open(rootFileName + ".dbg", ofstream::out | ofstream::trunc);
    tokenFile.open(rootFileName + ".p1", ofstream::out | ofstream::trunc);
    listingFile << "Input file: " << rootFileName + ".ss" << endl;

    getline(input, line);
    line += " \n";

    //linenum++;
    //listingFile << setw(5) << linenum << ": " << line;

    keyNames.insert(pair<string, string >("cons", "CONST_T"));
    keyNames.insert(pair<string, string>("if", "IF_T"));
    keyNames.insert(pair<string, string>("cond", "COND_T"));
    keyNames.insert(pair<string, string>("else", "ELSE_T"));
    keyNames.insert(pair<string, string>("display", "DISPLAY_T"));
    keyNames.insert(pair<string, string>("newline", "NEWLINE_T"));
    keyNames.insert(pair<string, string>("and", "AND_T"));
    keyNames.insert(pair<string, string>("or", "OR_T"));
    keyNames.insert(pair<string, string>("not", "NOT_T"));
    keyNames.insert(pair<string, string>("define", "DEFINE_T"));
    keyNames.insert(pair<string, string>("number?", "NUMBERP_T"));
    keyNames.insert(pair<string, string>("list?", "LISTP_T"));
    keyNames.insert(pair<string, string>("zero?", "ZEROP_T"));
    keyNames.insert(pair<string, string>("null?", "NULLP_T"));
    keyNames.insert(pair<string, string>("string?", "STRINGP_T"));
    keyNames.insert(pair<string, string>("modulo", "MODULO_T"));
    keyNames.insert(pair<string, string>("round", "ROUND_T"));
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
    // This function will complete the execution of the lexical analyzer class
    input.close();
    listingFile.close();
    debugFile.close();
    tokenFile.close();
}

token_type LexicalAnalyzer::GetToken ()
{
    // This function will find the next lexeme int the input file and return
    // the token_type value associated with that lexeme
    int state = 1;
    token_type table[][21] = {
            {STATE_3, STATE_2, STATE_4, DIV_T, MULT_T, STATE_7, STATE_8, EQUALTO_T, LPAREN_T, RPAREN_T , SQUOTE_T, STATE_9, STATE_12, STATE_12, STATE_12, STATE_12, STATE_5, STATE_1, ERROR_T, STATE_15, ERROR_T},
            {MINUS_T, MINUS_T, STATE_6, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, MINUS_T, STATE_2, MINUS_T, MINUS_T, MINUS_T, MINUS_T},
            {PLUS_T, PLUS_T, STATE_6, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, PLUS_T, STATE_3, PLUS_T, PLUS_T, PLUS_T, PLUS_T},
            {ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, ERROR_T, STATE_6, ERROR_T, ERROR_T, ERROR_T, ERROR_T},
            {NUMLIT_T, NUMLIT_T, STATE_6, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, STATE_5, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T},
            {NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T, STATE_6, NUMLIT_T, NUMLIT_T, NUMLIT_T, NUMLIT_T},
            {GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GTE_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T, GT_T},
            {LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LTE_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T, LT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, STATE_12, STATE_10, STATE_11, STATE_12, STATE_12, STATE_12, IDENT_T, STATE_12, IDENT_T, IDENT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, STATE_12, STATE_12, STATE_10, STATE_14, STATE_12, STATE_12, IDENT_T, STATE_12, IDENT_T, IDENT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, STATE_12, STATE_12, STATE_11, STATE_14, STATE_12, STATE_12, IDENT_T, STATE_12, IDENT_T, IDENT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, STATE_12, STATE_12, STATE_12, STATE_12, STATE_12, STATE_13, IDENT_T, STATE_13, IDENT_T, IDENT_T},
            {IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, IDENT_T, STATE_13, STATE_13, STATE_13, STATE_13, STATE_13, STATE_13, IDENT_T, STATE_13, IDENT_T, IDENT_T},
            {LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, LISTOP_T, STATE_12, STATE_12, STATE_12, STATE_12, STATE_12, STATE_12, LISTOP_T, STATE_12, LISTOP_T, LISTOP_T},
            {STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STATE_15, STRLIT_T, STATE_15}
    };

    char c;
    int col = 0;
    string word;
    while(state > 0) {
        c = line[pos];
        if (c == '\n'){
            if (getline(input, line)) {
                pos = 0;
                linenum++;
                line += " \n";
                c = line[pos];
                listingFile << setw(5) << linenum << ": " << line;

                if (err) {
                    string error = "Error at " + to_string(linenum) + "," + to_string(errPos) + ": Invalid character found: " + errorChar;
                    ReportError(error);
                }
                err = false;
                errPos = 0;
                line.clear();
            } else { // End of file
                listingFile << errors << " errors found in input file" << endl;
                return EOF_T;
            }
        }

        state--; // Because our states start at 1, but C++ starts at 0
        word += c;

        bool other = false;
        switch(c) {
            case '+':
                col = 0;break;
            case '-':
                col = 1;break;
            case '.':
                col = 2;break;
            case '/':
                col = 3;break;
            case '*':
                col = 4;break;
            case '>':
                col = 5;break;
            case '<':
                col = 6;break;
            case '=':
                col = 7;break;
            case '(':
                col = 8;break;
            case ')':
                col = 9;break;
            case '\'':
                col = 10;break;
            case 'c':
                col = 11;break;
            case 'a':
                col = 12;break;
            case 'd':
                col = 13;break;
            case 'r':
                col = 14;break;
            case ' ':
                col = 17;break;
            case '_':
                col = 18;break;
            case '"':
                col = 19;break;
            default:
                other = true;break;
        }
        if (other) {
            if (isalpha(c))
                col = 15;
            else if (isdigit(c))
                col = 16;
            else
                col = 20;
        }
        state = table[state][col];
        pos++;
    }
    tState = state;

    // Leading and trailing spaces messing up the map
    const auto strBegin = word.find_first_not_of(" \t");
    if (strBegin == std::string::npos)
        word = "";

    const auto strEnd = word.find_last_not_of(" \t");
    const auto strRange = strEnd - strBegin + 1;

    word = word.substr(strBegin, strRange);

    lexeme = word;

    if (state == 0) { // ERROR!
        err = true;
        errorChar = c;
        errPos = pos;
        errors++;
    }

    word.clear();

    return token;
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
    // The GetTokenName function returns a string containing the name of the
    // token passed to it.
    if (tState == -17) {
        map<string, string>::iterator it;

        it = keyNames.find(this->GetLexeme());
        if (it != keyNames.end())
            return it->second;
        else
            return token_names[tState + 17];
    } else {
        return token_names[tState + 17];
    }
}

string LexicalAnalyzer::GetLexeme () const
{
    // This function will return the lexeme found by the most recent call to
    // the get_token function
    return lexeme;
}

void LexicalAnalyzer::ReportError (const string & msg)
{
    // This function will be called to write an error message to a file
    listingFile << msg << endl;
}


