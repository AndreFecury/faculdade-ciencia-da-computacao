import javax.swing.*;
import java.awt.event.*;

public class SimOuNao {
    public static void main(String[] args) {
        JFrame janela = new JFrame("Sim ou Não");
        JButton sim = new JButton("Sim");
        JButton nao = new JButton("Não");
        JPanel painel = new JPanel();

        sim.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                System.out.println("Você clicou em: Sim");
            }
        });

        nao.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                System.out.println("Você clicou em: Não");
            }
        });

        painel.add(sim);
        painel.add(nao);
        janela.add(painel);
        janela.setSize(300, 200);
        janela.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        janela.setVisible(true);
    }
}