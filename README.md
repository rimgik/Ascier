# Ascier
Ascier is a C++ cli that can convert any images/videos into ascii art

# Usage 
command line arguments
```
-i              Path of target image
                If this flag is not specify, ascier will take rgb24 pixel stream from stdin instead
-width          Width of the input image from stdin
-height         Height of the image imgae from stdin
-scale          Scaling factor of final image, default to 1
-pixelWidth     How times each pixel is printed in the horizontal direction, default to 2
-o              Path of output text file
                If this flag is not specify, ascier will output the art through stdout
```
# Example
Convert test.png into ascii art into stdout with final image scaled half\
``` Ascier -i ./test.png -scale .5 ``` <br>
<br>Into output.txt instead of stdout\
``` Ascier -i ./test.png -scale .5 -o output.txt ```
<br>
## Pair with ffmpeg
You can convert videos into ascii art, by first turning the video into a **rgb24 pixel stream**, then feed it through the stdin of Ascier
The easier way to do this was probably through **ffmpeg** <br>
<br>This turn **rickroll.mp4 (640x360)** into AsciiArt, the output file are place under **frames** folder<br>
```ffmpeg -i rickroll.mp4 -pix_fmt rgb24 -f rawvideo - | Ascier -width 640 -height 360 -scale .5 -o ./frames/%03d.txt```<br>

https://github.com/rimgik/Ascier/assets/109288765/c5c4f913-d3cf-4714-a2d1-e95ba600358d

Note :\
```ffmpeg -i rickroll.mp4 -pix_fmt rgb24 -f rawvideo``` generate **rgb24 pixel stream**\
       ```%03d``` is a format specifier and will be replace by interger starting from 0
