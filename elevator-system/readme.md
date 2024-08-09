# The Elevator System

Instructions of how the system works:
- elevator up/down pulley control (2 LEDs, one for up and one for down)
- elevator can be called to each floor by a button on each floor
- elevator can be sent to each floor by a touchscreen in the elevator [Serial-input]
- sensors (buttons) on each floor indicate if elevator has arrived at that floor
- indicators in the elevator show the most recent elevator location (3 LEDs)
- weight in elevator measured by sensor, elevator will not move beyond [user-input]kgs
- ordering and prioritization of floor calls (always follow chronological order of calls)
- emergency stop & override buttons, to immediately stop and to restart elevator motion
- emergency personnel elevator override button (immediately forces elevator to main floor),
clears when pressed again
- logging (i.e. data storage) of path of travel, call times, arrival times, and the time of
emergency stop, override, and weight sensor faults for each day
- all logged values output serially at end of day