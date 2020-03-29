/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#include "Lexer.hpp"

string header();

int main(int , char** argv) {
    ofstream outFile;
    outFile.open(FN, ios_base::app);
    if (outFile.is_open()) {
        outFile << header();
    }
    outFile.close();
    cout << header();
    string filename(argv[1]);
    Lexer ForthLex(filename);

    cout << ForthLex;

    return 0;
}

string header() {
    string head = "============================================================"
        "======================\nJaron Bialecki & Evan Perry\nCSCI 6636\nLexer Unit Test\n";
    time_t rawtime;
    time(&rawtime);
    string curTime = ctime(&rawtime);
    head += curTime;
    head += "============================================================"
        "======================\n";
    return head;
}