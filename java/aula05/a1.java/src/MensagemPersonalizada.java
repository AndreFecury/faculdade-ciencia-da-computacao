import javax.swing.*;
import java.awt.event.*;

public class MensagemPersonalizada {
    public static void main(String[] args) {
        JFrame janela = new JFrame("Saudação");
        JTextField campo = new JTextField(15);
        JButton botao = new JButton("Enviar");
        JPanel painel = new JPanel();

        painel.add(campo);
        painel.add(botao);

        botao.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String nome = campo.getText();
                JOptionPane.showMessageDialog(null, "Olá, " + nome + "! Seja bem-vindo!");
            }
        });

        janela.add(painel);
        janela.setSize(400, 200);
        janela.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        janela.setVisible(true);
    }
}
