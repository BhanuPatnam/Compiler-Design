x <- 10
y <- 3.14
z <- 'A'

print(x)
print(y)
print(z)

function test_types(a, b, c)
    print(a)
    print(b)
    print(c)
    return a
end function

res <- test_types(x, y, z)
print(res)
