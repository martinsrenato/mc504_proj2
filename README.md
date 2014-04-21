mc504_proj2
===========

Projeto para a disciplina MC504 - UNICAMP

Aluno: Renato César Martins
RA 073668

===========

Objetivo: implementar e criar uma animação para um problema presente no livro "The Little Book of Semaphores" de Allen B. Downey.

Problema abordado: Cigarette smokers problem

O problema original: 
Três fumantes aguardam ingredientes - tabaco, papel e fósforos - para fazer e fumar um cigarro. Um agente tem um estoque infinito de todos os ingredientes. Os fumantes possuem estoque infinito de um ingrediente cada, ou seja, um fumante possui tabaco, outro possui papel e o último possui fósforos. O agente periodicamente fornecerá ingredientes randomicamente, enquanto os fumantes aguardam para pegar os ingredientes faltantes e, tendo os três, preparar um cigarro e fumar.

Modificações: 
Realizei algumas mudanças no problema
1. O número de fumantes é escolhido pelo usuário
2. O número de ingredientes iniciais de cada fumante é escolhido pelo usuário.
3. Os ingredientes iniciais do usuário são randomicos.

Recomendações e Limitações:
1. É necessário maximizar o terminal, assim problemas com a animação são evitados.
2. O número de fumantes mínimo é 1 e o máximo é 5, para que a animação não fique maior que a tela. *ainda pode haver problemas em sistemas com resolução muito baixa
3. O número de ingredientes iniciais deve ser 0, 1 e 2, por conta da própria especificação do problema.

Implementação:
Existe uma thread distinta para cada fumante, bem como uma para o agente. O controle do ingrediente disponibilizado é feito através de mutexes. Enquanto um fumante pega um ingrediente, o acesso dos outros é bloquado. O agente também bloquei acesso enquanto faz a mudança do ingrediente, para evitar que fumantes interfiram durante o processo.
A animação é feita por uma outra thread, que verificará o estado dos fumantes e do agente através de variáveis globais e desenhará a situação na tela de acordo.
Por fim, outra thread escolhe frases aleatorias sobre os maleficios do cigarro. Esse foi um detalhe implementado apenas para deixar a animação mais divertida, não afetando o processo entre agente e fumantes.

Melhorias possíveis:
Algumas melhorias poderiam ser implementadas, não tendo sido feitas por questões de tempo
1. O número de agentes também poderia ser variável. Algumas adaptações no código seriam necessárias.
2. A animação poderia aproveitar melhor a tela. No formato atual está muito vertical, podendo ser estendida na horizontal, colocando alguns fumantes lado a lado, por exemplo.
3. Downey também sugere que o número de ingrediente fornecidos pelo agente poderia ser cumulativo. Caso nenhum fumante pegasse o ingrediente disponivel no momento, este continuaria disponivel mesmo após o fornecimento de um novo pelo agente.
