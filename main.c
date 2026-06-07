#include "prototipos.h"

void main()
{
    Bairro *listaBairro = NULL, enderecoBairro = NULL;
    Equipe *listaEquipe = NULL;
    Sensor *enderecoSensor = NULL;

    int escMenu, escFuncionalidades escEntradaManual, codBairro, flagCarregarArquivos = 0, flagFuncoes//Flag geral para verificar se a funcao foi bem sucedida;
    int codSensor, tipoSensor, statusSensor
    char nomeBairro[50];

    printf("Bem vindo ao sistema de controle de SmartCity's\n");
    do {
        printf("\n1 - Carregar arquivos \n2 - Entrada manual de Dados\n3 - Simulação via arquivo de entrada\n0 - Sair do programa");
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
                    carregarOcorrencias(listaBairro);
                    carregarChamados(listaBairro, listaEquipe);
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
                    printf("\n1 - Cadastro de Bairros\n2 - Cadastro de Sensores\n3 - Registro de ocorrências\n4 - Geracao de chamados\n5 - Gerenciamento de Equipes\n0 - Voltar  ao menu anterior\n");
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
                                    insereBairro(&listaBairros, codBairro, nomeBairro, &flagFuncoes);
                                    if (flagFuncoes == 1)
                                        printf("\nBairro cadastrado");
                                    else 
                                        printf("\nErro! Bairro nao cadastrado.");
                                }break;

                                case 2:
                                {
                                    if (*listaBairro == NULL)
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
                                    } while(codBairro <= 0);
                                    enderecoBairro = verificaBairro(listaBairro, codBairro);
                                    if (enderecoBairro == NULL)
                                    {
                                        printf("\nNao existe bairro com esse codigo!");
                                        break;
                                    }
                                    printf("\n=================================");
                                    printf("\nBairro %s - cod: %d", enderecoBairro->nome, enderecoBairro->codigo);
                                    printf("\nQuantidade de ocorrências: %d", enderecoBairro->quantidadeOcorrencias);
                                    printf("\nQuantidade de sensores: %d", enderecoBairro->quantidadeSensores);
                                    printf("\n=================================");
                                    enderecoBairro = NULL;
                                } break;

                                case 3:
                                {
                                    listaBairros(*listaBairro); //Faz a verificacao se a lista de bairros esta vazia na funcao
                                } break;

                                case 4:
                                {
                                    if (*listaBairro == NULL)
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
                                    } while(codBairro <= 0);
                                    removeBairroCodigo(&listaBairro, codBairro, &flagFuncoes);
                                    if (flagFuncoes == 1)
                                        printf("\nBairro %d removido com sucesso!", codBairro);
                                    else
                                        printf("\nNenhum bairro foi removido!");
                                    }

                                    case 0:
                                    {
                                        printf("\nVoltando ao menu anterior...\n");
                                    }break;

                                    default:
                                    {
                                        printf("\nDigite uma opcao valida!");
                                    }break;
                            
                                }break;
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
                                    if (*listaBairro == NULL)
                                    {
                                        printf("\nCadastre algum bairro antes de inserir um sensor\n!");
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

                                    insereSensor(*listaBairro, codBairro, codSensor, tipoSensor, statusSensor, &flagFuncoes);
                                    if (flagFuncoes == 1)
                                        printf("\nSensor Registrado!");
                                    else 
                                        printf("\nErro! Sensor nao registrado");

                                }break;

                                case 2:
                                {
                                    do{
                                        printf("\nDigite o codigo do sensor que vai ser mudado o status: ");
                                        scanf("%d", &codSensor);
                                        getchar();
                                        if (codSensor < 1)
                                            printf("\nNao existe sensor com codigo negativo");
                                    } while (codSensor < 1);

                                    if (verificaSensorGlobal(*listaBairro, codSensor) == NULL)
                                    {
                                        printf("\nNao existe sensor com esse codigo!");
                                    }
                                    do{
                                        printf("\nDigite o status do sensor:\n1 - Ativo\n2 - Manutencao\n3 - Offline\n");
                                        scanf("%d", &statusSensor);
                                        getchar();
                                    }while(statusSensor < 1 || statusSensor > 3);
                                    alteraStatusSensor(listaBairro, codSensor, statusSensor, &flagFuncoes);
                                    if (flagFuncoes == 1)
                                        printf("\nStatus alterado!");
                                    else 
                                        printf("\nErro! Status nao alterado");
                                } break;

                                case 3:
                                {
                                    do{
                                        printf("\nDigite o codigo do sensor que vai ser buscado: ");
                                        scanf("%d", &codSensor);
                                        getchar();
                                        if (codSensor < 1)
                                            printf("\nNao existe sensor com codigo negativo");
                                    } while (codSensor < 1);
                                    enderecoSensor = verificaSensorGlobal(listaBairro, codSensor);
                                    if (enderecoSensor == NULL)
                                    {
                                        printf("\nNao existe sensor com esse codigo!");
                                        break;
                                    }
                                    listaSensor(Sensor *enderecoSensor);
                                    enderecoSensor = NULL;
                                }break;

                                case 4:
                                {
                                    do{
                                        printf("\nDigite o codigo do bairro para ser listado: ");
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
                                    listaSensoresPorBairro(listaBairro, codBairro, flagFuncoes);
                                }

                                case 0:
                                {
                                    printf("\nVoltando ao menu anterior...\n");
                                }break;

                                default:
                                {
                                    printf("\nDigite uma opcao valida!");
                                }break;


                        }break;

                        case 3://Funcoes ocorrencias
                        {

                        }
                        
                        case 0:
                        {
                            printf("\nVoltando ao menu anterior...\n")
                        }
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
                
                //Aqui onde ele vai ler o arquivo entrada_simulacao.txt e realizar as ações
                
            }break;

            case 0:
            {
                //Desaloca todos os ponteiros, a parte de salvamento é dentro das funções
            }break;

            default:
            {
                printf("\nDigite uma opção válida!");
            }break;

        }

    }while (escMenu != 0);

}
