import os
os.system("cls")

# 1. Solicitando os 4 tipos de variáveis
var_int = int(input("Digite um número inteiro: "))
var_float = float(input("Digite um número decimal: "))
var_str = input("Digite um texto: ")
var_bool = bool(input("Digite True ou False: ").capitalize() == "True")

# Imprimindo os tipos
print("\n1. Tipos das variáveis:")
print(f"Inteiro: {type(var_int)}")
print(f"Float: {type(var_float)}")
print(f"String: {type(var_str)}")
print(f"Bool: {type(var_bool)}")

# 2. Imprimindo com f-strings
print("\n2. Valores com f-strings:")
print(f"Inteiro: {var_int}, Float: {var_float}, String: '{var_str}', Bool: {var_bool}")

# 3. Operações com 2 números
num1 = float(input("\nDigite o primeiro número: "))
num2 = float(input("Digite o segundo número: "))

print("\n3. Operações:")
print(f"Soma: {num1 + num2}")
print(f"Subtração: {num1 - num2}")
print(f"Multiplicação: {num1 * num2}")
print(f"Divisão: {num1 / num2}")
print(f"Divisão Inteira: {num1 // num2}")
print(f"Módulo: {num1 % num2}")
print(f"Potência: {num1 ** num2}")

# 4. Operações com f-strings explicadas
print("\n4. Explicações:")
print(f"{num1} + {num2} = {num1 + num2}")
print(f"{num1} - {num2} = {num1 - num2}")
print(f"{num1} × {num2} = {num1 * num2}")
print(f"{num1} ÷ {num2} = {num1 / num2}")
print(f"{num1} // {num2} = {num1 // num2} (divisão inteira)")
print(f"{num1} % {num2} = {num1 % num2} (resto da divisão)")
print(f"{num1} ^ {num2} = {num1 ** num2} (potência)")

# 5. Trabalhando com listas
lista1 = [10, 20, 30, 40, 50]
lista2 = [1, 2, 3, 4, 5]

print("\n5.a) Índices:")
print(f"lista1[0] = {lista1[0]}, lista1[2] = {lista1[2]}, lista1[4] = {lista1[4]}")
print(f"lista2[1] = {lista2[1]}, lista2[3] = {lista2[3]}, lista2[4] = {lista2[4]}")

print("\n5.b) Concatenando listas:")
lista1.extend(lista2)
print(f"Lista1 após extend: {lista1}")

print("\n5.c) Métodos de lista:")
# i) Append
lista1.append(99)
print(f"Append(99): {lista1}")

# ii) Insert
lista1.insert(2, 55)
print(f"Insert(2, 55): {lista1}")

# iii) Pop
removido = lista1.pop()
print(f"Pop(): removido {removido}, lista: {lista1}")

# iv) Remove
lista1.remove(55)
print(f"Remove(55): {lista1}")

# v) Clear
copia_temporaria = lista1.copy()
copia_temporaria.clear()
print(f"Clear(): {copia_temporaria}")

# vi) Index
indice = lista1.index(20)
print(f"Index(20): {indice}")

# vii) Count
contagem = lista1.count(10)
print(f"Count(10): {contagem}")

# viii) Sort
lista1.sort()
print(f"Sort(): {lista1}")

# ix) Reverse
lista1.reverse()
print(f"Reverse(): {lista1}")

print("\n5.d) Cópias de lista:")
copia1 = lista1.copy()
copia2 = lista1[:]
print(f"Cópia 1 (copy()): {copia1}")
print(f"Cópia 2 (fatiamento): {copia2}")

# 6. Testes condicionais
condicao = True
print("\n6. Testes condicionais:")
if condicao:
    print("Condição é True")
else:
    print("Condição é False")

# 7. Operadores lógicos
a = 5
b = 10
print("\n7. Operadores lógicos:")
if a > b and a != 0:
    print("a é maior que b E diferente de zero")
elif a < b or b == 10:
    print("a é menor que b OU b é igual a 10")
else:
    print("Nenhuma condição atendida")

# 8. Alcançando intervalo superior
numero = int(input("\nDigite um número: "))
inferior = int(input("Limite inferior: "))
superior = int(input("Limite superior: "))

print(f"\n8. Alcançando {superior} a partir de {numero}:")
while numero < superior:
    print(f"Número atual: {numero}")
    numero += 1
print("Intervalo alcançado!")

# 9. Range com 3 parâmetros
print("\n9. Range com passo 2:")
for i in range(0, 10, 2):
    print(i)

# 10. Iterando em lista
print("\n10. Iterando em lista:")
lista = ['maçã', 'banana', 'laranja', 'uva', 'pera']
for fruta in lista:
    print(fruta)
print("Todos os elementos impressos!")

# 11. Iterando em string
print("\n11. Caracteres da string:")
palavra = "Python"
for letra in palavra:
    print(letra)

# 12. Listas aninhadas
print("\n12. Listas aninhadas:")
listas = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
print("Início do loop")
for lista in listas:
    print(lista)
print("Fim do loop")

# 13. Break e Continue
numeros = [2, 4, 6, 3, 8, 10, 5, 12, 14, 16]

print("\n13.a) Break no primeiro ímpar:")
for num in numeros:
    if num % 2 != 0:
        print(f"Número ímpar encontrado: {num}")
        break
    print(num)
print(f"Lista completa: {numeros}")

print("\n13.b) Continue para pares:")
for num in numeros:
    if num % 2 == 0:
        print(f"Número par: {num}")
        continue
    print(f"Ímpar encontrado: {num}")
print(f"Lista completa: {numeros}")