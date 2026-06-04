#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

typedef struct ocorrencia{
	int codigo;
	int severidade;
	char descricao[100];
	int status;
	int totalPorSeveridade[3]; //Armazenar a quantidade de ocorrencias por severidade, relatorio 6 (Sera se a simone deixa); ++
	struct ocorrencia *prox;
}Ocorrencia;

typedef struct sensor{
	int codigo;
	int tipo;
	int status;
	Ocorrencia *listaOcorrencias;
	struct sensor *prox;
}Sensor;

typedef struct bairro{
	int codigo;
	char nome[50];
	int quantidadeOcorrencias; //Ajuda a fazer o relatorio 1; ++
	int quantidadeSensores; //Ajuda a fazer o relatorio 5; ++
	Sensor *listaSensores;
	struct bairro *prox;
}Bairro;

typedef struct chamado{
	int codigo;
	int prioridade;
	int status;
	Ocorrencia *ocorrencia; //verifique se vai precisar
	struct chamado *prox;
}Chamado;

typedef struct equipe{
	int codigo;
	char nome[50];
	char especialidade[30];
	int totalAtendimentos; //Ajuda a fazer o relatorio 6;
	Chamado *listaChamados;
	struct equipe *prox;
}Equipe; //Equipes só podem atender chamados compatíveis com sua especialidade
