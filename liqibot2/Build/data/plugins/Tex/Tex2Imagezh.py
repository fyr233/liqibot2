'''
useage:
    Tex2Image.py "y=\int f(x) \mathrm d x"
output:
    Tex/Mon28Sep2020132441GMT.png

'''


import sys
import requests
from urllib.parse import quote
import pyvips



#知乎tex接口

url = 'https://www.zhihu.com/equation'

headers = {
    'accept': 'image/avif,image/webp,image/apng,image/*,*/*;q=0.8',
    'referer': 'https://www.zhihu.com/question/19611261/answer/86892924',
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36',
    }

data = {
    'tex': sys.argv[1]
    }

r = requests.get(url, headers=headers, params=data)


#svg转png

imgfolderdir = '../../mcl-1.0.5/data/net.mamoe.mirai-api-http/images/Tex/'
filename = r.headers['Date'].replace(' ', '').replace(',', '').replace(':', '') + '.png'

image = pyvips.Image.new_from_buffer(r.content, '', dpi=2000)
background = image.new_from_image([255, 255, 255])
background += 255
background.composite(image, 'over').write_to_file(imgfolderdir + filename)

print('Tex/' + filename)
