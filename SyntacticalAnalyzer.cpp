//
// Created by Will on 9/28/2019.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
    string rootFileName = filename;
    for (int i = 0; i < 3; ++i) {
        rootFileName.pop_back();
    }
    ofstream tokenFile;
    tokenFile.open(rootFileName + ".p1", ofstream::out | ofstream::trunc);
    lex = new LexicalAnalyzer (filename);
    token_type t;

    while ((t = lex->GetToken()) != EOF_T)
    {
        tokenFile << "\t\t" << left << setw(15) << lex->GetTokenName(t) << lex->GetLexeme() << endl;
    }
    tokenFile << "\t\t" << left << "EOF_T" << endl;
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
    delete lex;
}
