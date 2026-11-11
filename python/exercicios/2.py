import os
os.system('cls')

n1 = int(input('Digite o primero número \n''  R: '))
n2 = int(input('Digte o segundo número \n '' R: '))

print('\n----RESULTADO----\n')

if n1 > n2:
    print(f"{n1} é maior que {n2}!")
elif n1 == n2:
    print(f"{n1} é igual a {n2}!")
else:
    print(f"{n2} é maior que {n1}!")
    
print('\n-----------------\n')
    