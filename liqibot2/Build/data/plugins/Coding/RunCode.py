'''
usage:
    RunCode.py python 12345678.py
    RunCode.py python 12345678.py params params
    RunCode.py c 12345678.c
    RunCode.py c++ 12345678.cpp
output:
    stdout & stderr

'''

import docker
import random
import sys
import threading

from Config import config

client = docker.from_env(timeout=config['client_timeout'])
containers = []

def time_out(second, callback=None):
    def decorator(func):
        def wrapper(*args, **kwargs):
            t =threading.Thread(target=func, args=args, kwargs=kwargs)
            # 设置主线程技术子线程立刻结束
            t.setDaemon(True)  
            t.start()
            # 主线程阻塞等待interval秒，又名：挂起 
            t.join(second)
            if t.is_alive() and callback:
            	# 立即执行回调函数
                return threading.Timer(0, callback).start()  
            else:
                return
        return wrapper
    return decorator

def timeout_callback():
    print('运行超时',end='')

def cpuUsage(container):
    stats = container.stats(stream=False)
    cpuDelta = stats["cpu_stats"]["cpu_usage"]["total_usage"] - stats["precpu_stats"]["cpu_usage"]["total_usage"]
    sysDelta = stats["cpu_stats"]["system_cpu_usage"] - stats["precpu_stats"]["system_cpu_usage"]
    return cpuDelta / sysDelta * stats["cpu_stats"]["online_cpus"]

def memUsage(container):
    stats = container.stats(stream=False)
    return stats["memory_stats"]["usage"] / stats["memory_stats"]["limit"]

def getContainer():

    for item in client.containers.list(all=True):
        if len(item.name)>10 and item.name[:11]=='liqi-Coding':
            containers.append(item)
    
    while len(containers) < config['containers_size']:
        new_container = client.containers.create('ubuntu:liqi-Coding', 
            name='liqi-Coding' + str(random.random()), 
            cpu_count=config['cpu_count'],
            mem_limit=config['mem_limit'],
            volumes=config['volumes'],
            working_dir=config['working_dir'],
            pids_limit=config['pids_limit'],
            detach=True,
            tty=True)
        containers.append(new_container)

    for item in containers:
        if client.containers.get(item.short_id).status != 'running':
            item.restart()
            return item

    for item in containers:
        if cpuUsage(item)<config['cpu_usage_max'] and memUsage(item)<config['mem_usage_max']:
            return item

    rand_item = random.choice(containers)
    rand_item.restart()
    return rand_item

@time_out(config['client_timeout'], timeout_callback)
def runC(container, file, params):
    run_cmd = "tcc -run " + "codes/" + file + " " + params
    result = client.containers.get(container.short_id).exec_run(run_cmd)
    print(result[1].decode('utf-8'), end='')

@time_out(config['client_timeout'], timeout_callback)
def runPython(container, file, params):
    run_cmd = "python3 " + "codes/" + file + " " + params
    result = client.containers.get(container.short_id).exec_run(run_cmd)
    print(result[1].decode('utf-8'), end='')

@time_out(config['client_timeout'], timeout_callback)
def runCpp(container, file, params):
    exec_file = ''.join(file.split('.')[:-1])
    compile_cmd = "g++ " + "codes/" + file + " -o " + exec_file
    run_cmd = "./" + exec_file + " " + params
    result1 = client.containers.get(container.short_id).exec_run(compile_cmd)
    print(result1[1].decode('utf-8'), end='')
    result2 = client.containers.get(container.short_id).exec_run(run_cmd)
    print(result2[1].decode('utf-8'), end='')


codetype = sys.argv[1]
codefile = sys.argv[2]
params = ' '.join(sys.argv[3:])
container = getContainer()

if codetype in ['c', 'C']:
    runC(container, codefile, params)
elif codetype in ['cpp', 'Cpp', 'CPP', 'c++', 'C++']:
    runCpp(container, codefile, params)
elif codetype in ['python', 'Python', 'py', 'Py', '屁眼']:
    runPython(container, codefile, params)
else:
    print('不支持的语言')