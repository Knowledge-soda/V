Script Instructions
===================

Script *run.sh* is used to simplify compilation.

    ./run.sh name

This line compiles *name.v* in *name*. It is actually 3 step process.
    - first step is generating *name.asm* from *name.v*
    - second step is assembling *name.o* from *name.asm*
    - third step is compiling (linking) *name* from *name.o*
    - at the and *name.asm* and *name.o* are deleted

By default all steps are executed. If you want only some steps (or you want to keep *name.asm* or *name.o*) you can use special options starting with *-* either before or after *name*. Options are *g*, *a* and *c*. If you want to generate - use *g*, if you want to assemble - *a* and to link (compile) *c*.

    ./run.sh name -gac

Order of options is completely irrelevant, `-acg` is same as `-gac`. Note that when previous command is run, *name.asm* and *name.o* are deleted.
To delete them put *t* after *g* and *a* respectively. In example:

    /run.sh -gta name

This will create object file.

When using script without option it is same as using:

    /run.sh -gtatc name

Generating assembly code is done by piping program to *main* program. *main* reads from standard input and writes assembly code to standard output.

Assembling is done with NASM assembler. Format used if *elf32*. There are some debugger options.

GCC link object file. Notice that *-m32* flag is required.

In newest version (1.3) *preprocessor* is added. Now, file is first given to *preprocessor* which then pipes output to *main*. You might have to change some things in preprocessor source code if you want to use it on Windows (it uses `/` for directory separator which might cause errors).

To use V with Windows, you should probably use different format for assembler and different compiler. If you want to use script, you will probably have to rewrite it in Windows scripting language.
