num1 = int(input("Digite o primeiro valor: "))
num2 = int(input("Digite o segundo valor: "))

soma = num1 + num2
sub = num1 - num2
mult = num1 * num2
div = num1 / num2
porc = num2 % num1
poten = num1 ** num2
div_int = num1 // num2

print(f"A soma de {num1} + {num2} é {soma}")
print(f"A subtração de {num1} - {num2} é {sub}")
print(f"o produto de {num1} * {num2} é {mult}")
print(f"{num1} dividido por {num2} é {div}")
print(f"{num2}% de {num1} é {porc}")
print(f"{num1} elevado a {num2} é {poten}")
print(f"A divisão de {num1} por {num2} é {div_int}")
