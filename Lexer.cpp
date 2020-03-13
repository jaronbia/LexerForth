/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#include "Lexer.hpp"

Lexer::
Lexer(string filename) {
    cout << filename << endl;
    in.open(filename);
    if (in.good()) {
        lex();
    }
    else {
        cout << "Error, unable to open file." << endl;
        in.close();
    }
    
}

//-------------------------------------------------------------------------
void Lexer::
lex() {
    string line;

    for(int j = 0; ; j++) {
        getline(in, line);
        //if (line[j] == ' ')continue;  <-This line causes an error when you read in, commented out
        if (in.eof() || !in.good()) break;

        //for(int k = j + 1; ; ++k) {

        //}

        cout << line << "\n";
    }
    in.close();
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