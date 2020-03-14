/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#include "Lexer.hpp"

Lexer::Lexer(string filename) {
    in.open(filename);
    lex();
    in.close();
}

void Lexer::lex() {
    State st = START;
    string line;
    int x = 1;

    for (;;) {
        getline(in, line);
        if (in.eof() || !in.good()) break;
        if (st == START) startLex(line, st);
        if (st == SYMBOL) acquireSymbol(line, st);
        else if (st == STRINGS) acquireString(line, st);
        else foundToken(line, st);
    }
}

void Lexer::startLex(string& line, State& st) {
    int j, k = 1;
    bool skipws = false;
    bool fSymbol = false;
    Ttype = NUMBER;
    if (endParen) {
        temp.clear();
    }
    if (!line.empty()) {
        for (j = 0; j < line.length(); j++) {   // clear whitespace
            if (!isspace(line[j]) && !skipws && line[j] != 92 && endParen && line[j] != 40) {
                temp += line[j];
                skipws = true;
                fSymbol = true;
            }
            else if (skipws && line[j] != 92 && endParen && line[j] != 40) {
                temp += line[j];
                if (!fSymbol) {
                    fSymbol = true;
                }
            }
            else if (line[j] == 92 && isspace(line[k]) && endParen) {
                cout << line.substr(j) << endl;
                break;
            }
            else if (line[j] == 40 || !endParen) {

                if (line[j] == 41) {
                    cout << endl;
                    endParen = true;
                    break;
                }
                cout << line[j];
                endParen = false;
                if (endParen) {
                    break;
                }
            }
            k++;
        }
    }
    if (fSymbol && endParen) {
        st = SYMBOL;
        line = temp;

    }
}

void Lexer::acquireSymbol(string& line, State& st) {
    temp.clear();
    temp = line;
    print(cout);
    st = START;
}

void Lexer::acquireString(string& line, State& st) {

}

void Lexer::foundToken(string& line, State& st) {

}

ostream& Lexer::print(ostream& out) {
    if (!temp.empty()) {
        out << "not comment: ";
        out << temp << '\n';
    }
    return out;
}