/* Variables */
int data = 0;
int count = 0;
int p_diode = A0;
int threshold = 700;
int freq = 500;
int delay_freq = (500/freq)*1000;

String recebido;

void setup() {
  /*Set the serial output */
  Serial.begin(9600);
}

void loop() {
  /* Read data from ADC */
  data = analogRead(p_diode);
 // Serial.println(data);

  /*Begin of the stream */
  if (data < threshold)
  {
  	/*Wait for all the 11 remaining bits */
    while (count < 11)
    {
      /* 5 miliseconds delay (100Hz)*/
      delayMicroseconds(delay_freq);
      data = analogRead(p_diode);
      if (data > threshold)
        recebido += '1';

      if (data < threshold)
        recebido += '0';

      count ++;
    }
    /* Print data */
    Serial.println(recebido);
    recebido = "";
    count = 0;
  }
}
