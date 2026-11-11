public class App {
    public class main {
        public static void main(String[] args) {
            
            //clinica 

            System.out.println("\\n=== Clínica ===");
            Paciente p1 = new Paciente("João", 67);
            Paciente p2 = new Paciente("Maria", 30);
            System.out.println(p1.nome + ": " + p1.verifiarPrioridade());
            System.out.println(p2.nome + ": " + p2.verifiarPrioridade());

            //lanche

            System.out.println("\\n=== Lanche ===");
            Pedido pedido = new Pedido();
            System.out.println("Pedido 2 itens: R$ " + pedido.calcular(12.0, 5.0));
            System.out.println("Pedido 3 itens: R$ " + pedido.calculartotal(12.0, 5.0, 8.0));

            //Estacionamento
            System.out.println("\n=== Estacionamento ===");
            Estacionamento est = new Estacionamento();
            System.out.println("Sem desconto: R$ " + est.calcularvalor(3, 10.0));
            System.out.println("Com desconto: R$ " + est.calcularvalor(3, 10.0, 5.0));

            //Frete
            System.out.println("\n=== Frete ===");
            Frete frete = new Frete();
            System.out.println("Frete normal: R$ " + frete.calculaFrete(2.5, 8.0));
            System.out.println("Frete expresso: R$ " + frete.calculaFrete(2.5, 8.0, 15.0));
    }
}

}