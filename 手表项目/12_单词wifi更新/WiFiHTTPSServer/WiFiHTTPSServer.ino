#include <ESP8266WiFi.h>

/*** 该工程可以在2.4.0版本esp8266库中运行，没在更高版本库中进行测试 ***/

const char *ssid = "xwbtd";
const char *password = "a12345678b.";

WiFiServer server(80);

String readString = ""; //建立一个字符串对象用来接收存放来自客户的数据

//响应头
String responseHeaders =
    String("") +
    "HTTP/1.1 200 OK\r\n" +
    "Content-Type: text/html\r\n" +
    "Connection: close\r\n" +
    "\r\n";

//网页
String myhtmlPage =
    String("") +
    "<html>" +
    "<head>" +
    "    <title>ESP8266 Web Server Test</title>" +
    "    <script defer=\"defer\">" +
    "        function ledSwitch() {" +
    "            var xmlhttp;" +
    "            if (window.XMLHttpRequest) {" +
    "                xmlhttp = new XMLHttpRequest();" +
    "            } else {" +
    "                xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");" +
    "            }" +
    "            xmlhttp.onreadystatechange = function () {" +
    "                if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {" +
    "                    document.getElementById(\"txtState\").innerHTML = xmlhttp.responseText;" +
    "                }" +
    "            }," +
    "            xmlhttp.open(\"GET\", \"Switch\", true);" +
    "            xmlhttp.send(); " +
    "        }" +
    "    </script>" +
    "</head>" +
    "<body>" +
    "    <div id=\"txtState\">Unkwon</div>" +
    "    <input type=\"button\" value=\"Switch\" οnclick=\"ledSwitch()\">" +
    "</body>" +
    "</html>";

bool isLedTurnOpen = false; // 记录LED状态

void setup()
{
    pinMode(D1, OUTPUT);
    digitalWrite(D1, HIGH); // 熄灭LED

    Serial.begin(9600);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected");

    server.begin();
    Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

void loop()
{
    WiFiClient client = server.available(); //尝试建立客户对象
    if (client)                             //如果当前有客户可用
    {
        boolean currentLineIsBlank = true;
        Serial.println("[Client connected]");

        while (client.connected()) //如果客户端建立连接
        {
            if (client.available()) //等待有可读数据
            {
                char c = client.read(); //读取一字节数据
                readString += c;        //拼接数据
                /************************************************/
                if (c == '\n' && currentLineIsBlank) //等待请求头接收完成(接收到空行)
                {
                    //比较接收到的请求数据
                    if (readString.startsWith("GET / HTTP/1.1")) //如果是网页请求
                    {
                        client.print(responseHeaders); //向客户端输出网页响应
                        client.print(myhtmlPage);      //向客户端输出网页内容
                        client.print("\r\n");
                    }
                    else if (readString.startsWith("GET /Switch")) //如果是改变LED状态请求
                    {
                        if (isLedTurnOpen == false)
                        {
                            digitalWrite(D1, LOW); // 点亮LED
                            client.print("LED has been turn on");
                            isLedTurnOpen = true;
                        }
                        else
                        {
                            digitalWrite(D1, HIGH); // 熄灭LED
                            client.print("LED has been turn off");
                            isLedTurnOpen = false;
                        }
                    }
                    else
                    {
                        client.print("\r\n");
                    }
                    break;
                }

                if (c == '\n')
                {
                    currentLineIsBlank = true; //开始新行
                }
                else if (c != '\r')
                {
                    currentLineIsBlank = false; //正在接收某行中
                }
                /************************************************/
            }
        }
        delay(1);      //等待客户完成接收
        client.stop(); //结束当前连接:
        Serial.println("[Client disconnected]");

        Serial.println(readString); //打印输出来自客户的数据
        readString = "";
    }
}
