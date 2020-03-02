/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#include "Lexer.hpp"

Lexer::
Lexer(string filename) {
    in.open(filename);
    lex();
}

//-------------------------------------------------------------------------
void Lexer::
lex() {
    string line;

    for(int j = 0; ; ++j) {
        getline(in, line);
        
        if(line[j] == ' ') continue;
        if(in.eof() || !in.good()) break;

        //for(int k = j + 1; ; ++k) {

        //}

        cout << line << "\n";
    }
}

//-------------------------------------------------------------------------
void Lexer::
startLex() { 
    
}

//-------------------------------------------------------------------------
void Lexer:: 
acquireSymbol() {

}

//-------------------------------------------------------------------------      
void Lexer::
acquireString() {

}

//-------------------------------------------------------------------------
void Lexer::
foundToken() {

}