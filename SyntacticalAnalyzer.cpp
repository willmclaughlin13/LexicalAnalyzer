//
// Created by Will on 9/28/2019.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
    lex = new LexicalAnalyzer (filename);
    token_type t;
    while ((t = lex->GetToken()) != EOF_T)
    {
        //cout << lex->GetLexeme() << endl;
    }
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
    delete lex;
}
