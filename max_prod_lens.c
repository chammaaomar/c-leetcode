#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Given a string array words, find the maximum value of length(word[i]) * length(word[j])
    where the two words do not share common letters. You may assume that each word
    will contain only lower case letters. If no such two words exist, return 0.

    Runtime: 28 ms, faster than 76.32% of C online submissions for Maximum Product of Word Lengths.
    Memory Usage: 9.1 MB, less than 100.00% of C online submissions for Maximum Product of Word Lengths.
*/

int str2num(char* str) {
    // represents each string as a 26-bit vector; a bit is turned on
    // if the corresponding alphabet letter is present in the string
    int num = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        num |= 1 << (str[i] - 'a');
    }

    return num;
}

int charInCommon(int i, int j, int* wordNums) {
    // checks if a common bit is turned on; i.e. an alphabetical
    // character is shared between the two strings.
    // this function indexes into a pre-computed array so we 
    // don't have to compute the bit vector representation
    // on each function call
    int bitVector_i = wordNums[i];
    int bitVector_j = wordNums[j];
    return bitVector_i & bitVector_j;
}

int maxProduct(char** words, int wordsSize) {
    if (!wordsSize)
        return 0;
    int prod = 0;
    int candidate = 0;
    int i = 0;
    int* wordNums = (int*) calloc(sizeof(int), wordsSize);
    int* wordLens = (int*) calloc(sizeof(int), wordsSize);
    wordLens[i] = strlen(words[i]);
    wordNums[i] = str2num(words[i]);
    // We iterate once initially to calculate the lengths and bit-representations
    for (int j = i+1; j < wordsSize; j++) {
        wordLens[j] = strlen(words[j]);
        wordNums[j] = str2num(words[j]);
        candidate = wordLens[i] * wordLens[j];
        /* if the product of length is too small, it isn't worth checking whether
            the stringshave a character in common. This is achieved by using the
            logical and &&. If the first condition isn't met, the second condition
            isn't checked, thus avoiding the computation. 
        */
        if (candidate > prod && !charInCommon(i, j, wordNums)) {
            prod = candidate;
        }
    }
    
    for (i = 1; i < wordsSize; i++) {
        for (int j = i+1; j < wordsSize; j++) {
            candidate = wordLens[i] * wordLens[j];
            if (candidate > prod && !charInCommon(i, j, wordNums)) {
                prod = candidate;
            }
        }
    }
    
    free(wordNums);
    free(wordLens);
    return prod;
    
}

