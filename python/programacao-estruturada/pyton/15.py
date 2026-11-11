import os
os.system('cls')

lanche = int(input("Qual lanche você quer comprar?"))

match lanche:
    case 1:
        print("Misto quente.")
    case 2:
        print("X-Burguer.")
    case 3:
        print("X-Salada.")
    case 4:
        print("Pizza.")
    case 5:
        print("Coxinha.")
    case _:
        print("Não temos esse lanche.") 
        
        
forma_de_pagamento = int(input("Qual a forma de pagamento?"))

match forma_de_pagamento:
    case 1:
        print("Dinheiro.")
    case 2:
        print("Cartão.")
    case 3:
        print("Pix.")
    case _:
        print("Não vendo nada fiado.")
