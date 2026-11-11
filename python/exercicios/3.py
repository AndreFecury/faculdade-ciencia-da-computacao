import os
os.system('cls')

senhas = ["abc", "osmlktapá", "123456", "12345678", "123456789"]

len(senhas)

for senha in senhas:
    if len(senha) >= 6:
        print(f'{senha} é uma senha válida!\n')
    else:
        print(f'{senha} é uma senha inválida!\n')