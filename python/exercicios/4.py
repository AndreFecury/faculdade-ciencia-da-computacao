import os 
os.system('cls')

usuario = ''
senha = ''
tentativas = 0

while (usuario != 'andré' and senha != 'paocomovo') and tentativas < 3:
    usuario = input('\nDigite o nome de usuario \n  R: ')
    senha = input('\nDigite a senha\n  R: ')
    tentativas += 1

if usuario != 'andre' and senha != 'paocomovo':
    print('\n---AGUARDE 30 MINUTOS PARA TENTAR NOVAMENTE---\n')
else:
    print('\n---LOGIN BEM-SUCEDIDO!---\n')