Map
===

Map is a basic operation that takes a function of one argument (a unary
function or operator) and applies it to each element in an expression,
resulting in a new expression. The function has been __mapped__ over the
expression. A basic example is multiplying an expression by a scalar, which
results in a new expression whose elements are the elements of the original
expression multiplied by the scalar.

.. math::

   \texttt{map} : A \stackrel{f}{\mapsto} B

   B = \texttt{map}(A, f) \Leftrightarrow B_{i, j, \ldots} = f(A_{i, j, \ldots}) \forall i, j, \ldots

.. code-block:: c++ 

   auto f = [](auto elem){...};
   auto B = expr::map(A, f);
