#include <stdio.h>
int arraySign(int* nums, int numsSize) {
	int sign = 1;
	for(int i=0; i<numsSize; i++) {
		if (nums[i] == 0) {
			return 0;
		}
		if (nums[i] < 0) {
			sign=-sign;
		}
		return sign;
	}
	int main(void) {
		int nums1[] = {-1,-2,-3,-4,3,2,1};
		printf("%d\n", arraySign(nums1, 7));

		int nums2[] = {1,5,0,2,-3};
		printf("%d\n", arraySign(nums2, 5));

		int nums3[] = {-1,1,-1,1,-1};
		printf("%d\n", arraySign(nums3, 5));
		
		return 0;
	}
