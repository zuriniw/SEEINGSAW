# Remote See-saw
![](https://pic.superbed.cc/item/675a0762fa9f77b4dc1a8ac9.png)

## To Run the Code
- Open the `Seesaw` file; this is all you need to get started.
- Prepare two Photon 2 devices, and make sure you have registered two Particle accounts.
- Prepare electronic components for each device as the BOM table listed:

| Item | Component                                 | Quantity | Description                                                    |
|------|-------------------------------------------|----------|----------------------------------------------------------------|
| 1    | Particle Photon 2                         | 1        | IoT development board with dual-band Wi-Fi and BLE support6    |
| 2    | Linear Actuator (LA-T8-12-50-100/155-20)  | 1        | 12V, 50mm/s, 100mm stroke, 20N force, 10% duty cycle, IP54 rated |
| 3    | QTR-1RC Reflectance Sensor                | 1        | Infrared LED and phototransistor pair for object detection2    |
| 4    | LED                                       | 3        | Indicator lights (color not specified)                         |
| 5    | Push Button Switch                        | 3        | Optional, for testing purposes4                                |
| 6    | L298N H-Bridge Motor Driver               | 1        | Dual H-bridge driver for controlling the linear actuator7      |
| 7    | Power Supply 12V                          | 1        | For linear actuator and L298N motor driver                     |

- Wire them as shown below:
    - ![](https://pic.superbed.cc/item/675a160ffa9f77b4dc1b0713.png)
    - Pin Setup
        - LED indicators:
            - Red (D2)
            - Yellow (D0)
            - Blue (D1)
        - QTR-1RC sensor on D3
        - L298N motor controller:
            - Enable (A2)
            - Direction controls (D6, D7)
- Set up two webhooks, one for each device.
- Fill out the sketches in the Particle Web IDE for your two photons.
- In my case, Device 1 is ZW's Photon 2, while Device 2 is LL's Photon 2. There are some unknown issues with LL's Photon 2 that lead to disconnections when powering the linear actuator with a 12V power supply. Therefore, we use a 5V one instead, which leads to variance in full speed compared with device 1. Consequently, I adjusted the preset speed and total length in the `device_2` file to ensure the position reaches correctly. If your photon can work well with 12V (congrats!), you can adjust it back as what has been noted in the comment in the sketch for device 2.

## Our Project
[Under Construction 🏗]

### System Logic
![](https://pic.superbed.cc/item/675a3099fa9f77b4dc1be809.png)

**Components Function**
- Red LED: actuator position state
- Yellow LED: local object detection
- Blue LED: remote device's object detection

**Movement Logic**
- Both sides same state: Move to middle position (totalLength/2)
- Object in Device 1 only: Device 1 extends, Device 2 retracts
- Object in Device 2 only: Device 1 retracts, Device 2 extends

**Communication System**
- Communication via Particle.publish() and Particle.subscribe()
- Device 1 broadcasts "doBlue_1" events
- Device 2 broadcasts "doBlue_2" events
- Each device updates state based on partner events

**Actuator Control**
- Position tracking using time-based calculations
- Movement functions: extend, retract, stop
- Movement calculations based on speed
