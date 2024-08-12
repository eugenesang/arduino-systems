#include <Servo.h>

// Pin Definitions
const int windSpeedPin = A0;
const int windDirectionPin = A1;
const int turbineServoPin = 2;
const int bladePitchServoPin = 3;
const int emergencyStopButtonPin = 4;
const int endOfDayLogButtonPin = 5; // Button to trigger end-of-day data logging

// Servo Objects
Servo turbineServo;
Servo bladePitchServo;

// Wind Speed Parameters (to be set by user)
float cutInSpeed;
float cutOutSpeed;

// Variables to store the previous values for comparison
float previousWindSpeed = -1;
float previousWindDirection = -1;

// Universal angle variables
byte turbineAngle = 90;   // Default position (facing forward)
byte bladePitchAngle = 0; // Default pitch (flat)

// Logging Variables
const byte maxLogs = 60; // Reduced log size (1 log per minute for 1 hour)
struct DataLog
{
  float windSpeed;
  float windDirection;
  byte turbineAngle;
  byte bladePitchAngle;
};

DataLog logs[maxLogs]; // Array to store data logs
byte logIndex = 0;     // Current index in the log array

void setup()
{
  // Initialize the emergency stop and end-of-day logging button pins
  pinMode(emergencyStopButtonPin, INPUT_PULLUP);
  pinMode(endOfDayLogButtonPin, INPUT_PULLUP); // Initialize the log button pin

  // Attach the servos to their respective pins
  turbineServo.attach(turbineServoPin);
  bladePitchServo.attach(bladePitchServoPin);

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("System Initialized");

  // Get user input for cut-in and cut-out speeds
  Serial.println("Enter cut-in wind speed (m/s):");
  while (Serial.available() == 0)
  {
  }
  cutInSpeed = Serial.parseFloat();
  Serial.print(cutInSpeed);
  Serial.println("m/s");

  Serial.println("Enter cut-out wind speed (m/s):");
  while (Serial.available() == 0)
  {
  }
  cutOutSpeed = Serial.parseFloat();
  Serial.print(cutOutSpeed);
  Serial.println("m/s");

  Serial.print("Cut-in Speed: ");
  Serial.print(cutInSpeed);
  Serial.println(" m/s");
  Serial.print("Cut-out Speed: ");
  Serial.print(cutOutSpeed);
  Serial.println(" m/s");
  Serial.println("System Ready");
}

void loop()
{
  // Check if the emergency stop button is pressed
  if (digitalRead(emergencyStopButtonPin) == LOW)
  {
    handleEmergencyStop();
  }

  // Check if the end-of-day log button is pressed
  if (digitalRead(endOfDayLogButtonPin) == LOW)
  {
    logEndOfDayData();
    delay(200); // Debounce delay to prevent multiple logs
  }

  // Read wind speed and direction
  float windSpeed = readWindSpeed();
  float windDirection = readWindDirection();

  // Control the turbine and blade pitch based on wind speed
  controlTurbineAndBlades(windSpeed, windDirection);

  // Log data if there is a change in wind speed or direction
  if (hasDataChanged(windSpeed, windDirection))
  {
    logData(windSpeed, windDirection);
    previousWindSpeed = windSpeed;
    previousWindDirection = windDirection;
  }

  // Small delay to reduce CPU usage
  delay(100);
}

// Function to handle emergency stop
void handleEmergencyStop()
{
  Serial.println("Emergency Stop Activated!");
  turbineServo.write(90);   // Stop turbine rotation
  bladePitchServo.write(0); // Set blades to 0°

  while (digitalRead(emergencyStopButtonPin) == LOW)
  {
    // Wait until the button is released
  }
  Serial.println("Emergency Stop Released. System Reset.");
}

// Function to read wind speed from sensor
float readWindSpeed()
{
  int rawValue = analogRead(windSpeedPin);
  return rawValue * (5.0 / 1023.0) * 20.0; // Convert sensor value to wind speed
}

// Function to read wind direction from sensor
float readWindDirection()
{
  int rawValue = analogRead(windDirectionPin);
  return rawValue * (360.0 / 1023.0); // Convert sensor value to wind direction
}

// Function to control turbine and blade pitch based on wind speed and direction
void controlTurbineAndBlades(float windSpeed, float windDirection)
{
  if (windSpeed >= cutInSpeed && windSpeed <= cutOutSpeed)
  {
    // Rotate turbine to align with wind direction
    turbineAngle = constrain((int)windDirection, 0, 180);
    turbineServo.write(turbineAngle);

    // Adjust blade pitch based on wind speed
    if (windSpeed >= cutOutSpeed - 5.0)
    {
      // Linearly reduce blade pitch from 45° to 0° as wind speed approaches cut-out speed
      bladePitchAngle = map(windSpeed, cutOutSpeed - 5.0, cutOutSpeed, 45, 0);
    }
    else
    {
      // Set blade pitch to maximum (45 deg) for lower wind speeds
      bladePitchAngle = 45.0;
    }
    bladePitchAngle = constrain(bladePitchAngle, 0, 45);
    bladePitchServo.write(bladePitchAngle);
  }
  else
  {
    // Force blades to 0° and turbine to a fixed position
    bladePitchServo.write(0);
    turbineServo.write(90); // Fixed position if out of operational range
    turbineAngle = 90;
    bladePitchAngle = 0;
  }
}

// Function to log data if wind speed or direction changes
void logData(float windSpeed, float windDirection)
{
  Serial.println("Data Log:");
  Serial.print("Wind Speed: ");
  Serial.print(windSpeed);
  Serial.print(" m/s, Wind Direction: ");
  Serial.print(windDirection);
  Serial.print("\xB0, Turbine Angle: ");
  Serial.print(turbineAngle);
  Serial.print("\xB0, Blade Pitch Angle: ");
  Serial.print(bladePitchAngle);
  Serial.println("\xB0");

  // Log data into the array
  if (logIndex < maxLogs)
  {
    logs[logIndex].windSpeed = windSpeed;
    logs[logIndex].windDirection = windDirection;
    logs[logIndex].turbineAngle = turbineAngle;
    logs[logIndex].bladePitchAngle = bladePitchAngle;
    logIndex++;
  }
  else
  {
    Serial.println("Log storage full!");
  }
}

// Function to check if wind speed or direction has changed
bool hasDataChanged(float currentWindSpeed, float currentWindDirection)
{
  return (currentWindSpeed != previousWindSpeed) || (currentWindDirection != previousWindDirection);
}

// Function to perform end-of-day data logging when the button is pressed
void logEndOfDayData()
{
  Serial.println("End of Day Data Logging Triggered:");
  Serial.println("Log \t Wind Speed \t Wind Direction  Turbine Angle  Blade pitch angle");

  for (int i = 0; i < logIndex; i++)
  {
    Serial.print(i + 1);
    Serial.print(" \t ");
    Serial.print(logs[i].windSpeed);
    Serial.print(" m/s \t ");
    Serial.print(logs[i].windDirection);
    Serial.print("\xB0 \t ");
    Serial.print(logs[i].turbineAngle);
    Serial.print("\xB0 \t\t ");
    Serial.print(logs[i].bladePitchAngle);
    Serial.println("\xB0");
  }

  Serial.println("End of Day Data Log Complete.");
}