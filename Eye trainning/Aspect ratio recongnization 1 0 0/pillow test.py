from PIL import Image, ImageDraw
import random
import math
import time

im = Image.new('RGB', (100,100), color = 0x999999)
draw = ImageDraw.Draw(im)
draw.line((1,1)+(1,1), fill=0)
draw.line((3,3)+(3,4), fill=0)
draw.line((6,6)+(8,8), fill=0)


del draw
im.save('puzzle.png')


