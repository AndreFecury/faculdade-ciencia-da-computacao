public class comissao {

    public static void main(String[] args) {
        
        //valor total de vendas
        double totalVendas = 2500.0;
        
        //comissao 
        double comissao = totalVendas * 0.1;

        System.out.println("Total de vendas: " + totalVendas);
        System.out.println("Comissao: " + comissao);
        System.out.println("Total Final: " + (totalVendas + comissao));
    }
        
    
}
