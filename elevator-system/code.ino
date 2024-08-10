#define FLOOR_1_BTN 7
#define FLOOR_2_BTN 8
#define FLOOR_3_BTN 9

#define EMERGENCY_STOP_BTN 10
#define OVERRIDE_BTN 11

#define FLOOR_1_LED 2
#define FLOOR_2_LED 3
#define FLOOR_3_LED 4

#define EMERGENCY_STOP_LED 12
#define OVERRIDE_LED 13

#define ELEVATOR_UP_LED 5
#define ELEVATOR_DOWN_LED 6

const int BUTTON_PINS[] = {FLOOR_1_BTN, FLOOR_2_BTN, FLOOR_3_BTN, EMERGENCY_STOP_BTN, OVERRIDE_BTN};
const int LED_PINS[] = {FLOOR_1_LED, FLOOR_2_LED, FLOOR_3_LED, EMERGENCY_STOP_LED, OVERRIDE_LED};

const int FLOOR_BTNs[] = {FLOOR_1_BTN, FLOOR_2_BTN, FLOOR_3_BTN};
const int FLOOR_LEDs[] = {FLOOR_1_LED, FLOOR_2_LED, FLOOR_3_LED};

float weight = 0.0;
int weighLimit = 200; // weight in Kg

int currentFloor = 1;         // Elevator starts at floor 1
int direction = 0;            // -1 for down, 1 for up, 0 for idle
int nextFloor[3] = {0, 0, 0}; // Array to hold requests for the 3 floors

bool overrideOn = false;
bool emergencyStopOn = false;

void setup()
{
    Serial.begin(9600); // Initialize serial communication at 9600 baud

    // Floor buttons
    pinMode(FLOOR_1_BTN, INPUT); // floor 1
    pinMode(FLOOR_2_BTN, INPUT); // floor 2
    pinMode(FLOOR_3_BTN, INPUT); // floor 3

    // Special buttons
    pinMode(EMERGENCY_STOP_BTN, INPUT); // Emergency stop
    pinMode(OVERRIDE_BTN, INPUT);       // Override

    // LEDs
    // Floor LEDs
    pinMode(FLOOR_1_LED, OUTPUT); // Floor 1
    pinMode(FLOOR_2_LED, OUTPUT); // Floor 2
    pinMode(FLOOR_3_LED, OUTPUT); // Floor 3

    // Special LEDs
    pinMode(EMERGENCY_STOP_LED, OUTPUT); // Emergency stop
    pinMode(OVERRIDE_LED, OUTPUT);       // Override

    // directional LEDs
    pinMode(ELEVATOR_UP_LED, OUTPUT);   // Elevator Up
    pinMode(ELEVATOR_DOWN_LED, OUTPUT); // Elevator Down
}

void loop()
{
    handleButtonPress();
    lightLEDs();

    if (direction != 0 && !emergencyStopOn)
    {
        moveElevator();
    }
}

// Function to add a floor request to the queue
void addNextFloor(int floorNumber)
{
    if (floorNumber < 1 || floorNumber > 3)
    {
        Serial.println("Invalid floor request.");
        return;
    }

    // Allow adding the request if it's in the current direction or if idle
    if (direction == 0 || (direction == 1 && floorNumber > currentFloor) || (direction == -1 && floorNumber < currentFloor))
    {
        nextFloor[floorNumber - 1] = 1; // Mark the requested floor in the array
        if (direction == 0)
        {
            direction = (floorNumber > currentFloor) ? 1 : -1;
        }

        Serial.println("Floor ");
        Serial.println(floorNumber);
        Serial.println(" request added");
    }
    else if (direction == 1 && floorNumber < currentFloor)
    {
        // If moving up, and a lower floor is requested, allow adding it
        nextFloor[floorNumber - 1] = 1;
        Serial.println("Floor");
        Serial.println(floorNumber);
        Serial.println(" request added.");
    }
    else if (direction == -1 && floorNumber > currentFloor)
    {
        // If moving down, and a higher floor is requested, allow adding it
        nextFloor[floorNumber - 1] = 1;
        Serial.println("Floor");
        Serial.println(floorNumber);
        Serial.println(" request added.");
    }
    else
    {
        Serial.println("Floor");
        Serial.println(floorNumber);
        Serial.println(" request ignored due to current direction.");
    }
}

// Function to move the elevator based on the current direction
void moveElevator()
{
    if (direction == 1)
    { // Moving up
        for (int i = currentFloor; i < 3; ++i)
        {
            if (nextFloor[i] == 1)
            {
                moveToFloor(i + 1);
                nextFloor[i] = 0; // Clear the request after it's fulfilled
            }
        }
    }
    else if (direction == -1)
    { // Moving down
        for (int i = currentFloor - 2; i >= 0; --i)
        {
            if (nextFloor[i] == 1)
            {
                moveToFloor(i + 1);
                nextFloor[i] = 0; // Clear the request after it's fulfilled
            }
        }
    }
    delay(1000);
    // After fulfilling all requests, set the elevator to idle
    direction = 0;
}

// Function to simulate moving the elevator to a specified floor
void moveToFloor(int floor)
{
    Serial.print("Moving from floor ");
    Serial.print(currentFloor);
    Serial.print(" to floor ");
    Serial.println(floor);

    currentFloor = floor;

    Serial.print("The elevator is now at floor");
    Serial.println(currentFloor);
}

void lightLEDs()
{

    // dim all the LEDs
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(LED_PINS[i], LOW);
    }
    digitalWrite(ELEVATOR_UP_LED, LOW);
    digitalWrite(ELEVATOR_DOWN_LED, LOW);

    // light current floor LED
    digitalWrite(LED_PINS[currentFloor - 1], HIGH);

    // light directional LED
    if (direction == 1)
    {
        digitalWrite(ELEVATOR_UP_LED, HIGH);
    }

    if (direction == -1)
    {
        digitalWrite(ELEVATOR_DOWN_LED, HIGH);
    }

    if (overrideOn)
    {
        digitalWrite(OVERRIDE_LED, HIGH);
    }

    if (emergencyStopOn)
    {
        digitalWrite(EMERGENCY_STOP_LED, HIGH);
    }
}

void handleButtonPress()
{
    for (int i = 0; i < 3; i++)
    {
        if (digitalRead(FLOOR_BTNs[i]) == 1)
        {
            addNextFloor(i + 1);
            overrideOn = false;
            emergencyStopOn = false;
        }
    }

    if (digitalRead(EMERGENCY_STOP_BTN) == 1)
    {
        emergencyStop();
        overrideOn = false;
    }

    if (digitalRead(OVERRIDE_BTN) == 1)
    {
        override();
        emergencyStopOn = false;
    }
}

void emergencyStop()
{
    emergencyStopOn = !emergencyStopOn;

    Serial.println("Emergency stop initiated");
}

void override()
{
    for (int i = 0; i < 3; i++)
    {
        nextFloor[i] = 0;
    };
    currentFloor = 1;
    direction = 0;
    overrideOn = true;
    Serial.println("Override sequence initiated");
}