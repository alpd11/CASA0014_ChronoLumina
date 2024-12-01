# CASA0014 - Chrono Lumina

## Overview

**Chrono Lumina** is a MQTT controlled NeoPixel ring project designed for the CASA0014 coursework. The system combines ultrasonic sensors, RGB LED ring, and MQTT (Message Queuing Telemetry Transport) communication to create an interactive lighting experience. Distances measured by ultrasonic sensors are used to dynamically adjust the RGB intensity of LEDs, which is then transmitted to a broker for control and display.

---

## Features

- **Dynamic RGB Lighting**: Ultrasonic sensors detect distances, which are mapped to RGB intensity values for lighting.
- **MQTT Communication**: RGB values are transmitted over MQTT to a server for real-time integration with other systems.
- **Scalable Design**: Supports 12 LED pixels with adjustable RGBW values.
- **WiFi Connectivity**: Operates on a local WiFi network for seamless communication.

---

## How It Works

### Physical Setup

1. **Hardware Components**:
   - **Ultrasonic Sensors**: Three sensors (for red, green, and blue channels) measure distances in the range of 5-50 cm.
   - **Arduino MKR Wifi 1010**: Controls the sensors and communicates via WiFi and MQTT.
   - **RGBW LED Ring**: Displays the mapped light intensities based on the measured distances.

2. **Sensor Placement**:
   - Place the three ultrasonic sensors at desired positions. Each sensor corresponds to one color channel (R, G, B).
   - Ensure the sensors are unobstructed and can measure distances accurately in their respective directions.

3. **Lighting Display**:
   - The RGBW LEDs display a combination of colors whose intensity is determined by the distances measured by the sensors.

## How It Functions in the Physical World

When operational, the **Chrono Lumina** system interacts with the surrounding environment:

1. **User Interaction**:
   - When a user places their hand or an object in front of a sensor, the system measures the distance.
   - The RGB LED intensity changes dynamically, creating a visually engaging light response.

2. **Real-Time Feedback**:
   - The calculated RGB values are transmitted to an MQTT server.
   - Related system variables can be monitored using the Serial Monitor.

---

## Usage

1. Power on the system.
2. Place objects within the range of the ultrasonic sensors (5-50 cm).
3. Observe dynamic lighting changes on the RGBW LED matrix.
4. Monitor RGB values on the MQTT server for integration with other systems.

---

## Future Enhancements

- Add more sensors to expand functionality.
- Use different sketches for more sophisticated lighting effects.

---

Chrono Lumina offers a creative and functional way to bridge the physical and digital worlds through responsive lighting and IoT technology.
