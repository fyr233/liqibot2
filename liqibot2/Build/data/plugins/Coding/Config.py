import os

config={
    'containers_size': 1,
    'cpu_count': 2,
    'mem_limit': '4g',
    'volumes': {
        os.getcwd() + '/data/plugins/Coding/docker/codes': {
            'bind': '/home/root/codes',
            'mode': 'ro'
        },
        os.getcwd() + '/../../miraiOK_Release/data/MiraiApiHttp/images/Coding': {
            'bind': '/mnt/images',
            'mode': 'rw'
        }
    },
    'working_dir': '/home/root',
    'pids_limit': 50,
    'cpu_usage_max': 1.0,
    'mem_usage_max': 1.0,
    'client_timeout': 60,
}