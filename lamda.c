# include "lamda.h"

//--------------------------------------------------------------------------------------------------------------------//
//Typwnei to dentro me ta dedomena enos l-orou
void printtree(parenthesis * par)
{
    int i;
    if (!(par->content))return;
    printf("############\n");
    printf("Number of Node:%d \n Content of Node:%s \n Number of kids:%d \n",par->num,par->content,par->kids);
    printf("--VARIABLES--\n");
    for (i=0;((strlen(par->variables[i]))>0&&(i<=par->depth));i++)
    {
        printf("\t Variable(%d):%s\n",i,par->variables[i]);
    }
    printf("############\n");
    parenthesis ** kido=par->subparenthesis;
    i=0;
    while (i<(par->kids))
    {
        printtree(kido[i]);
        i++;
    }
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Diagrafh to dentro kai ta dedomena tou l-orou
void removetree(parenthesis * par)
{
    int i;
    if (!(par->content))return;
    parenthesis ** kido=par->subparenthesis;
    i=0;
    while (i<(par->kids))
    {
        removetree(kido[i]);
        free(kido[i]);
        i++;
    }
    for (i=0;(i<MAX);i++)
    {
        free((par->variables)[i]);
    }
    free(par->variables);
    free(par->content);
    free(par->subparenthesis);
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Dhmiourgia tou dentrou apo ena string
parenthesis* addpar(char * content,parenthesis * parent,parenthesis* current,int num,int depth)
{
    //---------------Initializing-----------------Memory Allocation---------------------------------//
    int next=0;
    int i=0;
    char * temporal=(char *) malloc(MAX);
    if(temporal  == NULL)
    {
		fprintf(stderr, "out of memory\n");
		return NULL;
    }
    char * inputemp=(char *) malloc(MAX);
    if(inputemp == NULL)
    {
		fprintf(stderr, "out of memory\n");
		return NULL;
    }
    char * input=(char*)malloc(MAX);
    if(input == NULL)
    {
		fprintf(stderr, "out of memory\n");
		return NULL;
    }
    if (current==NULL)
    {
        current=(parenthesis*) malloc(sizeof(parenthesis));
        if(current == NULL)
        {
            fprintf(stderr, "out of memory\n");
            return NULL;
        }
    }
    parenthesis ** newkids=(parenthesis**) malloc(sizeof(parenthesis*));
    if(newkids == NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    current->content=(char *)malloc(MAX);
    strcpy(current->content,content);
    current->parent=parent;
    current->num=num;
    current->depth=depth;
    if (parent){(parent->subparenthesis)[num]=current;}
    //---------------Initializing-----------------Memory Allocation---------------------------------//

    //--------------------------Allocate space for the variables---------------------------------------//
    current->variables = malloc(MAX* sizeof(char *));
	if(current->variables  == NULL)
    {
		fprintf(stderr, "out of memory\n");
		return;
    }
	for(i = 0; i < MAX; i++)
    {
		(current->variables)[i] = malloc(MAX * sizeof(char));
		if(current->variables[i]==NULL)
        {
			fprintf(stderr, "out of memory\n");
			return;
        }
    }
    //--------------------------Allocate space for the variables------------------------------------------------//

    //-----------------------Take the parents variables and add your own------------------------------------------//
    i=0;
    if (parent)
    {
        for (i=0;((strlen(parent->variables[i])>0)&&(i<MAX)&&(i<current->depth));i++)
        {
            strcpy((current->variables)[i],parent->variables[i]);
            if (i==MAX-1){break;}
        }
    }
    if (i<MAX-1){strcpy((current->variables)[i],varNparenthesis(NULL,content,temporal,NULL));}
    //-----------------------Take the parents variables and add your own------------------------------------------//

    //-----------------------Find the subrenthesis and add them to your subtree------------------------------------------//
    i=0;
    current->kids=0;
    strcpy(input,content);
    eatparethesis(input);
    while (next<strlen(input))
    {
        strcpy(inputemp,"");
        next=take_next_parenthesis(input,inputemp,next);
        newkids = (parenthesis **) realloc(newkids,(i+1)*sizeof(parenthesis*));
        if(newkids == NULL)
        {
            fprintf(stderr, "out of memory\n");
            return NULL;
        }
        newkids[i]=(parenthesis*) malloc(sizeof(parenthesis));
        current->subparenthesis=newkids;
        if(newkids[i]== NULL)
        {
            fprintf(stderr, "out of memory\n");
            return NULL;
        }
        if (strlen(inputemp)>0)
        {
            addpar(inputemp,current,(newkids[i]),i,(depth+1));
            i++;
            (current->kids)=(current->kids)+1;
        }
        current->subparenthesis=newkids;
    }
    //-----------------------Find the subrenthesis and add them to your subtree------------------------------------------//

    //-----------------------Free the Allocated memory you dont need anymore------------------------------------------//
    free(inputemp);
    free(input);
    free(temporal);
    //-----------------------Free the Allocated memory you dont need anymore------------------------------------------//
    return current;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Afairese tis perites parenthesis apo ena dentro
parenthesis * simplify(parenthesis * input)
{
    int i=0;
    parenthesis ** kids=input->subparenthesis;
    parenthesis * temporal;
    char * temp=(char*) malloc(MAX);
    if(temp== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    if((input->kids)==1)
    {
        strcpy(temp,kids[0]->content);
        wrapwithparenthesis(temp);
        if(!strcmp(temp,input->content))
        {
            replacement(input->content,input->content, kids[0]->content);
            temporal=addpar(input->content,input->parent,NULL,input->num,input->depth);
            removetree(input);
            free(input);
        }
        else
        {
            return input;
        }
        temporal=simplify(temporal);
        free(temp);
        return temporal;
    }
    else if ((input->kids)>1)
    {
        for(i=0;i<input->kids;i++)
        {
            strcpy(temp,(kids[i]->content));
            kids[i]= simplify(kids[i]);
            replacement(input->content,temp, kids[i]->content);
            temporal=addpar(input->content,input->parent,NULL,input->num,input->depth);
            removetree(input);
            free(input);
            input=temporal;
            kids=input->subparenthesis;
        }
        free(temp);
    }
    else
    {
            free(temp);
            return  input;
    }
    return  input;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Sunarthsh pou perikliei mia ekfrash se parenthseis p.x A->(A)
void wrapwithparenthesis(char * input)
{
    char * output=(char*) malloc(MAX);
    if(output== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return ;
    }
    strcpy(output,"(");
    strcat(output,input);
    strcat(output,")");
    strcpy(input,output);
    free(output);
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Synarthsh pou diagrafh mia ekswterikh parenthesh p.x (A)->A
int eatparethesis(char * input)
{
    int i;
    int flag=0;
    for(i=0;(input[i]!='\0');i++)
    {
        if (input[i]=='(')
        {
            flag=1;
            memmove(&input[i], &input[i+1],strlen(input)-i);
            break;
        }
    }
    for(i=strlen(input);i>=0;i--)
    {
        if (input[i]==')')
        {
            memmove(&input[i], &input[i+1],strlen(input)-i);
            break;
        }
    }
    return flag;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Sunarthsh pou sou dinei thn epomenh parenthsh apo mia ekfrash p.x (A)(B)(C)->(A)
int take_next_parenthesis(char  *input,char  *output,int start)
{
    int i=start;
    int flag=0;
    int parenthesis=-1;
    int k=0;
    for(i=start;(input[i]!='\0');i++)
    {
        if ((input[i]=='('))
        {
            if (parenthesis==-1)
            {
                parenthesis++;
            }
            parenthesis++;
        }
        if((input[i]==')'))
        {
            parenthesis--;
        }
        if (parenthesis>0)
        {
            output[k]=input[i];
            k++;
        }
        if (parenthesis==0)
        {
            output[k]=input[i];
            k++;
            output[k]='\0';
            break;
        }
    }
    return i+1;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Pernei ws eisodo mia ekfrash ths morfhs (\var.(smthing)) kai epistrefei var kai smth sto output
char *varNparenthesis(char ** before_parenthesis,char  *input,char  *output,char ** after_parenthesis)
{
    int i=0;
    int flag=0;
    int parenthesis=0;
    char lvar[100];
    char * token=NULL;
    int k=0;
    int parentesis=0;
    for(i=0;(input[i]!='\0');i++)
    {
        if (input[i]=='(')
        {
            parentesis++;
        }
        if (input[i]==')')
        {
            parentesis--;
        }
        if (((input[i]=='\\'))&&(parentesis<2))
        {
            flag=1;
            i++;
            sscanf(&(input[i])," \%s. ",lvar);
            token = strtok(lvar, ".");
            k=take_next_parenthesis(input,output,i);
            if (after_parenthesis!=NULL)
            {
                strcpy(*after_parenthesis,input+k);
                (*after_parenthesis)[strlen(*after_parenthesis)-1]='\0';
            }
            break;
        }
        if (before_parenthesis!=NULL)
        {
             if (input[i+1]!='\\')
            (*before_parenthesis)[i]=input[i];
        }
    }
    if (token==NULL)
    {
        return "null";
    }
    return token;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Synarthsh pou antikathista sto str opou brei orig to rep arxizwntas apo to start
char *replace_str(char *str, char *orig, char *rep, int start)
{
  static char temp[4096];
  static char buffer[4096];
  char *p;
  strcpy(temp, str + start);
  if(!(p = strstr(temp, orig)))
    {
       return str;//temp;
   }
  strncpy(buffer, temp, p-temp); // Copy characters from 'temp' start to 'orig' str
  buffer[p-temp] = '\0';
  sprintf(buffer + (p - temp), "%s%s", rep, p + strlen(orig));
  sprintf(str + start, "%s", buffer);
  return str;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Synarthsh pou antikathista se olo to str opou orig se rep
char *replacement(char *str, char *orig, char *rep)
{
    int i=0;
    char temp[4096];
    char temp2[100];
    char * sptr;
    if (!strcmp(rep,orig))
    {
        return str;
    }
    sptr=strpbrk(rep,orig);
    if (sptr!=NULL){strcpy(temp2,sptr);}
    char rep2[3]="_";
    if(sptr!=NULL)
    {
        temp2[1]='\0';
        rep=replacement(rep,orig,rep2);
    }
    while(1)
    {
        strcpy(temp,str);
        if (str[i]=='\0')
        {
            break;
        }
        str=replace_str(str,orig,rep,i);
        if (!strcmp(temp,str))
        {
            break;
        }
        i++;
    }
    if(sptr!=NULL)
    {
        str=replacement(str,rep2,orig);
    }
    return str;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Synarthsh pou psaxnei ena string se ena dentro
int isthere(parenthesis * input,char* string)
{
    int i;
    int flag=0;
    if(!strcmp(input->content,string)){return 1;}
    for (i=0;i<input->kids;i++)
    {
        if(isthere(input->subparenthesis[i],string)){flag=1; break;}
    }
    return flag;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Htta kanonikopoihsh an exeis (\x.((something)(x)))->(\x.(something)) to x den anoikei sto something
parenthesis* etta_transformation(parenthesis * input)
{
    int i;
    int j;
    int varflag=-1;
    int otherkidflag=-1;
    char temp[MAX];
    char temp2[MAX];
    parenthesis * temporal=input;
    parenthesis * temporal2=input;
    if (input->kids>0)
    {
        for(i=0;i<input->kids;i++)
        {
            temporal2=input->subparenthesis[i];
            temporal=input->subparenthesis[i];
            temporal=etta_transformation(temporal);
            if (temporal!=temporal2)
            {
                return temporal;
            }
        }
        if (temporal==temporal2){return input;}
    }
    else
    {
        strcpy(temp,input->content);
        eatparethesis(temp);
        if (input->parent)
        {
            if ((input->num==input->parent->kids-1)&&(input->num!=0))
            {
                for(j=0;j<=input->depth;j++)
                {
                    if (!strcmp(input->variables[j],temp))
                    {
                            varflag=j;
                            break;
                    }
                }
            }
            for(j=0;j<input->parent->kids;j++)
            {
                if (j==input->num) continue;
                if (isthere(input->parent->subparenthesis[j],input->content))
                {
                    otherkidflag=j;
                    break;
                }
            }
            if ((varflag>-1)&&(otherkidflag==-1))
            {
                    temporal=super_replace(input->parent,input->content,"");
                    return temporal;
            }
        }
    }
    return input;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Kanei antikatastash to rep sto orig se olo to dentro
parenthesis* super_replace(parenthesis* par,char *orig,char *rep)
{
    parenthesis * next=par;
    parenthesis * nextlast=next;
    char lastcontent[MAX];
    char origtemp[MAX];
    char reptemp[MAX];
    char origtemp1[MAX];
    strcpy(origtemp,orig);
    strcpy(reptemp,rep);
    while (next)
    {
        strcpy(origtemp1,next->content);
        replacement(next->content,origtemp,reptemp);
        strcpy(origtemp,origtemp1);
        strcpy(reptemp,next->content);
        nextlast=next;
        next=next->parent;
    }
    strcpy(lastcontent,nextlast->content);
    removetree(nextlast);
    free(next);
    return addpar(lastcontent,NULL,NULL,0,0);
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Synarthsh pou antikathista se mia ekfrash thn prwth b-metatroph pou mporei na ginei apo aristera pros ta deksia se depth 1 tou dentrou
//p,x ((1)(2)(\x.(x))(3)(4))->((1)(2)((3))(4))
char * replace_with_trees(parenthesis * input)
{
    //---------------Initializing-----------------Memory Allocation---------------------------------//
    parenthesis** subs=input->subparenthesis;
    parenthesis *sub0=NULL;
    parenthesis *sub1=NULL;
    int i=0;
    int j;
    int flagk=0;
    char * stringsub1=(char*)malloc(MAX);
    if(stringsub1== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    char * stringfinal=(char*)malloc(MAX);
    if(stringfinal== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    char * stringvar=(char*)malloc(MAX);
    if(stringvar== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    char * stringsub0=(char*)malloc(MAX);
    if(stringsub0== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    strcpy(stringsub0,"");
    strcpy(stringfinal,"");
    //---------------Initializing-----------------Memory Allocation---------------------------------//

    //---------------Make the first Replacement---------------------------------//
    i=0;
    while((i)<(input->kids))
    {
        if (subs[i])//check if the first kid is not null and take its content
        {
            strcpy(stringsub0,((subs[i])->content));
        }
        else
        {
            break;
        }
        if ((!flagk)&&(i+1)<(input->kids))//Check if had happened replacement
        {
            if ((subs[i])&&(subs[i+1]))//take the next two not null kids
            {
                if(strcmp(((subs[i]->variables)[subs[i]->depth]),"null"))//check if the first one have variable
                {
                    strcpy(stringsub0,(subs[i]->subparenthesis[0])->content);
                    strcpy(stringvar,((subs[i]->variables)[subs[i]->depth]));
                    strcpy(stringsub1,(subs[i+1]->content));
                    //---------------Rename the variable if you have to---------------------------------//
                    rename_variables(subs[i],stringsub1,subs[i+1]);
                    //---------------Rename the variable if you have to---------------------------------//
                    replacement(stringsub0,stringvar,stringsub1);
                    flagk=1;
                    i++;
                }
            }
            else
            {
                break;
            }
        }
        strcat(stringfinal,stringsub0);
        i++;
    }
    //---------------Make the first Replacement---------------------------------//

    //---------------Free and memory deallocation---------------------------------//
    free(stringsub0);
    free(stringvar);
    free(stringsub1);
    //---------------Free and memory deallocation---------------------------------//

    return stringfinal;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Synarthsh pou xrhsimopoieitai gia metanomasia metablhtwn otan ginetai b kanonikopoihsh an uparxei periptwsh desmeushs mh desmeymenhs metablhths
void rename_variables(parenthesis* par,char * input,parenthesis* par2)
{
        int i,j,flag;
        char temp[MAX];
        char temp2[MAX];
        for(i=0;i<=par->depth;i++)
        {
                flag=0;
                for (j=0;j<=par2->depth;j++)
                {
                    if (!strcmp((par->variables[i]),(par2->variables[j])))
                    {
                        flag=1;
                        break;
                    }
                }
                if (flag)continue;
                strcpy(temp,par->variables[i]);
                strcpy(temp2,par->variables[i]);
                wrapwithparenthesis(temp2);
                strcat(temp,"(renamed)");
                replacement(input,temp2,temp);
                strcpy(temp,"");
        }
        for (i=0;i<par->kids;i++)
        {
            rename_variables(par->subparenthesis[i],input,par2);
        }
        return;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Basikh sunarthsh kanonikopoihshs
char * normalization_part(char * input)
{
    strcpy(globalstring,input);
    //--------------Initialization  and allocation of the memory---------------//
    int i=0;
    char temporalstring[MAX];
    char * replacestring2;
    char * replacestring=(char*)malloc(MAX);
    if(replacestring== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    char * returnstring=(char*)malloc(MAX);
    if(returnstring== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    parenthesis * smth=(parenthesis*) malloc(sizeof(parenthesis));
    if(smth== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return NULL;
    }
    //--------------Initialization  and allocation of the memory---------------//

    //-------------------Create the tree and simpify it as you can also try etta transformation------------//
    smth=addpar(input,NULL,smth,0,0);
    strcpy(temporalstring,smth->content);
    smth=simplify(smth);
    while(strcmp(smth->content,temporalstring))
    {
        strcpy(temporalstring,smth->content);
        smth=simplify(smth);
    }
    smth=etta_transformation(smth);
    strcpy(temporalstring,smth->content);
    smth=simplify(smth);
    while(strcmp(smth->content,temporalstring))
    {
        strcpy(temporalstring,smth->content);
        smth=simplify(smth);
    }
    //-------------------Create the tree and simpify it as you can also try etta transformation------------//

    //--------------------------If you have more than one kids------------------------------------------------//
    if (smth->kids>1)
    {
            replacestring2=replace_with_trees(smth);
            wrapwithparenthesis(replacestring2);
            strcpy(replacestring,replacestring2);
            if (!strcmp(replacestring2,input))//An den mporei na ginei kapoia allagei se auto to depth phgaine pio bathia
            {
                    strcpy(returnstring,(smth->content));
                    for(i=0;i<smth->kids;i++)   //Gia kathe paidi  kane normalization kai antikatesthse to sto parent
                    {
                        free(replacestring2);
                        strcpy(replacestring,(smth->subparenthesis[i])->content);
                        replacestring2=(char*)normalization_part(replacestring);
                        replacement(returnstring,replacestring,replacestring2);
                    }
            }
            else
            {
                free(replacestring2);
                replacestring2=normalization_part(replacestring);
                strcpy(returnstring,replacestring2);
            }
            free(replacestring2);
            free(replacestring);
            removetree(smth);
            free(smth);
            return returnstring;
    }
    //--------------------------If you have more than one kids------------------------------------------------//

    //-------------------If you have only one kid-------------------------------------------------------//
    else if (smth->kids==1)
    {
            strcpy(replacestring,((smth->subparenthesis)[0])->content);
            replacestring2=(char*)normalization_part(replacestring);
            strcpy(returnstring,(smth->content));
            replacement(returnstring,replacestring,replacestring2);
            free(replacestring2);
            free(replacestring);
            removetree(smth);
            free(smth);
            return returnstring;
    }
    //-------------------If you have only one kid-------------------------------------------------------//

    //------------------------No kids--------------//
    else
    {
            strcpy(returnstring,input);
            free(replacestring);
            removetree(smth);
            free(smth);
            return returnstring;
    }
    //------------------------No kids--------------//
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//Signal handler gia ton termatismo tou programmatos
static void handler(int signum)
{
    printf("\nBye Bye :%s \n",globalstring);
    signal(SIGINT,SIG_DFL);
    free(globalstring);
    kill(getpid(),SIGTERM);
    exit(0);
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
int check_theinput(char* input)
{
    int i=0;
    int parentsis=0;
    while(input[i]!='\0')
    {
        if (input[i]=='(') parentsis++;
        if (input[i]==')') parentsis--;
        i++;
    }
    if (parentsis==0) return 1;
    else return 0;
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
void whitespaceignore(char * input)
{
    int i;
    for(i=0;(input[i]!='\0');i++)
    {
        if (input[i]==' ')
        {
            memmove(&input[i], &input[i+1],strlen(input)-i);
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
int main(void)
{
    //-------------Initiliazation---------//
    char input[MAX];
    char * output;
    globalstring=(char *) malloc(MAX);
    if(globalstring== NULL)
    {
            fprintf(stderr, "out of memory\n");
            return -1;
    }
    //-------------Initiliazation---------//
    printf("Welcome to lamda calculus normalization \n(Hit (Ctrl+C) when you done) \n");
    //-------------signal handler---------//
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    //-------------signal handler---------//
    {
        printf("SIGINT was hitted \n");
    }
    //----------Read the input-----//
    while(1)
    {
        strcpy(globalstring,"nothing to print!");
        printf("\tLamda Calculus:(Normalization)\n ->");
        scanf ("%[^\n]%*c",input);
        whitespaceignore(input);
        if (!check_theinput(input))
        {
            printf("Wrong input:Please check your input \n");
            continue;
        }
        output=normalization_part(input);
        printf("%s \n",output);
        free(output);
    }
    //----------Read the input-----//
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------//
