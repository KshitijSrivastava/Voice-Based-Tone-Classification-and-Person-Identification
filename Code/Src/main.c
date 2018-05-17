
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "arm_math.h"
#define USE_STATIC_INIT

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define MAX_BLOCKSIZE   512
uint16_t adc_data[256];
float32_t fft_inp[512];
float32_t fft_mag[256];

uint32_t fftSize = MAX_BLOCKSIZE/2;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;

uint32_t funda_index;
uint32_t funda_value;

uint32_t domi_index;
uint32_t domi_value=0;

float32_t filter_out[128];
float32_t filter_energy[6];
float32_t filter_energy1;

float32_t filter_temp[128];
float32_t temp_fft[128];

float32_t fft_mag_k1;
float32_t dom_freq;

float32_t kshi_aa[6]={90.473,222.704,137.440,38.204,40.660,52.377};
float32_t kshi_o[6]={142.179,95.701,29.240,18.244,13.677,18.432};
float32_t kshi_e[6]={70.346,22.544,7.502,8.968,21.077,40.163};

float32_t aman_aa[6]={54.792,125.485,66.649,17.411,11.935,21.319}; //interchanging aayush_aa
float32_t aman_o[6]={69.827,54.596,16.758,11.126,8.574,7.800};
float32_t aman_e[6]={40.668,13.383,6.927,5.272,5.879,9.626};

float32_t aayu_aa[6]={44.098,93.523,51.963,15.516,9.454,11.942}; //interchanging with aman_aa
float32_t aayu_o[6]={120.094,60.004,8.290,5.189,4.125,5.610};
float32_t aayu_e[6]={54.213,19.796,10.453,8.755,11.088,10.455};

float32_t rms_energy[9];
float32_t temp[6];

float32_t min_dist_value;
uint32_t min_dist_index;


float32_t test[64] =
{
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		1,1,1,1
		};

float32_t filter1[128]={0,0.50,1,0.75,0.50,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

float32_t filter2[128]={0,0,0,0.25,0.5,0.75,1,0.75,0.5,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

float32_t filter3[128]={0,0,0,0,0,0,0,0.25,0.5,0.75,1,0.8,0.6,0.4,0.2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

float32_t filter4[128]={0,0,0,0,0,0,0,0,0,0,0,0.2,0.4,0.6,0.8,1,0.833,0.667,0.5,0.33,0.1667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

float32_t filter5[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1667,0.33,0.5,0.667,0.833,1,0.833,0.667,0.5,0.33,0.1667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

float32_t filter6[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1667,0.33,0.5,0.667,0.833,1,0.875,0.75,0.625,0.5,0.375,0.25,0.125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//float32_t filter7[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.125000000000000,0.250000000000000,0.375000000000000,0.500000000000000,0.625000000000000,0.750000000000000,0.875000000000000,1,0.875000000000000,0.750000000000000,0.625000000000000,0.500000000000000,0.375000000000000,0.250000000000000,0.125000000000000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//float32_t filter8[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.125000000000000,0.250000000000000,0.375000000000000,0.500000000000000,0.625000000000000,0.750000000000000,0.875000000000000,1,0.900000000000000,0.800000000000000,0.700000000000000,0.600000000000000,0.500000000000000,0.400000000000000,0.300000000000000,0.200000000000000,0.100000000000000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//float32_t filter9[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.100000000000000,0.200000000000000,0.300000000000000,0.400000000000000,0.500000000000000,0.600000000000000,0.700000000000000,0.800000000000000,0.900000000000000,1,0.909090909090909,0.818181818181818,0.727272727272727,0.636363636363636,0.545454545454545,0.454545454545455,0.363636363636364,0.272727272727273,0.181818181818182,0.0909090909090909,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//float32_t filter10[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0909090909090909,0.181818181818182,0.272727272727273,0.363636363636364,0.454545454545455,0.545454545454545,0.636363636363636,0.727272727272727,0.818181818181818,0.909090909090909,1,0.923076923076923,0.846153846153846,0.769230769230769,0.692307692307692,0.615384615384615,0.538461538461538,0.461538461538462,0.384615384615385,0.307692307692308,0.230769230769231,0.153846153846154,0.0769230769230769,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//float32_t filter11[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0769230769230769,0.153846153846154,0.230769230769231,0.307692307692308,0.384615384615385,0.461538461538462,0.538461538461538,0.615384615384615,0.692307692307692,0.769230769230769,0.846153846153846,0.923076923076923,1,0.928571428571429,0.857142857142857,0.785714285714286,0.714285714285714,0.642857142857143,0.571428571428571,0.500000000000000,0.428571428571429,0.357142857142857,0.285714285714286,0.214285714285714,0.142857142857143,0.0714285714285714,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//float32_t filter12[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0714285714285714,0.142857142857143,0.214285714285714,0.285714285714286,0.357142857142857,0.428571428571429,0.500000000000000,0.571428571428571,0.642857142857143,0.714285714285714,0.785714285714286,0.857142857142857,0.928571428571429,1,0.941176470588235,0.882352941176471,0.823529411764706,0.764705882352941,0.705882352941177,0.647058823529412,0.588235294117647,0.529411764705882,0.470588235294118,0.411764705882353,0.352941176470588,0.294117647058824,0.235294117647059,0.176470588235294,0.117647058823529,0.0588235294117647,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//float32_t filter13[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0588235294117647,0.117647058823529,0.176470588235294,0.235294117647059,0.294117647058824,0.352941176470588,0.411764705882353,0.470588235294118,0.529411764705882,0.588235294117647,0.647058823529412,0.705882352941177,0.764705882352941,0.823529411764706,0.882352941176471,0.941176470588235,1,0.947368421052632,0.894736842105263,0.842105263157895,0.789473684210526,0.736842105263158,0.684210526315790,0.631578947368421,0.578947368421053,0.526315789473684,0.473684210526316,0.421052631578947,0.368421052631579,0.315789473684211,0.263157894736842,0.210526315789474,0.157894736842105,0.105263157894737,0.0526315789473684,0};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
//	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_data,8000);
//	HAL_Delay(2000);
//	HAL_ADC_Stop_DMA(&hadc1);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
//	HAL_Delay(1000);
//	printf("Buffer is full");
//	int j=0;
//		for(int i=0;i<=16000;i=i+1)
//		{
//			if(i%2 == 0)
//			{
//				fft_inp[i] = adc_data[j];
//				j = j + 1;
//			}
//			else
//			{
//				fft_inp[i]=0;
//			}
//		}
//		arm_status status;                           /* Status of the example */
//		arm_cfft_radix4_instance_f32 S;
//		status = arm_cfft_radix4_init_f32(&S,fftSize,ifftFlag, doBitReverse);
//		arm_cfft_radix4_f32(&S, fft_inp);
//		//arm_max_f32(testOutput, fftSize, &maxValue, &testIndex);
//}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
  	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_data,256);
  	HAL_Delay(1000);
  	HAL_ADC_Stop_DMA(&hadc1);
  	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
  	HAL_Delay(1000);

  int j=0;
  	for(int i=0;i<=512;i=i+1)
  	{
  		if(i%2 == 0)
  		{
  			fft_inp[i] = adc_data[j];
  			j = j + 1;
  		}
  		else
  		{
  			fft_inp[i]=0;
  		}
  	}

  	arm_status status;                           /* Status of the example */
  		arm_cfft_radix4_instance_f32 S;
  //		float32_t maxValue;

  	status = ARM_MATH_SUCCESS;

  	/* Initialize the CFFT/CIFFT module */
  	status = arm_cfft_radix4_init_f32(&S,fftSize,ifftFlag, doBitReverse);

  	/* Process the data through the CFFT/CIFFT module */
  	arm_cfft_radix4_f32(&S, fft_inp);

  	/* Process the data through the Complex Magnitude Module for
  	calculating the magnitude at each bin */
  	arm_cmplx_mag_f32(fft_inp,fft_mag ,fftSize);

  	//For finding the first fundamental frequency and dominant frequency
  	int flag=0;
  	fft_mag_k1=fft_mag[1];
  	for(int k=1;k<256/2;k++)
  	{

  		if (fft_mag[k-1]<fft_mag[k] && fft_mag[k]>fft_mag[k+1] && flag==0)
  		{
  			funda_index=k;
  			funda_value=fft_mag[k];
  			flag=1;
  		}

  		if(fft_mag[k]>domi_value)
  		{
  			domi_index=k;
  			domi_value=fft_mag[k];
  			dom_freq=(domi_index*8000)/256;

  		}

  	}

  	for (int l=0;l<128;l=l+1)
  	{
  		temp_fft[l]=fft_mag[l];
  	}

  	arm_mult_f32(temp_fft,filter1,filter_temp,128);//Filter1
  	arm_mean_f32(filter_temp,128,&filter_energy[0]);

  	arm_mult_f32(temp_fft,filter2,filter_temp,128);//Filter 2
  	arm_mean_f32(filter_temp,128,&filter_energy[1]);

  	arm_mult_f32(temp_fft,filter3,filter_temp,128);//Filter 3
  	arm_mean_f32(filter_temp,128,&filter_energy[2]);

  	arm_mult_f32(temp_fft,filter4,filter_temp,128);//Filter 4
  	arm_mean_f32(filter_temp,128,&filter_energy[3]);

  	arm_mult_f32(temp_fft,filter5,filter_temp,128);//Filter 5
  	arm_mean_f32(filter_temp,128,&filter_energy[4]);

  	arm_mult_f32(temp_fft,filter6,filter_temp,128);//Filter 6
  	arm_mean_f32(filter_temp,128,&filter_energy[5]);

  	// For classification - Calculating Distance
  	arm_sub_f32(filter_energy,kshi_aa,temp,6); //For Kshitij
  	arm_rms_f32(temp,6,&rms_energy[0]);

  	arm_sub_f32(filter_energy,kshi_o,temp,6);
  	arm_rms_f32(temp,6,&rms_energy[1]);

  	arm_sub_f32(filter_energy,kshi_e,temp,6);
  	arm_rms_f32(temp,6,&rms_energy[2]);

  	arm_sub_f32(filter_energy,aman_aa,temp,6); // For Aman
  	arm_rms_f32(temp,6,&rms_energy[3]);

  	arm_sub_f32(filter_energy,aman_o,temp,6);
  	arm_rms_f32(temp,6,&rms_energy[4]);

  	arm_sub_f32(filter_energy,aman_e,temp,6);
  	arm_rms_f32(temp,6,&rms_energy[5]);

  	arm_sub_f32(filter_energy,aayu_aa,temp,6);// For Aayush
  	arm_rms_f32(temp,6,&rms_energy[6]);

  	arm_sub_f32(filter_energy,aayu_o,temp,6);
  	arm_rms_f32(temp,6,&rms_energy[7]);

  	arm_sub_f32(filter_energy,aayu_e,temp,6);
  	arm_rms_f32(temp,6,&rms_energy[8]);

  	arm_min_f32(rms_energy,9,&min_dist_value,&min_dist_index); //Finding the minimum distance and value




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_8;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_MultiModeTypeDef multimode;
  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
