import os 
os.system("cls")

#input com todas as variaveis e print ddo tipo das variaveis 

input_str = input("Tem q ver com os cara lá? ")
input_int = int(input("Melhor número: "))
input_float = float(input("Digite o melhor número decimal: "))
input_bool = bool(input("Vc é betinha? "))

print(f"string {type(input_str)}")
print(f"Inteiro {type(input_int)}")
print(f"Float {type(input_float)}")
print(f"bool {type(input_bool)}")


#imprimir as variaveis utilizando f string

print(f"String: {input_str}, Inteiro: {input_int}, Float: {input_float}, Bool: {input_bool}")



