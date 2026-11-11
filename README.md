<div align="center">

# 🎓 Faculdade · Ciência da Computação

Exercícios, atividades e projetos das disciplinas do curso de **Ciência da Computação (UNIP · Manaus)**.

![Java](https://img.shields.io/badge/Java-ED8B00?style=for-the-badge&logo=openjdk&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![CSS3](https://img.shields.io/badge/CSS3-1572B6?style=for-the-badge&logo=css3&logoColor=white)
![Spring Boot](https://img.shields.io/badge/Spring_Boot-6DB33F?style=for-the-badge&logo=springboot&logoColor=white)
![Docker](https://img.shields.io/badge/Docker-2496ED?style=for-the-badge&logo=docker&logoColor=white)

</div>

## 📂 Estrutura

| Pasta | Conteúdo | Destaques |
|---|---|---|
| [`java/`](java) | POO em Java (aulas 01–05) | classes e objetos, métodos, herança, interfaces gráficas com **Swing** (calculadora, tela de login, sistema de cadastro) |
| [`c/`](c) | Linguagem C (aulas 2–5) | sistema bancário no terminal, CRUD de clientes em arquivo, **cadastro de alunos com interface colorida no terminal** (ANSI true color, navegação por setas) |
| [`python/`](python) | Programação estruturada e listas de revisão | variáveis, operadores, condicionais, laços, funções |
| [`web/`](web) | HTML e CSS | página sobre energias renováveis, layouts com framework CSS |
| [`springboot/`](springboot) | Projeto `demo` em Spring Boot (Java 17, Maven) | estrutura inicial de API |
| [`docker/`](docker) | PostgreSQL + pgAdmin com Docker Compose | `schema`, `seed` e `queries` de vendas |

## ▶️ Como rodar

<details>
<summary><b>Java</b></summary>

```bash
cd java/aula05/a1.java/src
javac *.java
java SistemaCadastro
```
</details>

<details>
<summary><b>C</b></summary>

```bash
cd c/Aula5
gcc ex1.c -o cadastro
./cadastro
```
</details>

<details>
<summary><b>Python</b></summary>

```bash
python python/lista-revisao/lista.py
```
</details>

<details>
<summary><b>Docker (PostgreSQL + pgAdmin)</b></summary>

```bash
cd docker
docker compose up -d
# pgAdmin em http://localhost:5050
```
</details>

## 🔗 Outros projetos

- [BD_POO](https://github.com/AndreFecury/BD_POO) · Banco de Dados + POO em Python
- [AES-python](https://github.com/AndreFecury/AES-python) · criptografia simétrica AES

---

<div align="center">
Feito por <a href="https://github.com/AndreFecury">André Fecury</a> · Manaus, AM
</div>
