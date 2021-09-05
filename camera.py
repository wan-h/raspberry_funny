'''
树莓派启动rtsp流脚本
v4l2rtspserver -W 600 -H 480
'''

import cv2
import queue
import threading

q = queue.Queue()

def camRecive(address):
    cap = cv2.VideoCapture(address)
    # 读取视频
    success, frame = cap.read()
    while success:
        q.put(frame)
        success, frame = cap.read()

def display():
    while True:
        if not q.empty():
            frame = q.get()
            cv2.imshow('frame', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            cv2.destroyAllWindows()
            break

if __name__ == '__main__':
    p = threading.Thread(target=display)
    p.start()
    camRecive('rtsp://192.168.0.105:8554/unicast')
