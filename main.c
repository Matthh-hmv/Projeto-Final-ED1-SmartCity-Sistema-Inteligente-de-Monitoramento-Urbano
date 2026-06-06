#include "prototipos.h"

void main()
{
    Bairro *listaBairros = NULL;
    Equipe *listaEquipe = NULL;

    int escMenu, escEntradaManual;
    printf("Bem vindo ao sistema de controle de SmartCity's\n");
    do {
        printf("\n1 - Carregar arquivos \n2 - Entrada manual de Dados\n3 - Simulação via arquivo de entrada\n0 - Sair do programa");
        scanf("%d", &escMenu);
        switch(escMenu)
        {
            case 1:
            {
                /*Aqui vai ser onde o sistema vai carregar os arquivos que guardam as informações, pensar na possibilidade de 
                fazer escolhas para o que carregar ou se vamos carregar todos os arquivos de uma vez*/

                //acho melhor carregar tudo de uma vez

                if(listaBairros == NULL)
                {
                    carregarBairros(&listaBairros);
                    carregarEquipes(&listaEquipe);

                    carregarSensores(listaBairros);
                    carregarOcorrencias(listaBairros);
                    carregarChamados(listaBairros, listaEquipe);
                }

                else
                    printf("Arquivos ja carregados!\n");
            }break;

            case 2:
            {
                if(listaBairros == NULL)
                    printf("Carregue os dados (opcao 1) antes! \n");

                else
                {
                    printf("\n1 - Inserir bairro, 2 - Buscar bairro, 3 - Listar todos os bairros, 4 - Remover um Bairro pro código ");
                    //Funções dos sensores
                    //Funções das ocorrencias
                    //Funções das equipes
                    //Funções dos chamados
                }
                
            }break;

            case 3:
            {
                if(listaBairros == NULL)
                    printf("Carregue os dados (opcao 1) antes! \n");

                else
                {
                    //Aqui onde ele vai ler o arquivo entrada_simulacao.txt e realizar as ações
                }
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
