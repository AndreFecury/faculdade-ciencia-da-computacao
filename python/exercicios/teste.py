import os
os.system('cls')

salario = float(input('digite o salario mensal do funcionario: '))
horas = int(input('digite o número de horas trabalhadas no mês: '))

valor_hora = salario/horas
print(f'o valor por hora do funcionario é {valor_hora}')

