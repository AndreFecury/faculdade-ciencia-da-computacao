class Paciente {
    String nome;
    int idade;

    Paciente(String nome, int idade) {
        this.nome = nome;
        this.idade = idade;
    }

    String verifiarPrioridade() {
        if (idade >= 60) return "Atendimento prioritario";
        return "Atendimento normal";
    }
}