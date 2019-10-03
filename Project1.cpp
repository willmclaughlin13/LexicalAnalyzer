#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "SetLimits.h"
#include "SyntacticalAnalyzer.h"

int main (int argc, char * argv[])
{
    if (argc < 2)
    {
        printf ("format: proj1 <filename>\n");
        exit (1);
    }

    SetLimits ();
    //generateLst(argv[1], lstFileName);
    SyntacticalAnalyzer parser (argv[1]);

    return 0;
}


/*void generateLst(const string& fileName, const string& outFileName) {
    ifstream infile(fileName);
    ofstream outfile;

    outfile.open(outFileName, ofstream::out | ofstream::trunc);

    string line;
    int lineNo = 1;

    while (getline(infile, line)) {
        outfile << lineNo << ": " << line << endl;
        lineNo++;
    }
    infile.close();
    outfile.close();
}*/
