local serialConn = hardware.uart12;
local buf = "";

function read(){
  local readBuffer = blob(0);
  local byte;
  
  while((byte = serialConn.read()) != -1) {
	  readBuffer.writen(byte, 'b');
	}

  local c = readBuffer.tostring();
  
  if (c == "\n") {
    onData(buf);
    buf = "";
  } else  {
    buf += c;
  }
}

function onData(buf) {  
  agent.send("data", buf);
}

serialConn.configure(9600, 8, PARITY_NONE, 1, NO_CTSRTS, read);