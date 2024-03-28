[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/ea68GWBk)
# lab4-pong-skeleton

    * Name: 
    * Github username: 
    * Github Repository URL: 
    * Description of hardware: (embedded hardware, laptop, etc) 

## Github Repo Submission Resources

## Q1
The ST7735 LCD controller differentiates between  command and data based on the state of the Data/Command pin. In the ST7735 code provided, the controller toggles between command and data states by setting or clearing the LCD_DC bit in the LCD_PORT register. When the DC pin is low (cleared), the transmission is seen as a command; concverly when the DC pin is low (set), the transmission is seen as data.

## Q2
The SPI is represented as is 8 bits, but colors are represented as 16 bits, so the color value is split into two 8-bit packets, which are sent over the SPI interface. The functions that handle this are the SPI_ControllerTx_16bit and SPI_ControllerTx_16bit_stream functions, which use bitwise operations to split the 16-bit values.

## Q3
The LCD_setAddr function defines a rectangular area on the screen where we can fill pixels.

By only setting the addresses of the pixels where you will draw your image, you can efficiently update specific parts of the screen, which elminates the need to refresh the entire screen.

## Q4
There are differences in the colors shown on the screeb because a 24-bit RGB can represent 2^8*2^8*2^8 shades, whereas 16-bit RGB represents 2^5*2^6*2^5 shades. However, for our application (pong), where we only use 3 colors, its is completely find to use 16-bit RGB values. 


## Q5
No, we don't need debouncing. We are converting a range of voltage inputs into a simple "up," "down," or "zero" vertical motion so it doesn't need to be accuracte as long as there are three difference responses depending on the postion of the joystick. 

* [ESE5160 Example Repo Submission](https://github.com/ese5160/example-repository-submission)
* [Markdown Guide: Basic Syntax](https://www.markdownguide.org/basic-syntax/)
* [Adobe free video to gif converter](https://www.adobe.com/express/feature/video/convert/video-to-gif)
* [Curated list of example READMEs](https://github.com/matiassingers/awesome-readme)
* [VS Code](https://code.visualstudio.com/) is heavily recommended to develop code and handle Git commits
  * Code formatting and extension recommendation files come with this repository.
  * Ctrl+Shift+V will render the README.md (maybe not the images though)
