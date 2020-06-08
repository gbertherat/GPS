#include <TinyGPS++.h> // Librairie pour traité les données GPS
#include <SoftwareSerial.h> // Librairie en parallèle pour le GPS
#include <LiquidCrystal_I2C.h> // Librairie pour l'affichage sur écran LCD

static const int RXPin = 4, TXPin = 3; // Le GPS est branché sur les branches 3(TX) et 4(RX)
static const uint32_t GPSBaud = 4800; // Baud-rate du GPS


TinyGPSPlus gps; // On créé un objet GPS
SoftwareSerial ss(RXPin, TXPin); // La connection sériale vers le GPS
LiquidCrystal_I2C lcd(0x27,20,4); // On créé un object LCD qui correspond à notre écran LCD 16 charactères, 2 lignes

void setup()
{
  Serial.begin(115200);

  // Initialisation du GPS
  ss.begin(GPSBaud);

  // Initialisation de l'écran LCD                     
  lcd.backlight();
}

void loop()
{
  // Tant que des données GPS sont reçus
  while (ss.available() > 0){
    if (gps.encode(ss.read())){ // On vérifie si elles sont lisibles
      printToLCD(); // Si oui, on appelle la fonction printToLCD qui permet l'affichaque des données sur l'écran LCD
    }
  }

  // Timeout à 3s
  if (millis() > 3000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("Pas de GPS detecté."));
  }
}

void printToLCD(){ // Fonction printToLCD() pour afficher les données sur un écran LCD
  if(gps.location.isValid()){ // Si la localisation est valide
    lcd.setCursor(0,0);
    lcd.print("Latitude: "); // On affiche la latitude sur la première ligne
    lcd.print(gps.location.lat(), 6);

    lcd.setCursor(0,1);
    lcd.print("Longitude: "); // On affiche la longitude sur la deuxième ligne
    lcd.print(gps.location.lng(), 6);
  } else { // Si la localisation n'est pas valide
    lcd.setCursor(0,0);
    lcd.print("Données invalides"); // On affiche un message d'erreur sur la première ligne de l'écran
  }
  delay(300); // On ajout un délai de 300ms (Evile le clignotement)
  lcd.clear(); // Puis on efface le contenu de l'écran
}
