import random
import time, threading

def thread_run(urls):
    print("current %s is runing ..." %(threading.current_thread().name))
    for url in urls:
        print("%s-->>>%s" %(threading.current_thread().name,url))
        time.sleep(random.random())
    print("%s ended" %(threading.current_thread().name))

def thread_demo():
    t1 = threading.Thread(target=thread_run, name="Thread_1",args=(['url_1','uri-2'],))
    t2 = threading.Thread(target=thread_run, name="Thread_2",args=(['url_3','uri-4'],))
    
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    print("%s ended." %threading.current_thread().name)


class SThread(threading.Thread):
    def __init__(self, name, urls):
        threading.Thread.__init__(self,name=name)
        self.urls=urls
    def run(self):
        print("current %s is runing..." %threading.current_thread().name)
        for url in self.urls:
            print("%s --->>>%s"%(threading.current_thread().name, url))
            time.sleep(random.random())
        
def thread_demo_extend():
    print("%s is running..." %(threading.current_thread().name))
    t1 = SThread(name="thread_1", urls=['url_1','url_2'])
    t2 = SThread(name="thread_2", urls=['url_3','url_4'])
    t1.start()
    t2.start()
    t1.join()
    t2.join()

if __name__ == "__main__":
    thread_demo()
    thread_demo_extend()