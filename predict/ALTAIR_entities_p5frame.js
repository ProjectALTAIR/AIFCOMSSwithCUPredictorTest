var    fontString             = 'assets/LucidaSansRegular.ttf';
var    arduinoPortString1     = "tty.usbmodem";
var    arduinoPortString2     = "COM4";
var    portSpeed              =  9600;
var    audibleAlarms          =  true;
var    numAlarms              =    33;
var    timeBtwALTAIRPosComms  =    59;    // in approximately 20's of milliseconds (so e.g. a value of 30 ~= 0.6 seconds)
var    timeBetweenAlarmSounds =    20;    // in approximately 20's of milliseconds (so e.g. a value of 30 ~= 0.6 seconds)
var    timeBtwScopeTrkComms   =   150;    // in approximately 20's of milliseconds (so e.g. a value of 30 ~= 0.6 seconds)
var    totalMicroSDSpace      =  8000.;   // in MB (approximate)
var    gravAcc                =     9.80665; // m/s^2

var    maxSetting             =    10.0;  // max power setting for each motor: setting ranges from 0 - 10
var    maxRPM                 =  5000.0;
var    maxTemp                =    50.0;  // degrees C
var    minTemp                =   -50.0;  // degrees C
var    maxCurrent             =    10.0;  // Amperes (per each motor+ESC pair)
var    maxDiskSpaceFraction   =     0.9;  // maximum fraction of the onboard microSD card disk space that should be occupied with data
var    oneBarRSSI             =  -110.0;  // in dBm -- the bare minimum to get any transmission at all 
var    twoBarRSSI             =   -85.0;  // in dBm -- marginal signal strength
var    threeBarRSSI           =   -60.0;  // in dBm -- minimum acceptable signal strength (below this, there is an alarm)
var    fourBarRSSI            =   -35.0;  // in dBm -- solid signal strength
var    fiveBarRSSI            =   -11.0;  // in dBm -- very strong signal strength
var    maxDroppedMessageFrac  =     0.2;  // alarm if more than 20% of messages are dropped
var    minVoltage             =    11.1;  // volts
var    maxVoltage             =    12.9;  // volts
var    minInfoAge             =     0.0;  // seconds
var    maxInfoAge             =    15.0;  // seconds

var    lightSources           =     3;    // 1 = integrating sphere light source installed, 2 = diffusive light source installed, 3 = both installed, 0 = neither installed

var    buttonSquareSize       =    12;    // pixels
var    buttonSqCornerRadius   =     6;    // pixels 
var    buttonSpacing          =    15;    // pixels

var    oldyaw = 0., oldpitch = 0., oldroll = 0.;

var    orientGraphics, accelGraphics;

var    alarm;
var    blockButtons      = false;
var    alarmOn           = []; // There are  numAlarms  alarms.  For each one: 0 == off, 1 == on but silenced, 2 == on (and not silenced)
var    alarmCounter      = 0;
var    scopeTrackCounter = 0;
var    altairPosCounter  = 0;

var    setting           = [];
var    rpm               = [];
var    current           = [];
var    temp              = [];
var    accel             = [];
// var    propRotAng        = [];
var    yaw, pitch, roll, rotAng;
var    UM7health, UM7temp;

var    voltage           = [];
var    pressure          = [];
var    humidity          = [];
var    photodiodeReadout = [];
var    age               = [];
var    previousTime      = new Date();
var    gpsInfoReceivedTime, orientInfoReceivedTime, mon2InfoReceivedTime;
var    lat, long, ele, horzSigma, vertSigma;
var    gLTAIRLat, gLTAIRLon, gLTAIRAlt;
var    groundLat, groundLong, groundEle;
var    GPSInUse;                                    // 0 = mast u-blox NEO-M8N, 1 = DFRobot u-blox G6010
var    orientInUse;                                 // 0 = mast Adafruit BNO055, 1 = interior CHRobotics UM7, 2 = mast Sparkfun HMC6343
var    radioInUse;                                  // 0 = DNT, 1 = SHX, 2 = RFM23BP
var    altairRSSI, groundRSSI, altairDropMessFrac, groundDropMessFrac;
var    altairAlt1RSSI, groundAlt1RSSI, altairAlt1DropMessFrac, groundAlt1DropMessFrac;
var    altairAlt2RSSI, groundAlt2RSSI, altairAlt2DropMessFrac, groundAlt2DropMessFrac;
var    microSDSpaceOccupied, microSDSpaceRemaining;
var    lightSourceStatus;
var    isCutdown;
var    cutdownSteeringServoRotAng;
var    heliumBleedValveRotAng;
var    controlGroundStationName;
var    numMonGroundStations;
var    monGroundStationName = [];

var    doTelescopeALTAIRTracking = false;

// var text;
var socket = new WebSocket("ws://localhost:8081");
var overButton = -999;

function preload() {
   if (audibleAlarms) alarm = loadSound('assets/Alarm.wav');
}

function setup() {
//   createCanvas(1350, 497, WEBGL);
   createCanvas(1350, 497);

   colorMode(RGB, 1);
   stroke(0);
   strokeWeight(0.5);
   smooth(5);

   for (var i = 0; i < numAlarms; ++i) alarmOn[i] =   0;
   for (var j = 0; j < 3        ; ++j)     age[j] = -99;

//    textAlign(CENTER);
//    textSize(32);
//    text("click mouse to add particle systems Power", width/2, height/2);
//    text("Power", 100, 200);

   textSize(17);
   textAlign(LEFT);

   socket.onopen = openSocket;
   socket.onmessage = receiveData;

   orientGraphics = createGraphics(150, 150, WEBGL);
   accelGraphics = createGraphics(115, 115, WEBGL);
   orientGraphics.colorMode(RGB, 1);
   accelGraphics.colorMode(RGB, 1);

   gLTAIRLat = window.document.getElementById('gLTAIRLat');
   gLTAIRLon = window.document.getElementById('gLTAIRLon');
   gLTAIRAlt = window.document.getElementById('gLTAIRAlt');
}

function draw() {
//   ellipse(50, 50, 80, 100);  
   background(1);

   getAltairArduinoInfoLine();
   displayPropulsionSystemInfo();
   displayUM7SystemInfo();
   displayGlobalStatusInfo();
   determineTelescopeAltAz();
}

function getAltairArduinoInfoLine() {
  if (testArduinoUnconnected) setFakeAltairValues();
  else {

  }
  ++altairPosCounter;
  if (altairPosCounter % timeBtwALTAIRPosComms == 0) {
    socket.send("ALTAIRPOS: "  + lat.toString() + " " + long.toString() + " " + ele.toString());
    socket.send("ALTAIRDATA: " + lat.toString() + " " + long.toString() + " " + ele.toString());
  }
}

function openSocket() {
//    text.html("Socket open");
    socket.send("LOG: Loaded+opened AIFCOMSS console");
}
 
function receiveData(incoming) {
  testArduinoUnconnected =  false;
  if (incoming.data != null) {
    var altairValues = split(incoming.data, ' ').map(val => parseInt(val,16));
//    var altairValues = split(incoming.data, ' ').map(parseFloat);
//    var altairValues = data.split(' ').map(parseFloat);
    altairValues.splice(0, 2);              // remove the "Data (HEX):" bits from the beginning of the array
    switch (altairValues.length) {
      case 44:
        procLongInArr(altairValues);
        break;
      case 34:
        procShortInArr(altairValues);
        break;
      case 16:
        procGSInArr(altairValues);
        break;
      default:
//        'Unparseable serial data received -- check logs!'
//        if (alarmOn[xy] == 0) alarmOn[xy] = 2;
    }
  }
}

function procLongInArr(altairValues) {
  socket.send("LOG: Heard a message of length " + 46 + " (43 vals), with 13th value = " + altairValues[12] + " and 14th value = " + altairValues[13]);
  if (altairValues[12] == regValue) {
    procGPS(altairValues);
    if (altairValues[28] == regValue) {
      procLongEnvMon(altairValues);
      if (altairValues[42] == regValue) {
        procLongOrientPropMon(altairValues);
      }
    }
  }
}

function procShortInArr(altairValues) {
  socket.send("LOG: Heard a message of length " + 36 + " (33 vals), with 12th value = " + altairValues[11] + " and 13th value = " + altairValues[12]);
  if (altairValues[11] == regValue) {     
    procTempsAndMon(altairValues);
    if (altairValues[24] == regValue) {
      procSpacePowServInfo(altairValues);
      if (altairValues[32] == regValue) {
        procLightInfo(altairValues);
      }
    }
  }
}

function procGSInArr(altairValues) {
  socket.send("LOG: Heard a message of length " + 18 + " (15 vals), with 15th value = " + altairValues[14] + " and 14th value = " + altairValues[13]);
  if (altairValues[14] == regValue) {
    procGSInfo(altairValues);
  }
}


function    procGPS(altairValues)   {
  gpsInfoReceivedTime = Date();
  var gpsLat     = ((altairValues[0] << 24) + (altairValues[1] << 16) + (altairValues[2] <<  8) + altairValues[3]) / 1000000. ;
  var gpsLon     = ((altairValues[4] << 24) + (altairValues[5] << 16) + (altairValues[6] <<  8) + altairValues[7]) / 1000000. ;
  var gpsEle     =  (altairValues[8] <<  8) +  altairValues[9];
  var gpsAge     =   altairValues[10];
  var hdop       =   altairValues[11];
  if (abs(gpsLat - lat) < maxGPSDelta && abs(gpsLon - long) < maxGPSDelta) {
      lat        =   gpsLat;
      long       =   gpsLon;
      ele        =   gpsEle;
      age[0]     =  0.256 * gpsAge; // in seconds
      horzSigma  =   hdop;          // fix
      vertSigma  =   hdop;          // fix
  }
}

function procLongEnvMon(altairValues) {
  var outPres    =  (altairValues[13] << 8) + altairValues[14];  // in units of 2 Pa
  pressure[1]    = 0.002 * outPres;                              // in units of  kPa
      temp[9]    =   altairValues[15];                           // in degrees C
  humidity[1]    =   altairValues[16];                           // in %
  var  inPres    =  (altairValues[17] << 8) + altairValues[18];  // in units of 2 Pa
  pressure[0]    = 0.002 * inPres;                               // in units of  kPa
      temp[8]    =   altairValues[19];                           // in degrees C
  humidity[0]    =   altairValues[20];                           // in %
  var balPres    =  (altairValues[21] << 8) + altairValues[22];  // in units of 2 Pa
  pressure[2]    = 0.002 * balPres;                              // in units of  kPa
      temp[10]   =   altairValues[23];                           // in degrees C
  humidity[2]    =   altairValues[24];                           // in %
  if (pressure[2] == 0. && temp[10] == 0. && humidity[2] == 0.) isCutdown = true;   // no data from sensor => must assume balloon has separated
  var rawAccelZ  =   altairValues[25];
  var rawAccelX  =   altairValues[26];
  var rawAccelY  =   altairValues[27];
      accel[0]   =  0.3333 * (rawAccelX - 120.); // In m/s^2.  These will be modified to subtract off Earth's gravitational acceleration in procLongOrientPropMon below.
      accel[1]   =  0.3333 * (rawAccelY - 120.); // In m/s^2.  These will be modified to subtract off Earth's gravitational acceleration in procLongOrientPropMon below.
      accel[2]   =  0.3333 * (rawAccelZ - 120.); // In m/s^2.  These will be modified to subtract off Earth's gravitational acceleration in procLongOrientPropMon below.
}

function procLongOrientPropMon(altairValues) {
      orientInfoReceivedTime = Date();
      age[1]     = 0.;
  var packCurr   = [];
  var rawYaw     = altairValues[29];
  var rawPitch   = altairValues[30];
  var rawRoll    = altairValues[31];
      UM7temp    = altairValues[32];
  var rawTypHeal = altairValues[33];
  if ((rawTypHeal &  7)  <  4)  { UM7health  =  0; }                         // healthy
  else                          { UM7health  =  1; }                         // unhealthy
  if ((rawTypHeal &  7) ==  0 || (rawTypHeal &  7) ==  4) orientInUse = 0;   // BNO055
  if ((rawTypHeal &  7) ==  1 || (rawTypHeal &  7) ==  5) orientInUse = 1;   // UM7
  if ((rawTypHeal &  7) ==  2 || (rawTypHeal &  7) ==  6) orientInUse = 2;   // HMC6343
  if ((rawTypHeal &  7) ==  3 || (rawTypHeal &  7) ==  7) orientInUse = 3;   // HMC5883L

  if ((rawTypHeal & 24) ==  0 || (rawTypHeal & 24) == 16) GPSInUse    = 0;   // NEO-M8N
  if ((rawTypHeal & 24) ==  8 || (rawTypHeal & 24) == 24) GPSInUse    = 1;   // DFRobot G6
  
  if ((rawTypHeal & 96) ==  0)  { socket.send('R0');      radioInUse  = 0; } // DNT900
  if ((rawTypHeal & 96) == 32)  { socket.send('R1');      radioInUse  = 1; } // SHX144
  if ((rawTypHeal & 96) == 64)  { socket.send('R2');      radioInUse  = 2; } // RFM23BP

      yaw        = 1.5   *  rawYaw;
      pitch      = 0.5   * (rawPitch - 120.);
      roll       = 0.5   * (rawRoll  - 120.);
// "Subtract off" Earth's gravitational acceleration
  accel[2] += gravAcc * Math.cos(radians(pitch)) * Math.cos(radians(roll));
  accel[1] += gravAcc * Math.sin(radians(pitch));
  accel[0] += gravAcc * Math.cos(radians(pitch)) * Math.sin(radians(roll));
  for (var   i   = 0; i < 4; ++i) {
         rpm[i]  =   altairValues[34+i] *   60 ;
    packCurr[i]  =   altairValues[38+i]        ;
    if (packCurr[i] > 127)  packCurr[i] -= 256 ;
     current[i]  = 0.25 *   packCurr[i]        ;
  }
}

function procTempsAndMon(altairValues) {
  for (var i  = 0; i < 8; ++i) {      
      temp[i] = 0.5 *   altairValues[i];   // These temps are sent down in units of 0.5 degrees C
  }                                        //  (see ALTAIR_GenTelInt.cpp and ALTAIRArduinoMicroRPMCurrentTempMon.ino).
  altairRSSI  =         altairValues[8];   // fix this
  voltage[0]  = 0.055 * altairValues[9];   // 
  voltage[1]  = 0.055 * altairValues[10];  // 
}

function procSpacePowServInfo(altairValues) {
  var  setProp = [];
  var  setServ = [];
  var  angServ = [];
  microSDSpaceOccupied  = (altairValues[12] << 8) + altairValues[13]    ;  // in MB
  microSDSpaceRemaining =  totalMicroSDSpace      - microSDSpaceOccupied;  // in MB
  for (var     i   = 0; i < 4; ++i) {
       setProp[i]  =       altairValues[14+i];
       setting[i]  = 0.1       *   setProp[i];
  }
  for (var     j   = 0; j < 3; ++j) {
       setServ[j]  = altairValues[18+(2*j)];
       angServ[j]  = altairValues[19+(2*j)];
  }
  setting[5]                 =  0.1  * setServ[0];
  setting[6]                 =  0.1  * setServ[1];
  setting[4]                 =  0.1  * setServ[2];
  rotAng                     =  0.02 * angServ[0];  // In volts for now, will change units to degrees later (when we measure the conversion factor).
  heliumBleedValveRotAng     =  0.02 * angServ[1];  // In volts for now, will change units to degrees later (when we measure the conversion factor).
  cutdownSteeringServoRotAng =  0.02 * angServ[2];  // In volts for now, will change units to degrees later (when we measure the conversion factor).
}

function procLightInfo(altairValues) {
      mon2InfoReceivedTime   =  Date();
      age[2]                 =  0.;
      lightSourceStatus      =  altairValues[25];
  var pd1ADRead              = (altairValues[26] <<  8) +  altairValues[27];  // in ADC units of 188uV/bit
  var pd2ADRead              = (altairValues[28] <<  8) +  altairValues[29];  // in ADC units of 188uV/bit
  var pd3ADRead              = (altairValues[30] <<  8) +  altairValues[31];  // in ADC units of 188uV/bit
      photodiodeReadout[0]   = 0.000188 * pd1ADRead;  // 188 uV is the volts per ADU on an ADS1115 ADC board
      photodiodeReadout[1]   = 0.000188 * pd2ADRead;  // 188 uV is the volts per ADU on an ADS1115 ADC board
      photodiodeReadout[2]   = 0.000188 * pd3ADRead;  // 188 uV is the volts per ADU on an ADS1115 ADC board
}

function procGSInfo(altairValues) {
  groundLat              = ((altairValues[0] << 24) + (altairValues[1] << 16) + (altairValues[2] <<  8) + altairValues[3]) / 1000000. ;
  groundLong             = ((altairValues[4] << 24) + (altairValues[5] << 16) + (altairValues[6] <<  8) + altairValues[7]) / 1000000. ;
  groundRSSI             =   altairValues[8] ;
  groundAlt1RSSI         =   altairValues[9] ;
  groundAlt2RSSI         =   altairValues[10];
  rawGroundDropMessFrac  =   altairValues[11];
  if ((rawGroundDropMessFrac & 192) ==  0) controlGroundStationName = groundStation0Name;
  if ((rawGroundDropMessFrac & 192) == 64) controlGroundStationName = groundStation1Name;
  rawAlt1DropMessFrac    =   altairValues[12];
  rawAlt2DropMessFrac    =   altairValues[13];
  groundDropMessFrac     = 1./63. * (rawGroundDropMessFrac & 63);
  groundAlt1DropMessFrac = 1./63. * (  rawAlt1DropMessFrac & 63);
  groundAlt2DropMessFrac = 1./63. * (  rawAlt2DropMessFrac & 63);
}

function displayPropulsionSystemInfo() {
  var upperMotorIsRed = 0, lowerMotorIsRed = 0, upperPropIsGreen = 0, lowerPropIsGreen = 0, upperPropIsRed = 0, lowerPropIsRed = 0;
  
  fill(0.8);
  noStroke();
  rect(500, 193, 110, 2);
  stroke(0);
  strokeWeight(0.7);
//  makeCylinder( 585., 280., 85, 105);
  rect(135, 300, 900, 5);
  makeBox( 565., 290., 150., 110.);
//  makeCylinder( 555., 190., 10,  15);
  makeBox( 553., 192., 18,  16);

  noStroke();
  rect(650, 301, 50, 3);
  textSize(10);
  drawType("1/2    Full",   640., 409., 0., 0., 0.);
  drawType("step  step",    640., 418., 0., 0., 0.);
  textSize(13);
  drawType("Modify power",  684., 433., 0., 0., 0.);
  drawType("to all motors", 684., 447., 0., 0., 0.);
  textSize(17);
  stroke(0);
  makeButtons(667., 421., 7);
  makeButtons(644., 421., 8);
  line(664, 300, 655, 300);
  line(664, 305, 655, 305);
  noFill();
//  makeCylinder(1263., 130., 65, 85);
  makeBox(1266., 135., 115, 88);

  for (var i = 0; i < 4; ++i) {
    var    isHigh = 0, isRight = 0;
    if (i == 0 || i == 3) {
           isHigh = 1;
    }
    if (i > 1) isRight = 1;

    var fillHeight = 40.*(setting[i]/(maxSetting*1.2)), fillHeightESC;
    var motorIsRed = 0, escIsRed = 0, propIsRed = 0, propIsGreen = 0;
    if (setting[i] > maxSetting || setting[i] < 0.) if (alarmOn[i] == 0) alarmOn[i] = 2;                                                      
    else                                            alarmOn[i] = 0;
    fill(color(1.,1.,1.));
    ellipse(18.  + 235.*i + 240.*isRight, 365.  - 30.*isHigh,                 4., 40.);
    fill(color(1.*(alarmOn[i] > 0 ? 1 : 0),0.7*(alarmOn[i] > 0 ? 0 : 1),0));
    noStroke();
    ellipse(18.5 + 235.*i + 240.*isRight, 383.5 - 30.*isHigh - fillHeight/2., 3., fillHeight);
    drawType("Power",              30. + 235.*i + 240.*isRight, 360. - 30.*isHigh, 1.*(alarmOn[i] > 0 ? 1 : 0),   0.,                             0.);
    drawType("set at:",            83. + 235.*i + 240.*isRight, 360. - 30.*isHigh, 1.*(alarmOn[i] > 0 ? 1 : 0),   0.,                             0.);
    textAlign(RIGHT);
    drawType(nf(setting[i],1,1),  163. + 235.*i + 240.*isRight, 360. - 30.*isHigh, 1.*(alarmOn[i] > 0 ? 1 : 0),   0.6*(alarmOn[i] > 0 ? 0 : 1),   0.);
    textSize(10);
    drawType(nf(int(maxSetting)),  14. + 235.*i + 240.*isRight, 356. - 30.*isHigh, 0.35,                          0.35,                           0.);    
    drawType("0",                  14. + 235.*i + 240.*isRight, 386. - 30.*isHigh, 0.35,                          0.35,                           0.);
    textSize(17);
    textAlign(LEFT);
    drawType("/10",               165. + 235.*i + 240.*isRight, 360. - 30.*isHigh,   0.,                          0.,                             0.);
    stroke(0);
    makeButtons(                  200. + 235.*i + 240.*isRight, 343. - 30.*isHigh,   i+3);

    fillHeight = 40.*(rpm[i]/(maxRPM*1.2));
    if (rpm[i] > 0.)                    { propIsGreen = 1;
                                          if (isHigh == 1)   { upperPropIsGreen = 1 - upperPropIsRed; }
                                          else               { lowerPropIsGreen = 1 - lowerPropIsRed; }
                                        }
    if (rpm[i] > maxRPM || rpm[i] < 0.) { motorIsRed = 1; propIsRed = 1; propIsGreen = 0;
                                          if (isHigh == 1)   { upperMotorIsRed = 1; upperPropIsRed = 1; upperPropIsGreen = 0; }
                                          else               { lowerMotorIsRed = 1; lowerPropIsRed = 1; lowerPropIsGreen = 0; }
                                          if (alarmOn[i+4] == 0) alarmOn[i+4] = 2;
                                        } 
    else                                { alarmOn[i+4] = 0; }
    fill(color(1.,1.,1.));
    ellipse(71.  + 235.*i + 240.*isRight, 390.  - 30.*isHigh,                 4., 40.);
    fill(color(1.*(alarmOn[i+4] > 0 ? 1 : 0), 0.7*(alarmOn[i+4] > 0 ? 0 : 1), 0));
    noStroke();
    ellipse(71.5 + 235.*i + 240.*isRight, 408.5 - 30.*isHigh - fillHeight/2., 3., fillHeight);
    drawType("RPM:",               83. + 235.*i + 240.*isRight, 400. - 30.*isHigh, 1.*(alarmOn[i+4] > 0 ? 1 : 0), 0.,                             0.); 
    textAlign(RIGHT);
    drawType(nf(int(rpm[i])),     173. + 235.*i + 240.*isRight, 400. - 30.*isHigh, 1.*(alarmOn[i+4] > 0 ? 1 : 0), 0.6*(alarmOn[i+4] > 0 ? 0 : 1), 0.);
    textSize(10);
    drawType(nf(int(maxRPM)),      67. + 235.*i + 240.*isRight, 381. - 30.*isHigh, 0.35,                          0.35,                           0.);    
    drawType("0",                  67. + 235.*i + 240.*isRight, 411. - 30.*isHigh, 0.35,                          0.35,                           0.);
    textSize(17);
    textAlign(LEFT);
    stroke(0);

    fillHeight    = 40.*((temp[i]   - minTemp)/((maxTemp - minTemp)*1.2));
    fillHeightESC = 40.*((temp[i+4] - minTemp)/((maxTemp - minTemp)*1.2));
    if (temp[i]   > maxTemp || temp[i]   < minTemp) { motorIsRed = 1;
                                                      if (isHigh == 1) { upperMotorIsRed = 1; }
                                                      else             { lowerMotorIsRed = 1; }
                                                      if (alarmOn[i+8] == 0)  alarmOn[i+8]  = 2;
                                                    } 
    else                                            { alarmOn[i+8]  = 0; }
    if (temp[i+4] > maxTemp || temp[i+4] < minTemp) { escIsRed = 1;
                                                      if (alarmOn[i+12] == 0) alarmOn[i+12] = 2;
                                                    } 
    else                                            { alarmOn[i+12] = 0; }
    fill(color(1.,1.,1.));
    ellipse( 71.  + 235.*i + 240.*isRight, 435.  - 30.*isHigh,                 4., 40.);
    ellipse(190.  + 190.*i + 115.*isRight, 250.              ,                 4., 40.);
    noStroke();
    fill(color(1.*(alarmOn[i+8]  > 0 ? 1 : 0), 0.7*(alarmOn[i+8]  > 0 ? 0 : 1), 0));
    ellipse( 71.5 + 235.*i + 240.*isRight, 453.5 - 30.*isHigh - fillHeight/2.,    3., fillHeight);
    fill(color(1.*(alarmOn[i+12] > 0 ? 1 : 0), 0.7*(alarmOn[i+12] > 0 ? 0 : 1), 0));
    ellipse(190.5 + 190.*i + 115.*isRight, 268.5              - fillHeightESC/2., 3., fillHeightESC);    
    drawType("Temp:",              83. + 235.*i + 240.*isRight, 440. - 30.*isHigh, 1.*(alarmOn[i+8]  > 0 ? 1 : 0), 0.,                              0.);
    drawType("Temp:",             202. + 190.*i + 115.*isRight, 265.             , 1.*(alarmOn[i+12] > 0 ? 1 : 0), 0.,                              0.);
    textAlign(RIGHT);
    drawType(nf(int(temp[i])),    173. + 235.*i + 240.*isRight, 440. - 30.*isHigh, 1.*(alarmOn[i+8]  > 0 ? 1 : 0), 0.6*(alarmOn[i+8]  > 0 ? 0 : 1), 0.);
    drawType(nf(int(temp[i+4])),  282. + 190.*i + 115.*isRight, 265.             , 1.*(alarmOn[i+12] > 0 ? 1 : 0), 0.6*(alarmOn[i+12] > 0 ? 0 : 1), 0.);
    textSize(10);
    drawType(nf(int(maxTemp)),     67. + 235.*i + 240.*isRight, 426. - 30.*isHigh, 0.35,                           0.35,                            0.);    
    drawType(nf(int(minTemp)),     67. + 235.*i + 240.*isRight, 456. - 30.*isHigh, 0.35,                           0.35,                            0.);
    drawType(nf(int(maxTemp)),    186. + 190.*i + 115.*isRight, 241.             , 0.35,                           0.35,                            0.);    
    drawType(nf(int(minTemp)),    186. + 190.*i + 115.*isRight, 271.             , 0.35,                           0.35,                            0.);
    textAlign(LEFT);
    drawType("o",                 178. + 235.*i + 240.*isRight, 431. - 30.*isHigh, 0.,                             0.,                              0.);
    drawType("o",                 287. + 190.*i + 115.*isRight, 256.             , 0.,                             0.,                              0.);
    textSize(17);
    drawType("C",                 183. + 235.*i + 240.*isRight, 440. - 30.*isHigh, 0.,                             0.,                              0.);
    drawType("C",                 292. + 190.*i + 115.*isRight, 265.             , 0.,                             0.,                              0.);
    stroke(0);

    fillHeight = 40.*(current[i]/(maxCurrent*1.2));
    if (current[i] > maxCurrent || current[i] < 0.) { motorIsRed = 1; escIsRed = 1;
                                                      if (isHigh == 1) { upperMotorIsRed = 1; }
                                                      else             { lowerMotorIsRed = 1; }
                                                      if (alarmOn[i+16] == 0) alarmOn[i+16] = 2;
                                                    } 
    else                                            { alarmOn[i+16] = 0; }
    fill(color(1.,1.,1.));
    ellipse(71.  + 235.*i + 240.*isRight, 480.  - 30.*isHigh,                 4., 40.);
    fill(color(1.*(alarmOn[i+16] > 0 ? 1 : 0), 0.7*(alarmOn[i+16] > 0 ? 0 : 1), 0));
    noStroke();
    ellipse(71.5 + 235.*i + 240.*isRight, 498.5 - 30.*isHigh - fillHeight/2., 3., fillHeight);
    drawType("Curr.:",             83. + 235.*i + 240.*isRight, 480. - 30.*isHigh, 1.*(alarmOn[i+16] > 0 ? 1 : 0), 0.,                                0.);
    textAlign(RIGHT);
    drawType(nf(int(current[i])), 173. + 235.*i + 240.*isRight, 480. - 30.*isHigh, 1.*(alarmOn[i+16] > 0 ? 1 : 0), 0.6*(alarmOn[i+16] > 0 ? 0 : 1),   0.);
    textSize(10);
    drawType(nf(int(maxCurrent)),  67. + 235.*i + 240.*isRight, 471. - 30.*isHigh, 0.35,                           0.35,                              0.);    
    drawType("0",                  67. + 235.*i + 240.*isRight, 501. - 30.*isHigh, 0.35,                           0.35,                              0.);
    textSize(17);
    textAlign(LEFT);
    drawType("A",                 178. + 235.*i + 240.*isRight, 480. - 30.*isHigh, 0.,                             0.,                                0.);
    stroke(0);

    fill(1.0, 0.5*(1.0-motorIsRed), 0.0); 
    makeCylinder(139. + 210.*i + 260.*isRight, 306. - 20.*isHigh, 8., 15.);
    fill(1.0, 0.5*(1.0-escIsRed),   0.0);
    rect(        225. + 195.*i + 105.*isRight, 294.,             18.,  6.);

    push();
    translate(139. + 210.*i + 260.*isRight, 325. - 43.*isHigh);
//    if (propIsGreen > 0 || propIsRed > 0) { rotateY(propRotAng[i]); ++propRotAng[i]; }
    fill(1.0*propIsRed, 1.0*propIsGreen, 0.0);
    if (propIsGreen > 0 || propIsRed > 0) {
      beginShape(TRIANGLES);
      vertex(   0.,  0.);
      vertex( 105.*sin(frameCount), -3.);
      vertex( 105.*sin(frameCount),  3.);
      vertex(   0.,  0.);
      vertex(-105.*sin(frameCount), -3.);
      vertex(-105.*sin(frameCount),  3.);
      endShape();
    } else {
      beginShape(TRIANGLES);
      vertex(   0.,  0.);
      vertex( 105., -3.);
      vertex( 105.,  3.);
      vertex(   0.,  0.);
      vertex(-105., -3.);
      vertex(-105.,  3.);
      endShape();
    }
    pop();

  }

  push();
//  translate(1052., 190., 0.);
  translate(1270., 160.);
//  rotateZ(radians(rotAng));
  rotate(radians(rotAng));
  fill(0.8);
  rect(-2., -2., 5., 5.);
  fill(1.0, 0.5*(1.0-upperMotorIsRed), 0.0);
  makeCylinder(0., -17.5, 8., 15.);
  fill(1.0, 0.5*(1.0-lowerMotorIsRed), 0.0);
  makeCylinder(0.,   4.5, 8., 15.);
  push();
  translate(0., -22.);
//  rotateX(radians(-28.));
//  if (upperPropIsGreen > 0 || upperPropIsRed > 0) { rotateY(propRotAng[4]); ++propRotAng[4]; }
//  rotate(radians(-28.));
//  if (upperPropIsGreen > 0 || upperPropIsRed > 0) { rotate(propRotAng[4]); ++propRotAng[4]; }
  fill(1.0*upperPropIsRed, 1.0*upperPropIsGreen, 0.0);
//  vertex(   0.,  0.,  0.);
//  vertex(  75., -2., -2.);
//  vertex(  75.,  2.,  2.);
//  vertex(   0.,  0.,  0.);
//  vertex( -75., -2.,  2.);
//  vertex( -75.,  2., -2.);
  if (upperPropIsGreen > 0 || upperPropIsRed > 0) { 
    beginShape(TRIANGLES);
    vertex(   0.,  0.);
    vertex(  75.*sin(frameCount), -2.);
    vertex(  75.*sin(frameCount),  2.);
    vertex(   0.,  0.);
    vertex( -75.*sin(frameCount), -2.);
    vertex( -75.*sin(frameCount),  2.);
    endShape();
  } else {
    beginShape(TRIANGLES);
    vertex(   0.,  0.);
    vertex(  75., -2.);
    vertex(  75.,  2.);
    vertex(   0.,  0.);
    vertex( -75., -2.);
    vertex( -75.,  2.);
    endShape();
  }
  pop();
  push();
  translate(0.,  22.);
//  rotateX(radians(-28.));
//  if (lowerPropIsGreen > 0 || lowerPropIsRed > 0) { rotateY(propRotAng[5]); ++propRotAng[5]; }
//  rotate(radians(-28.));
//  if (lowerPropIsGreen > 0 || lowerPropIsRed > 0) { rotate(propRotAng[5]); ++propRotAng[5]; }
  fill(1.0*lowerPropIsRed, 1.0*lowerPropIsGreen, 0.0);
//  vertex(   0.,  0.,  0.);
//  vertex(  75., -2., -2.);
//  vertex(  75.,  2.,  2.);
//  vertex(   0.,  0.,  0.);
//  vertex( -75., -2.,  2.);
//  vertex( -75.,  2., -2.);
  if (lowerPropIsGreen > 0 || lowerPropIsRed > 0) { 
    beginShape(TRIANGLES);
    vertex(   0.,  0.);
    vertex(  75.*sin(frameCount), -2.);
    vertex(  75.*sin(frameCount),  2.);
    vertex(   0.,  0.);
    vertex( -75.*sin(frameCount), -2.);
    vertex( -75.*sin(frameCount),  2.);
    endShape();
  } else {
    beginShape(TRIANGLES);
    vertex(   0.,  0.);
    vertex(  75., -2.);
    vertex(  75.,  2.);
    vertex(   0.,  0.);
    vertex( -75., -2.);
    vertex( -75.,  2.);
    endShape();
  }
  pop();
  pop();

}

function displayUM7SystemInfo() {

  var  orientInUseName,  alt1OrientName,  alt2OrientName;

  // default (orientInUse == 0)
  orientInUseName      = "mast Adafruit BNO055";
  alt1OrientName       = "inter. CHRobotics UM7";
  alt2OrientName       = "mast Sparkfun HMC6343";

  if (orientInUse == 1) {
      orientInUseName  = "inter. CHRobotics UM7";
      alt1OrientName   = "mast Adafruit BNO055";
      alt2OrientName   = "mast Sparkfun HMC6343";
  } else if (orientInUse == 2) {
      orientInUseName  = "mast Sparkfun HMC6343";
      alt1OrientName   = "mast Adafruit BNO055";
      alt2OrientName   = "inter. CHRobotics UM7";
  }

  accelGraphics.background(1.0, 1.0, 1.0);
  orientGraphics.background(1.0, 1.0, 1.0);

//  accelGraphics.rotateX(frameCount * 0.0001);
//  accelGraphics.rotateY(frameCount * 0.0001);

//  accelGraphics.rotateX(0.2);
//  accelGraphics.rotateZ(0.5);
//  accelGraphics.camera(30.*accel[0], -30.*accel[2], 0.);
//  accelGraphics.translate(0.,0.,-150.);
//  accelGraphics.translate(-75.,-75.,-150.);
//  accelGraphics.translate(-20.,-20.,-70.);
  accelGraphics.rotateY(radians(yaw));
  accelGraphics.rotateZ(-radians(pitch));
  accelGraphics.rotateX(radians(roll));

  accelGraphics.translate(40.*accel[0],0.,0.);
  accelGraphics.ambientLight(0.9);
  accelGraphics.ambientMaterial(0.83, 0.08, 0.08);
  accelGraphics.box(80.*accel[0], 5., 5.);

  accelGraphics.translate(-40.*accel[0],-40.*accel[2],0.);
  accelGraphics.ambientMaterial(0.05, 0.09, 0.85);
  accelGraphics.box(5., 80.*accel[2], 5.);

  accelGraphics.translate(0.,40.*accel[2],-40.*accel[1]);
  accelGraphics.ambientMaterial(0., 0.7, 0.);
  accelGraphics.box(5., 5., -80.*accel[1]);

//  accelGraphics.translate(40.*accel[0],0.,40.*accel[1]);
  accelGraphics.translate(0.,0.,40.*accel[1]);
//  accelGraphics.rotateZ(-0.5);
//  accelGraphics.rotateX(-0.2);
  accelGraphics.rotateX(-radians(roll));
  accelGraphics.rotateZ(radians(pitch));
  accelGraphics.rotateY(-radians(yaw));
//  accelGraphics.translate(75.,75.,150.);
//  accelGraphics.translate(20.,20.,70.);
//  accelGraphics.translate(0.,0.,150.);
//  accelGraphics.camera(-30.*accel[0], 30.*accel[2], 0.);

  // image(accelGraphics, 0, 0, 115, 115, 768, 37, 115, 115);
  image(accelGraphics, 768, 37, 115, 115, 0, 0, 115, 115);

//  orientGraphics.camera(30., -30., 0.);
//  orientGraphics.translate(0.,0.,-150.);
//  orientGraphics.translate(-110.,-65.,-150.);
  orientGraphics.translate(-30.,20.,0.);
  orientGraphics.rotateY(radians(yaw));
  orientGraphics.rotateZ(-radians(pitch));
  orientGraphics.rotateX(radians(roll));

  orientGraphics.translate(40.,0.,0.);
  orientGraphics.ambientLight(0.9);
  orientGraphics.ambientMaterial(0.83, 0.08, 0.08);
  orientGraphics.box(80., 5., 5.);
  orientGraphics.translate(40.,0.,0.);
  orientGraphics.rotateZ(-radians(90.));
  orientGraphics.cone(10., 20.);
  orientGraphics.rotateZ(radians(90.));

  orientGraphics.translate(-80.,-40.,0.);
  orientGraphics.ambientMaterial(0.05, 0.09, 0.85);
  orientGraphics.box(5., 80., 5.);
  orientGraphics.translate(0.,-40.,0.);
  orientGraphics.rotateX(-radians(180.));
  orientGraphics.cone(10., 20.);
  orientGraphics.rotateX(radians(180.));

  orientGraphics.translate(0.,80.,-40.);
  orientGraphics.ambientMaterial(0., 0.7, 0.);
  orientGraphics.box(5., 5., -80.);
  orientGraphics.translate(0.,0.,-40.);
  orientGraphics.rotateX(-radians(90.));
  orientGraphics.cone(10., 20.);
  orientGraphics.rotateX(radians(90.));

  orientGraphics.translate(0.,0.,80.);
  orientGraphics.rotateX(-radians(roll));
  orientGraphics.rotateZ(radians(pitch));
  orientGraphics.rotateY(-radians(yaw));
//  orientGraphics.translate(110.,65.,150.);
  orientGraphics.translate(30.,-20.,0.);
//  orientGraphics.translate(0.,0.,150.);
//  orientGraphics.camera(-30., 30., 0.);

  // image(orientGraphics, 0, 0, 150, 150, 880, 18, 150, 150);
  image(orientGraphics, 880, 18, 150, 150, 0, 0, 150, 150);

  noStroke();

  drawType("Z", 940.+
           70.*Math.sin(radians(pitch)),  
                                       160.-
               70.*Math.cos(radians(pitch))-
               50.*Math.cos(radians(roll)),  0.4,                       0.6,                           1.0);
//  drawType("X", 995.               , 110., 0.9,                       0.5,                           0.5);
  drawType("X", 880.+ 
           50.*Math.cos(radians(yaw))+
           70.*Math.cos(radians(pitch)), 
                                       100.+
               70.*Math.sin(radians(pitch)), 0.9,                       0.5,                           0.5);
  drawType("Y", 925.-                
           50.*Math.sin(radians(yaw)), 130.-
               50.*Math.sin(radians(roll)),  0.,                        0.7,                           0.);


  // print out text containing orientation, acceleration, and health info
  drawType("Yaw:  ",            1046.,  40., 0.,                        0.,                            0.);
  textAlign(RIGHT);
  drawType(nf(int(yaw)),        1123.,  40., 0.,                        0.7,                           0.);
  textSize(10);
  drawType("o",                 1130.,  33., 0.,                        0.,                            0.);
  textSize(17);
  textAlign(LEFT);
  drawType("Pitch:",            1046.,  65., 0.,                        0.,                            0.);
  textAlign(RIGHT);
  drawType(nf(int(pitch)),      1123.,  65., 0.,                        0.7,                           0.);
  textSize(10);
  drawType("o",                 1130.,  58., 0.,                        0.,                            0.);
  textSize(17);
  textAlign(LEFT);
  drawType("Roll: ",            1046.,  90., 0.,                        0.,                            0.);
  textAlign(RIGHT);
  drawType(nf(int(roll)),       1123.,  90., 0.,                        0.7,                           0.);
  textSize(10);
  drawType("o",                 1130.,  83., 0.,                        0.,                            0.);
  textSize(17);
  textAlign(LEFT);

  drawType("Axle Rotation",     1170.,  20., 0.,                        0.,                            0.);
  drawType("Servo set at:",     1170.,  65., 0.,                        0.,                            0.);
  textAlign(RIGHT);
  drawType(nf(setting[5],1,1),  1308.,  65., 0.,                        0.7,                           0.);
  textAlign(LEFT);
  drawType("/10",               1308.,  65., 0.,                        0.,                            0.);
  makeButtons(                  1152.,  47., 0);
  noStroke();
  drawType("Meas rot ang:  ",   1170.,  90., 0.,                        0.,                            0.);
  textAlign(RIGHT);
  drawType(nf(int(rotAng)),     1303.,  90., 0.,                        0.7,                           0.);
  textAlign(LEFT);
  textSize(10);
  drawType("o",                 1303.,  83., 0.,                        0.,                            0.);
  textSize(17);
  drawType("Accel    :",         625.,  40., 0.,                        0.,                            0.);
  drawType("z",                  675.,  40., 0.4,                       0.6,                           1.0);
  textAlign(RIGHT);
  drawType(nfp(accel[2],1,1),    730.,  40., 0.,                        0.7,                           0.);
  textAlign(LEFT);
  drawType("m/s",                735.,  40., 0.,                        0.,                            0.);
  textSize(10);
  drawType("2",                  762.,  33., 0.,                        0.,                            0.);
  textSize(17);
  drawType("Accel    :",         625.,  65., 0.,                        0.,                            0.);
  drawType("x",                  675.,  65., 0.9,                       0.5,                           0.5);
  textAlign(RIGHT);
  drawType(nfp(accel[0],1,1),    730.,  65., 0.,                        0.7,                           0.);
  textAlign(LEFT);
  drawType("m/s",                735.,  65., 0.,                        0.,                            0.);
  textSize(10);
  drawType("2",                  762.,  58., 0.,                        0.,                            0.);
  textSize(17);
  drawType("Accel    :",         625.,  90., 0.,                        0.,                            0.);
  drawType("y",                  675.,  90., 0.,                        0.7,                           0.);
  textAlign(RIGHT);
  drawType(nfp(accel[1],1,1),    730.,  90., 0.,                        0.7,                           0.);
  textAlign(LEFT);
  drawType("m/s",                735.,  90., 0.,                        0.,                            0.);
  textSize(10);
  drawType("2",                  762.,  83., 0.,                        0.,                            0.);
  textSize(17);
  drawType("Acceleration",       660.,  18., 0.,                        0.,                            0.);
  drawType("Orientation",        970.,  18., 0.,                        0.,                            0.);
  textSize(13);
  drawType("via",                777.,  12., 0.,                        0.,                            0.);
  drawType(orientInUseName,      797.,  12., 0.,                        0.7,                           0.);
  makeChangeOrientButtons();

  drawType("Orientation sensor", 697., 180., UM7health,                 0.,                            0.);
  drawType("health: ",           697., 200., UM7health,                 0.,                            0.);
  if (UM7health == 0) {
    alarmOn[20] = 0;
    drawType("OK",               767., 200., 0.,                        0.7,                           0.);
  } else {
    if (alarmOn[20] == 0) alarmOn[20] = 2;
    drawType("PROBLEM",          767., 200., 1.,                        0.,                            0.);
  }

  if (UM7temp > maxTemp || UM7temp < minTemp) if (alarmOn[21] == 0) alarmOn[21] = 2;
  else                                        alarmOn[21] = 0;
  var fillHeight = 40.*((UM7temp - minTemp)/((maxTemp - minTemp)*1.2));
  fill(color(1.,                           1.,                            1.));
  stroke(0);
  ellipse(875.,  185.                  , 4., 40.);
  noStroke();
  fill(color(1.*(alarmOn[21] > 0 ? 1 : 0), 0.7*(alarmOn[21] > 0 ? 0 : 1), 0.));
  ellipse(875.5, 203.5  - fillHeight/2., 3., fillHeight);    
  drawType("Orientation sensor", 887., 180., (alarmOn[21] > 0 ? 1 : 0), 0.,                            0.);
  drawType("temp: ",             887., 200., (alarmOn[21] > 0 ? 1 : 0), 0.,                            0.);
  textAlign(RIGHT);
  drawType(nf(int(UM7temp)),     967., 200., (alarmOn[21] > 0 ? 1 : 0), 0.7*(alarmOn[21] > 0 ? 0 : 1), 0.);
  textSize(10);
  drawType(nf(int(maxTemp)),     871., 176., 0.35,                      0.35,                          0.);    
  drawType(nf(int(minTemp)),     871., 206., 0.35,                      0.35,                          0.);
  textAlign(LEFT);
  drawType("o",                  972., 190., 0.,                        0.,                            0.);
  textSize(17);
  drawType("C",                  977., 200., 0.,                        0.,                            0.);


}

function displayGlobalStatusInfo() {

  makeLightSourceButtons();
  displaySDCardInfo();
  displayCutdownSteeringAndEnvInfo();
  displayGPSInfo();
  displayConnectionInfo();
  displayTimeInfo();
  makeAutomationButtons();

}

function displaySDCardInfo() {

  var diskFilledFraction = microSDSpaceOccupied/(microSDSpaceRemaining+microSDSpaceOccupied);
  var diskFillHeight     = 145. - (30.*diskFilledFraction);

  if (diskFilledFraction > maxDiskSpaceFraction) if (alarmOn[22] == 0) alarmOn[22] = 2;
  else                                           alarmOn[22] = 0;

  noStroke();
  drawType("Onboard microSD",   1042., 164., 0.,                        0.,                            0.);
  drawType("card disk space",   1047., 180., 0.,                        0.,                            0.);
  drawType("Occupied:",         1025., 205., 0.,                        0.,                            0.);
  drawType("MB",                1165., 205., 0.,                        0.,                            0.);
  drawType("Remaining:",        1025., 223., 0.,                        0.,                            0.);
  drawType("MB",                1165., 223., 0.,                        0.,                            0.);
  textAlign(RIGHT);
  drawType(nf(int(microSDSpaceOccupied)),   1160.,  205., (alarmOn[22] > 0 ? 1 : 0), 0.7*(alarmOn[22] > 0 ? 0 : 1), 0.);
  drawType(nf(int(microSDSpaceRemaining)),  1160.,  223., (alarmOn[22] > 0 ? 1 : 0), 0.7*(alarmOn[22] > 0 ? 0 : 1), 0.);
  textAlign(LEFT);

  fill(color(1.*(alarmOn[21] > 0 ? 1 : 0), 0.7*(alarmOn[21] > 0 ? 0 : 1), 0.));
  beginShape();
  vertex(1120., 145.);
  vertex(1100., 145.);
  vertex(1100., (diskFillHeight > 120. ? diskFillHeight : 120.));
  vertex(1105., diskFillHeight);
  vertex(1120., diskFillHeight);
  endShape();
  stroke(0);
  noFill();
  beginShape();
  vertex(1100., 120.);
  vertex(1105., 115.);
  vertex(1120., 115.);
  vertex(1120., 145.);
  vertex(1100., 145.);
  vertex(1100., 120.);
  endShape();
}

function displayCutdownSteeringAndEnvInfo() {

  if (!isCutdown) {
    fill(1.0, 1.0, 0.9);
    ellipse(555., 65.,100.,120.); 
    fill(1.0, 0.5, 0.5);
    line(555.,120.,555.,145.);
    line(545.,160.,550.,189.);
    line(565.,160.,560.,189.);
    triangle(525.,160.,585., 160.,555.,145.);

    fill(0., 0., 0.);
    triangle(555.,125.,565., 120.,565.,130.);
    triangle(555.,125.,545., 120.,545.,130.);
    line(550.,120.,560.,120.);
    line(480., 60.,540.,118.);
    line(540.,113.,540.,118.);
    line(535.,118.,540.,118.);
  } else {
    fill(1.0, 0.5, 0.5);
    line(520., 97.,550.,189.);
    line(590., 97.,560.,189.);
    triangle(485., 97.,625.,  97.,555., 72.);
  }
  line(586.,165.,555.,201.);
  line(555.,195.,555.,201.);
  line(561.,201.,555.,201.);

  noStroke();

  textSize(13);
  if (!isCutdown) {
    drawType("Pres:",                              509.,  57., 0.,                        0.,                            0.);
    drawType("kPa",                                581.,  57., 0.,                        0.,                            0.);
    drawType("Temp:",                              509.,  71., 0.,                        0.,                            0.);
    drawType("C",                                  584.,  71., 0.,                        0.,                            0.);
    drawType("Hum:" ,                              509.,  85., 0.,                        0.,                            0.);
    drawType("%",                                  581.,  85., 0.,                        0.,                            0.);

    drawType("Helium bleed",                       427.,  18., 0.,                        0.,                            0.);
    drawType("valve:",                             423.,  31., 0.,                        0.,                            0.);
    if (heliumBleedValveRotAng < 5.) {
      drawType("CLOSED",                           458.,  31., 0.,                        0.7,                           0.);
    } else {
      alarmOn[27] = 2;
      drawType("OPEN",                             458.,  31., 1.,                        0.,                            0.);
    }
    textAlign(RIGHT);
    drawType(nfp(int(heliumBleedValveRotAng)),     474.,  59., 0.,                        0.7,                           0.);
    drawType(nf(setting[6],1,1),                   485.,  45., 0.,                        0.7,                           0.);
    textAlign(LEFT);
    drawType("Servo:",                             423.,  45., 0.,                        0.,                            0.);
    drawType("/16",                                486.,  45., 0.,                        0.,                            0.);
    makeButtons(                   437.,  47., 1);
    noStroke();
  }

  drawType("Pres:",                                597., 112., 0.,                        0.,                            0.);
  drawType("kPa",                                  669., 112., 0.,                        0.,                            0.);
  drawType("Temp:",                                597., 126., 0.,                        0.,                            0.);
  drawType("C",                                    672., 126., 0.,                        0.,                            0.);
  drawType("Hum:" ,                                597., 140., 0.,                        0.,                            0.);
  drawType("%",                                    669., 140., 0.,                        0.,                            0.);

  drawType("Pres:",                                597., 158., 0.,                        0.,                            0.);
  drawType("kPa",                                  669., 158., 0.,                        0.,                            0.);
  drawType("Temp:",                                597., 172., 0.,                        0.,                            0.);
  drawType("C",                                    672., 172., 0.,                        0.,                            0.);
  drawType("Hum:" ,                                597., 186., 0.,                        0.,                            0.);
  drawType("%",                                    669., 186., 0.,                        0.,                            0.);

  textAlign(RIGHT);
  if (!isCutdown) {
    drawType(nf(pressure[2],3,2),                  580.,  57., 0.,                        0.7,                           0.);
    drawType(nf(int(temp[10])),                    580.,  71., 0.,                        0.7,                           0.);
    drawType(nf(int(humidity[2])),                 580.,  85., 0.,                        0.7,                           0.);
  }
  drawType(nf(pressure[1],3,2),                    668., 112., 0.,                        0.7,                           0.);
  drawType(nf(int(temp[9])),                       668., 126., 0.,                        0.7,                           0.);
  drawType(nf(int(humidity[1])),                   668., 140., 0.,                        0.7,                           0.);
  drawType(nf(pressure[0],3,2),                    668., 158., 0.,                        0.7,                           0.);
  drawType(nf(int(temp[8])),                       668., 172., 0.,                        0.7,                           0.);
  drawType(nf(int(humidity[0])),                   668., 186., 0.,                        0.7,                           0.);
  textAlign(LEFT);

  textSize(7);
  if (!isCutdown) { 
    drawType("o",                                  581.,  64., 0.,                        0.,                            0.);
    drawType("o",                                  474.,  53., 0.,                        0.,                            0.);
  }
  drawType("o",                                    669., 119., 0.,                        0.,                            0.);
  drawType("o",                                    669., 165., 0.,                        0.,                            0.);
  textSize(40);
  drawType("{",                                    584., 178., 0.,                        0.,                            0.);
  textSize(17);

  drawType("Cutdown status:",                      568., 210., 0.,                        0.,                            0.);
  if (isCutdown) { 
    drawType("Steering:",                          440.,  25., 0.,                        0.,                            0.);
    drawType("Servo",                              451.,  51., 0.,                        0.,                            0.);
    drawType("Setting:",                           451.,  67., 0.,                        0.,                            0.);
    drawType("/10",                                558.,  65., 0.,                        0.,                            0.);
    makeButtons(                   592.,  47., 2);
    noStroke();
    if (cutdownSteeringServoRotAng > 0.) {
      drawType("RIGHT",                            512.,  25., 0.,                        0.7,                           0.);
    } else if (cutdownSteeringServoRotAng < 0.) {
      drawType("LEFT",                             512.,  25., 0.,                        0.7,                           0.);
    } else {
      drawType("CENTRED",                          512.,  25., 0.,                        0.7,                           0.);
    }
    textAlign(RIGHT);
    drawType(nfp(int(cutdownSteeringServoRotAng)), 556.,  45., 0.,                        0.7,                           0.);
    drawType(nf(setting[4],1,1),                   556.,  65., 0.,                        0.7,                           0.);
    textAlign(LEFT);
    textSize(10);
    drawType("o",                                  556.,  37., 0.,                        0.,                            0.);
    textSize(13);
    drawType("IS CUTDOWN",                         585., 223., 0.,                        0.7,                           0.);
  } else {
    textSize(13);
    drawType("NOT CUTDOWN",                        580., 223., 0.,                        0.7,                           0.);
    drawType("Servo setting:",                     537., 237., 0.,                        0.,                            0.);
    drawType("/10",                                648., 237., 0.,                        0.,                            0.);
    drawType("Servo encoder:",                     537., 252., 0.,                        0.,                            0.);
    textAlign(RIGHT);
    drawType(nf(setting[4],1,1),                   647., 237., 0.,                        0.7,                           0.);
    drawType(nfp(int(cutdownSteeringServoRotAng)), 647., 252., 0.,                        0.7,                           0.);
    textAlign(LEFT);
    textSize(7);
    drawType("o",                                  647., 246., 0.,                        0.,                            0.);
  }
  textSize(17);


  var batt1ChargeFraction = (voltage[0] - minVoltage)/(maxVoltage - minVoltage);
  var batt2ChargeFraction = (voltage[1] - minVoltage)/(maxVoltage - minVoltage);
  var batt1FillHeight     = 195. - (20.*batt1ChargeFraction);
  var batt2FillHeight     = 195. - (20.*batt2ChargeFraction);

  if (voltage[0] < minVoltage || voltage[0] > maxVoltage) if (alarmOn[25] == 0) alarmOn[25] = 2;
  else                                                    alarmOn[25] = 0;
  if (voltage[1] < minVoltage || voltage[1] > maxVoltage) if (alarmOn[26] == 0) alarmOn[26] = 2;
  else                                                    alarmOn[26] = 0;

  drawType("Battery 1 (                       )",   65., 179., 0.,                        0.,                            0.);
  drawType("gen. operation",                       144., 179., 0.,                        0.,                            1.);
  drawType("Voltage =",                             92., 197., 0.,                        0.,                            0.);
  drawType("V",                                    203., 197., 0.,                        0.,                            0.);

  drawType("Battery 2 (                 )",        334., 179., 0.,                        0.,                            0.);
  drawType("propulsion",                           413., 179., 0.,                        0.,                            1.);
  drawType("Voltage =",                            348., 197., 0.,                        0.,                            0.);
  drawType("V",                                    459., 197., 0.,                        0.,                            0.);

  textAlign(RIGHT);
  drawType(nf(voltage[0],2,1),                     200., 197., (alarmOn[25] > 0 ? 1 : 0), 0.7*(alarmOn[25] > 0 ? 0 : 1), 0.);
  drawType(nf(voltage[1],2,1),                     456., 197., (alarmOn[26] > 0 ? 1 : 0), 0.7*(alarmOn[26] > 0 ? 0 : 1), 0.);

  textSize(10);
  drawType(nf(maxVoltage,2,1),                      22., 179., 0.35,                      0.35,                           0.);    
  drawType(nf(minVoltage,2,1),                      22., 199., 0.35,                      0.35,                           0.);
  drawType(nf(maxVoltage,2,1),                     289., 179., 0.35,                      0.35,                           0.);    
  drawType(nf(minVoltage,2,1),                     289., 199., 0.35,                      0.35,                           0.);
  textSize(17);

  textAlign(LEFT);

  fill(color(1.*(alarmOn[25] > 0 ? 1 : 0), 0.7*(alarmOn[25] > 0 ? 0 : 1), 0.));
  beginShape();
  vertex(59., 195.);
  vertex(24., 195.);
  vertex(24., batt1FillHeight);
  vertex(59., batt1FillHeight);
  endShape();

  fill(color(1.*(alarmOn[26] > 0 ? 1 : 0), 0.7*(alarmOn[26] > 0 ? 0 : 1), 0.));
  beginShape();
  vertex(326., 195.);
  vertex(291., 195.);
  vertex(291., batt2FillHeight);
  vertex(326., batt2FillHeight);
  endShape();


  stroke(0);
  noFill();
  beginShape();
  vertex(59., 195.);
  vertex(24., 195.);
  vertex(24., 175.);
  vertex(29., 175.);
  vertex(29., 171.);
  vertex(33., 171.);
  vertex(33., 175.);
  vertex(50., 175.);
  vertex(50., 171.);
  vertex(54., 171.);
  vertex(54., 175.);
  vertex(59., 175.);
  vertex(59., 195.);
  endShape();

  beginShape();
  vertex(326., 195.);
  vertex(291., 195.);
  vertex(291., 175.);
  vertex(296., 175.);
  vertex(296., 171.);
  vertex(300., 171.);
  vertex(300., 175.);
  vertex(317., 175.);
  vertex(317., 171.);
  vertex(321., 171.);
  vertex(321., 175.);
  vertex(326., 175.);
  vertex(326., 195.);
  endShape();

  noStroke();

}

function displayGPSInfo() {

  var GPSInUseName;

  // default (GPSInUse == 0)
  GPSInUseName     = "mast NEO-M8N";
  if (GPSInUse == 1) {
      GPSInUseName = "DFRobot G6010";
  }

  textSize(15);
  drawType("GPS",                    287.,  13., 0.,                        0.,                            0.);
  textSize(13);
  drawType("via",                    287.,  28., 0.,                        0.,                            0.);
  drawType(GPSInUseName,             307.,  28., 0.,                        0.7,                           0.);
  makeChangeGPSButton();

  textSize(17);
  drawType("Lat. =",                 260.,  47., 0.,                        0.,                            0.);
  if (lat > 0.)  drawType("N",       408.,  47., 0.,                        0.,                            0.);
  else           drawType("S",       408.,  47., 0.,                        0.,                            0.);
  drawType("Long. =",                260.,  69., 0.,                        0.,                            0.);
  if (long > 0.) drawType("E",       408.,  69., 0.,                        0.,                            0.);
  else           drawType("W",       408.,  69., 0.,                        0.,                            0.);
  drawType("Elev. =",                260.,  91., 0.,                        0.,                            0.);
  drawType("m ASL",                  408.,  91., 0.,                        0.,                            0.);

  drawType("Horiz. o :",             260., 113., 0.,                        0.,                            0.);
  drawType("m",                      361., 113., 0.,                        0.,                            0.);
  drawType("Vert. o :",              386., 113., 0.,                        0.,                            0.);
  drawType("m",                      479., 113., 0.,                        0.,                            0.);
  rect(314., 104., 7., 2.);
  rect(431., 104., 7., 2.);

  textAlign(RIGHT);
  drawType(nf(lat,2,5),              400.,  47., 0.,                        0.7,                           0.);
  drawType(nf(abs(long),3,5),        400.,  69., 0.,                        0.7,                           0.);
  drawType(nf(int(ele)),             400.,  91., 0.,                        0.7,                           0.);
  drawType(nf(int(horzSigma)),       357., 113., 0.,                        0.7,                           0.);
  drawType(nf(int(vertSigma)),       476., 113., 0.,                        0.7,                           0.);
  textAlign(LEFT);

  drawType("Ground station",         170., 134., 0.,                        0.,                            0.);
  drawType("in control:",            189., 148., 0.,                        0.,                            0.);
  drawType(controlGroundStationName, 186., 163., 0.,                        0.7,                           0.);
  drawType("Monitoring GSs:",        400., 134., 0.,                        0.,                            0.);
  textSize(10);
  drawType(monGroundStationName[0],  440., 148., 0.,                        0.7,                           0.);
  drawType("o",                      401.,  38., 0.,                        0.,                            0.);
  drawType("o",                      401.,  60., 0.,                        0.,                            0.);
  drawType("Lat. =",                 288., 134., 0.,                        0.,                            0.);
  if (groundLat > 0.)  drawType("N", 373., 134., 0.,                        0.,                            0.);
  else                 drawType("S", 373., 134., 0.,                        0.,                            0.);
  drawType("Long. =",                288., 146., 0.,                        0.,                            0.);
  if (groundLong > 0.) drawType("E", 373., 146., 0.,                        0.,                            0.);
  else                 drawType("W", 373., 146., 0.,                        0.,                            0.);
  drawType("Elev. =",                288., 158., 0.,                        0.,                            0.);
  drawType("m ASL",                  373., 158., 0.,                        0.,                            0.);

  textAlign(RIGHT);
  drawType(nf(groundLat,2,5),        369., 134., 0.,                        0.7,                           0.);
  drawType(nf(abs(groundLong),3,5),  369., 146., 0.,                        0.7,                           0.);
  drawType(nf(int(groundEle)),       369., 158., 0.,                        0.7,                           0.);
  textAlign(LEFT);

  textSize(5);
  drawType("o",                      370., 130., 0.,                        0.,                            0.);
  drawType("o",                      370., 142., 0.,                        0.,                            0.);
  textSize(17);
}

function displayConnectionInfo() {

  var  longRadioInUseName,  longAlt1RadioName,  longAlt2RadioName;
  var shortRadioInUseName, shortAlt1RadioName, shortAlt2RadioName;

  // default (radioInUse == 0)
  longRadioInUseName      = "DNT900 910 MHz";
  longAlt1RadioName       = "SHX144 144 MHz";
  longAlt2RadioName       = "RFM23b 440 MHz";
  shortRadioInUseName     = "DNT";
  shortAlt1RadioName      = "SHX";
  shortAlt2RadioName      = "RFM";

  if (radioInUse == 1) {
      longRadioInUseName  = "SHX144 144 MHz";
      longAlt1RadioName   = "DNT900 910 MHz";
      longAlt2RadioName   = "RFM23b 440 MHz";
      shortRadioInUseName = "SHX";
      shortAlt1RadioName  = "DNT";
      shortAlt2RadioName  = "RFM";
  } else if (radioInUse == 2) {
      longRadioInUseName  = "RFM23b 440 MHz";
      longAlt1RadioName   = "DNT900 910 MHz";
      longAlt2RadioName   = "SHX144 144 MHz";
      shortRadioInUseName = "RFM";
      shortAlt1RadioName  = "DNT";
      shortAlt2RadioName  = "SHX";
  }

  if (altairRSSI < threeBarRSSI)                  { if (alarmOn[23] == 0) alarmOn[23] = 2; }
  else                                            { alarmOn[23] = 0; }
  if (groundRSSI < threeBarRSSI)                  { if (alarmOn[24] == 0) alarmOn[24] = 2; }
  else                                            { alarmOn[24] = 0; }
  if (altairDropMessFrac > maxDroppedMessageFrac) { if (alarmOn[28] == 0) alarmOn[28] = 2; }
  else                                            { alarmOn[28] = 0; }
  if (groundDropMessFrac > maxDroppedMessageFrac) { if (alarmOn[29] == 0) alarmOn[29] = 2; }
  else                                            { alarmOn[29] = 0; }

  textSize(15);
  drawType("Connection Strength",    4.,  13., 0.,                        0.,                            0.);
  textSize(13);
  drawType("via",                    4.,  28., 0.,                        0.,                            0.);
  drawType(longRadioInUseName,      24.,  28., (alarmOn[23] > 0 || alarmOn[24] > 0 || alarmOn[28] > 0 || alarmOn[29] > 0 ? 1 : 0),                        
                                           0.7*(alarmOn[23] > 0 || alarmOn[24] > 0 || alarmOn[28] > 0 || alarmOn[29] > 0 ? 0 : 1),                            
                                                                                                         0.);
  textSize(17)


  drawType("ALTAIR -> ground",      60.,  46., 0.,                        0.,                            0.);
  textSize(12);
  drawType("RSSI =",                58.,  60., 0.,                        0.,                            0.);
  drawType("dBm",                  132.,  60., 0.,                        0.,                            0.);
  drawType("Dropped messages: ",    58.,  72., 0.,                        0.,                            0.);
  drawType("%",                    182.,  72., 0.,                        0.,                            0.);
  drawType(shortAlt1RadioName,     200.,  46., 0.,                        0.7,                           0.);
  drawType(shortAlt2RadioName,     230.,  46., 0.,                        0.7,                           0.);
  textSize(10);
  drawType("%",                    217.,  72., 0.,                        0.,                            0.);
  drawType("%",                    247.,  72., 0.,                        0.,                            0.);
  textSize(17);

  drawType("ground -> ALTAIR",      60.,  91., 0.,                        0.,                            0.);
  textSize(12);
  drawType("RSSI =",                58., 106., 0.,                        0.,                            0.);
  drawType("dBm",                  132., 106., 0.,                        0.,                            0.);
  drawType("Dropped messages: ",    58., 118., 0.,                        0.,                            0.);
  drawType("%",                    182., 118., 0.,                        0.,                            0.);
  textSize(10);
  drawType("%",                    217., 118., 0.,                        0.,                            0.);
  drawType("%",                    247., 118., 0.,                        0.,                            0.);
  textSize(17);

  drawType("Range =",               10., 146., 0.,                        0.,                            0.);
  drawType("m",                    130., 146., 0.,                        0.,                            0.);
  makeChangeRadioButtons();

  textAlign(RIGHT);
  textSize(12);
  drawType(nfp(altairRSSI,2,1),    129.,  60., (alarmOn[23] > 0 ? 1 : 0), 0.7*(alarmOn[23] > 0 ? 0 : 1), 0.);
  drawType(nfp(groundRSSI,2,1),    129., 106., (alarmOn[24] > 0 ? 1 : 0), 0.7*(alarmOn[24] > 0 ? 0 : 1), 0.);
  if ((altairDropMessFrac >= 0.) && (altairDropMessFrac < 1.)) {
    drawType(nf(int(altairDropMessFrac*100)),    
                                   181.,  72., (alarmOn[28] > 0 ? 1 : 0), 0.7*(alarmOn[28] > 0 ? 0 : 1), 0.);
  } else {
    drawType("N/A",                181.,  72., (alarmOn[28] > 0 ? 1 : 0), 0.7*(alarmOn[28] > 0 ? 0 : 1), 0.);
  }
  if ((groundDropMessFrac >= 0.) && (groundDropMessFrac < 1.)) {
    drawType(nf(int(groundDropMessFrac*100)),    
                                   181., 118., (alarmOn[29] > 0 ? 1 : 0), 0.7*(alarmOn[29] > 0 ? 0 : 1), 0.);
  } else {
    drawType("N/A",                181., 118., (alarmOn[29] > 0 ? 1 : 0), 0.7*(alarmOn[29] > 0 ? 0 : 1), 0.);
  }
  textSize(10);
  drawType(nfp(altairAlt1RSSI,2,1), 225.,  60., 0.,                       0.7,                           0.);
  drawType(nfp(groundAlt1RSSI,2,1), 225., 106., 0.,                       0.7,                           0.);
  if ((altairAlt1DropMessFrac >= 0.) && (altairAlt1DropMessFrac < 1.)) {
    drawType(nf(int(altairAlt1DropMessFrac*100.)),
                                    217.,  72., 0.,                       0.7,                           0.);
  } else {
    drawType("N/A",                 217.,  72., 0.,                       0.7,                           0.);
  }
  if ((groundAlt1DropMessFrac >= 0.) && (groundAlt1DropMessFrac < 1.)) {
    drawType(nf(int(groundAlt1DropMessFrac*100.)),
                                    217., 118., 0.,                       0.7,                           0.);
  } else {
    drawType("N/A",                 217., 118., 0.,                       0.7,                           0.);
  }
  drawType(nfp(altairAlt2RSSI,2,1), 255.,  60., 0.,                       0.7,                           0.);
  drawType(nfp(groundAlt2RSSI,2,1), 255., 106., 0.,                       0.7,                           0.);
  if ((altairAlt2DropMessFrac >= 0.) && (altairAlt2DropMessFrac < 1.)) {
    drawType(nf(int(altairAlt2DropMessFrac*100.)),
                                    247.,  72., 0.,                       0.7,                           0.);
  } else {
    drawType("N/A",                 247.,  72., 0.,                       0.7,                           0.);
  }
  if ((groundAlt2DropMessFrac >= 0.) && (groundAlt2DropMessFrac < 1.)) {
    drawType(nf(int(groundAlt2DropMessFrac*100.)),
                                    247., 118., 0.,                       0.7,                           0.);
  } else {
    drawType("N/A",                 247., 118., 0.,                       0.7,                           0.);
  }
  textSize(17);
  drawType(nf(int(getRange())),     126., 146., 0.                      , 0.7                          , 0.);
  textAlign(LEFT);

  fill(color(1.*(alarmOn[23] > 0 ? 1 : 0), 0., 0.));
  if (altairRSSI > oneBarRSSI)   rect( 3.,61.,8.,5.);  
  else   drawType("NO SIGNAL",         1.,60.,          1.,                        0.,                            0.);
  if (altairRSSI > twoBarRSSI)   rect(13.,56.,8.,10.);
  if (altairRSSI > threeBarRSSI) rect(23.,51.,8.,15.);
  if (altairRSSI > fourBarRSSI)  rect(33.,46.,8.,20.);
  if (altairRSSI > fiveBarRSSI)  rect(43.,41.,8.,25.);

  fill(color(1.*(alarmOn[24] > 0 ? 1 : 0), 0., 0.));
  if (groundRSSI > oneBarRSSI)   rect( 3.,106.,8.,5.);  
  else   drawType("NO SIGNAL",         1.,105.,         1.,                        0.,                            0.);
  if (groundRSSI > twoBarRSSI)   rect(13.,101.,8.,10.);
  if (groundRSSI > threeBarRSSI) rect(23., 96.,8.,15.);
  if (groundRSSI > fourBarRSSI)  rect(33., 91.,8.,20.);
  if (groundRSSI > fiveBarRSSI)  rect(43., 86.,8.,25.);

  stroke(0);
  line( 25, 29,  72, 29);
  line(200, 47, 225, 47);
  line(230, 47, 255, 47);
}

function displayTimeInfo() {
  var presentTime = new Date();
  noStroke();
  drawType("Current UTC time:",                60., 218., 0.,                           0. ,                            0.);
  drawType(":",                               222., 218., 0.,                           0. ,                            0.);
  drawType(":",                               245., 218., 0.,                           0. ,                            0.);
  drawType(nf(presentTime.getUTCHours()  ,2), 204., 218., 0.,                           0.7,                            0.);
  drawType(nf(presentTime.getUTCMinutes(),2), 227., 218., 0.,                           0.7,                            0.);
  drawType(nf(presentTime.getUTCSeconds(),2), 250., 218., 0.,                           0.7,                            0.);

  textSize(14);
  drawType("Age of:  GPS info:",               10., 234., 0.,                           0. ,                            0.);
  drawType("s",                               155., 234., 0.,                           0. ,                            0.);
  drawType("Orient info:",                     54., 250., 0.,                           0. ,                            0.);
  drawType("s",                               155., 250., 0.,                           0. ,                            0.);
  drawType("Mon2 info:",                       57., 266., 0.,                           0. ,                            0.);
  drawType("s",                               155., 266., 0.,                           0. ,                            0.);

  for (var i = 0; i < 3; ++i) {
    if ( age[i] >= 0.)                                   { age[i] += 0.001 * (presentTime.getTime() - previousTime.getTime()); }
    if ((age[i] >  maxInfoAge) || (age[i] < minInfoAge)) { if (alarmOn[30+i] == 0) alarmOn[30+i] = 2; }
    else                                                                         { alarmOn[30+i] = 0; }
    if ((age[i] >  maxInfoAge) && (alarmCounter % timeBetweenAlarmSounds == 0))  { socket.send('RH'); }
  }
  textAlign(RIGHT);
  drawType(nf(int(age[0])),                   152., 234., 1.*(alarmOn[30] > 0 ? 1 : 0), 0.6*(alarmOn[30] > 0 ? 0 : 1),  0.);
  drawType(nf(int(age[1])),                   152., 250., 1.*(alarmOn[31] > 0 ? 1 : 0), 0.6*(alarmOn[31] > 0 ? 0 : 1),  0.);
  drawType(nf(int(age[2])),                   152., 266., 1.*(alarmOn[32] > 0 ? 1 : 0), 0.6*(alarmOn[32] > 0 ? 0 : 1),  0.);
  textAlign(LEFT);
  textSize(17);

  previousTime = presentTime;
}

function makeChangeGPSButton() {
  var  altGPSName;

  // default (GPSInUse == 0)
  altGPSName     = "DFRobot G6";
  if (GPSInUse == 1) {
      altGPSName = "mast N-M8N";
  }

  var x = 323., y = 1.;
  var xSize = 100., ySize = 12., cornerRadius = 8.;
//  stroke(0);
  textSize(9);
  if (mouseX > x && mouseX < x+xSize &&
      mouseY > y && mouseY < y+ySize) {
    overButton = 32;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 1.);
    stroke(0., 0., 0.);
  }
  rect(x,                        y,                                      xSize,      ySize, cornerRadius);
  noStroke();
  drawType("Switch to",             x+5.,   y+9., ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.));
  drawType(altGPSName,              x+45.,  y+9., 0.,      0.7,                0.);
  textSize(17);

}

function makeChangeOrientButtons() {
  var  alt1OrientName,  alt2OrientName;

  // default (orientInUse == 0)
  alt1OrientName       = "CHRobotics UM7";
  alt2OrientName       = "Sparkfun HMC6343";

  if (orientInUse == 1) {
      alt1OrientName   = "Adafruit BNO055";
      alt2OrientName   = "Sparkfun HMC6343";
  } else if (orientInUse == 2) {
      alt1OrientName   = "Adafruit BNO055";
      alt2OrientName   = "CHRobotics UM7";
  }

  var x = 777., y = 17.;
  var xSize = 128., ySize = 12., cornerRadius = 8.;
//  stroke(0);
  textSize(9);
  if (mouseX > x && mouseX < x+xSize &&
      mouseY > y && mouseY < y+ySize) {
    overButton = 34;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 1.);
    stroke(0., 0., 0.);
  }
  rect(x,                        y,                                      xSize,      ySize, cornerRadius);
  noStroke();
  drawType("Switch to",             x+5.,  y+10., ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),      
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.));
  drawType(alt1OrientName,          x+45., y+10., 0.,      0.7,                0.);
  y = 33.;
  if (mouseX > x && mouseX < x+xSize &&
      mouseY > y && mouseY < y+ySize) {
    overButton = 35;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 1.);
    stroke(0., 0., 0.);
  }
  rect(x,                        y,                                      xSize,      ySize, cornerRadius);
  noStroke();
  drawType("Switch to",             x+5.,  y+10., ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.));
  drawType(alt2OrientName,         x+45., y+10., 0.,      0.7,                0.);
  textSize(17);
}

function makeChangeRadioButtons() {
  var  alt1RadioName,  alt2RadioName;

  // default (radioInUse == 0)
  alt1RadioName       = "SHX144 144 MHz";
  alt2RadioName       = "RFM23b 440 MHz";

  if (radioInUse == 1) {
      alt1RadioName   = "DNT900 910 MHz";
      alt2RadioName   = "RFM23b 440 MHz";
  } else if (radioInUse == 2) {
      alt1RadioName   = "DNT900 910 MHz";
      alt2RadioName   = "SHX144 144 MHz";
  }

  var x = 145., y = 1.;
  var xSize = 135., ySize = 12., cornerRadius = 8.;
//  stroke(0);
  textSize(10);
  if (mouseX > x && mouseX < x+xSize &&
      mouseY > y && mouseY < y+ySize) {
    overButton = 30;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 1.);
    stroke(0., 0., 0.);
  }
  rect(x,                        y,                                      xSize,      ySize, cornerRadius);
  noStroke();
  drawType("Switch to",             x+5.,  y+10., ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),      
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.));
  drawType(alt1RadioName,           x+50., y+10., 0.,      0.7,                0.);
  y = 18.;
  if (mouseX > x && mouseX < x+xSize &&
      mouseY > y && mouseY < y+ySize) {
    overButton = 31;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 1.);
    stroke(0., 0., 0.);
  }
  rect(x,                        y,                                      xSize,      ySize, cornerRadius);
  noStroke();
  drawType("Switch to",             x+5.,  y+10., ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.),
                                                  ((mouseX > x && mouseX < x+xSize && mouseY > y && mouseY < y+ySize) ? 1.: 0.));
  drawType(alt2RadioName,           x+50., y+10., 0.,      0.7,                0.);
  textSize(17);
}

function makeAutomationButtons() {

  if (!isCutdown) makeCutdownButton();

  makePanicButton();
  if (audibleAlarms && anyAlarmIsOn()) {
    soundAlarm();
    makeSilenceAlarmsButton();
  }

}

function determineTelescopeAltAz() {

  makeTelescopeTrackingButton();

  if (doTelescopeALTAIRTracking) {
    if (scopeTrackCounter % timeBtwScopeTrkComms == 0) sendScopeTrackingCommand(getScopeTrackingCommand());
    ++scopeTrackCounter;
  }
}

function sendScopeTrackingCommand(commandText) {
   socket.send("MOVESCOPE: " + commandText);
}

// implement Yorke's ComputeTarget() subroutine (from his ScopeControl.bas file) to find alt-az of target
function getScopeTrackingCommand() {
  var i      = 0;
  var j      = 0;

  // WGS84 parameters
  var a      = 6378137.;
  var e2     = 0.00669437999014;

  // All latitudes are geodetic
  // Find Target position in ECEF coords
  var sinLat = sin(radians(lat));
  var cosLat = cos(radians(lat));
  var sinLon = sin(radians(long));
  var cosLon = cos(radians(long));

  var chi    = sqrt(1.0 - (e2 * sinLat * sinLat));
  var r0     = (a / chi) + ele;

  var r1E    = [];
  r1E[0]     = r0 * cosLat * cosLon;
  r1E[1]     = r0 * cosLat * sinLon;
  r1E[2]     = (a / chi * (1.0 - e2) + ele) * sinLat;


  // Find Base position in ECEF coords
  sinLat     = sin(radians(groundLat));
  cosLat     = cos(radians(groundLat));
  sinLon     = sin(radians(groundLong));
  cosLon     = cos(radians(groundLong));
  
  chi        = sqrt(1.0 - e2 * sinLat * sinLat);
  r0         = (a / chi) + groundEle;

  var r0E    = [];
  r0E[0]     = r0 * cosLat * cosLon;
  r0E[1]     = r0 * cosLat * sinLon;
  r0E[2]     = (a / chi * (1.0 - e2) + groundEle) * sinLat;

  // Find range vector in Earth coords
  var m      = [];
  var sE     = [];
  var sB     = [];
  var ss     = 0.;
  for (i = 0; i < 3; ++i) {  
    sE[i]    = r1E[i] - r0E[i];
    ss       = ss + (sE[i] * sE[i]);
    m[i]     = [];
  }
  var range  = sqrt(ss);
  if (range == 0.) range = 1.  // avoid division by zero

  // Rotate Earth coords to base coords:
  // Earth coordinate system (ECEF) has: x through lat/lon=0/0
  //                                     y through lat/lon=90/0 and
  //                                     z through north pole
  //                                     Origin at geocenter
  // Base coordinate system (NED)   has: x horizontal north from base location
  //                                     y horizontal east
  //                                     z vertical down  (normal to ellipsoid surface)
  //                                     Origin at Base
  m[0][0]    = -sinLat * cosLon;
  m[0][1]    = -sinLat * sinLon;
  m[0][2]    =  cosLat;
  m[1][0]    = -sinLon;
  m[1][1]    =  cosLon;
  m[1][2]    =  0.;
  m[2][0]    = -cosLat * cosLon;
  m[2][1]    = -cosLat * sinLon;
  m[2][2]    = -sinLat;

  for (i = 0; i < 3; ++i) {
    sB[i]    = 0.;
    for (j = 0; j < 3; ++j) {    
      sB[i]  = sB[i] + sE[j] * m[i][j];
    }
  }

  // Compute Az and El (from base location to target).  Use 0 to 360 deg (0 to 2Pi rad) for Az, 0 to 90 deg (0 to Pi/2 rad) for El.

  var dist   = Math.sqrt(sB[0] * sB[0] + sB[1] * sB[1]);       // Ground distance
  if (dist == 0.) {                                            // Avoid division by zero
    dist     = 1.;
    sB[0]    = 1.;
    sB[1]    = 1.;
  }
  var az     =  Math.atan2(sB[1], sB[0]);
  if (az < 0.) az += TWO_PI;
  var el     = -Math.atan2(sB[2], dist);

  // Refraction correction is based on Saemundsson's Formula, which gives the
  // elevation offset for a celestial object as a function of actual elevation.
  // Properly, it only applies to the entire airmass, so would only be accurate
  // if the target were far above the atmosphere.  Consequently
  // we only apply a correction equal to the square of the fractional vertical
  // airmass multiplied by the nominal correction.  We approximate the
  // fractional vertical airmass by e^(-groundEle/H) - e^(-ele/H), where                 
  // groundEle is the elevation above sea level of the ground station, ele is     
  // the elevation above sea level of ALTAIR, and H is the typical scale
  // height of Earth's atmosphere -- equal to 7650 meters.
  // The magnitude of the refraction correction is negligibly small for
  // altitudes more than 40 deg above the horizon.  The formula
  // gives the elevation correction in minutes of arc.

  var refraction   = 1.02 / tan(radians(degrees(el) + (10.3 / (degrees(el) + 5.11))));
  var fracvertairm = exp(-groundEle/7650.) - exp(-ele/7650.);
  el = el + (radians(refraction/60.) * fracvertairm * fracvertairm);

  var azString = hex(round(az * 65536. / TWO_PI), 4);
  var elString = hex(round(el * 65536. / TWO_PI), 4);

  return "B" + azString + "," + elString;
}


function drawType(theText, x, y, r, g, b) {
  fill(r, g, b);
  text(theText, x, y);
}

function makeCylinder(x, y, radius, tall) {
  arc(x, y+tall, 2.*radius, 0.35*radius, 0, PI);
  line(x-radius, y, x-radius, y+tall);
  line(x+radius, y, x+radius, y+tall);
  noStroke();
  rect(x-radius, y, 2.*radius, tall);
  stroke(0);
  ellipse(x, y, 2.*radius, 0.35*radius);
}

function makeBox(x, y, width, height) {
  rect(x-(width/2.), y, width, height);
  quad(x+(width/2.), y, x+(width/2.), y+height, x+(2.*width/3.), y+height-(width/8.), x+(2.*width/3.), y-(width/8.));
  quad(x-(width/2.), y, x+(width/2.), y,        x+(2.*width/3.), y-(width/8.),        x-(width/3.),    y-(width/8.));
} 

function getRange() {
// Haversine formula for range btw ALTAIR and ground station
    var R = 6371e3; // metres
    var phi1 = radians(lat);
    var phi2 = radians(groundLat);
    var dphi = radians(groundLat-lat);
    var dlam = radians(groundLong-long);

    var a = Math.sin(dphi/2) * Math.sin(dphi/2) +
            Math.cos(phi1) * Math.cos(phi2) *
            Math.sin(dlam/2) * Math.sin(dlam/2);
    var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));

    return (R * c);
}


function make3DArrow(gra, len, fat, x, y, z) {

  gra.translate(x*(len+fat)/2., y*(len+fat)/2., z*(len+fat)/2.);
  gra.box((len-fat)*x + fat, (len-fat)*y + fat, (len-fat)*z + fat);
  gra.translate(x*(len+fat)/2., y*(len+fat)/2., z*(len+fat)/2.);
  gra.rotateX(radians(-90*(z)/(x+y+z)));
  gra.rotateZ(radians(90*(x)/(x+y+z)));
  gra.cone(fat, 2*fat);
  gra.rotateZ(radians(-90*(x)/(x+y+z)));
  gra.rotateX(radians(90*(z)/(x+y+z)));
  gra.translate(-x*(len+fat)/2., -y*(len+fat)/2., -z*(len+fat)/2.);
  fill(0, 0, 0);
  gra.translate(fat*(1.-x)/2., fat*(1.-y)/2., fat*(1.-z)/2.);
  gra.box((len-1.)*x + 1., (len-1.)*y + 1., (len-1.)*z + 1.);
  gra.translate(-fat*y, -fat*z, -fat*x);
  gra.box((len-1.)*x + 1., (len-1.)*y + 1., (len-1.)*z + 1.);
  gra.translate(-fat*z, -fat*x, -fat*y);
  gra.box((len-1.)*x + 1., (len-1.)*y + 1., (len-1.)*z + 1.);
  gra.translate(fat*y, fat*z, fat*x);
  gra.box((len-1.)*x + 1., (len-1.)*y + 1., (len-1.)*z + 1.);
  gra.translate(fat*(2.*z+x-1.)/2., fat*(2.*x+y-1.)/2., fat*(2.*y+z-1.)/2.);
  gra.translate(-x*(len+fat)/2., -y*(len+fat)/2., -z*(len+fat)/2.);
}


function makeButtons(x, y, servoNum) {
  if (mouseX > x && mouseX < x+buttonSquareSize && 
      mouseY > y && mouseY < y+buttonSquareSize) {
    overButton = servoNum*2;
    if (mouseIsPressed) {
      fill(0., 0.7, 0.);
//      blockButtons = true;
//      thread("buttonBlock");
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 1.);
    stroke(0., 0., 0.);
  }
  rect(x,                        y,                                      buttonSquareSize,      buttonSquareSize, buttonSqCornerRadius);
  line(x+   buttonSquareSize/4., y+5.*buttonSquareSize/8.,               x+buttonSquareSize/2., y+3.*buttonSquareSize/8.              );
  line(x+3.*buttonSquareSize/4., y+5.*buttonSquareSize/8.,               x+buttonSquareSize/2., y+3.*buttonSquareSize/8.              );
  if (mouseX > x && mouseX < x+buttonSquareSize && 
      mouseY > y+buttonSpacing && mouseY < y+buttonSpacing+buttonSquareSize) {
    overButton = servoNum*2 + 1;
    if (mouseIsPressed) {
      fill(0., 0.7, 0.);
//      blockButtons = true;
//      thread("buttonBlock");
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 1.);
    stroke(0., 0., 0.);
  }
  rect(x,                        y+buttonSpacing,                        buttonSquareSize,      buttonSquareSize, buttonSqCornerRadius);
  line(x+   buttonSquareSize/4., y+3.*buttonSquareSize/8.+buttonSpacing, x+buttonSquareSize/2., y+5.*buttonSquareSize/8.+buttonSpacing);
  line(x+3.*buttonSquareSize/4., y+3.*buttonSquareSize/8.+buttonSpacing, x+buttonSquareSize/2., y+5.*buttonSquareSize/8.+buttonSpacing);
  stroke(0., 0., 0.);
}

function makeCutdownButton() {
  var x = 470., y = 200.;
  var xSize = 60., ySize = 30., cornerRadius = 10.;
//  stroke(0);
  if (mouseX > x && mouseX < x+xSize && 
      mouseY > y && mouseY < y+ySize) {
    overButton = 99;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 0.8, 0.8);
    stroke(1., 0., 0.);
  }
  rect(x,                        y,                                      xSize,      ySize, cornerRadius);  
  noStroke();
  textSize(13);
  drawType("Initiate",             x+12., y+13., 1.,      0.,                 0.);
  drawType("cutdown",              x+6.,  y+25., 1.,      0.,                 0.);
  stroke(0., 0., 0.);
  textSize(17);
}

function makePanicButton() {
  var x = 525., y = 445.;
  var xSize = 80., ySize = 40., cornerRadius = 15.;
//  stroke(0);
  if (mouseX > x && mouseX < x+xSize && 
      mouseY > y && mouseY < y+ySize) {
    overButton = 100;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 0.8, 0.8);
    stroke(1., 0., 0.);
  }
  rect(x,                        y,                                      xSize,      ySize, cornerRadius);  
  drawType("PANIC!",             x+13.,  y+25., 1.,      0.,                 0.);
  stroke(0., 0., 0.);
}

function makeSilenceAlarmsButton() {
  var x = 1190., y = 415.;
  var xSize = 100., ySize = 60., cornerRadius = 15.;

  if (mouseX > x && mouseX < x+xSize && 
      mouseY > y && mouseY < y+ySize) {
    overButton = 101;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
      for (var i = 0; i < numAlarms; ++i) {
        if (alarmOn[i] == 2) alarmOn[i] = 1;
      }
    } else {
      overButton = -999;
      fill(1., 1., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 0.7);
    stroke(0., 0., 0.);
  }

  rect(x,                        y,                                      xSize,      ySize, cornerRadius);  
  noStroke();
  drawType("Silence",            x+20.,  y+20., 0.,      0.,                 0.);
  drawType("present",            x+20.,  y+35., 0.,      0.,                 0.);
  drawType("alarms",             x+23.,  y+50., 0.,      0.,                 0.);
  stroke(0., 0., 0.);  
}

function makeLightSourceButtons() {

  var x = [];  var y = 330;  var d = 30.;
  var r = [];  var g = [];   var b = [];
  var isOn = [];
  var rfill = 0.; var gfill = 0.; var bfill = 0.;
  x[0] = 514.; x[1] = 548.; x[2] = 582.; x[3] = 616.;
  r[0] = 0.; r[1] = 0.;  r[2] = 1.; r[3] = 0.6; r[4] = 0.;  r[5] = 0.;  r[6] = 0.9; r[7] = 1.;
  g[0] = 0.; g[1] = 0.6; g[2] = 0.; g[3] = 0.;  g[4] = 0.4; g[5] = 0.9; g[6] = 0.9; g[7] = 0.;
  b[0] = 1.; b[1] = 0.;  b[2] = 0.; b[3] = 0.;  b[4] = 0.8; b[5] = 0.;  b[6] = 0.;  b[7] = 0.;


  stroke(0);
//  fill(0, 1, 0);

  // integrating sphere light sources
  if (lightSources == 1 || lightSources == 3) {
    for (var i = 0; i < 4; ++i) {  

      textSize(32);
      if (((lightSourceStatus & (1 << i)) >> i) == 1) isOn[i] = true; 
      else                                            isOn[i] = false;

      if (isOn[i]) {
        fill(r[i], g[i], b[i]);
        rfill += r[i]; if (rfill > 1.) rfill = 1.;
        gfill += g[i]; if (gfill > 1.) gfill = 1.;
        bfill += b[i]; if (bfill > 1.) bfill = 1.;
      } else {        
        fill(0,    0,    0);
      }
      if ((x[i]-mouseX)*(x[i]-mouseX) + (y-mouseY)*(y-mouseY) < (d/2.)*(d/2.)) {
        overButton = 50 + 2*i;
        if (isOn[i]) ++overButton;
        if (mouseIsPressed) {
          fill(1.,0.5,0.5);
        } else {
          overButton = -999;
        }
        ellipse(x[i], y, d, d);
        if (isOn[i]) drawType("X", x[i] - 11., y + 11., r[i],  g[i],  b[i]);
        textSize(10);
        drawType("turn", x[i] - 10., y - 2., isOn[i] ? 0 : r[i], isOn[i] ? 0 : g[i], isOn[i] ? 0 : b[i]);
        if (isOn[i]) drawType("OFF", x[i] - 12., y + 8., 0, 0, 0);
        else         drawType("ON",  x[i] - 9.,  y + 8., r[i], g[i], b[i]);
      } else {
        ellipse(x[i], y, d, d);
        if (!isOn[i]) drawType("X", x[i] - 11., y + 11., r[i],  g[i],  b[i]);
        textSize(10);
        drawType("turn", x[i] - 10., y - 2., 1, 1, 1);
        if (isOn[i]) drawType("OFF", x[i] - 11., y + 8., 1, 1, 1);
        else         drawType("ON",  x[i] -  9., y + 8., 1, 1, 1);
      }
      if (isOn[i]) drawType("ON",  x[i] -  9., y - 26., 0, 0, 0);
      else         drawType("OFF", x[i] - 11., y - 26., 0, 0, 0);
    }

    fill(1, 1, 1);
    ellipse(565, 378, 45, 45);
    drawType(nfp(photodiodeReadout[0],1,2),    503.,  387., 0.,                        0.,                           0.);
    drawType(nfp(photodiodeReadout[1],1,2),    594.,  387., 0.,                        0.,                           0.);
    drawType("V", 530, 387, 0, 0, 0);
    drawType("V", 621, 387, 0, 0, 0);

    noStroke();
    if (rfill == 0 && gfill == 0 && bfill == 0) fill(1, 1, 1);
    else                                        fill(rfill, gfill, bfill);
    quad(560, 400,
         570, 400,
         630, 497,
         500, 497);
    fill(0, 0, 0);
    quad(581, 394,
         586, 389,
         591, 394,
         586, 399);
    quad(549, 394,
         544, 389,
         539, 394,
         544, 399);
    drawType("440 nm", 492, 313, 0, 0, 0);
    drawType("532 nm", 529, 313, 0, 0, 0);
    drawType("635 nm", 566, 313, 0, 0, 0);
    drawType("690 nm", 603, 313, 0, 0, 0);

    drawType("PD1", 510, 365, 0, 0, 0);
    drawType("readout:", 501, 375, 0, 0, 0);
    drawType("PD2", 602, 365, 0, 0, 0);
    drawType("readout:", 593, 375, 0, 0, 0);
  }

  // diffusive light sources
  if (lightSources == 2 || lightSources == 3) {
    rfill = 0.; gfill = 0.; bfill = 0.;

    noStroke();
    textSize(10);
    drawType("PD - D", 454, 385, 0, 0, 0);
    drawType("readout:", 451, 395, 0, 0, 0);

    stroke(0);
    strokeWeight(0.5);

    rect(504, 391, 3, 3);
    fill(1., 1., 1.);
    rect(496, 395, 37, 5);
    drawType(nfp(photodiodeReadout[2],1,2),    452.,  407., 0.,                        0.,                           0.);
    drawType("V", 479, 407, 0, 0, 0);

    for (var i = 4; i < 8; ++i) {
      var j = i - 4;

      if (((lightSourceStatus & (1 << i)) >> i) == 1) isOn[j] = true;
      else                                            isOn[j] = false;

      if (isOn[j]) {
        fill(r[i], g[i], b[i]);
        rfill += r[i]; if (rfill > 1.) rfill = 1.;
        gfill += g[i]; if (gfill > 1.) gfill = 1.;
        bfill += b[i]; if (bfill > 1.) bfill = 1.;
      } else {
        fill(0,    0,    0);
      }
      textSize(22);
      if ((mouseX > 470. && mouseX < 487.) && (mouseY > 412.+(22.*j) && mouseY < 429.+(22.*j))) { 
        overButton = 60 + 2*j;
        if (isOn[j]) ++overButton;
        if (mouseIsPressed) {
          fill(1.,0.5,0.5);
        } else {
          overButton = -999;
        }
        rect(470., 412.+(22.*j), 17., 17.);
        if (isOn[j]) drawType("X", 471., 429.+(22.*j), r[i],  g[i],  b[i]);
        textSize(7);
        drawType("turn", 472., 419.+(22.*j), isOn[j] ? 0 : r[i], isOn[j] ? 0 : g[i], isOn[j] ? 0 : b[i]);
        if (isOn[j]) drawType("OFF", 471., 428.+(22.*j), 0, 0, 0);
        else         drawType("ON",  473., 428.+(22.*j), r[i], g[i], b[i]);
      } else {
        rect(470., 412.+(22.*j), 17., 17.);
        if (!isOn[j]) drawType("X", 471., 429.+(22.*j), r[i],  g[i],  b[i]);
        textSize(7);
        drawType("turn", 472., 419.+(22.*j), 1, 1, 1);
        if (isOn[j]) drawType("OFF", 471., 428.+(22.*j), 1, 1, 1);
        else         drawType("ON",  473., 428.+(22.*j), 1, 1, 1);
      }
      textSize(9)
      if (isOn[j]) drawType("ON",  449., 417.+(22.*j), 0, 0, 0);
      else         drawType("OFF", 446., 417.+(22.*j), 0, 0, 0);


    }

    textSize(11);
    noStroke();
    if (rfill == 0 && gfill == 0 && bfill == 0) {
       fill(1, 1, 1);
    } else {
      fill(rfill, gfill, bfill);
      quad(496, 400,
           533, 400,
           542, 497,
           487, 497);
    }
    drawType("Blue",   444, 428, 0, 0, 0);
    drawType("Green",  439, 450, 0, 0, 0);
    drawType("Yellow", 438, 472, 0, 0, 0);
    drawType("Red",    445, 494, 0, 0, 0);
    


  }
  textSize(17);

}

function makeTelescopeTrackingButton() {
  var x = 1175., y = 260.;
  var xSize = 145., ySize = 50., cornerRadius = 15.;
//  stroke(0);
  if (mouseX > x && mouseX < x+xSize && 
      mouseY > y && mouseY < y+ySize) {
    overButton = 98;
    if (mouseIsPressed) {
      fill(1.,0.,0.);
    } else {
      overButton = -999;
      fill(0., 0., 0.);
    }
    stroke(1., 1., 1.);
  } else {
    fill(1., 1., 1.);
    stroke(0., 0., 0.);
  }
  rect(x,                        y,                                      xSize,      ySize, cornerRadius);  
  textSize(17);
  noStroke();
  if (doTelescopeALTAIRTracking) {
    drawType("Stop Telescope",            x+18., y+20., 1.,      0.,                 0.);
  } else {
    drawType("Start Telescope",           x+15., y+20., 1.,      0.,                 0.);
  }
  drawType("ALTAIR Tracking",             x+10., y+38., 1.,      0.,                 0.);
  stroke(0., 0., 0.);
  textSize(17);
}

function anyAlarmIsOn() {
  for (var i = 0; i < numAlarms; ++i) {
    if ( alarmOn[i] == 2 ) return true;
  }
  return false;
}

function soundAlarm() {
  if (alarmCounter % timeBetweenAlarmSounds == 0) alarm.play();
  ++alarmCounter;
}

function setFakeAltairValues() {
  var i;

  lat        =   48.49;                 // degrees (north = +, south = -)
  long       = -123.3117;               // degrees (east = +, west = -)
  ele        =  500.;                   // meters above mean sea level
  var strLat     =  lat.toString();
  var strLong    =  long.toString();
  var strEle     =  ele.toString();

  textSize(150);
  drawType("SIMULATED  DATA",             20., 305., 1.,                        0.85,                            0.85);
  textSize(17);

  for (i = 0; i <  7; ++i) setting[i]    = 2.9;
  for (i = 0; i <  4; ++i) rpm[i]        = 5134.;
  for (i = 0; i <  4; ++i) current[i]    = 0.;
  for (i = 0; i < 11; ++i) temp[i]       = 0.;
  for (i = 0; i <  3; ++i) accel[i]      = 0.;
//  for (i = 0; i <  5; ++i) propRotAng[i] = 0.;
  temp[5] = 90.;
  accel[2] = -0.5;
  accel[0] = -0.5;
//  accel[2] = 1.0;
//  accel[1] = 1.0;
//  accel[0] = 1.0;
  rotAng = 30.;
  rpm[2] = 0.;
  yaw = 0.; pitch = 0.; roll = 0.;
  UM7health = 0.; UM7temp = 0.;    

  for (i = 0; i < 2; ++i) voltage[i]           = 11.9;
  for (i = 0; i < 3; ++i) pressure[i]          = 101.000;  // kPa
  for (i = 0; i < 3; ++i) humidity[i]          = 11.0;     // percentage
  for (i = 0; i < 3; ++i) photodiodeReadout[i] = 0.        // voltage
//  lat        =   48.4593;               // degrees (north = +, south = -)
  gLTAIRLat.setAttribute("value", strLat);
  gLTAIRLon.setAttribute("value", strLong);
  gLTAIRAlt.setAttribute("value", strEle);
//  gLTAIRLat.setAttribute("value", "48.49");
//  gLTAIRLon.setAttribute("value", "-123.3117");
//  gLTAIRAlt.setAttribute("value", "500.");
  horzSigma  =   12.;                   // meters
  vertSigma  =   29.;                   // meters
  groundLat  =   48.48;                 // degrees (north = +, south = -)
  groundLong = -123.37;                 // degrees (east = +, west = -)
  groundEle  =   88.;                   // meters above mean sea level
  GPSInUse   =    0;                    // mast u-blox NEO-M8N
  orientInUse =   0;                    // mast Adafruit BNO055
  radioInUse =    0;                    // DNT900 910 MHz
  altairRSSI =  -10.1;                  // dBm, ALTAIR -> ground
  groundRSSI =  -11.5;                  // dBm, ground -> ALTAIR
  altairDropMessFrac =  0.11;           // ground -> ALTAIR
  groundDropMessFrac =  0.10;           // ALTAIR -> ground
  altairAlt1RSSI =  -18.4;              // alternate radio 1 dBm, ALTAIR -> ground
  groundAlt1RSSI =  -17.6;              // alternate radio 1 dBm, ground -> ALTAIR
  altairAlt1DropMessFrac =  -1;         // alternate radio 1 ground -> ALTAIR
  groundAlt1DropMessFrac =  -1;         // alternate radio 1 ALTAIR -> ground
  altairAlt2RSSI =  -21.2;              // alternate radio 2 dBm, ALTAIR -> ground
  groundAlt2RSSI =  -18.8;              // alternate radio 2 dBm, ground -> ALTAIR
  altairAlt2DropMessFrac =  -1;         // alternate radio 2 ground -> ALTAIR
  groundAlt2DropMessFrac =  -1;         // alternate radio 2 ALTAIR -> ground
  microSDSpaceOccupied        =  920.;  // in MB
  microSDSpaceRemaining       = 7040.;  // in MB
  lightSourceStatus           =    0 ;  // binary packed status integer -- 8 bits: 4 lsb for integrative sphere lasers, 4 msb for diffusive source LEDs
  isCutdown                   = false;
  setting[4]                  =   5.0;  //  out of 10
  setting[6]                  =   0.0;  //  out of 10
  cutdownSteeringServoRotAng  =   0.0;  // measured rotation angle
  heliumBleedValveRotAng      =   4.1;  // measured rotation angle
  controlGroundStationName    = groundStationXName;
  numMonGroundStations        =    1 ;  // number of monitoring (i.e. non-controlling) ground stations in operation
//  for (i = 0; i < 1; ++i) monGroundStationName[i]    = "DENEB";
  for (i = 0; i < 1; ++i) monGroundStationName[i]    = "N/A";

}


function mouseReleased() {
  var servoNum, lightNum;
  switch (overButton) {
    case 0:
    case 2:
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
      servoNum = overButton/2;     
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 's' + String.fromCharCode(servoNum+("A".charCodeAt(0))));
//        socket.send(0xFC); socket.send(0x02); socket.send('s');         // modify 's'ervo (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode(servoNum+("A".charCodeAt(0)))); // write 'A' if servo 0, 'B' if servo 1, etc.
//      }
      socket.send("LOG: Gave command to raise control setting # " + servoNum);
      break;
    case 14:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 's' + String.fromCharCode("U".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('s');         // modify 's'ervo (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("U".charCodeAt(0)));            // write 'U' to increase the power of each propulsion motor by 1 unit
//      }
      socket.send("LOG: Gave command to increase the power of each propulsion motor by 1 unit");
      break;
    case 16:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 's' + String.fromCharCode("H".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('s');         // modify 's'ervo (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("H".charCodeAt(0)));            // write 'H' to increase the power of each propulsion motor by 1/2 unit
//      }
      socket.send("LOG: Gave command to increase the power of each propulsion motor by 1/2 unit");
      break;
    case 30:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 'd' + String.fromCharCode("R".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('d');         // modify 'd'evice (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("R".charCodeAt(0)));            // write 'R' to change the radio to the 1st alternate radio
//      }
      socket.send("LOG: Gave command to change the radio to the 1st alternate radio");
      break;
    case 32:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 'd' + String.fromCharCode("G".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('d');         // modify 'd'evice (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("G".charCodeAt(0)));            // write 'G' to change the GPS to the alternate GPS receiver
//      }
      socket.send("LOG: Gave command to change the GPS to the alternate GPS");
      break;
    case 34:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 'd' + String.fromCharCode("O".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('d');         // modify 'd'evice (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("O".charCodeAt(0)));            // write 'O' to change the orientation sensor to the 1st alternate orientation/acceleration sensor
//      }
      socket.send("LOG: Gave command to change the orientation sensor to the 1st alternate orientation/acceleration sensor");
      break;
    case 50:
    case 52:
    case 54:
    case 56:
      lightNum = (overButton-50)/2;
      overButton = -999;
      socket.send('C2' + 'l' + String.fromCharCode(lightNum+("A".charCodeAt(0))));
//      socket.send(0xFC); socket.send(0x02); socket.send('l');           // modify 'l'ight (ensure we avoid modifications due to noise)
//      socket.send(String.fromCharCode(lightNum+("A".charCodeAt(0))));   // write 'A' if laser diode light source 0, 'B' if laser diode light source 1, etc.
      socket.send("LOG: Gave command to turn ON laser diode light source # " + lightNum);
      break;
    case 60:
    case 62:
    case 64:
    case 66:
      lightNum = (overButton-60)/2;
      overButton = -999;
      socket.send('C2' + 'l' + String.fromCharCode(lightNum+("F".charCodeAt(0))));
//      socket.send(0xFC); socket.send(0x02); socket.send('l');           // modify 'l'ight (ensure we avoid modifications due to noise)
//      socket.send(String.fromCharCode(lightNum+("F".charCodeAt(0))));   // write 'F' if diffusive light source 0, 'G' if diffusive light source 1, etc.
      socket.send("LOG: Gave command to turn ON diffusive light source # " + lightNum);
      break;
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
    case 11:
    case 13:
      servoNum = (overButton-1)/2;     
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 's' + String.fromCharCode(servoNum+("a".charCodeAt(0))));
//        socket.send(0xFC); socket.send(0x02); socket.send('s');         // modify 's'ervo (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode(servoNum+("a".charCodeAt(0)))); // write 'a' if servo 0, 'b' if servo 1, etc.
//      }
      socket.send("LOG: Gave command to lower control setting # " + servoNum);
      break;
    case 15:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 's' + String.fromCharCode("u".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('s');         // modify 's'ervo (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("u".charCodeAt(0)));            // write 'u' to decrease the power of each propulsion motor by 1 unit
//      }
      socket.send("LOG: Gave command to decrease the power of each propulsion motor by 1 unit");
      break;
    case 17:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 's' + String.fromCharCode("h".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('s');         // modify 's'ervo (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("h".charCodeAt(0)));            // write 'h' to decrease the power of each propulsion motor by 1/2 unit
//      }
      socket.send("LOG: Gave command to decrease the power of each propulsion motor by 1/2 unit");
      break;
    case 31:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 'd' + String.fromCharCode("r".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('d');         // modify 'd'evice (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("r".charCodeAt(0)));            // write 'r' to change the radio to the 2nd alternate radio
//      }
      socket.send("LOG: Gave command to change the radio to the 2nd alternate radio");
      break;
    case 35:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 'd' + String.fromCharCode("o".charCodeAt(0)));
//        socket.send(0xFC); socket.send(0x02); socket.send('d');         // modify 'd'evice (ensure we avoid modifications due to noise)
//        socket.send(String.fromCharCode("o".charCodeAt(0)));            // write 'o' to change the orientation sensor to the 2nd alternate orientation/acceleration sensor
//      }
      socket.send("LOG: Gave command to change the orientation sensor to the 2nd alternate orientation/acceleration sensor");
      break;
    case 51:
    case 53:
    case 55:
    case 57:
      lightNum = (overButton-51)/2;
      overButton = -999;
      socket.send('C2' + 'l' + String.fromCharCode(lightNum+("a".charCodeAt(0))));
//      socket.send(0xFC); socket.send(0x02); socket.send('l');           // modify 'l'ight (ensure we avoid modifications due to noise)
//      socket.send(String.fromCharCode(lightNum+("a".charCodeAt(0))));   // write 'a' if laser diode light source 0, 'b' if laser diode light source 1, etc.
      socket.send("LOG: Gave command to turn OFF laser diode light source # " + lightNum);
      break;
    case 61:
    case 63:
    case 65:
    case 67:
      lightNum = (overButton-61)/2;
      overButton = -999;
      socket.send('C2' + 'l' + String.fromCharCode(lightNum+("f".charCodeAt(0))));
//      socket.send(0xFC); socket.send(0x02); socket.send('l');           // modify 'l'ight (ensure we avoid modifications due to noise)
//      socket.send(String.fromCharCode(lightNum+("f".charCodeAt(0))));   // write 'f' if diffusive light source 0, 'g' if diffusive light source 1, etc.
      socket.send("LOG: Gave command to turn OFF diffusive light source # " + lightNum);
      break;
    case 98:
      overButton = -999;
      doTelescopeALTAIRTracking = !doTelescopeALTAIRTracking;
      scopeTrackCounter = 0;
      if (doTelescopeALTAIRTracking) socket.send("LOG: Started telescope ALTAIR tracking");
      else                           socket.send("LOG: Stopped telescope ALTAIR tracking");
      break;
    case 99:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 'sC');
//        socket.send(0xFC); socket.send(0x02); socket.send('s');         // modify 's'ervo (ensure we avoid modifications due to noise)
//        socket.send('C');                                               // 'C' = cut down the gondola
//      }
      socket.send("LOG: Cutdown initiated -- cutting down the gondola.");
      break;
    case 100:
      overButton = -999;
//      if (!testArduinoUnconnected) {
        socket.send('C2' + 'sx');
//        socket.send(0xFC); socket.send(0x02); socket.send('s');         // modify 's'ervo (ensure we avoid modifications due to noise)
//        socket.send('x');                                               // 'x' = shut em all down
//      }
      socket.send("LOG: Pressed PANIC button! -- sent command to shut down all motors.");
      break;
    case 101:
      overButton = -999;
      socket.send("LOG: Silencing alarms.");
    default:
  }
}

