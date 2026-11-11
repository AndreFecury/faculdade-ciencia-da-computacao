import javax.swing.*; 
 
public class textosimples { 
    public static void main(String[] args) { 
        JFrame janela = new JFrame("Sistema Escolar"); 
        JLabel texto = new JLabel("Olá, meu nome é André!"); 
 
        janela.add(texto); 
 
        janela.setSize(500, 400); 
        janela.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); 
        janela.setVisible(true); 
    } 
}
