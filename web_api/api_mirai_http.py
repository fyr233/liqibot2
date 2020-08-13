import requests
import websocket
import json

port = 8080
host = 'localhost'
url = 'http://localhost:' + str(port)
ws_url = 'ws://localhost:' + str(port)
authKey = '123456789'
qq = 2155679839

class Send:

    def __init__(self):
        self.port = port
        self.url = url
        self.authKey = authKey
        self.sessionKey = ''
        self.qq = qq

        #认证，返回一个sessionKey
        self.auth()
        if self.sessionKey != '':
            self.verify()

    def about(self):
        r = requests.get(self.url + '/about')
        return json.loads(r.text)

    def auth(self):
        d = {'authKey': self.authKey}
        r = requests.post(self.url + '/auth', data = json.dumps(d))
        print(str(r.request))
        response = json.loads(r.text)
        if response['code'] == 0:
            self.sessionKey = response['session']
        else:
            print('web_api/api_mirai_http.py::Send::auth code:', response['code'])

    def verify(self):
        d = {
            'sessionKey': self.sessionKey,
            'qq': self.qq
            }
        r = requests.post(self.url + '/verify', data = json.dumps(d))
        response = json.loads(r.text)
        if response['code'] == 0:
            print('verify success')
        else:
            print('web_api/api_mirai_http.py::Send::verify code:', response['code'])

    def release(self):
        d = {
            'sessionKey': self.sessionKey,
            'qq': self.qq
            }
        r = requests.post(self.url + '/release', data = json.dumps(d))
        response = json.loads(r.text)
        if response['code'] == 0:
            pass
        else:
            print('web_api/api_mirai_http.py::Send::release code:', response['code'])

    #返回messageId
    def sendFriendMessage(self, qq, quote, messageChain):
        d = {
            'sessionKey': self.sessionKey,
            'qq': qq,
            'messageChain': messageChain
            }
        if quote != 0:
            d['quote'] = quote
        
        r = requests.post(self.url + '/sendFriendMessage', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            return response['messageId']
        else:
            print('web_api/api_mirai_http.py::Send::sendFriendMessage code:', response['code'])
            return 0

    #返回messageId
    def sendTempMessage(self, group, qq, quote, messageChain):
        d = {
            'sessionKey': self.sessionKey,
            'qq': qq,
            'group': group,
            'messageChain': messageChain
            }
        if quote != 0:
            d['quote'] = quote
        
        r = requests.post(self.url + '/sendTempMessage', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            return response['messageId']
        else:
            print('web_api/api_mirai_http.py::Send::sendTempMessage code:', response['code'])
            return 0

    #返回messageId
    def sendGroupMessage(self, group, quote, messageChain):
        d = {
            'sessionKey': self.sessionKey,
            'group': group,
            'messageChain': messageChain
            }
        if quote != 0:
            d['quote'] = quote
        #print(d, 'send')
        
        r = requests.post(self.url + '/sendGroupMessage', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            return response['messageId']
        else:
            print('web_api/api_mirai_http.py::Send::sendGroupMessage code:', response['code'])
            return 0

    #TODO
    def sendImageMessage(self):
        pass

    #TODO
    def uploadImage(self):
        pass

    #撤回
    def recall(self, messageId):
        d = {
            'sessionKey': self.sessionKey,
            'target': messageId
            }
        
        r = requests.post(self.url + '/recall', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            pass
        else:
            print('web_api/api_mirai_http.py::Send::recall code:', response['code'])
    
    #获取好友列表，返回json对象
    def getFriendList(self):
        d = {
            'sessionKey': self.sessionKey
            }
        
        r = requests.get(self.url + '/friendList', params = d)
        response = json.loads(r.text)

        return response

    #获取群列表，返回json对象
    def getGroupList(self):
        d = {
            'sessionKey': self.sessionKey
            }
        
        r = requests.get(self.url + '/groupList', params = d)
        response = json.loads(r.text)

        return response

    #获取群成员列表，返回json对象
    def getMemberList(self, group):
        d = {
            'sessionKey': self.sessionKey,
            'target': group
            }
        
        r = requests.get(self.url + '/memberList', params = d)
        response = json.loads(r.text)

        return response

    def muteAll(self, group):
        d = {
            'sessionKey': self.sessionKey,
            'target': group
            }
        
        r = requests.post(self.url + '/muteAll', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            pass
        else:
            print('web_api/api_mirai_http.py::Send::muteAll code:', response['code'])

    def unmuteAll(self, group):
        d = {
            'sessionKey': self.sessionKey,
            'target': group
            }
        
        r = requests.post(self.url + '/unmuteAll', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            pass
        else:
            print('web_api/api_mirai_http.py::Send::muteAll code:', response['code'])

    def mute(self, group, qq, time):
        d = {
            'sessionKey': self.sessionKey,
            'target': group,
            'memberId': qq,
            'time': time
            }
        
        r = requests.post(self.url + '/mute', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            pass
        else:
            print('web_api/api_mirai_http.py::Send::mute code:', response['code'])

    def unmute(self, group, qq):
        d = {
            'sessionKey': self.sessionKey,
            'target': group,
            'memberId': qq
            }
        
        r = requests.post(self.url + '/unmute', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            pass
        else:
            print('web_api/api_mirai_http.py::Send::unmute code:', response['code'])

    #TODO
    def kick(self):
        pass

    #bot退群
    def quit(self, group):
        d = {
            'sessionKey': self.sessionKey,
            'target': group
            }
        
        r = requests.post(self.url + '/quit', data = json.dumps(d))
        response = json.loads(r.text)

        if response['code'] == 0:
            pass
        else:
            print('web_api/api_mirai_http.py::Send::quit code:', response['code'])

    #TODO 修改群设置
    def setGroupConfig(self):
        pass

    #获取群设置
    def getGroupConfig(self, group):
        d = {
            'sessionKey': self.sessionKey,
            'target': group
            }
        
        r = requests.get(self.url + '/groupConfig', params = d)
        response = json.loads(r.text)

        return response

    #TODO 修改群员资料
    def setMemberInfo(self):
        pass

    #获取群员资料
    def getMemberInfo(self, group, qq):
        d = {
            'sessionKey': self.sessionKey,
            'target': group,
            'memberId': qq
            }
        
        r = requests.get(self.url + '/memberInfo', params = d)
        response = json.loads(r.text)

        return response

    #获取指定Session的配置
    def getSessionConfig(self):
        d = {
            'sessionKey': self.sessionKey
            }
        
        r = requests.get(self.url + '/config', params = d)
        response = json.loads(r.text)

        return response

    #设置指定Session的配置
    def setSessionConfig(self, cacheSize, enableWS):
        d = {
            'sessionKey': self.sessionKey,
            'cacheSize': cacheSize,
            'enableWebsocket': enableWS
            }
        
        r = requests.post(self.url + '/config', data = json.dumps(d))
        response = json.loads(r.text)

        return response

    def msgChain(self, s):
        c = [
            {
                'type': 'Plain',
                'text': s
            },
        ]
        return c

class Receive:
    def __init__(self, sessionKey, msgRouter):
        self.sessionKey = sessionKey
        self.port = port
        self.msgRouter = msgRouter

    def on_message(self, message):
        response = json.loads(message)
        self.msgRouter.onReceived(response)

    def on_error(self, error):
        print('web_api/api_mirai_http.py::Receive::on_error:', error)

    def on_close(self):
        print('websocket closed')

    def startListen(self):
        #websocket.enableTrace(True)
        ws = websocket.WebSocketApp(ws_url + '/all?sessionKey=' + self.sessionKey,
        on_message=self.on_message,
        on_error=self.on_error,
        on_close=self.on_close)
        
        print('start receiving')
        ws.run_forever()

