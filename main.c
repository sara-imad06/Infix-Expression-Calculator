/*
 * Sara Imad Abdou - ID #1241167 - Section #4
 *
 * This is a C program to represent a calculator system
 * using stacks, linked lists, and expression trees.
 *
 * The program reads equations from a file, checks if they
 * are valid or invalid, converts valid infix equations to
 * postfix form, evaluates them, prints invalid equations,
 * builds expression trees, and saves the final report to
 * output.txt.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE    1000
#define MAX_POSTFIX 5000
#define MAX_TOKEN   100


// Stack to store brackets so we can check them
typedef struct charNode
{
    char data;
    struct charNode *next;
} CharNode;

// Stack stores numbers for (postfix evaluation)
typedef struct intNode
{
    long long data;
    struct intNode *next;
} IntNode;

//expression tree
// will look sth like this 4+(7-5)
/*    +
     / \
    4   -
       / \
      7   5

*/

typedef struct treeNode
{
    char *data;
    struct treeNode *left;
    struct treeNode *right;
} TreeNode;


//Stack stores tree nodes ,when it reads an operation -> create a subtree contains the numbers
typedef struct treeStackNode
{
    TreeNode *data;
    struct treeStackNode *next;
} TreeStackNode;


// structure stores the information of one equation
typedef struct equationInfo
{
    int number;
    char *infix;
    char *postfix;
    int valid;
    long long result;
    char *error;
} EquationInfo;

// linked list node that stores equations
// header -> equation 1 -> equation 2 -> equation 3 -> null
typedef struct equationNode
{
    EquationInfo data;
    struct equationNode *next;
} EquationNode;

EquationNode *equations = NULL;
int equationCount = 0;
int file_flag = 0;     // 1 after file is loaded



char *stringCopy(const char *str);
void  setString(char **old, const char *value);

int isDigit(char n);
int isSpace(char n);
int isOperator(char n);
int isOpening(char n);
int isClosing(char n);
int matched(char open, char close);
int precedence(char op);
int isEmptyLine(const char str[]);
int isUnarySign(const char expr[], int index, int prevType);
int isNumberToken(const char token[]);

void printMenu();
int  requireFileLoaded();
void removeNewLine(char str[]);

EquationNode *createEquationList();
void makeEmptyEquationList(EquationNode *L);
void insertEquationAtEnd(EquationNode *L, const char *infix);
EquationInfo *findEquationByNumber(EquationNode *L, int number);
void freeEquationInfo(EquationInfo *info);

CharNode *createCharStack();
int isEmptyCharStack(CharNode *S);
void makeEmptyCharStack(CharNode *S);
void pushChar(char x, CharNode *S);
void popChar(CharNode *S);
char topChar(CharNode *S);
char topAndPopChar(CharNode *S);
void disposeCharStack(CharNode *S);

IntNode *createIntStack();
int isEmptyIntStack(IntNode *S);
void makeEmptyIntStack(IntNode *S);
void pushInt(long long x, IntNode *S);
void popInt(IntNode *S);
long long topInt(IntNode *S);
long long topAndPopInt(IntNode *S, int *flag);
void disposeIntStack(IntNode *S);

TreeStackNode *createTreeStack();
int isEmptyTreeStack(TreeStackNode *S);
void makeEmptyTreeStack(TreeStackNode *S);
void pushTree(TreeNode *x, TreeStackNode *S);
void popTree(TreeStackNode *S);
TreeNode *topTree(TreeStackNode *S);
TreeNode *topAndPopTree(TreeStackNode *S);
void disposeTreeStack(TreeStackNode *S);

TreeNode *createTreeNode(const char element[]);
TreeNode *makeEmptyTree(TreeNode *T);
TreeNode *buildExpressionTreeFromPostfix(const char postfix[]);
void printIn(TreeNode *T);
void printPost(TreeNode *T);
void printPre(TreeNode *T);

int checkValidity(const char expr[], char **error);
void addTokenToPostfix(char postfix[], const char token[]);
char *infixToPostfix(const char infix[]);
long long evaluatePostfix(const char postfix[], int *flag);
void processAllEquations();
void readInputFile();
void printEquationsByOption(int option);
void expressionTreeOption();
void printAllToOutputFile();



//this function makes a separate copy of a string so we can store it safely in the list
char *stringCopy(const char *str)
{
    if (str == NULL) str = "";
    int   len  = (int)strlen(str);
    char *copy = (char *)malloc((len + 1) * sizeof(char));
    if (copy == NULL)
        printf("Out of memory!\n");
    else
        strcpy(copy, str);
    return copy;
}

void setString(char **old, const char *value)
{
    if (*old != NULL) free(*old);
    *old = stringCopy(value);
}

//checks if one character is a number from 0 to 9
int isDigit(char n)
{
    return n >= '0' && n <= '9';
}

//checks if one character is a space or new line so we can ignore it
int isSpace(char n)
{
    return n == ' ' || n == '\t' || n == '\n' || n == '\r';
}
int isOperator(char n)
{
    return n == '+' || n == '-' || n == '*' || n == '/' || n == '%';
}
int isOpening(char n)
{
    return n == '(' || n == '[' || n == '{';
}
int isClosing(char n)
{
    return n == ')' || n == ']' || n == '}';
}

int matched(char open, char close)
{
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

//gives each operator its priority so multiplication is done before addition
int precedence(char op)
{
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '+' || op == '-')              return 1;
    return 0;
}

//checks if the whole line is empty or only contains spaces
int isEmptyLine(const char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
        if (!isSpace(str[i])) return 0;
    return 1;
}

int isUnarySign(const char expr[], int index, int prevType)
{
    if (expr[index] != '+' && expr[index] != '-') return 0;
    if (prevType != 0) return 0;
    int next = index + 1;
    while (isSpace(expr[next])) next++;
    return isDigit(expr[next]);
}

int isNumberToken(const char token[])
{
    if (token[0] == '\0') return 0;
    int i = 0;
    if (token[0] == '-' || token[0] == '+')
    {
        if (token[1] == '\0') return 0;
        i = 1;
    }
    for (; token[i] != '\0'; i++)
        if (!isDigit(token[i])) return 0;
    return 1;
}

void printMenu()
{
    printf("\n ******* Calculator System ******* \n");
    printf("1. Read equations from the input file\n");
    printf("2. Check validity of equations\n");
    printf("3. Convert valid equations from infix to postfix and print the postfix expressions\n");
    printf("4. Evaluate the postfix expressions and print the results\n");
    printf("5. Print invalid equations from the input file\n");
    printf("6. Expression tree traversals\n");
    printf("7. Print all equations to output file\n");
    printf("8. Exit\n");
    printf("Choose an option: ");
}

int requireFileLoaded()
{
    if (!file_flag)
    {
        printf("Please read the input file first using option 1.\n");
        return 0;
    }
    return 1;
}

//this function removes \n and \r from the end of a line after reading it from the file
void removeNewLine(char str[])
{
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}



EquationNode *createEquationList()
{
    EquationNode *L = (EquationNode *)malloc(sizeof(EquationNode));
    if (L == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }
    L->next           = NULL;
    L->data.number    = 0;
    L->data.infix     = NULL;
    L->data.postfix   = NULL;
    L->data.error     = NULL;
    L->data.valid     = 0;
    L->data.result    = 0;
    return L;
}

void makeEmptyEquationList(EquationNode *L)
{
    EquationNode *p = L->next;
    L->next = NULL;
    while (p != NULL)
    {
        EquationNode *temp = p->next;
        freeEquationInfo(&p->data);
        free(p);
        p = temp;
    }
    equationCount = 0;
}

void insertEquationAtEnd(EquationNode *L, const char *infix)
{
    EquationNode *newNode = (EquationNode *)malloc(sizeof(EquationNode));
    if (newNode == NULL)
    {
        printf("Out of memory!\n");
        return;
    }
    equationCount++;
    newNode->data.number  = equationCount;
    newNode->data.infix   = stringCopy(infix);
    newNode->data.postfix = stringCopy("");
    newNode->data.error   = stringCopy("");
    newNode->data.valid   = 0;
    newNode->data.result  = 0;
    newNode->next         = NULL;

    EquationNode *p = L;
    while (p->next != NULL) p = p->next;
    p->next = newNode;
}

// this function searches the linked list to find the equation chosen by the user
EquationInfo *findEquationByNumber(EquationNode *L, int number)
{
    EquationNode *p = L->next;
    while (p != NULL)
    {
        if (p->data.number == number) return &p->data;
        p = p->next;
    }
    return NULL;
}

void freeEquationInfo(EquationInfo *info)
{
    if (info->infix   != NULL)
    {
        free(info->infix);
        info->infix   = NULL;
    }
    if (info->postfix != NULL)
    {
        free(info->postfix);
        info->postfix = NULL;
    }
    if (info->error   != NULL)
    {
        free(info->error);
        info->error   = NULL;
    }
}



CharNode *createCharNode(char data)
{
    CharNode *node = (CharNode *)malloc(sizeof(CharNode));
    if (node == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

CharNode *createCharStack()
{
    CharNode *S = (CharNode *)malloc(sizeof(CharNode));
    if (S == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }
    S->next = NULL;
    return S;
}

int isEmptyCharStack(CharNode *S)
{
    return S->next == NULL;
}

void makeEmptyCharStack(CharNode *S)
{
    while (!isEmptyCharStack(S)) popChar(S);
}

void pushChar(char x, CharNode *S)
{
    CharNode *temp = createCharNode(x);
    if (temp != NULL)
    {
        temp->next = S->next;
        S->next    = temp;
    }
}

void popChar(CharNode *S)
{
    if (isEmptyCharStack(S))
    {
        printf("Empty stack!\n");
        return;
    }
    CharNode *first = S->next;
    S->next = first->next;
    free(first);
}

char topChar(CharNode *S)
{
    return isEmptyCharStack(S) ? '\0' : S->next->data;
}

char topAndPopChar(CharNode *S)
{
    char top = topChar(S);
    if (!isEmptyCharStack(S)) popChar(S);
    return top;
}

void disposeCharStack(CharNode *S)
{
    if (S != NULL)
    {
        makeEmptyCharStack(S);
        free(S);
    }
}



IntNode *createIntNode(long long data)
{
    IntNode *node = (IntNode *)malloc(sizeof(IntNode));
    if (node == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

IntNode *createIntStack()
{
    IntNode *S = (IntNode *)malloc(sizeof(IntNode));
    if (S == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }
    S->next = NULL;
    return S;
}

int isEmptyIntStack(IntNode *S)
{
    return S->next == NULL;
}

void makeEmptyIntStack(IntNode *S)
{
    while (!isEmptyIntStack(S)) popInt(S);
}

void pushInt(long long x, IntNode *S)
{
    IntNode *temp = createIntNode(x);
    if (temp != NULL)
    {
        temp->next = S->next;
        S->next    = temp;
    }
}

void popInt(IntNode *S)
{
    if (isEmptyIntStack(S))
    {
        printf("Empty stack!\n");
        return;
    }
    IntNode *first = S->next;
    S->next = first->next;
    free(first);
}

long long topInt(IntNode *S)
{
    return isEmptyIntStack(S) ? 0 : S->next->data;
}

long long topAndPopInt(IntNode *S, int *flag)
{
    if (isEmptyIntStack(S))
    {
        *flag = 0;
        return 0;
    }
    long long top = topInt(S);
    popInt(S);
    return top;
}

void disposeIntStack(IntNode *S)
{
    if (S != NULL)
    {
        makeEmptyIntStack(S);
        free(S);
    }
}


TreeStackNode *createTreeStackNode(TreeNode *data)
{
    TreeStackNode *node = (TreeStackNode *)malloc(sizeof(TreeStackNode));
    if (node == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

TreeStackNode *createTreeStack()
{
    TreeStackNode *S = (TreeStackNode *)malloc(sizeof(TreeStackNode));
    if (S == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }
    S->next = NULL;
    return S;
}

int isEmptyTreeStack(TreeStackNode *S)
{
    return S->next == NULL;
}

void makeEmptyTreeStack(TreeStackNode *S)
{
    while (!isEmptyTreeStack(S)) popTree(S);
}

void pushTree(TreeNode *x, TreeStackNode *S)
{
    TreeStackNode *temp = createTreeStackNode(x);
    if (temp != NULL)
    {
        temp->next = S->next;
        S->next    = temp;
    }
}

void popTree(TreeStackNode *S)
{
    if (isEmptyTreeStack(S))
    {
        printf("Empty stack!\n");
        return;
    }
    TreeStackNode *first = S->next;
    S->next = first->next;
    free(first);
}

TreeNode *topTree(TreeStackNode *S)
{
    return isEmptyTreeStack(S) ? NULL : S->next->data;
}

TreeNode *topAndPopTree(TreeStackNode *S)
{
    TreeNode *top = topTree(S);
    if (!isEmptyTreeStack(S)) popTree(S);
    return top;
}

void disposeTreeStack(TreeStackNode *S)
{
    if (S != NULL)
    {
        makeEmptyTreeStack(S);
        free(S);
    }
}



TreeNode *createTreeNode(const char element[])
{
    TreeNode *T = (TreeNode *)malloc(sizeof(TreeNode));
    if (T == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }
    T->data  = stringCopy(element);
    T->left  = NULL;
    T->right = NULL;
    return T;
}

TreeNode *makeEmptyTree(TreeNode *T)
{
    if (T != NULL)
    {
        makeEmptyTree(T->left);
        makeEmptyTree(T->right);
        free(T->data);
        free(T);
    }
    return NULL;
}
//this function reads the postfix expression and builds the tree by connecting operators with operands
TreeNode *buildExpressionTreeFromPostfix(const char postfix[])
{
    char *copy = stringCopy(postfix);
    TreeStackNode *S = createTreeStack();

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL)
    {
        if (isNumberToken(token))
        {
            pushTree(createTreeNode(token), S);
        }
        else if (strlen(token) == 1 && isOperator(token[0]))
        {
            TreeNode *right = topAndPopTree(S);
            TreeNode *left  = topAndPopTree(S);
            TreeNode *node  = createTreeNode(token);
            node->left  = left;
            node->right = right;
            pushTree(node, S);
        }
        token = strtok(NULL, " \t\n\r");
    }

    TreeNode *result = topAndPopTree(S);
    disposeTreeStack(S);
    free(copy);
    return result;
}

// this function prints the expression tree in infix form
void printIn(TreeNode *T)
{
    if (T != NULL)
    {
        if (T->left != NULL || T->right != NULL) printf("( ");
        printIn(T->left);
        printf("%s ", T->data);
        printIn(T->right);
        if (T->left != NULL || T->right != NULL) printf(") ");
    }
}
//this function prints the expression tree in postfix form
void printPost(TreeNode *T)
{
    if (T != NULL)
    {
        printPost(T->left);
        printPost(T->right);
        printf("%s ", T->data);
    }
}

//his function prints the expression tree in prefix form
void printPre(TreeNode *T)
{
    if (T != NULL)
    {
        printf("%s ", T->data);
        printPre(T->left);
        printPre(T->right);
    }
}

//his function scans the infix equation and checks numbers, operators, and brackets order
int checkValidity(const char expr[], char **error)
{
    CharNode *brackets = createCharStack();
    int type = 0; //type = 0 means operand and 1 means operator.

    if (expr == NULL || isEmptyLine(expr))
    {
        setString(error, "empty equation");
        disposeCharStack(brackets);
        return 0;
    }

    for (int i = 0; expr[i] != '\0'; i++)
    {
        if (isSpace(expr[i])) continue;

        if (isDigit(expr[i]) || isUnarySign(expr, i, type))
        {
            if (type == 1)
            {
                int previous = i - 1;
                while (previous >= 0 && isSpace(expr[previous])) previous--;

                if (previous >= 0 && isClosing(expr[previous]))
                    setString(error, "there is no operator between closing bracket and operand");
                else
                    setString(error, "there is no operator between operands");

                disposeCharStack(brackets);
                return 0;
            }
            if (expr[i] == '+' || expr[i] == '-')
            {
                i++;
                while (isSpace(expr[i])) i++;
            }
            while (isDigit(expr[i])) i++;
            i--;
            type = 1;

        }
        else if (isOpening(expr[i]))
        {
            if (type == 1)
            {
                setString(error, "there is no operator before opening bracket");
                disposeCharStack(brackets);
                return 0;
            }
            pushChar(expr[i], brackets);
            type = 0;

        }
        else if (isClosing(expr[i]))
        {
            if (type == 0)
            {
                setString(error, "there is no operand before closing bracket");
                disposeCharStack(brackets);
                return 0;
            }
            if (isEmptyCharStack(brackets))
            {
                setString(error, "closing bracket is not opened");
                disposeCharStack(brackets);
                return 0;
            }
            char open = topAndPopChar(brackets);
            if (!matched(open, expr[i]))
            {
                setString(error, "brackets are not matched correctly");
                disposeCharStack(brackets);
                return 0;
            }
            type = 1;

        }
        else if (isOperator(expr[i]))
        {
            if (type == 0)
            {
                int previous = i - 1;
                while (previous >= 0 && isSpace(expr[previous])) previous--;

                if (previous >= 0 && isOpening(expr[previous]))
                    setString(error, "there is no operand after opening bracket");
                else
                    setString(error, "there is no operand before operator");

                disposeCharStack(brackets);
                return 0;
            }
            type = 0;

        }
        else
        {
            setString(error, "invalid character");
            disposeCharStack(brackets);
            return 0;
        }
    }

    if (type == 0)
    {
        setString(error, "equation ends with an operator or has empty brackets");
        disposeCharStack(brackets);
        return 0;
    }
    if (!isEmptyCharStack(brackets))
    {
        char open = topChar(brackets);

        if (open == '(')
            setString(error, "( is not closed");
        else if (open == '[')
            setString(error, "[ is not closed");
        else if (open == '{')
            setString(error, "{ is not closed");
        else
            setString(error, "opening bracket is not closed");

        disposeCharStack(brackets);
        return 0;
    }

    setString(error, "valid");
    disposeCharStack(brackets);
    return 1;
}

void addTokenToPostfix(char postfix[], const char token[])
{
    if (postfix[0] != '\0') strcat(postfix, " ");
    strcat(postfix, token);
}

//this function uses a stack to change a valid infix equation into postfix form
char *infixToPostfix(const char infix[])
{
    CharNode *operators = createCharStack();
    char postfix[MAX_POSTFIX];
    char token[MAX_TOKEN];
    int type = 0;   //type = 0 means expecting an operand and 1 means expecting an operator.
    postfix[0] = '\0';

    for (int i = 0; infix[i] != '\0'; i++)
    {
        if (isSpace(infix[i])) continue;

        if (isDigit(infix[i]) || isUnarySign(infix, i, type))
        {
            int k = 0;
            if (infix[i] == '+' || infix[i] == '-')
            {
                token[k++] = infix[i++];
                while (isSpace(infix[i])) i++;
            }
            while (isDigit(infix[i])) token[k++] = infix[i++];
            token[k] = '\0';
            i--;
            addTokenToPostfix(postfix, token);
            type = 1;

        }
        else if (isOpening(infix[i]))
        {
            pushChar(infix[i], operators);
            type = 0;

        }
        else if (isClosing(infix[i]))
        {
            while (!isEmptyCharStack(operators) && !isOpening(topChar(operators)))
            {
                char op = topAndPopChar(operators);
                token[0] = op;
                token[1] = '\0';
                addTokenToPostfix(postfix, token);
            }
            if (!isEmptyCharStack(operators)) popChar(operators);
            type = 1;

        }
        else if (isOperator(infix[i]))
        {
            while (!isEmptyCharStack(operators) &&
                    isOperator(topChar(operators)) &&
                    precedence(topChar(operators)) >= precedence(infix[i]))
            {
                char op = topAndPopChar(operators);
                token[0] = op;
                token[1] = '\0';
                addTokenToPostfix(postfix, token);
            }
            pushChar(infix[i], operators);
            type = 0;
        }
    }

    while (!isEmptyCharStack(operators))
    {
        char op = topAndPopChar(operators);
        token[0] = op;
        token[1] = '\0';
        addTokenToPostfix(postfix, token);
    }

    disposeCharStack(operators);
    return stringCopy(postfix);
}

//this function evaluates postfix by pushing numbers and applying operators on the last two numbers
long long evaluatePostfix(const char postfix[], int *flag)
{
    IntNode  *S    = createIntStack();
    char     *copy = stringCopy(postfix);
    long long result = 0;
    *flag = 1;

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL)
    {
        if (isNumberToken(token))
        {
            pushInt(atoll(token), S);
        }
        else
        {
            long long b = topAndPopInt(S, flag);
            long long a = topAndPopInt(S, flag);
            if (*flag == 0) break;

            switch (token[0])
            {
            case '+':
                result = a + b;
                break;
            case '-':
                result = a - b;
                break;
            case '*':
                result = a * b;
                break;
            case '/':
                if (b == 0)
                {
                    *flag = 0;
                }
                else result = a / b;
                break;
            case '%':
                if (b == 0)
                {
                    *flag = 0;
                }
                else result = a % b;
                break;
            default:
                *flag = 0;
            }
            if (*flag == 0) break;
            pushInt(result, S);
        }
        token = strtok(NULL, " \t\n\r");
    }

    if (*flag == 1)
    {
        result = topAndPopInt(S, flag);
        if (!isEmptyIntStack(S)) *flag = 0;
    }

    disposeIntStack(S);
    free(copy);
    return result;
}

// this function goes through all equations and stores their validity, postfix form, and result
void processAllEquations()
{
    EquationNode *p = equations->next;
    while (p != NULL)
    {
        p->data.valid = checkValidity(p->data.infix, &p->data.error);

        if (p->data.valid)
        {
            char *newPostfix = infixToPostfix(p->data.infix);
            setString(&p->data.postfix, newPostfix);
            free(newPostfix);

            int flag;
            p->data.result = evaluatePostfix(p->data.postfix, &flag);

            if (!flag)
            {
                p->data.valid = 0;
                setString(&p->data.error,   "cannot evaluate equation");
                setString(&p->data.postfix, "");
                p->data.result = 0;
            }
        }
        else
        {
            setString(&p->data.postfix, "");
            p->data.result = 0;
        }

        p = p->next;
    }
}

//asks for the file name, reads each equation line, stores it, then processes all equations
void readInputFile()
{
    char fileName[256];
    printf("Enter input file name: ");
    scanf("%255s", fileName);  //255 to prevent overflow

    FILE *in = fopen(fileName, "r");
    if (in == NULL)
    {
        printf("Could not open file %s\n", fileName);
        return;
    }

    makeEmptyEquationList(equations);

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, in) != NULL)
    {
        removeNewLine(line);
        if (!isEmptyLine(line))
            insertEquationAtEnd(equations, line);
    }

    fclose(in);
    processAllEquations();
    file_flag = 1;
    printf("%d equations loaded and processed successfully.\n", equationCount);
}

//prints valid, invalid, postfix, or result output based on the menu option
void printEquationsByOption(int option)
{
    if (!requireFileLoaded()) return;

    int found = 0;
    EquationNode *p = equations->next;

    while (p != NULL)
    {
        if (option == 2)
        {
            if (p->data.valid)
                printf("Equation No. %d -> valid\n", p->data.number);
            else
                printf("Equation No. %d -> invalid: %s\n", p->data.number, p->data.error);

        }
        else if (option == 3 && p->data.valid)
        {
            printf("Equation No. %d postfix expression: %s\n", p->data.number, p->data.postfix);
            found = 1;

        }
        else if (option == 4 && p->data.valid)
        {
            printf("Equation No. %d result: %lld\n", p->data.number, p->data.result);
            found = 1;

        }
        else if (option == 5 && !p->data.valid)
        {
            printf("Equation No. %d: %s\n", p->data.number, p->data.infix);
            printf("Reason: %s\n", p->data.error);
            found = 1;
        }

        p = p->next;
    }

    if (option == 3 && !found) printf("There are no valid equations to convert to postfix.\n");
    if (option == 4 && !found) printf("There are no valid postfix expressions to evaluate.\n");
    if (option == 5 && !found) printf("There are no invalid equations in the input file.\n");
}

//asks for an equation number and prints its tree traversals if it is valid
void expressionTreeOption()
{
    if (!requireFileLoaded()) return;

    int number;
    printf("Enter equation number: ");
    scanf("%d", &number);

    EquationInfo *eq = findEquationByNumber(equations, number);
    if (eq == NULL)
    {
        printf("Invalid equation number.\n");
    }
    else if (!eq->valid)
    {
        printf("Equation No. %d is invalid: %s\n", number, eq->error);
    }
    else
    {
        TreeNode *T = buildExpressionTreeFromPostfix(eq->postfix);
        printf("Inorder   : ");
        printIn(T);
        printf("\n");
        printf("Postorder : ");
        printPost(T);
        printf("\n");
        printf("Preorder  : ");
        printPre(T);
        printf("\n");
        makeEmptyTree(T);
    }
}

void printAllToOutputFile()
{
    if (!requireFileLoaded()) return;

    FILE *out = fopen("output.txt", "w");
    if (out == NULL)
    {
        printf("Could not open output.txt\n");
        return;
    }

    EquationNode *p = equations->next;
    while (p != NULL)
    {
        fprintf(out, "Equation No. %d: %s\n", p->data.number, p->data.infix);
        if (p->data.valid)
        {
            fprintf(out, "Status : valid\n");
            fprintf(out, "Postfix: %s\n", p->data.postfix);
            fprintf(out, "Result : %lld\n", p->data.result);
        }
        else
        {
            fprintf(out, "Status : invalid\n");
            fprintf(out, "Reason : %s\n", p->data.error);
        }
        fprintf(out, "***************\n");
        p = p->next;
    }

    fclose(out);
    printf("All equations were printed to output.txt\n");
}



int main()
{
    equations = createEquationList();

    if (equations == NULL)
    {
        printf("Cannot create equation list.\n");
        return 1;
    }

    int choice;

    do
    {
        printMenu();

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input.\n");
            break;
        }

        switch (choice)
        {
        case 1:
            readInputFile();
            break;
        case 2:
            printEquationsByOption(2);
            break;
        case 3:
            printEquationsByOption(3);
            break;
        case 4:
            printEquationsByOption(4);
            break;
        case 5:
            printEquationsByOption(5);
            break;
        case 6:
            expressionTreeOption();
            break;
        case 7:
            printAllToOutputFile();
            break;
        case 8:
            printf("Exiting....\n");
            break;
        default:
            printf("Invalid choice.\n");
        }

    }
    while (choice != 8);

    makeEmptyEquationList(equations);
    free(equations);
    return 0;
}
