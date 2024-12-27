with open("1.txt") as f:
    xys = (map(int, line.split()) for line in f)
    xs, ys = zip(*xys)
    result = sum(x * sum(x == y for y in ys) for x in xs)
    print(result)
