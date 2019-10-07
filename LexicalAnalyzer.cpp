#include <iomanip>
#include "LexicalAnalyzer.h"
#include <map>
#include <algorithm>

using namespace std;

static string token_names[] = {	"IDENT_T", "NUMLIT_T", "STRLIT_T", "LISTOP_T", "PLUS_T",
                                   "MINUS_T", "DIV_T", "MULT_T", "EQUALTO_T", "EOF_T",
                                   "GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T", "RPAREN_T",
                                   "SQUOTE_T", "ERROR_T"};

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
    // This function will initialize the lexical analyzer class
    token = NONE;
    linenum = 0;
    pos = -1;
    errors = 0;

    string rootFileName = filename;
    for (int i = 0; i < 3; ++i) {
        rootFileName.pop_back();
    }
    input.open(filename);
    listingFile.open(rootFileName + ".lst", ofstream::out | ofstream::trunc);
    debugFile.open(rootFileName + ".dbg", ofstream::out | ofstream::trunc);
    tokenFile.open(rootFileName + ".p1", ofstream::out | ofstream::trunc);
    listingFile << "Input file: " << rootFileName + ".ss" << endl;
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

    map<string, string> keyNames;
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

    char c;
    int inputVal = 0;
    int currentPos = 0;
    string word;
    while(state > 0) {
        if (!input.get(c)) {
            tokenFile << left << setw(10) << "EOF_T" << setw(20) << word << endl;
            linenum++;
            listingFile << setw(5) << linenum << ": " << line << endl;
            listingFile << errors << " errors found in input file\n";
            return EOF_T;
        }

        currentPos++;
        if (isblank(c) == 0 && input.peek() == '\n') // Prevent error on blank line
            input.get(c);
        if (c != '\n' || input.peek() == '\n') // Increment line count
            line += c;
        state--; // Because our states start at 1, but C++ starts at 0
        word += c;

        bool other = false;
        switch(c) {
            case '+':
                inputVal = 0;break;
            case '-':
                inputVal = 1;break;
            case '.':
                inputVal = 2;break;
            case '/':
                inputVal = 3;break;
            case '*':
                inputVal = 4;break;
            case '>':
                inputVal = 5;break;
            case '<':
                inputVal = 6;break;
            case '=':
                inputVal = 7;break;
            case '(':
                inputVal = 8;break;
            case ')':
                inputVal = 9;break;
            case '\'':
                inputVal = 10;break;
            case 'c':
                inputVal = 11;break;
            case 'a':
                inputVal = 12;break;
            case 'd':
                inputVal = 13;break;
            case 'r':
                inputVal = 14;break;
            case ' ':
                inputVal = 17;break;
            case '_':
                inputVal = 18;break;
            case '"':
                inputVal = 19;break;
            default:
                other = true;break;
        }
        if (other) {
            if (isalpha(c))
                inputVal = 15;
            else if (isdigit(c))
                inputVal = 16;
            else
                inputVal = 20;
        }
        state = table[state][inputVal];

        if (c == '\n') { // End of line
            linenum++;
            listingFile << setw(5) << linenum << ": " << line << endl;
            if (pos != -1)
                listingFile << "Error at " << linenum << "," << pos << ": Invalid character found: " << errorChar << endl;
            line.clear();

            pos = -1;
            currentPos = 0;
        }

    }
    token = table[state][inputVal];
    // Leading and trailing spaces fucking up the map
    word.erase(remove_if(word.begin(), word.end(), ::isspace), word.end());

    if (state == 0 && !word.empty() && word != "\n") { // ERROR!
        pos = currentPos;
        errorChar = c;
        errors++;
        //tokenFile << left << setw(10) << token_names[state+17] << setw(20) << word << endl;
        lexeme = token_names[state+17];

    } else if (state == -17) { // Token is IDENT_T?
        map<string, string>::iterator it;

        it = keyNames.find(word);
        if (it == keyNames.end())
            lexeme = token_names[state+17];
        else
            lexeme = it->second;
    } else
        lexeme = token_names[state+17];

    tokenFile << left << setw(10) << lexeme << setw(20) << word << endl;

    word.clear();

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
    return lexeme;
}

void LexicalAnalyzer::ReportError (const string & msg)
{
    // This function will be called to write an error message to a file
}


