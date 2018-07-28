!(n) := *(n, if(>(n, 1), /* if n < 2: */
    !, -(n, 1), /* call ! with arguments n - 1 */
1)) /* else return 1 */
print(!(12))
