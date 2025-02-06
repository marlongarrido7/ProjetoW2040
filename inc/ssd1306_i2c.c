#include "ssd1306.h"
#include "font.h"   // Cabeçalho com definições das fontes
#include <stdlib.h>
#include <string.h>

// Declaração da função que envia comandos via I2C
void ssd1306_send_command(ssd1306_t *ssd, uint8_t command);

/**
 * Inicializa o display SSD1306:
 * - Configura largura, altura, páginas e endereço.
 * - Aloca o buffer de vídeo.
 * - Envia a sequência de comandos de inicialização.
 */
void ssd1306_init(ssd1306_t *ssd, i2c_inst_t *i2c) {
    ssd->width = SSD1306_WIDTH;
    ssd->height = SSD1306_HEIGHT;
    ssd->pages = SSD1306_HEIGHT / 8;
    ssd->address = SSD1306_ADDRESS;
    ssd->i2c_port = i2c;
    ssd->buffer = calloc(ssd->pages * ssd->width, sizeof(uint8_t));
    if (!ssd->buffer) return; // Falha na alocação de memória

    uint8_t init_cmds[] = {
        0xAE,             // Desliga o display
        0x20, 0x00,       // Endereçamento horizontal
        0x40,             // Linha de início
        0xA1,             // Remapeia os segmentos
        0xC8,             // Scan invertido
        0xA8, SSD1306_HEIGHT - 1, // Altura do display
        0xD3, 0x00,       // Deslocamento vertical zero
        0xDA, 0x12,       // Configuração de hardware
        0x81, 0x7F,       // Contraste
        0xA4,             // Exibe RAM
        0xA6,             // Exibição normal
        0xD5, 0x80,       // Frequência do clock
        0x8D, 0x14,       // Ativa DC-DC
        0xAF              // Liga o display
    };

    for (uint8_t i = 0; i < sizeof(init_cmds); i++) {
        ssd1306_send_command(ssd, init_cmds[i]);
    }
}

/**
 * Envia um comando via I2C.
 * O byte 0x80 indica que o próximo byte é um comando.
 */
void ssd1306_send_command(ssd1306_t *ssd, uint8_t command) {
    uint8_t buffer[2] = {0x80, command};
    i2c_write_blocking(ssd->i2c_port, ssd->address, buffer, 2, false);
}

/**
 * Envia o buffer de dados para o display via I2C.
 * Adiciona o byte de controle 0x40 antes dos dados.
 */
void ssd1306_send_data(ssd1306_t *ssd) {
    uint8_t *buffer = malloc(ssd->pages * ssd->width + 1);
    if (!buffer) return;
    buffer[0] = 0x40;
    memcpy(&buffer[1], ssd->buffer, ssd->pages * ssd->width);
    i2c_write_blocking(ssd->i2c_port, ssd->address, buffer, ssd->pages * ssd->width + 1, false);
    free(buffer);
}

/**
 * Limpa o display, zerando o buffer e atualizando a tela.
 */
void ssd1306_clear(ssd1306_t *ssd) {
    memset(ssd->buffer, 0, ssd->pages * ssd->width);
    ssd1306_send_data(ssd);
}

/**
 * Desenha um caractere no display.
 * Suporta letras maiúsculas, minúsculas e dígitos.
 */
void ssd1306_draw_char(ssd1306_t *ssd, uint8_t x, uint8_t y, char c) {
    extern const uint8_t font_uppercase[26][5];
    extern const uint8_t font_lowercase[26][5];
    extern const uint8_t font_numbers[10][5];

    const uint8_t *font;
    if (c >= 'A' && c <= 'Z')
        font = font_uppercase[c - 'A'];
    else if (c >= 'a' && c <= 'z')
        font = font_lowercase[c - 'a'];
    else if (c >= '0' && c <= '9')
        font = font_numbers[c - '0'];
    else
        return; // Caractere não suportado

    for (uint8_t i = 0; i < 5; i++) {
        if (x + i < SSD1306_WIDTH)
            ssd->buffer[x + i + (y * ssd->width)] = font[i];
    }
    ssd1306_send_data(ssd);
}

/**
 * Desenha uma string no display, avançando a posição para cada caractere.
 */
void ssd1306_draw_string(ssd1306_t *ssd, uint8_t x, uint8_t y, const char *str) {
    while (*str) {
        ssd1306_draw_char(ssd, x, y, *str);
        x += 6;  // 5 pixels de caractere + 1 pixel de espaçamento
        if (x >= SSD1306_WIDTH) {
            x = 0;
            y += 8; // Nova linha (8 pixels)
            if (y >= SSD1306_HEIGHT)
                return;
        }
        str++;
    }
    ssd1306_send_data(ssd);
}
