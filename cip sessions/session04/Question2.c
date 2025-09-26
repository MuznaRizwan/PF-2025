#include<stdio.h>
//Initialise variables
char oper='0', withdraw='0', deposit='0';
float balance=0.0;
int main(void) {
	//Input ATM operation by user
	printf("Enter your balance: ");
	scanf("%f", &balance);
	printf("Chose between the ATM operations [1]Check balance [2]Withdraw money [3]Deposit money: ");
	scanf(" %c", &oper);
	switch(oper) {
		case '1':
			printf("Your balance is %0.2f",balance);
			break;
		case '2':
			printf("Enter the amount of money to withdraw [1]500 [2]1000 [3]5000: ");
			scanf(" %c", &withdraw);
			if(withdraw =='1') {
				if(balance>=500.00) {
					printf("\nRs 500 have been withdrawn\n");
					balance-=500.00;
					printf("Your current balance is %0.2f",balance);
				} else {
					printf("\nInsufficient balance");
				}
			} else if(withdraw =='2') {
				if(balance>=1000.00) {
					printf("\nRs 1000 have been withdrawn\n");
					balance-=1000.00;
					printf("Your current balance is %0.2f",balance);
				} else {
					printf("\nInsufficient balance");
				}
			} else if(withdraw =='3') {
				if(balance>=5000.00) {
					printf("\nRs 5000 have been withdrawn\n");
					balance-=5000.00;
					printf("\nYour current balance is %0.2f",balance);
				} else {
					printf("\nInsufficient balance");
				}
			} else {
				printf("Invalid option");
			}
			break;
		case '3':
			printf("Enter the amount of money to deposit [1]100 [2]500 [3]1000 [4]5000: ");
			scanf(" %c", &deposit);
			if(deposit =='1') {
				printf("\nRs 100 have been deposited\n");
				balance+=100.00;
				printf("Your current balance is %0.2f",balance);
			} else if(deposit =='2') {
				printf("\nRs 500 have been deposited\n");
				balance+=500.00;
				printf("Your current balance is %0.2f",balance);
			} else if(deposit =='3') {
				printf("\nRs 1000 have been deposited\n");
				balance+=1000.00;
				printf("Your current balance is %0.2f",balance);
			} else if(deposit =='4') {
				printf("\nRs 5000 have been deposited\n");
				balance+=5000.00;
				printf("Your current balance is %0.2f",balance);
			} else {
				printf("Invalid option");
			}
			break;
		default:
			printf("Invalid option");
			break;
	}
}