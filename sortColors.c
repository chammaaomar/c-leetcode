/*
Given an array with n objects colored red, white or blue, sort them in-place so
that objects of the same color are adjacent, with the colors in the order red, white and blue.

Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.

Note: You are not supposed to use the library's sort function for this problem.

A rather straight forward solution is a two-pass algorithm using counting sort.
First, iterate the array counting number of 0's, 1's, and 2's, then overwrite array
with total number of 0's, then 1's and followed by 2's.

Could you come up with a one-pass algorithm using only constant space?

Runtime: 0 ms, faster than 100.00% of C online submissions for Sort Colors.
Memory Usage: 7.1 MB, less than 100.00% of C online submissions for Sort Colors.
*/

#include <stdio.h>

void interchange(int i, int j, int* arr) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        return;
}

void sortColors(int* nums, int numsSize) {
    // single pass solution using O(1) memory
    // this number specifies where a 2 would end up; it is decremented upon interchange
    int upper_ind = numsSize - 1;
    // this number specifies where a 0 would end up; it is incremented upon interchange
    int lower_ind = 0;
    int i = 0;
    while (i < numsSize) {
        if (nums[i] == 2) {
            if (upper_ind > i)
                interchange(i, upper_ind--, nums); // if interchanged, don't increment
            else i++;
        }
        else if (nums[i] == 0) {
            if (lower_ind < i)
                interchange(i, lower_ind++, nums); // if interchanged, don't increment
            else i++;
        }
        else i++;
    }

    return;
}
