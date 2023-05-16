
// var gateway = `rorWebSocket://${window.location.hostname}/rorWebSocket`;
var gateway = `ws://${window.location.hostname}/rorWebSocket`;
var websocket;
var lastRoofPosition;


var ledBoxElement;
var ledElementRed;
var ledElementGreen;
var isScopeParked = false;


function doSomething(event) {
  if (isScopeParked) {
    websocket.send(JSON.stringify({ 'action': 'toggle' }));
    console.log('doSomething - send to server button triggered');
  }
}

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

window.addEventListener('load', onLoad);

function onLoad(event) {
  initWebSocket();
  ledBoxElement = document.getElementsByClassName("led-box");
  ledElementRed = document.getElementsByClassName("led-red");
  ledElementGreen = document.getElementsByClassName("led-green");
  ledElementRed[0].style.display = "none";
  ledElementGreen[0].style.display = "none";
  document.getElementById('buttonID').setAttribute("style", "background-color: black");
}

// ----------------------------------------------------------------------------
// WebSocket handling
// ----------------------------------------------------------------------------

function initWebSocket() {
  console.log('Trying to open a WebSocket connection...');
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
  console.log('End Of - Trying to open a WebSocket connection...');
}



function onOpen(event) {
  console.log('Connection opened');
}

function onClose(event) {
  console.log('Connection closed');
  setTimeout(initWebSocket, 2000);
}



function onMessage(event) {
  var obj = JSON.parse(event.data);
}

// Update the onMessage function

function onMessage(event) {
  var obj = JSON.parse(event.data);

  // Update the image based on the roof status

  var roofImage = document.getElementById('roofImage');
  var roofPosition = obj.status["RoRPosition"];

  if (lastRoofPosition !== roofPosition) {
    lastRoofPosition = roofPosition;
    if (roofPosition === "Opened") {
      roofImage.src = "ROROpen.jpg";
    } else if (roofPosition === "Moving") {
      roofImage.src = "RORMoving.gif";
    } else if (roofPosition === "Unknown") {
      roofImage.src = "RORUnknown.jpg";
    } else if (roofPosition === "Closed") {
      roofImage.src = "RORClosed.jpg";
    }
  }

  document.getElementById('rorpositionID').innerHTML = obj.status["RoRPosition"];
  document.getElementById('temperatureID').innerHTML = obj.status["temperature"];
  document.getElementById('humidityID').innerHTML = obj.status["humidity"];
  document.getElementById('pressureID').innerHTML = obj.status["pressure"];
  document.getElementById('altitudeMeterID').innerHTML = obj.status["altitudeMeter"];
  document.getElementById('altitudeFeetID').innerHTML = obj.status["altitudeFeet"];

  var scopeStatusTextA = document.getElementById('scopeStatusTextA');
  var scopeStatusTextB = document.getElementById('scopeStatusTextB');


  if (obj.status["IsScopeParkSafe"] === "ScopeIsParked") {
    scopeStatusTextA.innerHTML = "&nbsp;is Parked.&nbsp;&nbsp;";
    scopeStatusTextB.innerHTML = "&nbsp;&nbspCan move roof.&nbsp;";
    ledElementRed[0].style.display = "none";
    ledElementGreen[0].style.display = "block";
    isScopeParked = true;
    document.getElementById('buttonID').disabled = false;
    document.getElementById('buttonID').setAttribute("style", "background-color: green");
  } else if (obj.status["IsScopeParkSafe"] === "ScopeNotParked") {
    scopeStatusTextA.innerHTML = "&nbsp;is NOT Parked.&nbsp;";
    scopeStatusTextB.innerHTML = "Cannot move roof.";
    ledElementRed[0].style.display = "block";
    ledElementGreen[0].style.display = "none";
    isScopeParked = false;
    document.getElementById('buttonID').disabled = true;
    document.getElementById('buttonID').setAttribute("style", "background-color: black");
  }

  if (false) {
    console.log(obj.status["IsScopeParkSafe"]);
    console.log(obj.status["RoRPosition"]);
    console.log(obj.status["temperature"]);
    console.log(obj.status["humidity"]);
    console.log(obj.status["pressure"]);
    console.log(obj.status["altitudeMeter"]);
    console.log(obj.status["altitudeFeet"]);
  }
}

window.onload = function () {

  let dragItem = document.querySelector("#buttonID");
  let pressHoldEvent = new CustomEvent("pressHold");

  let timePressed = 0;
  let press = false;
  let pressHoldDuration = 100;
  let timerID;
  let disableItemCounter = 0;
  let disableItemDuration = 400;
  let disableEnable = false;

  dragItem.addEventListener("mousedown", pressingDown, false);
  dragItem.addEventListener("mouseup", notPressingDown, false);
  dragItem.addEventListener("touchstart", pressingDown, false);
  dragItem.addEventListener("touchend", notPressingDown, false);
  // Listening for our custom pressHold event
  dragItem.addEventListener("pressHold", doSomething, false);

  function counter() {
    if (!disableEnable && isScopeParked) {
      if (press) {
         timePressed++;
        dragItem.setAttribute("style", "background-color: green");
        scaleItem();
        if (timePressed >= pressHoldDuration) {
          timePressed = 0;
          disableItemCounter = 0;
          resetItem();
          dragItem.dispatchEvent(pressHoldEvent);
          disableEnable = true;
        }
      } else {
        timePressed = 0;
        resetItem();
      }
    }
    else {
      disableItemCounter++;
      if (disableItemCounter >= disableItemDuration) {
        dragItem.setAttribute("style", "background-color: green");
        disableItemCounter = 0;
        disableEnable = false;
      }
      else {
        dragItem.setAttribute("style", "background-color: black");
      }
    }
    timerID = requestAnimationFrame(counter);

  }
  counter();

  function pressingDown(e) {
    press = true;
    e.preventDefault();
  }
  function notPressingDown(e) {
    press = false;
    timePressed = 0;
  }

  function scaleItem() {
    let size = 1 + timePressed / 200;
    if (size > 2) {
      size = 2;
      container.classList.add("stripes");
    }
    dragItem.style.transitionDuration = "1s";
    dragItem.style.setProperty("--scale-value", size);
  }

  function resetItem() {
    dragItem.style.transitionDuration = ".2s";
    dragItem.style.setProperty("--scale-value", 1);
  }
}