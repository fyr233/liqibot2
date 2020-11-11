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

from Config import config

def cpuUsage(container):
    stats = container.stats(stream=False)
    cpuDelta = stats["cpu_stats"]["cpu_usage"]["total_usage"] - stats["precpu_stats"]["cpu_usage"]["total_usage"]
    sysDelta = stats["cpu_stats"]["system_cpu_usage"] - stats["precpu_stats"]["system_cpu_usage"]
    return cpuDelta / sysDelta * stats["cpu_stats"]["online_cpus"]

def memUsage(container):
    stats = container.stats(stream=False)
    return stats["memory_stats"]["usage"] / stats["memory_stats"]["limit"]

def getContainer():
    client = docker.from_env()
    containers = []

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

def runC(container, file, params):
    run_cmd = "tcc -run " + "codes/" + file + " " + params
    result = container.exec_run(run_cmd)
    print(result[1].decode('utf-8'), end='')

def runPython(container, file, params):
    run_cmd = "python3 " + "codes/" + file + " " + params
    result = container.exec_run(run_cmd)
    print(result[1].decode('utf-8'), end='')

def runCpp(container, file, params):
    exec_file = ''.join(file.split('.')[:-1])
    compile_cmd = "g++ " + "codes/" + file + " -o " + exec_file
    run_cmd = "./" + exec_file + " " + params
    container.exec_run(compile_cmd)
    result = container.exec_run(run_cmd)
    print(result[1].decode('utf-8'), end='')

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