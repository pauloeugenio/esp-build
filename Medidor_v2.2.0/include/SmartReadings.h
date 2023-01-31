/**
 * @file SmartReadings.h
 * @brief Arquivo de inicialização às funcionalidades da estrutura SmartReadings
 * 
 * @author --
 * @date 06 de Dezembro de 2021
 */

#ifndef _SMART_READINGS_
    #define _SMART_READINGS_

    #include <stdint.h>
    #include <stdlib.h>
    #include <math.h>

    #include <driver/adc.h>
    #include "esp_adc_cal.h"

    #include "dht_config.h"
    #include "adc_config.h"
    #include "gpio_config.h"
    #include "definitions.h"
    
    #define arrayLength(array) (sizeof((array)) / sizeof((array)[0]))
    #define BIT_READ(value, bit) (((value) >> (bit)) & 0x01)
    #define BIT_SET(value, bit) ((value) |= (1UL << (bit)))
    #define BIT_CLEAR(value, bit) ((value) &= ~(1UL << (bit)))
    #define BIT_WRITE(value, bit, bitvalue) ((bitvalue) ? BIT_SET(value, bit) : BIT_CLEAR(value, bit))

    #define SAMPLES_GPIO 1024

    //enum rect_comp_id{BIT_VA, BIT_VB, BIT_VC, BIT_IA, BIT_IB, BIT_IC}; // ids para bits de acesso ao vetor do comparador
    enum rect_comp_id{BIT_IA, BIT_VC, BIT_IC, NONE1, NONE2, BIT_IB, BIT_VA, BIT_VB}; // ids para bits de acesso ao vetor do comparador

    typedef struct 
    {
        // Tensao RMS
        float VA; 
        float VB; 
        float VC; 
        
        // Corrente RMS
        float IA; 
        float IB; 
        float IC; 
        
        // Pot. ativa
        float P;
        float PA;
        float PB;
        float PC;
        
        // Pot. aparente
        float S;
        float SA;
        float SB;
        float SC;
        
        // Pot. reativa
        float Q;
        float QA;
        float QB;
        float QC;

        // Energia
        float Ep;
        float Eq;
        float Es;

        // Fator de potencia
        float FP;    
        float FPA;
        float FPB;
        float FPC;

        // Frequencia
        float F;
        float FA;
        float FB;
        float FC;

        // Angulo de tensao
        float A_RS;
        float A_ST;
        float A_TR;

        // Fator de desequilibrio de tensao
        float FDV;

        // Distorcao Harmonica Total
        float DHT[6];
    } SmartReadings;

    typedef struct 
    {
        char ge_fname[16];             // Nome do arquivo que contém as variáveis de configuração do sensor.
        char id_ota[8];                // Nome (id) do sensor.

        char actualVersionSPIFFFS[16]; // Versão atual do SPIFFS.
        char nextVersionSPIFFFS[16];   // Próxima versão do SPIFFS.
        float spiffs_version;          // Versão atual do SPIFFS em formato float.
 
        char SSID[32];                 // "Service set identifier" da rede wi-fi a se conectar.
        char PASSWD[32];               // "Password" da rede wi-fi a se conectar.


        // relacao de transformacao de corrente
        float tc;

        // constantes de calibracao em laboratorio (bancada)
        float coefv1_lab[2];
        float coefv2_lab[2];
        float coefv3_lab[2];

        float coefi1_lab[3];
        float coefi2_lab[3];
        float coefi3_lab[3];

        // constantes de calibracao em campo (ambiente de producao)

        float coefv1_field[2];
        float coefv2_field[2];
        float coefv3_field[2];

        float coefi1_field[2];
        float coefi2_field[2];
        float coefi3_field[2];

    } SensorConstants;

    /**
     * @brief Inicializa valores da estrutura SensorConstants.
     * 
     * @return
     *     - Estrutura SensorConstants inicializada.
     */
    SensorConstants initialize_sensor_constants();

    /**
     * @brief Inicializa valores da estrutura SmartReadings
     * 
     * @return
     *     - Estrutura SmartReadings inicializada
     */
    SmartReadings initialize_readings();

    /**
     * @brief Executa os cálculos necessários em cima das formas de onda para gerar as variáveis elétricas de interesse.
     * 
     * @param readings estrutura SmartReadings a ser modifica com as novas leituras.
     * @param constants estrutura contendo as constantes inerentes ao sensor.
     * @param loop_period período de integração desde a última leitura para acúmulo das energias.
     * @param buffer buffer contendo as formas de onda lidas dos canais do ADC.
     * @param rectComp vetor contendo formas de onda retangulares oriundas da leitura dos comparadores, constituindo o envelope binário das ondas senoidais de entrada.
     * @param t vetor tempo.
     * @param samples_read número de amostras lidas no buffer.
     * @param num_adc_chan número de canais ADC utilizados na leitura do buffer.
     * 
     * @return
     *     - Estrutura SmartReadings com seus campos calculados.
     */
    SmartReadings compute_readings(SmartReadings readings, SensorConstants constants, float loop_period, uint16_t* buffer, uint8_t* rectComp, uint32_t* t, size_t samples_read, size_t num_adc_chan);

    /**
     * @brief Verifica e corrige valores inválidos como nan e inf
     * 
     * @param readings estrutura SmartReadings para que tenha seus valores verificados 
     * 
     * @return
     *     - Estrutura SmartReadings com os valores inválidos verificados e corrigidos
     */
    SmartReadings correct_invalid_values(SmartReadings readings);

    /**
     * @brief Imprime na saída padrão as variávies mais improtantes resultantes da leitura
     * 
     * @param readings estrutura SmartReadings a ter atributos apresentados
     */
    void print_readings(SmartReadings readings);

    /**
     * @brief Plota as formas de onda contidas no buffer
     * 
     * @param buffer buffer a ter as amostras plotadas
     * @param samples_read número de amostras contidas no buffer
     * @param num_adc_chan número de canais do ADC
     * @param rtc_current relação de transformação do TC
     */
    void plot_waveforms(uint16_t* buffer, size_t samples_read, size_t num_adc_chan, float rtc_current);

    /**
     * @brief Acumula a diferença de tempo entre distintos cruzamentos por zero para posterior
     *        cálculo das frequências e ângulos de tensão/corrente
     * 
     * @param deltaTime diferença de tempos a ser acumulada
     * @param timeAcc acumulador da diferença dos tempos
     * @param itr índice do ponto de cruzamento anterior ao que está sendo analisado
     * @param count contador da quantidade de ciclos de cruzamento por zero foram encontrados
     * @param i índice que indica a posição atual de iteração do vetor
     *
     */
    void time_calc(int deltaTime, double *timeAcc, int *itr, int *count, int i);

    /**
     * @brief Aplica coeficientes de uma curva de calibração polinomial à grandeza de interesse
     * 
     * @param input grandeza a ser ajustada pela curva de calibração
     * @param coef array de coeficientes cuja ordem dos graus é [c^0, c^1, ..., c^(size-1)]
     * @param size tamanho do array de coeficientes
     *
     * @return
     *     - Resultado do ajuste da grandeza pela curva de calibração
     */
    float calibration_calc(float input, float *coef, uint32_t size);

    /**
     * @brief Aplica as curvas de calibração de tensão e corrente diretamentes às grandezas RMS.
     * 
     * @param readings estrutura contendo as leituras prévias
     * @param constants estrutura contendo as constantes inerentes ao sensor.
     *
     */
    void voltage_current_calibration(SmartReadings* readings, SensorConstants constants);



#endif // _SMART_READINGS_