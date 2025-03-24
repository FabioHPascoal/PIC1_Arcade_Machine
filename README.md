
# Arcade Machine

A ideia central do projeto é construir uma máquina fliperama simplificada, com três jogos
clássicos programados a partir de um Arduino Uno, de modo que seja possível alternar a qualquer momento o jogo em execução através de um input do usuário.

## Descrição detalhada

O presente projeto busca descrever o funcionamento de uma máquina fliperama simplificada, com três jogos programados em sua memória: Tetris, Snake, e Frogger. A máquina deve contar com um Arduino Uno, ou qualquer outro microcontrolador com memória e pinos suficientes para executar o programa, uma matriz de leds RGBs 8x32, funcionando como monitor, um joystick para controle do jogador, dois botões para uso no jogo Tetris, pois este exige a rotação das peças em sentido horário e anti-horário, um switch tipo gangorra de três estados, para seleção entre os três jogos, e um display de 7 segmentos de quatro dígitos, para a exibição da pontuação do usuário. O esquemático do projeto é exibido a seguir, contendo todos os componentes a serem utilizados, bem como suas conexões.

## Vídeo do projeto em execução

https://www.youtube.com/watch?v=Vh4_vdK3K6M

## Componentes utilizados

Seguem os componentes eletrônicos exibidos no esquemático:

### Arduino Uno: 

Foi utilizado como microcontrolador um Arduino Uno, pois essa escolha facilitaria o trabalho com os compoenentes que operam apenas em 5 V, como a matrix de leds. No entanto, pelo fato de a memória do Arduino ser relativamente pequena, muitas otimizações foram necessárias a fim de comportar os três jogos no espaço limitado. Mas caso outros microcontroladores sejam utilizados, basta limitar a tensão de entrada com divisores de tensão.

### Matriz de leds 8x32:

Como monitor para os jogos, utilizou-se uma matriz de leds 8x32, que nada mais é do que uma fita led organizada em forma retangular. E pela matriz possuir um total de 256 leds, dependendo do estado em que se encontra o jogo, bastante potência é exigida, e por isso é altamente recomendado alimentá-la externamente com uma fonte de 5 V capaz de trabalhar com correntes de pelo menos 1,5 A. Para utilizar esse componente, certifique-se de que os pinos de alimentação e referência estejam conectados, e que o pino de dados esteja corretamente ligado ao Arduino.

### Joystick SANWA

Como os jogos desenvolvidos exigem algum tipo de controle, optou-se pela utilização de um joystick SANWA, pois dessa forma o projeto estaria mais de acordo com as máquinas fliperama tradicionais, além de ser bem melhor de controlar do que a alternativa de utilizar quatro botões para movimentação. Para conexão com o Arduino, conectam-se os quatro pinos existentes (referentes às quatro possíveis posições do stick) no microcontrolador, além de conectar o quinto pino (GND), ao negativo do Arduino. Caso resistores externos não sejam utilizados, declare INPUT_PULLUP na inicialização dos pinos para utilização dos resistores internos do Arduino.

### Botões SANWA

Os dois botões no diagrama possuem a função de rotacionar as peças nos dois possíveis sentidos no jogo tetris. A ligação é bem simples, cada um deles possui duas saídas, uma delas deve ir no negativo do Arduino, e a outra deve ir até algum pino de dados. O mesmo comentário sobre o uso de resistores pull-up vale aqui também.

### Switch tipo gangorra

Como existem três opções de jogos a serem selecionados, um switch tipo gangorra se mostrou uma ótima alternativa, mas qualquer outra alternativa para seleção poderia ter sido aplicada. O pino do meio deve ser conectado ao negativo do Arduino, e os dois das extremidades devem ser ligados a pinos de dados. O mesmo comentário sobre o uso de resistores pull-up vale aqui também.

### TM1637 - Display de 7 segmentos de 4 dígitos

Para pontuação do jogador nos três jogos, foi utilizado um display de 7 segmentos de 4 dígitos. O componente TM1637 utilizado possui uma entrada de alimentação 5 V, que idealmente deve ser ligada na fonte externa, um pino GND para ligação com o negativo do Arduino, e dois pinos (clock e dio), para controle dos dados exibidos, que devem ser ligados a pinos de dados do Arduino.
## Dependências

Adafruit NeoPixel - Versão 1.12.5, para controle da matriz de leds;
TM1637 - Versão 1.2.0, para controle do display de 7 segmentos de 4 dígitos.