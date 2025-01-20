import cv2
import socket
import struct

# sock hai hamara socket
# csock hai hamara client ka socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(("127.0.0.1", 1234))
sock.listen(100)
csock, address = sock.accept()

print(f"Connection from {address} has been established!")
csock.send(bytes("Welcome to the server!", "utf-8"))

cap = cv2.VideoCapture(0)
fps = cap.get(cv2.CAP_PROP_FPS)  # YAHA PE YEH ISILIYE HAI TAKI HAME VIDEO JITNE FRAMES PE AA RHI H UTNE PE HI SHOW HO JAYE WARNA HAM CUSTOM VALUE DAAL SKTE HAI'''

while cap.isOpened():
    ret, frame = cap.read()  # ret is bool, returns t/f agar the frame is received

    if not ret:
        break

    encoded, buffer = cv2.imencode('.jpg', frame)  # nparray to jpg
    if not encoded:  # encoded bata raha hai ki kya woh encode hua
        break

    byte_data = buffer.tobytes()  # jpg se byte stream
    frame_size = len(byte_data)  # byte stream ki length

    csock.sendall(struct.pack('!I', frame_size))  # pehle size send krow as tuple
    csock.sendall(byte_data)  # byte stream send krow

cap.release()
cv2.destroyAllWindows()

csock.send(bytes("Video Finished!", "utf-8"))
csock.close()
sock.close()

