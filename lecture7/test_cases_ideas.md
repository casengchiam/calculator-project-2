the calculator project takes in floating point numbers
uses the print command, ;
and has the quit method, q

WE WISH TO IMPLEMENT:
DONE : - negative numbers 
DONE : - remainder / modulo
DONE : - pre-defined symbolic variables (pi )
- variables 

TEST CASES:
1. testing the handling of parentheses
    -> non matching
    2 + (5 * 2));
    -> matching 
    2 + (5 * 2);
2. testing the handling of large/small floating point
    -> large only floats
    3.678943543512343241863274621809712048621973471823548612374901273402141234 + 6.78943543512343241863274621809712048621973471823548612374901273402141234;
    -> large and small floats/ints
    3.678943543512343241863274621809712048621973471823548612374901273402141234 + 1;
    3.678943543512343241863274621809712048621973471823548612374901273402141234 + 1.2;
4. testing PEMDAS
    -> testing same expression but without parentheses
    4 + 6 * 4 / 6 - 2;
    4 + ((6 * 4) / (6-2));
5. testing invalid tokens
    -> bad token
    4 @ 2;
    4 # 2;
6. testing negatives
    -> negative and positive values expression
    -3.6 + 2.0;
    -> multiple negatives on one numbers
    ---2.0 + 3.0;
    -> all negative expression
    - 2.0 - -3.0;
7. non-valid math expressions
    -> dividing by 0
    3 / 0;
    -> modulo by 0
    2 % 0;
8. too many spaces between characters, no spaces between characters
    -> testing symbolic variables without putting white space
    pi*7+9;
    -> having too much white space in an expression 
    pi          *           2;
    -> whitespace between characters w/o an operator
    pi 7 + 9;
    9 8 + 9;
9. symbolic variables mixed with other tokens w/o whitespace 
    -> symbolic variable followed by alpha token
    pia * 2;
    -> symbolic variable followed by numeric token
    pi3 * 2;
10. updating variables after first use
    x = 7;
    2 * x;
    x = 4;
    2 * x;


NEW NOTES 
**05/07 1:08 PM**
unable to use x as the left operand, only the right operand 