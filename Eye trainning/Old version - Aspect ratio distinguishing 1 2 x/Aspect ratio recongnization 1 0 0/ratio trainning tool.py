from PIL import Image, ImageDraw
import random
import math
import time

# Notice:
# For draw functions in pillow, the start point and end point are envolved in to a draw function.
# When draw a line for (1,1)+(1,3), 3 pixils are modified.
# When draw a rectangle with param of (1,1),(2,2), 4 pixils are modified.
# Which are not the same as discribed in its docs.
# Another thing, for a frame style, distance should be measured like
#  | . . . . |
#  ^1 2 3 4 5^  aligns to the mid of lines.
# But for a filled shape, it should look like
#  | | | | | |
# ^1 2 3 4 5 6^  aligns to the border.
# https://github.com/YagaoDirac/Dirac-s-Painting-Tool for more info and discord.



mode = 1 #not implemented yet
delay_duration = 1# in seconds.
background_color = 0x7f7f7f


#============ difficulty for mode 1 ============
#answer = random.randint(2,10)/10#warm up
#answer = random.randint(6,7)/10+0.05#warm up 2
#answer = random.randint(4,20)/20#esay
#answer = random.randint(7,24)/20#esay 2
#answer = random.randint(7,23)/20+0.025#esay 2
#answer = random.randint(14,30)/30#esay
answer = random.randint(8,48)/40#normal
#answer = random.randint(18,40)/40#normal
#answer = random.randint(50,100)/100#hard
#or customize the answer pool.
#answer_options = [0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1,1,1,1,1,1,1,1]#even duplicatable.
#answer_options = [0.6,0.6,0.65,0.7,0.7,0.7,0.75,0.8,0.8]
#answer = random.choice(answer_options)
#============ difficulty ============






resolution = (2560,1440)
#valid values: tuple(of 2 int) or '1k' or '2k'

noise_mode = 1    
#valid values:
# 1~4 for light ~ heavy noise, other value for nothing.

fill_mode = 'random'
#valid values:
# 'frame mode'
# 'normal filling'
# 'random'

auxiliary_line_mode = 'none'
#valid values:
# 'x mark' for x mark,
# 'circle' for a perfect circle in the mid.
# 'random'
# all others for 'none'

train_mode = True
#bool type. Receives only true and false.
ref_line_distance = 1/40 #temporarily not used.
ref_line_count = 4#temporarily not used.
ref_line_ratios = [0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2]


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
############################

def resolution_look_up(input):
    if type(input)!='string' :
        return input
    
    name = input.lower()
    
    if name=='1k':
        return(1920,1080)

    if name=='2k':
        return(2560,1440)
############################

def grey_index():
    return 1+256+256*256
############################

def get_actual_fill_mode(input):
    if 'frame mode' == input:
        return input
    if 'normal filling'== input:
        return input
    if 'random' == fill_mode:
        temp = random.random()
        if temp<0.3:
            return 'frame mode'
        else:
            return 'normal filling'

    #fallback as normal filling.
    return 'normal filling'
############################

def get_actual_auxiliary_line_mode(input):
    if 'x mark' == input:
        return input
    if 'circle' == input:
        return input
    if 'random' == input:
        temp = random.random()
        if temp<0.5:
            return 'none'
        elif temp<0.75:
            return 'x mark'
        else:
            return 'circle'
    #fall back
    return 'none'
        

###################

image_resolution = (resolution[0]-2,resolution[1]-152)
im = Image.new('RGB', image_resolution, color = background_color)



#core data
width_options = [200,225,250,275,300,330,360,400,450,500]
width_i = random.choice(width_options)

temp = random.random()
temp = temp*0.4-0.15
temp = max(0,temp)
move_up_ratio = random.random()*temp
temp = 'invalid'

#control the core data here for debug purpose.



actual_fill_mode = get_actual_fill_mode(fill_mode)

#calculate other data
height_i = math.floor(width_i*answer)
top = (im.size[1]-height_i)/2 *(1-move_up_ratio)
bottom = top+height_i
left = (im.size[0]-width_i)/2
right = left+width_i
if 'normal filling' == actual_fill_mode:
    bottom-=1
    right-=1
#for train mode
ref_line_bottom_i = []
if train_mode:
    for i in ref_line_ratios:
        temp = top + math.floor(width_i*i)
        if 'normal filling' == actual_fill_mode:
            temp-=1
        ref_line_bottom_i.append([i,temp])


#tails
tail_space_length = im.size[1]-bottom
avg_tail_length = min(tail_space_length*0.5,height_i)
tail_length_difference = (200+height_i*0.1)/2
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
    #background_color,
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
if 'normal filling'==actual_fill_mode:
    draw.rectangle(xy=((left+1,top+1),(right-1,bottom-1)),fill=fill_color)
    #注意，矩形的绘制是包含写入的数值的
    #例如，((1,1),(1,1))的情况下是1个点，((1,1),(2,2))是4个点

# if 0==random_index:#
#     random_index = random.choice([1,5,6])
#     fill_color =color_options[random_index]

#outlines
if 'frame mode'==actual_fill_mode:
    fill_color = color_options[random.choice([0,1,4,5])]
    #top
draw.line((left,top)+(right,top), fill=fill_color)
    #bottom
draw.line((left,bottom)+(right,bottom), fill=fill_color)
    #left
draw.line((left,top)+(left,bottom+left_tail_length), fill=fill_color)
    #right
draw.line((right,top)+(right,bottom+right_tail_length), fill=fill_color)

#auxiliary_line
actual_auxiliary_line_mode = get_actual_auxiliary_line_mode(auxiliary_line_mode)
fill_color_auxiliary_line = fill_color
if 'normal filling'==actual_fill_mode:
    fill_color_auxiliary_line = fill_color-grey_index()*20
if 'frame mode'==actual_fill_mode:
    fill_color_auxiliary_line = background_color-grey_index()*20

# x mark
if 'x mark' == actual_auxiliary_line_mode:
    draw.line((left,top)+(right,bottom), fill=fill_color_auxiliary_line)
    draw.line((left,bottom)+(right,top), fill=fill_color_auxiliary_line)
# circle
if 'circle' == actual_auxiliary_line_mode:
    circle_top = top
    circle_bottom = bottom
    circle_left = im.size[0]/2-height_i/2
    circle_right = circle_left+(bottom-top)
    
    draw.ellipse(((circle_left, circle_top), (circle_right, circle_bottom)), outline=fill_color_auxiliary_line)
    #Notice: the accuraty of this circle drawing function is not very good. 

color_options = 'invalid'




#scattering noise rects
if noise_mode in [1,2,3,4]:
    count = random.randint(round(width_i*0.03),round(width_i*0.07))
    x_coord = random.random()*10000
    y_coord = random.random()*10000
    x_coord_random_stride = random.random()*3457891
    y_coord_random_stride = random.random()*5676782
    x_mod = width_i/1.5
    y_mod = height_i/1.5
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
        
        noice_color = color_options[i% len(color_options)]
        #draw call
        draw.rectangle(xy=(top_left,bottom_right),fill=noice_color)#background_color+grey_index()*(i%7-5)*4)
        
        #late update
        x_coord=x_mid+x_coord_random_stride
        y_coord=y_mid+y_coord_random_stride
        x_mod+=3
        y_mod+=3*(9./16.)
    ################



#del draw

#save image
im.save('puzzle.png')

#defer for few seconds and then print the answer in console. So that let user to train with only console window and image browser window.
time.sleep(delay_duration)
display_answer = round((height_i)/(width_i),ndigits=3)
if 'normal filling'==actual_fill_mode:
    display_answer = round((height_i+1)/(width_i+1),ndigits=3)


print('answer: '+str(display_answer) +'   width: '+str(width_i)+'   height: '+str(round(height_i)))
if(0):
    print('--------------------')
    print('debug info:')
    print('--------------------')
    print("width"+str(width_i))
    print("height"+str(height_i))
    print("move_up_ratio"+str(move_up_ratio))
    
    print("top"+str(top))
    print("bottom"+str(bottom))
    print("left"+str(left))
    print("right"+str(right))
    print("left_tail_length"+str(left_tail_length))
    print("right_tail_length"+str(right_tail_length))
    print('--------------------')


import os
if not train_mode :
    exit()



os.system("pause")

#draw the reference lines
ref_line_color = fill_color


for i in ref_line_bottom_i:
    temp = (i[0]*5)%1
    if temp<0.01 or temp>0.99:
        draw.line((left-25,i[1])+(left+25,i[1]), fill=ref_line_color)
        draw.line((right-25,i[1])+(right+25,i[1]), fill=ref_line_color)
    else:
        draw.line((left-10,i[1])+(left+10,i[1]), fill=ref_line_color)
        draw.line((right-10,i[1])+(right+10,i[1]), fill=ref_line_color)

im.save('puzzle.png')



del draw

