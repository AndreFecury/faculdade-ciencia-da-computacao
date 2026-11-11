import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.RenderingHints;
import java.awt.event.ActionListener;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.border.EmptyBorder;

public class calc extends JFrame {

    // ─── Estado da calculadora ───────────────────────────────────────
    private double numeroAnterior = 0;
    private String operadorAtual = "";
    private boolean novaEntrada = true;
    private boolean acabouDeCalcular = false;

    // ─── Componentes visuais ─────────────────────────────────────────
    private JLabel displayPrincipal;
    private JLabel displayOperacao;

    // ─── Paleta de cores ─────────────────────────────────────────────
    private static final Color COR_FUNDO = new Color(28, 28, 30);
    private static final Color COR_DISPLAY_BG = new Color(28, 28, 30);
    private static final Color COR_BTN_NUMERO = new Color(58, 58, 60);
    private static final Color COR_BTN_OP = new Color(255, 159, 10);
    private static final Color COR_BTN_FUNC = new Color(72, 72, 74);
    private static final Color COR_BTN_HOVER_N = new Color(80, 80, 82);
    private static final Color COR_BTN_HOVER_O = new Color(255, 185, 70);
    private static final Color COR_BTN_HOVER_F = new Color(95, 95, 97);
    private static final Color COR_TEXTO_BRANCO = new Color(255, 255, 255);
    private static final Color COR_TEXTO_ESCURO = new Color(28, 28, 30);

    // ─── Construtor ──────────────────────────────────────────────────
    public calc() {
        setTitle("Calculadora");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setResizable(false);
        getContentPane().setBackground(COR_FUNDO);

        JPanel painelPrincipal = new JPanel(new BorderLayout());
        painelPrincipal.setBackground(COR_FUNDO);
        painelPrincipal.setBorder(new EmptyBorder(20, 16, 20, 16));

        // Display
        painelPrincipal.add(criarDisplay(), BorderLayout.NORTH);

        // Botões
        painelPrincipal.add(criarBotoes(), BorderLayout.CENTER);

        add(painelPrincipal);
        pack();
        setLocationRelativeTo(null);
    }

    // ─── Display ─────────────────────────────────────────────────────
    private JPanel criarDisplay() {
        JPanel painel = new JPanel();
        painel.setLayout(new BoxLayout(painel, BoxLayout.Y_AXIS));
        painel.setBackground(COR_DISPLAY_BG);
        painel.setBorder(new EmptyBorder(10, 8, 18, 8));

        // Linha de operação anterior (pequena)
        displayOperacao = new JLabel(" ");
        displayOperacao.setFont(new Font("SF Pro Display", Font.PLAIN, 18));
        displayOperacao.setForeground(new Color(160, 160, 165));
        displayOperacao.setAlignmentX(Component.RIGHT_ALIGNMENT);

        // Número principal (grande)
        displayPrincipal = new JLabel("0");
        displayPrincipal.setFont(new Font("SF Pro Display", Font.PLAIN, 72));
        displayPrincipal.setForeground(COR_TEXTO_BRANCO);
        displayPrincipal.setAlignmentX(Component.RIGHT_ALIGNMENT);

        painel.add(displayOperacao);
        painel.add(displayPrincipal);

        return painel;
    }

    // ─── Grade de botões ─────────────────────────────────────────────
    private JPanel criarBotoes() {
        JPanel grade = new JPanel(new GridLayout(5, 4, 10, 10));
        grade.setBackground(COR_FUNDO);

        // Linha 1
        grade.add(botaoFuncao("AC", e -> limparTudo()));
        grade.add(botaoFuncao("+/-", e -> inverterSinal()));
        grade.add(botaoFuncao("%", e -> porcentagem()));
        grade.add(botaoOperador("÷", "÷"));

        // Linha 2
        grade.add(botaoNumero("7", "7"));
        grade.add(botaoNumero("8", "8"));
        grade.add(botaoNumero("9", "9"));
        grade.add(botaoOperador("×", "×"));

        // Linha 3
        grade.add(botaoNumero("4", "4"));
        grade.add(botaoNumero("5", "5"));
        grade.add(botaoNumero("6", "6"));
        grade.add(botaoOperador("−", "−"));

        // Linha 4
        grade.add(botaoNumero("1", "1"));
        grade.add(botaoNumero("2", "2"));
        grade.add(botaoNumero("3", "3"));
        grade.add(botaoOperador("+", "+"));

        // Linha 5 — botão 0 ocupa 2 colunas (simulado com painel)
        JPanel zeroPanel = new JPanel(new BorderLayout());
        zeroPanel.setBackground(COR_FUNDO);
        zeroPanel.add(botaoZero(), BorderLayout.CENTER);

        grade.add(zeroPanel);
        grade.add(new JLabel()); // espaço vazio (já ocupado pelo zero largo)
        grade.add(botaoNumero(",", "."));
        grade.add(botaoOperador("=", "="));

        return grade;
    }

    // ─── Fábricas de botões ──────────────────────────────────────────
    private BotaoRedondo botaoNumero(String texto, String valor) {
        BotaoRedondo btn = new BotaoRedondo(texto, COR_BTN_NUMERO, COR_BTN_HOVER_N, COR_TEXTO_BRANCO);
        btn.addActionListener(e -> digitarNumero(valor));
        return btn;
    }

    private BotaoRedondo botaoOperador(String texto, String op) {
        BotaoRedondo btn = new BotaoRedondo(texto, COR_BTN_OP, COR_BTN_HOVER_O, COR_TEXTO_ESCURO);
        btn.addActionListener(e -> {
            if (op.equals("="))
                calcular();
            else
                definirOperador(op);
        });
        return btn;
    }

    private BotaoRedondo botaoFuncao(String texto, ActionListener acao) {
        BotaoRedondo btn = new BotaoRedondo(texto, COR_BTN_FUNC, COR_BTN_HOVER_F, COR_TEXTO_BRANCO);
        btn.addActionListener(acao);
        return btn;
    }

    private JButton botaoZero() {
        JButton btn = new JButton("0") {
            @Override
            protected void paintComponent(Graphics g) {
                Graphics2D g2 = (Graphics2D) g.create();
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                Color cor = getModel().isRollover() ? COR_BTN_HOVER_N : COR_BTN_NUMERO;
                g2.setColor(cor);
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), 60, 60);
                g2.setColor(COR_TEXTO_BRANCO);
                g2.setFont(getFont());
                FontMetrics fm = g2.getFontMetrics();
                // Alinha o "0" à esquerda como na calculadora Apple
                int x = 24;
                int y = (getHeight() - fm.getHeight()) / 2 + fm.getAscent();
                g2.drawString(getText(), x, y);
                g2.dispose();
            }
        };
        btn.setFont(new Font("SF Pro Display", Font.PLAIN, 26));
        btn.setForeground(COR_TEXTO_BRANCO);
        btn.setContentAreaFilled(false);
        btn.setBorderPainted(false);
        btn.setFocusPainted(false);
        btn.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        btn.setPreferredSize(new Dimension(71, 71));
        btn.addActionListener(e -> digitarNumero("0"));
        return btn;
    }

    // ─── Lógica da calculadora ───────────────────────────────────────
    private void digitarNumero(String digito) {
        if (digito.equals(".")) {
            if (novaEntrada) {
                displayPrincipal.setText("0,");
                novaEntrada = false;
                return;
            }
            if (!displayPrincipal.getText().contains(",")) {
                displayPrincipal.setText(displayPrincipal.getText() + ",");
            }
            return;
        }

        if (novaEntrada || acabouDeCalcular) {
            displayPrincipal.setText(digito);
            novaEntrada = false;
            acabouDeCalcular = false;
        } else {
            String atual = displayPrincipal.getText();
            if (atual.equals("0")) {
                displayPrincipal.setText(digito);
            } else {
                if (atual.replace(",", "").replace("-", "").length() < 9) {
                    displayPrincipal.setText(atual + digito);
                }
            }
        }
        ajustarFonteDisplay();
    }

    private void definirOperador(String op) {
        numeroAnterior = obterValorDisplay();
        operadorAtual = op;
        novaEntrada = true;
        displayOperacao.setText(formatarNumero(numeroAnterior) + " " + op);
    }

    private void calcular() {
        if (operadorAtual.isEmpty())
            return;

        double b = obterValorDisplay();
        double resultado = 0;
        displayOperacao.setText(formatarNumero(numeroAnterior) + " " + operadorAtual + " " + formatarNumero(b) + " =");

        switch (operadorAtual) {
            case "+":
                resultado = numeroAnterior + b;
                break;
            case "−":
                resultado = numeroAnterior - b;
                break;
            case "×":
                resultado = numeroAnterior * b;
                break;
            case "÷":
                if (b == 0) {
                    displayPrincipal.setText("Erro");
                    operadorAtual = "";
                    return;
                }
                resultado = numeroAnterior / b;
                break;
        }

        operadorAtual = "";
        displayPrincipal.setText(formatarNumero(resultado));
        acabouDeCalcular = true;
        ajustarFonteDisplay();
    }

    private void limparTudo() {
        numeroAnterior = 0;
        operadorAtual = "";
        novaEntrada = true;
        acabouDeCalcular = false;
        displayPrincipal.setText("0");
        displayOperacao.setText(" ");
        displayPrincipal.setFont(new Font("SF Pro Display", Font.PLAIN, 72));
    }

    private void inverterSinal() {
        double val = obterValorDisplay();
        val = -val;
        displayPrincipal.setText(formatarNumero(val));
    }

    private void porcentagem() {
        double val = obterValorDisplay();
        val = val / 100.0;
        displayPrincipal.setText(formatarNumero(val));
    }

    // ─── Utilitários ─────────────────────────────────────────────────
    private double obterValorDisplay() {
        try {
            return Double.parseDouble(displayPrincipal.getText().replace(",", "."));
        } catch (NumberFormatException e) {
            return 0;
        }
    }

    private String formatarNumero(double valor) {
        if (valor == (long) valor && Math.abs(valor) < 1_000_000_000) {
            return String.valueOf((long) valor);
        }
        // Limita casas decimais
        String s = String.format("%.8f", valor).replaceAll("0+$", "").replaceAll("\\.$", "");
        return s.replace(".", ",");
    }

    private void ajustarFonteDisplay() {
        int tamanho = displayPrincipal.getText().length();
        int fonte = tamanho <= 6 ? 72
                : tamanho <= 9 ? 56
                        : tamanho <= 12 ? 44
                                : 34;
        displayPrincipal.setFont(new Font("SF Pro Display", Font.PLAIN, fonte));
    }

    // ─── Classe interna: botão circular ──────────────────────────────
    static class BotaoRedondo extends JButton {
        private final Color corNormal;
        private final Color corHover;

        BotaoRedondo(String texto, Color corNormal, Color corHover, Color corTexto) {
            super(texto);
            this.corNormal = corNormal;
            this.corHover = corHover;
            setFont(new Font("SF Pro Display", Font.PLAIN, 26));
            setForeground(corTexto);
            setContentAreaFilled(false);
            setBorderPainted(false);
            setFocusPainted(false);
            setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
            setPreferredSize(new Dimension(72, 72));
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

            Color cor = getModel().isRollover() ? corHover : corNormal;
            // Sombra sutil
            g2.setColor(new Color(0, 0, 0, 60));
            g2.fillOval(2, 4, getWidth() - 4, getHeight() - 4);
            // Botão
            g2.setColor(cor);
            g2.fillOval(0, 0, getWidth() - 2, getHeight() - 2);

            // Texto centralizado
            g2.setColor(getForeground());
            g2.setFont(getFont());
            FontMetrics fm = g2.getFontMetrics();
            int x = (getWidth() - fm.stringWidth(getText())) / 2 - 1;
            int y = (getHeight() - fm.getHeight()) / 2 + fm.getAscent() - 1;
            g2.drawString(getText(), x, y);
            g2.dispose();
        }
    }

    // ─── Main ─────────────────────────────────────────────────────────
    public static void main(String[] args) {
        // Tenta aplicar look and feel nativo (melhor renderização)
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception ignored) {
        }

        SwingUtilities.invokeLater(() -> {
            calc calc = new calc();
            calc.setVisible(true);
        });
    }
}