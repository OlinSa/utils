import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


if __name__ == "__main__":
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", 8000))
    s.send(b"Hello, I am client")
    print("--->>"+s.recv(1024).decode("utf-8"))
    s.send(b"exit")
    s.close()