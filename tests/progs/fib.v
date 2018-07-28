fib(x) := ?(<(x, 2), x, ijmp(<(x, 2), fake) +(fib(-(x, 1)), fib(-(x, 2))) fake: _)
print(fib(20))
