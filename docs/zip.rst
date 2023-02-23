Zip
===

Zip is a basic operation that takes a function of two argument (a binary
function or operator) and applies it to each pair of elements in two 
expression, resulting in a new expression. The two expressions have been
__zipped__ together using the function. A basic example is elementwise addition
of two expressions, which results in a new expression whose elements are the 
elements of the original two expressions, added together.

.. math::

   \texttt{zip} : A, B \stackrel{f}{\mapsto} C

   C = \texttt{zip}(A, B, f) \Leftrightarrow C_{i, j, \ldots} = f(A_{i, j, \ldots}, B_{i, j, \ldots}) \forall i, j, \ldots

.. code-block:: c++

   auto f = [](auto elem_A, auto elem_B){...};
   auto C = expr::zip(A, B, f);
