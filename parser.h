#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>

struct Expression;
typedef struct Expression Expression;

struct Statement;
typedef struct Statement Statement;

typedef struct Actuals {
    int n;
    Expression *first;
    struct Actuals *rest;
} Actuals;

enum EKind {
    eVAR,
    eVAL,
    ePLUS,
    eMUL,
    eEQ,
    eNE,
    eLT,
    eGT,
    eCALL
};

struct Expression {
    enum EKind kind;
    union {
        /* EVAR */ char* varName;
        /* EVAL */ uint64_t val;
        /* EPLUS, EMUL, ... */ struct {
            Expression *left;
            Expression *right;
        };
        /* ECALL */ struct {
            char* callName;
            Actuals* callActuals;
        };
    };
};
    

typedef struct Block {
    int n;
    Statement *first;
    struct Block* rest;
} Block;

typedef struct While {
    Expression *condition;
    Statement *body;
} While;

typedef struct If {
    Expression *condition;
    Statement *thenPart;
    Statement *elsePart;
} If;

typedef struct Print {
    Expression *expression;
} Print;

enum SKind {
    sAssignment,
    sConstructor,
    sPrint,
    sIf,
    sWhile,
    sBlock,
    sReturn,
};

struct Statement {
    enum SKind kind;
    union {
        struct {
            char* assignName;
            Expression *assignValue;
        };
        struct {
            char* className;
            Actuals *actuals;
            char* pointerName;
            //Constructor *constructor;
        };
        Expression *printValue;
        struct {
            Expression* ifCondition;
            Statement* ifThen;
            Statement* ifElse;
        };
        struct {
            Expression *whileCondition;
            Statement* whileBody;
        };
        Block *block;
        Expression* returnValue;
    };
};

typedef struct Statements {
    Statement *first;
    struct Statements *rest;
} Statements;

typedef struct Formals {
    int n;
    char* first;
    struct Formals *rest;
} Formals;

typedef struct Fun {
    char* name;
    Formals *formals;
    Statement *body;
} Fun;

typedef struct Funs {
    int n;
    Fun *first;
    struct Funs *rest;
} Funs;

extern Funs* parseF();

typedef struct Instance {
    char *name;
    Statement *line;
} Instance;

typedef struct Instances {
    int n;
    char *name;
    Instance *first;
    struct Instances *rest;
} Instances;

typedef struct Constructor {
    int params;
    char *name;
    Formals *formals;
    Statement *body;
} Constructor;

typedef struct Constructors {
    int n;
    Constructor *first;
    struct Constructors *rest;
} Constructors;

typedef struct Class {
    char *name;
    Instances *instances;
    Constructors *constructors;
    Funs *funs;
} Class;

typedef struct Classes {
    int n;
    Class *first;
    struct Classes *rest;
} Classes;

extern Classes* parseC();
#endif
