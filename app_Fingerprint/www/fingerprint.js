var ip = location.host;
var ws_Url = "ws://" + ip + "/ws/";
var intervalSendWatchdog;


// Send a message on the WebSocket.
function sendMessage(msg) {  
  if(ws!= null)
  {
    ws.send(msg);
    console.log("Alive msg sent");  
  }
}

// Setup WebSocket connection and event handlers
function setup() {  
    
  ws = new WebSocket(ws_Url);

  // Listen for the connection open event then call the sendMessage function          
  ws.onopen = function (e) {      
  console.log("Connected");
  intervalSendWatchdog = setInterval(function(){ 
      sendMessage("{'hello'}"); 
    }, 2000);
  }

  // Listen for the close connection event
  ws.onclose = function (e) {      
    console.log("OnClose recieved - WS disconnected");
    clearInterval(intervalSendWatchdog);
    ws = null;
  }

  // Listen for connection errors
  ws.onerror = function (e) {      
    console.log("Error ");  // ToDo: What shall we do with a drunken error...
  }

  // Listen for new messages arriving at the client
  ws.onmessage = function (e) {      
    //var request = (e.data).split(",");
    //val = e.data;
    console.log(e.data);
    var val = JSON.parse(e.data);
    console.log(val);
  }
}

setup();
