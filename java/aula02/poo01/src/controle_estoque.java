public class controle_estoque {

    public static void main(String[] args) {
        
        //variaveis

        int estoqueinicial =100;
        int quantidadevendida =99;
        int estoquerestante = estoqueinicial - quantidadevendida;
    
        System.out.println("Estoque inicial: " + estoqueinicial);
        System.out.println("Quantidade vendida: " + quantidadevendida);
        System.out.println("Estoque restante: " + estoquerestante);

        if (estoquerestante < 10);
            System.out.println("Estoque baixo");

           

    }
    

}
 