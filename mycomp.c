#include "complex.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define UNKONWN_COMMAND_NAME "Undefined command name\n"
#define EXTRA_TEXT  "Extraneous text after end of command\n"
#define UNDEFINED_VAR "Undefined complex variable\n"
#define MISSING_VAR "Missing parameter\n"
#define EOF_FOUND "End of file reached while parsing\n"
#define BAD_COMMA "Illegal comma\n"
#define MISSING_COMMA "Missing comma\n"
#define EXPECTED_NUM "nvalid parameter – not a number\n"

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

int is_valid_command(command_type type){
	switch (type) {
	case UNKONWN_OVERFLOW:
	case UNKONWN:
	case EOFED:
		return 0;
	default:
		return 1;
	};
}

const size_t NUM_COMMANDS = 9;
#define COMMAND_MAX_SIZE  15/*15=sizeof("mult_comp_comp")*/

/*flush line returns 1 if non whitespace found exits with message on EOF*/
int flush_line(void){
	int c;
	/*try just whitespace*/
	do{
		c=getchar();
	}while(c!='\n'&&isspace(c));

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
		if(!isalpha(c) && c!='_'){
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
	do{
		c=getchar();
	}while(c!='\n'&&isspace(c));

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

char pop_char(){
	int c=getchar();
	if(c==EOF){
		printf(EOF_FOUND);
		exit(1);
	}
	return c;
}

int check_bad_comma(){
	char c=pop_char();
	if(c==','){
		printf(BAD_COMMA);
		flush_line();
		return 1;
	}
	ungetc(c,stdin);
	return 0;
}

int read_good_comma(){
	char c;

	skip_spaces();
	c=pop_char();
	if(c==','){
		return 1;
	}


	printf(MISSING_COMMA);
	
	ungetc(c,stdin);
	flush_line();
	return 0;
}

int check_line_end(){
	int c;
	skip_spaces();
	c = pop_char();
	ungetc(c,stdin);

	if(c=='\n'){
		return 1;
	}

	return 0;
}

int read_float(float* fp){
	if(check_line_end()){
		printf(MISSING_VAR);
		return 1;
	}

	if(scanf("%f",fp)!=1){
		printf(EXPECTED_NUM);
		return 1;
	}

	return 0;
}

void read_comp_func(complex vars[6]){
	complex ans;

	int id = read_var_index();
	if(id < 0) return;
	
	if(!read_good_comma()) return;
	if(!read_float(&ans.x)) return;
	if(!read_good_comma()) return;
	if(!read_float(&ans.y)) return;

	if(flush_line()){
		printf(EXTRA_TEXT);
	}

	vars[id] = ans;
}

void print_comp_func(complex vars[6]){
	int id = read_var_index();
	if(id < 0) return;

	if(flush_line()){
		printf(EXTRA_TEXT); 	
	}

	print_comp(vars[id]); putchar('\n');
}



int main(void){
	complex vars[6] = {0};
	char command_buffer[COMMAND_MAX_SIZE+1];
	command_type type;
	
	for(;;){
		type = read_type(command_buffer);

		printf("got (len = %ld): %s\n",strlen(command_buffer),command_buffer);

		skip_spaces();
		/*check for bad comma message*/
		if(
			is_valid_command(type)
			&&check_bad_comma()
		){
			flush_line();
			continue;
		};
		

		switch(type){
		case READ_COMP:
			read_comp_func(vars);
			break;
		case PRINT_COMP:
			print_comp_func(vars);
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
			if(flush_line() || command_buffer[0]){
				printf(UNKONWN_COMMAND_NAME);
			}
			/*printf("found string %s\n",command_buffer);*/

			break;


		default:
			flush_line();
			printf("not implemented\n");
		}


	}
	

	return 1;
}