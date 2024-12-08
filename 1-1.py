with open("1.txt") as f:
    xys = (map(int, line.split()) for line in f)
    xs, ys = map(sorted, zip(*xys))
    result = sum(abs(a - b) for a, b in zip(xs, ys))
    print(result)
