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


debug_max = 0

for x in range(img.size[0]):
    if x%16 ==0:
        print(str(int((x*100.)/hight))+"%")
    for y in range(img.size[1]):

            #step 1 http://www.easyrgb.com/en/math.php  standard RGB to XYZ

        var_R = pix[x,y][0] / 255.0
        var_G = pix[x,y][1] / 255.0
        var_B = pix[x,y][2] / 255.0

        if ( var_R > 0.04045 ) :
            var_R = pow( ( var_R + 0.055 ) / 1.055  ,2.4)
        else:                   
            var_R = var_R / 12.92
        if ( var_G > 0.04045 ) :
            var_G = pow( ( var_G + 0.055 ) / 1.055  ,2.4)
        else:
            var_G = var_G / 12.92
        if ( var_B > 0.04045 ):
            var_B = pow( ( var_B + 0.055 ) / 1.055  ,2.4)
        else:
            var_B = var_B / 12.92

        var_R = var_R * 100.0
        var_G = var_G * 100.0
        var_B = var_B * 100.0

        #X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805
        Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722
        #Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505
        
        #step2 XYZ to Cie Lab

        var_Y = Y / 100.0 # Reference_Y==100
        if ( var_Y > 0.008856 ):
            var_Y =pow( var_Y , ( 1.0/3.0 ))
        else:
            var_Y = ( 7.787 * var_Y ) + ( 16.0 / 116.0 )
        CIE_L = ( 116.0 * var_Y ) - 16


        debug_max = max(CIE_L,debug_max)
        #get the value into integer
        CIE_L = int(CIE_L*2.55)
        pix[x,y] = (CIE_L,CIE_L,CIE_L,255)
       

print(debug_max)
img.save("output eye sight brightness according to rgb to lab formula.png") 

exit()






