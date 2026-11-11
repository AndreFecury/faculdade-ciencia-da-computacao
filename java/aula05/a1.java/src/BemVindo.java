import javax.swing.*;
import java.awt.event.*;

public class BemVindo {

    public static void main(String[] args) {
        JFrame janela = new JFrame("Bem-vindo");
        JButton botao = new JButton("Clique aqui");

        botao.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JOptionPane.showMessageDialog(null, "Bem-vindo!");
            }
        });

        janela.add(botao);
        janela.setSize(300, 200);
        janela.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        janela.setVisible(true);
    }
}