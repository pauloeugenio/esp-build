/**
 * @file dht_config.h
 * @brief Arquivo cabeçalho de configuração para o cálculo da distorção harmônica total a partir da FFT
 *
 * @author --
 * @date 02 de Agosto de 2021
 */

#ifndef _DHT_CONFIG_
    #include <math.h>
    #include <stdint.h>
    #include "esp_adc_cal.h"

    #define _DHT_CONFIG_
    #define FFT_N 408                       // 6120/408 = 15 Hz/bin
    #define MAX_HARM 50                     // round(1/(SAMPLING_PERIOD)/60/2) max. harmonic that can be obtained
    #define DECI_FACTOR 4                   // fundamental_frequency/resolution_frequency
    #define HAMMING_CORRECTION_FACTOR 1.85  // factor to correct AMPLITUDE distortion caused by Hamming window

    /**
     * @brief Computa a transformada discreta de fourier por meio do algoritmo FFT com decimação no tempo radix-4.
     * 
     * @param input Ponteiro para um array de floats que representa o sinal a ser aplicado a fft.
     * @param N tamanho do sinal de entrada (deve ser um número na base 2).
     *
     * @return
     *     - Ponteiro para um array de floats de tamanho N/2 que representa a saída da fft para a metade do espectro de magnitude (considerando o sinal de entrada real e com espectro simétrico).
     */
    float* fft4(float* input, unsigned N);

    /**
     * @brief Realiza o cálculo da FFT sobre os arrays de tensão e corrente e computa a DHT de cada grandeza.
     *
     * @param input buffer da forma de onda de entrada como tipo inteiro.
     * @param samples número de amostras contida na forma de onda.
     * @param tdm_idx índice que indica a posição do canal ADC escolhido para computar a DHT considerando o buffer multiplexado por divisão no tempo (TDM).
     * @param num_chan número de canais utilizados no buffer de entrada.
     * @param offset valor de offset sobre os valores da forma de onda de entrada.
     * @param scaling valor de scale sobre os valores da forma de onda de entrada.
     * @param adcCalibration variável de calibração do adc.
     * 
     * @return 
     *      - Valor de distorção harmônica total para a forma de onda de entrada.
     */
    float compute_dht(uint16_t* input, int samples, int tdm_idx, int num_chan, float offset, float scaling, esp_adc_cal_characteristics_t adcCalibration);
    
#endif // _DHT_CONFIG_
