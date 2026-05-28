# Solar Power Calculation for IoT Devices

A simple tutorial for estimating:

* Daily energy consumption
* Battery capacity
* Solar panel size

for a low-power IoT system.

---

# Example IoT System

The example system contains:

* PIC Microcontroller
* Sensors: S1 ... Sn
* WiFi Module
* Battery Charger Circuit
* 3.7 V LiPo Battery
* Solar Panel

---

# System Operation

The IoT device operates as follows:

1. The system stays in sleep mode most of the time
2. Every 30 minutes the device wakes up
3. The sensors are read
4. Data is transmitted using WiFi
5. The system returns to sleep mode

---

# Step 1 — Define Operating Modes

| Mode        | Description                       |
| ----------- | --------------------------------- |
| Sleep Mode  | Low-power operation while waiting |
| Active Mode | Sensors and WiFi are operating    |

---

# Step 2 — Estimate Current Consumption

Estimate the current consumption for each component.

| Component           |        Sleep Current |        Active Current |
| ------------------- | -------------------: | --------------------: |
| PIC Microcontroller |               ___ mA |                ___ mA |
| Sensor S1           |               ___ mA |                ___ mA |
| Sensor S2           |               ___ mA |                ___ mA |
| Sensor Sn           |               ___ mA |                ___ mA |
| WiFi Module         |               ___ mA |                ___ mA |
| Other Circuits      |               ___ mA |                ___ mA |
| **TOTAL**           | **I_sleep = ___ mA** | **I_active = ___ mA** |

Where:

```text
I_sleep  = Total sleep current
I_active = Total active current
```

---

# Step 3 — Calculate Wake-Up Frequency

The system transmits data every 30 minutes.

## Number of Wake-Ups Per Day

```text
Wake-ups/day = (24 × 60) / 30
             = 48
```

The device wakes up:

```text
48 times per day
```

---

# Step 4 — Estimate Active Time

Assume each wake-up event takes:

```text
t_active = ___ seconds
```

## Example

If each transmission takes 10 seconds:

```text
Total active time/day
= 48 × 10
= 480 seconds/day
```

Convert seconds to hours:

```text
480 / 3600 = 0.133 hours/day
```

Sleep time per day:

```text
t_sleep = 24 − t_active
```

---

# Step 5 — Calculate Daily Energy Consumption

Daily battery usage is:

```text
Daily mAh =
(I_active × t_active)
+
(I_sleep × t_sleep)
```

---

# Example Calculation

| Parameter           |    Value |
| ------------------- | -------: |
| Active Current      |   180 mA |
| Sleep Current       |     2 mA |
| Active Time Per Day |  0.133 h |
| Sleep Time Per Day  | 23.867 h |

Calculation:

```text
Daily mAh
=
(180 × 0.133)
+
(2 × 23.867)
```

```text
Daily mAh = 71.67 mAh/day
```

Approximate result:

```text
72 mAh/day
```

---

# Step 6 — Calculate Battery Capacity

Assume the system must operate for 3 days without sunlight.

## Battery Requirement

```text
Battery Capacity
=
Daily mAh × Backup Days
```

Example:

```text
Battery Capacity
=
72 × 3
=
216 mAh
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

# Step 7 — Calculate Solar Panel Size

Assume the system receives:

```text
4 useful sunlight hours/day
```

## Required Charging Current

```text
Required Charging Current
=
Daily mAh / Sunlight Hours
```

Example:

```text
Required Charging Current
=
72 / 4
=
18 mA
```

Add a safety factor of 2:

```text
18 × 2 = 36 mA
```

---

# Step 8 — Estimate Solar Panel Power

Use:

```text
Power = Voltage × Current
```

For a 5 V solar panel:

```text
Power
=
5 × 0.036
=
0.18 W
```

Recommended minimum panel:

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

---

# Final Results Table

| Item                          | Your Answer |
| ----------------------------- | ----------- |
| Daily Energy Use              | ___ mAh/day |
| Minimum Battery Size          | ___ mAh     |
| Recommended Battery Size      | ___ mAh     |
| Minimum Solar Panel Current   | ___ mA      |
| Recommended Solar Panel Power | ___ W       |

---

# Important Notes

* Always include a safety margin
* Solar panels rarely operate at rated power
* WiFi transmission usually consumes the most current
* Sleep current is critical in low-power IoT systems
* Lower transmission frequency increases battery life

---

# Questions

* What happens if the device transmits every 5 minutes instead of every 30 minutes?
* What happens during cloudy weather?
* Which component consumes the most energy?
* How does sleep current affect battery life?
* How does battery size affect backup operation time?

---

