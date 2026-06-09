#include "prototipos.h"

void main()
{
    Bairro *listaBairro = NULL, *enderecoBairro = NULL;
    Equipe *listaEquipe = NULL;
    Sensor *enderecoSensor = NULL;

    int escMenu, escFuncionalidades, escEntradaManual, codBairro, flagCarregarArquivos = 0, flagFuncoes; //Flag geral para verificar se a funcao foi bem sucedida;
    int codSensor, tipoSensor, statusSensor, codChamado, codOcorrencia, prioridade, severidade, statusOcorrencia;
    char nomeBairro[50], descOcorrencia[100];

    char comandoSimnulacao[30], dadosLog[150];

    printf("Bem vindo ao sistema de controle de SmartCity's\n");
    do{
        printf("\n1 - Carregar arquivos \n2 - Entrada manual de Dados\n3 - Simulação via arquivo de entrada\n0 - Sair do programa\n");
        scanf("%d", &escMenu);
        getchar();
        switch(escMenu)
        {
            case 1:
            {
                /*Aqui vai ser onde o sistema vai carregar os arquivos que guardam as informações, pensar na possibilidade de 
                fazer escolhas para o que carregar ou se vamos carregar todos os arquivos de uma vez*/

                //acho melhor carregar tudo de uma vez

                if(flagCarregarArquivos == 0)
                {
                    carregarBairros(&listaBairro);
                    carregarEquipes(&listaEquipe);

                    carregarSensores(listaBairro);
                    carregarOcorrencias(&listaBairro);
                    carregarChamados(listaBairro, listaEquipe);
                    printf("\nArquivos Carregados!\n");
                    flagCarregarArquivos = 1;
                }

                else
                    printf("\nArquivos ja carregados!");
            }break;


            case 2:
            {
                if(flagCarregarArquivos == 0)
                {
                    printf("\nCarregue os dados (opcao 1) antes!");
                    break;
                }
                do{
                    printf("\n\n1 - Cadastro de Bairros\n2 - Cadastro de Sensores\n3 - Registro de ocorrências\n4 - Geracao de chamados\n5 - Gerenciamento de Equipes\n0 - Voltar  ao menu anterior\n");
                    scanf("%d", &escFuncionalidades);
                    getchar();

                    switch(escFuncionalidades)
                    {
                        case 1://funcoes bairros  
                        {
                            printf("\n1 - Cadastrar bairro\n2 - Buscar bairro\n3 - Listar todos os bairros\n4 - Remover um Bairro por código\n0 - Voltar ao menu anterior\n");
                            scanf("%d", &escEntradaManual);
                            getchar();

                            switch(escEntradaManual)
                            {
                                case 1:
                                {
                                    do{
                                        printf("\nDigite o codigo que o bairro vai ter: ");
                                        scanf("%d", &codBairro);
                                        getchar(); 

                                        if (codBairro <= 0)
                                            printf("\nNao pode fazer cadastro com codigo negativo ou igual a 0!");
                                    }while (codBairro <= 0 );

                                    if (verificaBairro(listaBairro, codBairro))
                                    {
                                        printf("\nJa existe bairo com esse codigo!");
                                        break;
                                    }

                                    printf("\nDigite o nome do Bairro: ");
                                    fgets(nomeBairro, 50, stdin);
                                    nomeBairro[strcspn(nomeBairro, "\n")] = '\0';
                                    
                                    if(verificaBairroNome(listaBairro, nomeBairro))
                                    {
                                        printf("\nJa existe bairro com esse nome!");
                                        break;
                                    }
                                    insereBairro(&listaBairro, codBairro, nomeBairro, &flagFuncoes);
                                    
                                    if (flagFuncoes == 1)
                                    {
                                        printf("\nBairro cadastrado");
                                        salvaSistema(listaBairro, listaEquipe);
                                    }

                                    
                                    else 
                                        printf("\nErro! Bairro nao cadastrado.");
                                }break;


                                case 2:
                                {
                                    if (listaBairro == NULL)
                                    {
                                        printf("\nNenhum bairro cadastrado!");
                                        break;
                                    }
                                    do{
                                        printf("\nDigite o codigo do bairro para buscar: ");
                                        scanf("%d", &codBairro);
                                        getchar();

                                        if(codBairro <= 0)
                                            printf("\nDigite um codigo maior que 0!");

                                    }while(codBairro <= 0);

                                    enderecoBairro = verificaBairro(listaBairro, codBairro);

                                    if (enderecoBairro == NULL)
                                    {
                                        printf("\nNao existe bairro com esse codigo!");
                                        break;
                                    }

                                    printf("\n  ---------------------------------");
                                    printf("\nBairro %s - cod: %d", enderecoBairro->nome, enderecoBairro->codigo);
                                    printf("\nQuantidade de ocorrências: %d", enderecoBairro->quantidadeOcorrencias);
                                    printf("\nQuantidade de sensores: %d", enderecoBairro->quantidadeSensores);
                                    printf("\n  ---------------------------------");
                                    enderecoBairro = NULL;
                                } break;


                                case 3:
                                {
                                    funListaBairro(listaBairro); //Faz a verificacao se a lista de bairros esta vazia na funcao
                                }break;


                                case 4:
                                {
                                    if (listaBairro == NULL)
                                    {
                                        printf("\nNenhum bairro cadastrado!");
                                        break;
                                    }
                                    do{
                                        printf("\nDigite o codigo do bairro para buscar: ");
                                        scanf("%d", &codBairro);
                                        getchar();

                                        if(codBairro <= 0)
                                            printf("\nDigite um codigo maior que 0!");

                                    }while(codBairro <= 0);

                                    removeBairroCodigo(&listaBairro, codBairro, &flagFuncoes);

                                    if (flagFuncoes == 1)
                                    {
                                        printf("\nBairro %d removido com sucesso!", codBairro);
                                        salvaSistema(listaBairro, listaEquipe);
                                    }

                                    else
                                        printf("\nNenhum bairro foi removido!");

                                }break;


                                case 0:
                                {
                                    printf("\nVoltando ao menu anterior...\n");
                                }break;


                                default:
                                {
                                    printf("\nDigite uma opcao valida!");
                                }break;
                            }
                        }break;

                    
                        case 2: //funcoes sensores
                        {
                            printf("\n1 - Cadastrar sensor\n2 - Alterar status\n3 - Buscar por codigo\n4 - Listar sensores por Bairro\n");
                            scanf("%d", &escEntradaManual);
                            getchar();

                            switch(escEntradaManual)
                            {
                                case 1:
                                {
                                    if (listaBairro == NULL)
                                    {
                                        printf("\nCadastre algum bairro antes de inserir um sensor!\n");
                                        break;
                                    }  
                                    do{
                                        printf("\nDigite o codigo do bairro em que o sensor vai ser adicionado: ");
                                        scanf("%d", &codBairro);
                                        getchar();

                                        if(codBairro <= 0)
                                            printf("\nDigite um codigo maior que 0!");

                                    } while(codBairro <= 0);


                                    if (verificaBairro(listaBairro, codBairro) == NULL)
                                    {
                                        printf("\nNao existe bairro com esse codigo");
                                        break;
                                    }
                                    do{
                                        printf("\nDigite o codigo do Sensor a ser adicionado: ");
                                        scanf("%d", &codSensor);
                                        getchar();

                                        if (codSensor < 1)
                                            printf("\nNao pode registrar sensor com codigo negativo");
                                    }while(codSensor < 1 );


                                    if (verificaSensorGlobal(listaBairro, codSensor))
                                    {
                                        printf("\nJa existe sensor com esse codigo!");
                                        break;
                                    }
                                    do{
                                        printf("\nDigite o tipo do sensor:\n1 - Temperatura\n2 - Enchente\n3 - Fumaca\n4 - Transito\n5 - Iluminacao_publica\n");
                                        scanf("%d", &tipoSensor);
                                        getchar();
                                    }while(tipoSensor < 1 || tipoSensor > 5);

                                    do{
                                        printf("\nDigite o status do sensor:\n1 - Ativo\n2 - Manutencao\n3 - Offline\n");
                                        scanf("%d", &statusSensor);
                                        getchar();
                                    }while(statusSensor < 1 || statusSensor > 3);

                                    insereSensor(listaBairro, codBairro, codSensor, tipoSensor, statusSensor, &flagFuncoes);
                                    
                                    if (flagFuncoes == 1)
                                    {
                                        printf("\nSensor Registrado!");
                                        salvaSistema(listaBairro, listaEquipe);
                                    }
                                    else
                                        printf("\nErro! Sensor nao registrado");
                                }break;


                                case 2:
                                {
                                    if (listaBairro == NULL)
                                    {
                                        printf("\nCadastre algum bairro antes de alterar o status de um sensor!\n");
                                        break;
                                    }  
                                    do{
                                        printf("\nDigite o codigo do sensor que vai ser mudado o status: ");
                                        scanf("%d", &codSensor);
                                        getchar();

                                        if (codSensor < 1)
                                            printf("\nNao existe sensor com codigo negativo");
                                    } while (codSensor < 1);

                                    enderecoSensor = verificaSensorGlobal(listaBairro, codSensor);

                                    if(enderecoSensor == NULL)
                                    {
                                        printf("\nNao existe sensor com esse codigo!");
                                        break;
                                    }

                                    do{
                                        printf("\nDigite o status do sensor:\n1 - Ativo\n2 - Manutencao\n3 - Offline\n");
                                        scanf("%d", &statusSensor);
                                        getchar();
                                    }while(statusSensor < 1 || statusSensor > 3);

                                    alteraStatusSensor(listaBairro, codSensor, statusSensor, &flagFuncoes);

                                    if (flagFuncoes == 1)
                                    {
                                        printf("\nStatus alterado!");
                                        salvaSistema(listaBairro, listaEquipe);

                                        if(statusSensor == 3)
                                        {
                                            int codChamadoAutomatico = 9000 + codSensor;
                                            if(verificaChamado(listaEquipe, codChamadoAutomatico) == NULL)
                                            {
                                                Chamado *novoChamado = alocaChamado(codChamadoAutomatico, 3, 1, NULL);
                                                inserirChamadoEquipe(listaEquipe, enderecoSensor->tipo, novoChamado, &flagFuncoes);

                                                printf("\nSensor %d ficou offline! Chamado %d gerado para a equipe de especialidade %d.", codSensor, codChamadoAutomatico, enderecoSensor->tipo);

                                            }
                                        }
                                    }
                                    
                                    else
                                        printf("\nErro! Status nao alterado");

                                    enderecoSensor = NULL;

                                }break;


                                case 3:
                                {
                                    if (listaBairro == NULL)
                                    {
                                        printf("\nCadastre algum bairro antes de buscar um sensor!\n");
                                        break;
                                    }  
                                    do{
                                        printf("\nDigite o codigo do sensor que vai ser buscado: ");
                                        scanf("%d", &codSensor);
                                        getchar();

                                        if (codSensor < 1)
                                            printf("\nNao existe sensor com codigo negativo");
                                    } while(codSensor < 1);

                                    enderecoSensor = verificaSensorGlobal(listaBairro, codSensor);

                                    if (enderecoSensor == NULL)
                                    {
                                        printf("\nNao existe sensor com esse codigo!");
                                        break;
                                    }

                                    listaSensor(enderecoSensor);
                                    enderecoSensor = NULL;
                                }break;

                                case 4:
                                {
                                    if (listaBairro == NULL)
                                    {
                                        printf("\nCadastre algum bairro antes de listar seus sensores!\n");
                                        break;
                                    }  
                                    do{
                                        printf("\nDigite o codigo do bairro para ser listado: ");
                                        scanf("%d", &codBairro);
                                        getchar();

                                        if(codBairro <= 0)
                                            printf("\nDigite um codigo maior que 0!");
                                    }while(codBairro <= 0);
                                    
                                    if (verificaBairro(listaBairro, codBairro) == NULL)
                                    {
                                        printf("\nNao existe bairro com esse codigo");
                                        break;
                                    }
                                    listaSensoresPorBairro(listaBairro, codBairro, &flagFuncoes);
                                }break;


                                case 0:
                                {
                                    printf("\nVoltando ao menu anterior...\n");
                                }break;

                                
                                default:
                                {
                                    printf("\nDigite uma opcao valida!");
                                }break;
                            }
                        }break;


                        case 3://Funcoes ocorrencias
                        {
                            if(flagCarregarArquivos == 0)
                            {
                                printf("\nCArregue os dados (opcao 1) antes!");
                                break;
                            }

                            
                            if(listaBairro == NULL)
                            {
                                printf("\nCadastre um bairro e um sensor antes!");
                                break;
                            }

                            printf("\nDigite o codigo do bairro: ");
                            scanf("%d", &codBairro);

                            enderecoBairro = verificaBairro(listaBairro, codBairro);

                            if(enderecoBairro == NULL)
                            {
                                printf("\nBairro nao encontrado!");
                                break;
                            }

                            printf("\nDigite o codigo do sensor: ");
                            scanf("%d", &codSensor);

                            enderecoSensor = verificaSensor(enderecoBairro->listaSensores, codSensor);

                            if(enderecoSensor == NULL)
                            {
                                printf("\nSensor nao encontrado neste bairro!");
                                break;
                            }


                            printf("\nDigite o codigo da ocorrencia: ");
                            scanf("%d", &codOcorrencia);
                            if (verificaOcorrenciaGlobal(listaBairro, codOcorrencia))
                            {
                                printf("\nJa existe uma ocorrencia com esse codigo!");
                                break;
                            }

                            do {
                                printf("\n1 - Baixa \n2 - Media \n3 - Alta \n4 - Critica \nDigite a severidade: ");
                                scanf("%d", &severidade);
                                getchar();
                            } while (severidade < 1 || severidade > 4);

                            printf("\nDigite a descricao: ");
                            fgets(descOcorrencia, 100, stdin);
                            descOcorrencia[strcspn(descOcorrencia, "\n")] = '\0';

                            statusOcorrencia = 1; //Vai ser 1 por a ocorrencia foi registrada e ficara em aberto, mudando conforme chamado

                            insereOcorrencia(&listaBairro, codBairro, &(enderecoSensor->listaOcorrencias), codOcorrencia, severidade, descOcorrencia, statusOcorrencia, &flagFuncoes);
                            enderecoBairro = NULL;

                            if(flagFuncoes == 1)
                            {
                                printf("\nOcorrencia registrada com sucesso!");
                                salvaSistema(listaBairro, listaEquipe);

                                if(severidade == 4 && statusOcorrencia == 1)
                                {
                                    int codChamadoAutomatico = 7000 + codOcorrencia;
                                    if(verificaChamado(listaEquipe, codChamadoAutomatico) == NULL)
                                    {
                                        Ocorrencia *ocorrenciaReal = verificaOcorrencia(enderecoSensor->listaOcorrencias, codOcorrencia);
                                        Chamado *novoChamado = alocaChamado(codChamadoAutomatico, 3, 1, ocorrenciaReal);
                                        inserirChamadoEquipe(listaEquipe, enderecoSensor->tipo, novoChamado, &flagFuncoes);

                                        if(flagFuncoes == 1 && ocorrenciaReal != NULL)
                                        {
                                            ocorrenciaReal->chamado = novoChamado;
    

                                            int flagMudaStatus;
                                            alteraStatusSensor(listaBairro, codSensor, 2, &flagMudaStatus);

                                            printf("\nOcorrencia critica detectada! Chamado %d gerado para a equipe %d. Sensor %d movido para Manutencao", codChamadoAutomatico, enderecoSensor->tipo, codSensor);
                                        }

                                        else
                                            free(novoChamado);
                                    }
                                }
                            }

                            else
                                printf("\nErro! Status nao alterado");

                            enderecoBairro = NULL;
                            enderecoSensor = NULL;

                        }break;


                        case 4: //gerar chamados
                        {
                            printf("\n1 - Gerar chamado por ocorrencia \n2 - Finalizar chamado \n0 - Voltar ao menu anterior\n");
                            scanf("%d", &escEntradaManual);
                            getchar();

                            switch(escEntradaManual)
                            {
                                case 1:
                                {
                                    if(listaBairro == NULL || listaEquipe == NULL)
                                    {
                                        printf("\nCertifique se existem bairros, sensores e equipes no sistema");
                                        break;
                                    }

                                    do{
                                        printf("Digite o codigo do novo chamado: ");
                                        scanf("%d", &codChamado);
                                        getchar();

                                        if(codChamado <= 0)
                                            printf("\nDigite um codigo maior que 0!");
                                    
                                    }while(codChamado <= 0);

                                    if(verificaChamado(listaEquipe, codChamado) != NULL)
                                    {
                                        printf("\nJa existe um chamado com esse codigo no sistema!");
                                        break;
                                    }

                                    do{
                                        printf("\nDigite o codigo da ocorrencia vinculada: ");
                                        scanf("%d", &codOcorrencia);
                                        getchar();

                                        if(codOcorrencia <= 0)
                                            printf("\nDigite um codigo maior que 0!");

                                    }while(codOcorrencia <= 0);

                                    do{
                                        printf("\nDigite a prioridade do chamado (1 a 4): ");
                                        scanf("%d", &prioridade);
                                        getchar();
                                    
                                    }while(prioridade < 1 || prioridade > 4);

                                    do{
                                        printf("\nDigite o status do sensor correspondente (1 a 3): ");
                                        scanf("%d", &statusSensor);
                                        getchar();

                                    }while(statusSensor < 1 || statusSensor > 3);

                                    gerarChamado(listaEquipe, listaBairro, codChamado, codOcorrencia, prioridade, statusSensor, &flagFuncoes);

                                    if(flagFuncoes == 1)
                                    {
                                        printf("\nChamado gerado com sucesso!");

                                        salvaSistema(listaBairro, listaEquipe);
                                    }

                                    else
                                        printf("\nErro ao gerar chamado.");
                                }break;

                                case 2: //finalizar chamado
                                {
                                    if(listaEquipe == NULL)
                                    {
                                        printf("\nNenhuma equipe cadastrada no sistema!");
                                        break;
                                    }

                                    printf("\nDigite o codigo do chamado: ");
                                        scanf("%d", &codChamado);
                                        getchar();

                                        if(verificaChamado(listaEquipe, codChamado) == NULL)
                                        {
                                            printf("\nChamado nao encontrado");
                                            break;
                                        }

                                        finalizarChamado(listaEquipe, codChamado, &flagFuncoes);

                                        if(flagFuncoes == 1)
                                        {
                                            printf("\nChamado finalizado com sucesso!");
                                            salvaSistema(listaBairro, listaEquipe);
                                        }

                                        else
                                            printf("\nChamado nao localizado na base de dados.");
                                }break;

                                case 0:
                                {
                                    printf("\nVoltando ao menu anterior...\n");
                                }break;


                                default:
                                {
                                    printf("\nDigite uma opcao valida");
                                }break;
                            }
                        }break;


                        case 5: //gerenciar equipes
                        {
                            printf("\n1 - Cadastrar nova equipe tecnica\n2 - Buscar equipe por codigo\n3 - Associar chamado a outra equipe\n0 - Voltar ao menu anterior \n");
                            scanf("%d", &escEntradaManual);
                            getchar(); 

                            switch(escEntradaManual)
                            {
                                case 1:
                                {
                                    do{
                                        printf("\nDigite o codigo da nova equipe: ");
                                        scanf("%d", &codChamado);
                                        getchar();

                                        if(codChamado <= 0)
                                            printf("\nDigite um codigo maior que 0!");
                                    
                                    }while(codChamado <= 0);

                                    if(buscarEquipe(listaEquipe, codChamado) != NULL)
                                    {
                                        printf("\nJa existe uma equipe com esse codigo!");
                                        break;
                                    }

                                    printf("\nDigite o nome da equipe: ");
                                    fgets(nomeBairro, 50, stdin);
                                    nomeBairro[strcspn(nomeBairro, "\n")] = '\0';

                                    do{
                                        printf("\n1 - Temperatura \n2 - Enchente \n3 - Fumaca \n 4 - Transito \n 5 - Iluminacao: ");
                                        scanf("%d", &tipoSensor);
                                        getchar();

                                    }while(tipoSensor < 1 || tipoSensor > 5);

                                    Equipe *novaEquipe = alocaEquipe(codChamado, nomeBairro, tipoSensor);
                                    if(novaEquipe != NULL)
                                    {
                                        inserirEquipe(&listaEquipe, novaEquipe, &flagFuncoes);

                                        if(flagFuncoes == 1)
                                        {
                                            printf("\nEquipe %s cadastrada com sucesso!", nomeBairro);
                                            salvaSistema(listaBairro, listaEquipe);
                                        }

                                        else
                                            free(novaEquipe);
                                    }
                                }break;

                                case 2: //buscar equiep
                                {
                                    if(listaEquipe == NULL)
                                    {
                                        printf("\nNenhuma equipe cadastrada!");
                                        break;
                                    }

                                    printf("\nDigite o codigo da equipe: ");
                                    scanf("%d", &codChamado);
                                    getchar();

                                    Equipe *achouEqp = buscarEquipe(listaEquipe, codChamado);
                                    if(achouEqp == NULL)
                                        printf("\nEquipe nao encontrada!");

                                    else
                                    {
                                        char nomeEspecialidade[30];

                                        switch (achouEqp->especialidade) 
                                        {
                                            case 1: strcpy(nomeEspecialidade, "Temperatura"); break;
                                            case 2: strcpy(nomeEspecialidade, "Enchente"); break;
                                            case 3: strcpy(nomeEspecialidade, "Fumaca"); break;
                                            case 4: strcpy(nomeEspecialidade, "Transito"); break;     
                                            case 5: strcpy(nomeEspecialidade, "Iluminacao Publica"); break;                                 
                                        }
                                        printf("\nEquipe: %s | Codigo: %d", achouEqp->nome, achouEqp->codigo);
                                        printf("\nEspecialidade: %s", nomeEspecialidade);
                                        printf("\nTotal de atendimentos concluidos: %d", achouEqp->totalAtendimentos);
                                    }
                                }break;

                                case 3: //associar chamado
                                {
                                    if(listaEquipe == NULL)
                                    {
                                        printf("\nNenhuma equipe cadastrada!");
                                        break;
                                    }

                                    printf("\nDigite o codigo do chamado: ");
                                    scanf("%d", &codChamado);

                                    printf("\nDigite o codigo da equipe de destino: ");
                                    scanf("%d", &codSensor);
                                    getchar();

                                    associarEquipe(listaEquipe, codChamado, codSensor, &flagFuncoes);

                                    if(flagFuncoes == 1)
                                        salvaSistema(listaBairro, listaEquipe);

                                    else
                                        printf("\nErro ao associar equipe");

                                }break;


                                case 0:
                                {
                                    printf("\nVoltando ao menu anterior...\n");
                                }break;

                                default:
                                {
                                    printf("\nDigite uma opcao valida");
                                }break;

                            }
                        }break;
                        
                        
                        case 0:
                        {
                            printf("\nVoltando ao menu anterior...\n");
                        }break;

                        
                        default:
                        {
                            printf("\nDigite uma opcao valida");
                        }break;
                    }
                }while(escFuncionalidades != 0);
                
            }break;

            case 3:
            {
                if(flagCarregarArquivos == 0)
                {
                    printf("\nCarregue os dados (opcao 1) antes! ");
                    break;
                }    
                                
                executarSimulacao(&listaBairro, &listaEquipe);                
            }break;

            case 0:
            {
                //Desaloca todos os ponteiros, a parte de salvamento é dentro das funções
                if (flagCarregarArquivos == 1)
                    salvaSistema(listaBairro, listaEquipe);
                gerarRelatorio(listaBairro, listaEquipe);
                liberarMemoria(&listaBairro, &listaEquipe);
            }break; 

            default:
            {
                printf("\nDigite uma opção válida!");
            }break;

        }

    }while (escMenu != 0);

}

