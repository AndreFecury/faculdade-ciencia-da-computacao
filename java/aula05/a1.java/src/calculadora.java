import javax.swing.*;
import java.awt.event.*;

public class calculadora {
    public static void main(String[] args) {
        JFrame janela = new JFrame("Calculadora");
        JTextField num1 = new JTextField(5);
        JTextField num2 = new JTextField(5);
        JButton soma = new JButton("+");
        JButton sub  = new JButton("-");
        JButton mult = new JButton("×");
        JButton div  = new JButton("÷");
        JPanel painel = new JPanel();

        painel.add(new JLabel("Num 1:"));
        painel.add(num1);
        painel.add(new JLabel("Num 2:"));
        painel.add(num2);
        painel.add(soma);
        painel.add(sub);
        painel.add(mult);
        painel.add(div);

        ActionListener calcular = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                double n1 = Double.parseDouble(num1.getText());
                double n2 = Double.parseDouble(num2.getText());
                double resultado = 0;
                String operacao = e.getActionCommand();

                switch (operacao) {
                    case "+": resultado = n1 + n2; break;
                    case "-": resultado = n1 - n2; break;
                    case "×": resultado = n1 * n2; break;
                    case "÷":
                        if (n2 == 0) {
                            JOptionPane.showMessageDialog(null, "Erro: divisão por zero!");
                            return;
                        }
                        resultado = n1 / n2;
                        break;
                }

                JOptionPane.showMessageDialog(null, "Resultado: " + resultado);
            }
        };

        soma.addActionListener(calcular);
        sub.addActionListener(calcular);
        mult.addActionListener(calcular);
        div.addActionListener(calcular);

        janela.add(painel);
        janela.setSize(350, 200);
        janela.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        janela.setVisible(true);
    }
}