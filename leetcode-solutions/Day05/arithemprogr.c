#include <stdio.h>    
#include <stdbool.h> 

void sortArray(int arr[], int size) {
    
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
           		int temp = arr[j];      // Hold first card temporarily
                arr[j] = arr[j + 1];    // Put second card in first position
                arr[j + 1] = temp;      // Put first card in second position
            }
        }
    }
}

bool canMakeArithmeticProgression(int arr[], int size) {
    if (size <= 1) {
        return true;
    }
    
    if (size == 2) {
        return true;
    }
    sortArray(arr, size);
    
    int commonDifference = arr[1] - arr[0];
    
    // Step 3: Check if ALL pairs have the same step size
    for (int i = 2; i < size; i++) {
        int currentDifference = arr[i] - arr[i - 1];
        
        // If any step is different, it's NOT arithmetic progression
        if (currentDifference != commonDifference) {
            return false;  // Not an arithmetic progression
        }
    }
    
    // If we reach here, all steps are the same!
    return true;  // Yes, it's an arithmetic progression!
}

// Helper function to print an array nicely
void printArray(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]");
}

// The main function - this is where our program starts!
int main() {
    printf("=== Arithmetic Progression Checker ===\n\n");
    printf("An arithmetic progression has the same difference between consecutive numbers.\n");
    printf("Example: 2, 5, 8, 11 (difference is always +3)\n\n");
    
    // Test Case 1: Should return true
    printf("TEST 1:\n");
    int test1[] = {3, 5, 1};  // This is our array of numbers
    int size1 = 3;            // How many numbers we have
    
    printf("Original array: ");
    printArray(test1, size1);
    
    bool result1 = canMakeArithmeticProgression(test1, size1);
    
    printf("\nAfter sorting: ");
    printArray(test1, size1);  // Now it's sorted: [1, 3, 5]
    
    if (result1) {
        printf("\nResult: TRUE - Can form arithmetic progression!");
        printf("\nExplanation: 1 ? 3 ? 5 (each step is +2)");
    } else {
        printf("\nResult: FALSE - Cannot form arithmetic progression!");
    }
    
    printf("\n\n" "=" * 50 "\n\n");
    
    // Test Case 2: Should return false
    printf("TEST 2:\n");
    int test2[] = {1, 2, 4};
    int size2 = 3;
    
    printf("Original array: ");
    printArray(test2, size2);
    
    bool result2 = canMakeArithmeticProgression(test2, size2);
    
    printf("\nAfter sorting: ");
    printArray(test2, size2);
    
    if (result2) {
        printf("\nResult: TRUE - Can form arithmetic progression!");
    } else {
        printf("\nResult: FALSE - Cannot form arithmetic progression!");
        printf("\nExplanation: 1 ? 2 ? 4 (steps are +1, then +2 - not the same!)");
    }
    
    printf("\n\n" "=" * 50 "\n\n");
    
    // Test Case 3: Edge case with 2 numbers
    printf("TEST 3 (Edge Case):\n");
    int test3[] = {10, 5};
    int size3 = 2;
    
    printf("Original array: ");
    printArray(test3, size3);
    
    bool result3 = canMakeArithmeticProgression(test3, size3);
    
    if (result3) {
        printf("\nResult: TRUE - Any 2 numbers always form arithmetic progression!");
    }
    
    printf("\n\nProgram finished successfully!\n");
    
    return 0;  // Tell the computer our program ended successfully
}