//
// Created by Will on 9/28/2019.
//

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer
{
public:
    SyntacticalAnalyzer (char * filename);
    ~SyntacticalAnalyzer ();
private:
    LexicalAnalyzer * lex;
    token_type token;
};

#endif
