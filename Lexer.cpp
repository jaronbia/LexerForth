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

enum LexPhase { ACQUIRELINE, PROCESSBLOCK, ENDBLOCK };

void Lexer:: 
startLex(string& line, State& st) {
    int j = 0, k;
    LexPhase phase;

    while(st != FOUNDSYMBOL) {
        getline(in, line);
        if(in.eof() || !in.good()) break;
        
        if(phase == PROCESSBLOCK) {   // Process block
            readBlkComment(line, j, phase);
            if(phase != ENDBLOCK) continue;
        }

        for(j = 0; iswspace(line[j]); ++j, k = j + 1);    // clear whitespace
         
        if(line[j] = '\n') out << line[j];
        else if(line[j] = ''\'' && line[k] = ' ') out << line.substr(j);   // read whole line comment
        else if(line[j] = '(' && line[k] = ' ') phase = PROCESSBLOCK;            // is a block statement
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
    while(phase == ENDBLOCK && j < line.size()) {
        if(line[j] == ')') phase = ENDBLOCK;
        out << line[j++];
    }
}