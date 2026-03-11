def calculate_sum(n):
    sum = 0
    for i in range(1, n + 1):
        sum = (sum + i)

    return sum

n = 10
result = calculate_sum(n)
print(result)
if (result > 50):
    print(1)
else:
    print(0)

