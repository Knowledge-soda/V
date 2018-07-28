term() := id(1)
!(n) := *(n, if-else(<(n, 2),
    term, !, -(n, 1)))
print(!(12))
