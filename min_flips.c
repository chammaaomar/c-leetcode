/* Given 3 positives numbers a, b and c. Return the minimum flips required in some 
bits of a and b to make ( a OR b == c ). (bitwise OR operation). Flip operation
consists of change any single bit 1 to 0 or change the bit 0 to 1 in their binary
representation.

Runtime: 0 ms, faster than 100.00% of C online submissions for Minimum Flips to Make a OR b Equal to c.
Memory Usage: 6.6 MB, less than 100.00% of C online submissions for Minimum Flips to Make a OR b Equal to c.
*/

#include <stdio.h>
#define getBit(w, j) (((w) >> j) & 1)
int minFlips(int a, int b, int c){
    int aBit = 0;
    int bBit = 0;
    int cBit = 0;
    int orBit = 0;
    int diffBit = 0;
    int maxBits = sizeof(int) * 8; // 1 byte == 8 bits
    int flips = 0;
    for (int i = 0; i < maxBits; i++) {
        // solution checks bit by bit
        aBit = getBit(a,i);
        bBit = getBit(b,i);
        cBit = getBit(c,i);
        orBit = aBit || bBit;
        diffBit = orBit ^ cBit;
        if (!diffBit) {
            // no flip necessary
            continue;
        }
        if (cBit) {
            // flip necessary
            // the bit in the bit vector a | b is different from
            // the bit in c, and the bit in c is 1; thus both bits
            // in a and b are 0; one flip of either of them is required
            flips++;
            continue;
        }
        if (aBit && bBit) {
            // flip necessary
            // in this case, the bit in c is turned off, and both a and b bits
            // are turned on; thus in order to turn off the bit in a | b, both
            // must be turned off; two flips necessary
            flips += 2;
            continue;
        }
        // finally, the bit in c is turned off and either one of a or b bits are turned
        // on. Thus, we need to turn one off; one flip necessary.
        flips++;
    }
    
    return flips;
}

