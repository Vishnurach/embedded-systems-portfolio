"""
LPC1768 Temperature Dashboard
Requires: pip install pyserial
"""
import tkinter as tk
from tkinter import scrolledtext
import serial
import serial.tools.list_ports
import threading

# CONFIGURATION
SERIAL_PORT = 'COM9'  # Update this to match your Virtual Port
BAUD_RATE = 9600

class DashboardApp:
    def __init__(self, root):
        self.root = root
        self.root.title("LPC1768 Temperature Monitor")
        self.root.geometry("600x480")
        
        self.ser = None
        self.is_reading = False

        # --- Connection Header ---
        self.conn_frame = tk.Frame(root, pady=10, bg="#f0f0f0")
        self.conn_frame.pack(side=tk.TOP, fill=tk.X)
        
        self.btn_connect = tk.Button(self.conn_frame, text="Connect", command=self.connect_serial)
        self.btn_connect.pack(side=tk.LEFT, padx=10)
        
        self.lbl_status = tk.Label(self.conn_frame, text="Status: Disconnected", fg="red", bg="#f0f0f0")
        self.lbl_status.pack(side=tk.LEFT, padx=10)

        # --- Main Layout ---
        self.main_container = tk.Frame(root)
        self.main_container.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        # --- Control Hub (Left) ---
        self.control_frame = tk.LabelFrame(self.main_container, text="Control Hub", padx=10, pady=10)
        self.control_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        tk.Label(self.control_frame, text="User LED (P0.1) Control", fg="gray").pack(pady=(0, 10))

        self.btn_on = tk.Button(self.control_frame, text="Turn LED ON", 
                                command=lambda: self.send_data('A'), height=2, bg="#90ee90")
        self.btn_on.pack(fill=tk.X, pady=5)

        self.btn_off = tk.Button(self.control_frame, text="Turn LED OFF", 
                                 command=lambda: self.send_data('B'), height=2, bg="#ffcccb")
        self.btn_off.pack(fill=tk.X, pady=5)

        tk.Label(self.control_frame, text="\nSystem Status:\nLED 1 (P0.0) is Heartbeat\nLED 2 (P0.1) is User", 
                 justify=tk.LEFT, fg="blue").pack(pady=20)

        # --- Live Data Feed (Right) ---
        self.log_frame = tk.LabelFrame(self.main_container, text="Sensor Feed (LM35)", padx=10, pady=10)
        self.log_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

        self.log_text = scrolledtext.ScrolledText(self.log_frame, width=35, height=15, state='disabled')
        self.log_text.pack(fill=tk.BOTH, expand=True)

    def connect_serial(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            self.lbl_status.config(text="Status: Disconnected", fg="red")
            self.btn_connect.config(text="Connect")
            self.is_reading = False
        else:
            try:
                self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
                self.lbl_status.config(text=f"Connected to {SERIAL_PORT}", fg="green")
                self.btn_connect.config(text="Disconnect")
                self.is_reading = True
                
                self.thread = threading.Thread(target=self.read_serial_loop)
                self.thread.daemon = True 
                self.thread.start()
            except Exception as e:
                self.lbl_status.config(text=f"Error: {e}", fg="red")

    def read_serial_loop(self):
        while self.is_reading and self.ser and self.ser.is_open:
            try:
                if self.ser.in_waiting > 0:
                    data = self.ser.readline().decode('utf-8', errors='ignore').strip()
                    if data:
                        self.update_logger(data)
            except Exception:
                break

    def update_logger(self, message):
        self.log_text.config(state='normal') 
        self.log_text.insert(tk.END, message + "\n") 
        self.log_text.see(tk.END) 
        self.log_text.config(state='disabled') 

    def send_data(self, char_to_send):
        if self.ser and self.ser.is_open:
            self.ser.write(char_to_send.encode())
            if char_to_send == 'A':
                self.update_logger(">> Status: User turned LED 2 ON")
            elif char_to_send == 'B':
                self.update_logger(">> Status: User turned LED 2 OFF")

if __name__ == "__main__":
    root = tk.Tk()
    app = DashboardApp(root)
    root.mainloop()
