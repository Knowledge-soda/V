-- define "ABND" "a, b, n := "
-- define "ABN" "a, b, n"
:ans, v2
info() := id(
    "What do you want to calculate?\n"
    "1 - nth fibonnaci term\n"
    "2 - sum of integers\n"
    "3 - gcd of two numbers\n"
)
invalid(var) := not(and(>(v(ans), 0), <(v(ans), 4)))
repeat() := if("Invalid option!\nTry again!\n" info() load(ans) invalid(ans), repeat)
info()
load(ans)
if(invalid(ans), repeat)

fib'(ABN) := if-else(n, {ABND
        fib'(b, +(a, b), -(n, 1))
    }, {ABND
        +(a, b)
    }, ABN)
fib(n) := if(>(n, 2), fib', 1, 1, -(n, 3), 1)

sum() := if(v(load(ans)), {+(v(ans), sum())}, 0)

gcd(a, b) := if(b, {a, b := gcd(b, %(a, b))}, a, b, a)

if-else(==(v(ans), 1), {
    print("Enter number: " load(ans) fib(v(ans)))
}, {
    if-else(==(v(ans), 2), {
        print("Enter summands.\nUse 0 to terminate.\n" sum())
    }, {
        print("Enter first number: " load(ans)
              "Enter second number: " load(v2)
              gcd(v(ans), v(v2)))
})})
