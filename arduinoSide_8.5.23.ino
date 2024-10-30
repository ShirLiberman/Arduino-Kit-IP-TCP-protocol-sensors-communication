char data[30];
char rdata[30];
int bytesRead;
int state =1;
int led_state;
int LDR_val;
int error, prev_error;
int target;
int L2_input;
int sampling_time;
int duty;
int duty_2;
unsigned long int now_time, start_time,prev_time=0;
//double duty[10];
//double sensor[10];
int i=0;
int delta_error;
int flag_error;
long int error_sum, sum_max;
long int P,I,D,I_MAX = 180;
int Kp, Ki, Kd;
long int pid;
double dt = 0.001;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  Serial.println("0,Arduino started");

}

void loop() {
  // put your main code here, to run repeatedly:
  start_time = millis ();
  int duty=0;
  switch (state) 
  {
      case 0: // LED control
        analogWrite(9, LOW);
        analogWrite(10, LOW);
        Serial.println("0,> Going to idle state 1");
        state = 1;
        break;

      case 1:
        if (Serial.available())
        {
          bytesRead = Serial.readBytesUntil('\0', rdata, 29);
          rdata[bytesRead] = '\0';
          sscanf (rdata, "%d", &state);
          Serial.print("0,>> ");
          Serial.println(rdata);
        }
        break;

      case 2: // LED control
        Serial.println("0,> State 2 is running");
        analogWrite(9, 0);
        analogWrite(10, 0);
        delay(2000);
        LDR_val = analogRead(0);
        Serial.print("0,> L1:0 L2:0:         ");
        Serial.println(LDR_val);

        analogWrite(9, 255);
        analogWrite(10, 0);
        delay(2000);
        LDR_val = analogRead(0);
        Serial.print("0,> L1:255 L2:0:     ");
        Serial.println(LDR_val);

        analogWrite(9, 0);
        analogWrite(10, 255);
        delay(2000);
        LDR_val = analogRead(0);
        Serial.print("0,> L1:0 L2:255:     ");
        Serial.println(LDR_val);

        analogWrite(9, 255);
        analogWrite(10, 255);
        delay(2000);
        LDR_val = analogRead(0);
        Serial.print("0,> L1:255 L2:255: ");
        Serial.println(LDR_val);
        Serial.println("0,> Returning to state 0");
        state = 0;
        break;
      
      case 3:
        sscanf(rdata + 2, "%d", &L2_input);
        analogWrite(10, L2_input);
        now_time = millis();
        while(now_time - start_time <= 5000)
        {
          if(now_time - prev_time  == 10)
          {
            LDR_val = analogRead(0);
            sprintf(data, "1,%lu,%d",now_time-start_time ,LDR_val);
            Serial.println(data);
            prev_time = now_time;
          }
          now_time = millis();
          
        }
        Serial.println("*");
        Serial.println("Returning to state 0");
        state = 0;
        break;

        case 4:
        Serial.println("get into state 4");
        for(int i = 50;i<=225; i +=25)
        {
          analogWrite(10,0);
          delay(30000);
          analogWrite(10,i);
          delay(5000);
          LDR_val = analogRead(0);
          sprintf(data,"2,%d,%d",LDR_val,i);
          Serial.println(data);
        }
        state = 0;
        break;

        case 5:
        sscanf(rdata + 2, "%d,%d,%d,%d", &target,&sampling_time,&duty,&duty_2);
        now_time = millis();
        while(now_time - start_time <= 5000)
        {
          if(now_time - start_time >= 2500){
            analogWrite(9,duty_2);
          }
          if (now_time - prev_time >= sampling_time )
          {
           
              LDR_val = analogRead(0);
              error = target -  LDR_val;
              sprintf(data, "1,%lu,%d",now_time-start_time ,LDR_val);
              Serial.println(data);
              //analogWrite(9,duty_2);
              if (error > 0)
              {
                analogWrite(10,duty);
              }
               
              else
                analogWrite(10, 0);
              
              prev_time = now_time;

      
          }
          now_time = millis();
          
        }
        Serial.println("*");
        Serial.println("Returning to state 0");
        state = 0;
        break;

        case 7:  /// algorythm
        sscanf(rdata + 2, "%d,%d,%d", &target,&sampling_time,&duty);
        now_time = millis();
        flag_error=0;
        while(now_time - start_time <= 5000)
        {
          if (now_time - prev_time >= sampling_time )
          {
           
              LDR_val = analogRead(0);
              error = target -  LDR_val;
              sprintf(data, "1,%lu,%d",now_time-start_time ,LDR_val);
              Serial.println(data);
              if(LDR_val >= target)
              {
                flag_error = 1 ;
              }
              
              if(flag_error == 0)
              {
                analogWrite(10,duty);
              }
              if(flag_error)
              {
                  if (error > 0)
                {
                  delta_error = (error / 2);
                  analogWrite(10,duty/delta_error);
                }
                
                  else
                {
                  analogWrite(10, 0);
                }
                  
              }
              
              
              prev_time = now_time;

      
          }
          now_time = millis();
          
        }
        Serial.println("*");
        Serial.println("Returning to state 0");
        state = 0;
        break;





        case 6:
        sscanf(rdata + 2, "%d,%d,%d,%d", &target,&Kp,&Ki,&Kd);
        now_time = millis();
        sum_max = (I_MAX / (Ki * dt));
        while(now_time - start_time <= 5000)
        {
         
          //if(now_time - start_time >= 2500){
          //  analogWrite(9,duty_2);
          //}
          if (now_time - prev_time >= 1 )
          {
           
              LDR_val = analogRead(0);
              error = target -  LDR_val;
              error_sum += error;

              if(error_sum > sum_max) error_sum = sum_max;

              D = (long int) Kd * (error - prev_error);
              P =(long int) Kp * error;
              I = (long int) Ki*error_sum*dt;

              pid = P + I + D;

              sprintf(data, "1,%lu,%d",now_time-start_time ,LDR_val);
              Serial.println(data);
              prev_error = error;
              if(now_time - start_time >= 2500) analogWrite(9,150);
              
              
              if (pid >= 255 ) pid=255;
              if (pid < 0 ) pid=0;
              analogWrite(10,pid);
        
               
             
              
              prev_time = now_time;

      
          }
          now_time = millis();
          
        }
        Serial.println("*");
        Serial.println("Returning to state 0");
        state = 0;
        break;


  }
    
       
}