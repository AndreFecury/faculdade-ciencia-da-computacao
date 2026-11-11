import os 
os.system('cls')

# Definindo o número de alunos
NUM_ALUNOS = 5

# 1. Inicialização das listas (equivalente aos vetores em Portugol)
nomes = []
nota1 = []
nota2 = []
medias = []

print("--- Cadastro de Notas ---")

# --- 2. Entrada de Dados e Cálculo da Média ---
# O 'range(NUM_ALUNOS)' gera números de 0 a 4 (5 iterações)
for i in range(NUM_ALUNOS):
    print(f"\nAluno {i + 1}:") # Usamos i + 1 para a contagem visual (1 a 5)

    # Leitura do nome
    nome = input("   Nome: ")
    nomes.append(nome)

    # Leitura da primeira nota, convertendo para float
    while True:
        try:
            n1 = float(input("   Primeira Nota (N1): "))
            if 0 <= n1 <= 10: # Validação básica de nota (opcional, mas bom)
                nota1.append(n1)
                break
            else:
                print("Nota inválida. Digite um valor entre 0 e 10.")
        except ValueError:
            print("Entrada inválida. Digite um número.")

    # Leitura da segunda nota, convertendo para float
    while True:
        try:
            n2 = float(input("   Segunda Nota (N2): "))
            if 0 <= n2 <= 10: # Validação básica de nota (opcional, mas bom)
                nota2.append(n2)
                break
            else:
                print("Nota inválida. Digite um valor entre 0 e 10.")
        except ValueError:
            print("Entrada inválida. Digite um número.")

    # Cálculo da média e armazenamento
    media = (n1 + n2) / 2
    medias.append(media)

print("\n" + "-" * 30)

# --- 3. Geração da Listagem de Saída ---
print("\n--- Listagem de Alunos e Médias ---\n")

# Imprime o cabeçalho usando formatação para alinhar as colunas
# O '<20' alinha à esquerda em 20 espaços; o '^8' centraliza em 8 espaços
print(f"{'Nome':<20}{'N1':^8}{'N2':^8}{'Média':^8}")
print("-" * 44)

# Itera sobre os dados armazenados para imprimir a listagem
for i in range(NUM_ALUNOS):
    # Imprime os dados, usando .2f para formatar as notas com 2 casas decimais
    print(f"{nomes[i]:<20}{nota1[i]:^8.2f}{nota2[i]:^8.2f}{medias[i]:^8.2f}")

print("\n--- Fim da Listagem ---")