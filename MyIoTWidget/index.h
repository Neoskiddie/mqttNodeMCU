const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta http-equiv="refresh" content="3" >
<style>

</style>
</head>
<body>
<center>
<h1 style="color:blue">WiFi interface for the IoT</h1><br>
<p>Temp: @@TEMP@@ C</p>
Click to turn <a href="ledOn" target="myIframe"><button>LED ON</button></a><br>
Click to turn <a href="ledOff" target="myIframe"><button>LED OFF</button></a><br>
LED State:<iframe name="myIframe" width="100" height="25" frameBorder="0"><br>

<hr>

</center>
 
</body>
</html>
)=====";
