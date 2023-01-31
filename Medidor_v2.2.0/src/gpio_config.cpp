/**
 * @file gpio_config.h
 * @brief Arquivo de inicialização às funcionalidas de leitura contínua de dos GPIOS para cálculo 
 *        de grandezas de fase
 * 
 * @author --
 * @date 01 de Março de 2022
 */

#include "gpio_config.h"

void gpio_global_config()
{
    // Configuracao dos pinos dos comparadores
    pinMode(COMP_VA, INPUT); // VA
    pinMode(COMP_VB, INPUT); // VB
    pinMode(COMP_VC, INPUT); // VC
    pinMode(COMP_IA, INPUT); // IA
    pinMode(COMP_IB, INPUT); // IB
    pinMode(COMP_IC, INPUT); // IC
}

void read_gpio_data(uint8_t (*buf)[SAMPLES_GPIO], uint32_t (*time)[SAMPLES_GPIO])
{
    uint32_t timeout;
    uint32_t aux=0;
    timeout = micros();
    bool initialState = digitalRead(COMP_VA);
    while((digitalRead(COMP_VA)==initialState) && ((micros()-timeout)<1e6)){} // esperar mudar estado ou timeout de 1s
    for (int i = 0; i < SAMPLES_GPIO; i++)
    {
        (*time)[i] = micros(); 
        aux = (REG_READ(GPIO_IN_REG)) & (GPIO_INPUT_PIN_SEL);
        (*buf)[i] = ((aux & 0x0e000000) >> 5*4) | ((aux & 0x00007000) >> 3*4); // turns 0x0e007000 into 0xe7 containing: VB/VA/IB/NONE/NONE/IC/VC/IA
        while (micros() - (*time)[i] <= SAMPLING_PERIOD_GPIO); // holds the 'while' until the time taken within the 'for' iteration reaches the SAMPLING_PERIOD
    }
}