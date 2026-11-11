class Estacionamento{
    double calcularvalor(int horas, double valorhora){
        return horas*valorhora;

    }

    double calcularvalor(int horas, double valorhora, double desconto){
        return (horas*valorhora)-desconto;
    }

}
