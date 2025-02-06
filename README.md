Segue abaixo um exemplo de README.md com uma linguagem profissional, detalhando os objetivos, funcionalidades e explicações sobre as principais funções do projeto:

---

# Comunicação Serial com Raspberry Pico W 2040, UART, SPI e I2C  
**Display SSD1328, Matriz 5x5 de LEDs WS2812, Lâmpadas RGB e Acessórios**

---

## Autor do Projeto  
**Dr. Marlon da Silva Garrido**  
Professor Associado IV (CENAMB - PPGEA)  
Universidade Federal do Vale do São Francisco (UNIVASF)

---

## Contexto e Objetivos

Este projeto foi desenvolvido no âmbito das atividades do EMBARCATECH 2024/25 e tem como propósito demonstrar a integração de hardware e software em microcontroladores, utilizando o Raspberry Pico W 2040. Entre os objetivos específicos estão:

- **Comunicação Serial e Protocolos:**  
  - Compreender e aplicar UART, SPI e I2C para transmissão e recepção de dados.
  
- **Controle de Periféricos:**  
  - Manipular LEDs comuns (RGB) e LEDs endereçáveis (WS2812) para criação de efeitos visuais.
  
- **Uso de Interrupções e Debouncing:**  
  - Implementar o tratamento de botões com rotinas de interrupção (IRQ) e debouncing via software para garantir a integridade dos sinais.
  
- **Integração de Componentes:**  
  - Demonstrar a aplicação prática dos conceitos estudados, combinando comunicação serial, controle de display e dispositivos de iluminação.

---

## Descrição do Projeto

O projeto integra os seguintes componentes conectados à placa BitDogLab (ou Raspberry Pico W 2040):

- **Matriz 5x5 de LEDs WS2812:**  
  Conectada à GPIO 7, esta matriz exibe números (0 a 9) por meio de um mapeamento específico de LEDs.

- **LED RGB:**  
  Com os pinos ligados às GPIOs 11 (Verde), 12 (Azul) e 13 (Vermelho). No código, a interação foca na alternância dos LEDs Verde e Azul, enquanto o LED Vermelho pode ser utilizado para indicadores de status ou efeitos.

- **Botões A e B:**  
  Conectados às GPIOs 5 e 6, respectivamente, utilizando resistores de pull-up internos. Estes botões acionam rotinas de interrupção que alternam os estados dos LEDs e atualizam a exibição no display.

- **Display OLED SSD1328:**  
  Conectado via I2C (GPIOs 14 e 15), responsável por exibir caracteres enviados pelo usuário e mensagens de status das operações.

---

## Funcionalidades do Projeto

1. **Modificação da Biblioteca font.h:**  
   - Expansão do conjunto de caracteres para incluir letras minúsculas, permitindo maior flexibilidade na exibição de mensagens.

2. **Entrada de Caracteres via Serial:**  
   - Os caracteres digitados pelo usuário (através do Serial Monitor do VS Code) são exibidos individualmente no display OLED.  
   - Se o caractere for um dígito (0-9), um padrão correspondente é exibido na matriz WS2812.

3. **Interação com Botões:**  
   - **Botão A:** Ao ser pressionado, alterna o estado do LED RGB Verde. A mudança é registrada tanto no display quanto via comunicação serial.  
   - **Botão B:** Ao ser acionado, alterna o estado do LED RGB Azul, com a operação igualmente registrada no display e via serial.

4. **Controle dos LEDs WS2812:**  
   - A matriz de LEDs WS2812 apresenta números em um formato 5x5, definido por um array de padrões que acende ou apaga os LEDs de acordo com o dígito recebido.

---

## Requisitos Técnicos

- **Uso de Interrupções:**  
  Todas as operações relacionadas aos botões são implementadas via IRQ, garantindo resposta imediata e confiável.

- **Debouncing via Software:**  
  Implementação de um mecanismo para evitar múltiplas leituras devido ao bouncing dos botões.

- **Controle de LEDs:**  
  Demonstração do domínio na manipulação de LEDs comuns e endereçáveis, utilizando tanto GPIOs padrão quanto PIO para os WS2812.

- **Comunicação I2C e UART:**  
  Utilização do display OLED (via I2C) para exibição e da UART para comunicação serial, evidenciando a integração dos protocolos de comunicação.

- **Organização do Código:**  
  O código é estruturado e comentado de forma clara, facilitando a compreensão e a manutenção.

---

## Diagrama Simplificado de Conexões

```
+-----------------------------------------------------+
|                Placa BitDogLab / Pico               |
|                                                     |
| GPIO 7   -----------------------> Matriz WS2812 (5x5) |
| GPIO 11  -----------------------> LED RGB (Verde)      |
| GPIO 12  -----------------------> LED RGB (Azul)       |
| GPIO 13  -----------------------> LED RGB (Vermelho)   |
| GPIO 5   -----------------------> Botão A (Pull-up)      |
| GPIO 6   -----------------------> Botão B (Pull-up)      |
| GPIO 14  -----------------------> SDA (Display OLED)     |
| GPIO 15  -----------------------> SCL (Display OLED)     |
+-----------------------------------------------------+
```
*Observação:* Embora o LED RGB possua três canais, o foco deste exemplo está nas interações dos canais verde e azul.

---

## Como Executar o Projeto

1. **Clonar o Repositório:**
   ```bash
   git clone https://github.com/seu-usuario/ProjetoW2040.git
   cd ProjetoW2040
   ```

2. **Configurar o Ambiente de Desenvolvimento:**
   - Instale o Raspberry Pico SDK ou configure um ambiente compatível com o microcontrolador.
   - Certifique-se de que todas as dependências (como bibliotecas para I2C, UART e PIO) estão corretamente instaladas.

3. **Compilar o Projeto:**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Transferir o Firmware:**
   - Conecte a placa (Raspberry Pico) ao computador.
   - Copie o arquivo `.uf2` gerado para o volume USB do dispositivo.

5. **Testar e Verificar:**
   - Abra o Serial Monitor no VS Code para enviar caracteres.
   - Teste a funcionalidade dos botões A e B e observe a atualização dos LEDs e do display.

---

## Estrutura do Código

- **projetow2040.pio.h:**  
  Contém o programa PIO para controle dos LEDs WS2812, gerado a partir do assembler do PIO.

- **main.c:**  
  Responsável por:
  - Configurar os GPIOs, UART, I2C e PIO.
  - Definir as rotinas de interrupção para os botões.
  - Implementar funções como `update_led_matrix()` e `process_input()`, que atualizam, respectivamente, a matriz de LEDs e o display OLED.
  - Gerenciar o loop principal, que monitora a entrada serial e atualiza os dispositivos conforme as interações.

### Explicação de Funções-Chave

- **ssd1306_init():**  
  Inicializa o display OLED, configurando dimensões, alocando o buffer e enviando a sequência de comandos via I2C para preparação do display.

- **ssd1306_draw_char() / ssd1306_draw_string():**  
  Funções responsáveis por desenhar caracteres e strings no display. Elas utilizam as fontes definidas na biblioteca `font.h` e atualizam o buffer do display.

- **update_led_matrix():**  
  Atualiza a matriz WS2812 com o padrão correspondente ao dígito recebido, enviando os dados através do PIO.

- **gpio_callback():**  
  Rotina de interrupção para os botões A e B, que alterna os estados dos LEDs RGB e atualiza o display com mensagens de status.

---

## Trecho de Código

```c
// Exemplo de mapeamento dos dígitos (0-9) em uma matriz 5x5
const uint32_t numbers[10][25] = {
    {1,1,1,1,1,  1,0,0,0,1,  1,0,0,0,1,  1,0,0,0,1,  1,1,1,1,1}, // 0
    {0,0,1,0,0,  0,0,1,0,0,  0,0,1,0,0,  0,0,1,0,0,  0,0,1,0,0}, // 1
    // ... demais dígitos ...
};

void update_led_matrix(uint8_t number) {
    for (int i = 0; i < 25; i++) {
        uint8_t r = numbers[number][i] ? 255 : 0;  
        uint32_t color = (0 << 16) | (r << 8) | 0; // Configura o canal vermelho (formato GRB)
        pio_sm_put_blocking(pio0, 0, color);
    }
    sleep_us(50);
}
```

---

## Referências

- [Raspberry Pi Pico SDK](https://www.raspberrypi.com/documentation/microcontrollers/)
- Documentação oficial do microcontrolador RP2040
- Tutoriais e guias sobre comunicação serial, controle de LEDs WS2812 e uso de interrupções em microcontroladores

---

## Demonstração em Vídeo

Assista à demonstração completa do projeto, com explicações detalhadas, integração com VSCode, simulação via Wokwi e testes práticos na placa BitDogLab:  
[Link do Vídeo](#) *(Insira o link do vídeo aqui)*

