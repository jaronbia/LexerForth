/***************************************
 * Author: Jaron Bialecki & Evan Perry
 * Date: March 2nd, 2020
***************************************/

#include <iostream>
#include "Lexer.hpp"

int main(int argc, char** argv) {
    string filename(argv[1]);
    Lexer ForthLex(filename);

    return 0;
}