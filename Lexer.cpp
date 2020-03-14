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

enum LexPhase { ACQUIRESTATE, PROCESSBLOCK }; // either try to find a symbol or process block comment

void Lexer:: 
startLex(string& line, State& st) {
    int j = 0, k;
    LexPhase phase = ACQUIRESTATE;

    while(st != FOUNDSYMBOL) {
        getline(in, line);
        if(in.eof() || !in.good()) break;
        
        if(phase == PROCESSBLOCK) {   // process block
            readBlkComment(line, j, phase);
            if(phase != ACQUIRESTATE) continue;
        }

        for(j = 0; iswspace(line[j]); ++j, k = j + 1);    // clear whitespace
         
        if(line[j] = '\n') out << line[j];
        else if(line[j] = ''\'' && line[k] = ' ') out << line.substr(j);   // read whole line comment, ignore it
        else if(line[j] = '(' && line[k] = ' ') phase = PROCESSBLOCK;      // is a block statement
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
readBlkComment(string& line, int& j, LexPhase& phase) {
    while(phase != ACQUIRESTATE && j < line.size()) {
        if(line[j] == ')') phase = ACQUIRESTATE; // end of block comment, acquire the state
        out << line[j++];
    }
}