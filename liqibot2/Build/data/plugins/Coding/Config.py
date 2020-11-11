import os

config={
    'containers_size': 2,
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
    'cpu_usage_max': 0.6,
    'mem_usage_max': 0.6
}