Tutorial
========

Running first program
---------------------

Syntax of V programming language is quite simple, almost all things are done with functions.

Before we get our hands dirty, I'll explain comments. They are same as in C (`/*` and `*/`, `//` isn't supported) except that they support nesting.

To get started run your favourite text editor, write:

    print(5) /* prints 5 */

And save it as `hello.v`.

To run a program in V, you need to use script called *run.sh*. It is a bit limited though. Read more about it [here](script.md).

    ./run.sh hello

This will compile program and the result will be called *hello*. If we run it, it will print 5 on screen.

Unlike C and most other imperative programming languages, in V all arithmetic operations are done with functions:

    print(+(2, 3)) /* prints 5 */
    print(*(+(5, 1), 8)) /* prints 48 */
    print(-(8, 5)) /* prints 3 */

Passing too few arguments causes undefined behaviour (usually SEGFAULT).  
On the other hand, when too much arguments are passed, they are just ignored so `+(5, 7, 2)` is same as `+(5, 7)`.

Defining functions
------------------

Examples are priceless:

    my sum(x, y) := +(x, y)
    print(my sum(5, 2)) /* prints 7 */

Important thing to notice is that spaces around `:=` are **required**.

Function name can consist of all characters except: `(`, `)` and `,`. It cannot be reserved name. You can't use character combination ` := ` in it. Colon at the beginning of the name or before space is also forbidden. All spaces at the beginning and end are truncated so only inner spaces are preserved.

Functions may call other functions regardless of the order they are defined in

    print square(12) /* prints 144 */
    print square(name of arg) := print(square(name of arg))
    square(x) := *(x, x)

Argument names are local and you can use same in different functions.

Chaining
--------

Not everything has to be passed as an argument in function, only expressions before comma and closed parenthesis are passed so you can happily debug inside your functions:

    f(x) := +(*(print(x) x, 5), 8) /* defines f(x) = 5x + 8 */
    print(f(5)) /* prints 5 in first line and 33 in second */

Results of various calculations are temporarily stored in two places in memory. Using them shortens the code sometimes and usually makes it more readable.  
One of them holds value of everything evaluated (functions, numeric arguments, passed arguments, etc.).  
`,` and `)` actually mean "make temporary value of last thing evaluated new argument".

    +(2, 8) print() /* prints 10 */

If we want to calculate sum of many numbers, we can naively do one of the following:

    print(+(+(+(+(2, 3), 5), 8), 11)) /* prints 29 */
    print(+(+(+(2, 3), +(5, 8)), 11)) /* same */

We can do better with usage of temporary variable:

    print(+(2, 3) +(,5) +(,8) +(,11)) /* prints 29 */

While this is equally long as our previous attempts, this is more readable.
Notice that it gets changed when you call a function so you can't define a function like this:

    ++(x) := +(,x)
    print(+(2, 3) ++(5) ++(8) ++(11)) /* prints very, very nasty number */

You may notice that it might be useful to have special variable to access result of the last **function** evaluated. 
For that very reason special value `_` is used.

    print(+(2, 3) *(_, _)) /* prints 25 */

Because `_` is preserved when function is called, we can safely define `++`.

    ++(x) := +(_, x)
    print(+(2, 3) ++(5) ++(8) ++(11)) /* prints 29 */

Remember that passing too many arguments is valid and additional arguments are ignored.
`print` doesn't change temporary value. That means that last argument passed is *returned*.

    print(print(5, 6, 8)) /* prints 5 and 8 */

Variables
---------

Global variables are a bit odd in V. Variables are declared in lines which start with `:`. They are separated by comma.

    :a, b

To set variable to particular value use function `=`.

    :var
    =(var, 18) /* var is now 18 */

Every variable is actually pointer to some value so you need to dereference it with function `v` if you want to get its value.

    :var
    =(var, 18)
    print(v(var)) /* prints 18 */

Variables can be function arguments same way like numerical arguments. Note that there is no type safety.

    +=(var, x) := =(var, +(v(var), x))

Function `load` puts input in given variable.

    +=(var, x) := =(var, +(v(var), x))
    :a, b
    load(a) /* loads first number */
    load(b) /* loads second number */
    +=(a, b) /* sums first and second number */
    print(v(a)) /* prints result */

`load` also doesn't change temporary value. There is common idiom `v(load(var))` which returns loaded number.

    :tmp
    print(+(v(load(tmp)), v(load(tmp)))) /* prints sum of two loaded numbers */

Conditionals
------------

In V true and false are represented by 0 and 1 respectively.
V supports a few comparison functions: `==`, `<` and `>`.
It also supports some logical functions: `and`, `or` and `not`.

    print(==(+(2, 2), 5)) /* prints 0 except in 1984 */
    print(>(5, 2)) /* prints 1 */
    print(not(>(8, 1))) /* prints 0 */
    print(and(2, 5)) /* print 1 because both are nonzero */

There is a function `if` which requires 3 arguments.
If first one is true (nonzero) it returns third argument else it returns third.

    abs(x) := if(<(x, 0), -(0, x), x)

Note that `if` isn't short-circuited - all 3 arguments will be evaluated.

    print(if(==(b, 0), 0, /(10, b))) /* still crashes when b is 0 */
    print(if(==(b, 0), 0, /(10, if(b, b, 0)))) /* works */

This means that you can't use `if` to terminate recursion.

There are 2 solutions for that. First one is functions `ijmp` and `jmp` but their use is deprecated.

Better way is `call` function. First argument of call is a function (built-in functions are not allowed though).
`call` passes all other arguments to that function and calls it.

    f(x, y) := +(x, *(x, y))
    print(call(f, 12, 22)) /* prints 276 */

We can finally define our favourite factorial function:

    term() := id(1)
    !(n) := *(n, call(
        if(<(n, 2), term, !),
        -(n, 1)))

There is a function which combines `call` and `if` - `ifc`.
It is a bit different though. First argument is conditional (like in `if`).
Second is function (like in `call`). Other arguments are passed to that function.
Notice that there is no else here. If conditional is false (0), `ifc` won't change value of temporary value.
That means that last argument passed is *returned*.

Now when we know all this we can define out function even more easily:

    !(n) := *(n, ifc(>(n, 1), /* if n > 1: */
        !, -(n, 1), /* call ! with arguments n - 1 */
    1)) /* else return 1 */

Function `ifec` is a combination of `if` and `call`.

Anonymous functions
-------------------

After using `ifc` and `ifec` a lot, defining new function every time becomes a annoying.
There is a solution for that: anonymous functions (also called lambda functions).

To define a lambda function use curly brackets (`{` & `}`).
Function arguments go before ` := ` and function body comes after.
If there are no arguments ` := ` may be omitted.

    gcd(a, b) := ifc(b, /* if b isn't zero */
        {a, b := gcd(b, %(a, b))}, /* call gcd */
        a, b, /* with arguments a and b */
        a) /* otherwise return a */


    sum() := ifc(v(load(ans)), /* if loaded number is not zero */
        {+(v(ans), sum())}, /* add it to a sum */
        0) /* else return 0
        This will return sum of zero terminated number sequence */

While this notation might seem similar to C-blocks it's different -
**you can't reference outer arguments** within anonymous function.

Arrays
------

Arrays are declared just like variables, except that they have their size in parenthesis specified:

    :array(28), variable /* both array of size 28 and variable declared */

Function `@` returns pointer to nth item in the array.
Changing and getting the value of nth item can be done this way:

    =(@(array, n), val) /* changes nth value to val */
    v(@(array, n)) /* gets nth value */

To simplify changing and getting values in array, functions `@=` and `[]` were added.
Code above can be written a bit shorter, with less parenthesis and a lot clearer this way:

    =@(array, n, val)
    [](array, n)

Floats
------

Computations with floats are similar to computations with integers, the difference is that every float function has prefix `f`.

    fprint(f+(1.3, 1.8))

Notice that numbers without dot always represent integers and if used in float functions will cause problems.

To convert float to int use function `int`. To convert int to float use `float`.

Note that some functions for variable management work for both integers and floats.

Hello world
-----------

Text manipulation is very limited in V. You can print string anywhere just by putting wanted text in double quotes (`"`).

    "Hello world\n" /* prints Hello world */

Just like in C, you can use `\n` and `\t` to print newline and tab. You can print text in any function.

    print(+("This is som" 22, "e text!\n", 11) /* prints This is some text and 33 */

Printing text changes both temporary values to length of text.

Function `printr` behaves exactly like `print` but it doesn't use newline so it is useful for combining it with string printing.


Is that all? Well, pretty much it is. You can look into [function list](functions.md) or [deprecated features](obsolete.md) if you are eager.
You can also try to code something in V or just take a look at examples and test (real tests are in *tests/progs/* directory).
