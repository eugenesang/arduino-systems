# Wind Turbine Controller

## Task

Design a controller for a wind turbine, with the following specifications:
- wind speed and wind direction sensors (analog inputs)
- 360° rotational control of turbine (servo motor and constant Serial monitor output)
- 45° blade pitch control (45° maximum attack, 0° minimum attack) (servo motor and
constant Serial monitor output)
- turbine cut-in wind speed is `user-input` m/s, cut-out wind speed `user-input` m/s
- turbine will not rotate below cut-in or above cut-out (force blades to turn horizontal, and
turbine to rotate parallel to wind)
- when active, turbine rotates to be perpendicular to wind, and blades at 45°
- blade pitch rotates to 0° to reduce rotational speed for high wind speeds. Blade pitch
should rotate linearly starting at 5m/s less than the `user-input` cut-out wind speed (e.g.
from 20-25 m/s, angle of attack reduced linearly from 45° to 0°)
- periodic logging (i.e. data storage) of wind speed sensor, wind direction sensor, and reading
times at `user-input` interval
- all logged values output serially at end of day
- Emergency-stop / disconnect button

## Components

### Potentiometers

**Wind Speed Input:**  
> **Role:** Simulate the wind speed  
**I/O type:** Input  
**Pin:** A0  

**Wind Direction Input:**  
> **Role:** Simulate the wind direction  
**I/O type:** Input  
**Pin:** A1  

### Servos

**Turbine Servo:**  
> **Role:** Simulate turbine motor  
**I/O type:** Output  
**Pin:** 2  

**Blade Pitch Servo:**  
> **Role:** Simulate blade pitch controller (hardware)  
**I/O type:** Output  
**Pin:** 3  

### Buttons

> **Emergency  Stop Button:**  
> **Role:** Trigger emergency stop  
**I/O type:** Input  
**Pin:** 4  

**Data Logger:**  
> **Role:** Trigger daily data logger  
**I/O type:** Input  
**Pin:** 5  

## How It works

- Open the project [Tinker CAD](https://www.tinkercad.com/things/aTX5L7aUOhp-wind-turbine/editel)
- Click the **`Code`** button to reveal the code and the **`Serial Monitor`**
- Click on the `Serial Monitor` to reveal the console
- Start the simulation by clicking the **`Start Simulation`** button.
- The system will start and the serial monitor will prompt you to provide the following:
    - Cut-out speed
    - Cut-in speed
- Enter these numerical values
- Some data will be logged in the console just after completing the questions
- Twist the potentiometer knobs: the changes will be logged too.
- Press the right button (Data Logger) to see the logs.
- Press the left button (Emergency Stop) to stop the system.