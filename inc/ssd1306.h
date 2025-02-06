#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

/**
 * @file ssd1306.h
 * @brief Interface para o display OLED SSD1306 via I2C.
 *
 * Este arquivo define as constantes, a estrutura e as funções necessárias para
 * inicializar e controlar o display SSD1306.
 */

// Definições da resolução e endereço I2C do display
#define SSD1306_WIDTH 128      ///< Largura do display em pixels
#define SSD1306_HEIGHT 64      ///< Altura do display em pixels
#define SSD1306_ADDRESS 0x3C   ///< Endereço I2C do display SSD1306

/**
 * @brief Estrutura que representa um display SSD1306.
 *
 * Armazena as dimensões, o endereço I2C, a instância da interface I2C e o buffer
 * de memória utilizado para armazenar os dados a serem exibidos.
 */
typedef struct {
    uint8_t width;           ///< Largura do display (em pixels)
    uint8_t height;          ///< Altura do display (em pixels)
    uint8_t pages;           ///< Número de páginas (cada página equivale a 8 linhas de pixels)
    uint8_t address;         ///< Endereço I2C do display
    i2c_inst_t *i2c_port;    ///< Ponteiro para a instância da interface I2C utilizada
    uint8_t *buffer;         ///< Buffer de memória para os dados de exibição
} ssd1306_t;

/**
 * @brief Inicializa o display SSD1306.
 *
 * Configura as dimensões, o endereço e a porta I2C, além de alocar o buffer de vídeo.
 *
 * @param ssd Ponteiro para a estrutura do display SSD1306.
 * @param i2c Ponteiro para a instância da interface I2C a ser utilizada.
 */
void ssd1306_init(ssd1306_t *ssd, i2c_inst_t *i2c);

/**
 * @brief Envia um comando para o display via I2C.
 *
 * Utilizado para configurar e controlar o display.
 *
 * @param ssd     Ponteiro para a estrutura do display SSD1306.
 * @param command Comando a ser enviado.
 */
void ssd1306_command(ssd1306_t *ssd, uint8_t command);

/**
 * @brief Transfere o buffer de vídeo para o display.
 *
 * Atualiza o display com o conteúdo atualmente armazenado no buffer.
 *
 * @param ssd Ponteiro para a estrutura do display SSD1306.
 */
void ssd1306_send_data(ssd1306_t *ssd);

/**
 * @brief Limpa o display.
 *
 * Zera o buffer interno e atualiza o display, resultando em uma tela em branco.
 *
 * @param ssd Ponteiro para a estrutura do display SSD1306.
 */
void ssd1306_clear(ssd1306_t *ssd);

/**
 * @brief Desenha um caractere no display.
 *
 * Posiciona um caractere individual na posição (x, y).
 *
 * @param ssd Ponteiro para a estrutura do display SSD1306.
 * @param x   Coordenada horizontal (em pixels).
 * @param y   Coordenada vertical (em páginas; cada página equivale a 8 pixels).
 * @param c   Caractere a ser desenhado.
 */
void ssd1306_draw_char(ssd1306_t *ssd, uint8_t x, uint8_t y, char c);

/**
 * @brief Desenha uma string no display.
 *
 * Percorre a string e desenha cada caractere, avançando a posição automaticamente.
 *
 * @param ssd Ponteiro para a estrutura do display SSD1306.
 * @param x   Coordenada horizontal inicial (em pixels).
 * @param y   Coordenada vertical inicial (em páginas).
 * @param str String a ser desenhada.
 */
void ssd1306_draw_string(ssd1306_t *ssd, uint8_t x, uint8_t y, const char *str);

#endif // SSD1306_H
