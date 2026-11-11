import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

public class SistemaCadastro {

    // Modelo de dados
    static class Aluno {
        String nome;
        int    idade;
        String curso;

        Aluno(String nome, int idade, String curso) {
            this.nome  = nome;
            this.idade = idade;
            this.curso = curso;
        }
    }

    private static ArrayList<Aluno> alunos = new ArrayList<>();
    private static DefaultTableModel tableModel;

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> criarJanela());
    }

    private static void criarJanela() {
        JFrame janela = new JFrame("Sistema de Cadastro");
        janela.setSize(560, 520);
        janela.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        janela.setLocationRelativeTo(null);

        JPanel painel = new JPanel(new BorderLayout(10, 10));
        painel.setBorder(BorderFactory.createEmptyBorder(16, 20, 16, 20));

        // ─── Formulário ───────────────────────────────────────────────
        JPanel formPanel = new JPanel(new GridBagLayout());
        formPanel.setBorder(BorderFactory.createTitledBorder("Dados do Aluno"));
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(6, 8, 6, 8);
        gbc.fill   = GridBagConstraints.HORIZONTAL;

        // Nome
        gbc.gridx = 0; gbc.gridy = 0; gbc.weightx = 0;
        formPanel.add(new JLabel("Nome completo:"), gbc);
        JTextField campoNome = new JTextField(20);
        gbc.gridx = 1; gbc.weightx = 1;
        formPanel.add(campoNome, gbc);

        // Idade
        gbc.gridx = 0; gbc.gridy = 1; gbc.weightx = 0;
        formPanel.add(new JLabel("Idade:"), gbc);
        JSpinner campoIdade = new JSpinner(new SpinnerNumberModel(18, 1, 120, 1));
        gbc.gridx = 1; gbc.weightx = 1;
        formPanel.add(campoIdade, gbc);

        // Curso
        gbc.gridx = 0; gbc.gridy = 2; gbc.weightx = 0;
        formPanel.add(new JLabel("Curso:"), gbc);
        String[] cursos = {"Informática", "Administração", "Enfermagem", "Contabilidade"};
        JComboBox<String> campoCurso = new JComboBox<>(cursos);
        gbc.gridx = 1; gbc.weightx = 1;
        formPanel.add(campoCurso, gbc);

        // Botões
        JPanel botoesPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 8, 0));
        JButton btnLimpar = new JButton("Limpar");
        JButton btnSalvar = new JButton("Salvar cadastro");
        btnSalvar.setBackground(new Color(74, 63, 207));
        btnSalvar.setForeground(Color.WHITE);
        btnSalvar.setFocusPainted(false);
        botoesPanel.add(btnLimpar);
        botoesPanel.add(btnSalvar);

        gbc.gridx = 0; gbc.gridy = 3; gbc.gridwidth = 2; gbc.weightx = 1;
        formPanel.add(botoesPanel, gbc);

        painel.add(formPanel, BorderLayout.NORTH);

        // ─── Tabela de registros ──────────────────────────────────────
        String[] colunas = {"#", "Nome", "Idade", "Curso"};
        tableModel = new DefaultTableModel(colunas, 0) {
            @Override public boolean isCellEditable(int r, int c) { return false; }
        };

        JTable tabela = new JTable(tableModel);
        tabela.setRowHeight(28);
        tabela.setFont(new Font("SansSerif", Font.PLAIN, 13));
        tabela.getTableHeader().setFont(new Font("SansSerif", Font.BOLD, 13));
        tabela.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        // Larguras das colunas
        tabela.getColumnModel().getColumn(0).setPreferredWidth(30);
        tabela.getColumnModel().getColumn(1).setPreferredWidth(180);
        tabela.getColumnModel().getColumn(2).setPreferredWidth(60);
        tabela.getColumnModel().getColumn(3).setPreferredWidth(120);

        JScrollPane scroll = new JScrollPane(tabela);
        scroll.setBorder(BorderFactory.createTitledBorder("Alunos cadastrados"));
        painel.add(scroll, BorderLayout.CENTER);

        // ─── Rodapé com total ─────────────────────────────────────────
        JLabel labelTotal = new JLabel("Total: 0 aluno(s)");
        labelTotal.setFont(new Font("SansSerif", Font.ITALIC, 12));
        labelTotal.setForeground(Color.GRAY);
        painel.add(labelTotal, BorderLayout.SOUTH);

        janela.add(painel);
        janela.setVisible(true);

        // ─── Ações ────────────────────────────────────────────────────
        btnSalvar.addActionListener(e -> {
            String nome  = campoNome.getText().trim();
            int    idade = (Integer) campoIdade.getValue();
            String curso = (String) campoCurso.getSelectedItem();

            if (nome.isEmpty()) {
                JOptionPane.showMessageDialog(janela,
                    "Informe o nome do aluno.",
                    "Atenção", JOptionPane.WARNING_MESSAGE);
                campoNome.requestFocus();
                return;
            }
            if (nome.length() < 3) {
                JOptionPane.showMessageDialog(janela,
                    "Nome deve ter pelo menos 3 caracteres.",
                    "Atenção", JOptionPane.WARNING_MESSAGE);
                return;
            }

            Aluno aluno = new Aluno(nome, idade, curso);
            alunos.add(aluno);

            // Adiciona linha na tabela
            tableModel.addRow(new Object[]{alunos.size(), nome, idade, curso});
            labelTotal.setText("Total: " + alunos.size() + " aluno(s)");

            // Exibe JOptionPane com resumo (conforme o desafio pede)
            JOptionPane.showMessageDialog(janela,
                "Cadastro realizado com sucesso!\n\n" +
                "Nome:   " + nome  + "\n" +
                "Idade:  " + idade + " anos\n" +
                "Curso:  " + curso,
                "Dados salvos", JOptionPane.INFORMATION_MESSAGE);

            // Log no console
            System.out.println("[CADASTRO] " + nome + " | " + idade + " anos | " + curso);

            // Limpa formulário
            campoNome.setText("");
            campoIdade.setValue(18);
            campoCurso.setSelectedIndex(0);
            campoNome.requestFocus();
        });

        btnLimpar.addActionListener(e -> {
            campoNome.setText("");
            campoIdade.setValue(18);
            campoCurso.setSelectedIndex(0);
            campoNome.requestFocus();
        });
    }
}