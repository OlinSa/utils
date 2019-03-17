import os
import time
import random
from multiprocessing import Pool, Queue, Process, Pipe

# 进程创建


def run_task(name):
    print("Task %s (pid=%s) is runing ... " % (name, os.getpid()))
    time.sleep(random.random() * 3)
    print("Task  %s end" % (name))


def fork_demo():
    pid = os.fork()
    if pid < 0:
        print("error in fork")
    elif pid == 0:
        print("%s create a child process (%s) ." % (os.getpid(), os.getppid()))
    else:
        print("create process failed")


def multi_demo():
    for i in range(5):
        p = Process(target=run_task, args=(str(i),))
        p.start()
    p.join()
    print("Process Edn")


def multi_process_pool_demo():
    # 使用进程池
    p = Pool(processes=3)
    for i in range(5):
        p.apply_async(run_task, args=(i,))
    print("wait for all subprocess done...")
    p.close()
    p.join()
    print("all subprocess done...")

# 进程间通信,有Queue、Pipe、value+array


def proc_write(q, urls):
    print('Prcess(%s) is writing...' % (os.getpid()))
    for url in urls:
        q.put(url)
        print("put %s to queue..." % url)
        time.sleep(random.random())


def proc_read(q):
    print('Prcess(%s) is reading...' % (os.getpid()))
    while True:
        url = q.get(True)
        print("Get %s from  queue." % url)


def queue_communicate_process_demo():
    q = Queue()
    proc_writer1 = Process(target=proc_write,  args=(
        q, ['url_1', 'url_2', 'url_3']))
    proc_writer2 = Process(target=proc_write,  args=(
        q, ['url_4', 'url_5', 'url_6']))
    proc_reader = Process(target=proc_read,  args=(q,))

    proc_writer1.start()
    proc_writer2.start()
    proc_reader.start()

    proc_writer1.join()
    proc_writer2.join()
    proc_reader.terminate()


def proc_pip_send(pipe, urls):
    for url in urls:
        print("Process(%s) send:%s" % (os.getpid(), url))
        pipe.send(url)
        time.sleep(random.random())


def proc_pip_recv(pipe):
    while True:
        print("Process(%s) recv:%s" % (os.getpid(), pipe.recv()))
        time.sleep(random.random())


def pip_commuicate_process_demo():
    pipe = Pipe()
    p1 = Process(target=proc_pip_send, args=(
        pipe[0], ['url_'+str(i) for i in range(10)]))
    p2 = Process(target=proc_pip_recv, args=(pipe[1],))
    p1.start()
    p2.start()
    p1.join()
    p2.join()


if __name__ == "__main__":
    print("current Process(%s) start..." % (os.getpid()))
    fork_demo()
    multi_demo()
    multi_process_pool_demo()
    queue_communicate_process_demo()
    pip_commuicate_process_demo()
