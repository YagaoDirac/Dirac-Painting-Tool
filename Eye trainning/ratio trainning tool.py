from PIL import Image, ImageDraw
import random
import math
import time

mode = 1 #not implemented yet
delay_duration = 5# in seconds.
background_color = 0x7f7f7f

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


###################


im = Image.new('RGB', (1600, 900), color = background_color)

#core data

# answer_options = [0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1]
# answer = random.choice(answer_options)
answer = random.randint(20,100)/100

width_options = [200,300,400,500,600,700,800]
width = random.choice(width_options)

temp = random.random()
temp = temp*0.4-0.15
temp = max(0,temp)
move_up_ratio = random.random()*temp

#control the core data here for debug purpose.



#calculate other data
height = width*answer
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
    #safety check
left_tail_length = max(0,min(tail_space_length,left_tail_length))
right_tail_length = max(0,min(tail_space_length,right_tail_length))

#color
color = random_dark_gray()#or fill=0xffffff for white rectangle

#draw~
draw = ImageDraw.Draw(im)
    #top
draw.line((left,top)+(right,top), fill=color)
    #bottom
draw.line((left,bottom)+(right,bottom), fill=color)
    #left
draw.line((left,top)+(left,bottom+left_tail_length), fill=color)
    #right
draw.line((right,top)+(right,bottom+right_tail_length), fill=color)

del draw

#save image
im.save('puzzle.png')

#save answer in text file
with open("answer.txt",'w') as f:
    f.write('xxxxxxx')

#defer for few seconds and then print the answer in console. So that let user to train with only console window and image browser window.
time.sleep(delay_duration)
print('answer: '+str(answer) +'   width: '+str(width)+'   height: '+str(height))
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

