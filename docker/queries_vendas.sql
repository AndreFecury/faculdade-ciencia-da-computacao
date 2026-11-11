SELECT * FROM vendedores;

SELECT * FROM vendedores ORDER BY nome;

SELECT nome FROM vendedores WHERE nome ILIKE '%na%';

SELECT SUM(valor) AS total_vendas FROM vendas;

SELECT v.nome, SUM(ve.valor) AS total_vendas
FROM vendedores v
JOIN vendas ve ON ve.vendedor_id = v.id
GROUP BY v.nome
ORDER BY total_vendas DESC;
