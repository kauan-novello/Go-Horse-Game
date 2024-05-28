# keyboard.h

keyboardInit: Inicializa o teclado para entrada não bloqueante e sem eco.
keyboardDestroy: Restaura as configurações originais do teclado.
keyhit: Verifica se uma tecla foi pressionada.
readch: Lê o caractere pressionado.

# screen.h

screenHomeCursor: Move o cursor para a posição inicial (0,0).
screenShowCursor: Mostra o cursor na tela.
screenHideCursor: Esconde o cursor na tela.
screenClear: Limpa a tela e move o cursor para a posição inicial.
screenUpdate: Atualiza a tela imediatamente (flusha o stdout).
screenSetNormal: Define o modo de texto para normal.
screenSetBold: Define o modo de texto para negrito.
screenSetBlink: Define o modo de texto para piscar.
screenSetReverse: Define o modo de texto para reverso.
screenBoxEnable: Ativa caracteres de caixa no terminal.
screenBoxDisable: Desativa caracteres de caixa no terminal.
screenInit: Inicializa a tela, opcionalmente desenha bordas.
screenDestroy: Limpa a tela e restaura o estado inicial.
screenGotoxy: Move o cursor para a posição (x, y).
screenSetColor: Define as cores do texto e do fundo.

# timer.h

timerInit: Inicializa o temporizador com um valor em milissegundos.
timerDestroy: Destrói o temporizador.
timerUpdateTimer: Atualiza o valor do temporizador.
timerTimeOver: Verifica se o tempo definido expirou.
timerPrint: Imprime o tempo decorrido