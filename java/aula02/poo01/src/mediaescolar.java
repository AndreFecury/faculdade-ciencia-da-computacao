public class mediaescolar {
    public static void main(String[] args) {
        
        //notas
        double nota1 = 8.5;
        double nota2 = 9.3;

        //medias
        double media = (nota1 + nota2) / 2;

        System.out.println("Media: " + media);

        if(media >= 7)
            System.out.println("Aprovado");
        else
            System.out.println("Reprovado");
    }
        
        
}
