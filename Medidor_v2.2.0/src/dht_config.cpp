/**
 * @file dht_config.cpp
 * @brief Arquivo de configuração para o cálculo da distorção harmônica total a partir da FFT
 *
 * @author --
 * @date 02 de Agosto de 2021
 */

#include "dht_config.h"
#include <cstdlib>
float* fft4(float* input, unsigned N){
  // constants
  unsigned rdx = 4, N4 = N/rdx; 
  float pi2 = 6.283185307179586, c = pi2/N4, d = pi2/N;
  // declarations
  float accCos=0, twdlCos=0, accSin=0, twdlSin=0, mag_re=0, mag_im=0; 
  float *re, *im, *reW, *imW;
  float *mag = (float*) malloc(sizeof(float) * N/2);

  // processing
  for(int k=0; k<N4; k++)
  {
    re = (float*) calloc(rdx, sizeof(float));
    im = (float*) calloc(rdx, sizeof(float));
    reW = (float*) calloc(rdx, sizeof(float));
    imW = (float*) calloc(rdx, sizeof(float));
    
    for(int n=0; n<N4; n++) // N/4-dft
    {
      accCos = cos(c*n*k);
      accSin = sin(c*n*k);
      for(int i=0; i<rdx; i++)
      {
        re[i] += input[rdx*n+i]*accCos;
        im[i] -= input[rdx*n+i]*accSin;
      }
    }
    for(int j=0; j<rdx; j++) // apply twiddle factors
    {
      twdlCos = cos(-d*k*j);
      twdlSin = sin(-d*k*j);
      reW[j] = re[j]*twdlCos - im[j]*twdlSin;
      imW[j] = re[j]*twdlSin + im[j]*twdlCos;
    }
    // assign to mag vector
    mag_re = reW[0] + reW[1] + reW[2] + reW[3];
    mag_im = imW[0] + imW[1] + imW[2] + imW[3];
    mag[k] = sqrt(pow(mag_re,2)+pow(mag_im,2)) * 2/N;
    mag_re = reW[0] + imW[1] - reW[2] - imW[3];
    mag_im = imW[0] - reW[1] - imW[2] + reW[3];
    mag[k+N4] = sqrt(pow(mag_re,2)+pow(mag_im,2)) * 2/N;
    
    free(re);
    free(im);
    free(reW);
    free(imW);
  }
  mag[0] = mag[0]/float(2);
  return mag;
}

float compute_dht(uint16_t* input, int samples, int tdm_idx, int num_chan, float offset, float scaling, esp_adc_cal_characteristics_t adcCalibration)
{
  int k;
  float dht, fundAmplitude;
  float *mag, *waveform;

  waveform = (float*) calloc(samples, sizeof(float));
  for(k=0; k<samples; k++) waveform[k] = ((float) esp_adc_cal_raw_to_voltage(input[num_chan*k+tdm_idx], &adcCalibration) / float(1000) - float(offset)) * float(scaling);
  
  mag = fft4(waveform, FFT_N);
  
  /*
  printf("mag%d = np.array([", tdm_idx);
  for (int ii=0; ii<FFT_N/2-1; ii++) printf("%f, ", mag[ii]);
  printf("%f])\n", mag[FFT_N/2-1]);
  */

  dht = float(0);
  fundAmplitude = mag[DECI_FACTOR];
  for (k=2; k<=MAX_HARM; k++) dht += pow(mag[DECI_FACTOR*k],2);
  dht = sqrt(dht)*100/fundAmplitude;

  free(mag);
  free(waveform);
  
  return dht;
}
