import numpy as np
from PIL import Image
img = Image.open("in.png") #jpg, png, etc.
pix = img.load()


def sRGBtoLin(colorChannel):
        #// Send this function a decimal sRGB gamma encoded color value
        #// between 0.0 and 1.0, and it returns a linearized value.

    if ( colorChannel <= 0.04045 ):
        return colorChannel / 12.92
    else:
        return np.power((( colorChannel + 0.055)/1.055),2.4)

#Y = (0.2126 * sRGBtoLin(vR) + 0.7152 * sRGBtoLin(vG) + 0.0722 * sRGBtoLin(vB))




hight = img.size[0]
Brightness = 0

Max_Brightness = 0.2126 * sRGBtoLin(255) + 0.7152 * sRGBtoLin(255) + 0.0722 * sRGBtoLin(255)




for x in range(img.size[0]):
    if x % 16 ==0:
        print(str(int((x*100.)/hight))+"%")
    for y in range(img.size[1]):
        Brightness = 0.2126 * sRGBtoLin(pix[x,y][0]) + 0.7152 * sRGBtoLin(pix[x,y][1]) + 0.0722 * sRGBtoLin(pix[x,y][2])
        Brightness =int(round(Brightness/Max_Brightness *255.0))
        pix[x,y] = (Brightness,Brightness,Brightness,255)
       


img.save("output gray scale in a simplified formula.png") 

exit()






