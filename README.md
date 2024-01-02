# CLOX

This is my version of the Lox implementation in C following Robert Nystrom's
[Crafting Interpreters](https://craftinginterpreters.com/)

I mostly followed the book here with few deviations to learn about the C programming language and
how to build compilers/interpreters. Some deviations include the OP_POPN instruction
to pop multiple values off the stack instead of issuing a OP_POP for each one or
a lua like concatination operator for strings ("str".."ing").
