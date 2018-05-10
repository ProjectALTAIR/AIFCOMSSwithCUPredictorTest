(function() {
    'use strict';
    /*jshint node:true*/

    var express = require('express');
    var compression = require('compression');
    var url = require('url');
    var request = require('request');

    var variables = require('./js_variables.js');
    var ROOT_DIR = variables.ROOT_DIR;

// JA Serial port connection code BEGIN  (added 13sep16)

    console.log("Thinking for a moment...");

    var fs = require('fs');                     // For 1) writing out to cu.usbserial, in order to move the telescope;
                                                // and 2) for writing out ALTAIR's position to /tmp/altairpos.txt
                                                // (so that the predictor can read it).
    var serialport = require('serialport');
    var WebSocketServer = require('ws').Server;

    var SERVER_PORT = 8081;               // port number for the webSocket server
    var wss = new WebSocketServer({port: SERVER_PORT}); // the webSocket server
    var connections = new Array;          // list of connections to the server

    var arduinoPortString1     = "cu.usbmodem";
    var arduinoPortString2     = "COM4";
    var arduinoPortString2a    = process.argv[2];
    var arduinoPortName        = "";
    var genericPortNamesList   = "Connected serial port names: ";
    var myPort                 = null;

    serialport.list(function (err, ports) {
        var isFirstPort = true;
        ports.forEach(function(port) {
           if (!isFirstPort) genericPortNamesList = genericPortNamesList.concat(" , ");
           genericPortNamesList = genericPortNamesList.concat(port.comName);
           if (arduinoPortString2a == "") {
              if (port.comName.indexOf(arduinoPortString2)  != -1) arduinoPortName = port.comName;
           } else {
              if (port.comName.indexOf(arduinoPortString2a) != -1) arduinoPortName = port.comName;
           }
           if (port.comName.indexOf(arduinoPortString1) != -1) arduinoPortName = port.comName;
           isFirstPort = false;
        });
        console.log(genericPortNamesList);
    });

    function showPortOpen() {
        console.log('Serial port open.  Data rate: ' + myPort.options.baudRate);
        console.log("");
    }
 
    // This function broadcasts messages to all webSocket clients
    function broadcast(data) {
        for (myConnection in connections) {   // iterate over the array of connections
           connections[myConnection].send(data); // send the data to each connection
        }
    }

    function sendToSerial(data) {
        var date = new Date(); 
        if (data.substring(0, 4) == "LOG:") {
           console.log(data + "   " + date + " + " + date.getMilliseconds() + " milliseconds");
        } else if (data.substring(0, 11) == "MOVESCOPE: ") {
           var pureData = data.replace("MOVESCOPE: ", "");
           console.log(data + "   " + date + " + " + date.getMilliseconds() + " milliseconds");
           fs.appendFile("/dev/cu.usbserial", pureData, function(err) {
              if (err) {
                return console.log(err);
              }
           });
        } else if (data.substring(0, 11) == "ALTAIRPOS: ") {
           var pureData = data.replace("ALTAIRPOS: ", "");
           var pos_path = "/tmp/altairpos.txt";
           if(process.platform.toLowerCase().includes("win")){
               pos_path = ROOT_DIR + "altairpos.txt";
           }
           fs.writeFile(pos_path, pureData, function(err) {
              if (err) {
                return console.log(err);
              }
           });
        } else {
           if (myPort != null) {
              console.log("Sending to serial: " + data + " at: " + date + " + " + date.getMilliseconds() + " milliseconds");
              myPort.write(data);
           } else {
              console.log("I tried to send the following to the NONEXISTANT serial connection: " + data + " at: " + date + " + " + date.getMilliseconds() + " milliseconds");
           }
        }
    }

    function sendSerialData(data) {
       console.log("Received from serial: " + data);
       // if there are webSocket connections, send the serial data to all of them:
       if (connections.length > 0) {
           broadcast(data);
       }
    }
 
    function saveLatestData(data) {
       console.log(data);
       // if there are webSocket connections, send the serial data to all of them:
       if (connections.length > 0) {
           broadcast(data);
       }
    }

    function showPortClose() {
       var date = new Date();
       console.log('Serial port closed at: ' + date + '!  You must completely restart   node server.js   if you want to communicate with ALTAIR.');
    }
 
    function showError(error) {
       console.log('Serial port error: ' + error);
    }

    wss.on('connection', handleConnection);
 
    function handleConnection(client) {
       var date = new Date();
       console.log("New websocket connection at: " + date); // you have a new client
       connections.push(client); // add this client to the connections array
 
       client.on('message', sendToSerial); // when a client sends a message,
 
       client.on('close', function() { // when a client closes its connection
          date = new Date();
          console.log("Websocket connection closed at: " + date); // print it out
          var position = connections.indexOf(client); // get the client's position in the array
          connections.splice(position, 1); // and delete it from the array
      });
    }

// JA Serial port connection code END



    var yargs = require('yargs').options({
        'port' : {
            'default' : 8080,
            'description' : 'Port to listen on.'
        },
        'public' : {
            'type' : 'boolean',
            'description' : 'Run a public server that listens on all interfaces.'
        },
        'upstream-proxy' : {
            'description' : 'A standard proxy server that will be used to retrieve data.  Specify a URL including port, e.g. "http://proxy:8000".'
        },
        'bypass-upstream-proxy-hosts' : {
            'description' : 'A comma separated list of hosts that will bypass the specified upstream_proxy, e.g. "lanhost1,lanhost2"'
        },
        'help' : {
            'alias' : 'h',
            'type' : 'boolean',
            'description' : 'Show this help.'
        }
    });
    var argv = yargs.argv;

    if (argv.help) {
        return yargs.showHelp();
    }

    // eventually this mime type configuration will need to change
    // https://github.com/visionmedia/send/commit/d2cb54658ce65948b0ed6e5fb5de69d022bef941
    // *NOTE* Any changes you make here must be mirrored in web.config.
    var mime = express.static.mime;
    mime.define({
        'application/json' : ['czml', 'json', 'geojson', 'topojson'],
        'model/vnd.gltf+json' : ['gltf'],
        'model/vnd.gltf.binary' : ['bgltf', 'glb'],
        'text/plain' : ['glsl']
    });

    var app = express();
    app.use(compression());
    app.use(express.static(__dirname));

    function sendSerialData(data) {
       console.log(data);
    }

    function getRemoteUrlFromParam(req) {
        var remoteUrl = req.params[0];
        if (remoteUrl) {
            // add http:// to the URL if no protocol is present
            if (!/^https?:\/\//.test(remoteUrl)) {
                remoteUrl = 'http://' + remoteUrl;
            }
            remoteUrl = url.parse(remoteUrl);
            // copy query string
            remoteUrl.search = url.parse(req.url).search;
        }
        return remoteUrl;
    }

    var dontProxyHeaderRegex = /^(?:Host|Proxy-Connection|Connection|Keep-Alive|Transfer-Encoding|TE|Trailer|Proxy-Authorization|Proxy-Authenticate|Upgrade)$/i;

    function filterHeaders(req, headers) {
        var result = {};
        // filter out headers that are listed in the regex above
        Object.keys(headers).forEach(function(name) {
            if (!dontProxyHeaderRegex.test(name)) {
                result[name] = headers[name];
            }
        });
        return result;
    }

    var upstreamProxy = argv['upstream-proxy'];
    var bypassUpstreamProxyHosts = {};
    if (argv['bypass-upstream-proxy-hosts']) {
        argv['bypass-upstream-proxy-hosts'].split(',').forEach(function(host) {
            bypassUpstreamProxyHosts[host.toLowerCase()] = true;
        });
    }

    app.get('/proxy/*', function(req, res, next) {
        // look for request like http://localhost:8080/proxy/http://example.com/file?query=1
        var remoteUrl = getRemoteUrlFromParam(req);
        if (!remoteUrl) {
            // look for request like http://localhost:8080/proxy/?http%3A%2F%2Fexample.com%2Ffile%3Fquery%3D1
            remoteUrl = Object.keys(req.query)[0];
            if (remoteUrl) {
                remoteUrl = url.parse(remoteUrl);
            }
        }

        if (!remoteUrl) {
            return res.status(400).send('No url specified.');
        }

        if (!remoteUrl.protocol) {
            remoteUrl.protocol = 'http:';
        }

        var proxy;
        if (upstreamProxy && !(remoteUrl.host in bypassUpstreamProxyHosts)) {
            proxy = upstreamProxy;
        }

        // encoding : null means "body" passed to the callback will be raw bytes

        request.get({
            url : url.format(remoteUrl),
            headers : filterHeaders(req, req.headers),
            encoding : null,
            proxy : proxy
        }, function(error, response, body) {
            var code = 500;

            if (response) {
                code = response.statusCode;
                res.header(filterHeaders(req, response.headers));
            }

            res.status(code).send(body);
        });
    });

    var server = app.listen(argv.port, argv.public ? undefined : 'localhost', function() {
        var date = new Date();
        console.log('');
        console.log("Starting server at: " + date);
        console.log('');
        if (argv.public) {
            console.log('Cesium development server running publicly.  Connect to http://localhost:%d/', server.address().port);
        } else {
            console.log('Cesium development server running locally.  Connect to http://localhost:%d/', server.address().port);
        }

// JA Serial port connection code part 2 BEGIN  (added 13sep16)

        console.warn("");
        if (arduinoPortName.localeCompare("") == 0) {
           console.warn("WARNING!  No Arduino, or/and radio transceiver, appears to be connected to any USB port.  Any attempted communication with ALTAIR will, thus, go nowhere...")
        } else {
           console.warn('Now also *connected* to ALTAIR Arduino DNT900P transceiver on serial port: %s .',  arduinoPortName);
           myPort = new serialport(arduinoPortName, {
               baudRate: 9600,
               // look for return and newline at the end of each data packet:
               parser: serialport.parsers.readline("\n")
           });
           myPort.on('open', showPortOpen);
           myPort.on('data', sendSerialData);
           myPort.on('close', showPortClose);
           myPort.on('error', showError);
        }

// JA Serial port connection code part 2 END


    });



//    server.on('data', sendSerialData);

    server.on('error', function (e) {
        if (e.code === 'EADDRINUSE') {
            console.log('Error: Port %d is already in use, select a different port.', argv.port);
            console.log('Example: node server.js --port %d', argv.port + 1);
        } else if (e.code === 'EACCES') {
            console.log('Error: This process does not have permission to listen on port %d.', argv.port);
            if (argv.port < 1024) {
                console.log('Try a port number higher than 1024.');
            }
        }
        console.log(e);
        process.exit(1);
    });

    server.on('close', function() {
        var date = new Date();
        console.log('ALTAIR AIFCOMSS Cesium development server stopped at: ' + date);
    });

    var isFirstSig = true;
    process.on('SIGINT', function() {
        var date = new Date();
        if (isFirstSig) {
            console.log('ALTAIR AIFCOMSS Cesium development server shutting down at: ' + date);
            server.close(function() {
              process.exit(0);
            });
            isFirstSig = false;
        } else {
            console.log('ALTAIR AIFCOMSS Cesium development server force kill at: ' + date);
            process.exit(1);
        }
    });

})();
