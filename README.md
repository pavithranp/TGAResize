<b>TGAResize </b>

Console application to produce TGA file of half the dimensions of input.
Supports 24bit RGB and 32 bit RGBA only, RLE compression not included.

Usage: <b>tgaresize.exe input file name output-name <optional field 0/1 for sampling method> </b>
  
  1. The application uses the tga specified structure used in the link http://www.paulbourke.net/dataformats/tga/ 
     All the header portion was read and copied to a temp .tga file except for the image width and height, which were halved before     copying. Any unsupported formats from the header, will be reported with an error.
     The image data portion was a single array of all the elements based on tga header(either RGB/RGBA). This was traversed as a two dimesional array using the width and height of the image.
  
  2. Two algorithms have been implemnted for resize:
     1. Down sampling(Default) - selects one elment out of four elements 
     2. 2x2 kernel average - selects four neighbouring pixels and averages and copies them to one pixel
  
 
