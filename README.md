TGAResize

Console application to produce TGA file of half the dimensions of input.
Supports 24bit RGB and 32 bit RGBA only, RLE compression not included.

Usage: tgaresize.exe <input file name> <output name > <optional field 0/1 for sampling method>
  
  1. The application uses the tga specified structure used in the link http://www.paulbourke.net/dataformats/tga/ 
     All the header portion was read and copied to a temp .tga file except for the image width and height, which were halved before     copying. Any unsupported formats from the header, will be reported with an error.
     The image data portion was a single array of all the elements based on tga header(either RGB/RGBA). This was traversed as a two dimesional array using the width and height of the image.
  
  2. Two algorithms have been implemnted for resize:
     1. Down sampling(Default) - selects one elment out of four elements 
     2. 2x2 kernel average - selects four neighbouring pixels and averages and copies them to one pixel
  
  3. Problems faced:
     1. TGA IDlength was 0 in most images tested. certain images which had imageID were producing corrupted/distorted half images. After reviewing the code multiple times and debugging, I found that the image ID was being interpreted as image pixels, which was causing the problem. I fixed it by copying the imageID before reading the image data.
     2. ODD numbered dimenions were causing segmentation faults. after some code review and debugging, realised that I was trying to read outside the array when the Image was odd numbered height or width.I fixed it by making the image width and height even
     3. using wrong image formats for debugging  resulted in some time waste.
     4. Using GIMP for debugging helped save some time, as GIMP always showed the image even if it was corrupted. This helped identify the TGA ID length issue mentioned above.
  
  4. Multiple TGA read/write libraries were refered to make this application. The goal was to make the code as simple as possible. and not support anything that was not required. These libraries
  http://www.paulbourke.net/dataformats/tga/
  https://github.com/ColumbusUtrigas/TGA
  https://github.com/npedotnet/TGAReader 
