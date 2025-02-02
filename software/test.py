import time

from serial_manager import DataType, SerialManager, pack_data

manager = SerialManager("JORCA")

joint_msg = [DataType.Int16, DataType.Int16, DataType.Int16, DataType.Int16]
enable_msg = [DataType.UInt8]

def send_joints(j1, j2, j3, j4):
    data = pack_data(joint_msg, [j1, j2, j3, j4])
    manager.write_bytes(data)

def send_enable(e1, e2, e3, e4):
    val = ((e4&1)<<3)|((e3&1)<<2)|((e2&1)<<1)|(e1&1)
    data = pack_data(enable_msg, [val])
    manager.write_bytes(data)

j1 = 0
j2 = 0
j3 = 0
j4 = 0

send_enable(1, 1, 1, 1)
while True:
    send_joints(int(j1), int(j2), int(j3), int(j4))
    time.sleep(0.01)
    j1 += 4