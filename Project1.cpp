#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "SetLimits.h"
#include "SyntacticalAnalyzer.h"

void generateLst(const string&);

int main (int argc, char * argv[])
{
    if (argc < 2)
    {
        printf ("format: proj1 <filename>\n");
        exit (1);
    }
    SetLimits ();
    generateLst(argv[1]);
    SyntacticalAnalyzer parser (argv[1]);


    return 0;
}


void generateLst(const string& fileName) {
    ifstream infile(fileName);
    ofstream outfile;
    outfile.open("P1-0.lst", ofstream::out | ofstream::trunc);

    string line;
    int lineNo = 1;
    cout << "Input file: " << fileName << endl;

    while (getline(infile, line)) {
        outfile << lineNo << ": " << line << endl;
        lineNo++;
    }
    infile.close();
    outfile.close();
}
