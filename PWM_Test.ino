// PINS
#define POT_IN 8 // A8
#define PWM_LED1 9 //A9 //Yellow
#define PWM_LED2 6 //A7 //Green
#define PWM_LED3 5  //Red
#define LED4 4
#define BUTTON_IN 2

//Motor
#define MOTOR_PWM 10 //Motor speed control

bool LED_State = false;
bool ButtonDown = false;

void setup() {
  // Setup Inputs
  pinMode(POT_IN, INPUT_PULLUP);
  pinMode(BUTTON_IN, INPUT_PULLUP);

  // Setup Output
  pinMode(PWM_LED1, OUTPUT);
  pinMode(PWM_LED2, OUTPUT);
  pinMode(PWM_LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);

  // Setup Serial
  Serial.begin(115200);
  Serial.println("Starting PWM Tester!");
}

void loop() {
 int readpot = analogRead(POT_IN);
 int Percentage = GetPercentAnalogSignal(readpot);
 int out_to_motor = GetPercentAnalog(Percentage);
 ApplyPercentageToLED(Percentage);
 analogWrite(MOTOR_PWM, out_to_motor);

 ReactToButton();
}

void ReactToButton(){
  
  // Read the button
  int ButtonPush = digitalRead(BUTTON_IN);
  if(ButtonPush == 0 && ButtonDown == false){
    Serial.println("Button Pushed!");
    ButtonDown = true;
    LED_State = !LED_State;
    digitalWrite(LED4, LED_State);
  }

  if(ButtonPush == 1 && ButtonDown == true){
    Serial.println("Button Up!");
    ButtonDown = false;
  }
}

int GetPercentAnalog(int amount){
  float percent_amount = (float(amount)/100) * 255.00;
  return int(percent_amount);
}

int GetPercentAnalogSignal(int amount){
  if(amount <= 17){return 0;} // 16 is the base reading on potentiometer
  float percent_amount = float(amount) / 1023;
  return int(percent_amount * 100);
}

void FadeUpAndDown(int pin){
   // put your main code here, to run repeatedly:
  for(int i=0; i<100; i++){
     analogWrite(pin, GetPercentAnalog(i));
     delay(10);
  }
  delay(1000);
  for(int i=100; i>0; i--){
     analogWrite(pin, GetPercentAnalog(i));
     delay(10);
  }
  delay(1000);

}

void ApplyPercentageToLED(int percent){
  int FullLED = 255 * 3;
  int PercentLED = float(percent / 100.00) * FullLED;
  int Applied[3] = {0,0,0};

  for(int i=0; i<3; i++){
    int taken = min(PercentLED, 255);
    PercentLED -= taken;
    Applied[i] = max(taken, 0);
  }
  
  analogWrite(PWM_LED1, Applied[0]);
  analogWrite(PWM_LED2, Applied[1]);
  analogWrite(PWM_LED3, Applied[2]);
  
}
