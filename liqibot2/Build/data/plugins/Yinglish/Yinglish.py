# -*- encoding: utf-8 -*-
'''
useage:
    Yinglish.py "真的可以吗"
output:
    真......真的可以吗

'''

import yinglish
import sys

s = sys.argv[1]

print(yinglish.chs2yin(s), end='')