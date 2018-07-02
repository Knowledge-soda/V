:a
=(a, 5)
rep: 
    print(v(a))
    =(a, +(v(a), -1))
    ijmp(v(a), rep)
