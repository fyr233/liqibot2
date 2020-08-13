import multiprocessing
from multiprocessing import Pipe
import parl

def check(message):
    if message['messageChain'][1]['text'] == 'begin':
        print('to ContinuousChat')
        return True
    else:
        return False

#@parl.remote_class
class MainClass:

    def __init__(self):
        pass

    def run(self, route_table_dynamic, queue, message, api_send):
        switch = {
            'GroupMessage': lambda x:api_send.sendGroupMessage(message['sender']['group']['id'], 0, x),
            'FriendMessage': lambda x:api_send.sendFriendMessage(message['sender']['id'], 0, x)
        }
        
        switch[message['type']](api_send.msgChain('waiting for end'))

        next_check = lambda m: True if m['messageChain'][1]['text'] == 'end' else False
        print(next_check)
        #q = multiprocessing.Queue()
        #conn1, conn2 = Pipe(True)
        #print(conn1, conn2)

        route_table_dynamic.append((next_check, queue))
        print(route_table_dynamic)
        
        try:
            #new_message = queue.get()
            switch[message['type']](api_send.msgChain('end!'))
        except:
            pass

        #route_table_dynamic.remove((next_check, q))


