import os
os.system('cls')

hokages = ["Hashirama" , "Tobirama" , "Hiruzen" , "Minato" , "Tsunade"]
senseis = ["Assuma" , "Kurenai" , "Kakashi" , "Guy"]
print(hokages)
print(senseis)

hokages.append(senseis) # Adiciona uma lista na outra
print(hokages)

hokages.insert(4, "Jiraya") # Insere um elemento na lista
print(hokages)

hokages.pop() # Remove o ultimo da lista
print(hokages)

hokages.remove("Hiruzen") # Orochimaru só tinha um trabalho
print(hokages)

hokages.clear() # Limpa a lista
print(hokages) 
