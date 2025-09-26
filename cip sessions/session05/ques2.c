#include<stdio.h>
int main(void) {
    int N;
    printf("Enter a number: ");
    scanf("%d", &N);
    
    for(int i = 1; i <= N; i++) {
        if(i % 2 == 0) {
            printf("%d is EVEN\n", i);
        } else {
            printf("%d is ODD\n", i);
        }
    }
    
    return 0;
}