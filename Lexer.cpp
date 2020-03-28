/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#include "Lexer.hpp"

/*Constructor loads file, starts the main lex function, and then closes
the file when done.*/
Lexer::Lexer(string filename) {
    cout << "Forth file to be lexed: " << filename << endl;
    in.open(filename);
    lex();
    in.close();
}

//------------------------------------------------------------------------------------------
/*Lex function controls the loop finds starting of lex, symbol, string, and token*/
void Lexer::lex() {
    int j = 0;
    string line;

    start();

    while (!finish()) {
        if (currSt == READ) readNewLine(line, j);
        else if (currSt == PROCESSBLK) readBlkComment(line, j);
        else if (currSt == STARTLEX) startLex(line, j);
        else if (currSt == FOUNDSYMBOL) acquireSymbol(line, j);
        else if (currSt == FOUNDSTRING) acquireString(line, j);
        else if (currSt == FOUNDTOKEN) foundToken(line, j);
    }
}

//-------------------------------------------------------------------------
/*Helper function that reads a new line and changes the state to STARTLEX,
resets j and then also if we are at the end of the file, sets the state
to done.*/
void Lexer::readNewLine(string& line, int& j) {
    getline(in, line);

    if (in.eof() || !in.good()) changeState(DONE);
    else if (currSt != PROCESSBLK) changeState(STARTLEX);
    line += '\n';

    j = 0;  // start position at 0
}

//-------------------------------------------------------------------------
/*Helper function to read block comment, if end of block comment is found
we change the state back to STARTLEX so control goes back to startlex function
otherwise we call the readNewLine function which will read in a new line
and reset j.*/
void Lexer::readBlkComment(string& line, int& j) {
    while (currSt == PROCESSBLK && j < int(line.size())) {
        if (line[j] == ')') changeState(STARTLEX); // end of block comment, acquire the state
        blkComments.push_back(line[j++]);
        if (line[j] != ')' && (j == line.size())) readNewLine(line, j);  // LOOK AT IT LATER
    }
}

//-------------------------------------------------------------------------
/*Helper function to read a single line comment.*/
void Lexer::readSingleLine(string& line, int& j) {
    if (line[j] == '\n') outlex << line[j];  // if endline read just that character
    else lineComments.push_back(line.substr(j));  // else output whole line
    currSt = READ;                  // read new line
}


//------------------------------------------------------------------------------------------
/*First we clear whitespace, then we check for a block comment, if not '(' then
we continue until we find either a comment or a symbol. If a symobol is found
we skip it, if a symbol is found we change the state. The state is changed to
PROCESSBLK is a leading '(' is found. Otherwise if no comment is found we
*/
void Lexer::startLex(string& line, int& j) {
    for (; line[j] != '\n' && isspace(line[j]); ++j); // clear whitespace
    if (line[j] == '(' && isspace(line[j + 1])) changeState(PROCESSBLK);   // is a block statement
    else if (line.empty() || line[j] == '\n' ||
        isSingleComment(line, j)) readSingleLine(line, j);  // read whole line comment, ignore it
    else changeState(FOUNDSYMBOL);  // found symbol, acquire it
}

//------------------------------------------------------------------------------------------
/*In aquireSymbol we are changing the state depending if we have found a token
a string, or we need to read a new line because we are at the end of a line.*/
void Lexer::acquireSymbol(string& line, int& j) {
    for (; line[j] != '\n' && isspace(line[j]); ++j); // clear whitespace
    if (line[j] == '\n') changeState(READ);
    else if (isString(line, j)) changeState(FOUNDSTRING);
    else changeState(FOUNDTOKEN);
}

//------------------------------------------------------------------------------------------
/*Helper function to aquire the string, we first add ." to the table, as that
is its own word, then we add the remainder of the string to the table.*/
void Lexer::acquireString(string& line, int& j) {
    int k = 0;
    string name = ".\"";
    addToken(name, WORD);
    for (; !isspace(line[j]); ++j);
    for (k = j + 1; line[k] != '\"'; ++k); // acquire string
    name = line.substr(j, k - j);
    addToken(name, STRING);
    j = k + 1;
}

//------------------------------------------------------------------------------------------
/*Helper function to add a token to the table. */
void Lexer::foundToken(string& line, int& j) {
    int k = 0;
    bool isNum = false;
    string name = "";
    TokenT tktype;

    // get token
    for (k = j + 1; !isspace(line[k]); ++k) {    // as long as it is not ws, end of line is processed in startLex
        if (!isdigit(line[k])) isNum = false; // check if token is number of not
        else if (isdigit(line[k])) isNum = true;
    }
    name = line.substr(j, k - j);
    if (isdigit(name[name.length() - 1])) isNum = true; //final check for number is required.
    tktype = isNum ? NUMBER : WORD; //We check wether token is a word or number then add it.
    addToken(name, tktype);
    j = k;
}

//-------------------------------------------------------------------------
/*Helder function to update table, if token is not found in table, increment
by 1, otherwise insert a new token.*/
void Lexer::updateTable(Token tk) {
    auto search = symbolTable.find(tk);
    if (search != symbolTable.end()) symbolTable[tk] += 1;
    else symbolTable.insert(make_pair(tk, 1));
}

//-------------------------------------------------------------------------
/*Printer function to print map and comments.*/
ostream& Lexer::print(ostream& out) const {
    ofstream outFile;
    outFile.open(FN, ios_base::app);
    if (outFile.is_open()) {
        outFile << "===============================Single Line Comments: =============================\n";
        for (int j = 0; j < int(lineComments.size()); j++) {
            outFile << lineComments[j];
        }
        outFile << "End of Line Comments: \n";
        outFile << "===================================================================================\n";
        outFile << "==============================Beginning of Block Comments==========================\n";
        for (int k = 0; k < int(blkComments.size()); k++) {
            outFile << blkComments[k];
            if (blkComments[k] == ')') {
                outFile << "\n";
            }
        }
        outFile << "=================================End of Block Comments=============================\n";
        outFile << "===================================================================================\n";
        outFile << "==================================Beginning of Token Map===========================\n";
        outFile << "Token Name\t\t\t\tType\t\t\t   Occurrences\n";
        for (auto const& pair : symbolTable) {
            outFile << '\n' << pair.first << pair.second;
        }
        outFile << "\n=================================End of Token Map================================\n";
        outFile << "\nTable Size: " << symbolTable.size() << '\n';
    }
    outFile.close();
    out << "===============================Single Line Comments: =============================\n";
    for (int j = 0; j < int(lineComments.size()); j++) {
        out << lineComments[j];
    }
    out << "End of Line Comments: \n";
    out << "===================================================================================\n";
    out << "==============================Beginning of Block Comments==========================\n";
    for (int k = 0; k < int(blkComments.size()); k++) {
        out << blkComments[k];
        if (blkComments[k] == ')') {
            out << "\n";
        }
    }
    out << "=================================End of Block Comments=============================\n";
    out << "===================================================================================\n";
    out << "==================================Beginning of Token Map===========================\n";
    out << "Token Name\t\t\t\tType\t\t\t   Occurrences\n";
    for (auto const& pair : symbolTable) {
        out << '\n' << pair.first << pair.second;
    }
    out << "\n=================================End of Token Map================================\n";
    out << "\nTable Size: " << symbolTable.size() << '\n';

    return out;
}

//------------------------------------------------------------------------
/*Helper print function for Token object. Used by print function from Lexer.*/
ostream& Token::print(ostream& out) const {
    out << name;

    if (type == 1) out << "\t\t\t\t\tNUMBER\t\t\t\t";
    if (type == 2) out << "\t\t\t\t\tWORD\t\t\t\t";
    if (type == 3) {

        if (name.length() > 19) {
            out << "\t\tString\t\t\t\t";
        }
        else if (name.length() < 13) {
            out << "\t\t\t\t\tString\t\t\t\t";
        }
        else {
            out << "\t\t\tString\t\t\t\t";
        }
    }

    return out;
}