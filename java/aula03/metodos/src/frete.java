class Frete {
    double calculaFrete(double peso, double valorporkg) {
        return peso * valorporkg;
    }

    double calculaFrete(double peso, double valorporkg, double taxaexpressa) {
        return (peso * valorporkg)  + taxaexpressa;
    }
}
