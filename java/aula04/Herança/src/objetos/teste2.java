package objetos;

public class teste2 {

    public static void barulho(animal Animal2){
        Animal2.fazerbarulho();
    }
    
    public static void main (String[] args) {
        cachorro cao = new cachorro();

        barulho(cao);
    }
}
