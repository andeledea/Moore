import tkinter as tk
import serial
import time
from itertools import chain

ser = serial.Serial('COM5', 460800, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)

direction = True
windings = True
angle_fine = False
speed = 0

# MATH
def map(x, v_min, v_max, t_min, t_max):
    T = t_max/((x - v_min) * (t_max/t_min - 1) / (v_max - v_min) + 1)
    return T

def calcALL(n):
    """
    Calculates from n f, v_r, v_m
    """
    clk = 84e6
    f = clk / (1 * 2 * map(n, 0, 65535, 1400, 3200000))
    n_step_giro = 250000 if angle_fine else 1000
    v_r = f / n_step_giro
    v_m = v_r / 10
    return f, v_r, v_m

# CALLBACKS
def on_direction_click():
    global direction
    direction = not direction
    send_speed()

def on_windings_click():
    global windings
    windings = not windings
    send_speed()

def on_angle_fine_click():
    global angle_fine
    angle_fine = not angle_fine
    send_speed()

def on_ramp_click():
    global speed
    step = int(number_entry.get())  
    for i in chain(range(0, 65536, step), range(65535, -1, -step)):
        speed = i
        send_speed()
        time.sleep(0.00005)
        root.update()
    speed = 0
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
    f, v_r, v_m = calcALL(speed)

    label.config(text=f"n: {speed}, f: {f / 1000:.2f} kHz, v_rot: {v_r:.2f} RPS, v_lin: {v_m:.2f} mm/s, sending: {send_string}")
    button1.config(text = 'fore' if direction else 'back')
    button2.config(text = 'w_on' if windings else 'w_off')
    button3.config(text = 'fine_angle' if angle_fine else 'default_angle')
    if speed > 0: 
        button1.config(state='disabled')
        button2.config(state='disabled')
        button3.config(state='disabled')
        button4.config(state='disabled')
    else: 
        button1.config(state='normal')
        button2.config(state='normal')
        button3.config(state='normal')
        button4.config(state='normal')

    speed_var.set(speed)

def send_speed():
    global speed, direction, windings, angle_fine
    send_string = f"{'f' if direction else 'b'}{'w' if windings else 'm'}{'s' if angle_fine else 'r'}{speed}\r"
    
    ser.write(send_string.encode('ascii'))
    update_ui(send_string)

def on_closing():
    ser.write(bytearray([0]))
    ser.close()
    root.destroy()  # Close the window

# TK
root = tk.Tk()
root.title("STM32 VEXTA")
root.geometry("900x400")

import tkinter.font as tkFont
default_font = tkFont.nametofont("TkDefaultFont")
default_font.configure(size=15)

label = tk.Label(root, text="Speed: 0")
label.pack(pady=20)

speed_var = tk.IntVar()
slider = tk.Scale(root,
                  variable=speed_var,
                  from_=0, to=65535, 
                  orient=tk.HORIZONTAL, command=set_speed)
slider.pack(fill=tk.X, padx=20, pady=20)

button_frame = tk.Frame(root)
button_frame.pack(fill=tk.X, padx=10, pady=10)

button1 = tk.Button(button_frame, command=on_direction_click, text = 'fore' if direction else 'back')
button1.pack(side=tk.LEFT, fill=tk.X, expand=True)

button2 = tk.Button(button_frame, command=on_windings_click, text = 'w_on' if windings else 'w_off')
button2.pack(side=tk.LEFT, fill=tk.X, expand=True)

button3 = tk.Button(button_frame, command=on_angle_fine_click, text = 'fine_angle' if angle_fine else 'default_angle')
button3.pack(side=tk.LEFT, fill=tk.X, expand=True)

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

root.protocol("WM_DELETE_WINDOW", on_closing)
root.mainloop()
