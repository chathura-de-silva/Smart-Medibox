#include <WiFi.h>
#include <Globals.h>
#include <Constants.h>
#include <Functions.h>

String wifi_username = DEFAULT_WIFI_SSID;
String wifi_password = DEFAULT_WIFI_PASSWORD;

String urlDecode(String input); // declaring two helper function which don't need to be called outside this file.
unsigned char hexToDec(char c);

void config_wifi()
{
    show_modal_page(wifi_config, 1500, "Wifi Config Mode", 16);
    // Set web server port number to 80
    WiFiServer server(80);

    // Variable to store the HTTP request
    String header;
    // Connect to Wi-Fi network with SSID and password
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    show_modal_page(wifi_config, -1, "Visit " + IP.toString(), 12); // This is a modal page. It will be displayed until the wifi credentials are entered.note that the period is given a negative number. this avoids delay in the modal page. Hence stays as it is until display.clear() is called somewhere.

    server.begin();
    wifi_username = "";
    wifi_password = "";

    while (wifi_username == "" || wifi_password == "")
    {
        WiFiClient client = server.available(); // Listen for incoming clients

        if (client)
        {                                  // If a new client connects,
            Serial.println("New Client."); // print a message out in the serial port
            String currentLine = "";       // make a String to hold incoming data from the client
            while (client.connected())
            { // loop while the client's connected
                if (client.available())
                {                           // if there's bytes to read from the client,
                    char c = client.read(); // read a byte, then
                    Serial.write(c);        // print it out the serial monitor
                    header += c;
                    if (c == '\n')
                    { // if the byte is a newline character
                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0)
                        {
                            // Note that HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();

                            // Check if the request contains username and password
                            if (header.indexOf("GET /login") >= 0)
                            {
                                // Parse username and password from the request
                                int usernameIndex = header.indexOf("username=");
                                int passwordIndex = header.indexOf("&password=");
                                if (usernameIndex >= 0 && passwordIndex >= 0)
                                {
                                    wifi_username = urlDecode(header.substring(usernameIndex + 9, passwordIndex));
                                    // Find end of password
                                    int passwordEndIndex = header.indexOf("HTTP/1", passwordIndex);
                                    if (passwordEndIndex >= 0)
                                    {
                                        wifi_password = urlDecode(header.substring(passwordIndex + 10, passwordEndIndex - 1));
                                    }
                                }
                            }
                            
                            // Display the HTML web page with login form
                            client.println("<!DOCTYPE html><html>");
                            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                            client.println("<link rel=\"icon\" href=\"data:,\">");
                            client.println("<style>");
                            client.println("body {");
                            client.println("    font-family: Arial, sans-serif;");
                            client.println("    background-color: #000;");
                            client.println("    color: #fff;");
                            client.println("    margin: 0;");
                            client.println("    padding: 0;");
                            client.println("    display: flex;");
                            client.println("    justify-content: center;");
                            client.println("    align-items: center;");
                            client.println("    height: 100vh;");
                            client.println("}");
                            client.println(".container {");
                            client.println("    background-color: #111;");
                            client.println("    border-radius: 10px;");
                            client.println("    padding: 20px;");
                            client.println("    box-shadow: 0 0 10px rgba(255, 255, 255, 0.1);");
                            client.println("    width: 300px;");
                            client.println("}");
                            client.println("h1 {");
                            client.println("    text-align: center;");
                            client.println("    margin-bottom: 20px;");
                            client.println("}");
                            client.println("input[type=\"text\"], input[type=\"password\"] {");
                            client.println("    width: 100%;");
                            client.println("    padding: 10px;");
                            client.println("    margin-bottom: 10px;");
                            client.println("    border: 1px solid #666;");
                            client.println("    border-radius: 5px;");
                            client.println("    box-sizing: border-box;");
                            client.println("    background-color: #222;");
                            client.println("    color: #fff;");
                            client.println("}");
                            client.println("input[type=\"submit\"] {");
                            client.println("    width: 100%;");
                            client.println("    padding: 10px;");
                            client.println("    border: none;");
                            client.println("    border-radius: 5px;");
                            client.println("    background-color: #fff;");
                            client.println("    color: #000;");
                            client.println("    cursor: pointer;");
                            client.println("}");
                            client.println("input[type=\"submit\"]:hover {");
                            client.println("    background-color: linear-gradient(to bottom right, #ffcccc, #ffffff);");
                            client.println("}");
                            client.println("</style></head>");

                            client.println("<body>");
                            client.println("<div class=\"container\">");
                            client.println("<h1>Smart Medibox</h1>");
                            // Display login form
                            client.println("<form action=\"/login\" method=\"get\">");
                            client.println("Wifi SSID: <input type=\"text\" name=\"username\"><br>");
                            client.println("Password: <input type=\"password\" name=\"password\"><br>");
                            client.println("<input type=\"submit\" value=\"Save\">");
                            client.println("</form>");
                            client.println("</div>");
                            client.println("</body></html>");

                            // The HTTP response ends with another blank line
                            client.println();

                            // Break out of the while loop
                            show_modal_page(wifi_config, -1, "Waiting for Data...", 12);
                            break;
                        }
                        else
                        { // if you got a newline, then clear currentLine
                            currentLine = "";
                        }
                    }
                    else if (c != '\r')
                    {                     // if you got anything else but a carriage return character,
                        currentLine += c; // add it to the end of the currentLine
                    }
                }
            }
            // Clear the header variable
            header = "";
            // Close the connection
            client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
        }
    }
    save_wifi_credentials(wifi_username, wifi_password);
    Serial.println("User: " + wifi_username + "     Password: " + wifi_password + ".");
}

String urlDecode(String input)
{ // This function is used to decode the URL encoded characters.
    String decoded = "";
    char c;
    char code0;
    char code1;
    for (size_t i = 0; i < input.length(); i++)
    {
        c = input.charAt(i);
        if (c == '+')
        {
            decoded += ' ';
        }
        else if (c == '%')
        {
            i++;
            code0 = input.charAt(i);
            i++;
            code1 = input.charAt(i);
            c = (hexToDec(code0) << 4) | hexToDec(code1);
            decoded += c;
        }
        else
        {
            decoded += c;
        }
    }
    return decoded;
}

unsigned char hexToDec(char c)
{
    if (c >= '0' && c <= '9')
    {
        return (unsigned char)(c - '0');
    }
    if (c >= 'A' && c <= 'F')
    {
        return (unsigned char)(c - 'A' + 10);
    }
    if (c >= 'a' && c <= 'f')
    {
        return (unsigned char)(c - 'a' + 10);
    }
    return 0;
}
