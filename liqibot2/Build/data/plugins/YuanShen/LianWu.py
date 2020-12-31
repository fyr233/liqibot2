#-*- coding:utf-8 -*-
'''
usage:
    LianWu.py 今天打什么
    LianWu.py 周一打什么
output:
    

'''

data = {
    '周一':{
        '塞西莉亚苗圃':{
            '材料':[
                '高塔孤王的破瓦',
                '高塔孤王的残垣',
                '高塔孤王的断片',
                '高塔孤王的碎梦'
            ],
            '武器':[
                '西风剑',
                '宗室长剑',
                '风鹰剑',
                '铁影阔剑',
                '钟剑',
                '西风秘典',
                '宗室秘法录',
                '绝弦',
                '苍翠猎弓',
            ]
        },
        '震雷连山密宫':{
            '材料':[
                '孤云寒林的光砂',
                '孤云寒林的辉岩',
                '孤云寒林的圣骸',
                '孤云寒林的神体'
            ],
            '武器':[
                '匣里龙吟',
                '黑岩长剑',
                '白影剑',
                '匣里日月',
                '黑岩绯玉',
                '弓藏',
                '黑岩战弓',
                '流月针',
                '和璞鸢',
            ]
        },
    },
    '周二':{
        '塞西莉亚苗圃':{
            '材料':[
                '凛风奔狼的始龀',
                '凛风奔狼的裂齿',
                '凛风奔狼的断牙',
                '凛风奔狼的怀乡'
            ],
            '武器':[
                '笛剑',
                '黑剑',
                '天空之刃',
                '祭礼大剑',
                '天空之傲',
                '流浪乐章',
                '天空之卷',
                '祭礼弓',
                '历练的猎弓',
                '神射手之誓',
                '天空之翼',
                '决斗之枪',
            ]
        },
        '震雷连山密宫':{
            '材料':[
                '孤云寒林的光砂',
                '孤云寒林的辉岩',
                '孤云寒林的圣骸',
                '孤云寒林的神体'
            ],
            '武器':[
                '吃虎鱼刀',
                '试作斩岩',
                '雨裁',
                '黑岩斩刀',
                '试作金铂',
                '昭心',
                '试作澹月',
                '匣里灭辰',
                '黑岩刺枪',
            ]
        },
    },
    '周三':{
        '塞西莉亚苗圃':{
            '材料':[
                '狮牙斗士的枷锁',
                '狮牙斗士的铁链',
                '狮牙斗士的镣铐',
                '狮牙斗士的理想'
            ],
            '武器':[
                '祭礼剑',
                '西风大剑',
                '宗室大剑',
                '狼的末路',
                '祭礼残章',
                '四风原典',
                '西风猎弓',
                '宗室长弓',
                '阿莫斯之弓',
            ]
        },
        '震雷连山密宫':{
            '材料':[
                '漆黑陨铁的一粒',
                '漆黑陨铁的一片',
                '漆黑陨铁的一角',
                '漆黑陨铁的一块'
            ],
            '武器':[
                '铁蜂刺',
                '试作古华',
                '螭骨剑',
                '万国诸海图谱',
                '尘世之锁',
                '钢轮弓',
                '试作星镰',
            ]
        },
    },
    '周四':{
        '塞西莉亚苗圃':{
            '材料':[
                '高塔孤王的破瓦',
                '高塔孤王的残垣',
                '高塔孤王的断片',
                '高塔孤王的碎梦'
            ],
            '武器':[
                '西风剑',
                '宗室长剑',
                '风鹰剑',
                '铁影阔剑',
                '钟剑',
                '西风秘典',
                '宗室秘法录',
                '绝弦',
                '苍翠猎弓',
            ]
        },
        '震雷连山密宫':{
            '材料':[
                '孤云寒林的光砂',
                '孤云寒林的辉岩',
                '孤云寒林的圣骸',
                '孤云寒林的神体'
            ],
            '武器':[
                '匣里龙吟',
                '黑岩长剑',
                '白影剑',
                '匣里日月',
                '黑岩绯玉',
                '弓藏',
                '黑岩战弓',
                '流月针',
                '和璞鸢',
            ]
        },
    },
    '周五':{
        '塞西莉亚苗圃':{
            '材料':[
                '凛风奔狼的始龀',
                '凛风奔狼的裂齿',
                '凛风奔狼的断牙',
                '凛风奔狼的怀乡'
            ],
            '武器':[
                '笛剑',
                '黑剑',
                '天空之刃',
                '祭礼大剑',
                '天空之傲',
                '流浪乐章',
                '天空之卷',
                '祭礼弓',
                '历练的猎弓',
                '神射手之誓',
                '天空之翼',
                '决斗之枪',
            ]
        },
        '震雷连山密宫':{
            '材料':[
                '孤云寒林的光砂',
                '孤云寒林的辉岩',
                '孤云寒林的圣骸',
                '孤云寒林的神体'
            ],
            '武器':[
                '吃虎鱼刀',
                '试作斩岩',
                '雨裁',
                '黑岩斩刀',
                '试作金铂',
                '昭心',
                '试作澹月',
                '匣里灭辰',
                '黑岩刺枪',
            ]
        },
    },
    '周六':{
        '塞西莉亚苗圃':{
            '材料':[
                '狮牙斗士的枷锁',
                '狮牙斗士的铁链',
                '狮牙斗士的镣铐',
                '狮牙斗士的理想'
            ],
            '武器':[
                '祭礼剑',
                '西风大剑',
                '宗室大剑',
                '狼的末路',
                '祭礼残章',
                '四风原典',
                '西风猎弓',
                '宗室长弓',
                '阿莫斯之弓',
            ]
        },
        '震雷连山密宫':{
            '材料':[
                '漆黑陨铁的一粒',
                '漆黑陨铁的一片',
                '漆黑陨铁的一角',
                '漆黑陨铁的一块'
            ],
            '武器':[
                '铁蜂刺',
                '试作古华',
                '螭骨剑',
                '万国诸海图谱',
                '尘世之锁',
                '钢轮弓',
                '试作星镰',
            ]
        },
    },
}

import time
import datetime
import json
import sys
import re

def print_data(key):
    print(key)
    for mijing in data[key].keys():
        print(mijing)
        print(' ' + '材料')
        for cailiao in data[key][mijing]['材料']:
            print('  ' + cailiao)
        print('')
        print(' ' + '武器')
        for wuqi in data[key][mijing]['武器']:
            print('  ' + wuqi)
        print('')

weekday = ''
try:
    weekday = re.findall('(.*?)打什么', sys.argv[1])[0]
except:
    pass

if not weekday in data.keys():
    if weekday=='今天':
        w = datetime.datetime.now().weekday()
    elif weekday=='明天':
        w = (datetime.datetime.now().weekday() + 1) % 7
    elif weekday=='后天':
        w = (datetime.datetime.now().weekday() + 2) % 7
    elif weekday=='昨天':
        w = (datetime.datetime.now().weekday() - 1) % 7
    else:
        exit()
    
    if w==0:
        weekday = '周一'
    if w==1:
        weekday = '周二'
    if w==2:
        weekday = '周三'
    if w==3:
        weekday = '周四'
    if w==4:
        weekday = '周五'
    if w==5:
        weekday = '周六'
    if w==6:
        weekday = '周日'

print_data(weekday)