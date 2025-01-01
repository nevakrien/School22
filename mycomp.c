#include "complex.h"
#include <string.h>
#include <stdlib.h>

#define UNKONWN_COMMAND_NAME "Undefined command name\n"
#define EXTRA_TEXT  "Extraneous text after end of command\n"
#define UNDEFINED_VAR "Undefined complex variable\n"
#define MISSING_VAR "Missing parameter\n"
#define EOF_FOUND "End of file reached while parsing\n"


typedef enum command_type{
	UNKONWN_OVERFLOW=-3,
	UNKONWN =-2,
	EOFED =-1,
	
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

/*flush line returns 1 if non whitespace found exits with message on EOF*/
int flush_line(void){
	int c;
	/*try just whitespace*/
	do{
		c=getchar();
	}while(c== ' ' || c=='\t');

	if(c=='\n') return 0;
	if(c==EOF) {
		printf(EOF_FOUND);
		exit(1);
	};
	/*found junk flush it*/
	do{
		c=getchar();
	}while(c!= '\n' && c!=EOF);

	if(c==EOF) {
		printf(EOF_FOUND);
		exit(1);
	};
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
		if(c=='\n'||c==','){
			ungetc(c,stdin);
			break;
		}

		buffer[i] = c;
	}
	buffer[i] = '\0';
	if(i==COMMAND_MAX_SIZE) return UNKONWN_OVERFLOW;
	

	/*
	simple read potentially could be made much more effishent 
	by reading mult abs etc sepratly but thats less clear also add more preasure on the cache
	*/
	for(i=0;i<NUM_COMMANDS;i++){
		if(strcmp(buffer,commands[i]) == 0) 
			return i;
	}

	return UNKONWN;
}


void skip_spaces(){
	int c;
	while((c=getchar())==' '|| c=='\t');

	if(c== EOF){
		printf(EOF_FOUND);
		exit(1);
	}
	ungetc(c,stdin);
}

int read_var_index(){
	char c; 
	int i;

	skip_spaces();
	if(scanf("%c",&c) == 0){
		flush_line();
		return -1;
	}

	if(c=='\n'){
		printf(MISSING_VAR);
		flush_line();
		return -1;
	}
	i = c - 'A'; /*using an asci trick from "the c programing languge"*/
	if(i<0 || i>=6){
		ungetc(c,stdin);
		printf(UNDEFINED_VAR);
		flush_line();
		return -1;
	}

	return i;
}

void read_comp_func(complex vars[6]){
	int id = read_var_index();
	if(id < 0) return;

	printf("not implemented\n");

	if(flush_line()){
		printf(EXTRA_TEXT);
	}
}

int main(void){
	complex vars[6] = {0};
	char command_buffer[COMMAND_MAX_SIZE+1];
	command_type type;
	
	for(;;){
		type = read_type(command_buffer);
		switch(type){
		case READ_COMP:
			read_comp_func(vars);
			break;
		case STOP:
			if(flush_line()){
				printf(EXTRA_TEXT);
				break;
			}
			return 0;
		case EOFED:
			printf("End of file reached in normal way\n");
			return 1;
		case UNKONWN:
		case UNKONWN_OVERFLOW:
			printf(UNKONWN_COMMAND_NAME);
			if(flush_line()<0){
				printf("End of file reached while reading unknown command\n");
				return 1;
			}
			break;


		default:
			flush_line();
			printf("not implemented\n");
		}


	}
	

	return 1;
}