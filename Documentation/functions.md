
| Function name | Input | Output | Notes |
| ------------- | ----- | ------ | ----- |
| `id` | 1 argument | input | |
| `+` | 2 arguments | sum | |
| `-` | 2 arguments | difference | |
| `*` | 2 arguments | factor | |
| `/` | 2 arguments | fraction | integer division |
| `%` | 2 arguments | division remainder | |
| `print` | 1 argument | | prints argument |
| `printr` | 1 argument | | prints argument without newline |
| `load` | variable | | stores number from stdin to variable |
| `=` | variable and value | value | stores value in variable |
| `v` | variable | value of variable | |
| `if` | condition and 2 arguments | first arguments if condition is true, second otherwise | |
| `==` | 2 arguments | 1 if they are same, 0 otherwise | |
| `<` | 2 arguments | 1 if first is smaller than second, 0 otherwise | |
| `>` | 2 arguments | 1 if first is bigger than second, 0 otherwise | |
| `and` | 2 condition | conjunction | |
| `or` | 2 condition | disjunction | |
| `not` | 1 condition | negation | |
| `call` | function and its arguments | whatever function outputs | calls function with arguments |
| `ifc` | condition, function and its arguments | | calls function if condition is true |
| `ifec` | condition, 2 functions and their arguments | | calls first function if condition is true, second otherwise |
| `@` | array and index | | pointer to nth item in array | |
| `@=` | array, index and argument | array | changes nth item in array to argument |
| `[]` | array and index | nth item in array | |
| `f+` | 2 floats | sum | |
| `f-` | 2 floats | difference | |
| `f*` | 2 floats | factor | |
| `f/` | 2 floats | fraction | |
| `fprint` | 1 float | | prints argument |
| `fprintr` | 1 float | | prints argument without newline |
| `f==` | 2 floats | 1 if they are same, 0 otherwise | floats are rarely really same |
| `f<` | 2 floats | 1 if first is smaller than second, 0 otherwise | |
| `f>` | 2 floats | 1 if first is bigger than second, 0 otherwise | |
| `int` | 1 float | integer | |
| `float` | 1 integer | float | |
| `jmp` | place | | [obsolete](obsolete.md) |
| `ijmp` | condition and place | | [obsolete](obsolete.md) |
