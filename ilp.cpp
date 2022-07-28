#include"function_map.hpp"
#include<cstdint>

extern "C"
uint64_t* wide_1(uint64_t threads, uint64_t * data, uint64_t size, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	register uint64_t i =0;
	for(i = 0; i < size; i++) {
		i = i+1;
		i = i+1;       
		i = i+1;       
		i = i+1;       
		i = i+1;
	}
	data[0] = i;
	return data;
}
FUNCTION(one_array_2arg, wide_1);

extern "C"
uint64_t* wide_2(uint64_t threads, uint64_t * data, uint64_t size, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	register uint64_t a = 4;
	register uint64_t b = 4;
	register uint64_t c = 4;
	register uint64_t d= 4;
	register uint64_t e =4 ;
	register uint64_t f= size;

	for(register uint64_t i = 0; i < size; i++) {
		i = i+1;  a = a+1; 
		i = i+1;	 a = a+1; 
		i = i+1;	 a = a+1; 
		i = i+1;	 a = a+1; 
		i = i+1;	 a = a+1; 
	}
	data[0] = a + b + c +d + e + f;
	return data;
}
FUNCTION(one_array_2arg, wide_2);

extern "C"
uint64_t* wide_3(uint64_t threads, uint64_t * data, uint64_t size, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	register uint64_t a = 4;
	register uint64_t b = 4;
	register uint64_t c = 4;
	register uint64_t d = 4;
	register uint64_t e =4 ;
	register uint64_t f= size;

	for(register uint64_t i = 0; i < size; i++) {
		i = i+1;  a = a+1; b = b+1;  
		i = i+1;	 a = a+1; b = b+1;  
		i = i+1;	 a = a+1; b = b+1;  
		i = i+1;	 a = a+1; b = b+1;  
		i = i+1;	 a = a+1; b = b+1;  
	}
	data[0] = a + b + c +d + e + f;
	return data;
}
FUNCTION(one_array_2arg, wide_3);

extern "C"
uint64_t* wide_4(uint64_t threads, uint64_t * data, uint64_t size, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	register uint64_t a = 4;
	register uint64_t b = 4;
	register uint64_t c = 4;
	register uint64_t d = 4;
	register uint64_t e = 4 ;
	register uint64_t f= size;

	for(register uint64_t i = 0; i < size; i++) {
		i = i+1;  a = a+1; b = b+1;  d = d+1; 
		i = i+1;	 a = a+1; b = b+1;  d = d+1; 
		i = i+1;	 a = a+1; b = b+1;  d = d+1; 
		i = i+1;	 a = a+1; b = b+1;  d = d+1; 
		i = i+1;	 a = a+1; b = b+1;  d = d+1; 
	}
	data[0] = a + b + c +d + e + f;
	return data;
}
FUNCTION(one_array_2arg, wide_4);
extern "C"
uint64_t* wide_5(uint64_t threads, uint64_t * data, uint64_t size, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	register uint64_t a = 4;
	register uint64_t b = 4;
	register uint64_t c = 4;
	register uint64_t d= 4;
	register uint64_t e =4 ;
	register uint64_t f= size;

	for(register uint64_t i = 0; i < size; i++) {
		i = i+1;  a = a+1; b = b+1;  d = d+1; e = e+1;
		i = i+1;	 a = a+1; b = b+1;  d = d+1; e = e+1;
		i = i+1;	 a = a+1; b = b+1;  d = d+1; e = e+1;
		i = i+1;	 a = a+1; b = b+1;  d = d+1; e = e+1;
		i = i+1;	 a = a+1; b = b+1;  d = d+1; e = e+1;
	}
	data[0] = a + b + c +d + e + f;
	return data;
}

FUNCTION(one_array_2arg, wide_5);
