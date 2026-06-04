#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct ocorrencia{
	int codigo;
	int severidade;
	char descricao[100];
	int status;
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
	int total_atendimentos;
	Chamado *listaChamados;
	struct equipe *prox;
}Equipe; //Equipes só podem atender chamados compatíveis com sua especialidade