Obsolete features
==================

V has very similar feature like `goto`s in C. You can define label which must end in colon i.e. `repeat:`. You can then *jump* to that code.
There are 2 jump function: `jmp` and `ijmp`.

`jmp` is unconditional jump just like `goto` in C, it takes one argument - label on which it will jump.

`ijmp` is conditional jump, it takes 2 arguments - condition and label. If condition is true, it will jump on label.

Both `jmp` and `ijmp` can only jump on place which is defined in same function argument space.

Simple loop with `ijmp`:

    :a
    =(a, 5)
    rep: 
        print(v(a))
        =(a, +(v(a), -1))
        ijmp(v(a), rep))

Indentation is completely irrelevant (just use spaces and not tabs). Note that there is space after `rep:` which is mandatory.

Just like `goto`s in C, `jmp` and `ijmp` should be used carefully. It is much wiser to use `ifc` and recursion instead.
