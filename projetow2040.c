#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"      // Biblioteca para o display SSD1306
#include "inc/font.h"         // Biblioteca de fontes (inclui caracteres maiúsculos e minúsculos)
#include "projetow2040.pio.h" // Programa PIO para WS2812

// Definição dos pinos
#define LED_G_PIN    11  // LED Verde (RGB)
#define LED_B_PIN    12  // LED Azul (RGB)
#define BUTTON_A_PIN  5  // Botão A
#define BUTTON_B_PIN  6  // Botão B
#define WS2812_PIN    7  // WS2812 (matriz 5x5)
#define I2C_SDA      14  // SDA para o display SSD1306
#define I2C_SCL      15  // SCL para o display SSD1306
#define UART_TX_PIN   0  // UART TX
#define UART_RX_PIN   1  // UART RX

// Variáveis de estado
volatile bool led_g_state = false;
volatile bool led_b_state = false;
volatile uint8_t current_number = 0;
volatile bool update_matrix = false;

// Estrutura do display
ssd1306_t display;

// Configuração dos WS2812 via PIO
PIO pio = pio0;
uint sm;

// Padrões 5x5 para dígitos de 0 a 9
const uint32_t numbers[10][25] = {
    {1,1,1,1,1,  1,0,0,0,1,  1,0,0,0,1,  1,0,0,0,1,  1,1,1,1,1}, // 0
    {0,0,1,0,0,  0,0,1,0,0,  0,0,1,0,0,  0,0,1,0,0,  0,0,1,0,0}, // 1
    {1,1,1,1,1,  1,0,0,0,0,  1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1}, // 2
    {1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1}, // 3
    {1,0,0,0,0,  0,0,0,0,1,  1,1,1,1,1,  1,0,0,0,1,  1,0,0,0,1}, // 4
    {1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1,  1,0,0,0,0,  1,1,1,1,1}, // 5
    {1,1,1,1,1,  1,0,0,0,1,  1,1,1,1,1,  1,0,0,0,0,  1,1,1,1,1}, // 6
    {1,0,0,0,0,  0,0,0,0,1,  1,0,0,0,0,  1,0,0,0,1,  1,1,1,1,1}, // 7
    {1,1,1,1,1,  1,0,0,0,1,  1,1,1,1,1,  1,0,0,0,1,  1,1,1,1,1}, // 8
    {1,0,0,0,0,  0,0,0,0,1,  1,1,1,1,1,  1,0,0,0,1,  1,1,1,1,1}  // 9
};

/**
 * Inicializa o PIO para controle dos WS2812.
 */
void ws2812_init() {
    uint offset = pio_add_program(pio, &ws2812_program);
    sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, offset, WS2812_PIN);
}

/**
 * Atualiza a matriz WS2812 com o padrão do dígito especificado.
 * Utiliza somente o canal vermelho (valor 255 para ligado).
 */
void update_led_matrix(uint8_t number) {
    for (int i = 0; i < 25; i++) {
        uint8_t r = numbers[number][i] ? 255 : 0;
        uint32_t color = (0 << 16) | (r << 8) | 0; // Formato GRB
        pio_sm_put_blocking(pio, sm, color);
    }
    sleep_us(50);
}

/**
 * Processa um caractere recebido pela interface serial.
 * Exibe o caractere no display e, se for dígito, sinaliza a atualização da matriz.
 */
void process_input(char c) {
    ssd1306_clear(&display);
    ssd1306_draw_char(&display, 0, 0, c);
    ssd1306_send_data(&display);
    printf("Recebido: %c\n", c);

    if (c >= '0' && c <= '9') {
        current_number = c - '0';
        update_matrix = true;
    }
}

// Variáveis para debouncing dos botões (em ms)
volatile uint32_t last_time_A = 0, last_time_B = 0;

/**
 * Callback de interrupção para os botões.
 * Alterna o estado dos LEDs e atualiza o display com o status.
 */
void gpio_callback(uint gpio, uint32_t events) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if (gpio == BUTTON_A_PIN) {
        if (now - last_time_A < 200) return;
        last_time_A = now;
        led_g_state = !led_g_state;
        gpio_put(LED_G_PIN, led_g_state);
        printf("Botão A: LED Verde %s\n", led_g_state ? "ON" : "OFF");
        ssd1306_clear(&display);
        ssd1306_draw_string(&display, 0, 0, led_g_state ? "Verde ON" : "Verde OFF");
        ssd1306_send_data(&display);
    } else if (gpio == BUTTON_B_PIN) {
        if (now - last_time_B < 200) return;
        last_time_B = now;
        led_b_state = !led_b_state;
        gpio_put(LED_B_PIN, led_b_state);
        printf("Botão B: LED Azul %s\n", led_b_state ? "ON" : "OFF");
        ssd1306_clear(&display);
        ssd1306_draw_string(&display, 0, 0, led_b_state ? "Azul ON" : "Azul OFF");
        ssd1306_send_data(&display);
    }
}

/**
 * Função principal.
 * Inicializa as interfaces e periféricos, e processa as entradas de forma contínua.
 */
int main() {
    stdio_init_all();
    sleep_ms(100); // Aguarda estabilização

    // Inicializa UART
    uart_init(uart0, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Configura os LEDs
    gpio_init(LED_G_PIN);
    gpio_set_dir(LED_G_PIN, GPIO_OUT);
    gpio_init(LED_B_PIN);
    gpio_set_dir(LED_B_PIN, GPIO_OUT);

    // Configura os botões com pull-up e registra a interrupção
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
    gpio_set_irq_enabled(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Configura o I2C para o display
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display SSD1306
    ssd1306_init(&display, i2c1);
    ssd1306_clear(&display);
    ssd1306_send_data(&display);

    // Inicializa os WS2812 via PIO
    ws2812_init();

    while (1) {
        // Processa entrada via USB CDC
        int c_usb = getchar_timeout_us(0);
        if (c_usb != PICO_ERROR_TIMEOUT) {
            process_input((char)c_usb);
        }
        // Processa entrada via UART
        if (uart_is_readable(uart0)) {
            process_input(uart_getc(uart0));
        }
        // Atualiza a matriz WS2812 se necessário
        if (update_matrix) {
            update_led_matrix(current_number);
            update_matrix = false;
        }
        sleep_ms(40);
    }
    
    return 0;
}
