:a, b
=(a, 5)
=(b, 0)
--(var) := =(var, +(v(var), -1))
++(var) := =(var, +(v(var), 1))
print(rep: --(a) ++(b) ijmp(v(a), rep) v(b))
