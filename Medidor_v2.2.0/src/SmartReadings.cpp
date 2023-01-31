/**
 * @file SmartReadings.cpp
 * @brief Arquivo de execução das funcionalidades da estrutura SmartReadings
 * 
 * @author --
 * @date 06 de Dezembro de 2021
 */

#include "SmartReadings.h"

SensorConstants initialize_sensor_constants()
{
  SensorConstants var;

  var.tc=1.0;
  var.spiffs_version = 0.0;

  var.coefv1_lab[0]=0.0;
  var.coefv1_lab[1]=1.0;

  var.coefv2_lab[0]=0.0;
  var.coefv2_lab[1]=1.0;
  
  var.coefv3_lab[0]=0.0;
  var.coefv3_lab[1]=1.0;
  
  var.coefv1_field[0]=0.0;
  var.coefv1_field[1]=1.0;

  var.coefv2_field[0]=0.0;
  var.coefv2_field[1]=1.0;

  var.coefv3_field[0]=0.0;
  var.coefv3_field[1]=1.0;

  var.coefi1_lab[0]=0.0;
  var.coefi1_lab[1]=1.0;
  var.coefi1_lab[2]=0.0;
  
  var.coefi2_lab[0]=0.0;
  var.coefi2_lab[1]=1.0;
  var.coefi2_lab[2]=0.0;
  
  var.coefi3_lab[0]=0.0;
  var.coefi3_lab[1]=1.0;
  var.coefi3_lab[2]=0.0;
  
  var.coefi1_field[0]=0.0;
  var.coefi1_field[1]=1.0;

  var.coefi2_field[0]=0.0;
  var.coefi2_field[1]=1.0;

  var.coefi3_field[0]=0.0;
  var.coefi3_field[1]=1.0;

  return var;
}

SmartReadings initialize_readings()
{
  SmartReadings var;

  var.VA = 0;
  var.VB = 0;
  var.VC = 0;

  var.IA = 0;
  var.IB = 0;
  var.IC = 0;

  var.P = 0;
  var.PA = 0;
  var.PB = 0;
  var.PC = 0;

  var.S = 0;
  var.SA = 0;
  var.SB = 0;
  var.SC = 0;

  var.Q = 0;
  var.QA = 0;
  var.QB = 0;
  var.QC = 0;

  var.Ep = 0;
  var.Eq = 0;
  var.Es = 0;

  var.FP = 0;
  var.FPA = 0;
  var.FPB = 0;
  var.FPC = 0;

  var.F = 0;
  var.FA = 0;
  var.FB = 0;
  var.FC = 0;

  var.A_RS = 0;
  var.A_ST = 0;
  var.A_TR = 0;

  var.FDV = 0;

  for(int ii=0; ii<6; ii++) var.DHT[ii] = 0;

  return var;
}

SmartReadings compute_readings(SmartReadings readings, SensorConstants constants, float loop_period, uint16_t* buffer, uint8_t* rectComp, uint32_t* t, size_t samples_read, size_t num_adc_chan)
{
    /*
    * Inicializacoes
    */

    int m1 = 0, m2 = 0, m3 = 0;
    int n1 = 0, n2 = 0, n3 = 0;
    int iant1 = 0, iant2 = 0, iant3 = 0;
    int iant4 = 0, iant5 = 0, iant6 = 0;

    double tv1_final = 0, tv2_final = 0, tv3_final = 0;
    double ti1_final = 0, ti2_final = 0, ti3_final = 0;

    int8_t sign_A, sign_B, sign_C;
    float beta = 0;
    float aux[6];

    float offset = 0.778;
    float scaling[6] = {679.0921518, 678.9127109, 686.1742357, 15.38926021, 17.28947914, 19.52049703};
    
    scaling[3] *= (float)(constants.tc);
    scaling[4] *= (float)(constants.tc);
    scaling[5] *= (float)(constants.tc);

    esp_adc_cal_characteristics_t adc_cal;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_2_5, ADC_WIDTH_BIT_12, 1100, &adc_cal);

    /*
    * Calculo da distorcao harmonica total de tensao e corrente
    */
    for(int ii=0; ii<num_adc_chan; ii++) readings.DHT[ii] = compute_dht(buffer, SAMPLES_ADC, ii, num_adc_chan, (float)offset, float(scaling[ii]), adc_cal);

    /*
    * Calculo dos valores derivados das formas de onda
    * ---
    * X_{RMS} = \sqrt{\frac{1}{n} \sum_{k=0}^{n-1} x^2[k]}
    * P_x = \frac{1}{n} \sum_{k=0}^{n-1} v_x[k]\cdot i_x[k] 
    */

    readings.VA = 0;
    readings.VB = 0;
    readings.VC = 0;

    readings.IA = 0;
    readings.IB = 0;
    readings.IC = 0;

    readings.PA = 0;
    readings.PB = 0;
    readings.PC = 0;
			
    for(int ii=0; ii<samples_read; ii+=num_adc_chan)
    {
        for(int jj=0; jj<num_adc_chan; jj++) aux[jj] = (float)((esp_adc_cal_raw_to_voltage(buffer[jj+ii], &adc_cal) / float(1000) - offset) * scaling[jj]);
		
        readings.VA += aux[0]*aux[0];
        readings.VB += aux[1]*aux[1];
        readings.VC += aux[2]*aux[2];

        readings.IA += aux[3]*aux[3];
        readings.IB += aux[4]*aux[4];
        readings.IC += aux[5]*aux[5];

        readings.PA += aux[0]*aux[3];
        readings.PB += aux[1]*aux[4];
        readings.PC += aux[2]*aux[5];
    }

    readings.VA = sqrt(readings.VA/(samples_read/num_adc_chan)); // V
    readings.VB = sqrt(readings.VB/(samples_read/num_adc_chan)); // V
    readings.VC = sqrt(readings.VC/(samples_read/num_adc_chan)); // V

    readings.IA = sqrt(readings.IA/(samples_read/num_adc_chan)); // A
    readings.IB = sqrt(readings.IB/(samples_read/num_adc_chan)); // A
    readings.IC = sqrt(readings.IC/(samples_read/num_adc_chan)); // A

    /*
    * aplicando reta de calibracao para tensao e corrente
    */

    // Ajuste das correntes e tensões de acordo com as curvas de calibração
    voltage_current_calibration(&readings, constants);

    /*
    * Computacao das grandezas de fase (frequencias e fases)
    * ---
    * A_{XY} = 360 \cdot f \cdot (\Delta t_X - \Delta t_Y)
    */

    for (int i=0; i<SAMPLES_GPIO-1; i++)
    {
        if (BIT_READ(rectComp[i+1], BIT_VA)==1 && BIT_READ(rectComp[i], BIT_VA)==0){time_calc(t[i+1]-t[iant1], &tv1_final, &iant1, &m1, i);}
        if (BIT_READ(rectComp[i+1], BIT_VB)==1 && BIT_READ(rectComp[i], BIT_VB)==0){time_calc(t[i+1]-t[iant2], &tv2_final, &iant2, &m2, i);}
        if (BIT_READ(rectComp[i+1], BIT_VC)==1 && BIT_READ(rectComp[i], BIT_VC)==0){time_calc(t[i+1]-t[iant3], &tv3_final, &iant3, &m3, i);}
        if (BIT_READ(rectComp[i+1], BIT_IA)==1 && BIT_READ(rectComp[i], BIT_IA)==0){time_calc(t[i+1]-t[iant4], &ti1_final, &iant4, &n1, i);}
        if (BIT_READ(rectComp[i+1], BIT_IB)==1 && BIT_READ(rectComp[i], BIT_IB)==0){time_calc(t[i+1]-t[iant5], &ti2_final, &iant5, &n2, i);}
        if (BIT_READ(rectComp[i+1], BIT_IC)==1 && BIT_READ(rectComp[i], BIT_IC)==0){time_calc(t[i+1]-t[iant6], &ti3_final, &iant6, &n3, i);}
    }

    // tranformando de uS para S os tempos;
    ti1_final /= 1000000;
    ti2_final /= 1000000;
    ti3_final /= 1000000;
    tv1_final /= 1000000;
    tv2_final /= 1000000;
    tv3_final /= 1000000;

    // Frequencias
    float F1_COMP = 1 / (tv1_final / m1);
    float F2_COMP = 1 / (tv2_final / m2);
    float F3_COMP = 1 / (tv3_final / m3);
    // float F4_COMP = 1 / (ti1_final / n1);
    // float F5_COMP = 1 / (ti2_final / n2);
    // float F6_COMP = 1 / (ti3_final / n3);

    readings.FA = F1_COMP;
    readings.FB = F2_COMP;
    readings.FC = F3_COMP;
    //readings.F = (F1_COMP + F2_COMP + F3_COMP + F4_COMP + F5_COMP + F6_COMP) / 6;
    readings.F = (F1_COMP + F2_COMP + F3_COMP) / 3;
    //printf("%f %f %f %f %f %f\n", F1_COMP, F2_COMP, F3_COMP, F4_COMP, F5_COMP, F6_COMP);

    // Fases das tensões de linha
    readings.A_RS = 360 * F1_COMP * (tv1_final - tv2_final);
    readings.A_ST = 360 * F2_COMP * (tv2_final - tv3_final);
    readings.A_TR = 360 * F3_COMP * (tv3_final - tv1_final);

    if (readings.A_RS > 180){readings.A_RS -= 360;}
    else if (readings.A_RS < -180){readings.A_RS += 360;}
    if (readings.A_ST > 180){readings.A_ST -= 360;}
    else if (readings.A_ST < -180){readings.A_ST += 360;}
    if (readings.A_TR > 180){readings.A_TR -= 360;}
    else if (readings.A_TR < -180){readings.A_TR += 360;}

    readings.A_RS -= 120; // 0+e
    readings.A_ST = readings.A_ST; // +120
    readings.A_TR -= 240; // -120

    if (readings.A_RS > 0)
    {
        readings.A_ST += readings.A_RS * 2; // +120
        readings.A_TR -= readings.A_RS;     // -120
        readings.A_RS -= readings.A_RS;     // 0
    }
    else
    {
        readings.A_ST += readings.A_RS / 2; // +120
        readings.A_TR += readings.A_RS / 2; // -120
        readings.A_RS -= readings.A_RS;     // 0
    }

    /*
    * Computacao das potencias, energias e fatores de potencia
    * ---
    * S = V_{1,RMS}\cdot I_{1,RMS} + V_{2,RMS}\cdot I_{2,RMS} + V_{3,RMS}\cdot I_{3,RMS}
    * Q = \sqrt{|S^2 - P^2|}
    * FP = \frac{P}{S}
    * E_p = P \cdot 1/180
    * E_s = S \cdot 1/180
    * E_q = Q \cdot 1/180
    */

    sign_A = (ti1_final>tv1_final) ? 1 : -1;
    sign_B = (ti2_final>tv2_final) ? 1 : -1;
    sign_C = (ti3_final>tv3_final) ? 1 : -1;

    readings.PA /= ((samples_read/num_adc_chan)*1000); // kW
    readings.PB /= ((samples_read/num_adc_chan)*1000); // kW
    readings.PC /= ((samples_read/num_adc_chan)*1000); // kW
    readings.P = readings.PA + readings.PB + readings.PC; // kW
  
    readings.SA = (readings.VA*readings.IA)/1000; // kVA
    readings.SB = (readings.VB*readings.IB)/1000; // kVA
    readings.SC = (readings.VC*readings.IC)/1000; // kVA
    readings.S = readings.SA + readings.SB + readings.SC; // kVA
  
    readings.QA = sign_A * sqrt(abs((readings.SA*readings.SA)-(readings.PA*readings.PA))); // kvar
    readings.QB = sign_B * sqrt(abs((readings.SB*readings.SB)-(readings.PB*readings.PB))); // kvar
    readings.QC = sign_C * sqrt(abs((readings.SC*readings.SC)-(readings.PC*readings.PC))); // kvar
    readings.Q = readings.QA + readings.QB + readings.QC; // kvar

    readings.FPA = readings.PA/readings.SA;
    readings.FPB = readings.PB/readings.SB;
    readings.FPC = readings.PC/readings.SC;
    readings.FP = readings.P/readings.S;

    // (maintaining same power pattern for 20s to sum up each energy)
    readings.Ep = readings.P * loop_period / 3600; // 20s = 20/3600h = 1/180h
    readings.Es = readings.S * loop_period / 3600; // 20s = 20/3600h = 1/180h
    readings.Eq = readings.Q * loop_period / 3600; // 20s = 20/3600h = 1/180h

    /*
    * Criterios de qualidade de energia
    * ---
    * \beta = \frac{(V1_{RMS}^4+V2_{RMS}^4+V3_{RMS}^4)}{(V1_{RMS}^2+V2_{RMS}^2+V3_{RMS}^2)^2}
    * fator_desequilibrio_tensao = 100 * \sqrt{\frac{1-\sqrt{3-6\beta}}{1+\sqrt{3-6\beta}}}
    */

    // desequilibrio de tensao
    beta = (pow(readings.VA,4)+pow(readings.VB,4)+pow(readings.VC,4))/pow(pow(readings.VA,2)+pow(readings.VB,2)+pow(readings.VC,2),2); 
    readings.FDV = 100 * sqrt((1-sqrt(3-6*beta))/(1+sqrt(3-6*beta)));

    readings = correct_invalid_values(readings);

    return readings;
}

SmartReadings correct_invalid_values(SmartReadings readings)
{
    /*
    * Garantir que nenhuma leitura seja nan ou inf
    */
    readings.VA = (isnan(readings.VA) || isinf(readings.VA)) ? 0 : readings.VA;
    readings.VB = (isnan(readings.VB) || isinf(readings.VB)) ? 0 : readings.VB;
    readings.VC = (isnan(readings.VC) || isinf(readings.VC)) ? 0 : readings.VC;
    readings.IA = (isnan(readings.IA) || isinf(readings.IA)) ? 0 : readings.IA;
    readings.IB = (isnan(readings.IB) || isinf(readings.IB)) ? 0 : readings.IB;
    readings.IC = (isnan(readings.IC) || isinf(readings.IC)) ? 0 : readings.IC;

    readings.PA = (isnan(readings.PA) || isinf(readings.PA)) ? 0 : readings.PA;
    readings.PB = (isnan(readings.PB) || isinf(readings.PB)) ? 0 : readings.PB;
    readings.PC = (isnan(readings.PC) || isinf(readings.PC)) ? 0 : readings.PC;

    readings.QA = (isnan(readings.QA) || isinf(readings.QA)) ? 0 : readings.QA;
    readings.QB = (isnan(readings.QB) || isinf(readings.QB)) ? 0 : readings.QB;
    readings.QC = (isnan(readings.QC) || isinf(readings.QC)) ? 0 : readings.QC;

    readings.SA = (isnan(readings.SA) || isinf(readings.SA)) ? 0 : readings.SA;
    readings.SB = (isnan(readings.SB) || isinf(readings.SB)) ? 0 : readings.SB;
    readings.SC = (isnan(readings.SC) || isinf(readings.SC)) ? 0 : readings.SC;

    readings.FPA = (isnan(readings.FPA) || isinf(readings.FPA)) ? 0 : readings.FPA;
    readings.FPB = (isnan(readings.FPB) || isinf(readings.FPB)) ? 0 : readings.FPB;
    readings.FPC = (isnan(readings.FPC) || isinf(readings.FPC)) ? 0 : readings.FPC;

    readings.P = (isnan(readings.P) || isinf(readings.P)) ? 0 : readings.P;
    readings.Q = (isnan(readings.Q) || isinf(readings.Q)) ? 0 : readings.Q;
    readings.S = (isnan(readings.S) || isinf(readings.S)) ? 0 : readings.S;
    readings.FP = (isnan(readings.FP) || isinf(readings.FP)) ? 0 : readings.FP;

    readings.DHT[0] = (isnan(readings.DHT[0]) || isinf(readings.DHT[0])) ? 0 : readings.DHT[0];
    readings.DHT[1] = (isnan(readings.DHT[1]) || isinf(readings.DHT[1])) ? 0 : readings.DHT[1];
    readings.DHT[2] = (isnan(readings.DHT[2]) || isinf(readings.DHT[2])) ? 0 : readings.DHT[2];
    readings.DHT[3] = (isnan(readings.DHT[3]) || isinf(readings.DHT[3])) ? 0 : readings.DHT[3];
    readings.DHT[4] = (isnan(readings.DHT[4]) || isinf(readings.DHT[4])) ? 0 : readings.DHT[4];
    readings.DHT[5] = (isnan(readings.DHT[5]) || isinf(readings.DHT[5])) ? 0 : readings.DHT[5];

    readings.Ep = (isnan(readings.Ep) || isinf(readings.Ep)) ? 0 : readings.Ep;
    readings.Eq = (isnan(readings.Eq) || isinf(readings.Eq)) ? 0 : readings.Eq;
    readings.FDV = (isnan(readings.FDV) || isinf(readings.FDV)) ? 0 : readings.FDV;

    readings.A_RS = (isnan(readings.A_RS) || isinf(readings.A_RS)) ? 0 : readings.A_RS;
    readings.A_ST = (isnan(readings.A_ST) || isinf(readings.A_ST)) ? 0 : readings.A_ST;
    readings.A_TR = (isnan(readings.A_TR) || isinf(readings.A_TR)) ? 0 : readings.A_TR;

    readings.F = (isnan(readings.F) || isinf(readings.F)) ? 0 : readings.F;
    readings.FA = (isnan(readings.FA) || isinf(readings.FA)) ? 0 : readings.FA;
    readings.FB = (isnan(readings.FB) || isinf(readings.FB)) ? 0 : readings.FB;
    readings.FC = (isnan(readings.FC) || isinf(readings.FC)) ? 0 : readings.FC;

    return readings;
}

void print_readings(SmartReadings readings)
{
    printf("-----------------------------\n");
    printf("VA = %f\nVB = %f\nVC = %f\n", readings.VA, readings.VB, readings.VC);
    printf("IA = %f\nIB = %f\nIC = %f\n", readings.IA, readings.IB, readings.IC);
    printf("PA = %f\nPB = %f\nPC = %f\n", readings.PA, readings.PB, readings.PC);
    printf("QA = %f\nQB = %f\nQC = %f\n", readings.QA, readings.QB, readings.QC);
    printf("SA = %f\nSB = %f\nSC = %f\n", readings.SA, readings.SB, readings.SC);
    printf("FPA = %f\nFPB = %f\nFPC = %f\n", readings.FPA, readings.FPB, readings.FPC);
    printf("P = %f\nQ = %f\nS = %f\nFP = %f \n", readings.P, readings.Q, readings.S, readings.FP);
    printf("Angulo V-RS = %f graus\nAngulo V-ST = %f graus\nAngulo V-TR = %f graus\n", readings.A_RS, readings.A_ST, readings.A_TR);
    printf("F = %f  Hz \nFDV = %f\n", readings.F, readings.FDV);
    printf("DHT(VA VB VC IA IB IC) = %f %f %f %f %f %f\n", readings.DHT[0], readings.DHT[1], readings.DHT[2], readings.DHT[3], readings.DHT[4], readings.DHT[5]);
    printf("-----------------------------\n\n");
}

void plot_waveforms(uint16_t* buffer, size_t samples_read, size_t num_adc_chan, float rtc_current)
{
  float aux[6];
  float offset = 0.778;
  float scaling[6] = {679.0921518, 678.9127109, 686.1742357, 15.38926021, 17.28947914, 19.52049703};
  
  scaling[3] *= (float)(rtc_current);
  scaling[4] *= (float)(rtc_current);
  scaling[5] *= (float)(rtc_current);

  esp_adc_cal_characteristics_t adc_cal;

  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_2_5, ADC_WIDTH_BIT_12, 1100, &adc_cal);
  for(int ii=0; ii<samples_read; ii+=num_adc_chan)
  {
    for(int jj=0; jj<num_adc_chan; jj++) aux[jj] = (float)((esp_adc_cal_raw_to_voltage(buffer[jj+ii], &adc_cal) / float(1000) - offset) * scaling[jj]);
    printf("%f %f %f %f %f %f\n", aux[0], aux[1], aux[2], aux[3], aux[4], aux[5]);
  }
}

void voltage_current_calibration(SmartReadings* readings, SensorConstants constants)
{

  // Calibracao em bancada
  (*readings).VA = calibration_calc((*readings).VA, constants.coefv1_lab, 2);
  (*readings).VB = calibration_calc((*readings).VB, constants.coefv2_lab, 2);
  (*readings).VC = calibration_calc((*readings).VC, constants.coefv3_lab, 2);   

  (*readings).IA = calibration_calc((*readings).IA, constants.coefi1_lab, 3);
  (*readings).IB = calibration_calc((*readings).IB, constants.coefi2_lab, 3);
  (*readings).IC = calibration_calc((*readings).IC, constants.coefi3_lab, 3); 

  // Calibracao em campo
  (*readings).VA = calibration_calc((*readings).VA, constants.coefv1_field, 2);
  (*readings).VB = calibration_calc((*readings).VB, constants.coefv2_field, 2);
  (*readings).VC = calibration_calc((*readings).VC, constants.coefv3_field, 2);   
  
  (*readings).IA = calibration_calc((*readings).IA, constants.coefi1_field, 2);
  (*readings).IB = calibration_calc((*readings).IB, constants.coefi2_field, 2);
  (*readings).IC = calibration_calc((*readings).IC, constants.coefi3_field, 2); 
  

  // float coefv1_lab[] = {-0.14649787, 0.99566895};
  // float coefv2_lab[] = {0.38651156, 0.99272804};
  // float coefv3_lab[] = {-1.24298223, 1.00059632};

  // float coefi1_lab[] = {-0.972456045, 1.08050016, -0.000975759657};
  // float coefi2_lab[] = {-1.18540439, 1.07860670, -0.000897036186};
  // float coefi3_lab[] = {-1.46413253, 1.06148397, -0.000614276172};

  /*
  R = {
  'IA': array([0.22638381]), nao aplicavel
  'IB': array([0.10660534]), nao aplicavel
  'IC': array([0.12479798]), nao aplicavel
  'VA': array([0.97741072]),
  'VB': array([0.97062119]),
  'VC': array([0.9811466])
  }

  Coeff(c0+c1*x) = {
  'IA': [array([[8.70418162, 0.26669373]])], 
  'IB': [array([[15.85425029,  0.60983748]])],
  'IC': [array([[14.16327477,  0.43647523]])],
  'VA': [array([[14.4850004 ,  0.95142886]])],
  'VB': [array([[9.16605079, 0.96227178]])],
  'VC': [array([[4.49998689, 0.94654807]])]
  }
  */
  /*
  float coefi1_field[] = {8.70418162, 0.26669373};
  float coefi2_field[] = {15.85425029, 0.60983748};
  float coefi3_field[] = {14.16327477, 0.43647523};
  */

  // float coefv1_field1[] = {14.4850004, 0.95142886};
  // float coefv2_field1[] = {9.16605079, 0.96227178};
  // float coefv3_field1[] = {4.49998689, 0.94654807};

  // Calibracao em campo
  // (*readings).VA = calibration_calc((*readings).VA, coefv1_field1, arrayLength(coefv1_field1));
  // (*readings).VB = calibration_calc((*readings).VB, coefv2_field1, arrayLength(coefv2_field1));
  // (*readings).VC = calibration_calc((*readings).VC, coefv3_field1, arrayLength(coefv3_field1));   
  /*
  (*readings).IA = calibration_calc((*readings).IA, coefi1_field, arrayLength(coefi1_field));
  (*readings).IB = calibration_calc((*readings).IB, coefi2_field, arrayLength(coefi2_field));
  (*readings).IC = calibration_calc((*readings).IC, coefi3_field, arrayLength(coefi3_field)); 
  */

  /*
  R = {
  'IA': array([0.93064699]),
  'IB': array([0.93432795]),
  'IC': array([0.90044503]),
  'VA': array([0.86355498]),
  'VB': array([0.80096672]),
  'VC': array([0.87211089])
  }
  Coeff(c0+c1*x) = {
  'IA': [array([[-2.84186804,  1.29444754]])],
  'IB': [array([[-0.44624435,  1.0844595 ]])],
  'IC': [array([[-0.35387255,  1.0305624 ]])],
  'VA': [array([[10.57894996,  0.95342623]])],
  'VB': [array([[9.02424257, 0.95828659]])],
  'VC': [array([[25.94252465,  0.88089276]])]
  }
  */

  // float coefv1_field2[] = {10.57894996, 0.95342623};
  // float coefv2_field2[] = {9.02424257, 0.95828659};
  // float coefv3_field2[] = {25.94252465, 0.88089276};

  // float coefi1_field2[] = {-2.84186804, 1.29444754};
  // float coefi2_field2[] = {-0.44624435, 1.0844595};
  // float coefi3_field2[] = {-0.35387255, 1.0305624};

  // Calibracao em campo
  // (*readings).VA = calibration_calc((*readings).VA, coefv1_field2, arrayLength(coefv1_field2));
  // (*readings).VB = calibration_calc((*readings).VB, coefv2_field2, arrayLength(coefv2_field2));
  // (*readings).VC = calibration_calc((*readings).VC, coefv3_field2, arrayLength(coefv3_field2));   

  // (*readings).IA = calibration_calc((*readings).IA, coefi1_field2, arrayLength(coefi1_field2));
  // (*readings).IB = calibration_calc((*readings).IB, coefi2_field2, arrayLength(coefi2_field2));
  // (*readings).IC = calibration_calc((*readings).IC, coefi3_field2, arrayLength(coefi3_field2)); 
  
}

void time_calc(int deltaTime, double *timeAcc, int *itr, int *count, int i)
{
  if (deltaTime > 100000)
  {
    deltaTime = 0;
  }
  *timeAcc += deltaTime;
  *itr = i + 1;
  *count += 1;
}

float calibration_calc(float input, float *coef, uint32_t size)
{
  float acc = 0;
  for (int i = 0; i < size; i++)
  {
    acc += coef[i] * pow(input, i);
  }
  return acc;
}

