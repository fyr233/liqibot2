#-*coding:utf-8*-
'''
useage:
    CheckSetu.py "http://xxx.com/01" {67979A05-A41E-EB0B-0A5F-8F2314B4D43B}.mirai
output:
    色图！

'''

import sys
import requests
import random

imgurl = sys.argv[1]
imgname = sys.argv[2]#后缀

setufolderdir = '../../mcl-1.0.5/data/net.mamoe.mirai-api-http/images/Setu/'
notsetufolderdir = '../../mcl-1.0.5/data/net.mamoe.mirai-api-http/images/NotSetu/'

def saveImg(url, folderdir):
    r = requests.get(url)
    with open(folderdir + imgname + '.' + r.headers['Content-Type'][6:], 'wb') as f:
        f.write(r.content)

def downloadImg(url):
    r = requests.get(url)
    return r.content, r.headers['Content-Type'][6:]

def Check_Baidu():
    imgContent, imgType = downloadImg(imgurl)
    if len(imgContent) < 5e3 or len(imgContent) > 4e6:
        return
    if imgType not in ['jpg', 'jpeg', 'png']:
        return

    from aip import AipImageCensor

    censor_APP_ID = '15708523'
    censor_API_KEY = '6USQY453ZVSjxYYej1F195IZ'
    censor_SECRET_KEY = 'r0rtrpRj8eHRnkPCfEhjOPRh2eO997Uv'

    #另一个百度账号
    if random.random() < 0:
        censor_APP_ID = '22908418'
        censor_API_KEY = 'Fz2zsXkSFmdH4BgcSwvGLeNP'
        censor_SECRET_KEY = 'VSbOwvqUNzG1cDW89O9iTnnweLvLyGGM'
    
    #classify_APP_ID = '17981247'
    #classify_API_KEY = '3HuleW8fwIPymQcRM1DNhigp'
    #classify_SECRET_KEY = 'LcClAOmKwGSIXR2st8ishMXUPXkiLaaI'
    
    censor_client = AipImageCensor(censor_APP_ID, censor_API_KEY, censor_SECRET_KEY)
    censor_result = censor_client.antiPorn(imgContent)

    # print(censor_result)
    isSetu = False
    if 'result_fine' in censor_result:
        for each in censor_result['result_fine']:
            #print('type', each['type'], 'prob', each['probability'])
            if each['class_name']=='一般色情' and each['probability']>0.9:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='卡通色情' and each['probability']>0.51:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='SM' and each['probability']>0.65:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='艺术品色情' and each['probability']>0.9:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='儿童裸露' and each['probability']>0.9:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='低俗' and each['probability']>0.95:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='性玩具' and each['probability']>0.7:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                break
            elif each['class_name']=='女性性感' and each['probability']>0.85:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='卡通女性性感' and each['probability']>0.51:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='男性性感' and each['probability']>0.95:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='自然男性裸露' and each['probability']>0.95:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='亲密行为' and each['probability']>0.90:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='卡通亲密行为' and each['probability']>0.65:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='特殊类' and each['probability']>0.95:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='一般正常' and each['probability']>1.0:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='卡通正常' and each['probability']>1.0:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='臀部特写' and each['probability']>0.85:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='裆部特写' and each['probability']>0.75:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='脚部特写' and each['probability']>0.85:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
            elif each['class_name']=='孕肚裸露' and each['probability']>0.99:
                print('色图！', end='')
                saveImg(imgurl, setufolderdir)
                isSetu = True
                break
    if not isSetu:
        pass#saveImg(imgurl, notsetufolderdir)

def Check_Tencent():
    pass

def main():
    Check_Baidu()

if __name__ == '__main__':
    main()

