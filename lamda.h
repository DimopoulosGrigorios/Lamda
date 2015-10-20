#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

//------------------------------------------//
int MAX=1000;
char * globalstring;
//------------------------------------------//
typedef struct parenthesis parenthesis;
typedef struct parenthesis
{
    int num;
    int kids;
    char * content;
    parenthesis ** subparenthesis;
    parenthesis * parent;
    char ** variables;
    int depth;
}parenthesis;
//------------------------------------------//
void printtree(parenthesis * par);
void removetree(parenthesis * par);
parenthesis* addpar(char * content,parenthesis * parent,parenthesis* current,int num,int depth);
char * b_transformation(char  *input);
int eatparethesis(char * input);
int take_next_parenthesis(char  *input,char  *output,int start);
char *varNparenthesis(char ** before_parenthesis,char  *input,char  *output,char ** after_parenthesis);
char *replace_str(char *str, char *orig, char *rep, int start);
char *replacement(char *str, char *orig, char *rep);
char * replace_with_trees(parenthesis * input);
char * normalization_part(char * input);
void wrapwithparenthesis(char * input);
parenthesis* super_replace(parenthesis* par,char *orig,char *rep);
void rename_variables(parenthesis* par,char * input,parenthesis* par2);
parenthesis * simplify(parenthesis * input);
int isthere(parenthesis * input,char* string);
parenthesis* etta_transformation(parenthesis * input);
static void handler(int signum);
int check_theinput(char* input);
void whitespaceignore(char * input);











