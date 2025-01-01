#include "complex.h"

void print_comp(complex a){
	printf("%6f ",a.x);
	if(a.y>=0) putchar('+');
	printf("%6fi",a.y);
}

complex add_comp(complex a,complex b){
	complex ans;

	ans.x = a.x+b.x;
	ans.y = a.y+b.y;

	return ans;
}

complex sub_comp(complex a,complex b){
	complex ans;

	ans.x = a.x-b.x;
	ans.y = a.y-b.y;
	
	return ans;
}

complex mult_comp_real(complex a,float b){
	complex ans;

	ans.x = a.x*b;
	ans.y = a.y*b;
	return ans;
}
complex mult_comp_img(complex a,float b){
	complex ans;
	
	ans.x = -b*a.y;
	ans.y = b*a.x;
	return ans;
}

complex mult_comp_comp(complex a,complex b){
	complex ans;

	ans.x = a.x*b.x-a.y*b.y;
	ans.y = a.x*b.y+a.y*b.x;
	
	return ans;
}

float abs_comp(complex a){
	return sqrt(a.x*a.x+a.y*a.y);
}