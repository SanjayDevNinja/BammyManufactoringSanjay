

/**
 * Module dependencies.
 */


var app = require('./app');
var debug = require('debug')('week-4:server');
var http = require('http');
const WebSocket = require('ws');

let connectedClients = [];
let connections = {
  count: { port: 8887 }
}

// Clients
const wss = new WebSocket.Server({ port: '8999' }, () => console.log(`WS Server is listening at 8999`));

wss.on('connection', ws => {
  ws.on('message', data => {
    if (ws.readyState !== ws.OPEN) return;
    connectedClients.push(ws);
  })

});



// Sensors
Object.entries(connections).forEach(([key, settings]) => {
  const connection = connections[key];
  connection.sensors = {};

  new WebSocket.Server({ port: settings.port }, () => console.log(`WS Server is listening at ${settings.port}`)).on('connection', (ws) => {
    ws.on('message', data => {
     
        console.log(data.toString());
      if (ws.readyState !== ws.OPEN) return;
      if (typeof data === 'object') {
        connection.sensors = data.toString();
        // For a future video, taking care of video stream from ESP32 Cam
      } else {
        connection.sensors = data.toString();
      }
      connectedClients.forEach(client => {
        client.send(data.toString());
      });
    });
  });
});








/**
 * Get port from environment and store in Express.
 */

var port = normalizePort(process.env.PORT || '3000');
app.set('port', port);

/**
 * Create HTTP server.
 */

var server = http.createServer(app);

/**
 * Listen on provided port, on all network interfaces.
 */

server.listen(port);
server.on('error', onError);
server.on('listening', onListening);

/**
 * Normalize a port into a number, string, or false.
 */

function normalizePort(val) {
  var port = parseInt(val, 10);

  if (isNaN(port)) {
    // named pipe
    return val;
  }

  if (port >= 0) {
    // port number
    return port;
  }

  return false;
}

/**
 * Event listener for HTTP server "error" event.
 */

function onError(error) {
  if (error.syscall !== 'listen') {
    throw error;
  }

  var bind = typeof port === 'string'
    ? 'Pipe ' + port
    : 'Port ' + port;

  // handle specific listen errors with friendly messages
  switch (error.code) {
    case 'EACCES':
      console.error(bind + ' requires elevated privileges');
      process.exit(1);
      break;
    case 'EADDRINUSE':
      console.error(bind + ' is already in use');
      process.exit(1);
      break;
    default:
      throw error;
  }
}

/**
 * Event listener for HTTP server "listening" event.
 */

function onListening() {

  console.log(`----Connected to http://localhost:${port}`)

  var addr = server.address();
  var bind = typeof addr === 'string'
    ? 'pipe ' + addr
    : 'port ' + addr.port;
  debug('Listening on ' + bind);
}

/* */
