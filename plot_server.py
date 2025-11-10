import matplotlib
matplotlib.use('TkAgg') # <--- Add this line here
import socket
import threading
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque

# --- Configuration ---
HOST = '0.0.0.0'  # Listen on all available network interfaces
PORT = 8888       # Must match the port in your ESP32 code

# --- Data storage for plotting ---
# Using a deque for efficient adding and removal of data points
max_data_points = 50
timestamps = deque(maxlen=max_data_points)
distances = deque(maxlen=max_data_points)

# --- Server logic ---
# A thread-safe queue to pass data from the server thread to the main thread
data_queue = deque()
lock = threading.Lock()

def handle_client(conn, addr):
    """Handles data reception from a single client (the ESP32)"""
    print(f"Connected by {addr}")
    try:
        while True:
            # Receive data in chunks of 1024 bytes
            data = conn.recv(1024)
            if not data:
                break
            
            # Decode the received data and split by newline character
            message = data.decode('utf-8').strip()
            
            # Process each distance reading
            if message:
                try:
                    distance = float(message)
                    with lock:
                        data_queue.append(distance)
                    print(f"Received: {distance} cm")
                except ValueError:
                    print(f"Invalid data received: {message}")
    finally:
        conn.close()
        print(f"Connection from {addr} closed")

def start_server():
    """Sets up and starts the TCP server in a separate thread"""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen(1)
        print(f"Server listening on port {PORT}...")
        conn, addr = s.accept()
        handle_client(conn, addr)

# --- Plotting logic ---
fig, ax = plt.subplots()

def animate(i):
    """Function to be called by FuncAnimation to update the plot"""
    with lock:
        # Check if new data is available in the queue
        while data_queue:
            new_distance = data_queue.popleft()
            distances.append(new_distance)
            timestamps.append(time.time())

    # Clear and re-draw the plot
    ax.clear()
    if timestamps:
        # Plot the data
        ax.plot(timestamps, distances, 'b-')

        # Format the plot
        plt.xlabel('Time (s)')
        plt.ylabel('Distance (cm)')
        plt.title('Real-time Distance Data from ESP32')
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()

# Start the server in a separate thread so it doesn't block the main program
server_thread = threading.Thread(target=start_server)
server_thread.daemon = True # Allow the program to exit even if this thread is running
server_thread.start()

# Start the real-time plot animation
ani = FuncAnimation(fig, animate, interval=250, cache_frame_data=False) # <--- Added cache_frame_data=False to fix the first warning
plt.show()