
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

- **projetow2040.c:**  
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

## Explicação Detalhada do Código Principal

Este código implementa um sistema utilizando o **Raspberry Pi Pico W**, combinando diferentes interfaces, incluindo **botões, LEDs RGB, matriz de LEDs WS2812, comunicação serial UART e um display SSD1306** via I2C. Ele processa entradas e exibe informações de forma interativa, enquanto também controla uma matriz de LEDs e responde a comandos externos.

### Bibliotecas Utilizadas
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "projetow2040.pio.h"
```
- **pico/stdlib.h** → Inicializa GPIOs, temporização e funções básicas do Pico.
- **hardware/uart.h** → Comunicação UART.
- **hardware/gpio.h** → Controle de GPIOs para LEDs e botões.
- **hardware/i2c.h** → Comunicação com o display OLED SSD1306 via I2C.
- **hardware/pio.h** → Controle do WS2812 via PIO (Programável IO).
- **hardware/clocks.h** → Manipulação de clocks do microcontrolador.
- **ssd1306.h e font.h** → Manipulação do display SSD1306.
- **projetow2040.pio.h** → Código PIO para controlar a matriz de LEDs WS2812.

### Definição de Pinos
```c
#define LED_G_PIN    11  // LED Verde (RGB)
#define LED_B_PIN    12  // LED Azul (RGB)
#define BUTTON_A_PIN  5  // Botão A
#define BUTTON_B_PIN  6  // Botão B
#define WS2812_PIN    7  // WS2812 (matriz 5x5)
#define I2C_SDA      14  // SDA para o display SSD1306
#define I2C_SCL      15  // SCL para o display SSD1306
#define UART_TX_PIN   0  // UART TX
#define UART_RX_PIN   1  // UART RX
```
- Define os **GPIOs** usados para LEDs, botões, comunicação I2C e UART.

### Variáveis Globais
```c
volatile bool led_g_state = false;
volatile bool led_b_state = false;
volatile uint8_t current_number = 0;
volatile bool update_matrix = false;
ssd1306_t display;
PIO pio = pio0;
uint sm;
```
- **led_g_state / led_b_state** → Estados dos LEDs RGB.
- **current_number** → Número atual exibido na matriz WS2812.
- **update_matrix** → Flag para indicar atualização da matriz de LEDs.
- **display** → Estrutura de controle do display SSD1306.
- **pio** → Identifica o PIO usado para o WS2812.
- **sm** → Máquina de estado para PIO.

### Inicialização dos WS2812
```c
void ws2812_init() {
    uint offset = pio_add_program(pio, &ws2812_program);
    sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, offset, WS2812_PIN);
}
```
- **pio_add_program()** → Adiciona o código PIO para o controle do WS2812.
- **pio_claim_unused_sm()** → Aloca uma máquina de estado disponível.
- **ws2812_program_init()** → Configura o PIO para controlar os LEDs WS2812.

### Atualização da Matriz WS2812
```c
void update_led_matrix(uint8_t number) {
    for (int i = 0; i < 25; i++) {
        uint8_t r = numbers[number][i] ? 255 : 0;
        uint32_t color = (0 << 16) | (r << 8) | 0; // Formato GRB
        pio_sm_put_blocking(pio, sm, color);
    }
    sleep_us(50);
}
```
- **Preenche a matriz 5x5** com o número atual usando o canal vermelho.
- **pio_sm_put_blocking()** → Envia os valores para os LEDs WS2812.

### Processamento de Entrada Serial
```c
void process_input(char c) {
    ssd1306_clear(&display);
    ssd1306_draw_char(&display, 0, 0, c);
    ssd1306_send_data(&display);
    printf("Recebido: %c
", c);

    if (c >= '0' && c <= '9') {
        current_number = c - '0';
        update_matrix = true;
    }
}
```
- **Exibe o caractere no display SSD1306**.
- **Se for um número (0-9), atualiza a matriz WS2812**.

### Interrupção dos Botões
```c
void gpio_callback(uint gpio, uint32_t events) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if (gpio == BUTTON_A_PIN) {
        if (now - last_time_A < 200) return;
        last_time_A = now;
        led_g_state = !led_g_state;
        gpio_put(LED_G_PIN, led_g_state);
        ssd1306_draw_string(&display, 0, 0, led_g_state ? "Verde ON" : "Verde OFF");
    }
    else if (gpio == BUTTON_B_PIN) {
        if (now - last_time_B < 200) return;
        last_time_B = now;
        led_b_state = !led_b_state;
        gpio_put(LED_B_PIN, led_b_state);
        ssd1306_draw_string(&display, 0, 0, led_b_state ? "Azul ON" : "Azul OFF");
    }
    ssd1306_send_data(&display);
}
```
- **Verifica debounce** (tempo mínimo entre pressões).
- **Alterna o estado dos LEDs RGB e exibe no display**.

### Função Principal
```c
int main() {
    stdio_init_all();
    sleep_ms(100);
    uart_init(uart0, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&display, i2c1);
    ws2812_init();

    while (1) {
        int c_usb = getchar_timeout_us(0);
        if (c_usb != PICO_ERROR_TIMEOUT) process_input((char)c_usb);
        if (uart_is_readable(uart0)) process_input(uart_getc(uart0));
        if (update_matrix) {
            update_led_matrix(current_number);
            update_matrix = false;
        }
        sleep_ms(40);
    }
    return 0;
}
```
- **Inicializa UART, I2C, botões e LEDs**.
- **Processa entradas de UART e USB**.
- **Atualiza a matriz WS2812 quando necessário**.

---

## Referências
- [Raspberry Pi Pico SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Documentação do RP2040](https://www.raspberrypi.com/documentation/microcontrollers/rp2040.html)

---

## Demonstração em Vídeo
Para conferir o projeto em funcionamento, acesse:
[Link do Vídeo](#) *(Em breve)*

Se este projeto foi útil para você, **considere deixar uma estrela no repositório GitHub!** 🌟

**Contato:**  
📧 Email: marlon.garrido@univasf.edu.br  
🌐 Site: [UNIVASF](https://www.univasf.edu.br/)

