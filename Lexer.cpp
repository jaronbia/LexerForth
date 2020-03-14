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
    if (endParen) { /*If endParen is true, which is its default starting
       position we set the string to empty*/
        temp.clear();
    }
    if (!line.empty()) { //we skip any empty lines
        for (j = 0; j < int(line.length()); j++) {   // clear whitespace
            /* first we check if there are no spaces, and skipws is still false, and
            we have not found an '\' and no beginning '(' we used the ascii numbers
            for them. If the conditions are statisfied we then add a temporary string
            next we set two booleans to be true, one that we have finished skipping
            all leading white space and one that we have found a token*/
            if (!isspace(line[j]) && !skipws && line[j] != 92 &&
                endParen && line[j] != 40) {
                temp += line[j];
                skipws = true;
                fSymbol = true;
            }
            /* Next we check to see if we found either a '\' or '(', if not we
            continue adding to our temporary value. We also set fSymbol to true
            just incase there was no leading white space*/
            else if (skipws && line[j] != 92 && endParen && line[j] != 40) {
                temp += line[j];
                if (!fSymbol) {
                    fSymbol = true;
                }
            }
            /* Next if we find a '\' we check to make sure the next character
            is a space and that endParen is true to make sure we are not in the
            middle of block comment. We then print out the resulting comment
            and break out of the loop. */
            else if (line[j] == 92 && isspace(line[k]) && endParen) {
                cout << line.substr(j) << endl;
                break;
            }
            /*Lastly we check for the '(' indicating the start of a comment
            block, if found we set endParen to be false, we do change it back
            when the ')' is found. We also print out the text with the exception of
            the ending ')'*/
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
            k++;  // k is started at 1, and then is incremented along.         
        }
    }
    /*If both fSymbol and endParen are true we can assume that we have found
    a symbol. We then change the state to SYMBOL and set the line to equal are
    temporary string.*/
    if (fSymbol && endParen) {
        st = SYMBOL;
        line = temp;

    }
}
/*For now this is just a proof to show that acquire symbol is getting the
text from startLex, we clear the temp to ensure it is empty for proper printing
next we set it equal the value we want to print for the print function. Then
once we are done we change it back to START so we can go back through the rest of
file. Eventually acquiringString and Foundtoken will be the functions to change
st back to Start.*/
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

/*Simple print function, does not print comments, but it does print the tokens
for now. We may have to change it to also print the comments as well, we should
be able to also modify this function to write to a file as well, as I believe
those are part of the program requirements.*/
ostream& Lexer::print(ostream& out) {
    if (!temp.empty()) {
        out << "not comment: ";
        out << temp << '\n';
    }
    return out;
}