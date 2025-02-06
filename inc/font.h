#ifndef FONT_H
#define FONT_H

#include <stdint.h>

/**
 * @file font.h
 * @brief Declaração das fontes para exibição de caracteres.
 *
 * Este arquivo define as fontes utilizadas para exibir caracteres no display.
 * As fontes são organizadas em arrays que contêm a representação gráfica
 * dos caracteres em formato 5x5 (ou similar).
 */

/**
 * @brief Fonte para letras maiúsculas.
 *
 * Um array de 26 elementos, onde cada elemento é um array de 5 bytes que
 * representa um caractere maiúsculo (A-Z).
 */
extern const uint8_t font_uppercase[26][5];

/**
 * @brief Fonte para letras minúsculas.
 *
 * Um array de 26 elementos, onde cada elemento é um array de 5 bytes que
 * representa um caractere minúsculo (a-z).
 */
extern const uint8_t font_lowercase[26][5];

/**
 * @brief Fonte para dígitos numéricos.
 *
 * Um array de 10 elementos, onde cada elemento é um array de 5 bytes que
 * representa um dígito (0-9).
 */
extern const uint8_t font_numbers[10][5];

#endif // FONT_H

