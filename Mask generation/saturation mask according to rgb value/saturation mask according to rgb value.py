# import os
# from os import walk

# f = []
# for (dirpath, dirnames, filenames) in walk(os.getcwd()):
#     print(filenames)
#     #f.extend(filenames)
#     break

# exit(0)


import numpy as np
from PIL import Image
img = Image.open("in.png") #jpg, png, etc.
pix = img.load()


max = 0.
min = 0.
ratio =0. 
hight = img.size[0]

for x in range(img.size[0]):
    if x % 16 ==0:
        print(str(int((x*100.)/hight))+"%")
    for y in range(img.size[1]):
        max = np.max(pix[x,y][0:3])
        min = np.min(pix[x,y][0:3])
        if 0==max:
            ratio = 0
        else:
            ratio = int(255*(min/max))
        pix[x,y] = (ratio,ratio,ratio,255)
       


img.save("output saturation mask according to rgb value.png") 

exit()






