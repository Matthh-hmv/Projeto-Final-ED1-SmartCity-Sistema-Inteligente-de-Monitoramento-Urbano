#include "prototipos.h"

//CARREGAR ARQUIVSO txt:
void carregarBairros(Bairro **listaBairros)
{
	FILE *arquivoBairros = fopen("entradas/bairros.txt", "r");

	if(!arquivoBairros)
	{
		printf("Erro ao abir entradas/bairros.txt\n");
		return;
	}

	int codBairro;
	char nomeBairro[50];

	while(fscanf(arquivoBairros, "%d %s", &codBairro, nomeBairro) == 2)
		inserirBairro();

	fclose(arquivoBairros);
}


void carregarChamados(Bairro *listaBairros, Equipe *listaEquipes)
{
	FILE *arquivoChamados = fopen("entradas/chamados.txt", "r");

	if(!arquivoChamados)
	{
		printf("Erro ao abir entradas/chamados.txt\n");
		return;
	}

	int codChamado, codOcorrencia, codEquipe, prioridade, statusSensor;

	while(fscanf(arquivoChamados, "%d %d %d %d %d", &codChamado, &codOcorrencia, &codEquipe, &prioridade, &statusSensor) == 5)
		inserirChamadoEquipe(listaEquipes, codEquipe, codChamado, prioridade, statusSensor, NULL);

	fclose(arquivoChamados);
}


void carregarEquipes(Equipe **listaEquipes)
{
	FILE *arquivoEquipes = fopen("entradas/equipes.txt", "r");

	if(!arquivoEquipes)
	{
		printf("Erro ao abir entradas/equipes.txt\n");
		return;
	}

	int cod, especialidade;
	char nomeEquipe[50];


	while(fscanf(arquivoEquipes, "%d %s %d", &cod, nomeEquipe, &especialidade) == 3)
		inserirEquipe(listaEquipes, cod, nomeEquipe, especialidade);

	fclose(arquivoEquipes);
}


void carregarOcorrencias(Ocorrencia **listaOcorrencia)
{
	FILE *arquivoOcorrencias = fopen("entradas/ocorrencias.txt", "r");

	if(!arquivoOcorrencias)
	{
		printf("Erro ao abir entradas/ocorrencias.txt\n");
		return;
	}

	int codOcorrencia, sev, statusSensor, codSensor, bairro;
	char desc[100];

	while(fscanf(arquivoOcorrencias, "%d %d %d %d %d %s", &codOcorrencia, &sev, &statusSensor, &codSensor, &bairro, desc) == 6)
		inserirOcorrencia();

	fclose(arquivoOcorrencias);
}


void carregarSensores(Bairro *listaBairros)
{
	FILE *arquivoSensores = fopen("entradas/sensores.txt", "r");

	if(!arquivoSensores)
	{
		printf("Erro ao abir entradas/sensores.txt\n");
		return;
	}

	int codSensor, tipoSensor, statusSensor, codBairro;

	while(fscanf(arquivoSensores, "%d %d %d %d", &codSensor, &tipoSensor, &statusSensor, &codBairro) == 4)
		inserirSensores();

	fclose(arquivoSensores);
}


// GERENCIAR EQUIPES:
void inserirEquipe(Equipe **listaEquipes, int codEquipe, char *nomeEquipe, int espec)
{
	Equipe *novo = NULL;
	novo = (Equipe *)malloc(sizeof(Equipe));

	if(novo == NULL)
	{
		printf("erro de alocacao!\n");
		return;
	}

	if(novo)
	{
		novo->codigo = codEquipe;
		strcpy(novo->nome, nomeEquipe);
		novo->especialidade = espec;
		novo->totalAtendimentos = 0;
		novo->listaChamados = NULL;
		novo->prox = NULL;

		if(*listaEquipes == NULL)
			*listaEquipes = novo;

		else
		{
			Equipe *paux = *listaEquipes;

			while(paux->prox != NULL)
				paux = paux->prox;

			paux->prox = novo;
		}
	}
}

Equipe *buscarEquipe(Equipe *listaEquipes, int codEquipe)
{
	while(listaEquipes != NULL)
	{
		if(listaEquipes->codigo == codEquipe)
			return listaEquipes;

		listaEquipes = listaEquipes->prox;
	}
	return NULL;
}
