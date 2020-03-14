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

/*Compiles successfully
One line comment found: \ Print the first n Fibonacci numbers
not comment: : fib
Block Comment found: ( n >>>
not comment: dup CR ." Print the first " . ." Fibonacci numbers." CR ."    "
not comment: dup 2 < if ." number must be 2 or greater."
not comment: else
not comment: 1 1
Block Comment found: ( initialize for 1st 2 terms of series.
not comment: 2dup . .
Block Comment found: ( and print those two terms.
not comment: rot
Block Comment found: ( bring loop limit to top of stack.
not comment: 1 do
Block Comment found: ( set up counted loop from one to n.
not comment: dup
Block Comment found: ( make a copy of newest term for later use.
not comment: rot
Block Comment found: ( bring 2nd newest term to top of stack.
not comment: + dup .
Block Comment found: ( new term is sum of 2 previous terms.  Print it.
not comment: loop
Block Comment found: ( return to top of loop.
not comment: then
not comment: CR
not comment: ;
not comment: 5 fib
not comment: 10 fib
not comment: 1 fib
not comment: 100 fib
not comment: 2 fib
One line comment found: \ End of Test File for Lexer
*/