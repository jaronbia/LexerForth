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
    State st = STARTLEX;
    string line;

    for(;;) {
        if(st == DONE) break;
        if(st == STARTLEX) startLex(line, st);
        else if(st == FOUNDSYMBOL) acquireSymbol(line, st);
        else if(st == FOUNDSTRING) acquireString(line, st);
        else foundToken(line, st);
    }
 }

//-------------------------------------------------------------------------
void Lexer:: 
startLex(string& line, State& st) {
    int j = 0, k;
    LexPhase phase = ACQUIRESTATE;

    while(st != FOUNDSYMBOL) {
        getline(in, line);
        if(in.eof() || !in.good()) break;
        if(line.empty()) continue;
        
        if(phase == PROCESSBLOCK) {   // process block
            readBlkComment(line, j, phase);
            if(phase != ACQUIRESTATE) continue;
        }

        for(j = 0; iswspace(line[j]); ++j);    // clear whitespace

        if(line[j] == '\n') outlex << line[j]; 
        else if(line[j] == '\\' && iswspace(line[j + 1])) outlex << line.substr(j);   // read whole line comment, ignore it
        else if(line[j] == '(' && iswspace(line[j + 1])) phase = PROCESSBLOCK;      // is a block statement
        else st = FOUNDSYMBOL;  // found symbol, acquire it
    }
}

//-------------------------------------------------------------------------
void Lexer:: 
acquireSymbol(string& line, State& st) {
    st = DONE;
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
readBlkComment(string& line, int& j, LexPhase& phase) {
    while(phase != ACQUIRESTATE && j < line.size()) {
        if(line[j] == ')') phase = ACQUIRESTATE; // end of block comment, acquire the state
        outlex << line[j++];
    }
}