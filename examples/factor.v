term() := id(1)
!(n) := *(n, call(
    ?(<(n, 2), term, !),
    -(n, 1)))
print(!(12))
