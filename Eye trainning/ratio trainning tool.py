from PIL import Image, ImageDraw
import random
import math
import time

mode = 1 #not implemented yet
delay_duration = 2# in seconds.
background_color = 0x7f7f7f

resolution = (2560,1440)
#valid values: tuple(of 2 int) or '1k' or '2k'

noise_mode = 1    
#valid values:
# 1~4 for light ~ heavy noise, other value for nothing.

fill_inner = 1
#valid values:
# 1 for fill, other value for nothing.




#functions for all situations
def random_dark_gray():
    temp = temp = random.random()
    temp *= temp
    temp *=1.15
    temp -=0.15
    temp = max(0,temp)
    color = math.floor(0x33*temp)
    color = color+color*256+color*256*256
    return color

def resolution_look_up(input):
    if type(input)!='string' :
        return input
    
    name = input.lower()
    
    if name=='1k':
        return(1920,1080)

    if name=='2k':
        return(2560,1440)

def grey_index():
    return 1+256+256*256
###################

image_resolution = (resolution[0]-2,resolution[1]-152)
im = Image.new('RGB', image_resolution, color = background_color)



#============ difficulty ============
#answer = random.randint(4,20)/20#esay
#answer = random.randint(14,30)/30#esay
#answer = random.randint(8,40)/40#normal
#answer = random.randint(18,40)/40#normal
answer = random.randint(60,90)/100#hard
#or customize the answer pool.
# answer_options = [0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1]
# answer = random.choice(answer_options)
#============ difficulty ============

#core data
width_options = [200,225,250,275,300,330,360,400,450,500]
width = random.choice(width_options)

temp = random.random()
temp = temp*0.4-0.15
temp = max(0,temp)
move_up_ratio = random.random()*temp
temp = 'invalid'

#control the core data here for debug purpose.



#calculate other data
height = math.floor(width*answer)
top = (im.size[1]-height)/2 *(1-move_up_ratio)
bottom = top+height
left = (im.size[0]-width)/2
right = (im.size[0]+width)/2

#tails
tail_space_length = im.size[1]-bottom
avg_tail_length = min(tail_space_length*0.5,height)
tail_length_difference = (200+height*0.1)/2
temp = random.random()
temp = temp*2-1
left_tail_length = avg_tail_length+100*temp
right_tail_length = avg_tail_length-100*temp
temp = 'invalid'
    #safety check
left_tail_length = max(0,min(tail_space_length,left_tail_length))
right_tail_length = max(0,min(tail_space_length,right_tail_length))

#color
color = random_dark_gray()#or fill=0xffffff for white rectangle


#draw~
draw = ImageDraw.Draw(im)


color_options = [
    background_color,
    background_color+grey_index()*100,
    background_color+grey_index()*50,
    background_color+grey_index()*30,
    background_color+grey_index()*(-30),
    background_color+grey_index()*(-50),
    background_color+grey_index()*(-100),
    ]
random_index = random.choice(range(len(color_options)))
fill_color =color_options[random_index]

#fill inner part
if fill_inner in[1]:
    draw.rectangle(xy=((left+1,top+1),(right-1,bottom-1)),fill=fill_color)

if 0==random_index:
    random_index = random.choice([1,5,6])
    fill_color =color_options[random_index]

#outlines
    #top
draw.line((left,top)+(right,top), fill=fill_color)
    #bottom
draw.line((left,bottom)+(right,bottom), fill=fill_color)
    #left
draw.line((left,top)+(left,bottom+left_tail_length), fill=fill_color)
    #right
draw.line((right,top)+(right,bottom+right_tail_length), fill=fill_color)

color_options = 'invalid'




#scattering noise rects
if noise_mode in [1,2,3,4]:
    count = random.randint(round(width*0.03),round(width*0.07))
    x_coord = random.random()*10000
    y_coord = random.random()*10000
    x_coord_random_stride = random.random()*3457891
    y_coord_random_stride = random.random()*5676782
    x_mod = width/1.5
    y_mod = height/1.5
    color_options = [
        #background_color+grey_index()*30,
        background_color+grey_index()*25,
        background_color+grey_index()*20,
        background_color+grey_index()*15,
        background_color+grey_index()*(-15),
        background_color+grey_index()*(-20),
        background_color+grey_index()*(-25),
        #background_color+grey_index()*(-30),
        ]

    mid_point_of_img = (resolution[0]/2,(top+bottom)/2)

    #adjusting
    if 2==noise_mode:
        count = random.randint(100,150)
    if 3==noise_mode:
        count = random.randint(200,275)
        color_options.append(background_color+grey_index()*(-30))
    if 4==noise_mode:
        count = random.randint(300,400)
        color_options.append(background_color+grey_index()*30,
        background_color+grey_index()*(-30),
        background_color+grey_index()*(-35),
        background_color+grey_index()*(-40))

    for i in range(count):
        #scale
        delta_of_top_left = i%4+4
        delta_of_bottom_right = (math.floor(i/4))%4+4
        #position
        x_mid = x_coord % x_mod
        y_mid = y_coord % y_mod
        if i%2 :
            x_mid=mid_point_of_img[0]+x_mid
        else:
            x_mid=mid_point_of_img[0]-x_mid

        if (i>>1)%2 :
            y_mid=mid_point_of_img[1]+y_mid
        else:
            y_mid=mid_point_of_img[1]-y_mid
        #final coordinates
        top_left = (x_mid-delta_of_top_left,y_mid-delta_of_top_left)
        bottom_right = (x_mid+delta_of_bottom_right,y_mid+delta_of_bottom_right)
        
        fill_color = color_options[i% len(color_options)]
        #draw call
        draw.rectangle(xy=(top_left,bottom_right),fill=fill_color)#background_color+grey_index()*(i%7-5)*4)
        
        #late update
        x_coord=x_mid+x_coord_random_stride
        y_coord=y_mid+y_coord_random_stride
        x_mod+=3
        y_mod+=3*(9./16.)
    ################



del draw

#save image
im.save('puzzle.png')

#defer for few seconds and then print the answer in console. So that let user to train with only console window and image browser window.
time.sleep(delay_duration)
display_answer = round(height/width,ndigits=3)
print('answer: '+str(display_answer) +'   width: '+str(width)+'   height: '+str(round(height)))
if(0):
    print('--------------------')
    print('debug info:')
    print('--------------------')
    print("width"+str(width))
    print("height"+str(height))
    print("move_up_ratio"+str(move_up_ratio))
    
    print("top"+str(top))
    print("bottom"+str(bottom))
    print("left"+str(left))
    print("right"+str(right))
    print("left_tail_length"+str(left_tail_length))
    print("right_tail_length"+str(right_tail_length))
    print('--------------------')

