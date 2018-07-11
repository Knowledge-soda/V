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

fib'(a, b, n) := ifec(n, {a, b, n := 
        fib'(b, +(a, b), -(n, 1))
    }, {a, b, n := 
        +(a, b)
    }, a, b, n)
fib(n) := ifc(>(n, 2), fib', 1, 1, -(n, 3), 1)

sum() := ifc(v(load(ans)), {+(v(ans), sum())}, 0)

gcd(a, b) := ifc(b, {a, b := gcd(b, %(a, b))}, a, b, a)

ifec(==(v(ans), 1), {
    print("Enter number: " load(ans) fib(v(ans)))
}, {
    ifec(==(v(ans), 2), {
        print("Enter summands.\nUse 0 to terminate.\n" sum())
    }, {
        print("Enter first number: " load(ans)
              "Enter second number: " load(v2)
              gcd(v(ans), v(v2)))
})})
