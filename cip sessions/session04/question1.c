#include<stdio.h>
//Initialise variables
float amount=0.0, discount=0.0, A_Net=0.0;
int main(void) {
	//Input from user
	printf("Enter the amount of purchase(in Rs): ");
	scanf("%f", &amount);
	if(amount<1000) {
		discount=0;
	} else if(amount>=1000 && amount<=2000) {
		discount=0.05*amount;
	} else if(amount>2000) {
		discount=0.1*amount;
	}
	//calculate price after discount
	A_Net=amount-discount;
	//Display the selling price
	printf("The net amount after discount in Rs is: %0.2f",A_Net);
}