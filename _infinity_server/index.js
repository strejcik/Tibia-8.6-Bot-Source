'use strict';



let express = require('express');
let app = express();
let http = require('http');
let server = http.createServer(app);
const io = require('socket.io')(server);
var net = require('net');
var StringDecoder = require('string_decoder').StringDecoder;

const {request_switch} = require('./switch');



var s = net.createServer(function(socket) {
  console.log('Client connected #1');

  socket.on('data', (data) => {
    console.log(data);
    request_switch(data, socket);
  });

  socket.on('end', function () {
    console.log('end, socket disconnected', socket.name);
    socket.end();
      socket.destroy();
  });

  socket.on('close', function () {
      console.log('close, socket disconnected', socket.name);
      socket.end();
      socket.destroy();
  });

  socket.on('error', function (error) {
      console.log('error, socket disconnected', error);
  });
  
});




s.listen(8081, '192.168.1.106');