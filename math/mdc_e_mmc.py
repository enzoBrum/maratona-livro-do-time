def mdc(a, b):
    dividendo = a
    divisor = b
    if b > a:
        dividendo = b
        divisor = a
    while 1:
        resto = dividendo % divisor
        if resto == 0:
            break
        dividendo, divisor = divisor, resto
    return divisor

#Dont use without defining mdc().
def mmc(a, b):
    return a * b // mdc(a, b)
