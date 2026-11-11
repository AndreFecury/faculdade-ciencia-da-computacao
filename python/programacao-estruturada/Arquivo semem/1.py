import os
os.system('cls')

arquivo = open("C:\\Users\\Fecur\\OneDrive\\Documentos\\Programas\\Programação estruturada\\Arquivo semem\\qualquercoisa.txt", "r")
#print(arquivo.readable()) # para verificar se o arquivo pode ser lido
#print(arquivo.read())

print (arquivo.readline())
print (arquivo.readline())
print (arquivo.readline())
print (arquivo.readline())
print (arquivo.readline())
print (arquivo.readline())
print (arquivo.readline())
print (arquivo.readline())

arquivo.close()