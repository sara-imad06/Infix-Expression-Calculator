# Data Structures Project 1 — Infix Expression Calculator

## 1. Project Description and Data Structures Used

### Project Description

This project implements a calculator system that reads and evaluates a collection of mathematical expressions written in infix notation from an input file.

The application processes any number of equations and provides the following functionalities:

- Read equations from an input file.
- Check whether each equation is valid or invalid.
- Display the reason for invalid equations.
- Convert valid equations from infix notation to postfix notation.
- Evaluate valid postfix expressions.
- Print all invalid equations.
- Construct an expression tree for a selected valid equation.
- Display inorder, preorder, and postorder traversals.
- Store all results in an output file named `output.txt`.

### Data Structures Used

The project uses the following data structures:

- **Stack:** Used for validating expressions, handling operators and parentheses, converting infix expressions to postfix, and evaluating postfix expressions.
- **Binary Expression Tree:** Used to represent valid expressions and perform inorder, preorder, and postorder traversals.
- **Arrays / Dynamic Storage:** Used to store and manage the equations read from the input file.
- **File Handling:** Used to read equations from the input file and write the results to `output.txt`.

---

## 2. Application Menu

The application provides a menu that allows the user to select the required operation.

```text
========================================
       DATA STRUCTURES PROJECT 1
       INFIX EXPRESSION CALCULATOR
========================================

1. Read Equations
2. Check Validity
3. Convert Infix to Postfix
4. Evaluate Postfix Expressions
5. Print Invalid Equations
6. Expression Tree
7. Export Results to output.txt
8. Exit

Enter your choice:
```

## 3. Converting and Storing Details

### Infix to Postfix Conversion

Valid infix expressions are converted into postfix notation using a stack.

The conversion process handles:

- Operator precedence.
- Parentheses and brackets.
- Operators and operands.
- Correct ordering of operators.
- Nested expressions.

Supported operators:

+  -  *  /

Supported brackets:

( )  [ ]

### Operator Precedence

The operators are processed according to their precedence:

Highest:    *  /
Lowest:     +  -

Parentheses and brackets are used to control the order of operations.

For example:

4 + (7 - 5)

is converted to:

4 7 5 - +

### Expression Evaluation

After converting a valid expression to postfix notation, the postfix expression is evaluated using a stack.

The evaluation process is performed by:

1. Reading the postfix expression from left to right.
2. Pushing operands onto the stack.
3. Popping the required operands when an operator is encountered.
4. Performing the corresponding operation.
5. Pushing the result back onto the stack.
6. Continuing until the entire expression has been processed.
7. Taking the final value from the stack as the result.

Example:

Infix:
4 + (7 - 5)

Postfix:
4 7 5 - +

Result:
6

### Expression Tree

For a selected valid equation, an expression tree is constructed to represent the expression.

Operators are stored in internal nodes, while operands are stored in leaf nodes.

The expression tree supports three traversal methods:

#### Inorder Traversal

Inorder traversal produces the infix form of the expression.

Left -> Root -> Right

#### Postorder Traversal

Postorder traversal produces the postfix form of the expression.

Left -> Right -> Root

#### Preorder Traversal

Preorder traversal produces the prefix form of the expression.

Root -> Left -> Right

### Validation

Before converting or evaluating an equation, the program checks whether the expression is valid.

The program checks for errors such as:

- Unmatched parentheses.
- Unmatched brackets.
- Unclosed parentheses or brackets.
- Missing operators between operands.
- Missing operands around operators.
- Incorrect operator placement.
- Invalid expression structure.

Example:

4+(16-11)6*[8+(9+7)-3]

Output:

Invalid: There is no operator between 11) and 6

Another example:

4+(16-11)+6*[8+(9+7-3]

Output:

Invalid: ( is not closed

### Storing Details

For each equation, the program stores the following information:

- Equation number.
- Original expression.
- Validity status.
- Error message if the expression is invalid.
- Postfix expression if the expression is valid.
- Evaluation result if the expression is valid.

The stored information allows the different menu options to access and process the equations without reading the input file again.

### Output File

The final results are exported to:

output.txt

The output file contains the processing information for every equation.

For a valid equation:

Equation No. 1 -> Valid
Postfix: 4 16 11 - + 6 8 9 7 + + 3 - * +
Result: 135

For an invalid equation:

Equation No. 2 -> Invalid
Reason: There is no operator between 11) and 6

### Example Input

4+(16-11)+6*[8+(9+7)-3]
4+(16-11)6*[8+(9+7)-3]
4+(16-11)+6*[8+(9+7-3]
4+(7-5)

### Example Processing

Equation No. 1 -> Valid
Postfix: 4 16 11 - + 6 8 9 7 + + 3 - * +
Result: 135

Equation No. 2 -> Invalid
Reason: There is no operator between 11) and 6

Equation No. 3 -> Invalid
Reason: ( is not closed

Equation No. 4 -> Valid
Postfix: 4 7 5 - +
Result: 6
