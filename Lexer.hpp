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

enum TokenT { DEFAULT, WORD, STRING, COMMENT, COMMENTBLK };

class Token {
    private:
        int occur;      // Number of occurrences 
        string name;    // Name of token
        TokenT type;

    public:
        Token(int occ = 0, string n = "", TokenT t = DEFAULT) : occur(occ), name(n), type(t) {};
        ~Token() = default;    

        const bool equals(const Token& t) const { return name == t.name; }
        operator size_t() const { 
            size_t h1 = hash<int>()(occur);
            size_t h2 = hash<string>()(name);
            size_t h3 = hash<TokenT>()(type);
            return h1 ^ h2 ^ h3;
        }
};

inline bool operator == (const Token& t1, const Token& t2) { return t1.equals(t2); }

//-------------------------------------------------------------------------

enum State { STARTLEX, FOUNDSYMBOL, FOUNDSTRING, FOUNDTOKEN, DONE };
enum LexPhase { ACQUIRESTATE, PROCESSBLOCK }; // either try to find a symbol or process block comment within startlex

class Lexer {
    private:
        ifstream in;
        ofstream outlex;
        unordered_map<Token, int> symbolTable;

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