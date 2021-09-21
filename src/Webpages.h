#ifndef WEBPAGES_HH
#define WEBPAGES_HH

char home_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<title>Wellcome to CASWS</title>

		<script language="javascript" type="text/javascript">

		var url = "ws://"+location.hostname+":1337/";

		// This is called when the page finishes loading
		function ws_config() {		
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


		// Call the init function as soon as the page loads
		window.addEventListener("load", ws_config, false);

		</script>
	</head>
	<h1>Cayan Async Sample WebServer</h1>
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
