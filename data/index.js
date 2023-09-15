var gateway = `ws://${window.location.hostname}/rorWebSocket`;
var websocket;
var lastRoofPosition;
var ledBoxElement;
var ledElementRed;
var ledElementGreen;
var isScopeParked = false;
var roofPositionName = "Unknown";
function doSomething(event) {
  if (isScopeParked) {
    websocket.send(JSON.stringify({ 'action': 'toggle' }));
  }
}
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
function initWebSocket() {
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}
function onOpen(event) {

}
function onClose(event) {
  setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
  var obj = JSON.parse(event.data);
  var roofImage = document.getElementById('roofImage');
  var roofPosition = obj.status["RoRCurrentPosition"];
  if (lastRoofPosition !== roofPosition) {
    lastRoofPosition = roofPosition;
    switch (roofPosition) {
      case 0: // shutterOpen
        roofImage.src = "ROROpen.jpg";
        roofPositionName = "Opened";
        break;
      case 1: // shutterClosed
        roofImage.src = "RORClosed.jpg";
        roofPositionName = "Closed";
        break;
      case 2: // shutterOpening
      case 3: // shutterClosing
        roofImage.src = "RORMoving.gif";
        roofPositionName = "Moving";
        break;
      case 4: // shutterError
        roofImage.src = "RORUnknown.jpg";
        roofPositionName = "Unknown";
        break;
      default:
    }
  }
  document.getElementById('rorpositionID').innerHTML = roofPositionName;
  document.getElementById('temperatureInID').innerHTML = obj.status["indoorTemperature"];
  document.getElementById('humidityInID').innerHTML = obj.status["indoorHumidity"];
  document.getElementById('temperatureOutID').innerHTML = obj.status["outdoorTemperature"];
  document.getElementById('humidityOutID').innerHTML = obj.status["outdoorHumidity"];

  var scopeStatusTextA = document.getElementById('scopeStatusTextA');
  var scopeStatusTextB = document.getElementById('scopeStatusTextB');
  var parkPosition = obj.status["IsScopeParkSafe"];
  switch (parkPosition) {
    case 0:
      scopeStatusTextA.innerHTML = "&nbsp;is Parked.&nbsp;&nbsp;";
      scopeStatusTextB.innerHTML = "&nbsp;&nbspCan move roof.&nbsp;";
      ledElementRed[0].style.display = "none";
      ledElementGreen[0].style.display = "block";
      isScopeParked = true;
      document.getElementById('buttonID').disabled = false;
      document.getElementById('buttonID').setAttribute("style", "background-color: green");
      break;
    case 1:
      scopeStatusTextA.innerHTML = "&nbsp;is NOT Parked.&nbsp;";
      scopeStatusTextB.innerHTML = "Cannot move roof.";
      ledElementRed[0].style.display = "block";
      ledElementGreen[0].style.display = "none";
      isScopeParked = false;
      document.getElementById('buttonID').disabled = true;
      document.getElementById('buttonID').setAttribute("style", "background-color: black");
      break;
    default:
  }
}

window.onload = function () {
  let dragItem = document.querySelector("#buttonID");
  let pressHoldEvent = new CustomEvent("pressHold");
  let timePressed = 0;
  let press = false;
  let pressHoldDuration = 100;
  let disableItemCounter = 0;
  let disableItemDuration = 400;
  let disableEnable = false;
  dragItem.addEventListener("mousedown", pressingDown, false);
  dragItem.addEventListener("mouseup", notPressingDown, false);
  dragItem.addEventListener("touchstart", pressingDown, false);
  dragItem.addEventListener("touchend", notPressingDown, false);
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