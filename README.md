COVID19 Stats Display
=====================

Display COVID-19 stats on Wio Terminal.

## Installation
1. Change the `ssid` and `password` to your WiFi network.
   ```c++:src/main.cpp
   const char *ssid = "Your WiFi SSID";         // WiFi SSID
   const char *password = "Your WiFi Password"; // WiFi PASS
   ```
2. (Optional) Change the default country and default district.
   Country ID is shown in [the table below](#list-of-countries).
   Set `defaultDistrictID=0` to disable default district.
   To get district IDs, see serial monitor.
   ```c++:src/main.cpp
   int defaultCountryID = 18;  // Default: Japan
   int defaultDistrictID = 22; // Default district ID of default country.
   ```
3. (Optional) Change the refresh interval.
   ```c++:src/main.cpp
   const unsigned long REFRESH_INTERVAL_SUCCESS = 3600000; // 60min. When it succeeds to fetch the data.
   const unsigned long REFRESH_INTERVAL_FAILED = 300000;   //  5min. When it fails to fetch the data.
   ```
4. (Optional) Change the Time Zone.
   ```c++:src/main.cpp
   const int TIME_ZONE = 9; // Time zone
   ```
5. Upload to Wio Terminal

## Usage
### Switch the information displayed
Press `LEFT/RIGHT` (5way switch) to change the information displayed.

Wio Terminal can display
- COVID-19 cases summary of the country.
- COVID-19 cases summary of the district.
- COVID-19 Vaccination data of the country.

### Change country
1. Press `C` button.Then the list of countries will be displayed.
2. Choose the country.
   | Button                  | When Pressed              |
   | ----------------------- | ------------------------- |
   | `UP/DOWN` (5way switch) | Scroll list of countries. |
   | `PRESS` (5way switch)   | Select the countries.     |
   | `C` (upper side button) | Cancel.                   |

### Change district
1. Press `B` button.Then the list ofdistricts will be displayed.
2. Choose the district.
   | Button                  | When Pressed              |
   | ----------------------- | ------------------------- |
   | `UP/DOWN` (5way switch) | Scroll list of districts. |
   | `PRESS` (5way switch)   | Select the districts.     |
   | `B` (upper side button) | Cancel.                   |

### Force refresh
Press `A` button.

### When unable to connect the WiFi or server
Reset Wio Terminal.
Almost all errors are solved by reset.

## Data Sources
Fetch the information from
- [M-Media-Group/Covid-19-API](https://github.com/M-Media-Group/Covid-19-API)
- [UN COVID-19 Data Hub Cases Country](https://covid-19-data.unstatshub.org/datasets/1cb306b5331945548745a5ccd290188e_2).

## List of Countries
| ID  | Country        |
| --- | -------------- |
| 0   | Australia      |
| 1   | Belgium        |
| 2   | Brazil         |
| 3   | Canada         |
| 4   | Chile          |
| 5   | China          |
| 6   | Denmark        |
| 7   | Egypt          |
| 8   | Finland        |
| 9   | France         |
| 10  | Germany        |
| 11  | India          |
| 12  | Indonesia      |
| 13  | Iran           |
| 14  | Iraq           |
| 15  | Ireland        |
| 16  | Israel         |
| 17  | Italy          |
| 18  | Japan          |
| 19  | Malaysia       |
| 20  | Mexico         |
| 21  | Nepal          |
| 22  | Netherlands    |
| 23  | New Zealand    |
| 24  | Norway         |
| 25  | Pakistan       |
| 26  | Peru           |
| 27  | Philippines    |
| 28  | Poland         |
| 29  | Portugal       |
| 30  | Russia         |
| 31  | Saudi Arabia   |
| 32  | Singapore      |
| 33  | South Africa   |
| 34  | South Korea    |
| 35  | Spain          |
| 36  | Sweden         |
| 37  | Switzerland    |
| 38  | Taiwan         |
| 39  | Thailand       |
| 40  | Turkey         |
| 41  | United Kingdom |
| 42  | United States  |
| 43  | Vietnam        |
| 44  | World          |

## License
Copyright (c) 2021 Takayuki YANO

The source code is licensed under the MIT License, see LICENSE.
