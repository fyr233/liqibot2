#import parl

def check(message):
    if message['sender']['id'] == 1325275429:
        print('to repeat')
        return True
    else:
        return False

#@parl.remote_class
class MainClass:

    def __init__(self):
        pass

    def run(self, route_table_dynamic, queue, message, api_send):
        print('in repeat')
        switch = {
            'GroupMessage': lambda :api_send.sendGroupMessage(message['sender']['group']['id'], 0, message['messageChain']),
            'FriendMessage': lambda :api_send.sendFriendMessage(message['sender']['id'], 0, message['messageChain'])
        }
        
        switch[message['type']]()

        print('repeat', [(i['text'] if 'text' in i else '') for i in message['messageChain']])

    def run2(self, message):
        print('in repeat')
