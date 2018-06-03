#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
double pi(int n) {
	double x, y;
	unsigned long m = 0, i;
 
	for (i = 0; i < n; i++) {
		x = rand() / (RAND_MAX + 1.0);
		y = rand() / (RAND_MAX + 1.0);
		if (x * x + y * y < 1) m++;
	}

	return 4 * (double) m / n;
}
 
int main() {
	printf("Pi is %f\n", pi(1000000));
	return 0;
}
