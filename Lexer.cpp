/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#include "Lexer.hpp"

Lexer::
Lexer(string filename) {
    cout << filename << endl;
    in.open(filename);
    lex();
    in.close();
}

//------------------------------------------------------------------------------------------
void Lexer::
lex() {
    int j = 0;
    string line;
    Token tk("CR", STRING);

    currSt = STARTLEX;

    while(!lexfinish()) {
        if(currSt == STARTLEX) startLex(line, j);
        else if(currSt == FOUNDSYMBOL) acquireSymbol(line, j);
        else if(currSt == FOUNDSTRING) acquireString(line, j);
        else if(currSt == FOUNDTOKEN) foundToken(line, j);
    }

    for(auto const& pair : symbolTable) 
       cout << '\n' << pair.first << " : " << pair.second << '\n';
    cout << "Table Size: " << symbolTable.size() << '\n';
 }

//------------------------------------------------------------------------------------------
void Lexer:: 
startLex(string& line, int& j) {
    LexPhase phase = ACQUIRESTATE;

    while(currSt != FOUNDSYMBOL) {
        readNewLine(line, j);
        if(lexfinish()) break;   // lexing done
        
        if(phase == PROCESSBLOCK) {   // process block
            readBlkComment(line, phase, j);
            if(phase != ACQUIRESTATE) continue; // read a new line in
        }

        for( ; isspace(line[j]); ++j); // clear whitespace

        if(line[j] == '\n') outlex << line[j];   // endline or empty line encountered, add to output stream
        else if(line.empty() || ( line[j] == '\\' && isspace(line[j + 1]) )) outlex << line.substr(j);    // read whole line comment, ignore it
        else if(line[j] == '(' && isspace(line[j + 1])) phase = PROCESSBLOCK;   // is a block statement
        else changeState(FOUNDSYMBOL);  // found symbol, acquire it

        cout << line << '\n';
        cout << currSt << '\n';
    }
}

//------------------------------------------------------------------------------------------
void Lexer:: 
acquireSymbol(string& line, int& j) {
    while(currSt != FOUNDSTRING && currSt != FOUNDTOKEN) {
        for( ; isspace(line[j]); ++j); // clear whitespace, JUST REMOVED J=0 FROM THE BEGINNING

        if(line[j] == '\n') {
            readNewLine(line, j);
            //if(lexfinish()) break;   // lexing done, NEEDS UPDATING WONT WORK AS IS
            continue;
        }
        
        if(isString(line, j)) changeState(FOUNDSTRING);
        else changeState(FOUNDTOKEN);
    }
}

//------------------------------------------------------------------------------------------ 
void Lexer::
acquireString(string& line, int& j) {
    int k;
    string name;

    for( ; isspace(line[j]); ++j); // clear whitespace
    for(k = j + 1; line[k] != '\"'; ++k); // acquire string

    name = line.substr(j, k - 1);
    addToken(name, STRING);

    j = k;
}

//------------------------------------------------------------------------------------------
void Lexer::
foundToken(string& line, int& j) {
    int k;
    bool isNum = true;
    string name = "";
    TokenT tktype;

    cout << "FOUND TOKEN ACQUIRING......" << endl;
    cout << "Line: " << line << endl;
    cout << "Symbol: " << line[j];

    // get token
    for(k = j + 1; !isspace(line[k]); ++k) {
        if(!(line[k] >= '0' || line[k] <= '9')) isNum = false;
    }

    cout << "\nFOUND TOKEN ACQUIRING......" << endl;

    name = line.substr(j, k);
    cout << "Name: " << name << endl; 
    tktype = isNum ? NUMBER : WORD;
    addToken(name, tktype);

    j = k;
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
readNewLine(string& line, int& j) {
    getline(in, line);
    if(in.eof() || !in.good()) changeState(DONE);
    j = 0;  // start position at 0
}

//-------------------------------------------------------------------------
void Lexer::
updateTable(Token tk) {
    auto search = symbolTable.find(tk);
    if(search != symbolTable.end()) symbolTable[tk] += 1;
    else symbolTable.insert(make_pair(tk, 1));
}