#-*coding:utf-8*-
'''
useage:
    Statistics.py 
    Statistics.py 20201027
output:
    Statistics/xxxxx.png

'''

import time
import subprocess
import os
import sys


logfolderdir = 'data/logs/'
setulogfolderdir = 'data/plugins/Setu/log/'
setufolderdir = '../../miraiOK_Release/data/MiraiApiHttp/images/Setu/'
drawdatapath = 'data/plugins/Statistics/webpage/static/js/drawdata.js'
htmlpath = 'data/plugins/Statistics/webpage/index.html'
imgfolderdir = '../../miraiOK_Release/data/MiraiApiHttp/images/Statistics/'
imgwidth = 1200
imgheight = 1920

localtime = time.localtime(time.time())

def QQavatarurl(qq):
    return "http://q1.qlogo.cn/g?b=qq&nk=" + qq + "&s=640"

def calculate(filename, logfile, setulogfile, outpath):
    
    staticDate = filename[:-4].replace('-', '/')

    #统计时间
    dateString = staticDate + ' 的统计数据'

    #今日消息增长
    todayMsgInc_data = {
        'recv':[],
        'send':[]
    }
    recv_count = 0
    send_count = 0
    for eachline in logfile.readlines():
        contents = eachline.split('\t')
        #todayMsgInc_data['time'].append(contents[0])
        if contents[1]=='recv':
            recv_count += 1
        elif contents[1]=='send':
            send_count += 1
        todayMsgInc_data['recv'].append([staticDate + ' ' + contents[0], recv_count])
        todayMsgInc_data['send'].append([staticDate + ' ' + contents[0], send_count])

    #今日收发消息总数
    todayMsgStr = "今日收到消息" + str(recv_count) + "次\t" + "发送消息" + str(send_count) + "次"

    #今日色图增长
    todaySetuInc_data = {
        'recv':[],
        'send':[]
    }
    setu_recv_count = 0
    setu_send_count = 0
    for eachline in setulogfile.readlines():
        contents = eachline.split('\t')
        #todaySetuInc_data['time'].append(contents[0])
        if contents[1]=='recv':
            setu_recv_count += 1
        elif contents[1]=='send':
            setu_send_count += 1
        todaySetuInc_data['recv'].append([staticDate + ' ' + contents[0], setu_recv_count])
        todaySetuInc_data['send'].append([staticDate + ' ' + contents[0], setu_send_count])

    #历史色图增长
    hisSetuInc_data = {
        'recv':[]
    }
    setufilelist = os.listdir(setufolderdir)
    setutimelist = [os.path.getctime(setufolderdir + f) for f in setufilelist]
    setutimelist.sort()
    datelist = [time.strftime("%Y/%m/%d %H:%M:%S", time.localtime(t)) for t in setutimelist]
    for i in range(len(datelist)):
        hisSetuInc_data['recv'].append([datelist[i], i])

    #今日色图贡献榜
    #今日色图需求榜
    todaySetuGiveRank_data = {
        'image':[],
        'count':[]
    }
    todaySetuGetRank_data = {
        'image':[],
        'count':[]
    }
    give_count = {}
    get_count = {}
    setulogfile.seek(0)
    for eachline in setulogfile.readlines():
        contents = eachline.split('\t')
        if contents[1]=='recv':
            try:
                give_count[contents[2]] += 1
            except:
                give_count[contents[2]] = 1
        elif contents[1]=='send':
            try:
                get_count[contents[2]] += 1
            except:
                get_count[contents[2]] = 1
    give_rank = sorted(give_count.items(), key = lambda kv:(kv[1], kv[0]), reverse=True)
    get_rank = sorted(get_count.items(), key = lambda kv:(kv[1], kv[0]), reverse=True)
    while len(give_rank) < 5:
        give_rank.append(('0', 0))
    while len(get_rank) < 5:
        get_rank.append(('0', 0))
    for i in range(5):
        todaySetuGiveRank_data['image'].append(QQavatarurl(give_rank[i][0]))
        todaySetuGiveRank_data['count'].append(give_rank[i][1])
        todaySetuGetRank_data['image'].append(QQavatarurl(get_rank[i][0]))
        todaySetuGetRank_data['count'].append(get_rank[i][1])
        
    #写入drawdata.js文件
    with open(outpath, 'w', encoding='utf-8') as f:
        f.write('dateString = \"' + str(dateString) + '\";\n\n')
        f.write('todayMsgStr = \"' + str(todayMsgStr) + '\";\n\n')
        f.write('todayMsgInc_data = ' + str(todayMsgInc_data) + ';\n\n')
        f.write('todaySetuInc_data = ' + str(todaySetuInc_data) + ';\n\n')
        f.write('hisSetuInc_data = ' + str(hisSetuInc_data) + ';\n\n')
        f.write('todaySetuGiveRank_data = ' + str(todaySetuGiveRank_data) + ';\n\n')
        f.write('todaySetuGetRank_data = ' + str(todaySetuGetRank_data) + ';\n\n')

def render(imgpath):
    cmd = 'wkhtmltoimage'
    cmd += ' --crop-w '+str(imgwidth)
    cmd += ' --crop-h '+str(imgheight)
    cmd += ' --width '+str(imgwidth)
    cmd += ' --height '+str(imgheight)
    cmd += ' --javascript-delay '+str(2000)
    cmd += ' --enable-local-file-access'
    cmd += ' --quality 40'
    cmd += ' '+htmlpath+' '+imgpath
    p = subprocess.Popen(cmd, stdout = subprocess.PIPE, shell = True)
    p.wait()




filename = ''


try:
    date = sys.argv[1]
    filename = date[:4] + '-' + date[4:6] + '-' + date[6:8] + '.log'
    logfile = open(logfolderdir + filename, 'r', encoding='utf-8')
    setulogfile = open(setulogfolderdir + filename, 'r', encoding='utf-8')
    
except:
    filename = str(localtime[0]) + '-' + str(localtime[1]) + '-' + str(localtime[2]) + '.log'
    logfile = open(logfolderdir + filename, 'r', encoding='utf-8')
    setulogfile = open(setulogfolderdir + filename, 'r', encoding='utf-8')

finally:
    #统计数据
    calculate(filename, logfile, setulogfile, drawdatapath)

    #渲染图片
    imgname = str(localtime[0]) + str(localtime[1]) + str(localtime[2]) + str(localtime[3]) + str(localtime[4]) + str(localtime[5])
    render(imgfolderdir + imgname + '.png')

    #newimage = pyvips.Image.new_from_file(imgfolderdir + imgname + '.png')
    #newimage.write_to_file(imgfolderdir + imgname+ '.jpg')

    print('Statistics/' + imgname+ '.png')

    logfile.close()
    setulogfile.close()
