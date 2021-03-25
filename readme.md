# Hőmérő rendszer


## Adatbázis csatlakozás

  - includes/database.inc.php   =>  Adatbázis beállítása


## Routes

  - GET   /index.php        => Fő oldal, grafikon, idő választó mező
  - GET   /insert_data.php  => API point, ahol a temp_1, temp_2 és voltage mezőket várja a szerver
      - POST kérész szebb lenne, az API - hoz


## ESP program

1. Wifi hálózatnév / Jelszó 
``` c++
// Replace with your network details
const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWORD";
```
2. Web szerver doman neve
``` c++
char server[] = "WEBSERVER DOMAIN NAME";
WiFiClient client;
```
3. DS18B20 szenzorok címe
    - elvileg fel lehetne ismerni autómatikusan a szenzor címeket, viszont így gyorsabb
```c++
DeviceAddress TempSensor1 = { 0x28, 0x91, 0xA9, 0xE2, 0x08, 0x00, 0x00, 0x33 };
```

### Ismert hibák

  Az összes SQL lekérdezés [SQL Injection](https://en.wikipedia.org/wiki/SQL_injection) támadható!
