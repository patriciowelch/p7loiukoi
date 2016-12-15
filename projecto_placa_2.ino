#include <DHT11.h>
#include <Servo.h>
#include <NewPing.h>

#define PIN_ULTRA 41
#define salir 1
#define entrar 2

#define port_abierto 135
#define port_cerrado 22

Servo porton;
DHT11 sensor(14);
NewPing ultra(PIN_ULTRA,PIN_ULTRA,200);

//sensor de temperatura 14
//luz exterior 27

#define abierto 1
#define cerrado 2
#define AUTO_ADENTRO 13

#define PIN_L_SALA 40
#define PIN_L_BANO 38
#define PIN_L_CUARTO 37
#define PIN_L_GARAJE 39
#define PIN_L_EXT 27
#define PIN_VENTILADOR 53

int c = 9;
float temp,hum;
int err;
int autman = 1;
int pos;
int sub_est;

int est_port = cerrado;

int distancia;

int l_sala;
int l_bano;
int l_cuarto;
int l_garaje;
int l_externa;
int ventilador;

int autman_port = 0;

void setup()  
{
  Serial.begin(115200);
  pinMode(PIN_VENTILADOR,OUTPUT);
  pinMode(PIN_L_SALA,OUTPUT);
  pinMode(PIN_L_BANO,OUTPUT);
  pinMode(PIN_L_CUARTO,OUTPUT);
  pinMode(PIN_L_GARAJE,OUTPUT);
  pinMode(PIN_L_EXT,OUTPUT);
  pinMode(13,OUTPUT);
  Serial2.begin(9600);
  porton.attach(9);
  porton.write(port_cerrado);
  delay(1000);
  porton.detach();
}

void loop()
{ 
  if(Serial2.available() > 0)
  {
    c = Serial2.read();
    if(c == 0)
    {
      l_sala = !l_sala;
      digitalWrite(PIN_L_SALA,l_sala);
    }
    if(c == 1)
    {
      l_bano = !l_bano;
      digitalWrite(PIN_L_BANO,l_bano);
    }
    if(c == 2)
    {
      l_cuarto = !l_cuarto;
      digitalWrite(PIN_L_CUARTO,l_cuarto);
    }
    if(c == 3)
    {
      l_garaje = !l_garaje;
      digitalWrite(PIN_L_GARAJE,l_garaje);
    }
    if(c == 4)
    {
      l_externa = !l_externa;
      digitalWrite(PIN_L_EXT,l_externa);
    }
    if(c == 5)
    {
      autman = !autman;
      if(autman == 0)
      {
        digitalWrite(PIN_VENTILADOR, HIGH);
      }
    }
    if((c == 6) && (autman == 1))
    {
      ventilador = 0;
      digitalWrite(PIN_VENTILADOR,ventilador);
    }
    if((c == 7) && (autman == 1))
    {
      ventilador = 1;
      digitalWrite(PIN_VENTILADOR,ventilador);
    }
    if((c == 8) && (est_port == cerrado))
    {
      est_port = abierto;
      digitalWrite(PIN_L_GARAJE,1);
      porton.attach(9);
      for(pos = port_cerrado; pos < port_abierto; pos++)
      {
        porton.write(pos);
        delay(15);
      }
      delay(100);
    }
    if((c == 9) && (est_port == abierto) && (autman_port == 0))
    {
      est_port = cerrado;
      porton.attach(9);
      for(pos = port_abierto; pos >= port_cerrado; pos--)
      {
        porton.write(pos);
        delay(15);
      }
      delay(100);
      digitalWrite(PIN_L_GARAJE,0);
    }
    if(c == 10)
    {
      digitalWrite(PIN_L_SALA,0);
      digitalWrite(PIN_L_BANO,0);
      digitalWrite(PIN_L_CUARTO,0);
      digitalWrite(PIN_L_GARAJE,0);
      digitalWrite(PIN_L_EXT,1);
      digitalWrite(PIN_VENTILADOR,1);
      l_sala = 0;
      l_bano = 0;
      l_cuarto = 0;
      l_garaje = 0;
      l_externa = 1;

      if(est_port == abierto)
      {
        est_port = cerrado;
        porton.attach(9);
        for(pos = 140; pos >= 15; pos--)
        {
          porton.write(pos);
          delay(15);
        }
        delay(100);
        sub_est = 0;
      }
    }
    if(c == 11)
    {
      autman_port = !autman_port;
    }
    c = 12;
  }
  
  
  
  
  
  if(autman == 0)
  {
    while((temp == 0) && (hum == 0))
    {
      digitalWrite(13,1);
      sensor.read(hum,temp);
      delay(1000);
      digitalWrite(13,0);
    }
    if(temp > 28)
    {
      digitalWrite(PIN_VENTILADOR, LOW);
      delay(1000);
    }
    else if(temp <= 26)
    {
      digitalWrite(PIN_VENTILADOR, HIGH);
    }
    temp = 0;
    hum = 0;
  }
  
  
  
  
  
  
  
  
  if(autman_port == 1)
  {
    if(est_port == abierto)
    {
      distancia = ultra.ping_cm();
      delay(15);
      Serial.println(distancia);
      if(sub_est == 0)
      {
        if((distancia < AUTO_ADENTRO) && (distancia != 0))
        {
          sub_est = salir;
        }
        if((distancia > AUTO_ADENTRO) || (distancia == 0))
        {
          sub_est = entrar;
        }
      }
      if((distancia < 6) && (distancia != 0) && (sub_est == entrar))
      {
        delay(3500);
        est_port = cerrado;
        porton.attach(9);
        for(pos = port_abierto; pos >= port_cerrado; pos--)
        {
          porton.write(pos);
          delay(15);
        }
        delay(100);
        sub_est = 0;
        digitalWrite(PIN_L_GARAJE,0);
      }
      if(((distancia > 25) || (distancia == 0)) && (sub_est == salir))
      {
        delay(8000);
        est_port = cerrado;
        porton.attach(9);
        for(pos = port_abierto; pos >= port_cerrado; pos--)
        {
          porton.write(pos);
          delay(15);
        }
        delay(100);
        sub_est = 0;
        digitalWrite(PIN_L_GARAJE,0);
      }
    }
  }
  
  
  
}








