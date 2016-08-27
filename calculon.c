#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include "scanner.h"
#include "linked_list.h"
#include "queue.h"
#include "element.h"

const char* NAME = "Ivan T. Johnson";
const char* FLAG_SHOWNAME="v";
const char* FLAG_PRINTPOSTFIX="d";

//TODO MAKE SURE USING MALLOC PROPERLY; ESP. SIZEOF(CHAR)*WHATEVER
/*
char* strCat (char* arg1, char* arg2){
  char *c = malloc(sizeof(char) * (strlen(arg1) + strlen(arg2) + 1));
  if (c == 0){
    printf("out of memory\n");
    exit(1);
  }
  sprintf(c,"%s%s",arg1,arg2);
  return c;
}
//*/

typedef struct{
  bool printName;
  bool printPostfix;
  FILE* file;
} parsedArgs;

void showUsage(char *progName){
  printf("Usage: %s [option] [filename]\n",progName);
}

void validateMemory(void* pointer){
  if (pointer == NULL){
    printf("out of memory\n");
    exit(1);
  }
}

parsedArgs *processArguments (int argc, char **argv){
  parsedArgs *args = malloc(sizeof(parsedArgs));
  validateMemory(args);
  args->printName = false;
  args->printPostfix = false;
  args->file = NULL;
  for (int i = 1; i<argc; i++){
    if (argv[i][0] == '-'){//is a flag
      int j = 1;
      while (argv[i][j] != '\0'){
	if (argv[i][j]==*FLAG_SHOWNAME)
	  args->printName = true;
	else if (argv[i][j]==*FLAG_PRINTPOSTFIX)
	  args->printPostfix = true;
	else {
	  printf("Error: \'%c\' is not a valid option for %s\n",argv[i][j],argv[0]);
	  exit(1);
	}
	j++;
      }
    } else {
      if (args->file != NULL){
	printf("Error: received two or more filename parameters");
	exit(1);
      }
      args->file = fopen(argv[i],"r");
    }
  }
  if (args->file == NULL)
    args->file = stdin;
  return args;
}

void* readExpression(FILE *f){
  Element *v=NULL;
  void* queue = new_queue();
  
  while (!feof(f)) {
    if (stringPending(f)){
      v = newElementString(readString(f));
    } else {
      char *str = readToken(f);
      if (str == NULL){
	continue;
      }
      if (strchr(str,'.') != 0)
	v = newElementDouble(atof(str));
      else if (*str == '-' || isdigit(*str))
	v = newElementInteger(atoi(str));
      else if (!isalpha(*str) || strcmp(str,ELEMENT_OPERATOR_VARIABLE_DECLARATION)==0)
	if (*str == ';')
	  return queue;
	else
	  v = newElementOperator(str);
      else
	v = newElementVariable(str);
      str = NULL;//completely unneccessary, but it makes me happy.
    }
    if (v != NULL)
      enqueue(queue, v);
  }
  return queue;
}


int main(int argc, char **argv) {
  parsedArgs *args = processArguments(argc, argv);
  if (args->printName){
    printf("%s",NAME);
    return 0;
  }
  void* infix = readExpression(args->file);
  int i = 0;
  
  while (size(infix)>0){//TMP
    Element *e = (Element *)dequeue(infix);
    char *text = elementToString(e);
    printf("%i: %s\n",i,text);
    free(text);
    i++;
  }
  /*
  quene* postfix = convert(infix);
  if (args->printPostfix){
    printf(toString(postfix));
    return 0;
  }
  stack* reversePostfix = reverse(postfix);
  printf(evaluate(reversePostfix));
  */
  return 0;
}