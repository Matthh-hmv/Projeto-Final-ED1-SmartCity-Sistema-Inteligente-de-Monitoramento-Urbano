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
		insereBairro(listaBairros, codBairro, nomeBairro);

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

	int codSensor, tipoSensor, statusSensor, codBairro;

	while(fscanf(arquivoSensores, "%d %d %d %d", &codSensor, &tipoSensor, &statusSensor, &codBairro) == 4)
	{
		Bairro *bairroDestino = verificaBairro(listaBairros, codBairro);

		if(bairroDestino != NULL)
			insereSensor(bairroDestino, codBairro, codSensor, tipoSensor, statusSensor);
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

void inserirEquipe(Equipe **listaEquipes, Equipe *novaEquipe)
{

	if(novaEquipe == NULL)
		return;

	if(*listaEquipes == NULL)
		*listaEquipes = novaEquipe;

	else
	{
		Equipe *paux = *listaEquipes;

		while(paux->prox != NULL)
			paux = paux->prox;

		paux->prox = novaEquipe;
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


void associarEquipe(Equipe *listaEquipes, int codChamado, int codEquipe)
{
	Equipe *equipeDestino = buscarEquipe(listaEquipes, codEquipe);

	if(equipeDestino == NULL)
	{
		printf("equipe nao encontrada!\n");
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
		return;
	}

	if(equipeDestino->especialidade != equipeAntiga->especialidade)
	{
		printf("A equipe %s nao pode atender ao chamado %d. Especialidade incompativel!", equipeDestino->nome, codChamado);
		return;
	}

	if(anteriorChamado == NULL)
		equipeAntiga->listaChamados = chamadoAlvo->prox;

	else
		anteriorChamado->prox = chamadoAlvo->prox;


	chamadoAlvo->prox = NULL;

	inserirChamadoEquipe(listaEquipes, equipeDestino->codigo, chamadoAlvo);

	printf("Chamado %d associado com sucesso a equipe %s \n", codChamado, equipeDestino->nome);
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


void inserirChamadoEquipe(Equipe *listaEquipes, int codEquipe, Chamado *novoChamado)
{
	if(novoChamado == NULL)
		return;

	Equipe *eqp = buscarEquipe(listaEquipes, codEquipe);

	if(eqp == NULL)
	{
		printf("equipe nao encntrada!\n");
		free(novoChamado);
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


void gerarChamado(Equipe *listaEquipes, Bairro *listaBairros, int codChamado, int codOcorrencia, int priori, int statusSensor)
{
	if(verificaChamado(listaEquipes, codChamado) != NULL)
	{
		printf("O chamado %d ja esta cadastrado no sistema. \n", codChamado);
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
		return;
	}

	Chamado *novoChamado = alocaChamado(codChamado, priori, statusSensor, OcorrenciaReal);
	inserirChamadoEquipe(listaEquipes, equipeCompativel->codigo, novoChamado);

	OcorrenciaReal->chamado = novoChamado;

	printf("Chamado %d gerado automaticamente e atribuido a equipe %s \n", codChamado, equipeCompativel->nome);
}


void finalizarChamado(Equipe *listaEquipes, int codChamado)
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
		return;
	}

	chamadoAlvo->status = 3; // 3 = chamado finalizado
	equipeResponsavel->totalAtendimentos++;

	if(chamadoAlvo->ocorrencia != NULL)
		chamadoAlvo->ocorrencia->status = 0; //ocorrencia resovlida

	printf("Chamado %d finalizado com sucesso! \n", codChamado);
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

}


//MATHEUS


//Fazer chamado automatico dependendo da severidade ou offline do sensor (precisa da funcao do chamado)

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
    printf("\n=================================");
    printf("\nSensor cod - %d", enderecoSensor -> codigo);
    printf("\nTipo - %s", tipoSensorChar);
    printf("\nStatus - %s", statusSensorChar);
    printf("\n=================================");
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

void removeOcorrenciaCodigo(Bairro *enderecoBairro, Ocorrencia **listaOcorrencia, int codOcorrencia) //Ja faz com que o ponteiro da ocorrencia do chamado seja setado como nulo
{
	Ocorrencia *pauxOcorrencia = *listaOcorrencia;
	if (pauxOcorrencia == NULL)
	{
		printf("\nLista de ocorrências deste sensor está vazia!");
		return;
	}
	Ocorrencia *enderecoOcorrencia = verificaOcorrencia(*listaOcorrencia, codOcorrencia); 
	if (enderecoOcorrencia == NULL)
	{
		printf("\nNão existe ocorrencia com esse codigo nesse sensor!");
		return;
	} //Tratamento de erros
	enderecoBairro -> quantidadeOcorrencias--;
	if (pauxOcorrencia == enderecoOcorrencia )
	{
		*listaOcorrencia = pauxOcorrencia -> prox;
		if(pauxOcorrencia -> chamado)
			(pauxOcorrencia -> chamado) -> ocorrencia = NULL;
		switch (pauxOcorrencia -> severidade)
		{
			case 1: enderecoBairro -> totalPorSeveridade [0]--; break;
			case 2: enderecoBairro -> totalPorSeveridade [1]--; break;
			case 3: enderecoBairro -> totalPorSeveridade [2]--; break;	
		}
		free(pauxOcorrencia);
		return;
	}//Remocao se for a primeira ocorrencia

	while(pauxOcorrencia -> prox != NULL)
	{
		if (pauxOcorrencia -> prox  == enderecoOcorrencia)
		{
			pauxOcorrencia -> prox = enderecoOcorrencia -> prox;
			if (enderecoOcorrencia -> chamado)	
				(enderecoOcorrencia -> chamado) -> ocorrencia = NULL;
			switch (enderecoOcorrencia -> severidade)
			{
				case 1: enderecoBairro -> totalPorSeveridade [0]--; break;
				case 2: enderecoBairro -> totalPorSeveridade [1]--; break;
				case 3: enderecoBairro -> totalPorSeveridade [2]--; break;	
			}
			free(enderecoOcorrencia);
			return;
		}	
		pauxOcorrencia = pauxOcorrencia -> prox;
	}//Remocao no inicio e no fim

} //Usado so para remover as ocorrencias quando um sensor for apagado ou na liberacao final de memoria

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


