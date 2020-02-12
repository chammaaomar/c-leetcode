/*
Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.

An input string is valid if:

Open brackets must be closed by the same type of brackets.
Open brackets must be closed in the correct order.
Note that an empty string is also considered valid.

Runtime: 0 ms, faster than 100.00% of C online submissions for Valid Parentheses.
Memory Usage: 6.9 MB, less than 46.15% of C online submissions for Valid Parentheses.
*/
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_left(char c) {
    return (c == '(') || (c == '{') || (c == '[');
}

bool is_right(char c) {
    return (c == ')') || (c == '}') || (c == ']');
}

char valid_half(char c) {
    switch(c) {
        case '(':
            return ')';
        case '{':
            return '}';
        case '[':
            return ']';
        default:
            return '\0';
    }
}

bool isValid(char * str) {
    unsigned int len = strlen(str);
    char* left = (char*) malloc(len * sizeof(char));
    int put_indx = 0;
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if (is_left(c))
            left[put_indx++] = c;
        
        else if (!put_indx && is_right(c)) {
            // unbalanced right parenthesis (or occured before any left parentheses)
            free(left);
            return false;
        }
        
        else if ((is_right(c) && c != valid_half(left[--put_indx]))){
            // right parenthesis but doesn't match the closest left parenthesis
            free(left);
            return false;
        }
    }
    free(left);
    if (put_indx > 0)
        // there are remaining left parentheses unbalanced
        return false;
    return true;
}