import os
os.system('cls')

idade = int(input("Qual a sua idade? ")) 

if  idade >= 60:
   
    print("Você é Idoso.")

elif idade >= 18 and idade <= 59:
    
    print("Voce é Adulto.")

elif idade <= 17 and idade >= 13:
   
    print("Você é Adolescente.")

elif idade <= 12 and idade >= 3:
    
    print("Você é Criança.")

elif idade <= 2 and idade >= 0:
    
    print("Voce é um Bebê.")
    
else:
    
    print("Idade inválida.")