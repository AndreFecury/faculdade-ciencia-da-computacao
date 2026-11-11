import os 
os.system("cls")

#arquivo = open ("teste2.txt", "w")
#arquivo.write("os coiso dos cara la ")
#arquivo.close

if os .path.exists("teste2.txt"):
    os.remove("teste2.txt")
else:
     print("O arquivo não existe")