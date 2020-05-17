import random
from PIL import Image

background_gray_scale = 185

img = Image.new('RGB', (200, 200), color = (background_gray_scale,background_gray_scale,background_gray_scale))
pix = img.load()

#色相用6个1000来表示。所以从红到黄，色相就是0到1000，黄到绿是1000到2000
hue = random.randint(0,5999)
#饱和度也用0到1000.
saturation = random.randint(0,1000)
#亮度用0到255，没什么好换算的。
velocity = random.randint(0,255)

#在这个地方写测试值，或者限制在新的范围内。
hue = random.randint(2000,3500)
saturation = random.randint(0,1000)
velocity = random.randint(155,255)


if hue<0:
    hue=hue+6000

channel_max = velocity
channel_min = velocity*(1000-saturation)/1000.0
ratio = 0
hue_mod_2000 = hue%2000
if hue_mod_2000<=1000:
    ratio = hue_mod_2000/1000.
else:
    ratio = (2000-hue_mod_2000)/1000.
channel_middle = channel_max*ratio+channel_min*(1-ratio)



channel_max = round(channel_max)
channel_middle = round(channel_middle)
channel_min = round(channel_min)


color = (255,0,255)
if hue<1000:
    color = (channel_max , channel_middle , channel_min)
elif hue<2000:
    color = (channel_middle , channel_max , channel_min)
elif hue<3000:
    color = (channel_min, channel_max , channel_middle)
elif hue<4000:
    color = (channel_min , channel_middle, channel_max)
elif hue<5000:
    color = (channel_middle, channel_min , channel_max)
else:
    color = (channel_max  , channel_min, channel_middle)



#random_color = (random.randint(0,255),random.randint(0,255),random.randint(0,255))

for x in range(50,150):
    for y in range(50,150):
        delta_x = x-100
        delta_y = y-100
        if(delta_x*delta_x+delta_y*delta_y)<2500:
            pix[x,y] = color






img.save('random color.png')
