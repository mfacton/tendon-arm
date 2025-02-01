import time

from serial_manager import DataType, SerialManager, pack_data

manager = SerialManager("JORCA")

types = [DataType.UInt8, DataType.UInt32]


data = pack_data(types, [0, 1])
manager.write_bytes(data)
time.sleep(0.02)

while True:
    # max 1000ish for ppr 2 (div 4)
    print("loop")
    for freq in range(100, 1000, 3):
        div = 1000000/4/freq
        data = pack_data(types, [1, int(div)])
        manager.write_bytes(data)
        time.sleep(0.02)