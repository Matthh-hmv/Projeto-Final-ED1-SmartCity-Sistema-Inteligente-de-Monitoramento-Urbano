#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h> //Funcao tlower()

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
	int totalPorSeveridade[4]; //Armazenar a quantidade de ocorrencias por severidade por bairro, relatorio 6; ++
	int quantidadeSensores; //Ajuda a fazer o relatorio 5; ++
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
	int especialidade;
	int totalAtendimentos; //Ajuda a fazer o relatorio 6;
	Chamado *listaChamados;
	struct equipe *prox;
}Equipe; //Equipes só podem atender chamados compatíveis com sua especialidade

//Carregar arquivos 
void carregarBairros(Bairro **listaBairros);
void carregarChamados(Bairro *listaBairros, Equipe *listaEquipes);
void carregarEquipes(Equipe **listaEquipes);
void carregarOcorrencias(Bairro **listaBairros);
void carregarSensores(Bairro *listaBairros);

void salvaSistema(Bairro *listaBairros, Equipe *listaEquipes); //melhor que fazer em cada funcao de inserir, usa na main toda vez que chamar uma funcao
void gerarRelatorio(Bairro *listaBairros, Equipe *listaEquipes);
void registraLog(const char *operacao, const char *status, const char *dados);
void executarSimulacao(Bairro **listaBairro, Equipe **listaEquipe);


//Equipe
Equipe *alocaEquipe(int codEquipe, char *nomeEquipe, int espec);
void inserirEquipe(Equipe **listaEquipes, Equipe *novaEquipe, int *flag);
Equipe *buscarEquipe(Equipe *listaEquipes, int codEquipe);
void associarEquipe(Equipe *listaEquipes, int codChamado, int codEquipe, int *flag);
void removeEquipeInicio(Equipe **listaEquipe);

//Chamado
Chamado *alocaChamado(int cod, int priori, int statusChamado, Ocorrencia *OcorrenciaReal);
void inserirChamadoEquipe(Equipe *listaEquipes, int codEquipe, Chamado *novoChamado, int *flag);
Chamado *verificaChamado(Equipe *listaEquipes, int codChamado);
void gerarChamado(Equipe *listaEquipes, Bairro *listaBairros, int codChamado, int codOcorrencia, int priori, int statusSensor, int *flag);
void finalizarChamado(Equipe *listaEquipes, int codChamado, int *flag);
void removeChamadoInicio(Chamado **listaChamado);


//Bairro
Bairro * alocaBairro(int codigo, char nome[]);
void insereBairro(Bairro **listaBairro, int codigo, char nome[], int *flag);
void funListaBairro(Bairro *pauxBairro);
void removeBairroCodigo(Bairro **listaBairro, int codBairro, int *flag);
void removeBairroInicio(Bairro **listaBairro); //Usar para a desolacação de memória
Bairro * verificaBairro(Bairro *pauxBairro, int codBairro);
int verificaBairroNome(Bairro *pauxBairro, char nomeBairro[]);//Usado para nao criar dois bairros com o mesmo nome

//Sensores
Sensor * alocaSensor(int codigo, int tipo,  int status);
void insereSensor(Bairro *pauxBairro, int codBairro, int codSensor, int tipo,  int status,int *flag);
void removeSensorInicio(Bairro *enderecoBairro);
Sensor * verificaSensor (Sensor *pauxSensor, int codSensor);
Sensor* verificaSensorGlobal(Bairro *pauxBairro, int codSensor);
void alteraStatusSensor(Bairro *listaBairros, int codSensor, int novoStatus, int *flag);
void listaSensor(Sensor *enderecoSensor);
void listaSensoresPorBairro(Bairro *listaBairro, int codBairro, int *flag);

//Ocorrências
Ocorrencia * alocaOcorrencia(int codigo, int severidade, char descricao[], int status);
void insereOcorrencia(Bairro **ListaBairro, int codBairro, Ocorrencia **listaOcorrencia, int codigo, int severidade, char descricao[], int status, int *flag);
void removeOcorrenciaInicio(Bairro *enderecoBairro, Ocorrencia **listaOcorrencia);
Ocorrencia * verificaOcorrencia(Ocorrencia *pauxOcorrencia, int codOcorrencia);
Ocorrencia* verificaOcorrenciaGlobal(Bairro *pauxBairro, int codOcorrencia);
void listaOcorrencia(Ocorrencia *enderecoOcorrencia); 
void listaOcorrencias(Bairro *pauxBairro, int *flag); 

void liberarMemoria (Bairro **listaBairro, Equipe **listaEquipe);

#endif
