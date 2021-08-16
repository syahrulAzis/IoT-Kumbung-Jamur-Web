#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//Seting Jaringan.
const char* ssid = "Hayo";
const char* password = "1234567890";
const char* host = "192.168.43.21"; //Alamat IP server

byte termometru[8] = //icon for termometer
{
 B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110
};

byte picatura[8] = //icon for water droplet
{
 B00100, B00100, B01010, B01010, B10001, B10001, B10001, B01110
};
byte dia[8] = { //icon for sun
  0b00000, 0b00100, 0b10101, 0b01110, 0b11111, 0b01110, 0b10101, 0b00100
};
byte co2[8] = //icon for water droplet
{
 B10101, B01110, B01110, B01110, B11111, B11111, B11111, B01110
};

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define PIN_LED 12
#define DHTPIN 14     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
//char ulangi = 'delay(1000)';
float counter;
const int MistMaker= 12;
const int MistLampu= 13;
const int kipas= 16;


float kelembapan, suhu, pinLDR;  //Stores humidity value

String keadaan,keadaanLcd ;
float suhu_rendah, suhu_cukup, suhu_tinggi;
float kelembapan_kering, kelembapan_cukup, kelembapan_basah;
float cahaya_redup, cahaya_cukup, cahaya_terang;
float support;
float confidence;
float lift_ratio;

void setup() {
  Serial.begin(115200); 
  pinMode(PIN_LED, OUTPUT);
  pinMode(MistMaker, OUTPUT);
  pinMode(MistLampu, OUTPUT);
  pinMode(kipas, OUTPUT);
  pinMode(pinLDR, INPUT);
  lcd.begin();
  // Print a message to the LCD.
  lcd.print("Skripsi Syahrul A");
  lcd.setCursor(0,1);
  lcd.print("..In Development");
  delay(5000);
  lcd.clear();
  dht.begin();
  
  //Setting koneksi Wifi 
  WiFi.hostname("NodeMCU");
  
  //Mulai Start Koneksi Wifi dengan memasukan sssid dan password
  WiFi.begin(ssid, password);
  
  //Cek Koneksi Wifi
  //Selama wifi status konek wifinya tidak terkoneksi  
  while(WiFi.status() != WL_CONNECTED)
  {
  //led Mati
  digitalWrite(PIN_LED,LOW);
  //NodeMcu terus mencoba koneksi 
  Serial.print(".");
  delay(500);
  }
  //Apabila berhasil terkoneksi
  digitalWrite(PIN_LED, HIGH);
  //Tampilkan pesan di serial Wifi connected
  Serial.println("Wifi Connected");
  }
float cahaya = 0;
void fuzzy(){
    if(suhu>=1 && suhu<=29.99){
            suhu_rendah++;
        }else if(suhu>=30 && suhu<36){
            suhu_cukup++;
        }else if(suhu>=36 && suhu<=45){
            suhu_tinggi++;
        }
        if(kelembapan>=1 && kelembapan<=79.99){
            kelembapan_kering++;
        }else if(kelembapan>=80 && kelembapan<91){
            kelembapan_cukup++;
        }else if(kelembapan>=91 && kelembapan<=110){
            kelembapan_basah++;
        }
        if(cahaya>=1 && cahaya<=99.99){
            cahaya_redup++;
        }else if(cahaya>=100 && cahaya<300){
            cahaya_cukup++;
        }else if(cahaya>=301 && cahaya<=500){
            cahaya_terang++;
        }
}

void ARM(){
    if(suhu_cukup&&kelembapan_cukup&&cahaya_cukup){
        keadaan = "1";
        keadaanLcd = "Kumbung Baik";
        support = (suhu_cukup,kelembapan_cukup,cahaya_cukup)/counter;
        confidence = (suhu_cukup,kelembapan_cukup,cahaya_cukup)/suhu_cukup;
        lift_ratio = (suhu_cukup,kelembapan_cukup,cahaya_cukup)/counter/support;
    }else if(suhu_rendah&&kelembapan_kering&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung_Tdk_Baik";
        support = (suhu_rendah,kelembapan_kering,cahaya_redup)/counter;
        confidence = (suhu_rendah,kelembapan_kering,cahaya_redup)/suhu_rendah;
        lift_ratio = (suhu_rendah,kelembapan_kering,cahaya_redup)/counter/support;
    }else if(suhu_cukup&&kelembapan_kering&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung_Tdk_Baik";
        support = (suhu_cukup,kelembapan_kering,cahaya_redup)/counter;
        confidence = (suhu_cukup,kelembapan_kering,cahaya_redup)/suhu_cukup;
        lift_ratio = (suhu_cukup,kelembapan_kering,cahaya_redup)/counter/support;
    }else if(suhu_rendah&&kelembapan_cukup&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung_Tdk_Baik";
        support = (suhu_rendah,kelembapan_cukup,cahaya_redup)/counter;
        confidence = (suhu_rendah,kelembapan_cukup,cahaya_redup)/suhu_rendah;
        lift_ratio = (suhu_rendah,kelembapan_cukup,cahaya_redup)/counter/support;
    }else if(suhu_rendah&&kelembapan_kering&&cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung_Tdk_Baik";
        support = (suhu_rendah,kelembapan_kering,cahaya_cukup)/counter;
        confidence = (suhu_rendah,kelembapan_kering,cahaya_cukup)/suhu_rendah;
        lift_ratio = (suhu_rendah,kelembapan_kering,cahaya_cukup)/counter/support;
    }else if(suhu_rendah&&kelembapan_cukup&&cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung_Tdk_Baik";
        support = (suhu_rendah,kelembapan_cukup,cahaya_cukup)/counter;
        confidence = (suhu_rendah,kelembapan_cukup,cahaya_cukup)/suhu_rendah;
        lift_ratio = (suhu_rendah,kelembapan_cukup,cahaya_cukup)/counter/support;
    }else if(suhu_cukup&&kelembapan_kering&&cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung_Tdk_Baik";
        support = (suhu_cukup,kelembapan_kering,cahaya_cukup)/counter;
        confidence = (suhu_cukup,kelembapan_kering,cahaya_cukup)/suhu_cukup;
        lift_ratio = (suhu_cukup,kelembapan_kering,cahaya_cukup)/counter/support;
    }else if(suhu_cukup&&kelembapan_cukup&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung_Tdk_Baik";
        support = (suhu_cukup,kelembapan_cukup,cahaya_redup)/counter;
        confidence = (suhu_cukup,kelembapan_cukup,cahaya_redup)/suhu_cukup;
        lift_ratio = (suhu_cukup,kelembapan_cukup,cahaya_redup)/counter/support;
        //2 Kondisi
    }else if(suhu_tinggi&&kelembapan_basah){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_tinggi,kelembapan_basah)/counter;
        confidence = (suhu_tinggi,kelembapan_basah)/suhu_tinggi;
        lift_ratio = (suhu_tinggi,kelembapan_basah)/counter/support;
    }else if(suhu_tinggi&&kelembapan_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_tinggi,kelembapan_cukup)/counter;
        confidence = (suhu_tinggi,kelembapan_cukup)/suhu_tinggi;
        lift_ratio = (suhu_tinggi,kelembapan_cukup)/counter/support;
    }else if(suhu_tinggi&&kelembapan_kering){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_tinggi,kelembapan_kering)/counter;
        confidence = (suhu_tinggi,kelembapan_kering)/suhu_tinggi;
        lift_ratio = (suhu_tinggi,kelembapan_kering)/counter/support;
    }else if(suhu_cukup&&kelembapan_basah){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_cukup,kelembapan_basah)/counter;
        confidence = (suhu_cukup,kelembapan_basah)/suhu_cukup;
        lift_ratio = (suhu_cukup,kelembapan_basah)/counter/support;
    }else if(suhu_cukup&&kelembapan_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_cukup,kelembapan_cukup)/counter;
        confidence = (suhu_cukup,kelembapan_cukup)/suhu_cukup;
        lift_ratio = (suhu_cukup,kelembapan_cukup)/counter/support;
    }else if(suhu_cukup&&kelembapan_kering){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_cukup,kelembapan_kering)/counter;
        confidence = (suhu_cukup,kelembapan_kering)/suhu_cukup;
        lift_ratio = (suhu_cukup,kelembapan_kering)/counter/support;
    }else if(suhu_rendah&&kelembapan_basah){
        keadaan = "Kumbung Tidak Baik";
        support = (suhu_rendah,kelembapan_basah)/counter;
        confidence = (suhu_rendah,kelembapan_basah)/suhu_rendah;
        lift_ratio = (suhu_rendah,kelembapan_basah)/counter/support;
    }else if (suhu_rendah&&kelembapan_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_rendah,kelembapan_cukup)/counter;
        confidence = (suhu_rendah,kelembapan_cukup)/suhu_rendah;
        lift_ratio = (suhu_rendah,kelembapan_cukup)/counter/support;
    }else if (suhu_rendah&&kelembapan_kering){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_rendah,kelembapan_kering)/counter;
        confidence = (suhu_rendah,kelembapan_kering)/suhu_rendah;
        lift_ratio = (suhu_rendah,kelembapan_kering)/counter/support;
    }else if (suhu_tinggi&&cahaya_terang){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_tinggi,cahaya_terang)/counter;
        confidence = (suhu_tinggi,cahaya_terang)/suhu_tinggi;
        lift_ratio = (suhu_tinggi,cahaya_terang)/counter/support;
    }else if(suhu_tinggi && cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_tinggi,cahaya_cukup)/counter;
        confidence = (suhu_tinggi,cahaya_cukup)/suhu_tinggi;
        lift_ratio = (suhu_tinggi,cahaya_cukup)/counter/support;
    }else if(suhu_tinggi&& cahaya_redup){
        keadaan = "Kumbung Tidak Baik";
        support = (suhu_tinggi,cahaya_redup)/counter;
        confidence = (suhu_tinggi,cahaya_redup)/suhu_tinggi;
        lift_ratio = (suhu_tinggi,cahaya_redup)/counter/support;
    }else if(suhu_cukup&&cahaya_terang){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_cukup,cahaya_terang)/counter;
        confidence = (suhu_cukup,cahaya_terang)/suhu_cukup;
        lift_ratio = (suhu_cukup,cahaya_terang)/counter/support;
    }else if(suhu_cukup&&cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_cukup,cahaya_cukup)/counter;
        confidence = (suhu_cukup,cahaya_cukup)/suhu_cukup;
        lift_ratio = (suhu_cukup,cahaya_cukup)/counter/support;
    }else if(suhu_cukup && cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_cukup,cahaya_redup)/counter;
        confidence = (suhu_cukup,cahaya_redup)/suhu_cukup;
        lift_ratio = (suhu_cukup,cahaya_redup)/counter/support;
    }else if(suhu_cukup&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_cukup,cahaya_redup)/counter;
        confidence = (suhu_cukup,cahaya_redup)/suhu_cukup;
        lift_ratio = (suhu_cukup,cahaya_redup)/counter/support;
    }else if(suhu_rendah&&cahaya_terang){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_rendah,cahaya_terang)/counter;
        confidence = (suhu_rendah,cahaya_terang)/suhu_rendah;
        lift_ratio = (suhu_rendah,cahaya_terang)/counter/support;
    }else if(suhu_rendah&&cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_rendah,cahaya_cukup)/counter;
        confidence = (suhu_rendah,cahaya_cukup)/suhu_rendah;
        lift_ratio = (suhu_rendah,cahaya_cukup)/counter/support;
    }else if(suhu_rendah&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (suhu_rendah,cahaya_redup)/counter;
        confidence = (suhu_rendah,cahaya_redup)/suhu_rendah;
        lift_ratio = (suhu_rendah,cahaya_redup)/counter/support;
    }else if(kelembapan_basah&&cahaya_terang){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_basah,cahaya_terang)/counter;
        confidence = (kelembapan_basah,cahaya_terang)/kelembapan_basah;
        lift_ratio = (kelembapan_basah,cahaya_terang)/counter/support;
    }else if(kelembapan_basah&&cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_basah,cahaya_cukup)/counter;
        confidence = (kelembapan_basah,cahaya_cukup)/kelembapan_basah;
        lift_ratio = (kelembapan_basah,cahaya_cukup)/counter/support;
    }else if(kelembapan_basah&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_basah,cahaya_redup)/counter;
        confidence = (kelembapan_basah,cahaya_redup)/kelembapan_basah;
        lift_ratio = (kelembapan_basah,cahaya_redup)/counter/support;
    }else if(kelembapan_cukup&&cahaya_terang){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_cukup,cahaya_terang)/counter;
        confidence = (kelembapan_cukup,cahaya_terang)/kelembapan_cukup;
        lift_ratio = (kelembapan_cukup,cahaya_terang)/counter/support;
    }else if(kelembapan_cukup&&cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_cukup,cahaya_cukup)/counter;
        confidence = (kelembapan_cukup,cahaya_cukup)/kelembapan_cukup;
        lift_ratio = (kelembapan_cukup,cahaya_cukup)/counter/support;
    }else if(kelembapan_cukup&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_cukup,cahaya_redup)/counter;
        confidence = (kelembapan_cukup,cahaya_redup)/kelembapan_cukup;
        lift_ratio = (kelembapan_cukup,cahaya_redup)/counter/support;
    }else if(kelembapan_kering&&cahaya_terang){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_kering,cahaya_terang)/counter;
        confidence = (kelembapan_kering,cahaya_terang)/kelembapan_kering;
        lift_ratio = (kelembapan_kering,cahaya_terang)/counter/support;
    }else if(kelembapan_kering&&cahaya_cukup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_kering,cahaya_cukup)/counter;
        confidence = (kelembapan_kering,cahaya_cukup)/kelembapan_cukup;
        lift_ratio = (kelembapan_kering,cahaya_cukup)/counter/support;
    }else if(kelembapan_kering&&cahaya_redup){
        keadaan = "2";
        keadaanLcd = "Kumbung Tdk Baik";
        support = (kelembapan_kering,cahaya_redup)/counter;
        confidence = (kelembapan_kering,cahaya_redup)/kelembapan_kering;
        lift_ratio = (kelembapan_kering,cahaya_redup)/counter/support;
    }
}


void loop()
{
  counter = 0;
  while(counter<5){
    //Read data and store it to variables hum and temp
    kelembapan = dht.readHumidity()-16;
    suhu= dht.readTemperature();
    cahaya = analogRead(pinLDR);

    Serial.println("Suhu : " + String(suhu));
    Serial.println("Kelembapan : " + String(kelembapan));
    Serial.println("Cahaya : " + String(cahaya));
    
    tampilan();
    basis_rule();

    fuzzy();
    counter++;
    Serial.println("Jumlah keseluruhan data : " + String(counter));
    ARM();   
    }
    //Print temp and humidity values to serial monitor

    Serial.println("Suhu Rendah : " + String(suhu_rendah));
    Serial.println("Suhu Cukup : " + String(suhu_cukup));
    Serial.println("Suhu Tinggi : " + String(suhu_tinggi));

    Serial.println("Kelembapan Kering : " + String(kelembapan_kering));
    Serial.println("Kelembapan Cukup : " + String(kelembapan_cukup));
    Serial.println("Kelembapan Basah : " + String(kelembapan_basah));

    Serial.println("Cahaya Redup : " + String(cahaya_redup));
    Serial.println("Cahaya Cukup : " + String(cahaya_cukup));
    Serial.println("Cahaya Terang : " + String(cahaya_terang));

    Serial.println("Support : " + String(support));
    Serial.println("Confidence : " + String(confidence));
    Serial.println("Nilai Lift Rasio : " + String(lift_ratio));

    Serial.println("Keadaan Kumbung : " + String(keadaan));
    
    
    //process cek Koneksi ke server 
    WiFiClient client;
    //Jika Clinet tidak terkoneksi konek ke host
    if(!client.connect(host, 80))
    {
      Serial.println("Connect Failed");
      //Jika Gagal Ulang kembali dari awal 
      return;
      }
      
      //Prosess Pengiriman data ke server 
      String Link;
      HTTPClient http;
      
      //saat mengeksekusi kirimdata.php maka dia mengirim nilai sensornya ke variabel sensornya
      //Link = "http://192.168.172.232/KJamur/arduino/write.php?suhu=" + String(suhu) + "&kelembaban=" + String(lembap) + "&cahaya=" + String(ldr) + "&gas=" + String(gas);
      Link = "http://" + String(host) + "/KJamur/arduino/write.php?suhu=" + String(suhu) + "&kelembaban=" + String(kelembapan) + "&cahaya=" + String(cahaya) + "&keadaan=" + String(keadaan);
      
      //Eksekusi Link
      http.begin(Link);
      
      //Mode Get
      http.GET();
      
      //Baca respon setelah berhasil kirim nilai sensor
      String respon = http.getString();
      Serial.println(respon);
      
      //Jika program telah selesai maka ditutup
      http.end();
      delay(2000);
}

void tampilan() {  
  lcd.clear();
  delay(1000);
  lcd.createChar(1,termometru);
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.print(":");
  lcd.print(suhu);
  lcd.print((char)223);
  lcd.print("ARM:");
  lcd.print(lift_ratio);
  lcd.setCursor(0,1);
  lcd.print(keadaanLcd);

  delay(3000);
  
  lcd.clear();
  delay(1000);
  lcd.createChar(1,picatura);
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.print(":");
  lcd.print(kelembapan);
  lcd.print("%ARM:");
  lcd.print(lift_ratio);
  lcd.setCursor(0,1);
  lcd.print(keadaanLcd);

  delay(3000);

  lcd.clear();
  delay(1000);
  lcd.createChar(1, dia);
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.print(":");
  lcd.print(cahaya);
  lcd.print("%ARM:");
  lcd.print(lift_ratio);
  lcd.setCursor(0,1);
  lcd.print(keadaanLcd);

  delay(3000);
}

void basis_rule(){
  //mesin kabut
  if (kelembapan <=80.00){
    digitalWrite (MistMaker,LOW); //LOW=mesin kabut menyala jika dipasang NC
  }else if((kelembapan >=80) && (kelembapan <=90)){
    digitalWrite(MistMaker, LOW);
  }else if (kelembapan >=90.00){
    digitalWrite (MistMaker,HIGH); 
  }
  //Lampu
  if (cahaya <50){
    digitalWrite(MistLampu, LOW);
  }
  else if((cahaya >=50) && (cahaya <=300)){
    digitalWrite(MistLampu, LOW);
  }else if (cahaya >300){
    digitalWrite(MistLampu, HIGH);
  }
   
   //Kipas
  if(kipas <=33.99){
    digitalWrite (kipas,HIGH); 
  }else if(kipas >=33.99){
    digitalWrite (kipas,LOW); 
  }
}
