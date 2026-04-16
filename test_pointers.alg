int x
int * p
int ** pp

x <- 42
p <- &x
pp <- &p

print(x)
print(*p)
print(**pp)

**pp <- 100
print(x)
print(*p)
