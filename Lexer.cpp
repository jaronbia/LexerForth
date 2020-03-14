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

    for (;;) {
        getline(in, line);
        if (in.eof() || !in.good()) break;
        if (st == START) startLex(line, st);
        else if (st == SYMBOL) acquireSymbol(line, st);
        else if (st == STRINGS) acquireString(line, st);
        else foundToken(line, st);
    }
}

void Lexer::startLex(string& line, State& st) {
    int j, k = 1;
    bool skipws = false;
    Ttype = NUMBER;
    if (endParen) {
        temp.clear();
    }
    if (line.empty()) { // empty line, return to lex()
        return;
    }
    else {
        //cout << line.length();
        for (j = 0; j < line.length(); j++) {   // clear whitespace
            if (!isspace(line[j]) && !skipws && line[j] != 92 && endParen && line[j] != 40) {
                temp += line[j];
                skipws = true;
            }
            else if (skipws && line[j] != 92 && endParen && line[j] != 40) {
                temp += line[j];
            }
            else if (line[j] == 92 && isspace(line[k]) && endParen) {
                temp = line.substr(j);
                Ttype = COMMENT;
                break;
            }
            else if (line[j] == 40 || !endParen) {
                if (line[j] == 41) {
                    endParen = true;
                    Ttype = COMMENTBLK;
                    break;
                }
                if (endParen) {
                    print(cout);
                    temp.clear();
                }
                endParen = false;
                temp += line[j];
            }
            k++;

        }
    }
    print(cout);
}

void Lexer::acquireSymbol(string& line, State& st) {

}

void Lexer::acquireString(string& line, State& st) {

}

void Lexer::foundToken(string& line, State& st) {

}

ostream& Lexer::print(ostream& out) {
    if (Ttype == COMMENT) {
        out << "One line comment found: " << temp << '\n';
    }
    else if (Ttype == COMMENTBLK) {
        out << "Block Comment found: " << temp << '\n';
    }
    else {
        out << "not comment: ";
        out << temp << '\n';
    }

    return out;
}