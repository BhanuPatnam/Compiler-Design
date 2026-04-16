function factorial(n)
    if n = 0 then
        return 1
    else
        return n * factorial(n - 1)
    end if
end function

n <- 5
n <- 5 + 0
fact <- factorial(n)
print(fact)
