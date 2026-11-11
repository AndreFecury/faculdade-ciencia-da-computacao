import os
os.system("cls")

arquivo = open("C:\\Users\\Fecur\\OneDrive\\Documentos\\Programas\\Programação estruturada\\Arquivo semem\\qualquercoisa.txt", "r")
lista = arquivo.readlines()

#print (lista)

for n in lista:
    print (n)

arquivo.close