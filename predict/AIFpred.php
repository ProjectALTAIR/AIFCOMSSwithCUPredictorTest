<?php

require_once("includes/config.inc.php");
require_once("includes/functions.inc.php");

// Get the time for pre-populating the form
$time = time() + 3600;
?>
<html>
<head>
  <META HTTP-EQUIV="Access-Control-Allow-Origin" CONTENT="*">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
<link rel="stylesheet" href="https://ajax.googleapis.com/ajax/libs/jqueryui/1.12.1/themes/smoothness/jquery-ui.css">
<script src="https://ajax.googleapis.com/ajax/libs/jqueryui/1.12.1/jquery-ui.min.js"></script>
<link href="css/pred.css" type="text/css" rel="stylesheet" />
<link href="css/calc.css" type="text/css" rel="stylesheet" />
<link rel="stylesheet" href="css/tipsy.css" type="text/css" />
<link href="css/cupertino/jquery-ui-1.8.1.custom.css" type="text/css" rel="stylesheet">
<script src="js/jquery/jquery.form.js" type="text/javascript"></script>
<script src="js/jquery/jquery.jookie.js" type="text/javascript"></script>
<script src="js/jquery/jquery.tipsy.js" type="text/javascript"></script>
<script src="js/utils/date.jsport.js" type="text/javascript"></script>

<script src="js/pred/pred-config.js" type="text/javascript"></script>
<script src="js/pred/pred-ui.js" type="text/javascript"></script>
<script src="js/pred/pred-cookie.js" type="text/javascript"></script>
<script src="js/pred/pred-map.js" type="text/javascript"></script>
<script src="js/pred/pred-event.js" type="text/javascript"></script>
<script src="js/pred/pred.js" type="text/javascript"></script>
<script src="js/calc/calc.js" type="text/javascript"></script>

</head>
<body>

<!-- Burst calculator window -->
<div id="burst-calc-wrapper" class="box ui-corner-all">
    <img src="images/drag_handle.png" class="handle" />
    <div id="burst-calc">
        <b>Burst Calculator</b>
        <br>
        <table id="input_table">
            <tr class="input_row">
                <td class="input_label" colspan="2">Payload Mass (g)</td>
                <td class="input_instruction" rowspan="3">AND</td>

                <td class="input_label" colspan="2">Balloon Mass (g)</td>
            </tr>
            <tr class="input_row">
                <td colspan="2">
                    <input type="text" id="mp" class="input_field" value="1500" 
                        tabindex="1"/>
                </td>
                <td colspan="2">
                    <select class="input_field" id="mb" tabindex="2"> 
                        <option value="k200">Kaymont - 200</option>
                        <option value="k300">Kaymont - 300</option>
                        <option value="k350">Kaymont - 350</option>
                        <option value="k450">Kaymont - 450</option>
                        <option value="k500">Kaymont - 500</option>
                        <option value="k600">Kaymont - 600</option>
                        <option value="k700">Kaymont - 700</option>
                        <option value="k800">Kaymont - 800</option>
                        <option value="k1000" selected="selected">Kaymont - 1000</option>
                        <option value="k1200">Kaymont - 1200</option>
                        <option value="k1500">Kaymont - 1500</option>
                        <option value="k2000">Kaymont - 2000</option>
                        <option value="k3000">Kaymont - 3000</option>
                        <option value="h200">Hwoyee - 200</option>
                        <option value="h300">Hwoyee - 300</option>
                        <option value="h350">Hwoyee - 350</option>
                        <option value="h450">Hwoyee - 400</option>
                        <option value="h500">Hwoyee - 500</option>
                        <option value="h600">Hwoyee - 600</option>
                        <option value="h700">Hwoyee - 750</option>
                        <option value="h800">Hwoyee - 800</option>
                        <option value="h800">Hwoyee - 950</option>
                        <option value="h1000">Hwoyee - 1000</option>
                        <option value="h1200">Hwoyee - 1200</option>
                        <option value="h1500">Hwoyee - 1500</option>
                        <option value="h1600">Hwoyee - 1600</option>
                        <option value="h2000">Hwoyee - 2000</option>
                        <option value="p1200">PAWAN - 1200</option>
                    </select>
                </td>
            </tr>
            <tr class="warning_row">
                <td colspan="2" id="mp_w">&nbsp;</td>
                <td colspan="2" id="mb_w">&nbsp;</td>
            </tr>
            <tr>
                <td class="input_instruction" colspan="5">THEN</td>
            </tr>
            <tr class="input_row">
                <td class="input_label" colspan="2">Target Burst Altitude (m)</td>

                <td class="input_instruction" rowspan="3">OR</td>
                <td class="input_label" colspan="2">Target Ascent Rate (m/s)</td>
            </tr>
            <tr class="input_row">
                <td colspan="2">
                    <input type="text" id="tba" class="input_field" value="33000" tabindex="3"/>
                </td>
                <td colspan="2">

                    <input type="text" id="tar" class="input_field" tabindex="4"/>
                </td>
            </tr>
            <tr class="warning_row">
                <td id="tba_w" colspan="2">&nbsp;</td>
                <td id="tar_w" colspan="2">&nbsp;</td>
            </tr>
            <tr class="output_row">
                <td class="output_label">Burst Altitude:</td>
                <td class="output_data"><span id="ba">33000</span> m</td>
                <td></td>
                <td class="output_label">Ascent Rate:</td>
                <td class="output_data"><span id="ar">2.33</span> m/s</td>

            </tr>
            <tr class="output_row">
                <td class="output_label">Time to Burst:</td>
                <td class="output_data"><span id="ttb">238</span> min</td>
                <td></td>
                <td class="output_label">Neck Lift:</td>
                <td class="output_data"><span id="nl">1733</span> g</td>

            </tr>
            <tr class="output_row">
                <td class="output_label">Launch Volume:</td>
                <td class="output_data"><span id="lv_m3">2.66</span> 
                    m<sup>3</sup></td>
                <td></td>
                <td class="output_data"><span id="lv_l">2660</span> L</td>
                <td class="output_data"><span id="lv_cf">93.9</span>
                     ft<sup>3</sup></td>

            </tr>
        </table>
        <br>
        <input type="button" id="burst-calc-advanced-show"
            name="burst-calc-advanced-show" value="Advanced">
        <input type="button" id="burst-calc-use" name="burst-calc-submit" 
            value="Use Values"/ >
        <input type="button" id="burst-calc-close" name="burst-calc-submit" 
            value="Close"/ >
    </div>

    <!-- these are the burst calc constants -->
    <div id="burst-calc-constants">
        <div class="constants_header">Constants</div><br />
        <div class="constants_warning">
            For advanced use only! You can probably leave these alone.
        </div><br />
        <label class="constant_label" for="gas">Gas</label><br />
        <select id="gas" class="constant_field">
            <option value="he">Helium</option>
            <option value="h">Hydrogen</option>

            <option value="ch4">Methane</option>
            <option value="custom">Custom</option>
        </select><br />
        <label class="constant_label" for="rho_g">Gas Density (kg/m<sup>3</sup>)</label><br />
        <input type="text" id="rho_g" value="0.1786" class="constant_field" size="9" disabled="disabled"/><br />
        <label class="constant_label" for="rho_a">Air Density (kg/m<sup>3</sup>)</label><br />

        <input type="text" id="rho_a" value="1.2050" class="constant_field" size="9"/><br />
        <label class="constant_label" for="adm">Air Density Model</label><br />
        <input type="text" id="adm" value="7238.3" class="constant_field" size="9"/><br />
        <label class="constant_label" for="ga">Gravitational<br />Acceleration (m/s<sup>2</sup>)</label><br />
        <input type="text" id="ga" value="9.80665" class="constant_field" size="9" /><br />
        <label class="constant_label" for="bd">Burst Diameter (m)</label><br />

        <input type="checkbox" id="bd_c" />
        <input type="text" id="bd" class="constant_field" size="9" disabled="disabled" value="7.86"/><br />
        <label class="constant_label" for="cd">Balloon Cd</label><br />
        <input type="checkbox" id="cd_c" />
        <input type="text" id="cd" class="constant_field" size="9" disabled="disabled" value="0.3"/><br />
        <br />
        <input type="button" id="burst-calc-advanced-hide" 
            name="burst-calc-advanced-hide" value="Back">
    </div>
</div>


</body>
</html>
