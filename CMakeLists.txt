# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializa o SDK da Raspberry Pi Pico
include(pico_sdk_import.cmake)

project(projetow2040 C CXX ASM)

# Inicializa o Raspberry Pi Pico SDK
pico_sdk_init()

# Adiciona o executável do projeto
add_executable(projetow2040
    projetow2040.c
    inc/ssd1306_i2c.c
    inc/font.c
)

# Agora podemos gerar o cabeçalho da PIO corretamente
pico_generate_pio_header(projetow2040 ${CMAKE_CURRENT_SOURCE_DIR}/projetow2040.pio)

# Adiciona diretórios de inclusão para os headers personalizados
target_include_directories(projetow2040 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Linka as bibliotecas necessárias
target_link_libraries(projetow2040
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_gpio
    hardware_uart
    hardware_clocks
)

# Habilita a comunicação via UART e USB
pico_enable_stdio_uart(projetow2040 1)
pico_enable_stdio_usb(projetow2040 1)

# Gera arquivos de saída adicionais (.bin, .hex, .uf2)
pico_add_extra_outputs(projetow2040)
