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

			var url = "ws://"+location.hostname+":1337/";

			var packet_obj = new Object();

			var packet_data;
			var led_button;
			var led_shape;
			var led_text;
			var led_data = 0;

			var valid_username = "admin";
			var valid_password = "admin";
			var show_pass_btn;
			var input_user;
			var input_pass;

			var data_table;
			var voltage_value = 0;
			var currenet_value = 0;
			var temperature_value = 0;

			// This is called when the page finishes loading
			function ws_config() {
				led_button = document.getElementById("btn1");
				led_body = document.getElementsByClassName("led_shape")[0];
				led_text = document.getElementById("led_status");

				input_user = document.getElementById("user_name");
				input_pass = document.getElementById("pass_word");

				data_table = document.getElementById("table_1");
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

			// Called when a message is received from the server
			function onMessage(evt) {
				console.log("Received: " + evt.data);
				received_packet = JSON.parse(evt.data);
				voltage_value = received_packet.VOLTAGE;
				currenet_value = received_packet.CURRENT;
				temperature_value = received_packet.TEMPERATURE;
				Update_Data_Table();
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

			function show_password() {
				if (input_pass.type === "password")
				{
					input_pass.type = "text";
				}
				else
				{
					input_pass.type = "password";
				}
			}

			function try_loin() {
				if(input_user.value == valid_username)
				{
					if(input_pass.value == valid_password)
					{
						window.location = 'http://'+location.hostname+'/second-page';
					}
				}
				else
				{
					alert("Wrong username or password");
				}
			}

			function Update_Data_Table() {
				var voltage_Cells = data_table.rows.item(1).cells;
				var current_Cells = data_table.rows.item(2).cells;
				var temperatue_Cells = data_table.rows.item(3).cells;
				voltage_Cells.item(1).innerHTML = voltage_value;
				current_Cells.item(1).innerHTML = currenet_value;
				temperatue_Cells.item(1).innerHTML = temperature_value;
				console.log(voltage_Cells.item(1));
				console.log(current_Cells.item(1));
				console.log(temperatue_Cells.item(1));
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

			#user_data_title {
				color: rgba(105,12,58,255);
				font-family: "Lucida Console", "Courier New", monospace;
				font-size: 30px;
				text-align: center;
				margin-bottom: 20px;
			}
			.user_data_table {
				margin-left: auto;
				margin-right: auto;
				overflow-x: auto;
			}
			table, th, td {
			  border: 2px solid rgba(52,52,59,255);
				border-radius: 5px;
				border-spacing: 10px;
				vertical-align: middle;
				margin-bottom: 20px;
			}
			th, td {
			  border: 2px solid rgba(52,52,59,255);
				border-radius: 5px;
				padding-top: 10px;
				padding-bottom: 20px;
				padding-left: 30px;
				padding-right: 40px;
				font-family: "Lucida Console", "Courier New", monospace;
				font-size: 15px;
				font-weight: bold;
				text-align: center;
				vertical-align: middle;
			}
			tr:nth-child(even) {
  			background-color: #D6EEEE;
			}
			tr:nth-child(odd){
				background-color: rgba(99,168,141,255);
			}

			.message_container {
				  display: flex;
				  flex-wrap: nowrap;
				  /*background-color: DodgerBlue;*/
					flex-direction: row;
					justify-content: center;
					align-items: center;
				}
			.message_container > div {
				  background-color: #f1f1f1;
					border-radius: 7px;
				  width: 190px;
				  margin: 30px;
				  text-align: center;
				  line-height: 70px;
				  font-size: 30px;
			}
			@media (max-width: 800px) {
			  	.message_container {
						flex-wrap: nowrap;
						flex: 100%;
				    flex-direction: column;
						justify-content: center;
						align-items: center;
			  	}
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
			<input type="checkbox" style="margin-left: 85px;" onclick="show_password()">
			<label class="labels">Show Password</label>
			<br><br>
			<input type="submit" value="Login" id="login_btn" onclick="try_loin()">
		</div>
		<hr class="sepretor">
	</div>

	<div>
		<h2 id="user_data_title">User Input Data</h2>
		<div>
			<table class="user_data_table" id="table_1">
				<tr>
					<th>Parameter</th>
					<th>Value</th>
					<th>Unit</th>
				</tr>
				<tr>
			    <td>Voltage</td>
			    <td>0</td>
			    <td>v</td>
			  </tr>
			  <tr>
			    <td>Current</td>
			    <td>0</td>
			    <td>A</td>
			  </tr>
				<tr>
			    <td>Temperature</td>
			    <td>0</td>
			    <td>c</td>
			  </tr>
			</table>

			<div class="message_container">
				<div>Voltage Normal</div>
				<div>Current Normal</div>
				<div>Temperature Normal</div>
			</div>
		</div>
	</div>
	<hr class="sepretor">
<!--<div style="border: 3px solid green;">
</div>-->
</html>

)=====";

char second_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<meta name="description" content="Sample webserver using ESP32">
		<meta name="author" content="Saeed Hosseini">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Wellcome to CASWS Second Page</title>

		<script language="javascript" type="text/javascript">

			function back_to_home() {
				window.location = 'http://'+location.hostname+'/';
			}
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
				margin-top: 20px;
			}

			#home_btn {
				background-color: rgba(222,69,58,255);
				width: 400px;
				height: 100px;
				border-radius: 15px;
				color: white;
				padding: 16px 32px;
				text-decoration: none;
				display: inline-block;
				font-size: 26px;
				margin: 4px 2px;
				transition-duration: 0.2s;
				cursor: pointer;
			}
		</style>

	</head>

	<h1>Second Page</h1>

	<hr class="solid">

	<div>
		<div class="btn_pos">
		  <button id="home_btn" onclick="back_to_home()">BACK TO HOME</button>
		</div>
	</div>
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
