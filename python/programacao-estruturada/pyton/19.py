import os
os.system('cls')

folha = ["Hashirama" , "Tobirama" , "Hiruzen" , "Minato" , "Tsunade"]
pedra = ["Ishiwaka", "Mu", "Oonoki", "Kurotsuchi" ]
nuvem = ["Shodaime", "Nidaime", "Sandaime", "Yondaime", "Godaime "]

for kages in folha:             
    print(folha)

for kages in pedra:
    print(pedra)                            # <---- prints de forma errada
    
for kages in nuvem:
    print(nuvem)
    
else:
    print("Todas as listas foram impressos.")

# ---prints de outra maneira---

for kages in folha:
    print (kages)    
    
for kages in pedra:
    print (kages)                           # <---- prints de forma correta
    
for kages in nuvem:
    print (kages)
    
else:
    print("Todas as listas foram impressos.")