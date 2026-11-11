import os
os.system('cls')

salarios = [2000, 1200, 5000, 3000]
total = 0

for salario in salarios:
    total = total + salario

print(f"O total é --> R${total}")