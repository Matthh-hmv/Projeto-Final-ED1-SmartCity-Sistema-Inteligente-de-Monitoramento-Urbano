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
	Chamado *chamado; //Aponta para seu chamado correspondente, se ele tiver um
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
	int totalPorSeveridade[3]; //Armazenar a quantidade de ocorrencias por severidade, relatorio 6 (Sera se a simone deixa); ++
	int quantidaSensores; //Ajuda a fazer o relatorio 5; ++
	Sensor *listaSensores;
	struct bairro *prox;
}Bairro;

typedef struct chamado{
	int codigo;
	int prioridade;
	int status;
	Ocorrencia *ocorrencia; //Aponta para sua ocorrencia correspondente
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

//Carregar arquivos 
void carregarBairros(Bairro **listaBairros);
void carregarChamados(Bairro *listaBairros, Equipe *listaEquipes);
void carregarEquipes(Equipe **listaEquipes);
void carregarOcorrencias(Bairro *listaBairros);
void carregarSensores(Bairro *listaBairros);



//Equipe
void inserirEquipe(Equipe **listaEquipes, int codEquipe, char *nomeEquipe, int espec);
Equipe *buscarEquipe(Equipe *listaEquipes, int codEquipe);

#endif
