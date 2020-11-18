import subprocess
#import parl

def check(message):
    if message['sender']['id'] == 1325275429:
        if message['messageChain'][1]['type'] == 'Plain' and message['messageChain'][1]['text'][:4] == 'ping':
            print('to ping')
            return True
    return False

#@parl.remote_class
class MainClass:

    def __init__(self):
        pass

    def run(self, route_table_dynamic, queue, message, api_send):
        command_str = ''.join([(i['text'] if i['type']=='Plain' else '') for i in message['messageChain']]).split('\n')[0] + '\n'

        p1 = subprocess.Popen(command_str, shell=True, stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        result = p1.communicate()[0].decode('ANSI')
        ans = [
            {
                'type': 'Plain',
                'text': result,
            },
        ]

        switch = {
            'GroupMessage': lambda :api_send.sendGroupMessage(message['sender']['group']['id'], 0, ans),
            'FriendMessage': lambda :api_send.sendFriendMessage(message['sender']['id'], 0, ans)
        }
        
        switch[message['type']]()

