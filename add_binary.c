/*
Given two binary strings, return their sum (also a binary string).
The input strings are both non-empty and contains only characters 1 or 0.

Runtime: 4 ms, faster than 56.02% of C online submissions for Add Binary.
Memory Usage: 7.4 MB, less than 25.00% of C online submissions for Add Binary.
*/
#include <string.h>
#include <stdlib.h>

typedef struct digit_adder {
    int add;
    int carry_over;
} digit_adder;

digit_adder* add_digit(int a, int b, int c) {
    // c is the carry-over bit from the previous addition
    digit_adder* result = (digit_adder*) malloc(sizeof(digit_adder));
    digit_adder tmp; // declare on stack because it won't leave function
    // add first a and c
    tmp.add = a ^ c;
    tmp.carry_over = a & c; // there is only carry-over when 1+1 == 10;

    // adding the result of adding (a and c) to b
    result->add = tmp.add ^ b;
    /* There are two sources of carry-over: the first addition or the second
        one. But the or is exclusive; not both of them can lead to carry-over
        if the first addition leads to carry-over, then that means we added
        1+1 = 0 and carry-over of 1. So the second addition involves adding
        0 to something, and that cannot have carry-over. On the other hand,
        the only way the second addition can have carry-over is it involves
        a 1, and thus the first addition cannot have had carry-over, since
        carry over would lead to 0 and carry-over of 1
    */
    result->carry_over = tmp.carry_over | (b & tmp.add);

    return result;
}

char* addBinary(char* a, char* b) {
    int i = strlen(a) - 1; // this will traverse a
    int j = strlen(b) - 1; // this will traverse b
    // for example "1" + "1" == "10" so there can be an additional digit
    unsigned int len_ans = i > j ? i+2 : j+2;
    char* ans = (char*) malloc(sizeof(char) * (len_ans+1));
    ans[len_ans] = '\0';
    int k = len_ans - 1; // this will traverse ans
    digit_adder* p = NULL;
    int c = 0; // carry-over
    // we need to traverse the strings in reverse
    // because numbers are read and added right to left
    while (i >= 0 && j >= 0) {
        p = add_digit(a[i--] - '0', b[j--] - '0', c);
        ans[k--] = p->add + '0';
        c = p->carry_over;
    }

    while (i >= 0) {
        // a was longer than b and wasn't entirely consumed
        p = add_digit(a[i--] - '0', c, 0);
        ans[k--] = p->add + '0';
        c = p->carry_over;
    }
    while (j >= 0) {
        // b was the longer string and has digits remaining
        p = add_digit(b[j--] - '0', c, 0);
        ans[k--] = p->add + '0';
        c = p->carry_over;
    }

    if (c) {
        ans[k--] = '1';
    }
    
    if (k >= 0) {
        ans[k--] = '0';
    }

    free(p);
    if (ans[0] == '0')
        // silly hack: LeetCode doesn't like a leading 0
        return ans+1;
    return ans;
}
