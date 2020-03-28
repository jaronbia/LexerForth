/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>

using namespace std;

//-------------------------------------------------------------------------

enum TokenT { DEFAULT = 0, NUMBER = 1, WORD = 2, STRING = 3, COMMENT, COMMENTBLK };

class Token {
    private:
        string name;    // Name of token
        TokenT type;

    public:
        Token(string n = "", TokenT t = DEFAULT) : name(n), type(t) {};
        ~Token() = default;    

        const bool equals(const Token& t) const { return name == t.name; }
        const size_t hashtk() const { return ((hash<string>()(name) ^ hash<TokenT>()(type)) << 1) >> 1; }
        ostream& print(ostream& out) const { out << "Name: " << name << " Type: " << type; return out; }
};

inline bool operator == (const Token& t1, const Token& t2) { return t1.equals(t2); }
inline ostream& operator << (ostream& out, const Token t1) { return t1.print(out); }

//-------------------------------------------------------------------------
struct TokenHasher {
    std::size_t operator()(const Token& tk) const {
        return tk.hashtk();
    }
};

//-------------------------------------------------------------------------

enum State { READ = 0, PROCESSBLK = 100, 
            STARTLEX = 200, FOUNDSYMBOL = 300, 
            FOUNDSTRING = 400, FOUNDTOKEN = 500, DONE = 600 };

class Lexer {
    private:
        ifstream in;
        ofstream outlex;
        State currSt, prevSt;
        unordered_map<Token, int, TokenHasher> symbolTable;

        // Lexing functions
        void startLex(string& line, int& j);
        void acquireSymbol(string& line, int& j);
        void acquireString(string& line, int& j);
        void foundToken(string& line, int& j);
        bool isString(string& line, int& j) { return line[j] == '.' && line[j+1] == '\"'; }
        void changeState(State st) { currSt = st; }

        // start & finish of function
        void start() { currSt = READ; }
        bool finish() { return currSt == DONE; }

        // Reading functions
        void readNewLine(string& line, int& j);
        void readSingleLine(string& line, int& j);
        void readBlkComment(string& line, int& j);
        bool isSingleComment(string& line, int& j) { return line[j] == '\\' && line[j + 1] == ' '; }

        // Symbol table functions
        void updateTable(Token tk);
        void addToken(string name, TokenT tktype) { 
            updateTable(Token(name, tktype)); 
            changeState(STARTLEX);
        }

    public:
        Lexer(string filename);
        ~Lexer() = default;
        void lex();

        ostream& print(ostream& out) const;
};

inline ostream& operator << (ostream& out, Lexer& lr) { return lr.print(out); }