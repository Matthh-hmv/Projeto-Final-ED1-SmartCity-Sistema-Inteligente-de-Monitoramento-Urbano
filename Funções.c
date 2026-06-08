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
	int flagAux;

	while(fscanf(arquivoBairros, "%d %s", &codBairro, nomeBairro) == 2)
		insereBairro(listaBairros, codBairro, nomeBairro, &flagAux);

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

	int codChamado, codOcorrencia, codEquipe, priori, statusSensor;

	while(fscanf(arquivoChamados, "%d %d %d %d %d", &codChamado, &codOcorrencia, &codEquipe, &priori, &statusSensor) == 5)
	{
		Ocorrencia *OcorrenciaReal = NULL;
		Bairro *pauxBairro = listaBairros;

		while(pauxBairro != NULL && OcorrenciaReal == NULL)
		{
			Sensor *pauxSensor = pauxBairro->listaSensores;

			while(pauxSensor != NULL && OcorrenciaReal == NULL)
			{
				OcorrenciaReal = verificaOcorrencia(pauxSensor->listaOcorrencias, codOcorrencia);
				pauxSensor = pauxSensor->prox;
			}

			pauxBairro = pauxBairro->prox;
		}
		Chamado *novoChamado = alocaChamado(codChamado, priori, statusSensor, OcorrenciaReal);
		inserirChamadoEquipe(listaEquipes, codEquipe, novoChamado);
	}

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

	int cod, espec;
	char nomeEquipe[50];


	while(fscanf(arquivoEquipes, "%d %s %d", &cod, nomeEquipe, &espec) == 3)
	{
		Equipe *novo = alocaEquipe(cod, nomeEquipe, espec);
		inserirEquipe(listaEquipes, novo);
	}

	fclose(arquivoEquipes);
}


void carregarOcorrencias(Bairro *listaBairros)
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
	{
		Bairro *bairroDestino = verificaBairro(listaBairros, bairro);

		if(bairroDestino != NULL)
		{
			Sensor *pauxSensor = verificaSensor(bairroDestino->listaSensores, codSensor); //busca o sensor no bairro informado

			if(pauxSensor != NULL)  
				insereOcorrencia(bairroDestino, &(pauxSensor->listaOcorrencias), codOcorrencia, sev, desc, statusSensor);



			if(pauxSensor == NULL)  // nao achou no bairro informado, procura em outro bairro
			{
				Bairro *pauxBairro = listaBairros;

				while(pauxBairro != NULL && pauxSensor == NULL)
				{
					pauxSensor = verificaSensor(pauxBairro->listaSensores, codSensor);

					if(pauxSensor != NULL)  // achou em outro bairro, atualiza o bairro de destino para o certo
						bairroDestino = pauxBairro;

					pauxBairro = pauxBairro->prox;
				}
			}
		}
	}

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

	int codSensor, tipoSensor, statusSensor, codBairro, flagAux;

	while(fscanf(arquivoSensores, "%d %d %d %d", &codSensor, &tipoSensor, &statusSensor, &codBairro) == 4)
	{
		Bairro *bairroDestino = verificaBairro(listaBairros, codBairro);

		if(bairroDestino != NULL)
			insereSensor(listaBairros, codBairro, codSensor, tipoSensor, statusSensor, &flagAux);
	}

	fclose(arquivoSensores);
}


// GERENCIAR EQUIPES:
Equipe *alocaEquipe(int codEquipe, char *nomeEquipe, int espec)
{
	Equipe *novo = NULL;
	novo = (Equipe *)malloc(sizeof(Equipe));

	if(novo == NULL)
	{
		printf("erro de alocacao!\n");
		return NULL;
	}

	if(novo)
	{
		novo->codigo = codEquipe;
		strcpy(novo->nome, nomeEquipe);
		novo->especialidade = espec;
		novo->totalAtendimentos = 0;
		novo->listaChamados = NULL;
		novo->prox = NULL;
	}
	return novo;
}

void inserirEquipe(Equipe **listaEquipes, Equipe *novaEquipe, int *flag)
{

	if(novaEquipe == NULL)
	{
		*flag = 0;
		return;
	}

	if(*listaEquipes == NULL)
		*listaEquipes = novaEquipe;

	else
	{
		Equipe *paux = *listaEquipes;

		while(paux->prox != NULL)
			paux = paux->prox;

		paux->prox = novaEquipe;
	}

	*flag = 1;
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


void associarEquipe(Equipe *listaEquipes, int codChamado, int codEquipe, int *flag)
{
	Equipe *equipeDestino = buscarEquipe(listaEquipes, codEquipe);

	if(equipeDestino == NULL)
	{
		printf("equipe nao encontrada!\n");
		*flag = 0;
		return;
	}

	Equipe *equipeAtual = listaEquipes;
	Chamado *chamadoAlvo = NULL;
	Chamado *anteriorChamado = NULL;
	Equipe *equipeAntiga = NULL;

	while(equipeAtual != NULL && chamadoAlvo == NULL)
	{
		Chamado *pauxChamado = equipeAtual->listaChamados;
		Chamado *pauxAnterior = NULL;

		while(pauxChamado != NULL)
		{
			if(pauxChamado->codigo == codChamado)
			{
				chamadoAlvo = pauxChamado;
				anteriorChamado = pauxAnterior;
				equipeAntiga = equipeAtual;
				break;
			}

			pauxAnterior = pauxChamado;
			pauxChamado = pauxChamado->prox;
		}

		equipeAtual = equipeAtual->prox;
	}

	if(chamadoAlvo == NULL)
	{
		printf("Chamado %d nao encontrado no sistema. \n", codChamado);
		*flag = 0;
		return;
	}

	if(equipeDestino->especialidade != equipeAntiga->especialidade)
	{
		printf("A equipe %s nao pode atender ao chamado %d. Especialidade incompativel!", equipeDestino->nome, codChamado);
		*flag = 0;
		return;
	}

	if(anteriorChamado == NULL)
		equipeAntiga->listaChamados = chamadoAlvo->prox;

	else
		anteriorChamado->prox = chamadoAlvo->prox;


	chamadoAlvo->prox = NULL;
	int flagInterna;

	inserirChamadoEquipe(listaEquipes, equipeDestino->codigo, chamadoAlvo, &flagInterna);

	printf("Chamado %d associado com sucesso a equipe %s \n", codChamado, equipeDestino->nome);
	*flag = 1;
}

void removeEquipeInicio(Equipe ** listaEquipe)
{
	if (*listaEquipe == NULL)
	{
		return;
	}
	Equipe *pauxEquipe = *listaEquipe;
	*listaEquipe = pauxEquipe -> prox;
	while (pauxEquipe -> listaChamados)
		removeChamadoInicio(&(pauxEquipe->listaChamados));
	free(pauxEquipe);
	pauxEquipe = NULL;
	return;		
}


Chamado *alocaChamado(int cod, int priori, int statusChamado, Ocorrencia *OcorrenciaReal)
{
	Chamado *novo = (Chamado *)malloc(sizeof(Chamado));
	if(novo == NULL)
	{
		printf("Erro de alocacao \n");
		return NULL;
	}

	novo->codigo = cod;
	novo->prioridade = priori;
	novo->status = statusChamado;
	novo->ocorrencia = OcorrenciaReal;
	novo->prox = NULL;

	return novo;
}


void inserirChamadoEquipe(Equipe *listaEquipes, int codEquipe, Chamado *novoChamado, int *flag)
{
	if(novoChamado == NULL)
	{
		*flag = 0;
		return;
	}

	Equipe *eqp = buscarEquipe(listaEquipes, codEquipe);

	if(eqp == NULL)
	{
		printf("equipe nao encntrada!\n");
		free(novoChamado);
		*flag = 0;
		return;
	}


	if(eqp->listaChamados == NULL)
		eqp->listaChamados = novoChamado;

	else
	{
		Chamado *paux = eqp->listaChamados;

		while(paux->prox != NULL)
			paux = paux->prox;

		paux->prox = novoChamado;
	}

	*flag = 1;
}




Chamado *verificaChamado(Equipe *listaEquipes, int codChamado)
{
	Equipe *equipeAtual = listaEquipes;

	while(equipeAtual != NULL)
	{
		Chamado *pauxChamado = equipeAtual->listaChamados;

		while(pauxChamado != NULL)
		{
			if(pauxChamado->codigo == codChamado)
				return pauxChamado;  //enconrou chamado na memoria

			pauxChamado = pauxChamado->prox;
		}
		equipeAtual = equipeAtual->prox;
	}
	return NULL; // nao existe no sistema
}


void gerarChamado(Equipe *listaEquipes, Bairro *listaBairros, int codChamado, int codOcorrencia, int priori, int statusSensor, int *flag)
{
	if(verificaChamado(listaEquipes, codChamado) != NULL)
	{
		printf("O chamado %d ja esta cadastrado no sistema. \n", codChamado);
		*flag = 0;
		return;
	}

	Ocorrencia *OcorrenciaReal = NULL;	
	int tipoSensor = -1;

	Bairro *pauxBairro = listaBairros;


	while(pauxBairro != NULL && OcorrenciaReal == NULL)
	{
		Sensor *pauxSensor = pauxBairro->listaSensores;

		while(pauxSensor != NULL && OcorrenciaReal == NULL)
		{
			OcorrenciaReal = verificaOcorrencia(pauxSensor->listaOcorrencias, codOcorrencia);
			
			if(OcorrenciaReal != NULL)
			{
				tipoSensor = pauxSensor->tipo;
				break;
			}

			pauxSensor = pauxSensor->prox;
		}
		pauxBairro = pauxBairro->prox;
	}


	if(OcorrenciaReal == NULL)
	{
		printf("Ocorrencia %d nao encontrada. Chamado %d cancelado \n", codOcorrencia, codChamado);
		*flag = 0;
		return;
	}

	Equipe *equipeCompativel = listaEquipes;

	while(equipeCompativel != NULL)
	{
		if(equipeCompativel->especialidade == tipoSensor)
			break;

		equipeCompativel = equipeCompativel->prox;
	}

	if(equipeCompativel == NULL)
	{
		printf("Nenhuma equipe com especialidade %d cadastrada para atender o chamado %d\n", tipoSensor, codChamado);
		*flag = 0;
		return;
	}

	Chamado *novoChamado = alocaChamado(codChamado, priori, statusSensor, OcorrenciaReal);
	inserirChamadoEquipe(listaEquipes, equipeCompativel->codigo, novoChamado, &flagInterna);

	OcorrenciaReal->chamado = novoChamado;

	printf("Chamado %d gerado automaticamente e atribuido a equipe %s \n", codChamado, equipeCompativel->nome);
	*flag = 1;
}


void finalizarChamado(Equipe *listaEquipes, int codChamado, int *flag)
{
	Equipe *equipeAtual = listaEquipes;
	Chamado *chamadoAlvo = NULL;
	Equipe *equipeResponsavel = NULL;

	while(equipeAtual != NULL && chamadoAlvo == NULL)
	{
		Chamado *pauxChamado = equipeAtual->listaChamados;

		while(pauxChamado != NULL)
		{
			if(pauxChamado->codigo == codChamado)
			{
				chamadoAlvo = pauxChamado;
				equipeResponsavel = equipeAtual;
				break;
			}
			pauxChamado = pauxChamado->prox;
		}
		equipeAtual = equipeAtual->prox;
	}

	if(chamadoAlvo == NULL)
	{
		printf("Chamado %d nao encontrado no sistema \n", codChamado);
		*flag = 0;
		return;
	}

	chamadoAlvo->status = 3; // 3 = chamado finalizado
	equipeResponsavel->totalAtendimentos++;

	if(chamadoAlvo->ocorrencia != NULL)
		chamadoAlvo->ocorrencia->status = 0; //ocorrencia resovlida

	printf("Chamado %d finalizado com sucesso! \n", codChamado);
	*flag = 1;
}

void removeChamadoInicio(Chamado **listaChamado)
{
	if (*listaChamado == NULL)
	{
		return;
	}
	Chamado *pauxChamado = *listaChamado;
	*listaChamado = pauxChamado -> prox;
	Ocorrencia *pauxOcorrencia = pauxChamado -> ocorrencia;
	if (pauxOcorrencia -> chamado)
		pauxOcorrencia -> chamado = NULL
	free(pauxChamado);
	return;	
}

void salvaSistema(Bairro *listaBairros, Equipe *listaEquipes)
{
	//ATUALIZAR ARQUIVO txt BAIRROS
	FILE *arquivoBairros = fopen("entradas/bairros.txt", "w");
	
	if(arquivoBairros)
	{
		Bairro *pauxB = listaBairros;

		while(pauxB != NULL)
		{
			fprintf(arquivoBairros, "%d %s\n", pauxB->codigo, pauxB->nome);
			pauxB = pauxB->prox;
		}
		fclose(arquivoBairros);
	}
	else
		printf("Erro ao atualizar entradas/bairros.txt\n");


	//ATUALIZAR ARQUIVO txt EQUIPES
	FILE *arquivoEquipes = fopen("entradas/equipes.txt", "w");
	
	if(arquivoEquipes)
	{
		Equipe *pauxE = listaEquipes;

		while(pauxE != NULL)
		{
			fprintf(arquivoEquipes, "%d %s %d\n", pauxE->codigo, pauxE->nome, pauxE->especialidade);
			pauxE = pauxE->prox;
		}
		fclose(arquivoEquipes);
	}
	else
		printf("Erro ao atualizar entradas/equipes.txt\n");


	//ATUALIZAR ARQUIVO txt SENSORES E OCORRENCIAS
	FILE *arquivoSensores = fopen("entradas/sensores.txt", "w");
	FILE *arquivoOcorrencias = fopen("entradas/ocorrencias.txt", "w");

	if(arquivoSensores && arquivoOcorrencias)
	{
		Bairro *pauxBairroAtual = listaBairros;

		while(pauxBairroAtual != NULL)
		{
			Sensor *pauxSensorAtual = pauxBairroAtual->listaSensores;

			while(pauxSensorAtual != NULL)
			{
				fprintf(arquivoSensores, "%d %d %d %d\n", pauxSensorAtual->codigo, pauxSensorAtual->tipo, pauxSensorAtual->status, pauxBairroAtual->codigo);

				Ocorrencia *pauxOcorrenciaAtual = pauxSensorAtual->listaOcorrencias;

				while(pauxOcorrenciaAtual != NULL)
				{
					fprintf(arquivoOcorrencias, "%d %d %d %d %d %s\n", pauxOcorrenciaAtual->codigo, pauxOcorrenciaAtual->severidade, pauxOcorrenciaAtual->status, pauxSensorAtual->codigo, pauxBairroAtual->codigo, pauxOcorrenciaAtual->descricao);

					pauxOcorrenciaAtual = pauxOcorrenciaAtual->prox;
				}
				pauxSensorAtual = pauxSensorAtual->prox;
			}
			pauxBairroAtual = pauxBairroAtual->prox;
		}

		fclose(arquivoSensores);
		fclose(arquivoOcorrencias);
	}

	else
	{
		printf("Erro ao abrir sensores ou ocorrencias para salvar\n");

		if(arquivoSensores)
			fclose(arquivoSensores);

		if(arquivoOcorrencias)
			fclose(arquivoOcorrencias);
	}


	//ATUALIZAR ARQUIVO txt CHAMADOS
	FILE *arquivoChamados = fopen("entradas/chamados.txt", "w");
	
	if(arquivoChamados)
	{
		Equipe *pauxEquipeAtual = listaEquipes;

		while(pauxEquipeAtual != NULL)
		{
			Chamado *pauxChamadoAtual = pauxEquipeAtual->listaChamados;

			while(pauxChamadoAtual != NULL)
			{
				int idOcorrencia;

				if(pauxChamadoAtual->ocorrencia != NULL)
					idOcorrencia = pauxChamadoAtual->ocorrencia->codigo;

				else
					idOcorrencia = -1;

				fprintf(arquivoChamados, "%d %d %d %d %d\n", pauxChamadoAtual->codigo, idOcorrencia, pauxEquipeAtual->codigo, pauxChamadoAtual->prioridade, pauxChamadoAtual->status);
				
				pauxChamadoAtual = pauxChamadoAtual->prox;
			} 

			pauxEquipeAtual = pauxEquipeAtual->prox;
		}
		fclose(arquivoChamados);
	}

	else
		printf("Erro ao atualizar entradas/chamados.txt\n");
}

void gerarRelatorio(Bairro *listaBairros, Equipe *listaEquipes)
{
	FILE *relatorio = fopen("saidas/relatorio_final.txt", "w");
	if(!relatorio)
	{
		printf("Erro ao criar saidas/relatorio_final.txt\n");
		return;
	}

	fprintf(relatorio, "SMARTCITY - RELATORIO FINAL \n");

	//relatorio 1
	fprintf(relatorio, "RELATORIO 1: BAIRROS COM MAIOR QUANTIDADE DE OCORRENCIAS \n");

	Bairro *pauxB = listaBairros;

	if(pauxB == NULL)
		fprintf(relatorio, "Nenhum bairro cadastrado.\n");

	else
	{
		while(pauxB != NULL)
		{
			fprintf(relatorio, "Bairro: %s (cod: %d) | Total de ocorrências: %d\n", pauxB->nome, pauxB->codigo, pauxB->quantidadeOcorrencias);
			pauxB = pauxB->prox;
		}
	}
	fprintf(relatorio, "\n");


	//relatorio 2
	fprintf(relatorio, "RELATORIO 2: SENSORES OFFLINES \n");
	pauxB = listaBairros;
	int encontrouOff = 0;

	while(pauxB != NULL)
	{
		Sensor *pauxS = pauxB->listaSensores;

		while(pauxS != NULL)
		{
			if(pauxS->status == 3)
			{
				fprintf(relatorio, "Sensor ID: %d | Tipo: %d no Bairro: %d\n", pauxS->codigo, pauxS->tipo, pauxB->nome);
				encontrouOff = 1;
			}
			pauxS = pauxS->prox;
		}
		pauxB = pauxB->prox;
	}

	if(!encontrouOff)
		fprintf(relatorio, "Nenhum sensor offline no momento.\n");

	fprintf(relatorio, "\n");


	//realtorio 3
	fprintf(relatorio, "RELATORIO 3: OCORRÊNCIAS CRÍTICAS ABERTAS \n");
	pauxB = listaBairros;
	int encontrouCritica = 0;

	while(pauxB != NULL)
	{
		Sensor *pauxS = pauxB->listaSensores;

		while(pauxS != NULL)
		{
			Ocorrencia *pauxO = pauxS->listaOcorrencias;

			while(pauxO != NULL)
			{
				if(pauxO->severidade == 4 && pauxO->status == 1)
				{
					fprintf(relatorio, "Ocorrencia ID: %d | Descrição: %s\n", pauxO->codigo, pauxO->descricao);
					encontrouCritica = 1;
				}
				pauxO = pauxO->prox;
			}
			pauxS = pauxS->prox;
		}
		pauxB = pauxB->prox;
	}

	if(!encontrouCritica)
		fprintf(relatorio, "Nenhuma ocorrencia critica aberta detectada.\n");

	fprintf(relatorio, "\n");


	//relatorio 4
	fprintf(relatorio, "RELATÓRIO 4: EQUIPE COM MAIS NÚMERO DE ATENDIMENTOS \n");
	Equipe *pauxE = listaEquipes;
	Equipe *maisAtendimentos = listaEquipes;

	if(pauxE == NULL)
		fprintf(relatorio, "Nenhuma equipe cadastrada.\n");

	else
	{
		while(pauxE != NULL)
		{
			if(pauxE->totalAtendimentos > maisAtendimentos->totalAtendimentos)
				maisAtendimentos = pauxE;

			pauxE = pauxE->prox;
		}

		if(maisAtendimentos != NULL && maisAtendimentos->totalAtendimentos > 0)
			fprintf(relatorio, "%s com %d atendimentos.\n", maisAtendimentos->nome, maisAtendimentos->totalAtendimentos);

		else
			fprintf(relatorio, "Nenhuma equipe realizou atendimentos ainda.\n");
	}

	fprintf(relatorio, "\n");

	//relatorio 5
	fprintf(relatorio, "RELATÓRIO 5: QUANTIDADE DE SENSORES POR BAIRRO \n");
	pauxB = listaBairros;

	if(pauxB == NULL)
		fprintf(relatorio, "Nenhum bairro monitorado.\n");

	else
	{
		while(pauxB != NULL)
		{
			fprintf(relatorio, "Bairro: %s | Quantidade de sensores: %d", pauxB->nome, pauxB->quantidadeSensores);
			pauxB = pauxB->prox;
		}
	}

	fprintf(relatorio, "\n");

	//relatorio 6
	fprintf(relatorio, "RELATÓRIO 6: QUANTIDADE DE OCORRÊNCIAS POR SEVERIDADE \n");

	int total[4] = {0, 0, 0, 0}; //0 = baixa, 1 = media, 2 = alta, 3 = critivca
	pauxB = listaBairros;

	while(pauxB != NULL)
	{
		total[0] += pauxB->totalPorSeveridade[0];
		total[1] += pauxB->totalPorSeveridade[1];
		total[2] += pauxB->totalPorSeveridade[2];
		total[3] += pauxB->totalPorSeveridade[3];

		pauxB = pauxB->prox;
	}

	fprintf(relatorio, "Severidade 1: %d\n", total[0]);
	fprintf(relatorio, "Severidade 2: %d\n", total[1]);
	fprintf(relatorio, "Severidade 3: %d\n", total[2]);
	fprintf(relatorio, "Severidade 4: %d\n", total[3]);

	fprintf(relatorio, "\n");

	//relatorio geral
	fprintf(relatorio, "RELATORIO GERAL \n");

	//sensores ativos e offline
	pauxB = listaBairros;
	int temSensor = 0;

	while(pauxB != NULL)
	{
		Sensor *pauxS = pauxB->listaSensores;

		while(pauxS != NULL)
		{
			char statusS[20];

			if(pauxS->status == 1)
				strcpy(statusS, "ATIVO");

			else if(pauxS->status == 2)
				strcpy(statusS, "MANUTENCAO");

			else if(pauxS->status == 3)
				strcpy(statusS, "OFFLINE");

			else
				strcpy(statusS, "DESCONHENCIDO");

			fprintf(relatorio, "Sensor ID: %d | Tipo: %d | Bairro: %s | Status: %s \n", pauxS->codigo, pauxS->tipo, pauxB->nome, statusS);
			
			temSensor = 1;		
			pauxS = pauxS->prox;
		}
		pauxB = pauxB->prox;
	}

	if(!temSensor)
		fprintf(relatorio, "Nenhum sensor implantado \n");

	fprintf(relatorio, "\n");


	//ocorrencias registradas
	pauxB = listaBairros;
	int temOcorrencia = 0;

	while(pauxB != NULL)
	{
		Sensor *pauxS = pauxB->listaSensores;

		while(pauxS != NULL)
		{
			Ocorrencia *pauxO = pauxS->listaOcorrencias;

			while(pauxO != NULL)
			{
				fprintf(relatorio, "Ocorrencia: %d | Gravidade: %d | Status: %d | Descricao: %s\n", pauxO->codigo, pauxO->severidade, pauxO->status, pauxO->descricao);
				temOcorrencia = 1;
				
				pauxO = pauxO->prox;
			}
			pauxS = pauxS->prox;
		}
		pauxB = pauxB->prox;
	}

	if(!temOcorrencia)
			fprintf(relatorio, "Nenhuma ocorrencia registrada");

	fprintf(relatorio, "\n");


	//bairros monitorados
	pauxB = listaBairros;

	if(pauxB == NULL)
		fprintf(relatorio, "Nenhum bairro cadastrado");
	
	else
	{
		while(pauxB != NULL)
		{
			fprintf(relatorio, "Bairro: %s | Cod: %d\n", pauxB->nome, pauxB->codigo);
			pauxB = pauxB->prox;
		}
	}		

	fprintf(relatorio, "\n");


	//equipes e atendimentos
	pauxE = listaEquipes;

	if(pauxE == NULL)
		fprintf(relatorio, "Nenhuma equipe tecnica cadastrada");

	else
	{
		while(pauxE != NULL)
		{
			fprintf(relatorio, "Equipe: %s | Cod: %d | Especialidade: %d | Atendimentos concluidos: %d\n", pauxE->nome, pauxE->codigo, pauxE->especialidade, pauxE->totalAtendimentos);
			
			pauxE = pauxE->prox;
		}
	}
	fclose(relatorio);
	printf("Arquivo relatorio.txt gerado com sucesso!  \n");
}



void registraLog(const char *operacao, const char *status, const char *dados)
{
	FILE *arquivoLog = fopen("saidas/log_execucao.txt", "a");

	if(!arquivoLog)
	{
		printf("Erro ao abrir log_execucao.txt. \n");
		return;
	}

	static int horarioLogico = 1; //static para manter o valor na memoria entre as chamadas da funcao

	fprintf(arquivoLog, "PASSO %d | Operacao: %d | Status: %s | Dados: %s\n", horarioLogico, operacao, status, dados);

	horarioLogico++;
	fclose(arquivoLog);
}


void executarSimulacao(Bairro **listaBairro, Equipe **listaEquipe)
{
	FILE *arquivoSimulacao = fopen("entradas/entrada_simulacao.txt", "r");

	if(arquivoSimulacao)
	{
		printf("Erro ao abrir entradas/entrada_simulacao.txt\n");
        return;
	}

	printf("\n--- INICIANDO SIMULACAO AUTOMATICA VIA ARQUIVO ---\n");
	char comandoSimulacao[30], dadosLog[150], nomeBairro[50], descOcorrencia[100];
    int codBairro, codEquipe, codSensor, codOcorrencia, codChamado, tipoSensor, statusSensor, statusOcorrencia, severidade, prioridade;
    int flagFuncoes;

    while(fscanf(arquivoSimulacao, "%s", comandoSimulacao) == 1 && strcmp(comandoSimulacao, "FIM") != 0)
    {
    	if(strcmp(comandoSimulacao, "cadastrarBairro") == 0)
    	{
    		fscanf(arquivoSimulacao, "%d %s", &codBairro, nomeBairro);

    		if(codBairro <= 0)
    		{
    			sprintf(dadosLog, "Codigo de bairro invalido");
    			registraLog("cadastrarBairro", "FALHA", dadosLog);
    		}

    		if(verificaBairro(*listaBairro, codBairro) != NULL)
    		{
    			sprintf(dadosLog, "Codigo de bairro ja cadastrado");
    			registraLog("cadastrarBairro", "FALHA", dadosLog);
    		}

    		else
    		{
    			insereBairro(listaBairro, codBairro, nomeBairro, &flagFuncoes);
    			sprintf(dadosLog, "Codigo: %d, Nome: %s", codBairro, nomeBairro);
    			registraLog("cadastrarBairro", "SUCESSO", dadosLog);
    		}
    	}

    	if(strcmp(comandoSimulacao, "cadastrarSensor") == 0)
    	{
    		fscanf(arquivoSimulacao, "%d %d %d %d", &codSensor, &tipoSensor, &statusSensor, &codBairro);

    		Bairro *bairroDestino = verificaBairro(*listaBairro, codBairro);

    		if(bairroDestino == NULL)
    		{
    			sprintf(dadosLog, "Bairro %d nao existe", codBairro);
    			registraLog("cadastrarSensor", "FALHA", dadosLog);
    		}

    		if(codSensor <= 0)
    		{
    			sprintf(dadosLog, "Codigo do sensor invalido");
    			registraLog("cadastrarSensor", "FALHA", dadosLog);
    		}

    		if(tipoSensor < 1 || tipoSensor > 5)
    		{
    			sprintf(dadosLog, "Tipo do sensor invalido");
    			registraLog("cadastrarSensor", "FALHA", dadosLog);
    		}

    		if(statusSensor < 1 || statusSensor > 3)
    		{
    			sprintf(dadosLog, "Status do sensor invalido");
    			registraLog("cadastrarSensor", "FALHA", dadosLog);
    		}

    		if(verificaSensorGlobal(*listaBairro, codSensor) != NULL)
    		{
    			sprintf(dadosLog, "Sensor %d ja existe", codSensor);
    			registraLog("cadastrarSensor", "FALHA", dadosLog);
    		}

    		else
    		{
    			insereSensor(*listaBairro, codBairro, codSensor, tipoSensor, statusSensor, &flagFuncoes);
    			sprintf(dadosLog, "Cod: %d, Tipo: %d, Bairro: %d", codSensor, tipoSensor, codBairro);
    			registraLog("cadastrarSensor", "SUCESSO", dadosLog);
    		}
    	}


    	if(strcmp(comandoSimulacao, "cadastrarEquipe") == 0)
    	{
    		fscanf(arquivoSimulacao, "%d %s %d", &codEquipe, nomeBairro, &tipoSensor);

    		if(codEquipe <= 0)
    		{
    			sprintf(dadosLog, "Codigo de equipe inválido");
                registrarLog("cadastrarEquipe", "FALHA", dadosLog);
    		}

    		if(buscarEquipe(*listaEquipe, codEquipe) != NULL)
    		{
    			sprintf(dadosLog, "Equipe %d ja cadastrada", codEquipe);
                registrarLog("cadastrarEquipe", "FALHA", dadosLog);
    		}

    		if(tipoSensor < 1 || tipoSensor > 5)
    		{
    			sprintf(dadosLog, "Especialidade invalida");
                registrarLog("cadastrarEquipe", "FALHA", dadosLog);
    		}

    		else
    		{
    			Equipe *novaEquipe = alocaEquipe(codEquipe, nomeBairro, tipoSensor);

    			if(novaEquipe != NULL)
    			{
    				inserirEquipe(listaEquipe, novaEquipe, &flagFuncoes);

    				if(flagFuncoes == 1)
    				{
    					sprintf(dadosLog, "Codigo: %d, Nome: %s, Especialidade: %d", codEquipe, nomeBairro, tipoSensor);
                		registrarLog("cadastrarEquipe", "SUCESSO", dadosLog);
    				}

    				else
    				{
    					free(novaEquipe);
    					sprintf(dadosLog, "Erro ao inserir equipe");
                		registrarLog("cadastrarEquipe", "FALHA", dadosLog);
    				}
    			}
    		}
    	}

    	if(strcmp(comandoSimulacao, "registrarOcorrencia") == 0)
    	{
    		fscanf(arquivoSimulacao, "%d %d %d %d %d  %s", &codOcorrencia, &severidade, &statusOcorrencia, &codSensor, &codBairro, descOcorrencia);
    		
    		Bairro *enderecoBairro = verificaBairro(listaBairro, codBairro);

    		if(enderecoBairro != NULL)
    		{
    			Sensor *enderecoSensor = verificaSensor(enderecoBairro->listaSensores, codSensor);

    			if(enderecoSensor != NULL)
    			{
    				insereOcorrencia(enderecoBairro, &(enderecoSensor->listaOcorrencias), codOcorrencia, severidade, descOcorrencia, statusOcorrencia, &flagFuncoes);

    				if(flagFuncoes == 1)
    				{
    					sprintf(dadosLog, "ID: %d, Sensor: %d, Gravidade: %d", codOcorrencia, codSensor, severidade);
                        registrarLog("registrarOcorrencia", "SUCESSO", dadosLog);

                        if(severidade == 4 && statusOcorrencia == 1)
                        {
                        	int codChamadoAutomatico = 7000 + codOcorrencia;

                        	if(verificaChamado(*listaEquipe, codChamadoAutomatico) == NULL)
                        	{
                        		Ocorrencia *OcorrenciaReal = verificaOcorrencia(enderecoSensor->listaOcorrencias, codOcorrencia);
                        		Chamado *novoChamado = alocaChamado(codChamadoAutomatico, 3, 1, OcorrenciaReal);

                        		int flagAuto;
                        		inserirChamadoEquipe(*listaEquipe, enderecoSensor->tipo, novoChamado, &flagAuto);

                        		if(flagAuto == 1 && OcorrenciaReal != NULL)
                        		{
                        			OcorrenciaReal->chamado = novoChamado;

                        			int flagStatus;
                        			alteraStatusSensor(*listaBairro, codSensor, 2, &flagStatus);

                        			sprintf(dadosLog, "Chamado %d gerado. Sensor %d em manutencao", codChamadoAutomatico, codSensor);
                        			registrarLog("ChamadoAutomatico", "SUCESSO", dadosLog);
                        		}

                        		else
                        			free(novoChamado);
                        	}
                        }
    				}
    			}

    			else
    			{
    				sprintf(dadosLog, "Sensor %d nao localizado no bairro %d", codSensor, codBairro);
                    registrarLog("registrarOcorrencia", "FALHA", dadosLog);
    			}
    		}

    		else
    		{
    			sprintf(dadosLog, "Bairro %d nao existe", codBairro);
            	registrarLog("registrarOcorrencia", "FALHA", dadosLog);
    		}
    	}

    	if(strcmp(comandoSimulacao, "gerarChamado") == 0)
    	{
    		fscanf(arquivoSimulacao, "%d %d %d %d", &codChamado, &codOcorrencia, &prioridade, &statusSensor);

    		if(verificaChamado(*listaEquipe, codChamado) != NULL)
    		{
    			sprintf(dadosLog, "Chamado %d ja existente", codChamado);
                registrarLog("gerarChamado", "FALHA", dadosLog);
    		}

    		else
    		{
    			gerarChamado(*listaEquipe, *listaBairro, codChamado, codOcorrencia, prioridade, statusSensor, &flagFuncoes);

    			if(flagFuncoes == 1)
    			{
    				sprintf(dadosLog, "Cod: %d, Ocorrencia: %d", codChamado, codOcorrencia);
                	registrarLog("gerarChamado", "SUCESSO", dadosLog);
    			}

    			else
    			{
    				sprintf(dadosLog, "Erro ao vincular o chamado %d", codChamado);
                	registrarLog("gerarChamado", "FALHA", dadosLog);
    			}
    		}
    	}


    	if(strcmp(comandoSimulacao, "associarEquipe") == 0)
    	{
    		fscanf(arquivoSimulacao, "%d %d", &codChamado, &codEquipe);
            associarEquipe(*listaEquipe, codChamado, codEquipe, &flagFuncoes);

            if(flagFuncoes == 1)
            {
            	sprintf(dadosLog, "Chamado: %d -> Nova Equipe: %d", codChamado, codEquipe);
                registrarLog("associarEquipe", "SUCESSO", dadosLog);
            }

            else
            {
            	sprintf(dadosLog, "Nao foi possivel associar o Chamado %d a Equipe %d", codChamado, codEquipe);
                registrarLog("associarEquipe", "FALHA", dadosLog);
            }
    	}


    	if(strcmp(arquivoBairros, "alteraStatusSensor") == 0)
    	{
    		fscanf(arquivoSimulacao, "%d %d %d", &codSensor, &codBairro, &statusSensor);

    		if(statusSensor < 1 || statusSensor > 3)
    		{
    			sprintf(dadosLog, "Status invalido");
                registrarLog("alterarStatusSensor", "FALHA", dadosLog);
    		}

    		else
    		{
    			Sensor *sensorAlvo = verificaSensorGlobal(*listaBairro, codSensor);
    			alteraStatusSensor(*listaBairro, codSensor, statusSensor, &flagFuncoes);

    			if(flagFuncoes == 1 && sensorAlvo != NULL)
    			{
    				sprintf(dadosLog, "Sensor: %d, Novo Status: %d", codSensor statusSensor);
                	registrarLog("alterarStatusSensor", "SUCESSO", dadosLog);

                	if(statusSensor == 3)
                	{
                		int codChamadoAutomatico = 9000 + codSensor;
                		if(verificaChamado(*listaEquipe, codChamadoAutomatico) == NULL)
                		{
                			Chamado *novoChamado = alocaChamado(codChamadoAutomatico, 3, 1, NULL);

                			int flagAuto;
                			inserirChamadoEquipe(*listaEquipe, sensorAlvo->tipo, novoChamado, &flagAuto);

                			if(flagAuto == 1)
                			{
                				sprintf(dadosLog, "Chamado %d aberto por perda de sinal", codChamadoAutomatico);
                				registrarLog("ChamadoAutomatico", "FALHA", dadosLog);
                			}

                			else
                				free(novoChamado);
                		}


                	}
    			}

    			else
    			{
    				sprintf(dadosLog, "Sensor %d nao encontrado", codSensor);
                	registrarLog("alterarStatusSensor", "FALHA", dadosLog);
    			}
    		}
    	}

    	if(strcmp(comandoSimulacao, "finalizarChamado") == 0)
    	{
    		scanf(arquivoSimulacao, "%d", &codChamado);
    		finalizarChamado(*listaEquipe, codChamado, &flagFuncoes);

    		if(flagFuncoes == 1)
    		{
    			sprintf(dadosLog, "Chamado %d finalizado", codChamado);
                registrarLog("finalizarChamado", "SUCESSO", dadosLog);
    		}

    		else
    		{
    			sprintf(dadosLog, "Chamado %d nao localizado", codChamado);
                registrarLog("finalizarChamado", "FalHA", dadosLog);
    		}
    	}


    	if(strcmp(comandoSimulacao, "listaSensoresBairro") == 0)
    	{
    		scanf(arquivoSimulacao, "%d", &codBairro);

    		Bairro *bairroAlvo = verificaBairro(*listaBairro, codBairro);

    		if(bairroAlvo == NULL)
    		{
    			sprintf(dadosLog, "Bairro %d nao localizado", codBairro);
                registrarLog("listarSensoresBairro", "FALHA", dadosLog);
    		}

    		else
    		{
    			printf("\nSensores no Bairro %d: %s \n", bairroAlvo->nome);
    			listaSensoresPorBairro(*listaBairro, codBairro, &flagFuncoes);

    			sprintf(dadosLog, "Bairro: %d, Nome: %s", codBairro, bairroAlvo->nome);
                registrarLog("listarSensoresBairro", "SUCESSO", dadosLog);
    		}
    	}


    	if(strcmp(comandoSimulacao, "listarOcorrencias") == 0)
    	{
    		fscanf(arquivoSimulacao, "%d", &codSensor);

    		Sensor *sensorAlvo = verificaSensorGlobal(*listaBairro, codSensor);

    		if(sensorAlvo == NULL)
    		{
    			sprintf(dadosLog, "Sensor %d nao encontrado para listar ocorrencias", codSensor);
                registrarLog("listarOcorrencias", "FALHA", dadosLog);
                printf("\nSensor %d nao encontrado\n", codSensor);
    		}

    		else
    		{
    			listarOcorrencias(sensorAlvo, &flagFuncoes);
    			sprintf(dadosLog, "Sensor %d", codSensor);
                registrarLog("listarOcorrencias", "SUCESSO", dadosLog);
    		}
    	}
    }

    registrarLog("FIM", "SUCESSO", "Arquivo de simulacao automatica chegou ao fim.");
    fclose(arquivoSimulacao);

    salvaSistema(*listaBairro, *listaEquipe);
    printf("\nSimulcao automatica concluida com sucesso!\n");
}

//Funções Bairro
Bairro * alocaBairro(int codigo, char nome[])
{
	Bairro *novo = malloc(sizeof(Bairro));
	if (novo == NULL) 
		return NULL;
	novo -> codigo = codigo;
	strcpy(novo->nome, nome);
	novo -> quantidadeOcorrencias = 0;
	novo -> totalPorSeveridade[0] = 0;
	novo -> totalPorSeveridade[1] = 0;
	novo -> totalPorSeveridade[2] = 0;
	novo -> totalPorSeveridade[3] = 0;
	novo -> quantidadeSensores = 0;
	novo -> listaSensores = NULL;
	novo -> prox = NULL;
	return novo;
}

void insereBairro(Bairro **listaBairro, int codigo, char nome[], int *flag)
{
	if (verificaBairro(*listaBairro, codigo))
    {
    	*flag = 0;
    	return;
    }
    if (verificaBairroNome(*listaBairro, nome) == 1)
  	{
  		*flag = 0;
  		return;
  	}
  	Bairro *novo = alocaBairro(codigo, nome);

	if (novo == NULL)
	{
		*flag = 0;
		return;
	}	
	if(*listaBairro == NULL)
	{
		*listaBairro = novo;
		*flag = 1;
		return;
	}
	Bairro *pauxBairro = *listaBairro;
	while (pauxBairro -> prox != NULL)
		pauxBairro = pauxBairro -> prox;
	pauxBairro -> prox = novo;
	*flag = 1;
	return;
}

void listaBairro(Bairro *pauxBairro)
{
	if (pauxBairro == NULL)
	{
		printf("\nNenhum bairro cadastrado!");
		return;
	}
	while(pauxBairro != NULL)
	{
		printf("\nCodigo: %d, Bairro %s", pauxBairro->codigo, pauxBairro->nome);
		pauxBairro = pauxBairro -> prox;
	}
	return;
}

void removeBairroCodigo(Bairro **listaBairro, int codBairro, int *flag)
{
	Bairro *pauxBairro = *listaBairro;
	Bairro * enderecoBairro = verificaBairro(pauxBairro, codBairro); 
	if (enderecoBairro == NULL)
	{
		printf("\nNão existe bairro com esse codigo!");
		*flag = 0;
		return;
	} //Tratamento de erros
	while (enderecoBairro -> listaSensores)
			removeSensorInicio(enderecoBairro);
	if (pauxBairro == enderecoBairro)
	{
		*listaBairro = pauxBairro -> prox;
		free(pauxBairro);
		*flag = 1;
		return;
	}//Remocao se o bairro for o primeiro

	while(pauxBairro -> prox != NULL)
	{
		if ((pauxBairro -> prox) == enderecoBairro)
		{
			pauxBairro -> prox = enderecoBairro -> prox;
			free(enderecoBairro);
			*flag = 1;
			return;
		}	
		pauxBairro = pauxBairro -> prox;
	}//Remocao no inicio e no fim

}

void removeBairroInicio(Bairro **listaBairro)
{
	if (*listaBairro == NULL)
	{
		return;
	}
	Bairro *pauxBairro = *listaBairro;
	*listaBairro = pauxBairro -> prox;
	while (pauxBairro -> listaSensores)
		removeSensorInicio(pauxBairro);
	free(pauxBairro);
	pauxBairro = NULL;
	return;		
} //Usado para desalocacao final, nao precisa remover dos arquivos

Bairro * verificaBairro(Bairro *pauxBairro, int codBairro) //Verifica se o bairro existe na lista, se sim retorna 1, se nao retorna 0
{
	while (pauxBairro != NULL) //Percorre os bairros
	{
		if (pauxBairro->codigo == codBairro)
			return pauxBairro;
		pauxBairro = pauxBairro->prox;	
	}
	return NULL;
} //Ainda pode usar na verificacao de codigo para nao criar dois bairros com o mesmo codigo, se o valor retornado nao for nulo eh pq ja existe

int verificaBairroNome(Bairro *pauxBairro, char nomeBairro[])
{
    char nomeBuscaMinusculo[50]; 
    int i;

    for(i = 0; nomeBairro[i] != '\0'; i++)
    {
        nomeBuscaMinusculo[i] = tolower(nomeBairro[i]);
    }
    nomeBuscaMinusculo[i] = '\0'; 

    while (pauxBairro != NULL)
    {
        char nomeBairroSalvoMinusculo[50];
        
        for(i = 0; pauxBairro->nome[i] != '\0'; i++)
        {
            nomeBairroSalvoMinusculo[i] = tolower(pauxBairro->nome[i]);
        }
        nomeBairroSalvoMinusculo[i] = '\0';

        if (strcmp(nomeBairroSalvoMinusculo, nomeBuscaMinusculo) == 0)
        {
            return 1; //Ja existe um bairro com esse nome
        }
        
        pauxBairro = pauxBairro->prox;
    }
    return 0; // Nao existe um bairro com esse nome
}


//Funções Sensores

Sensor * alocaSensor(int codigo, int tipo,  int status)
{
	Sensor *novo = malloc(sizeof(Sensor));
	if (novo == NULL)
		return NULL;
	novo -> codigo = codigo;
	novo -> tipo = tipo;
	novo -> status = status;
	novo -> listaOcorrencias = NULL;
	novo -> prox = NULL;
	return novo;
}

void insereSensor(Bairro *listaBairro, int codBairro, int codSensor, int tipo,  int status, int *flag)//fazer verificacao do endereco do bairro na main
{
	Bairro * enderecoBairro = verificaBairro(listaBairro, codBairro)
	if(enderecoBairro == NULL)
	{
        *flag = 0;
        return;
	}
	if(verificaSensorGlobal(listaBairro, codSensor))
    {
        *flag = 0;
        return;
    }
	
	Sensor *novo = alocaSensor(codSensor, tipo, status);
	if (novo == NULL)
	{
		*flag = 0;
		return;
	}	
	Sensor **listaSensores = &(enderecoBairro -> listaSensores);
	if (*listaSensores == NULL)
	{
		*listaSensores = novo;
		enderecoBairro -> quantidadeSensores++;
		*flag = 1;
		return;
	}
	Sensor *pauxSensor = *listaSensores;
	while (pauxSensor -> prox != NULL)
		pauxSensor = pauxSensor -> prox;
	pauxSensor -> prox = novo;
	enderecoBairro -> quantidadeSensores++;
	*flag = 1;
	return;
}

void removeSensorInicio(Bairro *enderecoBairro)
{
	if (enderecoBairro -> listaSensores == NULL)
	{
			return;
	}
	Sensor * pauxSensor = enderecoBairro -> listaSensores;
	enderecoBairro -> listaSensores = pauxSensor -> prox;
	while(pauxSensor -> listaOcorrencias)
	{
		removeOcorrenciaInicio(enderecoBairro, &(pauxSensor -> listaOcorrencias));
	}	
	free(pauxSensor);
	pauxSensor = NULL;
	return;
}//Usado so para remover os sensores quando um bairro for apagado ou na liberacao final de memoria

Sensor * verificaSensor (Sensor *pauxSensor, int codSensor)
{
	while (pauxSensor != NULL) //Percorre os sensores do bairro
	{
		if (pauxSensor->codigo == codSensor)
			return pauxSensor;
		pauxSensor = pauxSensor->prox;	
	}
	return NULL;

} //Usado para achar o endereco do sensor no bairro

Sensor* verificaSensorGlobal(Bairro *pauxBairro, int codSensor)
{
    while (pauxBairro != NULL)
    {
        Sensor *pauxSensor = pauxBairro->listaSensores;
        while (pauxSensor != NULL)
        {
            if (pauxSensor->codigo == codSensor)
                return pauxSensor; 
            pauxSensor = pauxSensor->prox;    
        }  
        pauxBairro = pauxBairro->prox;
    }
    return NULL; 
}//Verifica se existe um sensor em outros bairros com o mesmo codigo

void alteraStatusSensor(Bairro *listaBairros, int codSensor, int novoStatus, int *flag)
{
    Sensor *sensorAlvo = verificaSensorGlobal(listaBairros, codSensor);
    if (sensorAlvo == NULL)
    {
        *flag = 0; 
        return;
    }
    sensorAlvo->status = novoStatus;
    *flag = 1; 
}

void listaSensor(Sensor *enderecoSensor)
{
	char tipoSensorChar[30], statusSensorChar[30];
	switch(enderecoSensor -> tipo)
    {
        case 1: strcpy(tipoSensorChar, "Temperatura");break;
        case 2: strcpy(tipoSensorChar, "Enchente");break;
        case 3: strcpy(tipoSensorChar, "Fumaca");break;
        case 4: strcpy(tipoSensorChar, "Transito");break;
        case 5: strcpy(tipoSensorChar, "Iluminacao_publica");break;
    }
    switch(enderecoSensor -> status)
    {
    case 1: strcpy(statusSensorChar, "Ativo");break;
    case 2: strcpy(statusSensorChar, "Manutencao");break;
    case 3: strcpy(statusSensorChar, "Offline");break;
    }
    printf("\n---------------------------------");
    printf("\nSensor cod - %d", enderecoSensor -> codigo);
    printf("\nTipo - %s", tipoSensorChar);
    printf("\nStatus - %s", statusSensorChar);
    printf("\n---------------------------------");
}

void listaSensoresPorBairro(Bairro *listaBairro, int codBairro, int *flag)
{
    Bairro * enderecoBairro = verificaBairro(listaBairro, codBairro)
	if(enderecoBairro == NULL)
	{
        *flag = 0;
        return;
	}
    if (enderecoBairro -> listaSensores == NULL)
    {
        printf("\nNenhum sensor cadastrado neste bairro!\n");
        *flag = 0;
        return;
    }
    
    Sensor *pauxSensor = enderecoBairro->listaSensores;
    
    printf("\nBairro %s - Cod: %d", enderecoBairro->nome, enderecoBairro->codigo);
    
    while (pauxSensor != NULL)
    {
        listaSensor(pauxSensor);
        pauxSensor = pauxSensor->prox;
    }
    *flag = 1;
}

//Funções Ocorrências

Ocorrencia * alocaOcorrencia(int codigo, int severidade, char descricao[], int status)
{
	Ocorrencia *novo = malloc(sizeof(Ocorrencia));
	if (novo == NULL)
		return NULL;
	novo -> codigo = codigo;
	novo -> severidade = severidade;
	strcpy(novo -> descricao, descricao);
	novo -> status = status;
	novo -> chamado = NULL;
	novo -> prox = NULL;
	return novo;
}

void insereOcorrencia(Bairro *enderecoBairro, Ocorrencia **listaOcorrencia, int codigo, int severidade, char descricao[], int status) //O ponteiro listaOcorrencia deve ser do sensor a ser inserido, passar pela main;
{
	Ocorrencia *novo = alocaOcorrencia(codigo, severidade, descricao, status);
	if (novo == NULL)
		return;
	enderecoBairro -> quantidadeOcorrencias++;
	switch (novo -> severidade)
	{
		case 1: enderecoBairro -> totalPorSeveridade [0]++; break;
		case 2: enderecoBairro -> totalPorSeveridade [1]++; break;
		case 3: enderecoBairro -> totalPorSeveridade [2]++; break;	
		case 4: enderecoBairro -> totalPorSeveridade [3]++; break;
	}
	if (*listaOcorrencia == NULL)
	{
		*listaOcorrencia = novo;
		return;
	}
	Ocorrencia *pauxOcorrencia = *listaOcorrencia;
	while(pauxOcorrencia -> prox != NULL)
		pauxOcorrencia = pauxOcorrencia -> prox;
	pauxOcorrencia -> prox = novo;
	return;
} //Lembrar de verificar se o bairro existe na main, aí não terá como cadastrar em NULL



void removeOcorrenciaInicio(Bairro *enderecoBairro, Ocorrencia **listaOcorrencia)//Ja faz com que o ponteiro da ocorrencia do chamado seja setado como nulo
{
	if (*listaOcorrencia == NULL)
	{
		printf("\nLista de ocorrências vazia!");
		return;
	}
	Ocorrencia *pauxOcorrencia = *listaOcorrencia;
	*listaOcorrencia = (*listaOcorrencia) -> prox;
	if(pauxOcorrencia -> chamado)
			(pauxOcorrencia -> chamado) -> ocorrencia = NULL;
	enderecoBairro -> quantidadeOcorrencias--;
	switch (pauxOcorrencia -> severidade)
	{
		case 1: enderecoBairro -> totalPorSeveridade [0]--; break;
		case 2: enderecoBairro -> totalPorSeveridade [1]--; break;
		case 3: enderecoBairro -> totalPorSeveridade [2]--; break;
		case 4: enderecoBairro -> totalPorSeveridade [3]--; break;	
	}
	free(pauxOcorrencia);
	return;
}
 
Ocorrencia * verificaOcorrencia(Ocorrencia *pauxOcorrencia, int codOcorrencia) //Verifica se a ocorrencia existe na lista, se sim retorna 1, se nao retorna 0
{
	while (pauxOcorrencia != NULL) //Percorre as ocorrencias
	{
		if (pauxOcorrencia -> codigo == codOcorrencia)
			return pauxOcorrencia;
		pauxOcorrencia = pauxOcorrencia->prox;	
	}
	return NULL;
}

Ocorrencia* verificaOcorrenciaGlobal(Bairro *pauxBairro, int codOcorrencia)
{
    while (pauxBairro != NULL)
    {
        Sensor *pauxSensor = pauxBairro->listaSensores;
        while (pauxSensor != NULL)
        {

            Ocorrencia *enderecoOcorrencia = verificaOcorrencia((Ocorrencia*)pauxSensor->listaOcorrencias, codOcorrencia);
          
            if (enderecoOcorrencia != NULL)
            {
                return enderecoOcorrencia; 
            }

            pauxSensor = pauxSensor->prox; 
        }

        pauxBairro = pauxBairro->prox; 
    }
    
    return NULL; // Se percorreu tudo e não achou, retorna NULL
}



void listaOcorrencia(Ocorrencia *enderecoOcorrencia)
{
    char severidadeChar[20];
    
    // Traduz a severidade (Exemplo: 1 - Baixa, 2 - Média, 3 - Alta)
    switch(enderecoOcorrencia->severidade)
    {
        case 1: strcpy(severidadeChar, "Baixa"); break;
        case 2: strcpy(severidadeChar, "Media"); break;
        case 3: strcpy(severidadeChar, "Alta"); break;
        case 4:	strcpy(severidadeChar, "Crítica"); break;
    }
    printf("\nOcorrência - %d, Severidade - %s", enderecoOcorrencia->codigo, severidadeChar);
    printf("\nDescrição: %s", enderecoOcorrencia->descricao);
}

void listaOcorrencias(Bairro *pauxBairro, int *flag) 
{
    if (pauxBairro == NULL)
    {
        *flag = 0;
        return;
    }
    *flag = 0;//Nao tem ocorrencias no sistema
    int encontrouAlguma = 0; 
    while (pauxBairro != NULL)
    {
        Sensor *pauxSensor = pauxBairro->listaSensores;

        while (pauxSensor != NULL)
        {
            Ocorrencia *pauxOcorrencia = (Ocorrencia*) pauxSensor->listaOcorrencias;

            while (pauxOcorrencia != NULL)
            {
                
                if (pauxOcorrencia == (Ocorrencia*) pauxSensor->listaOcorrencias)
                {
                    printf("\n\n Bairro: %s Sensor : %d", pauxBairro->nome, pauxSensor->codigo);
                }

                listaOcorrencia(pauxOcorrencia);
                *flag = 1; // Ativa a flag informando que o sistema não está vazio
                pauxOcorrencia = pauxOcorrencia->prox; 
            }

            pauxSensor = pauxSensor->prox; 
        }
        pauxBairro = pauxBairro->prox; 
    }
}
		
void liberarMemoria (Bairro **listaBairro, Equipe **listaEquipe)
{
	while(*listaBairros)
		removeBairroInicio(listaBairro);
	while(*listaEquipe)
		removeEquipeInicio(listaEquipe);
	return;

}
