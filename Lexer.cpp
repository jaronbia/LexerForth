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

    for(auto const& pair : symbolTable) 
        cout << '\n' << pair.first << " : " << pair.second << '\n';
    cout << symbolTable.size() << '\n';
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
    while(st != FOUNDSTRING && st != FOUNDTOKEN) {
        for(j = 0; iswspace(line[j]); ++j); // clear whitespace

        if(line[j] == '\n') {
            readNewLine(line, st, j);
            continue;
        }
        
        if(line[j] == '.' && line[j+1] == '\"') st = FOUNDSTRING;
        else st = FOUNDTOKEN;
    }
}

//------------------------------------------------------------------------------------------ 
void Lexer::
acquireString(string& line, State& st, int& j) {
    int k;
    string name;
    Token tk;

    for(j = 0; iswspace(line[j]); ++j); // clear whitespace
    for(k = j + 1; line[k] != '\"'; ++k); // acquire string

    name = line.substr(j, k - 1);
    tk = Token(name, STRING);

    auto search = symbolTable.find(tk);
    if(search != symbolTable.end()) symbolTable[tk] += 1; // increment if in table
    else symbolTable.insert(make_pair(tk, 1)); 

    st = STARTLEX;
}

//------------------------------------------------------------------------------------------
void Lexer::
foundToken(string& line, State& st, int& j) {
    int k;
    bool isNum = true;
    string name;
    TokenT tktype;
    Token tk;

    // get token
    for(k = j + 1; !iswspace(line[k]); ++k) {
        if(!(line[k] >= '0' || line[k] <= '9')) isNum = false;
    }

    name = line.substr(j, k - 1);
    tktype = isNum ? NUMBER : WORD;
    tk = Token(name, tktype);

    auto search = symbolTable.find(tk);
    if(search != symbolTable.end()) symbolTable[tk] += 1;
    else symbolTable.insert(make_pair(tk, 1));

    st = STARTLEX;
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