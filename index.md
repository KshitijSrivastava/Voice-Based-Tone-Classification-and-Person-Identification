# Voice based Tone and Person Identification on STM32L4

This project was made as part of EED308- Embedded System Hardware, Spring 2018 at Shiv Nadar University under the supervised guidance of Professor R.N Biswas.

In this project, we created a smart voice based human identification system that is capable of recognizing periodic sounds like aa, ee & oo and the human who is pronouncing them. The audio input is sampled through a microphone and analyzed in real time using STM32L4 series microcontroller. Our system is trained to recognize  the voice of the speaker on which the model is trained in our case we three. In addition, it is also capable of identifying the sounds of aa,ee,oo pronounced by any human being  in real time. Some of the topics we explored extensively in this project are Fast Fourier Transform, human speech analysis, MFCC and Machine Learning techniques for classification.

## Key Points for this Project 

### Usage of IDE's

Free versions of IDE's like Keil, IAR wont work with CMSIS DSP Library, since they have limited memory in the free version. Instead free IDE's like Atollic True Studio, GNU Eclipse based free IDE's should be used which dont have the memory constraint.

### Installation of CMSIS DSP Library

Download the CMSIS Library from the following [link](https://github.com/ARM-software/CMSIS_5) and follow the instruction as given in the [video](https://www.youtube.com/watch?v=dLHkYlF3suc&feature=youtu.be).

## Block Diagram

![Procedure](images/Procedure.JPG)

## Materials needed
- Analog Mic/ Sound Detector Module
- Resistance and Capacitances to make a low pass filter
- LED


## Initialization in Cube Mx

### For ADC-DMA
We need sampling frequency of the ADC to be around 8k Hz since the voice signal can hardly exceed this voice.
We initialized ADC DMA in Continuous Mode with High Speed Clock Frequency as 16 Mhz. The Prescaler value of 8 was chosen so that ADC Clock Frequency=2Mhz. Nnumber of cycles is chosen to be 247.5, so that the sampling freq= 7600 Hz.

Sampling frequency=2 Mhz/(247.5 + Tconv), where Tconv=12.5 for STM32L4. Refer to this document- [link](www.st.com/resource/en/application_note/cd00258017.pdf)

Total of 256 data points is being stored in the array which is stored circularly.


### For Interrupt Generation

PC13 could also be used as an interrupt to start the ADC-DMA whenever the voice signal exceed a threshold value that is set on the analog microphone(The Digital Output of Mic connected to the PC13). Whenever the voice exceed the threshold, the program would go as the Interrupt Service Routine.

For delay to work in the interrupt service routine, In the NVIC set the GPIO interrupt priority to a higher value than the delay intrrupt.  

### For LED Pins
3 LED pins were chosen for the purpose of showing which person is speaking, 3 LED was used for showing what alphabet is he being spoken, 1 LED for showing misclassified answer and 1 LED to know when DMA is storing the data.

![PIN Diagram](images/Pin_diagram.JPG)

## Algorithmic Development

We initialized ADC DMA and took 256 samples of the data with the sampling freq= 7600 Hz.
Then we computed 256 length FFT using the CMSIS DSP library and extracted the dominant frequency of the voice by finding the maximum value of the FFT and its corresponding frequency. This dominant frequency found was used for real time classification of O, Aa, E.

Further to improve the accuracy and robustness of our system MFCC was used to identify person classification based on a trained model
The Procedure to use MFCC (Mel Frequency Cepstral Coefficient) was to use 6 filter banks for the corresponding FFT size of 256.

Then Passing the same through the FFT of the signal through each filter banks and then computing the energy for each signal passed through the filter banks. This forms the feature vector.

![MFCC](images/Mel_filterbank.JPG)



You can use the [editor on GitHub](https://github.com/KshitijSrivastava/Voice-Based-Tone-Classification-and-Person-Identification/edit/master/index.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/KshitijSrivastava/Voice-Based-Tone-Classification-and-Person-Identification/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
