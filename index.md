# Voice based Tone and Person Identification on STM32L4

This project was made as part of EED308- Embedded System Hardware, Spring 2018 at Shiv Nadar University under the supervised guidance of Professor R.N Biswas.

In this project, we created a smart voice based human identification system that is capable of recognizing periodic sounds like aa, ee & oo and the human who is pronouncing them. The audio input is sampled through a microphone and analyzed in real time using STM32L4 series microcontroller. Our system is trained to recognize  the voice of the speaker on which the model is trained in our case we three. In addition, it is also capable of identifying the sounds of aa,ee,oo pronounced by any human being  in real time. Some of the topics we explored extensively in this project are Fast Fourier Transform, human speech analysis, MFCC and Machine Learning techniques for classification.

## Key Points for this Project 

### Usage of IDE's

Free versions of IDE's like Keil, IAR wont work with CMSIS DSP Library, since they have limited memory in the free version. Instead free IDE's like Atollic True Studio, GNU Eclipse based free IDE's should be used which dont have the memory constraint.

### Installation of CMSIS DSP Library

Download the CMSIS Library from the following [link](https://github.com/ARM-software/CMSIS_5) and follow the instruction as given in the [video](https://www.youtube.com/watch?v=dLHkYlF3suc&feature=youtu.be).

## Block Diagram

![Procedure](https://github.com/KshitijSrivastava/Voice-Based-Tone-Classification-and-Person-Identification/images/Procedure.JPG)







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
