
//ο χρόνος που θέλει ο sensor για να συνεργαστεί, δλδ μετα από 10 δευτεόλεπτα που αρχιζει η προσομοίωση άμα ανιχνεύσει κίνηση ανάβει το led (10-60 secs according to the datasheet) 
int calibrationTime = 20; 

//οχρόνος που το sensor outputs ανιχνευσει low impulse 
long unsigned int lowIn;                    //Unsigned long variables are extended size variables for number storage,
                                            //and store 32 bits (4 bytes). Unlike standard longs unsigned longs won’t store negative numbers,
                                            //making their range from 0 to 4,294,967,295 (2^32 - 1).

//τα milliseconds του sensor οπου αυτός πρέπει να είναι σε κατάσταση LOW πριν θεωρήσουμε οτι έχει σταματήσει κάθε κίνηση.
long unsigned int pause = 5000; 
boolean lockLow = true; 
boolean takeLowTime; 
int pirPin = 3; // digital pin συνδεδεμένο στο PIR sensor's output 
int ledPin = 13; 

void setup()
{ 
  Serial.begin(9600);
  pinMode(pirPin, INPUT); 
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);                //δίνουμε κάποιο χρόνο στο sensor να ενεργοποιηθεί, άρα αρχικά τον έχουμε απενεγοποιημένο, όσο είναι και το calibrationTime
  Serial.print("calibrating sensor ");      //Αυτό εμφανίζεται στο serial monitor
  for(int i = 0; i < calibrationTime; i++)
  { 
     Serial.print(".");                    //και ακολουθούν τελείες κάθε 1000 miliseconds
     delay(1000); 
  } 
  Serial.println(" done");                 //και στο τέλος όταν περάσουν 20 δευτερα έχουμε την μορφή 'calibrating sensor.........done'
  Serial.println("SENSOR ACTIVE");         //                                                        'SENSOR ACTIVE'
  delay(50); 
} 

void loop()
{ 
  if(digitalRead(pirPin) == HIGH)
  {
      digitalWrite(ledPin, HIGH);         //το led βλέπει την κατάσταση του pin output του sensor(HIGH) 
      if(lockLow)                         //βεβαιώνει ότι περιμένουμε για μετάβαση στη LOW κατάσταση πριν γίνει οποιοδήποτε άλλο output 
      { 
            lockLow = false; 
            Serial.println("---"); 
            Serial.print("motion detected at ");
            Serial.print(millis()/1000); 
            Serial.println(" sec"); delay(50);
      } 
      takeLowTime = true; 
  } 
  if(digitalRead(pirPin) == LOW)
  { 
      digitalWrite(ledPin, LOW);        //το led βλέπει την κατάσταση του pin output του sensor(LOW)
      if(takeLowTime)
       { 
         lowIn = millis();              //αποθηκεύει το χρόνο μεταφοράς από την κατάσταση HIGH στη LOW
         takeLowTime = false;           //βεβαιώνει αυτή η γραμή κώδικα τι αυτό γίνεται στην αρχή της LOW κατάστασης
       }                                //αν ο sensor είναι σε κατάσταση LOW 
                                        //για παραπάνω απο το χρονο pause που έχει δωθεί τότε υποθέτουμε ότι 
                                        //δεν πρόκειτε να γίνει κάποια άλλη κίνηση
      if(!lockLow && millis() - lowIn > pause)   // βεβαιώνει οτι αυτο το loop κωδικα εκτελείτε ξανά μόνο όταν ανιχνευτεί μια καινούργια κινηση
      {                                 
         lockLow = true; 
         Serial.print("motion ended at "); //output 
         Serial.print((millis() - pause)/1000); 
         Serial.println(" sec"); delay(50); 
      } 
  } 
}
