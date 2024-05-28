# TinyML-enhanced-Multifunctional-Home-Smart-System

Smart Home technology seamlessly integrates various technologies into home networks to enhance the quality of human life. Researchers have been actively exploring the potential of intelligent home technology for over a decade. It represents a convergence of networks, services, and more, aiming to improve the overall living experience. This project introduces a multifunctional smart home system implemented on Arduino Nano 33 BLE, using a machine learning algorithm to interpret user commands for responsive light control. In the event of a fire, the system utilizes a flame sensor to detect it, triggering a buzzer and concurrently sending the user an email notification.

## High-Level Architecture
![tinyml-arch](https://github.com/soyraghda/TinyML-enhanced-Multifunctional-Home-Smart-System/assets/32418549/d54a9350-8c80-42c6-b98c-da76b567fe89)


## Components Overview: 

Processors: 
 
1. 	Arduino Nano 33 BLE: The Arduino Nano 33 BLE, a compact and versatile microcontroller, is the heart of our smart home system. Its Bluetooth capabilities and integrated sensors make it an ideal choice for controlling and coordinating various components.
 
2. 	ESP-01: The ESP-01 module, a compact and versatile Wi-Fi module, is designed for seamless communication between electronic devices and external networks. Equipped with a powerful ESP8266 chip, this module facilitates wireless connectivity, enabling devices to exchange data and commands over Wi-Fi.
 
Sensors:
 
1. 	Fire Flame IR Detector: This sensor utilizes infrared technology to detect the presence of flames. It acts as a critical element in our circuit for early fire detection, contributing to enhanced safety within the home environment.
 
Actuators:
 
1. 	LED (Light-Emitting Diode): An LED light is a semiconductor device that emits light when an electric current passes through it. LEDs are known for their energy efficiency, durability, and compact size. The LED light in our smart home automation system plays a crucial role, adding functionality and aesthetic appeal to the overall design. As an essential element, the LED contributes to the system's efficiency, enhancing the overall user experience in our intelligent home automation circuit.
 
2. 	Buzzer: The buzzer serves as an audible alert mechanism, producing distinctive sounds when triggered by the fire flame detector. This component ensures that occupants are promptly alerted to potential fire hazards.
Supporting Components:
1.	Resistors: Resistors are integrated into the circuit to regulate the current in the LED circuit, preventing damage to the LED.
2.	Jumper Wires: Jumper wires establish connections between different components in the circuit, ensuring a well-organized and functional circuit layout.

## Voice Detection System

### Data Collection

The dataset contains audio recordings featuring expressions like "lights on," "lights off," and background noise. The data used for the phrases "lights on" and "lights off" were systematically collected from a diverse demographic, including individuals of varied genders, ages, and accents, comprising family members, friends, and university students. Regarding the noise recordings, it was created by imitating noises and recording them, as well as using available noise recordings on YouTube. The temporal duration of the dataset spans 6 minutes and 12 seconds, encompassing a collection of 208 recordings, with 79 recordings for the "lights off" phrase, 77 for "lights on,” and 52 for background noise.

### Machine Learning Model

The dataset was split into 75% for training and 25% for testing to build the machine-learning model. The Convolutional Neural Network (CNN) model was trained for 100 epochs with a learning rate of 0.005. It achieved optimal results using 1-dimensional convolutional layers followed by max pooling layers, with neuron counts of eight in the first layer, 16 in the second, 32 in the third, and 64 in the fourth. Finally, a flattening layer and a dropout rate of 0.5 were included.

### Model Deployment

The model was deployed using the EON compiler from Edge Impulse, which converts TensorFlow Lite models into .cpp and .h files optimized for embedded systems. This approach shifts heavy computational tasks to servers, producing C++ files for efficient model inference on devices. The generated .h file was integrated into the Arduino IDE for coding the system’s firmware. The model predicts audio inputs from the Arduino’s microphone, classifying them as “lights on,” “lights off,” or “noise.” Actions are taken based on the highest prediction, such as turning lights on or off or doing nothing for noise.

## Fire Detection System

### Fire sensor and ESP-01 Connection
For the fire detection system, an infrared fire sensor was utilized, connected to an Arduino on PIN "D2" to detect fire. When a fire was detected, the Arduino turned off the light, activated a buzzer, and sent an email alert. Since the Arduino Nano 33 BLE lacked internet capability, an ESP-01 Wi-Fi module was used to send email notifications via IFTTT. The ESP-01 was programmed using the Arduino IDE and connected to a USB adapter to upload the firmware.

### Email Notification
An email notification system was developed to alert users about fire occurrences using IFTTT (If This Then That), a web-based automation platform. IFTTT enables different devices, services, and applications to communicate and initiate predefined actions based on specific conditions through "applets," which are conditional statements specifying trigger events and resulting actions. When the fire flame sensor detects a fire, IFTTT triggers a pre-made event named “fire_detected,” which sends the user an email titled “FIRE IS DETECTED IN YOUR ROOM” with the message body “THE SYSTEM HAS DETECTED FIRE IN THE ROOM, PLEASE COME AND CHECK.”

![fire](https://github.com/soyraghda/TinyML-enhanced-Multifunctional-Home-Smart-System/assets/32418549/6f948d1f-169b-470d-aac9-809983933720)

 ## Integration of Voice and Fire Systems
To integrate both systems together, The  main system firmware was made to continuously records the user's voice input and includes an interrupt code to halt operations upon fire detection. When a fire is detected, the system turns off lights and sends a serial signal denoted "1" to the ESP-01 module, indicating the fire occurrence. Upon receiving the signal, the ESP-01 verifies its first instance and attempts to connect to Wi-Fi. Upon successful connection, it triggers an email notification via an HTTP GET request to the IFTTT application, ensuring only one notification every 25 minutes using a timer function. If not the first signal reception, the ESP-01 checks the signal and timer status, connecting to Wi-Fi and triggering an email notification if conditions are met, then resetting the timer. Otherwise, it waits until the timer expires before attempting another notification.






