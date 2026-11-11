import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Login {

    // Credenciais válidas (poderia vir de um banco de dados)
    private static final String USUARIO_VALIDO = "admin";
    private static final String SENHA_VALIDA = "1234";

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> criarJanela());
    }

    private static void criarJanela() {
        JFrame janela = new JFrame("Sistema Escolar — Login");
        janela.setSize(380, 280);
        janela.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        janela.setLocationRelativeTo(null); // centraliza na tela

        // Painel principal com padding
        JPanel painelPrincipal = new JPanel();
        painelPrincipal.setLayout(new GridBagLayout());
        painelPrincipal.setBorder(BorderFactory.createEmptyBorder(20, 30, 20, 30));
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets = new Insets(6, 0, 6, 0);

        // Título
        JLabel titulo = new JLabel("Entrar no Sistema", SwingConstants.CENTER);
        titulo.setFont(new Font("SansSerif", Font.BOLD, 18));
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 2;
        painelPrincipal.add(titulo, gbc);

        // Campo usuário
        JLabel lblUsuario = new JLabel("Usuário:");
        gbc.gridy = 1;
        gbc.gridwidth = 2;
        painelPrincipal.add(lblUsuario, gbc);

        JTextField campoUsuario = new JTextField();
        campoUsuario.setPreferredSize(new Dimension(280, 32));
        gbc.gridy = 2;
        painelPrincipal.add(campoUsuario, gbc);

        // Campo senha
        JLabel lblSenha = new JLabel("Senha:");
        gbc.gridy = 3;
        painelPrincipal.add(lblSenha, gbc);

        JPasswordField campoSenha = new JPasswordField();
        campoSenha.setPreferredSize(new Dimension(280, 32));
        gbc.gridy = 4;
        painelPrincipal.add(campoSenha, gbc);

        // Botão entrar
        JButton btnEntrar = new JButton("Entrar");
        btnEntrar.setPreferredSize(new Dimension(280, 36));
        gbc.gridy = 5;
        painelPrincipal.add(btnEntrar, gbc);

        janela.add(painelPrincipal);
        janela.setVisible(true);

        // Ação do botão (e tecla Enter no campo senha)
        ActionListener loginAction = e -> {
            String usuario = campoUsuario.getText().trim();
            String senha = new String(campoSenha.getPassword());

            if (usuario.isEmpty() || senha.isEmpty()) {
                JOptionPane.showMessageDialog(janela,
                        "Preencha usuário e senha.",
                        "Atenção", JOptionPane.WARNING_MESSAGE);
                return;
            }

            if (usuario.equals(USUARIO_VALIDO) && senha.equals(SENHA_VALIDA)) {
                JOptionPane.showMessageDialog(janela,
                        "Login efetuado com sucesso!\nBem-vindo, " + usuario + "!",
                        "Sucesso", JOptionPane.INFORMATION_MESSAGE);
                campoSenha.setText("");
            } else {
                JOptionPane.showMessageDialog(janela,
                        "Usuário ou senha incorretos.",
                        "Erro", JOptionPane.ERROR_MESSAGE);
                campoSenha.setText("");
                campoSenha.requestFocus();
            }
        };

        btnEntrar.addActionListener(loginAction);
        campoSenha.addActionListener(loginAction); // Enter no campo senha
    }
}