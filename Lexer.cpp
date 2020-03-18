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

//------------------------------------------------------------------------------------------
void Lexer::
lex() {
    int j = 0;
    State st = STARTLEX;
    string line;

    while(st != DONE) {
        if(st == STARTLEX) startLex(line, st, j);
        else if(st == FOUNDSYMBOL) acquireSymbol(line, st, j);
        else if(st == FOUNDSTRING) acquireString(line, st, j);
        else if(st == FOUNDTOKEN) foundToken(line, st, j);
    }
 }

//------------------------------------------------------------------------------------------
void Lexer:: 
startLex(string& line, State& st, int& j) {
    LexPhase phase = ACQUIRESTATE;

    while(st != FOUNDSYMBOL) {
        readNewLine(line, st, j);
        if(st == DONE) break;   // lexing done
        
        if(phase == PROCESSBLOCK) {   // process block
            readBlkComment(line, phase, j);
            if(phase != ACQUIRESTATE) continue; // read a new line in
        }

        for( ; iswspace(line[j]); ++j); // clear whitespace

        if(line[j] == '\n') outlex << line[j];   // endline or empty line encountered, add to output stream
        else if(line.empty() || ( line[j] == '\\' && iswspace(line[j + 1]) )) outlex << line.substr(j);   // read whole line comment, ignore it
        else if(line[j] == '(' && iswspace(line[j + 1])) phase = PROCESSBLOCK;      // is a block statement
        else st = FOUNDSYMBOL;  // found symbol, acquire it

        cout << line << '\n';
    }
}

//------------------------------------------------------------------------------------------
void Lexer:: 
acquireSymbol(string& line, State& st, int& j) {
    int k;

    while(st != FOUNDSTRING || st != FOUNDTOKEN) {
        for(j = 0; iswspace(line[j]); ++j); // clear whitespace

        if(line[j] == '\n') {
            readNewLine(line, st, j);
            continue;
        }
        
        if(line[j] == '.' && line[k] == '\"') st = FOUNDSTRING;
        else st = FOUNDTOKEN;
    }
}

//------------------------------------------------------------------------------------------ 
/*
• Skip leading whitespace by setting j to the first non-whitespace character. Then scan
with k to first quote character. The chars from j to k-1 are a STRING; add your STRING
token to the symbolTable. (Alternative: declare that the array, starting with position j,
is a stringstream and use getline() with three parameters to stop the read operation at
the first ’double-quote character.)
• Set j to k+1 and change the state variable to start.
• Note: this rule means that a string must start and end on the same line.
*/   
void Lexer::
acquireString(string& line, State& st, int& j) {
    int k;

    for(j = 0; iswspace(line[j]); ++j); // clear whitespace
    for(k = j + 1; line[k] != '\"'); ++k); // acquire string

}

//------------------------------------------------------------------------------------------
void Lexer::
foundToken(string& line, State& st, int& j) {

}

//-------------------------------------------------------------------------
void Lexer::
readBlkComment(string& line, LexPhase& phase, int& j) {
    while(phase != ACQUIRESTATE && j < line.size()) {
        if(line[j] == ')') phase = ACQUIRESTATE; // end of block comment, acquire the state
        outlex << line[j++];
    }
}

//-------------------------------------------------------------------------
void Lexer::
readNewLine(string& line, State& st, int& j) {
    getline(in, line);
    if(in.eof() || !in.good()) st = DONE;
    j = 0;  // start position at 0
}