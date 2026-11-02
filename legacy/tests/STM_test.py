import tkinter as tk
import serial
import time
from itertools import chain

ser = serial.Serial('COM7', 57600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)

activeaxis = 'x'
direction = True
enable = False
speed = 0

MINspeed = 0
MAXspeed = 65535

# MATH
def map(x, v_min, v_max, t_min, t_max):
    T = t_max/((x - v_min) * (t_max/t_min - 1) / (v_max - v_min) + 1)
    return T

# CALLBACKS
def on_direction_click():
    global direction
    direction = not direction
    send_speed()

def on_enable_click():
    global enable
    enable = not enable
    send_speed()

def on_ramp_click():
    global speed, enable
    step = int(number_entry.get())
    enable = True
    for i in chain(range(MINspeed, MAXspeed, step), range(MAXspeed, MINspeed-1, -step)):
        speed = i
        send_speed()
        time.sleep(0.00005)
        root.update()
    speed = 0
    enable = False
    send_speed()

def on_zero_click():
    global speed
    step = int(number_entry.get())  
    for i in range(speed, -1, -step):
        speed = i
        send_speed()
        time.sleep(0.00005)
        root.update()
    speed = 0
    send_speed()

def set_speed(value):
    global direction, speed
    speed = int(value)
    send_speed()

def update_ui(send_string):

    label.config(text=f"n: {speed}, sending: {send_string}")
    button1.config(text = 'fore' if direction else 'back')
    button2.config(text = 'enabled' if enable else 'disabled')

    if speed > 0: 
        button1.config(state='disabled')
        button2.config(state='disabled')
        button4.config(state='disabled')
    else: 
        button1.config(state='normal')
        button2.config(state='normal')
        button4.config(state='normal')

    speed_var.set(speed)

def send_speed():
    global speed, activeaxis, direction, enable
    send_string = f"{activeaxis}{'e' if enable else 'd'}{'b' if direction else 'f'}{speed}\r"
    
    ser.write(send_string.encode('ascii'))
    update_ui(send_string)

def on_closing():
    ser.write(bytearray([0]))
    ser.close()
    root.destroy()  # Close the window

def set_axis_selection():
    global activeaxis
    activeaxis = axisselector.get()
    send_speed()

# TK
root = tk.Tk()
root.title("STM32")
root.geometry("900x400")

import tkinter.font as tkFont
default_font = tkFont.nametofont("TkDefaultFont")
default_font.configure(size=15)

label = tk.Label(root, text="Speed: 0")
label.pack(pady=20)

speed_var = tk.IntVar()
slider = tk.Scale(root,
                  variable=speed_var,
                  from_=MINspeed, to=MAXspeed, 
                  orient=tk.HORIZONTAL, command=set_speed)
slider.pack(fill=tk.X, padx=20, pady=20)

button_frame = tk.Frame(root)
button_frame.pack(fill=tk.X, padx=10, pady=10)

button1 = tk.Button(button_frame, command=on_direction_click, text = 'fore' if direction else 'back')
button1.pack(side=tk.LEFT, fill=tk.X, expand=True)

button2 = tk.Button(button_frame, command=on_enable_click, text = 'enabled' if enable else 'disabled')
button2.pack(side=tk.LEFT, fill=tk.X, expand=True)

button4 = tk.Button(button_frame, command=on_ramp_click, text = 'ramp')
button4.pack(side=tk.LEFT, fill=tk.X, expand=True)

input_frame = tk.Frame(root)
input_frame.pack(padx=10, pady=10)

label2 = tk.Label(input_frame, text="Ramp step: ")
label2.pack(side=tk.LEFT, fill=tk.X, expand=True)
number_entry = tk.Entry(input_frame)
number_entry.insert(0, "50")  
number_entry.pack(side=tk.LEFT, fill=tk.X, expand=True)
button5 = tk.Button(input_frame, command=on_zero_click, text = 'zero')
button5.pack(side=tk.LEFT, fill=tk.X, expand=True)

# Create a variable to hold the selected value
axisselector = tk.StringVar(value="x")  # Default value

# Create radio buttons
radio_x = tk.Radiobutton(root, text="X", variable=axisselector, value="x", command=set_axis_selection)
radio_y = tk.Radiobutton(root, text="Y", variable=axisselector, value="y", command=set_axis_selection)
radio_z = tk.Radiobutton(root, text="Z", variable=axisselector, value="z", command=set_axis_selection)

radio_x.pack(side=tk.LEFT, fill=tk.X, expand=True)
radio_y.pack(side=tk.LEFT, fill=tk.X, expand=True)
radio_z.pack(side=tk.LEFT, fill=tk.X, expand=True)

root.protocol("WM_DELETE_WINDOW", on_closing)
root.mainloop()
