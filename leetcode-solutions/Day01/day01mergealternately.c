#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * mergeAlternately(char * word1, char * word2) {
    int len1 = strlen(word1);
    int len2 = strlen(word2);
    int total_len = len1 + len2;
    
    // Allocate memory for the result string (+1 for null terminator)
    char* result = (char*)malloc((total_len + 1) * sizeof(char));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    int i = 0, j = 0, k = 0;
    
    // Merge characters alternately while both strings have characters
    while (i < len1 && j < len2) {
        result[k++] = word1[i++];
        result[k++] = word2[j++];
    }
    
    // Append remaining characters from word1 (if any)
    while (i < len1) {
        result[k++] = word1[i++];
    }
    
    // Append remaining characters from word2 (if any)
    while (j < len2) {
        result[k++] = word2[j++];
    }
    
    // Null terminate the string
    result[k] = '\0';
    
    return result;
}
