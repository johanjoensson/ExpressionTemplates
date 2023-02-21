Reduce
======

Reduce is a basic operation that takes a function of two arguments (the
accumulator and an element) and an expression and applies the function to each
element in the expression and the accumulator, storing the result in the 
accumulator, then returns the final value of the accumulator. The expression 
has been __reduced__ to a scalar value, by the function. A basic example is 
calculating the sum of all elements in an expression, the function takes an 
element and adds it to the accumulator, then when all elements have been added
to the accumulator, it is returned. In order to reduce an expression, it is
necessary to supply an initial value for the accumulator (by default the code 
will try to use the numeric value 0).
$$
\texttt{reduce} : A \stackrel{f}{\mapsto} s
$$
```c++
auto f = [](auto accumulator, auto elem){...};
auto s = expr::reduce(A, f, accumulator_init);
```
