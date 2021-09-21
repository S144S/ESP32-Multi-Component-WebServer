#ifndef WEBPAGES_HH
#define WEBPAGES_HH

char home_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<meta name="description" content="Sample webserver using ESP32">
		<meta name="author" content="Saeed Hosseini">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Wellcome to CASWS</title>

		<script language="javascript" type="text/javascript">

			var url = "ws://192.168.1.1:1337/";
			var packet_obj = new Object();
			var packet_data;
			var led_button;
			var led_shape;
			var led_text;
			var led_data = 0;

			// This is called when the page finishes loading
			function ws_config() {
				led_button = document.getElementById("btn1");
				led_body = document.getElementsByClassName("led_shape")[0];
				led_text = document.getElementById("led_status");
				// Connect to WebSocket server
				connect_to_ws(url);
			}

			// Call this to connect to the WebSocket server
			function connect_to_ws(url) {

				// Connect to WebSocket server
				websocket = new WebSocket(url);

				// Assign callbacks
				websocket.onopen = function(evt) { onOpen(evt) };
				websocket.onclose = function(evt) { onClose(evt) };
				websocket.onmessage = function(evt) { onMessage(evt) };
				websocket.onerror = function(evt) { onError(evt) };
			}

			// Called when a WebSocket connection is established with the server
			function onOpen(evt) {
				// Log connection state
				console.log("Connected");
				// Get the current state of the LED
				send_to_server("CL: client connected");
			}

			// Called when the WebSocket connection is closed
			function onClose(evt) {

				// Log disconnection state
				console.log("Disconnected");

				// Try to reconnect after a few seconds
				setTimeout(function() { wsConnect(url) }, 200);
			}
			// Called when a WebSocket error occurs
			function onError(evt) {
				console.log("ERROR: " + evt.data);
			}

			// Sends a message to the server (and prints it to the console)
			function send_to_server(message) {
				console.log("Sending: " + message);
				websocket.send(message);
			}

			function led_toggle() {
				led_data = !led_data;

				if (led_data)
				{
					led_button.innerHTML = "ON";
					led_body.style.backgroundColor = 'blue';
					led_text.innerHTML = "LED is now on"
				}
				else
				{
					led_button.innerHTML = "OFF";
					led_body.style.backgroundColor = 'gray';
					led_text.innerHTML = "LED is now off"
				}
				stream_packet();
			}

			function stream_packet() {
				packet_obj.LED = led_data;
				packet_data = JSON.stringify(packet_obj);
				console.log("Packet: " + packet_data);
				send_to_server(packet_data);
			}

			// Call the init function as soon as the page loads
			window.addEventListener("load", ws_config, false);

		</script>

		<style>
			body {
				background-color: rgba(252,163,3,255);
			}
			h1 {
				color: white;
				font-family: "Lucida Console", "Courier New", monospace;
				font-size: 50px;
				text-align: center;
			}
			hr.solid {
				border-top: 3px solid rgba(52,195,235,255);
			}

			hr.sepretor {
				border-top: 3px solid rgba(108,112,109,255);
			}

			.btn_pos {
				display: flex;
			  justify-content: center;
			  align-items: center;
			  height: 80px;
			}

			#btn1 {
				background-color: rgba(24,27,125,200);
				border-radius: 15px;
				color: white;
				padding: 16px 32px;
				text-decoration: none;
				display: inline-block;
				font-size: 16px;
				margin: 4px 2px;
				transition-duration: 0.2s;
				cursor: pointer;
			}
			#btn1:hover {
			  background-color: rgba(52,195,235,200);
			  color: white;
			}

			.led_shape {
				display: flex;
				margin: auto;
				border: 3px solid black;
				height: 50px;
				width: 50px;
				background-color: #555;
				border-radius: 50%;
			}

			#led_status {
				margin: 10px 0px 0px 0px;
				text-align: center;
				font-family: "Lucida Console", "Courier New", monospace;
				font-size: 32px;
			}

			.labels {
				font-family: "Lucida Console", "Courier New", monospace;
				font-size: 20px;
				text-align: center;
			}
			input[type=text], input[type=password] {
				width: 20%;
				margin: 8px 0;
				padding: 12px 20px;
				display: inline-block;
				font-family: "Lucida Console", "Courier New", monospace;
				font-size: 20px;
				border: 2px solid;
				border-color: rgba(28,13,87,255);
				border-radius: 5px;
				box-sizing: border-box;
			}
			#login_btn {
				background-color: rgba(69,19,55,255);
				border-radius: 15px;
				color: white;
				padding: 16px 32px;
				cursor: pointer;
			}
		</style>

	</head>

	<h1>Cayan Async Sample WebServer</h1>

	<hr class="solid">

	<div>
		<div class="btn_pos">
		  <button id="btn1" onclick="led_toggle()">OFF</button>
		</div>
		<div class="led_shape"></div>
		<p id="led_status">LED is now off</p>
		<hr class="sepretor">
	</div>

	<div>
		<div style="text-align: center;margin-top: 20px;">
			<label class="labels">Username:</label>
			<input type="text" placeholder="Username" name="username" id="user_name" required>
		</div>
		<div style="text-align: center;margin-top: 20px;">
			<label class="labels">Password:</label>
			<input type="password" placeholder="Password" name="password" id="pass_word" required>
			<br>
			<input type="checkbox" style="margin-left: 85px;">
			<label class="labels">Show Password</label>
			<br><br>
			<input type="submit" value="Login" id="login_btn">
		</div>
		<hr class="sepretor">
	</div>

	<!--<div style="border: 3px solid green;">
		<button id="btn2" onclick="btn_2()">some text</button>
	</div>-->

</html>

)=====";

char page_not_found[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<style>
*{
    transition: all 0.6s;
}

html {
    height: 100%;
}

body{
    font-family: 'Lato', sans-serif;
    color: #888;
    margin: 0;
}

#main{
    display: table;
    width: 100%;
    height: 100vh;
    text-align: center;
}

.fof{
	  display: table-cell;
	  vertical-align: right;
}

.fof h1{
	  font-size: 70px;
	  display: inline-block;
	  padding-right: 12px;
	  animation: type .5s alternate infinite;
}

@keyframes type{
	  from{box-shadow: inset -3px 0px 0px #888;}
	  to{box-shadow: inset -3px 0px 0px transparent;}
}
</style>
</head>
<body>

	<div id="main">
			<div class="fof">
					<h1>Page not found!</h1>
			</div>
	</div>
</body>
</html>
)=====";



#endif
