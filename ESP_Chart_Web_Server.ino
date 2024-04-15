#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Redmi-Note-10S";
const char* password = "jojo1234";

const int mq = 34;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readMQ2data() {
  // Read temperature as Celsius (the default)
  float t = analogRead(mq);
  // Convert temperature to Fahrenheit
  //t = 1.8 * t + 32;
  if (isnan(t)) {    
    Serial.println("Failed to read from MQ2 sensor!");
    return "";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>DashBoard</title>
    <style>
      @import url("https://fonts.googleapis.com/css2?family=Ubuntu:wght@300;400;500;700&display=swap");
      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
        font-family: "Ubuntu", sans-serif;
      }
      :root {
        --blue: #287bff;
        --white: #fff;
        --grey: #f5f5f5;
        --black1: #222;
        --black2: #999;
      }
      body {
        min-height: 100vh;
        overflow-x: hidden;
      }
      .container {
        position: relative;
        width: 100%;
      }
      .navigation {
        position: fixed;
        width: 300px;
        height: 100%;
        background: var(--blue);
        border-left: 10px solid var(--blue);
        transition: 0.5s;
        overflow: hidden;
      }
      .navigation.active {
        width: 80px;
      }
      .navigation ul {
        position: absolute;
        top: 0;
        left: 0;
        width: 100%;
      }
      .navigation ul li {
        position: relative;
        width: 100%;
        list-style: none;
        border-left: none;
        border-top-left-radius: 30px;
        border-bottom-left-radius: 30px;
      }
      .navigation ul li:hover,
      .navigation ul li.hovered {
        background: var(--white);
      }
      .navigation ul li:nth-child(1) {
        margin-bottom: 40px;
        pointer-events: none;
      }
      .navigation ul li a {
        position: relative;
        display: block;
        width: 100%;
        display: flex;
        text-decoration: none;
        color: var(--white);
      }
      .navigation ul li:hover a,
      .navigation ul li.hovered a {
        color: var(--blue);
      }
      .navigation ul li a .icon {
        position: relative;
        display: block;
        min-width: 60px;
        height: 60px;
        line-height: 70px;
        text-align: center;
      }
      .navigation ul li a .icon ion-icon {
        font-size: 1.75em;
      }
      .navigation ul li a .title {
        position: relative;
        display: block;
        padding: 0 10px;
        height: 60px;
        line-height: 60px;
        text-align: start;
        white-space: nowrap;
      }
      .navigation ul li:hover a::before,
      .navigation ul li.hovered a::before {
        content: "";
        position: absolute;
        right: 0;
        top: -50px;
        width: 50px;
        height: 50px;
        background: transparent;
        border-radius: 50%;
        box-shadow: 35px 35px 0 10px var(--white);
        pointer-events: none;
      }

      .navigation ul li:hover a::after,
      .navigation ul li.hovered a::after {
        content: "";
        position: absolute;
        right: 0;
        bottom: -50px;
        width: 50px;
        height: 50px;
        background: transparent;
        border-radius: 50%;
        box-shadow: 35px -35px 0 10px var(--white);
        pointer-events: none;
      }
      /* main */

      .main {
        position: absolute;
        width: calc(100% - 300px);
        left: 300px;
        min-height: 100vh;
        background: var(-- white);
        transition: 0.5s;
      }
      .main.active {
        width: calc(100% -80px);
        left: 80px;
      }

      .topbar {
        width: 100%;
        height: 80px;
        display: flex;
        justify-content: space-between;
        align-items: center;
        padding: 0 10px;
      }
      .toggle {
        position: relative;
        width: 60px;
        height: 60px;
        display: flex;
        justify-content: center;
        align-items: center;
        font-size: 2.5em;
        cursor: pointer;
      }

      .search {
        position: relative;
        width: 400px;
        margin: 0 10px;
      }
      .search label {
        position: relative;
        width: 100%;
      }
      .search label input {
        width: 100%;
        height: 40px;
        border-radius: 40px;
        padding: 5px 20px;
        padding-left: 35px;
        font-size: 18px;
        outline: none;
        border: 1px solid var(--black2);
      }

      .search label ion-icon {
        position: absolute;
        top: 0;
        left: 10px;
        font-size: 1.2em;
      }
      .user {
        position: relative;
        width: 40px;
        height: 40px;
        border-radius: 50%;
        overflow: hidden;
        cursor: pointer;
      }
      .user img {
        position: absolute;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        object-fit: cover;
      }
      .cardBox {
        position: relative;
        width: 100%;
        padding: 20px;
        display: grid;
        grid-template-columns: repeat(4, 1fr);
        grid-gap: 30px;
      }

      .cardBox .card {
        position: relative;
        background: var(--white);
        padding: 30px;
        border-radius: 20px;
        display: flex;
        justify-content: space-between;
        cursor: pointer;
        box-shadow: 0 7px 25px rgba(0, 0, 0, 0.08);
      }

      .cardBox .card .numbers {
        position: relative;
        font-weight: 500;
        font-size: 2.5em;
        color: var(--blue);
      }

      .cardBox .card .cardName {
        color: var(--blue);
      }

      .card .cardName {
        color: var(--black2);
        font-size: 1.1em;
        margin-top: 5px;
      }

      .cardBox .card .iconBx {
        font-size: 3.5em;
        color: var(--black2);
      }
      .cardBox .card:hover {
        background: var(--blue);
      }

      .cardBox .card:hover .numbers,
      .cardBox .card:hover .cardName,
      .cardBox .card:hover .iconBx {
        color: var(--white);
      }

      /* GRAPH */
      .graphBox {
        position: relative;
        width: 100%;
        padding: 20px;
        display: grid;
        grid-template-columns: 1.5fr 1.5fr;
        grid-gap: 30px;
        min-height: 550px;
      }
      .graphBox .box {
        position: relative;
        background: #fff;
        padding: 20px;
        width: 100%;
        box-shadow: 0 7px 25px rgba(0, 0, 0, 0.08);
        border-radius: 20px;
      }

      /* DETAILS */

      .details {
        position: relative;
        width: 100%;
        padding: 20px;
        display: grid;
        grid-template-columns: repeat(2, 1fr);
        grid-gap: 30px;
        margin-top: 10px;
      }
      .details .recentOrders {
        position: relative;
        display: grid;
        min-height: 50px;
        background: var(--white);
        padding: 20px;
        box-shadow: 0 7px 25px rgba(0, 0, 0, 0.08);
        border-radius: 20px;
      }
      .cardHeader {
        display: flex;
        justify-content: space-between;
        align-items: flex-start;
      }

      .cardHeader h2 {
        font-weight: 600;
        color: var(--blue);
      }
      .btn {
        position: relative;
        padding: 5px 10px;
        background: var(--blue);
        text-decoration: none;
        color: var(--white);
        border-radius: 6px;
      }

      .details table {
        width: 100%;
        border-collapse: collapse;
        margin-top: 10px;
      }

      .details table thead td {
        font-weight: 600;
      }

      .details .recentOrders table tr {
        color: var(--black1);
        border-bottom: 1px solid rgba(0, 0, 0, 0.1);
      }

      .details .recentOrders table tr:last-child {
        border-bottom: none;
      }

      .details .recentOrders table tbody tr:hover {
        background: var(--blue);
        color: var(--white);
      }
      .details .recentOrders table tr td {
        padding: 10px;
      }
      .details .recentOrders table tr td:last-child {
        text-align: end;
      }
      .details .recentOrders table tr td:nth-child(2) {
        text-align: end;
      }
      .details .recentOrders table tr td:nth-child(3) {
        text-align: center;
      }

      .status.delivered {
        padding: 2px 4px;
        background: #8de02c;
        color: var(-- white);
        border-radius: 4px;
        font-size: 14px;
        font-weight: 500;
      }
      .status.pending {
        padding: 2px 4px;
        background: #f9ca3f;
        color: var(-- white);
        border-radius: 4px;
        font-size: 14px;
        font-weight: 500;
      }
      .status.return {
        padding: 2px 4px;
        background: #f00;
        color: var(-- white);
        border-radius: 4px;
        font-size: 14px;
        font-weight: 500;
      }
      .status.inprogress {
        padding: 2px 4px;
        background: #1795ce;
        color: var(-- white);
        border-radius: 4px;
        font-size: 14px;
        font-weight: 500;
      }
      .recentCustomers {
        position: relative;
        display: grid;
        min-height: 500px;
        padding: 20px;
        background: var(-- white);
        box-shadow: 0 7px 25px rgba(0, 0, 0, 0.08);
        border-radius: 20px;
      }
      .recentCustomers .imgBx {
        position: relative;
        width: 40px;
        height: 40px;
        border-radius: 50%;
        overflow: hidden;
      }
      .recentCustomers .imgBx img {
        position: absolute;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        object-fit: cover;
      }
      .recentCustomers table tr:hover {
        background: var(--blue);
        color: var(--white);
      }
      .recentCustomers table tr td {
        padding: 12px 10px;
      }
      .recentCustomers table tr td h4 {
        font-size: 16px;
        font-weight: 500;
        line-height: 1.2em;
      }
      .recentCustomers table tr td h4 span {
        font-size: 14px;
        color: var(--black2);
      }
      .recentCustomers table tr:hover {
        background: var(--blue);
        color: var(--white);
      }
      .recentCustomers table tr:hover td h4 span {
        color: var(--white);
      }
    </style>
  </head>
  <body>
    <div class="container">
      <div class="navigation">
        <ul>
          <li>
            <a href="#">
              <span class="icon"><ion-icon name="globe"></ion-icon></span>
              <span class="title">Carbon Footprint Sensor</span>
            </a>
          </li>
          <li>
            <a href="https://akashtripathi12.github.io/home/">
              <span class="icon"
                ><ion-icon name="home-outline"></ion-icon
              ></span>
              <span class="title">Home</span>
            </a>
          </li>
          <li>
            <a href="https://akashtripathi12.github.io/customers/">
              <span class="icon"
                ><ion-icon name="people-outline"></ion-icon
              ></span>
              <span class="title">Customers</span>
            </a>
          </li>
          <li>
            <a href="#">
              <span class="icon"
                ><ion-icon name="help-outline"></ion-icon
              ></span>
              <span class="title"> Help</span>
            </a>
          </li>
          <li>
            <a href="#">
              <span class="icon"
                ><ion-icon name="settings-outline"></ion-icon
              ></span>
              <span class="title">Settings</span>
            </a>
          </li>
          <li>
            <a href="https://akashtripathi12.github.io/season/">
              <span class="icon"
                ><ion-icon name="link-outline"></ion-icon
              ></span>
              <span class="title">More</span>
            </a>
          </li>
          <li>
            <a href="https://akashtripathi12.github.io/logout/">
              <span class="icon"
                ><ion-icon name="log-out-outline"></ion-icon
              ></span>
              <span class="title">Sign Out</span>
            </a>
          </li>
        </ul>
      </div>
      <div class="main">
        <div class="topbar">
          <div class="toggle">
            <ion-icon name="menu-outline"></ion-icon>
          </div>
          <div class="search">
            <label>
              <input type="text" placeholder="Search here" />
              <ion-icon name="search-outline"></ion-icon>
            </label>
          </div>
        </div>
        <!--cards-->
        <div class="cardBox">
          <div class="card">
            <div>
              <div class="numbers">1,504</div>
              <div class="cardName">Daily Views</div>
            </div>
            <div class="iconBx"><ion-icon name="eye-outline"></ion-icon></div>
          </div>
          <div class="card">
            <div>
              <div class="numbers">80</div>
              <div class="cardName">Sales</div>
            </div>
            <div class="iconBx"><ion-icon name="cart-outline"></ion-icon></div>
          </div>
          <div class="card">
            <div>
              <div class="numbers">284</div>
              <div class="cardName">Comments</div>
            </div>
            <div class="iconBx">
              <ion-icon name="chatbubbles-outline"></ion-icon>
            </div>
          </div>
          <div class="card">
            <div>
              <div class="numbers">₹9,834</div>
              <div class="cardName">Earning</div>
            </div>
            <div class="iconBx"><ion-icon name="cash-outline"></ion-icon></div>
          </div>
        </div>

        <!-- add charts-->
        <div class="graphBox">
          <div id="sensor1" class="box"></div>
          <div class="box">
            <canvas id="myChart"></canvas>
          </div>
        </div>

        <!--data list-->
        <div class="details">
          <div class="recentOrders">
            <div class="cardHeader">
              <h2>Recent Orders</h2>
              <a href="#" class="btn">View All</a>
            </div>
            <table>
              <thead>
                <tr>
                  <td>Name</td>
                  <td>Price</td>
                  <td>Payment</td>
                  <td>Status</td>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td>MQ2</td>
                  <td>₹1000</td>
                  <td>Paid</td>
                  <td><span class="status delivered">Delivered</span></td>
                </tr>
                <tr>
                  <td>ESP32</td>
                  <td>₹200</td>
                  <td>Paid</td>
                  <td><span class="status inprogress">In Progress</span></td>
                </tr>
                <tr>
                  <td>ESP8266</td>
                  <td>₹500</td>
                  <td>Paid</td>
                  <td><span class="status return">Return</span></td>
                </tr>
                <tr>
                  <td>Arduino UNO</td>
                  <td>₹900</td>
                  <td>Paid</td>
                  <td><span class="status pending">Pending</span></td>
                </tr>
                <tr>
                  <td>UNO cable</td>
                  <td>₹100</td>
                  <td>Paid</td>
                  <td><span class="status delivered">Delivered</span></td>
                </tr>
                <tr>
                  <td>Arduino NANO</td>
                  <td>₹2200</td>
                  <td>Paid</td>
                  <td><span class="status delivered">Delivered</span></td>
                </tr>
              </tbody>
            </table>
          </div>
          <!-- New Customers -->
          <div class="recentCustomers">
            <div class="cardHeader">
              <h2>Recent Customers and Reviews</h2>
            </div>
            <table>
              <tr>
                <td width="60px">
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                </td>
                <td>
                  <h4>David- Good<br /><span>Italy</span></h4>
                </td>
              </tr>
              <tr>
                <td>
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                  <ion-icon name="star-half"></ion-icon>
                </td>
                <td>
                  <h4>David- Fabulous<br /><span>Italy</span></h4>
                </td>
              </tr>
              <tr>
                <td>
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                  <ion-icon name="star-half"></ion-icon>
                </td>
                <td>
                  <h4>David- Great!!<br /><span>Italy</span></h4>
                </td>
              </tr>
              <tr>
                <td>
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                  <ion-icon name="star-half"></ion-icon>
                </td>
                <td>
                  <h4>David- unbelievable <br /><span>Italy</span></h4>
                </td>
              </tr>
              <tr>
                <td>
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                  <ion-icon name="star-half"></ion-icon>
                </td>
                <td>
                  <h4>David- Fabulous<br /><span>Italy</span></h4>
                </td>
              </tr>
              <tr>
                <td>
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                </td>

                <td>
                  <h4>David-Cool<br /><span>Italy</span></h4>
                </td>
              </tr>
              <tr>
                <td>
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                  <ion-icon name="star"></ion-icon>
                </td>
                <td>
                  <h4>David-Damnn<br /><span>Italy</span></h4>
                </td>
              </tr>
              <tr>
                <td>
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                </td>
                <td>
                  <h4>David-Nice<br /><span>Italy</span></h4>
                </td>
              </tr>
              <tr>
                <td>
                  <ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon
                  ><ion-icon name="star"></ion-icon>
                </td>
                <td>
                  <h4>David- Superb<br /><span>Italy</span></h4>
                </td>
              </tr>
            </table>
          </div>
        </div>
      </div>
    </div>
    <script
      type="module"
      src="https://unpkg.com/ionicons@7.1.0/dist/ionicons/ionicons.esm.js"
    ></script>
    <script
      nomodule
      src="https://unpkg.com/ionicons@7.1.0/dist/ionicons/ionicons.js"
    ></script>
    <script src="https://code.highcharts.com/highcharts.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script>
      var chartT = new Highcharts.Chart({
        chart: { renderTo: "sensor1" },
        title: { text: "MQ 2 sensor data" },
        series: [
          {
            showInLegend: false,
            data: [],
          },
        ],
        plotOptions: {
          line: { animation: false, dataLabels: { enabled: true } },
          series: { color: "#059e8a" },
        },
        xAxis: {
          title: { text: "Date & Time" },
          type: "datetime",
          dateTimeLabelFormats: { second: "%H:%M:%S" },
        },
        yAxis: {
          title: { text: "Sensor Reading (in ppm)" },
        },
        credits: { enabled: false },
      });
      setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            var x = new Date().getTime(),
              y = parseFloat(this.responseText);
            if (chartT.series[0].data.length > 40) {
              chartT.series[0].addPoint([x, y], true, true, true);
            } else {
              chartT.series[0].addPoint([x, y], true, false, true);
            }
          }
        };
        xhttp.open("GET", "/sensor", true);
        xhttp.send();
      }, 1000);
      const ctx = document.getElementById("myChart").getContext("2d");

      var chartP = new Chart(ctx, {
        type: "polarArea",
        data: {
          labels: [">1k", ">3k", "<900", "<500", ">2k"],
          datasets: [
            {
              label: "Sensor Value",
              data: [
                549, 517, 2400, 1166, 1901, 1474, 622, 2570, 2611, 1140, 13,
                914, 1060, 1241, 1819, 284, 2850, 1322, 2336, 1435, 549, 517,
                2400, 1166, 101, 174, 3622, 2570, 2611, 1140, 13, 914, 1060,
                1241, 1819, 384, 2850, 1322, 2336, 1435, 549, 517, 2400, 1166,
                1901, 1474, 622, 2570, 2611, 1140, 13, 914, 1060, 1241, 1819,
                284, 2850, 1322, 2336, 1435, 549, 517, 2400, 1166, 1901, 1474,
                622, 2570, 2611, 1140, 13, 914, 1060, 1241, 1819, 284, 2850,
                1322, 2336, 1435, 549, 517, 2400, 1166, 1901, 1474, 622, 2570,
                2611, 1140, 13, 914, 1060, 1221, 1819, 284, 2850, 1322, 2336,
                1435, 549, 517, 2400, 1166, 1901, 1474, 622, 2570, 2611, 1140,
                13, 914, 1060, 1241, 1819, 284, 2850, 1322, 2336, 1435, 549,
                517, 2400, 1166, 1901, 1474, 632, 2570, 2611, 1140, 13, 914,
                1060, 1241, 1819, 284, 280, 322, 2336, 1435, 549, 517, 2400,
                1166, 1901, 1474, 622, 2570, 211, 1140, 13, 914, 1060, 1241,
                1819, 284, 2850, 1322, 2336, 1435, 549, 517, 2400, 1166, 1901,
                1474, 622, 2570, 2611, 140, 13, 914, 1060, 1241, 1819, 284,
                2850, 1322, 2336, 1435, 1549, 517, 2400, 1166, 1901, 1474, 622,
                2570, 2611, 1140, 13, 914, 1060, 1241, 1819, 284, 2850, 1322,
                2336, 1435, 549, 517, 2400, 1166, 1901, 1474, 622, 2570, 2611,
                1140, 13, 914, 1060, 1241, 1819, 284, 2850, 1322, 2336, 1435,
                549, 517, 2400, 1166, 1901, 1474, 622, 2570, 2611, 1140, 13,
                914, 1060, 1241, 1819, 284, 2850, 1322, 2336, 1435,
              ],
              backgroundColor: [
                "rgba(255,99,132,1)",
                "rgba(54,162,235,1)",
                "rgba(255,206,86,1)",
                "rgba(75,192,192,1)",
                "rgba(153,102,255,1)",
              ],
              borderWidth: 1,
            },
          ],
        },
      });
    </script>
    <script>
      // Menu Toggle
      let toggle = document.querySelector(".toggle");
      let navigation = document.querySelector(".navigation");
      let main = document.querySelector(".main");
      toggle.onclick = function () {
        navigation.classList.toggle("active");
        main.classList.toggle("active");
      };
      //add hovered class in selected list item
      let list = document.querySelectorAll(".navigation li");
      function activeLink() {
        list.forEach((item) => item.classList.remove("hovered"));
        this.classList.add("hovered");
      }
      list.forEach((item) => item.addEventListener("mouseover", activeLink));
    </script>
  </body>
</html>
    )rawliteral");
    });
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readMQ2data().c_str());
  });
  // server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", readBME280Humidity().c_str());
  // });
  // server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", readBME280Pressure().c_str());
  // });

  // Start server
  server.begin();
}
 
void loop(){
  
}
