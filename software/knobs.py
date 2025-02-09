import time
import tkinter as tk
from tkinter import ttk

from serial_manager import DataType, SerialManager, pack_data

manager = SerialManager("JORCA")

joint_msg = [DataType.Float, DataType.Float]
enable_msg = [DataType.UInt8]

def send_joints():
    data = pack_data(joint_msg, [j1.get(), j2.get()])
    manager.write_bytes(data)
    root.after(10, send_joints)  # Schedule the function to run at 100Hz

def send_enable():
    val = ((e2.get() & 1) << 1) | (e1.get() & 1)
    data = pack_data(enable_msg, [val])
    manager.write_bytes(data)

def toggle_enable():
    send_enable()

root = tk.Tk()
root.title("Motor Control")

j1, j2 = tk.IntVar(), tk.IntVar()
e1, e2 = tk.IntVar(value=1), tk.IntVar(value=1)

for i, (var, label) in enumerate([(j1, "Joint 1"), (j2, "Joint 2")]):
    frame = ttk.Frame(root)
    frame.pack(pady=5, fill='x')
    ttk.Label(frame, text=label).pack()
    scale = ttk.Scale(frame, from_=0.1, to=2.0, variable=var, orient='horizontal', length=800)
    scale.pack()

enable_frame = ttk.Frame(root)
enable_frame.pack(pady=10)

ttk.Label(enable_frame, text="Enable Motors:").pack()
for i, (var, label) in enumerate([(e1, "E1"), (e2, "E2")]):
    chk = ttk.Checkbutton(enable_frame, text=label, variable=var, command=toggle_enable)
    chk.pack(side=tk.LEFT)

send_joints()  # Start sending joint updates at 100Hz

root.mainloop()