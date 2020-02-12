#include <limits.h>

/*
Implement atoi which converts a string to an integer.

The function first discards as many whitespace characters as necessary until the first non-whitespace character is found.
Then, starting from this character, takes an optional initial plus or minus sign followed by as many numerical digits as
possible, and interprets them as a numerical value.

The string can contain additional characters after those that form the integral number, which are ignored and have no
effect on the behavior of this function. If the first sequence of non-whitespace characters in str is not a valid integral number,
or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed.
If no valid conversion could be performed, a zero value is returned.

Note:

Only the space character ' ' is considered as whitespace character.
Assume we are dealing with an environment which could only store integers within the 32-bit signed integer range: [−2**31,  2**31 − 1].
If the numerical value is out of the range of representable values, INT_MAX (231 − 1) or INT_MIN (−231) is returned.

Runtime: 4 ms, faster than 69.42% of C online submissions for String to Integer (atoi).
Memory Usage: 6.9 MB, less than 71.43% of C online submissions for String to Integer (atoi).
*/

int myAtoi(char * str){
    // we need a long int to test if it overflows
    // you cannot check with an int obviously
    long int ans = 0;
    int sign = 1;
    int i = 0;
    /*
    has to be stored as long int because on my system
    int cannot hold -INT_MIN because -INT_MIN > INT_MAX
    we could store in an unsigned int but it's a bit tricky
    because (unsigned int) (-INT_MIN) overflows, and 
    - (unsigned int) INT_MIN is wrong. So simplest is long int
    sub-optimal memory usage though.
    */
    long int ABS_INT_MIN = -(long int) INT_MIN;
    for (; str[i] != '\0'; i++) {
        // for this problem, only space is considered valid whitespace
        if (str[i] == ' ')
            continue;
        if (str[i] == '-') {
            sign = -1;
            i++;
            break;
        }
        if (str[i] == '+') {
            i++;
            break;
        }
        if (str[i] >= '0' && str[i] <= '9')
            break;
        // if it is anything but whitespace, +-, or a digit it isn't valid character
        return 0;
    }
    // detected number; let's start parsing; anything else after is ignored per
    // problem statement
    while (str[i] >= '0' && str[i] <= '9') {
        ans = ans*10 + (str[i++] - '0');
        if (ans >= INT_MAX && sign == 1) {
            return INT_MAX;
        }
        if (ans >= ABS_INT_MIN && sign == -1) {
            return INT_MIN;
        }
    }
    
    return sign*ans;
}

