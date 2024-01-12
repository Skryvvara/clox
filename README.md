# CLOX

This is my version of the Lox implementation in C following Robert Nystrom's
[Crafting Interpreters](https://craftinginterpreters.com/)

I mostly followed the book here with few deviations to learn about the C programming language and
how to build compilers/interpreters. Some deviations include the OP_POPN instruction
to pop multiple values off the stack instead of issuing a OP_POP for each one or
a lua like concatination operator for strings ("str".."ing").

## Example
```
class example {
  init(name) {
    this.name = name;
  }

  say() {
    print "Hello, my name is " .. this.name;
  }
}

class suber < example {
  init(name) {
    super.init(name);
  }

  tell() {
    super.say();
  }
}

var inst = suber("test");
inst.tell();
```
> Hello, my name is test

## Stdlib

The term might be a bit far fetched in this case but I have added some builtin functions to my version of clox

### clock
```
> print clock();
0.005036
```

### is_string
```
> var a = "name";
> print is_string(a);
true
```

### strlen
```
> print strlen(a);
4
```

### has_own
```
> inst.field = "test";
> print has_own(inst, "field");
true
```

### is_even
```
> print is_even(254);
true
```
