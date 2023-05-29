// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyCnhOAFefqOs_o27k6nys4McEfEjrchTus",
  authDomain: "do-an-1-2fc90.firebaseapp.com",
  databaseURL: "https://do-an-1-2fc90-default-rtdb.firebaseio.com",
  projectId: "do-an-1-2fc90",
  storageBucket: "do-an-1-2fc90.appspot.com",
  messagingSenderId: "1063064140755",
  appId: "1:1063064140755:web:24205ceaf5258020e00d26"
};
firebase.initializeApp(firebaseConfig);

// Open menu
var hamburger = document.querySelector(".hamburger");
var wrapper  = document.querySelector(".wrapper");
var backdrop = document.querySelector(".backdrop");

hamburger.addEventListener("click", function(){
    wrapper.classList.add("active");
})

backdrop.addEventListener("click", function(){
    wrapper.classList.remove("active");
})


// Change TABS
const $ = document.querySelector.bind(document);
const $$ = document.querySelectorAll.bind(document);

const tabs = $$(".tab-item");
const panes = $$(".tab-pane");

const tabActive = $(".tab-item.active");


tabs.forEach((tab, index) => {
  const pane = panes[index];

  tab.onclick = function () {
    $(".tab-item.active").classList.remove("active");
    $(".tab-pane.active").classList.remove("active");

    this.classList.add("active");
    pane.classList.add("active");
  };
});

                      // CONTROL
                              
window.onload = function(){
  var sliderclock = document.getElementById("clock");
  var resultclock = document.getElementById("clock-result");
  sliderclock.oninput = function(){
      resultclock.innerHTML = sliderclock.value ;
  }

  let elm = document.querySelector('.toggle');
  elm.addEventListener('click', () => {
    elm.classList.toggle('active');
  });

  let elm2 = document.querySelector('.toggle2');
  elm2.addEventListener('click', () => {
    elm2.classList.toggle('active');
  });

  let elm3 = document.querySelector('.toggle3');
  elm3.addEventListener('click', () => {
    elm3.classList.toggle('active');
  });

  let elm4 = document.querySelector('.toggle4');
  elm4.addEventListener('click', () => {
    elm4.classList.toggle('active');
  });

  // START CLOCK SCRIPT
  initClock();
  // END CLOCK SCRIPT
}

Number.prototype.pad = function(n) {
  for (var r = this.toString(); r.length < n; r = 0 + r);
  return r;
};

function updateClock() {
  var now = new Date();
  var milli = now.getMilliseconds(),
    sec = now.getSeconds(),
    min = now.getMinutes(),
    hou = now.getHours(),
    mo = now.getMonth(),
    dy = now.getDate(),
    yr = now.getFullYear();
  var months = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
  var tags = ["mon", "day", "year", "hour", "minute", "second"],
    corr = [months[mo], dy, yr, hou.pad(2), min.pad(2), sec.pad(2), milli];
  for (var i = 0; i < tags.length; i++)
    document.getElementById(tags[i]).firstChild.nodeValue = corr[i];
}

function initClock() {
  updateClock();
  window.setInterval("updateClock()", 1);
}

                      //  FIREBASE

// Auto load Do Am-------------------------
firebase.database().ref("/Data/received_from_esp/dirt_humidity").on("value", function(snapshot){
  var nd = snapshot.val();
  document.getElementById("hum").innerHTML = nd;
  console.log(snapshot.key + nd);
});


// Auto load Temperature----------------------------
firebase.database().ref("/Data/received_from_esp/temperature").on("value", function(snapshot){
  var nd = snapshot.val();
  var formattedTemperature = nd.toFixed(2);
  document.getElementById("temp").innerHTML = formattedTemperature;
  console.log(snapshot.key + formattedTemperature);
});

// Auto load light_sensor----------------------------
firebase.database().ref("/Data/received_from_esp/light_sensor").on("value", function(snapshot){
  var br = snapshot.val();
  var brightnessElement = document.getElementById("brightness");
  if (br == 1) {
    brightnessElement.innerHTML = "LOW";
  } else if (br == 0) {
    brightnessElement.innerHTML = "HIGH";
  } else {
    brightnessElement.innerHTML = br; // Show the actual value if it's neither 0 nor 1
  }
  console.log(snapshot.key + br);
});

// Auto load clock ----------------------------
var airConditionInput = document.getElementById('clock');
airConditionInput.addEventListener('input', function(event) {
  var value = parseInt(event.target.value, 10);
  firebase.database().ref("/Data/send_to_esp/timeset").set(value);
});

// Auto load robot ----------------------------
var sw = document.querySelector('.sw');
sw.addEventListener('click', function() {
  var isOn = sw.classList.toggle('OFF');
  var motorState = isOn ? "ON" : "OFF";
  firebase.database().ref('/Data/send_to_esp/motor').set(motorState);
});

// Auto load light ----------------------------
var sw2 = document.querySelector('.sw2');
sw2.addEventListener('click', function() {
  var isOn = sw2.classList.toggle('OFF');
  var motorState = isOn ? "ON" : "OFF";
  firebase.database().ref('/Data/send_to_esp/light').set(motorState);
});

// Auto load fan ----------------------------
var sw3 = document.querySelector('.sw3');
sw3.addEventListener('click', function() {
  var isOn = sw3.classList.toggle('OFF');
  var motorState = isOn ? "ON" : "OFF";
  firebase.database().ref('/Data/send_to_esp/fan').set(motorState);
});

// Auto load WARNING ----------------------------
var sw4 = document.querySelector('.sw4');
sw4.addEventListener('click', function() {
  var isOn = sw4.classList.toggle('OFF');
  firebase.database().ref('/Data/send_to_esp/WARNING').set(isOn);
});

//status--------------------------------------
firebase.database().ref("/Data/status/motor").on("value", function(snapshot) {
  var status = snapshot.val();
  var motorImage = document.getElementById("motorImage");
  
  if (status == 1) {
    motorImage.src = 'img/pump.png';
  } else {
    motorImage.src = 'img/pump (1).png'; // Change this to the path of your second image
  }
});

firebase.database().ref("/Data/status/light").on("value", function(snapshot) {
  var status2 = snapshot.val();
  var bulbimg = document.getElementById("bulbimg");
  
  if (status2 == 1) {
    bulbimg.src = 'img/idea.png';
  } else {
    bulbimg.src = 'img/idea (1).png'; // Change this to the path of your second image
  }
});

firebase.database().ref("/Data/status/fan").on("value", function(snapshot) {
  var status3 = snapshot.val();
  var fanimg = document.getElementById("fanimg");
  
  if (status3 == 1) {
    fanimg.src = 'img/fan.png';
  } else {
    fanimg.src = 'img/fan (1).png'; // Change this to the path of your second image
  }
});























