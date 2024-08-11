#include <Servo.h>

// Pin Definitions
const int windSpeedPin = A0;
const int windDirectionPin = A1;
const int turbineServoPin = 2;
const int bladePitchServoPin = 3;
const int emergencyStopButtonPin = 4;

// Servo Objects
Servo turbineServo;
Servo bladePitchServo;

// Wind Speed Parameters
const float cutInSpeed = 5.0;   // Minimum operational wind speed in m/s
const float cutOutSpeed = 25.0; // Maximum operational wind speed in m/s

// Logging Intervals
const unsigned long logInterval = 60000;        // Logging interval in milliseconds (1 minute)
const unsigned long dataLogInterval = 86400000; // Daily data logging interval (24 hours in milliseconds)

// Timing Variables
unsigned long lastLogTime = 0;
unsigned long lastDataLogTime = 0;

void setup()
{
  // Initialize the emergency stop button pin
  pinMode(emergencyStopButtonPin, INPUT_PULLUP);

  // Attach the servos to their respective pins
  turbineServo.attach(turbineServoPin);
  bladePitchServo.attach(bladePitchServoPin);

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("System Initialized");
}

void loop()
{
  // Check if the emergency stop button is pressed
  if (digitalRead(emergencyStopButtonPin) == LOW)
  {
    handleEmergencyStop();
  }

  // Read wind speed and direction
  float windSpeed = readWindSpeed();
  float windDirection = readWindDirection();

  // Control the turbine and blade pitch based on wind speed
  controlTurbineAndBlades(windSpeed, windDirection);

  // Log data at regular intervals
  logData();

  // Perform end-of-day logging
  logEndOfDayData(windSpeed, windDirection);

  // Small delay to reduce CPU usage
  delay(100);
}

// Function to handle emergency stop
void handleEmergencyStop()
{
  Serial.println("Emergency Stop Activated!");
  turbineServo.write(90);   // Stop turbine rotation
  bladePitchServo.write(0); // Set blades to 0 degrees

  while (true)
  {
  } // Halt the program indefinitely
}

// Function to read wind speed from sensor
float readWindSpeed()
{
  int rawValue = analogRead(windSpeedPin);
  float windSpeed = rawValue * (5.0 / 1023.0) * 20.0; // Convert sensor value to wind speed
  Serial.print("Raw Wind Speed Sensor Value: ");
  Serial.println(rawValue);
  return windSpeed;
}

// Function to read wind direction from sensor
float readWindDirection()
{
  int rawValue = analogRead(windDirectionPin);
  float windDirection = rawValue * (360.0 / 1023.0); // Convert sensor value to wind direction
  Serial.print("Raw Wind Direction Sensor Value: ");
  Serial.println(rawValue);
  return windDirection;
}

// Function to control turbine and blade pitch based on wind speed and direction
void controlTurbineAndBlades(float windSpeed, float windDirection)
{
  if (windSpeed >= cutInSpeed && windSpeed <= cutOutSpeed)
  {
    // Rotate turbine to align with wind direction
    int turbineAngle = constrain((int)windDirection, 0, 180);
    turbineServo.write(turbineAngle);

    // Adjust blade pitch based on wind speed
    float bladePitchAngle = map(windSpeed, cutInSpeed, cutOutSpeed, 45, 0);
    bladePitchAngle = constrain(bladePitchAngle, 0, 45);
    bladePitchServo.write(bladePitchAngle);

    Serial.print("Wind Speed: ");
    Serial.print(windSpeed);
    Serial.print(" m/s, Wind Direction: ");
    Serial.print(windDirection);
    Serial.print(" degrees, Blade Pitch: ");
    Serial.print(bladePitchAngle);
    Serial.println(" degrees");
  }
  else
  {
    // Force blades to 0 degrees and turbine to a fixed position
    bladePitchServo.write(0);
    turbineServo.write(90); // Fixed position if out of operational range
  }
}

// Function to log data at regular intervals
void logData()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastLogTime >= logInterval)
  {
    Serial.print("Logging data at ");
    Serial.print(currentMillis / 1000);
    Serial.println(" seconds");

    lastLogTime = currentMillis;
  }
}

// Function to perform end-of-day data logging
void logEndOfDayData(float windSpeed, float windDirection)
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastDataLogTime >= dataLogInterval)
  {
    Serial.println("End of Day Data:");
    Serial.print("Wind Speed: ");
    Serial.print(windSpeed);
    Serial.print(" m/s, Wind Direction: ");
    Serial.print(windDirection);
    Serial.println(" degrees");
    lastDataLogTime = currentMillis;
  }
}
