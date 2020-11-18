#from multiprocessing import Pipe, Manager, Queue
import threading
import queue
from concurrent.futures import ThreadPoolExecutor
#import parl
import sys

import plugins

sys.path.append("./plugins")

class MsgRouter:
    def __init__(self, pool, manager, api_send):

        #self.pool = pool
        self.api_send = api_send
        #self.manager = Manager()
        #elf.executor = ThreadPoolExecutor(max_workers=2)
        #parl.connect("localhost:8010")

        self.route_table_static = [
            #plugins.ping,
            plugins.repeat,
            plugins.continuousChat
        ]
        self.route_table_dynamic = []

    def onReceived(self, response):
        print(response)
        print(self.route_table_dynamic)

        if len(self.route_table_dynamic) > 0:
            for e in self.route_table_dynamic:
                if e[0](response):
                    e[1].put(response)

        for e in self.route_table_static:
            if e.check(response):
                t = threading.Thread(target=self.run_remote, args=(e, response))
                t.start()
                #e_instance = e.main_class()
                #q = queue.Queue()
                #e_instance.run(self.route_table_dynamic, q, response, self.api_send)
                #self.pool.apply(e_instance.run, args=(self.route_table_dynamic, q, response, self.api_send))
                #self.executor.submit(e_instance.run, (self.route_table_dynamic, q, response, self.api_send))
                #t = threading.Thread(target=e_instance.run, args=(self.route_table_dynamic, q, response, self.api_send))
                #t.start()

    def run_remote(self, e, response):
        e_instance = e.MainClass()
        q = queue.Queue()
        e_instance.run(self.route_table_dynamic, q, response, self.api_send)

    def close(self):
        pass
