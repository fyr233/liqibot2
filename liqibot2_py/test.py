import parl
import threading
import time
import queue
from multiprocessing import Pipe, Manager, Queue

@parl.remote_class
class Actor(object):
    def hello_world(self):
        print("Hello world.")

    def add(self, a, b, f, l):
        time.sleep(2)
        f()
        l.append(b)
        return a+b

# Connect to the master node.
parl.connect("localhost:8010")

def thread(i, a, f, l):
    ans = a.add(1, i, f, l)
    print(ans)

def f():
    print('f')

def main():

    ts = []
    #l = []
    q = queue.Queue()
    manager = Manager()
    l = manager.list([])
    
    for i in range(5):
        q.put(i)
        a = Actor()
        thread(i, a, f, l)
        #t = threading.Thread(target=thread, args=(i, a, f, q))
        #t.start()
        #ts.append(t)
    
    #for t in ts:
    #    t.join()
        
    print(l)

if __name__ == '__main__':
    main()