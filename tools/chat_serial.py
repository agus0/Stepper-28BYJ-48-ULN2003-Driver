import serial
import threading
import time 

def es_numero(entrada):
    try:
        float(entrada)
        return True
    except ValueError:
        return False

# Configurar el puerto serial
ser = serial.Serial('COM3', baudrate=921600, timeout=1)

# Función para procesar una línea
def procesar_linea(linea):
    if es_numero(linea):
        pasos_por_grado = 2049 / 360
        message = int(linea) * pasos_por_grado
    elif linea.startswith('n'):
        message = linea[1:]
    elif linea.startswith('v'):
        message = float(linea[1:]) * 2049
    elif linea.startswith('d'):
        time.sleep(float(linea[1:]))
        return  # No enviar el comando 's', saltar al siguiente ciclo del bucle
    else:
        message = linea  # Mensaje no modificado
    
    message_with_newline = str(message) + " "
    ser.write(message_with_newline.encode())
    
# Función para abrir y procesar un archivo
def open_file(filename):
    try:
        with open(filename, 'r') as file:
            lineas = file.readlines()
            total_lineas = len(lineas)
            print(f"{filename} abierto...")
            for i, linea in enumerate(lineas, start=1):
                print(f"Linea {i} de {total_lineas}: {linea.strip()}")
                procesar_linea(linea.strip())
            print(f"\n{filename} finalizado...")
    except FileNotFoundError:
        print("El archivo no existe.")

# Función para enviar mensaje
def send_message():
    while True:
        message = input()
        if message.endswith('.txt'):
            # Iniciar un hilo para abrir y procesar el archivo
            file_thread = threading.Thread(target=open_file, args=(message,))
            file_thread.start()
        else:
            procesar_linea(message)

# Función para leer mensajes
def read_messages():
    while True:
        if ser.in_waiting > 0:
            message = ser.readline().decode().strip()
            print("\n\nMensaje recibido:", message, "\n")

# Iniciar hilos para enviar y recibir mensajes
send_thread = threading.Thread(target=send_message)
send_thread.daemon = True
send_thread.start()

read_thread = threading.Thread(target=read_messages)
read_thread.daemon = True
read_thread.start()

# Mantener el programa en ejecución
while True:
    pass
