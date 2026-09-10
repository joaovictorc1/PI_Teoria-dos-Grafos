=========================================================================
Estrutura do Projeto e Arquivos
=========================================================================
Esta pasta contém a infraestrutura de dados (ETL) e o núcleo do sistema de grafos, divididos em três arquivos principais. 
A arquitetura foi pensada para isolar o tratamento de dados pesados da implementação pura em C.

1. etl_caida.py (Pipeline de Dados Oficial)

    - Objetivo: Processar o dataset real do CAIDA ITDK (Internet Topology Data Kit).

    - O que ele faz: Como os arquivos brutos do CAIDA são gigantescos e possuem dados do mundo inteiro, este script em Python atua como um filtro inteligente. 
      Ele varre o dataset procurando apenas roteadores localizados no Brasil (BR).

    - Modelagem de Pesos (Fase II): Para cada conexão (link) encontrada entre dois roteadores brasileiros, 
      o script cruza suas latitudes e longitudes e aplica a Fórmula de Haversine para calcular a distância geográfica real em quilômetros. 
      Esse valor se torna o "peso" da aresta.

    - Saída: Ele interrompe a execução ao atingir exatamente 1.000 vértices (conforme a restrição RF01 do projeto)
      e exporta os arquivos limpos vertices_br.csv e arestas_br.csv para serem consumidos pelo programa em C.

2. gerador_mock_brasil.py (Gerador de Dados Sintéticos)

    - Objetivo: Acelerar o desenvolvimento e permitir testes locais sem depender do download dos arquivos massivos do CAIDA.

    - O que ele faz: Este script simula o comportamento do pipeline oficial. Ele gera 1.000 nós com latitudes e longitudes aleatórias, 
      restritas à caixa delimitadora (bounding box) do território brasileiro, e cria conexões matemáticas entre eles (garantindo que o grafo gerado seja conexo).

    - Por que é útil? Ele exporta os CSVs exatos no mesmo formato que o etl_caida.py. 
      Isso permite que a equipe que está programando em C comece a escrever e testar os algoritmos (Tarjan, Kruskal, etc.) imediatamente,
      sem precisar configurar o dataset real na máquina de desenvolvimento.

3. main.c (Core do Sistema de Grafos)

    - Objetivo: O motor principal do projeto, onde as Fases I e II são executadas, implementado em linguagem C pura (respeitando o requisito não funcional RNF01).

    - Modelagem Estrutural: Contém a implementação autoral e do zero das estruturas de Lista de Adjacência (com alocação dinâmica de nós) 
      e Matriz de Adjacência (com array de double para armazenar o peso em km).

    - Carregamento Otimizado: Lê o arquivo arestas_br.csv de forma extremamente rápida, instanciando as ligações diretamente na memória RAM.

    - Requisitos Funcionais:

        + Já possui o mecanismo (variável usarMatriz) para alternar a execução entre Lista ou Matriz (RF02).

        + Já possui wrappers usando <sys/time.h> para medir e logar os milissegundos precisos que cada algoritmo leva para rodar (RF03).

    - Próximos Passos: O arquivo contém as assinaturas prontas (stubs) das funções que a equipe precisa desenvolver:
      encontrarArticulacoesTarjan (Fase I), executarKruskalOuPrim e vertexCoverGulosos (Fase II).
=========================================================================
INSTRUÇÕES
=========================================================================
1. Rode 'python gerador_mock_brasil.py' para criar os CSVs de teste local.
2. Compile o C: 'gcc main.c -o grafo'
3. Execute: './grafo'