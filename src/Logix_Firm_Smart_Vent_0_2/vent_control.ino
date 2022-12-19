void open_vent () {
    char outputline[20];
    
    Serial.println ("OPENING");
    digitalWrite(5, HIGH);
    digitalWrite(4, LOW);
    status_rgb.clear();
    status_rgb.setBrightness(50);
    status_rgb.setPixelColor(0, status_rgb.Color(0, 0, 255));
    status_rgb.show();
    delay (MOTOR_RUN_TIME*1000);
    status_rgb.clear();
    status_rgb.setBrightness(100);
    status_rgb.setPixelColor(0, status_rgb.Color(0, 255, 0));
    status_rgb.show();
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
 //    client.publish(reportTopic, outputline);
    Serial.println ("OPENED");
 
}

void close_vent () {
     char outputline[20];
    
   
    Serial.println ("CLOSING");
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    status_rgb.clear();
    status_rgb.setBrightness(50);
    status_rgb.setPixelColor(0, status_rgb.Color(0, 0, 255));
    status_rgb.show();
    delay (MOTOR_RUN_TIME*1000);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    status_rgb.clear();
    status_rgb.setBrightness(100);
    status_rgb.setPixelColor(0, status_rgb.Color(255, 0, 0));
    status_rgb.show();
 //    client.publish(reportTopic, outputline);
    Serial.println ("CLOSED");
 
}
