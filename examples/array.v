:array size, array(1000), read
+=(var, n) := =(var, +(v(var), n))
add(n) := @=(array, v(array size), n,
            +=(array size, 1))
delete(n) := if-else(==(+(n, 1), v(array size)),
    {+=(array size, -1)},
    {n := delete(@=(array, n, [](array, +(n, 1))) +(n, 1))},
    n
)
delete all(n, i, j) := if-else(==(i, v(array size)),
    {n, i, j := =(array size, j)},
    {n, i, j := if-else(==([](array, i), n),
        {n, i, j := delete all(n, +(i, 1), j)},
        {n, i, j := delete all(@=(array, j, [](array, i))
                               n, +(i, 1), +(j, 1))},
         n, i, j
    )},
     n, i, j
)
print array(n) := if(<(n, v(array size)),
    {n := print array(print([](array, n))
        +(n, 1))}, n)

main(v(load(read)))
main(n) := if(n,
    {n := main(-(n, 1),
        load(read)
        /* print(v(read)) */
        if-else(==(v(read), 1),
            {add(v(load(read)))},
        {if-else(==(v(read), 2),
            {delete(-(v(load(read)), 1))},
        {if-else(==(v(read), 3),
            {delete all(v(load(read)), 0, 0)},
            {print array(0)})})},
    ))},
    n
)
