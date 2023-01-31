/**
 * @file adc_config.h
 * @brief Arquivo de execução das funcionalidas de leitura contínua de múltiplos canais
 *        e em alta velocidade do ADC SAR do ESP32
 * 
 * @author --
 * @date 06 de Dezembro de 2021
 */

#include "adc_config.h"

void adc_configuration()
{
    // adc configurations
    adc_i2s_mode_init(ADC_UNIT_1, ADC_CHANNEL_0);
    adc_i2s_mode_init(ADC_UNIT_1, ADC_CHANNEL_3);
    adc_i2s_mode_init(ADC_UNIT_1, ADC_CHANNEL_4);
    adc_i2s_mode_init(ADC_UNIT_1, ADC_CHANNEL_5);
    adc_i2s_mode_init(ADC_UNIT_1, ADC_CHANNEL_6);
    adc_i2s_mode_init(ADC_UNIT_1, ADC_CHANNEL_7);

    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_VA, ADC_ATTEN); 
    adc1_config_channel_atten(ADC_VB, ADC_ATTEN); 
    adc1_config_channel_atten(ADC_VC, ADC_ATTEN); 
    adc1_config_channel_atten(ADC_IA, ADC_ATTEN); 
    adc1_config_channel_atten(ADC_IB, ADC_ATTEN); 
    adc1_config_channel_atten(ADC_IC, ADC_ATTEN); 
    adc_set_i2s_data_source(ADC_I2S_DATA_SRC_ADC);
}

void i2s_configuration()
{
    // i2s configuration
    i2s_config_t i2s_config = {};
    i2s_config.mode = (i2s_mode_t)                        (I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN);
    i2s_config.sample_rate = (int)                        SAMPLING_FREQUENCY_ADC;
    i2s_config.bits_per_sample = (i2s_bits_per_sample_t)  16;
    i2s_config.channel_format = (i2s_channel_fmt_t)       (I2S_CHANNEL_FMT_ONLY_LEFT);
    i2s_config.communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S); 
    i2s_config.intr_alloc_flags = (int)                   (ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM);
    i2s_config.dma_buf_count = (int)                      4*NUM_ADC_CHAN;
    i2s_config.dma_buf_len = (int)                        1020;
    i2s_config.use_apll = (bool)                          false;
    i2s_config.fixed_mclk = (int)                         0;

    i2s_driver_install((i2s_port_t) I2S_PORT, &i2s_config, 0, NULL);
    i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_MAX); // numero de canais definidos como o maximo
    vTaskDelay(pdMS_TO_TICKS(10));
}

void reg_configuration()
{
    uint32_t aux;

    // habilita varredura em multiplos canais (e.g. 6)
    SET_PERI_REG_BITS(SYSCON_SARADC_CTRL_REG, SYSCON_SARADC_SAR1_PATT_LEN, NUM_ADC_CHAN-1, SYSCON_SARADC_SAR1_PATT_LEN_S); 

    // configura pattern tables
    aux = (uint32_t) 0x00000000;
    aux |= ((ADC_VA << 4) | (ADC_WIDTH << 2) | (ADC_ATTEN << 0)) << 24; // CH0
    aux |= ((ADC_VB << 4) | (ADC_WIDTH << 2) | (ADC_ATTEN << 0)) << 16; // CH3
    aux |= ((ADC_VC << 4) | (ADC_WIDTH << 2) | (ADC_ATTEN << 0)) << 8;  // CH6
    aux |= ((ADC_IA << 4) | (ADC_WIDTH << 2) | (ADC_ATTEN << 0)) << 0;  // CH5
    WRITE_PERI_REG(SYSCON_SARADC_SAR1_PATT_TAB1_REG, aux); // pattern table 1 SAR ADC1
    aux = (uint32_t) 0x00000000;
    aux |= ((ADC_IB << 4) | (ADC_WIDTH << 2) | (ADC_ATTEN << 0)) << 24; // CH7
    aux |= ((ADC_IC << 4) | (ADC_WIDTH << 2) | (ADC_ATTEN << 0)) << 16; // CH4
    WRITE_PERI_REG(SYSCON_SARADC_SAR1_PATT_TAB2_REG, aux); // pattern table 2 SAR ADC1

    // inverter sequencia de leitura dos dados, ja que a leitura crua do adc e invertida pelo DMA
    SET_PERI_REG_MASK(SYSCON_SARADC_CTRL2_REG, SYSCON_SARADC_SAR1_INV); 

    // delay structural para atualizar as configuracoes anteriores
    vTaskDelay(pdMS_TO_TICKS(10));

    // remove o limite de leituras antes do adc resetar
    //CLEAR_PERI_REG_MASK(SYSCON_SARADC_CTRL2_REG, SYSCON_SARADC_MEAS_NUM_LIMIT); // mesmo que: SYSCON.saradc_ctrl2.meas_num_limit=0
}

void adc_global_config()
{
    adc_configuration();
    i2s_configuration();
    reg_configuration();
    vTaskDelay(pdMS_TO_TICKS(1000)); // delay estrutural
    i2s_adc_enable((i2s_port_t) I2S_PORT); // habilitar leitura
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void read_data(uint16_t (*buf)[NUM_ADC_CHAN*SAMPLES_ADC], size_t* smpl_read)
{
    size_t bytes_read; // numero de bytes lidos
    i2s_read((i2s_port_t) I2S_PORT, (void*) (*buf), sizeof((*buf)), &bytes_read, portMAX_DELAY); // ler buffer
	*smpl_read = bytes_read/sizeof(uint16_t); // converte quantidade de bytes lidos para amostras lidas

    /*
    * Organizar leituras na ordem do canal e.g.: VA, VB, VC, IA, IB, IC 
    */
    uint16_t adc_map[8] = {0,0,0,1,5,3,2,4};
    uint16_t var[6], tempch;   

    for(int ii=0; ii<*smpl_read; ii+=NUM_ADC_CHAN)
    {
        for(int jj=0; jj<NUM_ADC_CHAN; jj++)
        {
            tempch = getCh((*buf)[jj+ii]); // numero do canal e.g.: 0,3,6,5,7,4
            var[adc_map[tempch]] = getSmpl((*buf)[jj+ii]); // distribui leitura de acordo com o canal e.g.: 0->va,3->vb,6->vc,5->ia,7->ib,4->ic
        }
        for(int jj=0; jj<NUM_ADC_CHAN; jj++) (*buf)[jj+ii] = var[jj];
    }
}
