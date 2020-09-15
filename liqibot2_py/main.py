from multiprocessing import Manager,Pool

from web_api import api_mirai_http as API
import MsgRouter

def main():

    api_send = API.Send()
    msgRouter = MsgRouter.MsgRouter(Pool(), Manager(), api_send)
    api_receive = API.Receive(api_send.sessionKey, msgRouter)

    print(api_send.getSessionConfig())

    api_receive.startListen()

    api_send.release()
    msgRouter.close()

if __name__ == '__main__':
    main()