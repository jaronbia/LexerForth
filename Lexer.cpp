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

    start();

    while(!finish()) {
        if(currSt == READ) readNewLine(line, j);
        else if(currSt == PROCESSBLK) readBlkComment(line, j);
        else if(currSt == STARTLEX) startLex(line, j);
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
    for( ; line[j] == ' '; ++j); // clear whitespace

    if(line[j] == '(' && line[j + 1] == ' ') changeState(PROCESSBLK);   // is a block statement
    else if(line.empty() || line[j] == '\n' || isSingleComment(line, j)) readSingleLine(line, j);  // read whole line comment, ignore it
    else changeState(FOUNDSYMBOL);  // found symbol, acquire it

    cout << line << '\n';
    cout << currSt << '\n';
}

//------------------------------------------------------------------------------------------
void Lexer:: 
acquireSymbol(string& line, int& j) {
    for( ; line[j] == ' '; ++j); // clear whitespace, JUST REMOVED J=0 FROM THE BEGINNING

    if(line[j] == '\n') changeState(READ);
    else if(isString(line, j)) changeState(FOUNDSTRING);
    else changeState(FOUNDTOKEN);
}

//------------------------------------------------------------------------------------------ 
void Lexer::
acquireString(string& line, int& j) {
    int k;
    string name;

    for( ; line[j] == ' '; ++j); // clear whitespace
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

    // get token
    for(k = j + 1; !isspace(line[k]); ++k) {    // as long as it is not ws, end of line is processed in startLex
        if(!(line[k] >= '0' || line[k] <= '9')) isNum = false;
    }

    name = line.substr(j, k);
    tktype = isNum ? NUMBER : WORD;
    addToken(name, tktype);

    j = k;
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
readSingleLine(string& line, int& j) {
    if(line[j] == '\n') outlex << line[j];  // if endline read just that character
    else outlex << line.substr(j);  // else output whole line
    currSt = READ;                  // read new line
}

//-------------------------------------------------------------------------
void Lexer::
readBlkComment(string& line, int& j) {
    while(currSt == PROCESSBLK && j < line.size()) {
        if(line[j] == ')') changeState(STARTLEX); // end of block comment, acquire the state
        outlex << line[j++];
    }
}

//-------------------------------------------------------------------------
void Lexer::
updateTable(Token tk) {
    auto search = symbolTable.find(tk);
    if(search != symbolTable.end()) symbolTable[tk] += 1;
    else symbolTable.insert(make_pair(tk, 1));
}