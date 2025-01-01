#ifndef COMPLEX_H
#define COMPLEX_H
#include <stdio.h>

typedef struct complex{
	float x;
	float y;
}complex; /*x+yi*/

/*
note: it would have been better for the optimizer to inline the definitions of basic arithmetic.
This is because SIMD is more likely to be used when the abstract operations are knowen.
if you do want to optimize this remeber to add static

HOWEVER the assiment specified using an object file so we are doing it like they asked
*/

void print_comp(complex a);

complex add_comp(complex a,complex b);
complex sub_comp(complex a,complex b);

complex mult_comp_real(complex a,float b);
complex mult_comp_img(complex a,float b);
complex mult_comp_comp(complex a,complex b);

float abs_comp(complex a);


#endif /*COMPLEX_H*/