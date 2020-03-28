\ Print the first n Fibonacci numbers

: fib ( n >>> )
    dup CR ." Print the first " . ." Fibonacci numbers." CR ."    "
    dup 2 < if ." number must be 2 or greater."
    else
        1 1 		( initialize for 1st 
		2 terms of series. )
        2dup . . 	( and print those two terms. )
        rot 		( bring loop limit to top of stack. )
        1 do 		( set up counted loop from one to n. )
            dup  	( make a copy of newest term for later use. )
	    rot  	( bring 2nd newest term to top of stack. )
	    + dup .  	( new term is sum of 2 previous terms.  Print it. )
        loop  		( return to top of loop. )
    then
    CR
;


5 fib
10 fib
1 fib
100 fib
2 fib
\ End of Test File for Lexer
