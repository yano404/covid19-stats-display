#include <Arduino.h>
#include <rpcWiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Free_Fonts.h"
#include "TFT_eSPI.h"

TFT_eSPI tft;
WiFiClientSecure clientMMedia;
WiFiClientSecure clientUNStats;

const char *ssid = "Your WiFi SSID";            // WiFi SSID
const char *password = "Your WiFi Password";    // WiFi PASS
const unsigned long REFRESH_INTERVAL = 3600000; // 60min
int countryID = 18;                             // Default: Japan
const int TRY_MAX_NUM = 5;

// COUNTRIES_VACCINE_QUERY is used to get the number of vaccinated from API.
// COUNTRIES_CASES_QUERY is used to get the cases from API.
const int COUNTRIES_NUM = 45;
const static char *COUNTRIES[] = {
    "Australia",
    "Belgium",
    "Brazil",
    "Canada",
    "Chile",
    "China",
    "Denmark",
    "Egypt",
    "Finland",
    "France",
    "Germany",
    "India",
    "Indonesia",
    "Iran",
    "Iraq",
    "Ireland",
    "Israel",
    "Italy",
    "Japan",
    "Malaysia",
    "Mexico",
    "Nepal",
    "Netherlands",
    "New Zealand",
    "Norway",
    "Pakistan",
    "Peru",
    "Philippines",
    "Poland",
    "Portugal",
    "Russia",
    "Saudi Arabia",
    "Singapore",
    "South Africa",
    "South Korea",
    "Spain",
    "Sweden",
    "Switzerland",
    "Taiwan",
    "Thailand",
    "Turkey",
    "United Kingdom",
    "United States",
    "Vietnam",
    "World",
};
const static char *COUNTRIES_VACCINE_QUERY[] = {
    "Australia",
    "Belgium",
    "Brazil",
    "Canada",
    "Chile",
    "China",
    "Denmark",
    "Egypt",
    "Finland",
    "France",
    "Germany",
    "India",
    "Indonesia",
    "Iran",
    "Iraq",
    "Ireland",
    "Israel",
    "Italy",
    "Japan",
    "Malaysia",
    "Mexico",
    "Nepal",
    "Netherlands",
    "New%20Zealand",
    "Norway",
    "Pakistan",
    "Peru",
    "Philippines",
    "Poland",
    "Portugal",
    "Russia",
    "Saudi%20Arabia",
    "Singapore",
    "South&20Africa",
    "South%20Korea",
    "Spain",
    "Sweden",
    "Switzerland",
    "Taiwan",
    "Thailand",
    "Turkey",
    "United%20Kingdom",
    "United%20States",
    "Vietnam",
    "World",
};
const static char *COUNTRIES_CASES_QUERY[] = {
    "Australia",
    "Belgium",
    "Brazil",
    "Canada",
    "Chile",
    "China",
    "Denmark",
    "Egypt",
    "Finland",
    "France",
    "Germany",
    "India",
    "Indonesia",
    "Iran",
    "Iraq",
    "Ireland",
    "Israel",
    "Italy",
    "Japan",
    "Malaysia",
    "Mexico",
    "Nepal",
    "Netherlands",
    "New%20Zealand",
    "Norway",
    "Pakistan",
    "Peru",
    "Philippines",
    "Poland",
    "Portugal",
    "Russia",
    "Saudi%20Arabia",
    "Singapore",
    "South%20Africa",
    "Korea,%20South",
    "Spain",
    "Sweden",
    "Switzerland",
    "Taiwan*",
    "Thailand",
    "Turkey",
    "United%20Kingdom",
    "US",
    "Vietnam",
    "Global",
};
const static char *COUNTRIES_OBJECT_IDS[] = {
    "9",
    "17",
    "24",
    "33",
    "36",
    "37",
    "48",
    "54",
    "62",
    "63",
    "67",
    "80",
    "81",
    "82",
    "83",
    "84",
    "85",
    "86",
    "88",
    "108",
    "115",
    "124",
    "125",
    "126",
    "131",
    "133",
    "137",
    "138",
    "139",
    "140",
    "143",
    "151",
    "156",
    "161",
    "92",
    "163",
    "167",
    "168",
    "170",
    "173",
    "178",
    "183",
    "179",
    "188",
    "" // global does not exist
};
const int COUNTRY_WORLD_ID = COUNTRIES_NUM - 1;

String baseURLMMedia = "https://covid-api.mmediagroup.fr/v1/";
const char *mmedia_root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
    "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
    "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
    "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
    "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
    "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
    "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
    "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
    "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
    "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
    "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
    "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
    "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
    "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
    "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
    "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
    "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
    "rqXRfboQnoZsG4q5WTP468SQvvG5\n"
    "-----END CERTIFICATE-----\n";

String baseURLUNStats = "https://services1.arcgis.com/0MSEUqKaxRlEPj5g/arcgis/rest/services/ncov_cases2_v1/FeatureServer/2/";
const char *unstats_root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
    "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n"
    "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
    "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
    "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n"
    "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n"
    "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n"
    "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n"
    "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n"
    "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n"
    "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n"
    "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n"
    "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n"
    "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n"
    "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n"
    "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n"
    "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n"
    "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n"
    "+OkuE6N36B9K\n"
    "-----END CERTIFICATE-----\n";

const uint16_t TFT_TEXT_COLOR = tft.color565(230, 230, 230);
const uint16_t TFT_BG_COLOR = tft.color565(5, 5, 5);
const uint16_t TFT_TEXT_BG_COLOR = tft.color565(40, 40, 40);

void setup()
{
  Serial.begin(115200);
  //while (!Serial); // Enable this line only when debugging.
  delay(1000);

  Serial.println();
  Serial.println("=======================");
  Serial.println(" COVID19 Stats Display ");
  Serial.println("=======================");
  Serial.println();
  Serial.println("Start");
  Serial.println();

  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_A, INPUT_PULLUP);

  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(3);
  tft.setFreeFont(FM18);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(BC_DATUM);
  tft.drawString("COVID-19", 160, 115);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("STATS DISPLAY", 160, 125);
  tft.setTextDatum(TL_DATUM);

  // Connect to WiFi
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(500);
  WiFi.begin(ssid, password);

  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(FMB12);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor((320 - tft.textWidth("Connecting to Wi-Fi..")) / 2, 120);
  tft.print("Connecting to Wi-Fi..");

  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(500);
  }

  Serial.print("Connected to");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor((320 - tft.textWidth("WiFi Connected!")) / 2, 120);
  tft.print("WiFi Connected!");
  delay(1000);

  // Setup WiFi client
  clientMMedia.setCACert(mmedia_root_ca);
  clientUNStats.setCACert(unstats_root_ca);
}

int loopCount = 0;
DynamicJsonDocument docCases(JSON_OBJECT_SIZE(1000));
DynamicJsonDocument docVaccinated(JSON_OBJECT_SIZE(50));
/* History data is too heavy */
//DynamicJsonDocument docHistory(JSON_OBJECT_SIZE(5000));
DynamicJsonDocument docUNStats(JSON_OBJECT_SIZE(300));

/*
 * 0 -> summary
 * 1 -> district
 * 2 -> vaccinated
 */
int displaySwitch = 0;
int districtID = 0; // default: 0 -> All
String country;
float population;
unsigned long totalConfirmed;
unsigned long totalDeaths;
unsigned long totalRecovered;
unsigned long totalActive;
unsigned long vaccinated;
unsigned long partiallyVaccinated;
unsigned long administered;
float incidentRate;
float mortalRate;
float vaccinationRate;
float partiallyVaccinationRate;
unsigned long districtConfirmed;
unsigned long districtDeaths;
unsigned long districtRecovered;
unsigned long districtActive;
float districtMortalRate;
String districtMsg;
String vaccinesMsg;
String summaryMsg;
const char *districtList[100];
int districtNum;

// Get data from API and deserialize JSON
int getDataMMedia(const String, DynamicJsonDocument &);
int getDataUNStats(int, DynamicJsonDocument &);

// Display
void display();
// Display summary data
void displaySummary();
// Display district data
void displayDistrict();
// Display people vaccinated
void displayVaccinated();
// Display widget to change country
int widgetChangeCountry();
// Display widget to change district
void widgetChangeDistrict();
// Funtion to set the values of the district
void setDistrict(int);
// Function to clip text
String clipText(const char *, int);

void drawFloatNum(float, int, uint16_t, uint16_t);

void loop()
{
  tft.setTextDatum(TL_DATUM); // Align top left (default)
  tft.setFreeFont(FMB12);

  String urlCases = baseURLMMedia + "cases?country=" + COUNTRIES_CASES_QUERY[countryID];
  String urlVaccinated = baseURLMMedia + "vaccines?country=" + COUNTRIES_VACCINE_QUERY[countryID];
  //String urlHistory = baseURLMMedia + "history?country=" + COUNTRIES_CASES_QUERY[countryID] + "&status=confirmed";

  tft.fillScreen(TFT_BG_COLOR);
  tft.fillRoundRect(10, 20, 300, 60, 5, TFT_TEXT_BG_COLOR);
  tft.fillRoundRect(10, 90, 300, 60, 5, TFT_TEXT_BG_COLOR);
  tft.fillRoundRect(10, 160, 300, 60, 5, TFT_TEXT_BG_COLOR);
  tft.fillCircle(45, 50, 25, TFT_BG_COLOR);
  tft.fillCircle(45, 120, 25, TFT_BG_COLOR);
  tft.fillCircle(45, 190, 25, TFT_BG_COLOR);
  tft.setTextDatum(ML_DATUM); // Align mid left
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.drawString("Cases", 90, 50);
  tft.drawString("Vaccination", 90, 120);
  tft.drawString("UNStats", 90, 190);
  tft.setTextDatum(TL_DATUM); // Align top left (default)

  tft.setTextDatum(MC_DATUM); // Align mid center
  int successCases = 0;
  int successVaccines = 0;
  int successUNStats = 0;
  for (int i = 0; i < TRY_MAX_NUM; i++)
  {
    Serial.println(i);
    if (getDataMMedia(urlCases, docCases))
    {
      successCases = 1;
      break;
    }
  }
  if (successCases)
  {
    tft.fillCircle(45, 50, 25, TFT_DARKGREEN);
    tft.drawString("OK", 45, 50);
    delay(100);
  }
  else
  {
    tft.fillCircle(45, 50, 25, TFT_RED);
    tft.drawString("!", 45, 50);
    delay(100);
  }
  for (int i = 0; i < TRY_MAX_NUM; i++)
  {
    Serial.println(i);
    if (getDataMMedia(urlVaccinated, docVaccinated))
    {
      successVaccines = 1;
      break;
    }
  }
  if (successVaccines)
  {
    tft.fillCircle(45, 120, 25, TFT_DARKGREEN);
    tft.drawString("OK", 45, 120);
    delay(100);
  }
  else
  {
    tft.fillCircle(45, 120, 25, TFT_RED);
    tft.drawString("!", 45, 120);
    delay(100);
  }
  //getDataMMedia(urlHistory, docHistory); // Too heavy
  for (int i = 0; i < TRY_MAX_NUM; i++)
  {
    Serial.println(i);
    if (getDataUNStats(countryID, docUNStats))
    {
      successUNStats = 1;
      break;
    }
  }
  if (successUNStats)
  {
    tft.fillCircle(45, 190, 25, TFT_DARKGREEN);
    tft.drawString("OK", 45, 190);
    delay(100);
  }
  else
  {
    tft.fillCircle(45, 190, 25, TFT_RED);
    tft.drawString("!", 45, 190);
    delay(100);
  }
  delay(1000);
  tft.setTextDatum(TL_DATUM); // Align top left (default)

  country = COUNTRIES[countryID];

  if (successCases)
  {
    totalConfirmed = docCases["All"]["confirmed"].as<unsigned long>();
    totalDeaths = docCases["All"]["deaths"].as<unsigned long>();
    totalRecovered = docCases["All"]["recovered"].as<unsigned long>();
    totalActive = totalConfirmed - totalDeaths - totalRecovered;
    mortalRate = (float)totalDeaths / totalConfirmed * 100.0;

    // Check the docCases has population.
    // If population is not contained, unable to calculate accinationRate
    if (docCases["All"].as<JsonObject>().containsKey("population"))
    {
      population = docCases["All"]["population"].as<float>();
    }
    else if (successVaccines && docVaccinated["All"].as<JsonObject>().containsKey("population"))
    {
      population = docVaccinated["All"]["population"].as<float>();
    }
    else
    {
      population = NAN;
    }
    // Generate districtList
    districtNum = 0;
    for (JsonObject::iterator itr = docCases.as<JsonObject>().begin(); itr != docCases.as<JsonObject>().end(); ++itr)
    {
      districtList[districtNum] = itr->key().c_str();
      districtNum++;
    }
    summaryMsg = "Success";
  }
  else if (successUNStats && countryID != COUNTRY_WORLD_ID)
  {
    // If it is failed to get the data from mmedia API, use the data from UNStats.
    totalConfirmed = docUNStats["features"][0]["attributes"]["Confirmed"].as<unsigned long>();
    totalDeaths = docUNStats["features"][0]["attributes"]["Deaths"].as<unsigned long>();
    totalRecovered = docUNStats["features"][0]["attributes"]["Recovered"].as<unsigned long>();
    totalActive = docUNStats["features"][0]["attributes"]["Active"].as<unsigned long>();
    mortalRate = docUNStats["features"][0]["attributes"]["Mortality_Rate"].as<float>();
    // UNStats data does not contain population, so can not find vaccinationRate.
    // Some vaccination data contains population, if so, use this.
    if (successVaccines && docVaccinated["All"].as<JsonObject>().containsKey("population"))
    {
      population = docVaccinated["All"]["population"].as<float>();
    }
    else
    {
      population = NAN;
    }

    districtNum = 1;
    districtList[0] = "All";
    deserializeJson(
        docCases,
        "{\"All\": {\"confirmed\":" +
            String(totalConfirmed) + "," +
            "\"recovered\":" +
            String(totalRecovered) + "," +
            "\"deaths\":" +
            String(totalDeaths) + "}}");
    summaryMsg = "Partially failed";
  }
  else
  {
    // Unable to cases
    totalConfirmed = 0;
    totalDeaths = 0;
    totalRecovered = 0;
    totalActive = 0;
    mortalRate = NAN;
    population = NAN;
    districtNum = 1;
    districtList[0] = "All";
    deserializeJson(
        docCases,
        "{\"All\": {\"confirmed\":" +
            String(totalConfirmed) + "," +
            "\"recovered\":" +
            String(totalRecovered) + "," +
            "\"deaths\":" +
            String(totalDeaths) + "}}");
    summaryMsg = "Failed";
  }

  if (successUNStats)
  {
    if (countryID == COUNTRY_WORLD_ID)
    {
      incidentRate = NAN;
    }
    else
    {
      incidentRate = docUNStats["features"][0]["attributes"]["Incident_Rate"].as<float>();
    }
  }
  else
  {
    incidentRate = NAN;
  }

  if (successVaccines)
  {
    vaccinated = docVaccinated["All"]["people_vaccinated"].as<unsigned long>();
    partiallyVaccinated = docVaccinated["All"]["people_partially_vaccinated"].as<unsigned long>();
    vaccinesMsg = docVaccinated["All"]["updated"].as<String>();
    administered = docVaccinated["All"]["administered"].as<unsigned long>();
  }
  else
  {
    vaccinated = 0;
    partiallyVaccinated = 0;
    administered = 0;
    vaccinesMsg = "Unable to get the data";
  }
  vaccinationRate = (float)vaccinated / population * 100.0;
  partiallyVaccinationRate = (float)partiallyVaccinated / population * 100.0;

  Serial.println("[Data]");
  Serial.print("Country                    : ");
  Serial.println(docCases["All"]["country"].as<String>());
  Serial.print("Population                 : ");
  Serial.printf("%.2f\n", population);
  Serial.print("Confirmed                  : ");
  Serial.println(totalConfirmed);
  Serial.print("Deaths                     : ");
  Serial.println(totalDeaths);
  Serial.print("Recovered                  : ");
  Serial.println(totalRecovered);
  Serial.print("Incident Rate              : ");
  Serial.println(incidentRate);
  Serial.print("Death Rate                 : ");
  Serial.println(mortalRate);
  Serial.print("Vaccinated                 : ");
  Serial.println(vaccinated);
  Serial.print("Partially Vaccinated       : ");
  Serial.println(partiallyVaccinated);
  Serial.print("Admnistered                : ");
  Serial.println(administered);
  Serial.print("Vaccination Rate (x2)      : ");
  Serial.println(vaccinationRate);
  Serial.print("Vaccination Rate (x1 + x2) : ");
  Serial.println(partiallyVaccinationRate);
  Serial.print("Vaccination Msg(Updated)   : ");
  Serial.println(vaccinesMsg);
  Serial.println();

  // Check districtList
  Serial.println("[Districts]");
  for (int i = 0; i != districtNum; ++i)
  {
    Serial.printf(" %03d: ", i);
    Serial.println(districtList[i]);
  }

  if (districtID >= districtNum)
  {
    // There are the cases docCases does not contain enough data.
    districtID = 0;
  }

  setDistrict(districtID);

  Serial.println();
  Serial.print("Refresh Interval: ");
  Serial.print(REFRESH_INTERVAL / 60000);
  Serial.println(" min");
  Serial.println();

  // Display
  display();

  unsigned long t0 = millis();
  unsigned long t1 = millis();
  while (t1 >= t0 && t1 - t0 < REFRESH_INTERVAL)
  {
    int swC = digitalRead(WIO_KEY_C);
    if (swC == LOW)
    {
      // Open the widget to cahnge country
      if (widgetChangeCountry())
      {
        break;
      }
      display();
      delay(100);
    }

    int swB = digitalRead(WIO_KEY_B);
    if (swB == LOW)
    {
      // Open the widget to change district
      widgetChangeDistrict();
      display();
      delay(100);
    }

    int swA = digitalRead(WIO_KEY_A);
    if (swA == LOW)
    {
      // Force refresh
      break;
    }

    int swLEFT = digitalRead(WIO_5S_LEFT);
    int swRIGHT = digitalRead(WIO_5S_RIGHT);
    if (swRIGHT == LOW)
    {
      displaySwitch++;
      displaySwitch %= 3;
      display();
      delay(100);
    }
    if (swLEFT == LOW)
    {
      displaySwitch += 3;
      displaySwitch--;
      displaySwitch %= 3;
      display();
      delay(100);
    }
    t1 = millis();
  }

  loopCount++;
  Serial.print("loop count: ");
  Serial.println(loopCount);
}

int getDataMMedia(const String url, DynamicJsonDocument &doc)
{
  HTTPClient https;
  int success = 0;

  // Set timeout
  https.setTimeout(1000);
  Serial.println("Attempting to get the data from MMedia");
  Serial.print("[HTTPS] begin...\n");

  if (https.begin(clientMMedia, url))
  { // HTTPS
    Serial.println("Fetch data from");
    Serial.println(url);
    Serial.print("[HTTPS] GET...\n");
    // Start connection and send HTTP header
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // File found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        // Get Information
        String data = https.getString();
        Serial.println(data);
        // Deserialize JSON
        deserializeJson(doc, data);
        success = 1;
      }
    }
    else
    {
      Serial.printf("[HTTPS] GET data from MMedia... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  }
  return success;
}

int getDataUNStats(int countryID, DynamicJsonDocument &doc)
{
  int success = 0;
  if (countryID == COUNTRY_WORLD_ID)
  {
    // If country == Global: incident rate does not exist
    success = 1;
  }
  else
  {
    HTTPClient https;

    String url = baseURLUNStats +
                 "query?where=&objectIds=" +
                 COUNTRIES_OBJECT_IDS[countryID] +
                 "&outFields=OBJECTID,Country_Region,Confirmed,Deaths,Recovered,Active,Incident_Rate,People_Tested,People_Hospitalized,Mortality_Rate,Last_Update" +
                 "&outSR=4326&f=pjson";

    // Set timeout
    https.setTimeout(1000);

    Serial.print("Attempting to get the data from UNStats\n");
    Serial.print("[HTTPS] begin...\n");

    if (https.begin(clientUNStats, url))
    { // HTTPS
      Serial.println("Fetch data from");
      Serial.println(url);
      Serial.print("[HTTPS] GET...\n");
      // Start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0)
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // File found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          // Get Information.
          String data = https.getString();
          Serial.println(data);
          // Deserialize JSON
          deserializeJson(doc, data);
          success = 1;
        }
      }
      else
      {
        Serial.printf("[HTTPS] GET data from UNStats... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
  }
  return success;
}

void display()
{
  switch (displaySwitch)
  {
  case 0:
    displaySummary();
    break;

  case 1:
    displayDistrict();
    break;

  case 2:
    displayVaccinated();
    break;

  default:
    displaySummary();
    break;
  }
}

// Display summary data
void displaySummary()
{
  tft.fillScreen(TFT_BG_COLOR);
  tft.setFreeFont(FF17);
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.setTextDatum(TC_DATUM); // Align top center
  tft.drawString("COVID-19 Summary / " + country, 160, 10);

  tft.fillRect(8, 35, 150, 55, TFT_TEXT_BG_COLOR);
  tft.fillRect(8, 95, 150, 55, TFT_TEXT_BG_COLOR);
  tft.fillRect(8, 155, 150, 55, TFT_TEXT_BG_COLOR);

  tft.fillRect(162, 35, 150, 55, TFT_TEXT_BG_COLOR);
  tft.fillRect(162, 95, 150, 55, TFT_TEXT_BG_COLOR);
  tft.fillRect(162, 155, 150, 55, TFT_TEXT_BG_COLOR);

  tft.setFreeFont(FM9);
  tft.drawString("Confirmed", 83, 40);
  tft.drawString("Deaths", 83, 100);
  tft.drawString("Recovered", 83, 160);
  tft.drawString("Active", 236, 40);
  tft.drawString("Incident Rate", 236, 100);
  tft.drawString("Death Rate", 236, 160);

  tft.setTextDatum(TR_DATUM); // Align top right
  tft.setFreeFont(FMB12);

  tft.setTextColor(TFT_RED);
  tft.drawNumber(totalConfirmed, 148, 65);
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.drawNumber(totalDeaths, 148, 125);
  tft.setTextColor(TFT_GREEN);
  tft.drawNumber(totalRecovered, 148, 185);

  tft.setTextColor(TFT_RED);
  tft.drawNumber(totalActive, 302, 65);
  tft.setTextColor(TFT_TEXT_COLOR);
  drawFloatNum(incidentRate, 2, 302, 125);
  drawFloatNum(mortalRate, 2, 302, 185);

  tft.setTextDatum(TR_DATUM); // Align top right
  tft.setFreeFont(FM9);
  tft.setTextColor(tft.color565(224, 225, 232));
  tft.drawString(summaryMsg, 310, 220);
}

// Display district data
void displayDistrict()
{
  tft.fillScreen(TFT_BG_COLOR);
  tft.setFreeFont(FF17);
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.setTextDatum(TC_DATUM); // Align top center
  tft.drawString(clipText(districtList[districtID], 310), 160, 10);

  tft.fillRect(8, 35, 304, 55, TFT_TEXT_BG_COLOR);

  tft.fillRect(8, 95, 150, 55, TFT_TEXT_BG_COLOR);
  tft.fillRect(8, 155, 150, 55, TFT_TEXT_BG_COLOR);

  tft.fillRect(162, 95, 150, 55, TFT_TEXT_BG_COLOR);
  tft.fillRect(162, 155, 150, 55, TFT_TEXT_BG_COLOR);

  tft.setFreeFont(FM9);
  tft.drawString("Total Confirmed", 160, 40);

  tft.drawString("Deaths", 83, 100);
  tft.drawString("Recovered", 83, 160);
  tft.drawString("Active", 236, 100);
  tft.drawString("Death Rate", 236, 160);

  tft.setTextDatum(TC_DATUM); // Align top center
  tft.setFreeFont(FMB12);

  tft.setTextColor(TFT_RED);
  tft.drawNumber(districtConfirmed, 160, 65);

  tft.setTextDatum(TR_DATUM); // Align top right
  tft.setFreeFont(FMB12);

  tft.setTextColor(TFT_TEXT_COLOR);
  tft.drawNumber(districtDeaths, 148, 125);
  tft.setTextColor(TFT_GREEN);
  tft.drawNumber(districtRecovered, 148, 185);

  tft.setTextColor(TFT_RED);
  tft.drawNumber(districtActive, 302, 125);
  tft.setTextColor(TFT_TEXT_COLOR);
  drawFloatNum(districtMortalRate, 2, 302, 185);

  tft.setTextDatum(TR_DATUM); // Align top right
  tft.setFreeFont(FM9);
  tft.setTextColor(tft.color565(224, 225, 232));
  tft.drawString(districtMsg, 310, 220);
}

// Display people vaccinated
void displayVaccinated()
{
  tft.fillScreen(TFT_BG_COLOR);
  tft.setFreeFont(FF17);
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.setTextDatum(TC_DATUM); // Align top center
  tft.drawString("Vaccination / " + country, 160, 10);

  tft.fillRect(10, 35, 300, 55, TFT_TEXT_BG_COLOR);
  tft.fillRect(10, 95, 300, 55, TFT_TEXT_BG_COLOR);

  tft.setFreeFont(FM9);
  tft.drawString("Fully Vaccinated", 160, 40);
  tft.drawString("Partially Vaccinated", 160, 100);
  tft.drawString("Rate", 160, 160);

  tft.setFreeFont(FMB12);
  tft.setTextColor(TFT_GREEN);
  tft.drawNumber(vaccinated, 160, 65);
  tft.setTextColor(TFT_GREENYELLOW);
  tft.drawNumber(partiallyVaccinated, 160, 125);

  // Draw chart
  tft.fillRect(10, 155, 300, 55, TFT_TEXT_BG_COLOR);
  tft.fillRect(11, 156, (int)298 * partiallyVaccinationRate / 100.0, 53, TFT_GREENYELLOW);
  tft.fillRect(11, 156, (int)298 * vaccinationRate / 100.0, 53, TFT_GREEN);

  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.setTextDatum(TR_DATUM); // Align top right
  drawFloatNum(vaccinationRate, 2, 300, 165);
  drawFloatNum(partiallyVaccinationRate, 2, 300, 190);

  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.drawString(vaccinesMsg, 310, 220);
}

// Display widget to change country
int widgetChangeCountry()
{
  int selectedCountryID = countryID;
  int changed = 0;

  tft.fillScreen(TFT_BG_COLOR);
  tft.setFreeFont(FF17);
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.setTextDatum(TC_DATUM); // Align top center
  tft.drawString("Select Country", 160, 10);

  tft.fillRoundRect(10, 35, 300, 35, 5, TFT_TEXT_BG_COLOR);
  tft.fillRoundRect(10, 75, 300, 35, 5, TFT_TEXT_BG_COLOR);
  tft.fillRoundRect(10, 115, 300, 35, 5, TFT_TEXT_COLOR);
  tft.fillRoundRect(10, 155, 300, 35, 5, TFT_TEXT_BG_COLOR);
  tft.fillRoundRect(10, 195, 300, 35, 5, TFT_TEXT_BG_COLOR);

  tft.setTextDatum(MC_DATUM); // Align mid center
  tft.setFreeFont(FF17);
  tft.setTextPadding(300);

  while (1)
  {
    if (selectedCountryID == 0)
    {
      tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
      tft.drawString(COUNTRIES[selectedCountryID], 160, 133);
      tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
      tft.drawString("", 160, 53);
      tft.drawString("", 160, 93);
      tft.drawString(COUNTRIES[selectedCountryID + 1], 160, 173);
      tft.drawString(COUNTRIES[selectedCountryID + 2], 160, 213);
    }
    else if (selectedCountryID == 1)
    {
      tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
      tft.drawString(COUNTRIES[selectedCountryID], 160, 133);
      tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
      tft.drawString("", 160, 53);
      tft.drawString(COUNTRIES[selectedCountryID - 1], 160, 93);
      tft.drawString(COUNTRIES[selectedCountryID + 1], 160, 173);
      tft.drawString(COUNTRIES[selectedCountryID + 2], 160, 213);
    }
    else if (selectedCountryID == COUNTRY_WORLD_ID)
    {
      tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
      tft.drawString(COUNTRIES[selectedCountryID], 160, 133);
      tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
      tft.drawString(COUNTRIES[selectedCountryID - 2], 160, 53);
      tft.drawString(COUNTRIES[selectedCountryID - 1], 160, 93);
      tft.drawString("", 160, 173);
      tft.drawString("", 160, 213);
    }
    else if (selectedCountryID == COUNTRIES_NUM - 2)
    {
      tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
      tft.drawString(COUNTRIES[selectedCountryID], 160, 133);
      tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
      tft.drawString(COUNTRIES[selectedCountryID - 2], 160, 53);
      tft.drawString(COUNTRIES[selectedCountryID - 1], 160, 93);
      tft.drawString(COUNTRIES[selectedCountryID + 1], 160, 173);
      tft.drawString("", 160, 213);
    }
    else
    {
      tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
      tft.drawString(COUNTRIES[selectedCountryID], 160, 133);
      tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
      tft.drawString(COUNTRIES[selectedCountryID - 2], 160, 53);
      tft.drawString(COUNTRIES[selectedCountryID - 1], 160, 93);
      tft.drawString(COUNTRIES[selectedCountryID + 1], 160, 173);
      tft.drawString(COUNTRIES[selectedCountryID + 2], 160, 213);
    }

    delay(100); // Without delay, scroll speed is too fast.

    int swUP = digitalRead(WIO_5S_UP);
    int swDOWN = digitalRead(WIO_5S_DOWN);
    int swC = digitalRead(WIO_KEY_C);
    int swPRESS = digitalRead(WIO_5S_PRESS);
    if (swDOWN == LOW)
    {
      if (selectedCountryID != COUNTRY_WORLD_ID)
      {
        selectedCountryID++;
      }
    }
    else if (swUP == LOW)
    {
      if (selectedCountryID != 0)
      {
        selectedCountryID--;
      }
    }
    if (swC == LOW)
    {
      break;
    }
    else if (swPRESS == LOW)
    {
      countryID = selectedCountryID;
      districtID = 0;
      changed = 1;
      break;
    }
  }
  return changed;
};

// Display widget to change district
void widgetChangeDistrict()
{
  int selectedDistrictID = districtID;

  tft.fillScreen(TFT_BG_COLOR);
  tft.setFreeFont(FF17);
  tft.setTextColor(TFT_TEXT_COLOR);
  tft.setTextDatum(TC_DATUM); // Align top center
  tft.drawString("Select District", 160, 10);

  tft.fillRoundRect(10, 35, 300, 35, 5, TFT_TEXT_BG_COLOR);
  tft.fillRoundRect(10, 75, 300, 35, 5, TFT_TEXT_BG_COLOR);
  tft.fillRoundRect(10, 115, 300, 35, 5, TFT_WHITE);
  tft.fillRoundRect(10, 155, 300, 35, 5, TFT_TEXT_BG_COLOR);
  tft.fillRoundRect(10, 195, 300, 35, 5, TFT_TEXT_BG_COLOR);

  tft.setTextDatum(MC_DATUM); // Align mid center
  tft.setFreeFont(FF17);
  tft.setTextPadding(300);

  int max_text_width = 290;

  switch (districtNum)
  {
  case 1:
    /* [All] */
    while (1)
    {
      tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
      tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
      tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
      tft.drawString("", 160, 53);
      tft.drawString("", 160, 93);
      tft.drawString("", 160, 173);
      tft.drawString("", 160, 213);

      delay(100); // Without delay, scroll speed is too fast.

      int swB = digitalRead(WIO_KEY_B);
      int swPRESS = digitalRead(WIO_5S_PRESS);

      if (swB == LOW || swPRESS == LOW)
      {
        break;
      }
    }
    break;

  case 2:
    /* [All, x1] */
    while (1)
    {
      if (selectedDistrictID == 0)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString("", 160, 53);
        tft.drawString("", 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString("", 160, 213);
      }
      else
      {
        /* selectedDistrictID == 1 */
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString("", 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString("", 160, 173);
        tft.drawString("", 160, 213);
      }

      delay(100); // Without delay, scroll speed is too fast.

      int swUP = digitalRead(WIO_5S_UP);
      int swDOWN = digitalRead(WIO_5S_DOWN);
      int swB = digitalRead(WIO_KEY_B);
      int swPRESS = digitalRead(WIO_5S_PRESS);
      if (swDOWN == LOW)
      {
        if (selectedDistrictID != districtNum - 1)
        {
          selectedDistrictID++;
        }
      }
      else if (swUP == LOW)
      {
        if (selectedDistrictID != 0)
        {
          selectedDistrictID--;
        }
      }
      if (swB == LOW)
      {
        break;
      }
      else if (swPRESS == LOW)
      {
        setDistrict(selectedDistrictID);
        break;
      }
    }
    break;

  case 3:
    /* [All, x1, x2] */
    while (1)
    {
      if (selectedDistrictID == 0)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString("", 160, 53);
        tft.drawString("", 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString(clipText(districtList[selectedDistrictID + 2], max_text_width), 160, 213);
      }
      else if (selectedDistrictID == 1)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString("", 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString("", 160, 213);
      }
      else
      {
        /* selectedDistrictID == 2 */
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID - 2], max_text_width), 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString("", 160, 173);
        tft.drawString("", 160, 213);
      }

      delay(100); // Without delay, scroll speed is too fast.

      int swUP = digitalRead(WIO_5S_UP);
      int swDOWN = digitalRead(WIO_5S_DOWN);
      int swB = digitalRead(WIO_KEY_B);
      int swPRESS = digitalRead(WIO_5S_PRESS);
      if (swDOWN == LOW)
      {
        if (selectedDistrictID != districtNum - 1)
        {
          selectedDistrictID++;
        }
      }
      else if (swUP == LOW)
      {
        if (selectedDistrictID != 0)
        {
          selectedDistrictID--;
        }
      }
      if (swB == LOW)
      {
        break;
      }
      else if (swPRESS == LOW)
      {
        setDistrict(selectedDistrictID);
        break;
      }
    }
    break;

  case 4:
    /* [All, x1, x2, x3] */
    while (1)
    {
      if (selectedDistrictID == 0)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString("", 160, 53);
        tft.drawString("", 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString(clipText(districtList[selectedDistrictID + 2], max_text_width), 160, 213);
      }
      else if (selectedDistrictID == 1)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString("", 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString(clipText(districtList[selectedDistrictID + 2], max_text_width), 160, 213);
      }
      else if (selectedDistrictID == districtNum - 1)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID - 2], max_text_width), 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString("", 160, 173);
        tft.drawString("", 160, 213);
      }
      else
      {
        /* selectedDistrictID == 2 */
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID - 2], max_text_width), 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString("", 160, 213);
      }

      delay(100); // Without delay, scroll speed is too fast.

      int swUP = digitalRead(WIO_5S_UP);
      int swDOWN = digitalRead(WIO_5S_DOWN);
      int swB = digitalRead(WIO_KEY_B);
      int swPRESS = digitalRead(WIO_5S_PRESS);
      if (swDOWN == LOW)
      {
        if (selectedDistrictID != districtNum - 1)
        {
          selectedDistrictID++;
        }
      }
      else if (swUP == LOW)
      {
        if (selectedDistrictID != 0)
        {
          selectedDistrictID--;
        }
      }
      if (swB == LOW)
      {
        break;
      }
      else if (swPRESS == LOW)
      {
        setDistrict(selectedDistrictID);
        break;
      }
    }
    break;

  default:
    while (1)
    {
      if (selectedDistrictID == 0)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString("", 160, 53);
        tft.drawString("", 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString(clipText(districtList[selectedDistrictID + 2], max_text_width), 160, 213);
      }
      else if (selectedDistrictID == 1)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString("", 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString(clipText(districtList[selectedDistrictID + 2], max_text_width), 160, 213);
      }
      else if (selectedDistrictID == districtNum - 1)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID - 2], max_text_width), 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString("", 160, 173);
        tft.drawString("", 160, 213);
      }
      else if (selectedDistrictID == districtNum - 2)
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID - 2], max_text_width), 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString("", 160, 213);
      }
      else
      {
        tft.setTextColor(TFT_TEXT_BG_COLOR, TFT_TEXT_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID], max_text_width), 160, 133);
        tft.setTextColor(TFT_TEXT_COLOR, TFT_TEXT_BG_COLOR);
        tft.drawString(clipText(districtList[selectedDistrictID - 2], max_text_width), 160, 53);
        tft.drawString(clipText(districtList[selectedDistrictID - 1], max_text_width), 160, 93);
        tft.drawString(clipText(districtList[selectedDistrictID + 1], max_text_width), 160, 173);
        tft.drawString(clipText(districtList[selectedDistrictID + 2], max_text_width), 160, 213);
      }

      delay(100); // Without delay, scroll speed is too fast.

      int swUP = digitalRead(WIO_5S_UP);
      int swDOWN = digitalRead(WIO_5S_DOWN);
      int swB = digitalRead(WIO_KEY_B);
      int swPRESS = digitalRead(WIO_5S_PRESS);
      if (swDOWN == LOW)
      {
        if (selectedDistrictID != districtNum - 1)
        {
          selectedDistrictID++;
        }
      }
      else if (swUP == LOW)
      {
        if (selectedDistrictID != 0)
        {
          selectedDistrictID--;
        }
      }
      if (swB == LOW)
      {
        break;
      }
      else if (swPRESS == LOW)
      {
        setDistrict(selectedDistrictID);
        break;
      }
    }
    break;
  }
}

void setDistrict(int id)
{
  districtID = id;
  districtConfirmed = docCases[districtList[id]]["confirmed"].as<unsigned long>();
  districtDeaths = docCases[districtList[id]]["deaths"].as<unsigned long>();
  districtRecovered = docCases[districtList[id]]["recovered"].as<unsigned long>();
  districtActive = districtConfirmed - districtDeaths - districtRecovered;
  if (districtConfirmed == 0)
  {
    districtMortalRate = NAN;
  }
  else
  {
    districtMortalRate = (float)districtDeaths / districtConfirmed * 100.0;
  }
  if (id == 0)
  {
    districtMsg = "";
  }
  else
  {
    districtMsg = docCases[districtList[districtID]]["updated"].as<String>();
  }
}

void drawFloatNum(float num, int fmt, uint16_t x, uint16_t y)
{
  if (isnan(num))
  {
    tft.drawString("NaN", x, y);
  }
  else
  {
    tft.drawFloat(num, fmt, x, y);
  }
}

String clipText(const char *text, int maxWidth)
{
  int width = tft.textWidth(text);
  String strText = String(text);
  if (width > maxWidth)
  {
    while (width >= maxWidth)
    {
      int length = strText.length();
      strText = strText.substring(0, length - 1);
      width = tft.textWidth(strText + "...");
    }
    return strText + "...";
  }
  else
  {
    return strText;
  }
}