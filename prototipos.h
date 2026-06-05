#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h> //tolower()

#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

typedef struct chamado Chamado; //tem que declarar aqui no começo, porque a struct ocorrencia chama a struct chamado mas ela ainda "nao existe" porque foi criada depois

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
	int totalPorSeveridade[3]; //Armazenar a quantidade de ocorrencias por severidade por bairro, relatorio 6; ++
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
void inserirChamadoEquipe(Equipe *listaEquipes, int codEquipe, int codChamado, int priori, int statusSensor, Ocorrencia *OcorrenciaReal);
void associarEquipe(Equipe *listaEquipes, int codChamado, int codEquipe);

//Chamado
void gerarChamado(Equipe *listaEquipes, Bairro *listaBairros, int codChamado, int codOcorrencia, int priori, int statusSensor);
void finalizarChamado(Equipe *listaEquipes, int codChamado);

//Bairro
Bairro * alocaBairro(int codigo, char nome[]);
void insereBairro(Bairro **listaBairro, int codigo, char nome[]);
void buscaBairro(Bairro *pauxBairro, int codBairro);
void listaBairro(Bairro *pauxBairro);
void removeBairroCodigo(Bairro **listaBairro, int codBairro);//apagar os sensores (nao fiz o comando do sensor ainda)
void removeBairroInicio(Bairro **listaBairro); //Terminar
Bairro *verificaBairro(Bairro *pauxBairro, int codBairro);

//Sensores
Sensor * alocaSensor(int codigo, int tipo,  int status);
void insereSensor(Bairro *pauxBairro, int codBairro, int codSensor, int tipo,  int status);
//void removeSensorCodigo(Bairro *enderecoBairro, int codSensor);//terminar

//Ocorrências
Ocorrencia * alocaOcorrencia(int codigo, int severidade, char descricao[], int status);
void insereOcorrencia(Bairro *enderecoBairro, Ocorrencia **listaOcorrencia, int codigo, int severidade, char descricao[], int status);
void removeOcorrenciaCodigo(Bairro *enderecoBairro, Ocorrencia **listaOcorrencia, int codOcorrencia);
void removeOcorrenciaInicio(Bairro *enderecoBairro, Ocorrencia **listaOcorrencia);
Ocorrencia * verificaOcorrencia(Ocorrencia *pauxOcorrencia, int codOcorrencia);

#endif
