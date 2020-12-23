// handle stuff, keep the game together
var canvas   = document.querySelector("canvas");
canvas.width = 200, canvas.height = 300;
var context  = canvas.getContext("2d");

context.beginPath();
context.moveTo(0, 0);
context.lineTo(100, 100);
context.closePath();
context.stroke();