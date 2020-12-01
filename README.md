# APA102 LED Library
Click on the image to watch a demonstration video
[![Watch the video](https://user-images.githubusercontent.com/57394912/100681331-c425cf80-3373-11eb-8953-f07d3ce68d79.png)](https://bit.ly/embapaprojekt)

## Use of the library and included functionality
The library was made with flexibility/readability and ease of use in mind and is made with procedural 
function-based code for the user to handily adjust for their own needs if needed.
 
### apaControl - main controlling function of the library

![image](https://user-images.githubusercontent.com/57394912/100681173-701aeb00-3373-11eb-9933-f77cd8892f6c.png)

This function is made for user to easily control any kind of APA102 component – be it LED strip, circle, matrix etc. by adjusting parameters based on the type of component. 
Parameters:
1.	**bright** = brightness of the LEDs – set 1-31 with 31 being brightest
2.	**ledcount** = total number of LEDs in the component
3.	**effectname** = name of the effect with each effect having its specific number, 99 being static lit up LEDs with default color set to purple-ish  
4.  **red, green, blue** color mixture by setting up 0-255 values
---

Effect name | Effect number
------------ | -------------
movingled | 11
matrixeffect | 22
nyancat | 33
staticlights | 99

#### movingled(int bright, int ledcount, int red, int green, int blue)
Single LED moving by one step to the next one in the sequence with color and brightness set based on the values in the parameters

#### matrixeffect(int bright, int ledcount)  
Matrix effect based on the Movie Matrix with symbols moving down vertically in iconic green color, this effect is the best with LED matrix e.g. 8x8

#### nyancat(int bright, int ledcount)
Upgraded version of movingled function with nyan cat theme colors

#### staticlights(int bright, int red, int green, int blue, int ledcount)
Staticly lit up LEDs with the colour and brightness set in the parameters

---

## Small structural functions in the library
#### buildLED(int bright, int red, int green, int blue)
Used to send 4-byte frame with the colour and brightness(1-31) set in the parameters
#### emptyFrame(void)
Sends empty 4-byte frame, used for start and end frame
#### resetBoard(int ledcount)
Clears (turns off) all the LEDs set by the number in ledcount parameter

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
