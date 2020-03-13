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
        getline(in, line);
        if(in.eof() || !in.good()) break;

        if(st == START) startLex(string& line, State& st);
        else if(st == SYMBOL) acquireSymbol(string& line, State& st);
        else if(st == STRING) acquireString(string& line, State& st);
        else foundToken(string& line, State& st);
    }
 }

//-------------------------------------------------------------------------
void Lexer:: 
startLex(string& line, State& st) {
    int j, k;

    for(j = 0; line[j] != ' '; ++j) {   // clear whitespace
        if(line[j] == '\n') return;     // empty line, return to lex()
    }

    k = j + 1;

    if(line[j] = ''\'' && line[k] = ' ') out << line.substr(j) << '\n';     // read line comment
    else if(line[j] = '(' && line[k] = ' ') readBlkComment(line, j);    
    else st = SYMBOL;
        
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
readBlkComment(string& line, int j) {
    out << line.substr(j) << '\n';  // read first part of comment in

    for( ; line[j] == ')'; ++j) {
        getline(in, line);  // read new line in and check for error

    }
}