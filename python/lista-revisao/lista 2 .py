import os 
os.system("cls")

#operações matematicas

num1 = int(input("Digite o primeiro número: "))
num2 = int(input("Digite o segundo número: "))

soma = print(f"Soma", num1 + num2)
subtracao = print(f"Subtração", num1 - num2)
multiplicacao = print(f"Multiplicação", num1 * num2)
divisao = print(f"Divisão", num1 / num2)
potenciacao = print(f"Potenciação", num1 ** num2)
radiciacao = print(f"Radiciação", num1 // num2)
resto = print(f"Resto da divisao", num1 % num2)

print(f"{num1} + {num2} = {num1 + num2}")
print(f"{num1} - {num2} = {num1 - num2}")
print(f"{num1} × {num2} = {num1 * num2}")
print(f"{num1} ÷ {num2} = {num1 / num2}")
print(f"{num1} // {num2} = {num1 // num2} (divisão inteira)")
print(f"{num1} % {num2} = {num1 % num2} (resto da divisão)")
print(f"{num1} ^ {num2} = {num1 ** num2} (potência)")


