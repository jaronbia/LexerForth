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

enum TokenT { DEFAULT, NUMBER, WORD, STRING, COMMENT, COMMENTBLK };

class Token {
    private:
        string name;    // Name of token
        TokenT type;

    public:
        Token(string n = "", TokenT t = DEFAULT) : name(n), type(t) {};
        ~Token() = default;    

        const bool equals(const Token& t) const { return name == t.name; }
        const size_t hashtk() const { return ((hash<string>()(name) ^ hash<TokenT>()(type)) << 1) >> 1; }
        ostream& print(ostream& out) const { out << name; return out; }
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

enum State { STARTLEX, FOUNDSYMBOL, FOUNDSTRING, FOUNDTOKEN, DONE };
enum LexPhase { ACQUIRESTATE, PROCESSBLOCK }; // either try to find a symbol or process block comment within startlex

class Lexer {
    private:
        ifstream in;
        ofstream outlex;
        unordered_map<Token, int, TokenHasher> symbolTable;

        void startLex(string& line, State& st, int& j);
        void acquireSymbol(string& line, State& st, int& j);
        void acquireString(string& line, State& st, int& j);
        void foundToken(string& line, State& st, int& j);

        void readNewLine(string& line, State& st, int& j);
        void readBlkComment(string& line, LexPhase& phase, int& j);

    public:
        Lexer(string filename);
        ~Lexer() = default;
        void lex();

        ostream& print(ostream& out) { return out; }
};

inline ostream& operator << (ostream& out, Lexer& lr) { return lr.print(out); }