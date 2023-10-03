#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

const byte numChars = 32;
//bool for checking input
boolean booleanCheck = false;


//wifi variables
char SSID[numChars];
char password[numChars];


//Email variables
#define AUTHOR_EMAIL "d1minipro@gmail.com"
#define AUTHOR_PASSWORD "ibnvtpmpiyansois"

char SMTP_HOST[numChars];
int SMTP_PORT;

char RECIPIENT_NAME[numChars];
char RECIPIENT_EMAIL[numChars];
char MAIL_TEXT[numChars];
char MAIL_SUBJECT[numChars];



SMTPSession smtp;
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
  Serial.begin(9600);
}


void loop() {
  Menu();
}

void Menu() {

  Serial.println();
  Serial.println(("1. Wifi Konfiguration"));
  Serial.println(("2. Email Konfiguration"));

  delay(200);
  while (Serial.available() == 0) {
    int menuChoice = Serial.parseInt();

    switch (menuChoice) {
      case 1:
        //Wifi config


        WifiConfig();
        break;

      case 2:

        EmailConfig();
        break;

      default:
        continue;
    }
  }
}

//(Use UserInput() when working)
void WifiConfig() {



  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("Du er allerede tilsluttet til internettet");
    Serial.println();
    Serial.println("1. for at logge på nyt internet");
    Serial.println("2. Tilbage");

    while (Serial.available() == 0) {
      int interChoice = Serial.parseInt();

      switch (interChoice) {
        case 1:
          WiFi.disconnect();
          break;

        case 2:
          Menu();
      }
    }
  }
  Serial.println("Skriv SSID og Adgangskode til wifi for at logge ind");
  Serial.println();

  //UserInput()
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  Serial.println("SSID: ");

  while (booleanCheck == false) {


    if (Serial.available() > 0) {
      rc = Serial.read();

      if (rc != endMarker) {
        SSID[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        SSID[ndx] = '\0';  // terminate the string

        ndx = 0;
        booleanCheck = true;
      }
    }
  }
  delay(500);
  if (booleanCheck == true) {
    Serial.println(SSID);
  }

  booleanCheck = false;


  //password
  //UserInput()
  static byte ndxpas = 0;
  char endMarkerpas = '\n';
  char rcpas;
  Serial.println("password: ");

  while (booleanCheck == false) {


    if (Serial.available() > 0) {
      rcpas = Serial.read();

      if (rcpas != endMarkerpas) {
        password[ndxpas] = rcpas;
        ndxpas++;
        if (ndxpas >= numChars) {
          ndxpas = numChars - 1;
        }
      } else {
        password[ndxpas] = '\0';  // terminate the string

        ndxpas = 0;
        booleanCheck = true;
      }
    }
  }
  delay(500);
  if (booleanCheck == true) {
    Serial.println(password);
  }

  booleanCheck = false;


  //connecting to wifi with written SSID and pass
  Serial.println();
  WiFi.begin(SSID, password);
  Serial.print("Forbinder til Wi-Fi");
  for (int i = 0; i < 20; i++) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Arduino er tilsluttet til internettet");
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Menu();

  } else {
    Serial.print("Kan ikke tilslutte arduino til internettet. prøv igen");

    SSID[32] = '\0';      // terminate the string
    password[32] = '\0';  // terminate the string


    Menu();
  }
  resetFunc();
}

//email
void EmailConfig() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println();
    Serial.println("Du er ikke tilsluttet til internettet, gå tilbage for at logge på internettet");
    Serial.println();
    Serial.println("1. forsæt alligevel");
    Serial.println("2. Tilbage");

    while (Serial.available() == 0) {
      int interChoice = Serial.parseInt();

      switch (interChoice) {
        case 1:
          break;
        case 2:
          Menu();
      }
    }
  }

  
  Serial.println();
  Serial.println("Indtast Afsender Email:");
 //UserInput()
  static byte ndxAE = 0;
  char endMarkerAE = '\n';
  char rcAE;
  while (booleanCheck == false) {


    if (Serial.available() > 0) {
      rcAE = Serial.read();

      if (rcAE != endMarkerAE) {
        RECIPIENT_EMAIL[ndxAE] = rcAE;
        ndxAE++;
        if (ndxAE >= numChars) {
          ndxAE = numChars - 1;
        }
      } else {
        RECIPIENT_EMAIL[ndxAE] = '\0';  // terminate the string

        ndxAE = 0;
        booleanCheck = true;
      }
    }
  }
  delay(500);
  if (booleanCheck == true) {
    Serial.println(RECIPIENT_EMAIL);
  }

  booleanCheck = false;
  
  Serial.println();
  Serial.println("Indtast Afsenderens Navn:");
 //UserInput()
  static byte ndxAN = 0;
  char endMarkerAN = '\n';
  char rcAN;

  while (booleanCheck == false) {


    if (Serial.available() > 0) {
      rcAN = Serial.read();

      if (rcAN != endMarkerAN) {
        RECIPIENT_NAME[ndxAN] = rcAN;
        ndxAN++;
        if (ndxAN >= numChars) {
          ndxAN = numChars - 1;
        }
      } else {
        RECIPIENT_NAME[ndxAN] = '\0';  // terminate the string

        ndxAN = 0;
        booleanCheck = true;
      }
    }
  }
  delay(500);
  if (booleanCheck == true) {
    Serial.println(RECIPIENT_NAME);
  }

  booleanCheck = false;

  Serial.println("Indtast SMTP Server:");
   //UserInput()
  static byte ndxSMTP = 0;
  char endMarkerSMTP = '\n';
  char rcSMTP;

  while (booleanCheck == false) {


    if (Serial.available() > 0) {
      rcSMTP = Serial.read();

      if (rcSMTP != endMarkerSMTP) {
        SMTP_HOST[ndxSMTP] = rcSMTP;
        ndxSMTP++;
        if (ndxSMTP >= numChars) {
          ndxSMTP = numChars - 1;
        }
      } else {
        SMTP_HOST[ndxSMTP] = '\0';  // terminate the string

        ndxSMTP = 0;
        booleanCheck = true;
      }
    }
  }
  delay(500);
  if (booleanCheck == true) {
    Serial.println(SMTP_HOST);
  }

  booleanCheck = false;

  Serial.println("SMTP port:");
  while (Serial.available() == 0) {
    int interChoice = Serial.parseInt();
    SMTP_PORT = interChoice;

  }
  Serial.println(SMTP_PORT);
  
  //mail text og emne
   //UserInput()
  Serial.println("Indtast Emne (på mailen):");
  static byte ndxSBJ = 0;
  char endMarkerSBJ = '\n';
  char rcSBJ;

  while (booleanCheck == false) {


    if (Serial.available() > 0) {
      rcSBJ = Serial.read();

      if (rcSBJ != endMarkerSBJ) {
        MAIL_SUBJECT[ndxSBJ] = rcSBJ;
        ndxSBJ++;
        if (ndxSBJ >= numChars) {
          ndxSBJ = numChars - 1;
        }
      } else {
        MAIL_SUBJECT[ndxSBJ] = '\0';  // terminate the string

        ndxSBJ = 0;
        booleanCheck = true;
      }
    }
  }
  delay(500);
  if (booleanCheck == true) {
    Serial.println(MAIL_SUBJECT);
  }

  booleanCheck = false;

  Serial.println("Indtast Besked(mail):");
   //UserInput()
  static byte ndxTX = 0;
  char endMarkerTX = '\n';
  char rcTX;

  while (booleanCheck == false) {


    if (Serial.available() > 0) {
      rcTX = Serial.read();

      if (rcTX != endMarkerTX) {
        MAIL_TEXT[ndxTX] = rcTX;
        ndxTX++;
        if (ndxTX >= numChars) {
          ndxTX = numChars - 1;
        }
      } else {
        MAIL_TEXT[ndxTX] = '\0';  // terminate the string

        ndxTX = 0;
        booleanCheck = true;
      }
    }
  }
  delay(500);
  if (booleanCheck == true) {
    Serial.println(MAIL_TEXT);
  }

  booleanCheck = false;
  
  SendEmail();

  resetFunc();
  
}

void SendEmail() {
  /*  Set the network reconnection option */
  MailClient.networkReconnect(true);

  /** Enable the debug via Serial port
   * 0 for no debugging
   * 1 for basic level debugging
   *
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
   */
  smtp.debug(0);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;

  /* Set the session config */
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";

  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 2;
  config.time.day_light_offset = 2;

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("Peter");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = MAIL_SUBJECT;
  message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);

  // /*Send HTML message*/
  // String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
  // message.html.content = htmlMsg.c_str();
  // message.html.content = htmlMsg.c_str();
  // message.text.charSet = "us-ascii";
  // message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  String mailReturnText = String(MAIL_TEXT);
  //Send raw text message
  message.text.content = mailReturnText.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


  /* Connect to the server */
  if (!smtp.connect(&config)) {
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn()) {
    Serial.println("\nNot yet logged in.");
  } else {
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
}



/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status) {
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()) {
    // ESP_MAIL_PRINTF used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. AVR, SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++) {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}

// hmm
// char UserInput[32](char Inp[32]) {
//   static byte anNdx = 0;
//   char anEndMarker = '\n';
//   char anRc;

//   while (booleanCheck == false) {

//     if (Serial.available() > 0) {
//       anRc = Serial.read();

//       if (anRc != anEndMarker) {
//         Inp[anNdx] = anRc;
//         anNdx++;
//         if (anNdx >= numChars) {
//           anNdx = numChars - 1;
//         }
//       } else {
//         Inp[anNdx] = '\0';  // terminate the string

//         anNdx = 0;
//         booleanCheck = true;
//       }
//     }
//   }
//   delay(500);
//   if (booleanCheck == true) {
//     Serial.println(Inp);
//   }

//   booleanCheck = false;
//   return Inp;
// }
