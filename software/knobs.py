import time
import tkinter as tk
from tkinter import ttk

from serial_manager import DataType, SerialManager, pack_data

manager = SerialManager("JORCA")

joint_msg = [DataType.Int16, DataType.Int16, DataType.Int16, DataType.Int16]
enable_msg = [DataType.UInt8]

def t(val):
    if val == 0:
        return 30000
    else:
        print(int(6000/val))
        return int(6000/val)

def send_joints():
    data = pack_data(joint_msg, [t(j1.get()), t(j2.get()), t(j3.get()), t(j4.get())])
    manager.write_bytes(data)
    root.after(10, send_joints)  # Schedule the function to run at 100Hz

def send_enable():
    val = ((e4.get() & 1) << 3) | ((e3.get() & 1) << 2) | ((e2.get() & 1) << 1) | (e1.get() & 1)
    data = pack_data(enable_msg, [val])
    manager.write_bytes(data)

def toggle_enable():
    send_enable()

root = tk.Tk()
root.title("Motor Control")

j1, j2, j3, j4 = tk.IntVar(), tk.IntVar(), tk.IntVar(), tk.IntVar()
e1, e2, e3, e4 = tk.IntVar(value=1), tk.IntVar(value=1), tk.IntVar(value=1), tk.IntVar(value=1)

for i, (var, label) in enumerate([(j1, "Joint 1"), (j2, "Joint 2"), (j3, "Joint 3"), (j4, "Joint 4")]):
    frame = ttk.Frame(root)
    frame.pack(pady=5, fill='x')
    ttk.Label(frame, text=label).pack()
    scale = ttk.Scale(frame, from_=10, to=100, variable=var, orient='horizontal', length=800)
    scale.pack()

enable_frame = ttk.Frame(root)
enable_frame.pack(pady=10)

ttk.Label(enable_frame, text="Enable Motors:").pack()
for i, (var, label) in enumerate([(e1, "E1"), (e2, "E2"), (e3, "E3"), (e4, "E4")]):
    chk = ttk.Checkbutton(enable_frame, text=label, variable=var, command=toggle_enable)
    chk.pack(side=tk.LEFT)

send_joints()  # Start sending joint updates at 100Hz

root.mainloop()