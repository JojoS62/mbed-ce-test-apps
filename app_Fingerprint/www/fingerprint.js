// code for fingerprint server

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

function onTestbutton() {
  console.log("onbutton");
  const pic = document.getElementById('imgFinger');
  pic.src = "apple-icon-114x114.png";
}

// Setup WebSocket connection and event handlers
function setup() {  
    
  ws = new WebSocket(ws_Url);

  // Listen for the connection open event then call the sendMessage function          
  ws.onopen = function (e) {      
  console.log("Connected");
  intervalSendWatchdog = setInterval(function(){ 
      sendMessage("{'hello'}"); 
    }, 10000);
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
    const obj = JSON.parse(e.data);
    console.log(obj);

    document.getElementById('lblStatus').innerHTML = obj.finger;

    if (obj.finger == "detect failed"){
      document.getElementById('lblStatus').style.backgroundColor = "red";
    } else {
      document.getElementById('lblStatus').style.backgroundColor = "limegreen";
    }

    if (obj.finger == "image saved") {
      var pic = document.getElementById('imgFinger');
      pic.src = "R503-Image.bmp?t=" + new Date().getTime();
    } 
  }
}

setup();
