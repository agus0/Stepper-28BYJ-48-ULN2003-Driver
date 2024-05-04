#include <AccelStepper.h>
#define HALFSTEP 4 //4 full //8half

// Motor pin definitions
#define motorPin1  8     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  10     // IN3 on the ULN2003 driver 1
#define motorPin4  11     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  stepper1.setMaxSpeed(500);
  stepper1.setSpeed(500);
  stepper1.setAcceleration(2000);
  Serial.begin(921600);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Leer el comando desde el puerto serie

    // Verificar si el último carácter es una nueva línea
    if (command.endsWith("\n") || command.endsWith("\0")) {
      // Eliminar el último carácter (nueva línea) de la cadena
      command.remove(command.length() - 1);
    }

    if (command.startsWith("speed ")) { // Si el comando comienza con "speed "
      int speedValue = command.substring(6).toInt(); // Obtener el valor de velocidad después de "speed "
      // Limitar la velocidad dentro del rango permitido
      speedValue = max(1, min(speedValue, 500));
      stepper1.setMaxSpeed(speedValue);
      stepper1.setSpeed(speedValue);
      Serial.println("speed " + String(speedValue)); // Enviar confirmación por serial
    } else if (command == "stop") { // Si el comando es "stop"
      //stopMotor(); // Llamar a la función para detener el motor
      stepper1.stop();
      Serial.println("motor stop"); // Enviar confirmación por serial
    } else if (command == "isrunning") { // Si el comando es "reboot"
      Serial.println("isrunning "+String(stepper1.isRunning())); 
    } else if (command == "reboot") { // Si el comando es "reboot"
      Serial.println("reboot"); // Enviar confirmación por serial
      rebootArduino(); // Llamar a la función para reiniciar el Arduino
    } else { // Si no es "stop" ni "reboot", ni "speed XX", se asume que es un número
      long steps = command.toInt();
      if (steps == 0){
        stepper1.stop();
        stopMotor();
        Serial.println("motor stop");
      }else{
        stepper1.move(steps);
        Serial.println("mov "+String(steps)); // Enviar confirmación por serials
      }
    }
  }
  stepper1.run();
}

// Función para detener el motor
void stopMotor() {
  // Poner todos los pines de control del motor en LOW
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

// Función para reiniciar el Arduino
void rebootArduino() {
  // Reiniciar el Arduino
  asm volatile ("  jmp 0");
}
