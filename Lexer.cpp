/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#include "Lexer.hpp"

Lexer::
Lexer(string filename) {
    in.open(filename);
    lex();
    in.close();
}

//-------------------------------------------------------------------------
void Lexer::
lex() {
    State st = START;
    string line;

    for(;;) {
        if(st == START) startLex(string& line, State& st);
        else if(st == FOUNDSYMBOL) acquireSymbol(string& line, State& st);
        else if(st == FOUNDSTRING) acquireString(string& line, State& st);
        else foundToken(string& line, State& st);
    }
 }

//-------------------------------------------------------------------------
void Lexer:: 
startLex(string& line, State& st) {
    int j = 0, k;

    st = ACQUIRELINE;

    while(st != FOUNDSYMBOL) {
        if(st == ACQUIRELINE) {
            getline(in, line);
            if(in.eof() || !in.good()) break;
        }

        if(st == PROCESSBLOCK) {          // process block statement
            readBlkComment(line, j); 
            if(st != ENDBLOCK) continue;  // if end of block comment acquire state of nxt obj
            else st = ACQUIRELINE;
        }

        for(j = 0; iswspace(line[j]); ++j, k = j + 1);    // clear whitespace
        if(line[j] == '\n') continue;

        if(line[j] = ''\'' && line[k] = ' ') out << line.substr(j) << '\n';   // read whole line comment
        else if(line[j] = '(' && line[k] = ' ') st = PROCESSBLOCK;            // is a block statement
        else st = FOUNDSYMBOL;  // found symbol, acquire it
    }
        
    cout << line << "\n";

}

//-------------------------------------------------------------------------
void Lexer:: 
acquireSymbol(string& line, State& st) {

}

//-------------------------------------------------------------------------      
void Lexer::
acquireString(string& line, State& st) {

}

//-------------------------------------------------------------------------
void Lexer::
foundToken(string& line, State& st) {

}

//-------------------------------------------------------------------------
void Lexer::
readBlkComment(string& line, int& j) {
    while(st != ENDBLOCK && j < line.size()) {
        if(line[j++] == ')') st = ENDBLOCK;
        else out << line[j++];
    }
}