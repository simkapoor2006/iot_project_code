# IoT Fire Detection System using ESP32 and Firebase

An IoT-based smart fire detection and alarm system developed using ESP32 and Firebase for real-time monitoring and alert generation. The system continuously monitors the environment using a flame sensor and activates a buzzer whenever fire is detected. Sensor data is also updated to Firebase for remote monitoring.

---

## Features

- Real-time fire detection
- Instant buzzer/alarm activation
- Firebase-based real-time monitoring
- Wi-Fi enabled communication using ESP32
- Lightweight and cost-effective IoT solution

---

## Technologies Used

- ESP32
- Arduino IDE
- Firebase Realtime Database
- Embedded C/C++
- IoT

---

## Components Used

- ESP32 Development Board
- Flame Sensor
- Temperature sensor
- Smoke sensor
- Buzzer
- Breadboard
- Jumper Wires
- USB Cable

---

## Working Principle

The flame sensor continuously monitors the surroundings for fire or flames. When fire is detected:

1. The flame sensor sends input to the ESP32
2. ESP32 processes the sensor data
3. The buzzer is activated immediately
4. Fire detection data is uploaded to Firebase
5. Real-time monitoring is achieved through internet connectivity

This system helps in improving fire safety by enabling quick detection and remote monitoring.

---

## Project Structure

```text
iot_project_code/
│
├── iot_code.ino
├── README.md
```

---

## Setup Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/iot_project_code.git
```

### 2. Open the Project

Open the `.ino` file using Arduino IDE.

### 3. Install Required Libraries

Install the necessary ESP32 and Firebase libraries from Arduino IDE Library Manager.

### 4. Configure Credentials

Replace placeholder values in the code with your own credentials:

```cpp
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define API_KEY "YOUR_FIREBASE_API_KEY"
```

### 5. Upload Code to ESP32

- Select the correct ESP32 board
- Select the correct COM port
- Upload the code to ESP32

---

## Future Improvements

- Mobile app notifications
- LCD/OLED display integration
- SMS/Email alert system
- Battery backup support

---

## Team

Developed by:
- Simran Kapoor
- Sharanya Singh