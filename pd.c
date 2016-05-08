#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

//Global variables
int tableCount = 0;
int first = 0;
int elseCount = 0;
int completeCount = 0;
int againCount = 0;
int finishedCount = 0;
Funs *p;
Classes *c;
int inClass = 0;
char *objName;

//Struct
struct Entry {
    struct Entry *next;
    char *name;
};

//Global struct
struct Entry *table;

static void genClass(Class * c);

//Adds variable to linked list
void set(char *id) {
    int same = 0;
    struct Entry *current = (struct Entry *) malloc(sizeof(struct Entry));
    if (first == 0) {
        table->name = id;
        table->next = NULL;
        first++;
        tableCount++;
    }
    else {
        current = table;
        while (current->next != NULL) {
            if (strcmp(current->name, id) == 0) {
                same = 1;
            }
            current = current->next;
        }
        if (strcmp(current->name, id) == 0) {
            same = 1;
        }
    }
    if (!same) {
        current->next = malloc(sizeof(struct Entry));
        //No memory left
        if (current->next == NULL) {
            printf("Out of memory\n");
        }
        //Assign and print values
        else {
            current = current->next;
            current->name = id;
            current->next = NULL;
            tableCount++;
        }
    }
}

//Renames functions to deal with reserved names in Assembly x86
char* funcRename(char * var) {
    if (strcmp(var, "main") == 0) {
        return var;
    }
    size_t size = strlen(var) + 1;
    char *fullName = malloc(size);
    strcpy(fullName, "_");
    strcat(fullName, var);
    return fullName;
}

//Returns the index of the variable on the stack if it's local, or -1 if it's global.
int formal(Fun * p, char * s) {
    if ((p != NULL)/* || (p != 0)*/) {
        if (p->formals != NULL) {
            Formals *myFormal = p->formals;
            for (int i = 0; i < p->formals -> n; i++) {
                if (strcmp(s, myFormal->first) == 0) {
                    return (p->formals->n) - (myFormal->n) + 1;
                }
                myFormal = myFormal->rest;
            }
        }
    }
    return -1;
}

//Handles expression enums
void myExpression (Expression * e, Fun * p, Class * c) {
    if (e != NULL) {
    switch (e->kind) {
        case eVAR : {
            int inside = formal(p, e->varName);
            if (inside == -1) {		
	    	if(inClass){
	    		int len = 0;
            		while (objName[len++] != 0); 
	    		char *tempStr = malloc(len+1);
	    		for(int i=0; i<len;i++){
				tempStr[i] = objName[i];
		    	}
			strcat(tempStr,"_");	
			strcat(tempStr,e->varName);
			e->varName = tempStr;	
	        }
                set(e->varName);
                printf("    mov %s, %%r15\n", e->varName);
            }
            else {
                printf("    mov %d(%%rbp), %%r15\n", 8 * (inside + 1));
            }
            break;
        }
        case eVAL : {
            printf("    push %%r13\n");
            printf("    mov $");
            printf("%lu", e->val);
            printf(", %%r13\n");
            printf("    mov %%r13, %%r15\n");
            printf("    pop %%r13\n");
            break;
        }
        case ePLUS : {
            printf("    push %%r13\n");
            myExpression(e->left, p, 0);
            printf("    pop %%r13\n");
            printf("    mov %%r15, %%r13\n");
            printf("    push %%r13\n");
            myExpression(e->right, p, 0);
            printf("    pop %%r13\n");
            printf("    add %%r13, %%r15\n");
            break;
        }
        case eMUL : {
            printf("    push %%r13\n");
            myExpression(e->left, p, 0);
            printf("    pop %%r13\n");
            printf("    mov %%r15, %%r13\n");
            printf("    push %%r13\n");
            myExpression(e->right, p, 0);
            printf("    pop %%r13\n");
            printf("    imul %%r13, %%r15\n");
            break;
        }
        case eEQ : {
            myExpression(e->left, p, 0);
            printf("    mov %%r15, %%r13\n");
            myExpression(e->right, p, 0);
            printf("    cmp %%r13, %%r15\n");
            printf("    setz %%r15b\n");
            printf("    movzx %%r15b, %%r15\n");
            break;
        }
        case eNE : {
            myExpression(e->left, p, 0);
            printf("    mov %%r15, %%r13\n");
            myExpression(e->right, p, 0);
            printf("    cmp %%r13, %%r15\n");
            printf("    setnz %%r15b\n");
            printf("    movzx %%r15b, %%r15\n");
            break;
        }
        case eLT : {
            myExpression(e->left, p, 0);
            printf("    mov %%r15, %%r13\n");
            myExpression(e->right, p, 0);
            printf("    cmp %%r15, %%r13\n");
            printf("    setl %%r15b\n");
            printf("    movzx %%r15b, %%r15\n");
            break;
        }
        case eGT : {
            myExpression(e->left, p, 0);
            printf("    mov %%r15, %%r13\n");
            myExpression(e->right, p, 0);
            printf("    cmp %%r15, %%r13\n");
            printf("    setg %%r15b\n");
            printf("    movzx %%r15b, %%r15\n");
            break;
        }
        case eCALL : {
            if (e != NULL) {
            if (e->callActuals != NULL) {
            for (int i = e->callActuals->n - 1; i >= 0; i--) {
                Actuals *actual = e->callActuals;
                for (int a = 0; a < i; a++) {
                    actual = actual->rest;
                }
                myExpression(actual->first, p, 0);
                printf("    push %%r15\n");
            }
            }
            }
            char *newName = funcRename(e->callName);
            printf("    call ");
            printf("%s\n", newName);
            if (e != NULL) {
            if (e->callActuals != NULL) {
            for (int i = 0; i < e->callActuals->n; i++) {
                printf("    pop %%r14\n");
            }
            }
            }
            break;
        }
        default : {
            break;
        }
    }
    }
}
   
//Handles statement enums
void myStatement(Statement * s, Fun * p) {
    if (s != NULL) {
    switch (s->kind) {
        case sAssignment : {
	//name -> obj_name
	    if(inClass){
	    	int len = 0;
            	while (objName[len++] != 0); 
	    	char *tempStr = malloc(len+1);
	    	for(int i=0; i<len;i++){
			tempStr[i] = objName[i];
	    	}
		strcat(tempStr,"_");	
		strcat(tempStr,s->assignName);
		s->assignName = tempStr;	
	    }
            myExpression(s->assignValue, p, 0);
            int inside = formal(p, s->assignName);
            if (inside == -1) {
                set(s->assignName);
                printf("    mov %%r15, ");
                printf("%s\n", s->assignName);
            }
            else {
                printf("    mov %%r15, %d(%%rbp)\n", 8 * (inside + 1));
            }
	    //s->assignName = savedName;
            break;
        }
	case sConstructor : {
	    Classes *temp = c;
	    Class *obj;
	    //char *savedName = objName;
	    objName = s->pointerName;
       	    int len = 0;
            while (s->className[len++] != 0);
	    while(temp!=NULL) {	
	        if (strncmp(temp->first->name, s->className, len) == 0) {
	            obj = temp->first;
 	        }
		temp = temp->rest;
	    }
            if(s != NULL) {	
                if (s->actuals != NULL) {
            	    for (int i = s->actuals->n - 1; i >= 0; i--) {
            	        Actuals *actual = s->actuals;
            		for (int a = 0; a < i; a++) {
       		            actual = actual->rest;
     		    	}
                	myExpression(actual->first, p, obj);
              		printf("    push %%r15\n");
            	    }
                }
		printf("    call %s%d\n", s->className, s->actuals->n);
	    }
            if (s != NULL) {
            	if (s->actuals != NULL) {
                    for (int i = 0; i < s->actuals->n; i++) {
            	        printf("    pop %%r14\n");
            	    }
            	}
            }
	    printf("    call %sEND\n", s->className);
            genClass(obj);
	    printf("%sEND:\n", s->className);
	    break;	
	}
        case sPrint : {
            myExpression(s->printValue, p, 0);
            printf("    mov $p4_format, %%rdi\n");
            printf("    mov %%r15, %%rsi\n");
            printf("    mov $0, %%rax\n");
            printf("    call printf\n");
            break;
        }
        case sIf : {
            myExpression(s->ifCondition, p, 0);
            int elseTemp = elseCount;
            int completeTemp = completeCount;
            elseCount++;
            completeCount++;
            printf("    cmp $0, %%r15\n");
            printf("%s%d\n", "    je else", elseTemp);
            myStatement(s->ifThen, p);
            printf("%s%d\n", "    jmp complete", completeTemp);
            printf("%s%d%s\n", "    else", elseTemp, ":");
            myStatement(s->ifElse, p);
            printf("%s%d%s\n", "    complete", completeTemp, ":");
            break;
        }
        case sWhile : {
            int againTemp = againCount;
            int finishedTemp = finishedCount;
            againCount++;
            finishedCount++;
            printf("%s%d%s\n", "    again", againTemp, ":");
            myExpression(s->whileCondition, p, 0);
            printf("    cmp $0, %%r15\n");
            printf("%s%d\n", "    je finished", finishedTemp);
            myStatement(s->whileBody, p);
            printf("%s%d\n", "    jmp again", againTemp);
            printf("%s%d%s\n", "    finished", finishedTemp, ":");
            break;
        }
        case sBlock : {
            Block *current = s->block;
            while (current != NULL) {
                myStatement(current->first, p);
                current = current->rest;
            }
            break;
        }
        case sReturn : {
            myExpression(s->returnValue, p, 0);
            printf("    mov %%rbp, %%rsp\n");
            printf("    pop %%rbp\n");
            printf("    mov $0,%%rax\n");
            printf("    ret\n");
            break;
        }
        default : {
            break;
        }
    }
    }
}
void genFun(Fun * p) {
    char *rename = funcRename(p->name);
    printf("    .global %s\n", rename);
    printf("%s:\n", rename);
    printf("    push %%rbp\n");
    printf("    mov %%rsp, %%rbp\n");
    myStatement(p->body, p);
    printf("    mov %%rbp, %%rsp\n");
    printf("    pop %%rbp\n");
    printf("    mov $0,%%rax\n");
    printf("    ret\n");
}

void genFuns(Funs * p) {
    if (p == 0)
        return;
    genFun(p->first);
    genFuns(p->rest);
}

/*void myConstructor(Constructor * cn, Class * c) {

}*/

void genInstance(Instance * i, Class * c) {
    myStatement(i->line, 0);
}

void genInstances(Instances * i, Class * c) {
    if (i == 0)
        return;
    genInstance(i->first, c);
    genInstances(i->rest, c);
}

void genConstructor(Constructor * cn, Class * c) {
    printf("    .global %s\n", cn->name);
    printf("%s:\n", cn->name);
    printf("    push %%rbp\n");
    printf("    mov %%rsp, %%rbp\n");
    //Change parameters to myStatement
    //Modify eCALL or add case like eCONST to handle constructor(function) calls

    myStatement(cn->body, 0);
    printf("    mov %%rbp, %%rsp\n");
    printf("    pop %%rbp\n");
    printf("    mov $0,%%rax\n");
    printf("    ret\n");
}

void genConstructors(Constructors * cn, Class * c) {
    if (cn == 0) {
        //printf("#no constructors\n");
        return;
    }
    //printf("#%s%d\n", "number of constructors ", cn->n);
    genConstructor(cn->first, c);
    genConstructors(cn->rest, c);
}

void genFunClass(Fun * p) {
    if(inClass){
                int len = 0;
            	while (objName[len++] != 0); 
	    	char *tempStr = malloc(len+1);
	    	for(int i=0; i<len;i++){
			tempStr[i] = objName[i];
	    	}
		strcat(tempStr,"_");	
		strcat(tempStr,p->name);
		p->name = tempStr;	
    }
    p->name = funcRename(p->name);
    printf("    .global %s\n", p->name);
    printf("%s:\n", p->name);
    printf("    push %%rbp\n");
    printf("    mov %%rsp, %%rbp\n");
    myStatement(p->body, p);
    printf("    mov %%rbp, %%rsp\n");
    printf("    pop %%rbp\n");
    printf("    mov $0,%%rax\n");
    printf("    ret\n");
}

void genFunsClasses(Funs * p) {
    if (p == 0)
        return;
    genFunClass(p->first);
    genFunsClasses(p->rest);
}

void genClass(Class * c) {
    //printf("#%s%d\n", "number of constructors ", c->constructors->n);
    char *savedName = objName;
    inClass = 1;
	printf("#%s\n",objName);
    genInstances(c->instances, c);
	objName = savedName;
	printf("#%s\n",objName);
    genConstructors(c->constructors, c);
    genFunsClasses(c->funs);
    inClass = 0;
}

/*void genClasses(Classes * c) {
    if (c == 0){
        return;
    }
    //printf("#%s%d\n", "number of instances ", c->first->instances->n);
    genClass(c->first);
    genClasses(c->rest);
}*/

int main(int argc, char *argv[]) {
/*    Classes *c =parseC();
    if(c==0)
        printf("#no classes\n");
*/        //Add Classes pointer set to other parse function to receive all classes
    p = parseF();
    c = parseC();
    //c = c;
    //printf("#%s%d\n", "number of classes ", c->n);
    //printf("#%s%s\n", "name of class ", c->first->name);

    /*if(c==0)
        printf("#no classes\n");
    */printf("    .text\n");
    table = (struct Entry *) malloc(sizeof(struct Entry));
    genFuns(p);
    //genClasses(c);
    printf("    .data\n");
    printf("p4_format: .string\"%%d\\n\"\n");
    if (tableCount != 0) {
        while (table->next != NULL) {
            printf("    ");
            printf("%s", table->name);
            printf(": .quad 0\n");
            table = table->next;
        }
        printf("    ");
        printf("%s", table->name);
        printf(": .quad 0\n");
        table = table->next;
    }
    return 0;
}
