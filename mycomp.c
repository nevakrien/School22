#include "complex.h"
#include <string.h>

typedef enum command_type{
	UNKONWN_OVERFLOW=-3,
	UNKONWN =-2,
	EOFED =-1, /*if we see eof when reading*/

	READ_COMP =0,/*order matters here!!!*/
	PRINT_COMP,
	STOP,

	ADD_COMP,
	SUB_COMP,

	MULT_COMP_REAL,
	MULT_COMP_IMG,
	MULT_COMP_COMP,

	ABS_COMP

} command_type;

const size_t NUM_COMMANDS = 9;
#define COMMAND_MAX_SIZE  15/*15=sizeof("mult_comp_comp")*/

/*flush line returns 1 if non whitespace found -1 if EOF -2 if junk followed by EOF*/
int flush_line(void){
	int c;
	/*try just whitespace*/
	do{
		c=getchar();
	}while(c== ' ' || c=='\t');

	if(c=='\n') return 0;
	if(c==EOF) return -1;

	/*found junk flush it*/
	do{
		c=getchar();
	}while(c!= '\n' && c!=EOF);

	if(c==EOF) return -2;
	return 1;

}

command_type read_type(char buffer[COMMAND_MAX_SIZE+1]){
	int i,c;

	const char* commands[] = {
	    "read_comp",/*order matters here!!!*/
	    "print_comp",
	    "stop",

	    "add_comp",
	    "sub_comp",

	    "mult_comp_real",
	    "mult_comp_img",
	    "mult_comp_comp",

	    "abs_comp"
	};

	/*get the word in*/
	for(i=0;i<COMMAND_MAX_SIZE;i++){
		c = getchar();
		if(c==EOF) return EOFED;
		if(c==' ') break;
		if(c=='\n'){
			ungetc('\n',stdin);
			break;
		}

		buffer[i] = c;
	}
	buffer[i] = '\0';
	if(i==COMMAND_MAX_SIZE) return UNKONWN_OVERFLOW;
	

	/*
	simple read could be made much more effishent 
	by reading mult abs etc sepratly but thats less clear and reads should still be VERY fast either way
	*/
	for(i=0;i<NUM_COMMANDS;i++){
		if(strcmp(buffer,commands[i]) == 0) 
			return i;
	}

	return UNKONWN;
}

#define EXTRA_TEXT  "Extraneous text after end of command\n"

int main(void){
	char command_buffer[COMMAND_MAX_SIZE+1];
	command_type type;
	
	for(;;){
	loop_start:

		type = read_type(command_buffer);
		switch(type){
		case STOP:
			if(flush_line()){
				printf(EXTRA_TEXT);
				goto loop_start;
			}
			return 0;
		default:
			flush_line();
			printf("not implemented\n");
		}

	}
	

	return 0;
}