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
