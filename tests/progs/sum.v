:a, b
+=(var, n) := =(var, +(v(var), n))
sum(x) := v(=(b, 0) =(a, 0) rep: =(b, +(v(b), 1)) +=(a, v(b)) ijmp(<(v(b), x), rep) a)
print(sum(12))
