# An image-proseccor (or BMP photo editor)
 
This is a сonsole application that allows to apply filters to pictures in BMP format.

## Comand line arguments format

{program name} {input file adress} {output file adress} [-{name of the 1st filter} [1st filter parameter] [2nd filter parameter] ...] [-{name of the 2nd filter} [1st filter parameter] [2nd filter parameter] ...] ...

### Example:

./image_processor input.bmp /tmp/output.bmp -crop 800 600 -gs -blur 0.5

## Available filters

#### Crop (-crop width height)
Crops an image to given width and height. Left top part of an image is being used.

If the given width and height are more than the size of the origin photo than nothing happens.

#### Grayscale (-gs)
The photo is being transformed to black and white using the following formula

![encoding](https://latex.codecogs.com/svg.image?R'%20=%20G'%20=%20B'%20=0.299%20R%20&plus;%200%20.587%20G%20&plus;%200%20.%20114%20B)

#### Negative (-neg)
The photo is being transformed to negative using the following formula

![encoding](https://latex.codecogs.com/svg.image?R'%20=%201%20-%20R,%20G'%20=%201%20-%20G,%20B'%20=%201%20-%20B)

#### Sharpening (-sharp)
The following matrix is being used

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%205%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

#### Edge Detection (-edge threshold)
The photo is being transformed to black and white and then the following matrix is being used.

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%204%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

Than pixels with a value that exceed 'threshold' are colores into while, others are coloured into black.

#### Gaussian Blur (-blur sigma)

The values of every color of every pixel `C[x0][y0]` is determined by the following formula.

![encoding](https://latex.codecogs.com/svg.image?C%5Bx_0%5D%5By_0%5D%20=%20%5Csum_%7Bx=0,y=0%7D%5E%7Bwidth-1,%20height-1%7DC%5Bx%5D%5By%5D%5Cfrac%7B1%7D%7B%5Csqrt%5B%5D%7B2%5Cpi%5Csigma%5E2%7D%7De%5E%7B-%5Cfrac%7B%5Cleft%7Cx_o-x%5Cright%7C%5E2%20&plus;%20%5Cleft%7Cy_o-y%5Cright%7C%5E2%7D%7B2%5Csigma%5E2%7D%7D)

#### Blocks filter (-blocks width height)
Turns the picture into a group of pixels of the given size (just makes the pixels larger and worsens the quality of the picture)
