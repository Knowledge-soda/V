:ans, v2
info() := id(
    "What do you want to calculate?\n"
    "1 - nth fibonnaci term\n"
    "2 - sum of integers\n"
    "3 - gcd of two numbers\n"
)
invalid(var) := not(and(>(v(ans), 0), <(v(ans), 4)))
repeat() := ifc("Invalid option!\nTry again!\n" info() load(ans) invalid(ans), repeat)
info()
load(ans)
ifc(invalid(ans), repeat)

fib'(a, b, n) := ifc(n, fib', b, +(a, b), -(n, 1), +(a, b))
fib(n) := ifc(>(n, 2), fib', 1, 1, -(n, 3), 1)
fib prog() := print("Enter number: " load(ans) fib(v(ans)))

sum() := +(v(load(ans)), ifc(v(ans), sum, 0))
sum prog() := print("Enter summands.\nUse 0 to terminate.\n" sum())

gcd(a, b) := ifc(b, gcd, b, %(a, if(b, b, 1)), a)
gcd prog() := print("Enter first number: " load(ans)
                    "Enter second number: " load(v2)
                    gcd(v(ans), v(v2)))

ifc(==(v(ans), 1), fib prog)
ifc(==(v(ans), 2), sum prog)
ifc(==(v(ans), 3), gcd prog)
