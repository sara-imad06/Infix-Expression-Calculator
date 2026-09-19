#  Data Structures — Project 1
## Infix Expression Calculator

A Data Structures project that implements a calculator capable of
reading multiple mathematical expressions from an input file,
validating them, converting valid expressions from infix to postfix,
evaluating the results, and constructing expression trees.

---

##  Project Overview

The application reads a collection of mathematical equations
written in infix notation from an input file.

For each equation, the system can:

- Validate the expression
- Convert valid expressions from infix to postfix
- Evaluate postfix expressions
- Display invalid equations
- Construct an expression tree
- Perform tree traversals
- Export the results to `output.txt`

---

##  Features

### 1.  Read Input File
The user provides the name of the input file containing
the equations.

### 2.  Validate Equations
Each equation is checked to determine whether it is valid.

Example:

Equation No. 1 → Valid

Equation No. 2 → Invalid:
there is no operator between `11)` and `6`

Equation No. 3 → Invalid:
`(` is not closed

Equation No. 4 → Valid

### 3.  Infix → Postfix
Valid expressions are converted from infix notation
to postfix notation.

### 4.  Evaluate Expressions
The postfix expressions are evaluated and the result
of each valid equation is displayed.

### 5.  Display Invalid Equations
All invalid equations from the input file can be displayed.

### 6.  Expression Tree
For a selected valid equation, an expression tree is constructed.

The tree can be displayed using:

- Inorder → Infix
- Postorder → Postfix
- Preorder → Prefix

### 7.  Export Results
All equations and their results are written to:

`output.txt`

### 8.  Exit
Exit the application.

---

##  Data Structures Used

- Stack
- Binary Expression Tree
- Linked List / Dynamic Data Structures
- File Handling

---

##  Example Input

```text
4+(16-11)+6*[8+(9+7)-3]
4+(16-11)6*[8+(9+7)-3]
4+(16-11)+6*[8+(9+7-3]
4+(7-5)
