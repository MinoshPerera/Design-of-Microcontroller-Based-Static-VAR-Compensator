#include <LiquidCrystal_I2C.h> // I2C LCD library
#include <math.h> // For cosine calculation
#include "EmonLib.h" // Include Emon Library

// Define LCD address and dimensions
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Constants for phase angle calculation
const int zeroCrossPin = 2; // Pin for zero-crossing detector input
const float rad_to_deg = 57.2957951; // 1 radian = approx 57 deg.
const float max_angle_deg = 180;
const float frequency = 50; // Frequency in Hz
const float micro_to_sec = 1e-6; // Conversion from microseconds to seconds

// Relay control pins (ATmega328P Physical Pins 11, 12, 13, 14)
const int relayPins[4] = {5, 6, 7, 8}; // Digital Pins D5, D6, D7, D8

const double capacitorValue = 2.5; // Capacitance of each capacitor in μF (2.5μF)

double voltage_value = 0;
double current_value = 0;
double phase_angle_value = 0;
double power_factor_value = 0;
double active_power_value = 0;

float angle_max = 0;
float pf_max = 0;

EnergyMonitor emon1; // Create an instance for voltage and current sensing

bool stabilized = false; // Flag to indicate stabilization

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize voltage sensor with EmonLib
  emon1.voltage(A0, 200, 1.7); // Voltage: input pin, calibration, phase_shift
  
  // Initialize current sensor with EmonLib
  emon1.current(A1, 30); // Current: input pin, calibration factor (adjust as needed)
  
  // Initialize zero-crossing detector
  phase_angle_init();
  
  // Initialize relay pins
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // Set all relays to OFF initially
  }
  
  // Display initialization message
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  
  stabilizeReadings(); // Wait for measurements to stabilize
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power Factor System");
  delay(2000);
}

void loop() {
  if (!stabilized) {
    stabilizeReadings();
  } else {
    // Get voltage RMS using EmonLib
    emon1.calcVI(20, 2000);
    voltage_value = emon1.Vrms;
    
    // Get current RMS using EmonLib
    current_value = emon1.calcIrms(1480); // Calculate Irms based on 1480 sample points
    
    // Get phase angle
    phase_angle_value = get_phase_angle();
    
    // Calculate power factor and active power
    power_factor_value = get_power_factor();
    active_power_value = get_active_power();
    
    // Perform power factor correction by controlling the capacitor bank
    power_factor_correction();
    
    // Display the readings on the LCD
    display_readings();
    
    // Wait before taking new measurements
    delay(1000);
  }
}

// Function to stabilize initial readings
void stabilizeReadings() {
  const int stabilizationSamples = 50;
  double voltageSum = 0, currentSum = 0;
  
  for (int i = 0; i < stabilizationSamples; i++) {
    emon1.calcVI(20, 2000);
    voltageSum += emon1.Vrms;
    currentSum += emon1.calcIrms(1480);
    delay(50);
  }
  
  voltage_value = voltageSum / stabilizationSamples;
  current_value = currentSum / stabilizationSamples;
  stabilized = true;
}

// Initialize zero-crossing detector
void phase_angle_init() {
  pinMode(zeroCrossPin, INPUT);
}

// Phase angle measurement using pulseIn
double get_phase_angle() {
  int ctr;
  angle_max = 0;
  for (ctr = 0; ctr <= 4; ctr++) {
    float pulse_width = pulseIn(zeroCrossPin, HIGH) * micro_to_sec;
    float angle = pulse_width * frequency * max_angle_deg;
    
    if (angle > angle_max) {
      angle_max = angle;
      pf_max = cos(angle_max / rad_to_deg);
    }
  }
  
  if (angle_max > max_angle_deg) {
    angle_max = 0;
    pf_max = 1.0;
  }
  
  if (pf_max < 0) {
    pf_max = 0;
  }
  
  phase_angle_value = angle_max;
  return angle_max;
}

// Power factor calculation
double get_power_factor() {
  return pf_max;
}

// Active power calculation
double get_active_power() {
  return voltage_value * current_value * get_power_factor();
}

// Function to calculate the required capacitance for correction
double calculate_required_capacitance() {
  double target_pf = 0.95;
  double old_angle = acos(power_factor_value);
  double new_angle = acos(target_pf);
  
  double Qc = active_power_value * (tan(old_angle) - tan(new_angle));
  double required_capacitance = Qc / (pow(voltage_value, 2) * 2 * M_PI * frequency);
  
  return required_capacitance * 1e6; // Convert to μF
}

// Function to activate capacitors based on required capacitance
void power_factor_correction() {
  double requiredCapacitance = calculate_required_capacitance();
  int numRelaysToActivate = round(requiredCapacitance / capacitorValue);
  numRelaysToActivate = constrain(numRelaysToActivate, 0, 4);
  
  for (int i = 0; i < 4; i++) {
    if (i < numRelaysToActivate) {
      digitalWrite(relayPins[i], LOW); // Activate relay
    } else {
      digitalWrite(relayPins[i], HIGH); // Deactivate relay
    }
  }
}

// Display values on the 20x4 LCD
void display_readings() {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("V(RMS): ");
  lcd.print(voltage_value, 2);
  
  lcd.setCursor(0, 1);
  lcd.print("I(RMS): ");
  lcd.print(current_value, 2);
  
  lcd.setCursor(0, 2);
  lcd.print("Phase: ");
  lcd.print(phase_angle_value, 2);
  
  lcd.setCursor(0, 3);
  lcd.print("PF: ");
  lcd.print(power_factor_value, 2);
  
  lcd.setCursor(10, 3);
  lcd.print("P: ");
  lcd.print(active_power_value, 2);
}