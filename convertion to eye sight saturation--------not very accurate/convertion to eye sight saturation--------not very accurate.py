if(1):
    print('this version doesn\'t give out a very reasonable rerult. Tht core formula is IMAGINED BY ME. Don\' trust it too much. If you would like it to work anyway, open this .py file with any text editor, modify the first line as if(0): and DON\'T edit anything else if you are not capable of programming.')
    exit()

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




Max_Brightness = 0.2126 * sRGBtoLin(255) + 0.7152 * sRGBtoLin(255) + 0.0722 * sRGBtoLin(255)

Linear_R=0.2126 * sRGBtoLin(0)
Linear_G=0.7152 * sRGBtoLin(255)
Linear_B=0.0722 * sRGBtoLin(0)
Sum_of_Linear = Linear_R+Linear_G+Linear_B
Sum_of_delta_of_Linear = abs(Sum_of_Linear*0.2126-Linear_R)+abs(Sum_of_Linear*0.7152-Linear_G)+abs(Sum_of_Linear*0.0722-Linear_B)

#In my test, max saturation appears when ff00ff or 00ff00. Purple or cya
Max_Saturation = Sum_of_delta_of_Linear/Max_Brightness


#now I need the table to look up the linear rgb value of gray color with the same brightness 
gray_scale_brightness_table = [sRGBtoLin(i) for i in range(256)]





hight = img.size[0]
for x in range(img.size[0]):
    if x % 16 ==0:
        print(str(int((x*100.)/hight))+"%")
    for y in range(img.size[1]):
        

        Linear_R=0.2126 * sRGBtoLin(pix[x,y][0])
        Linear_G=0.7152 * sRGBtoLin(pix[x,y][1])
        Linear_B=0.0722 * sRGBtoLin(pix[x,y][2])
        Sum_of_Linear = Linear_R+Linear_G+Linear_B

        #the following line is according to my own imagination. It's not from any widely received researchment.
        Sum_of_delta_of_Linear = abs(Sum_of_Linear*0.2126-Linear_R)+abs(Sum_of_Linear*0.7152-Linear_G)+abs(Sum_of_Linear*0.0722-Linear_B)

        Saturation = int(round(Sum_of_delta_of_Linear/Max_Saturation/Max_Brightness*255.0))


        pix[x,y] = (Saturation,Saturation,Saturation,255)
       


img.save("output eye sight saturation--------not very accurate.png") 

exit()






