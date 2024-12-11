# Remote See-saw
![](https://pic.superbed.cc/item/675a0762fa9f77b4dc1a8ac9.png)

## To run the code
- Open the `Seesaw` file; this is all you need to get started.
- Prepare two Photon 2 devices, and make sure you have registered two Particle accounts.
- Wire them as this:
    - ![](https://pic.superbed.cc/item/675a160ffa9f77b4dc1b0713.png)
- Set up two webhooks, one for each device.
- Fill out the sketches in the Particle Web IDE for your two photons.
- In my case, Device 1 is ZW's Photon 2, while Device 2 is LL's Photon 2. There are some unknown issues with LL's Photon 2 that lead to disconnections when powering the linear actuator with a 12V power supply. Therefore, we use a 5V one instead, which leads to variance in full speed compared with device 1. Consequently, I adjusted the preset speed and total length in the `device_2` file to ensure the position reaches correctly. If your photon can work well with 12V (congrats!), you can adjust it back as what has been noted in the comment in the sketch for device 2.

## Our Project
[Under Construction 🏗]
