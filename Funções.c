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


void gerarChamado(Equipe *listaEquipes, Bairro *listaBairros, int codChamado, int codOcorrencia, int priori, int statusSensor)
{
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


void finalizarChamado(Equipe *listaEquipes, int codChamado)
{
	Chamado *chamadoAlvo = verificaChamado(listaEquipes, codChamado);

	if(chamadoAlvo == NULL)
	{
		printf("Chamado %d nao encontrado no sistema \n", codChamado);
		return;
	}

	chamadoAlvo->status = 3; // 3 = chamado finalizado

	if(chamadoAlvo->ocorrencia != NULL)
		chamadoAlvo->ocorrencia->status = 0; //ocorrencia resovlida

	printf("Chamado %d finalizado com sucesso! \n", codChamado);
}

//MATHEUS

//Remover os sensores e ocorrencias quando fazer a remocao dos bairros
//Terminar funcoes dos sensores
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

void insereBairro(Bairro **listaBairro, int codigo, char nome[])
{
	Bairro *novo = alocaBairro(codigo, nome);
	if (novo == NULL)
		return;
	if(*listaBairro == NULL)
	{
		*listaBairro = novo;
		return;
	}
	Bairro *pauxBairro = *listaBairro;
	while (pauxBairro -> prox != NULL)
		pauxBairro = pauxBairro -> prox;
	pauxBairro -> prox = novo;
	return;
}

void buscaBairro(Bairro *pauxBairro, int codBairro)
{
	Bairro *enderecoBairro = verificaBairro(pauxBairro, codBairro);
	if (enderecoBairro == NULL)
	{
		printf("\nBairro não cadastrado!");
		return;
	}
	printf("\nBairro %s - cod: %d", enderecoBairro->nome, enderecoBairro->codigo);
	printf("\nQuantidade de ocorrências: %d", enderecoBairro->quantidadeOcorrencias);
	printf("\nQuantidade de sensores: %d", enderecoBairro->quantidadeSensores);
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

void removeBairroCodigo(Bairro **listaBairro, int codBairro)
{
	Bairro *pauxBairro = *listaBairro;
	if (pauxBairro == NULL)
	{
		printf("\nLista de bairros está vazia!");
		return;
	}
	Bairro * enderecoBairro = verificaBairro(pauxBairro, codBairro); 
	if (enderecoBairro == NULL)
	{
		printf("\nNão existe bairro com esse codigo!");
		return;
	} //Tratamento de erros

	if (pauxBairro == enderecoBairro)
	{
		*listaBairro = pauxBairro -> prox;
		//Apagar sensores
		free(pauxBairro);
		return;
	}//Remocao se o bairro for o primeiro

	while(pauxBairro -> prox != NULL)
	{
		if ((pauxBairro -> prox) == enderecoBairro)
		{
			pauxBairro -> prox = enderecoBairro -> prox;
			//Apagar sensores
			free(enderecoBairro);
			return;
		}	
		pauxBairro = pauxBairro -> prox;
	}//Remocao no inicio e no fim

}

void removeBairroInicio(Bairro **listaBairro)
{
	//fazer para a desalocacao final
}

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

void insereSensor(Bairro *enderecoBairro, int codBairro, int codSensor, int tipo,  int status)//fazer verificacao do endereco bairro na main
{
	Sensor *novo = alocaSensor(codSensor, tipo, status);
	if (novo == NULL)
		return;
	Sensor **listaSensores = &(enderecoBairro -> listaSensores);
	if (*listaSensores == NULL)
	{
		*listaSensores = novo;
		enderecoBairro -> quantidadeSensores++;
		return;
	}
	Sensor *pauxSensor = *listaSensores;
	while (pauxSensor -> prox != NULL)
		pauxSensor = pauxSensor -> prox;
	pauxSensor -> prox = novo;
	enderecoBairro -> quantidadeSensores++;
	return;
}

void removeSensorCodigo(Bairro *enderecoBairro, int codSensor)//Acha o endereco do bairro na main
{
	Sensor *pauxSensor = enderecoBairro->listaSensores;
	if (pauxSensor == NULL)
	{
		printf("\nLista de sensores desse bairro está vazia!");
		return;
	}
	Sensor *enderecoSensor = verificaSensor(pauxSensor, codSensor);
	if (enderecoSensor == NULL)
	{
		printf("\nNão existe sensor com esse codigo nesse bairro");
		return;
	}
	if (pauxSensor == enderecoSensor)
	{
		enderecoBairro -> listaSensores = enderecoSensor -> prox;
		while(enderecoSensor -> listaOcorrencias)
			removeOcorrenciaInicio(enderecoBairro, &(enderecoSensor -> listaOcorrencias));
		free(enderecoSensor);
		return;
	}//Remocao se o sensor for o primeiro

	while(pauxSensor -> prox != NULL)
	{
		if ((pauxSensor -> prox) == enderecoSensor)
		{
			pauxSensor -> prox = enderecoSensor -> prox;
			while(enderecoSensor -> listaOcorrencias)
				removeOcorrenciaInicio(enderecoBairro, &(enderecoSensor -> listaOcorrencias));
			free(enderecoSensor);
			return;
		}	
		pauxSensor = pauxSensor -> prox;
	}

}

Sensor * verificaSensor (Sensor *pauxSensor, int codSensor)
{
	while (pauxSensor != NULL) //Percorre os bairros
	{
		if (pauxSensor->codigo == codSensor)
			return pauxSensor;
		pauxSensor = pauxSensor->prox;	
	}
	return NULL;

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

}

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

