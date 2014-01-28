
function m2x_log(stream, value) {
  
  local m2x_key = "796c9a1db55668dcf2b39591d6dd03cd";
  local m2x_feed_id = "31147eb194e58826e4e92c192a2cf5a0";
  
  local url = "http://api-m2x.att.com/v1/feeds/" + m2x_feed_id + "/streams/" + stream;
    
  local data = http.jsonencode({
    value = value
  });
  
  http.put(url, {
      "X-M2X-KEY": m2x_key,
      "Content-type": "application/json"
    }, data).sendsync();
}

device.on("data", function (val) {
  server.log("data: " + val);
  local d = http.jsondecode(val);
  m2x_log("weight", d.weight);
  m2x_log("loadLevel", d.loadLevel);
});