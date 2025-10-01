#include <stdio.h>
#include <stdbool.h>

bool isMonotonic(int nums[], int n) {
    int i = 0;

    // Step 1: find the direction
    while (i < n - 1 && nums[i] == nums[i + 1]) {
        i++;
    }

    if (i == n - 1) {
        return true; // all elements are equal
    }

    bool increasing;
    if (nums[i] < nums[i + 1]) {
        increasing = true;
    } else {
        increasing = false;
    }

    // Step 2: check the rest
    i++;
    while (i < n) {
        if (increasing) {
            if (nums[i] < nums[i - 1]) {
                return false; // violation in increasing order
            }
        } else {
            if (nums[i] > nums[i - 1]) {
                return false; // violation in decreasing order
            }
        }
        i++;
    }

    return true;
}

#include <stdio.h>
#include <stdbool.h>

bool isMonotonic(int nums[], int n) {
    int i = 0;

    // Step 1: find the direction
    while (i < n - 1 && nums[i] == nums[i + 1]) {
        i++;
    }

    if (i == n - 1) {
        return true; // all elements are equal
    }

    bool increasing;
    if (nums[i] < nums[i + 1]) {
        increasing = true;
    } else {
        increasing = false;
    }

    // Step 2: check the rest
    i++;
    while (i < n) {
        if (increasing) {
            if (nums[i] < nums[i - 1]) {
                return false; // violation in increasing order
            }
        } else {
            if (nums[i] > nums[i - 1]) {
                return false; // violation in decreasing order
            }
        }
        i++;
    }

    return true;
}

int main() {
    int nums1[] = {1, 2, 2, 3};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    printf("%s\n", isMonotonic(nums1, n1) ? "true" : "false"); // true

    int nums2[] = {6, 5, 4, 4};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    printf("%s\n", isMonotonic(nums2, n2) ? "true" : "false"); // true

    int nums3[] = {1, 3, 2};
    int n3 = sizeof(nums3) / sizeof(nums3[0]);
    printf("%s\n", isMonotonic(nums3, n3) ? "true" : "false"); // false

    return 0;
}
