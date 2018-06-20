fib(x) := +(ifc(>(x, 2), fib, -(x, 2), 1), ifc(>(x, 2), fib, -(x, 1), 0))
print(fib(40))
