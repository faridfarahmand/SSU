````markdown
# Solar Power Calculation for IoT Devices

A simple tutorial for calculating battery size and solar panel sizing for low-power IoT systems.

---

# Introduction

This tutorial explains how to estimate the power requirements of an IoT device powered using:

- PIC Microcontroller
- Multiple Sensors (S1 ... Sn)
- WiFi Module
- Battery Charger Module
- 3.7 V LiPo Battery
- Solar Panel

The IoT device wakes up every 30 minutes, reads sensors, transmits data using WiFi, and then goes back to sleep.

---

# Step 1: Identify the Operating Modes

| Mode | Description |
|---|---|
| Sleep Mode | The system is mostly off and consumes very little power |
| Active Mode | The microcontroller, sensors, and WiFi module are active |

---

# Step 2: Estimate Current Consumption

Students should estimate the current consumption for each component.

| Component | Sleep Current | Active Current |
|---|---:|---:|
| PIC Microcontroller | ___ mA | ___ mA |
| Sensor S1 | ___ mA | ___ mA |
| Sensor S2 | ___ mA | ___ mA |
| Sensor Sn | ___ mA | ___ mA |
| WiFi Module | ___ mA | ___ mA |
| Other Circuits | ___ mA | ___ mA |
| **Total** | **I_sleep = ___ mA** | **I_active = ___ mA** |

---

# Step 3: Determine Wake-Up Frequency

The system transmits data every 30 minutes.

```text
Number of transmissions per day =
(24 × 60) / 30 = 48
````

The device wakes up **48 times per day**.

---

# Step 4: Estimate Active Time

Assume each transmission cycle takes:

```text
t_active = ___ seconds
```

Example:

If each transmission takes 10 seconds:

```text
Total active time =
48 × 10 = 480 seconds/day
```

Convert to hours:

```text
480 / 3600 = 0.133 hours/day
```

Sleep time per day:

```text
t_sleep = 24 − t_active
```

---

# Step 5: Calculate Daily Battery Consumption

The daily energy usage is:

```text
Daily mAh =
(I_active × t_active)
+
(I_sleep × t_sleep)
```

## Example

| Parameter           |    Value |
| ------------------- | -------: |
| Active Current      |   180 mA |
| Sleep Current       |     2 mA |
| Active Time Per Day |  0.133 h |
| Sleep Time Per Day  | 23.867 h |

```text
Daily mAh =
(180 × 0.133)
+
(2 × 23.867)
```

```text
Daily mAh = 71.67 mAh/day
```

Therefore, the system consumes approximately:

```text
72 mAh/day
```

---

# Step 6: Calculate Battery Size

Assume the system must operate for 3 days without sunlight.

```text
Battery Capacity =
Daily mAh × Backup Days
```

```text
Battery Capacity =
72 × 3 = 216 mAh
```

Add a safety factor of 2:

```text
216 × 2 = 432 mAh
```

Recommended battery:

```text
500 mAh or larger
```

---

# Step 7: Calculate Solar Panel Size

Assume there are 4 useful sunlight hours per day.

```text
Required Charging Current =
Daily mAh / Sunlight Hours
```

```text
Required Charging Current =
72 / 4 = 18 mA
```

Add a safety factor of 2:

```text
18 × 2 = 36 mA
```

For a 5 V solar panel:

```text
Power = Voltage × Current
```

```text
Power = 5 × 0.036 = 0.18 W
```

Recommended minimum solar panel:

```text
5 V, 0.5 W solar panel
```

---

# Student Exercise

For your own IoT project, calculate:

1. Total sleep current
2. Total active current
3. Number of transmissions per day
4. Total active time per day
5. Total sleep time per day
6. Daily battery usage
7. Required battery size
8. Required solar panel current
9. Required solar panel power

| Item                          | Your Answer |
| ----------------------------- | ----------- |
| Daily Energy Use              | ___ mAh/day |
| Minimum Battery Size          | ___ mAh     |
| Recommended Battery Size      | ___ mAh     |
| Minimum Solar Panel Current   | ___ mA      |
| Recommended Solar Panel Power | ___ W       |

---

# Notes

* Always add a safety factor to battery and solar panel sizing.
* Real solar panels rarely operate at their rated output power.
* WiFi transmissions often consume the largest amount of current.
* Sleep current is extremely important in low-power IoT systems.

---

# Answer the following questions: 

* What happens if the transmission interval changes from 30 minutes to 5 minutes?
* What happens if the WiFi transmission takes longer?
* What happens during cloudy weather?
* How does battery size affect backup time?
* Which component consumes the most energy?

---

# Version 
Dr. Farid Farahmand - May 2025

```
```
