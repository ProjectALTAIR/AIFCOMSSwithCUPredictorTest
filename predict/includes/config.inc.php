<?php

// Enter the Google Maps API for your site
// define("GMAPS_API_KEY", "ABQIAAAA4T7AS90KUqGrNPOsl6eyCBS4mbmQPYGFaQoYTVDm_qb3IIB-WBSwfZE_KhJy2GpxCqBbVm7PoSKM9Q");
define("GMAPS_API_KEY", "AIzaSyCuoR_30YvunXAl3RPIARoQ4Ybv7UQSKUM");

// Who should we email about errors etc?
// define("ADMIN_EMAIL", "jon@hexoc.com");
define("ADMIN_EMAIL", "jalbert@uvic.ca,jon@hexoc.com");

define("LOCATION_SAVE_ENABLE", true);

// Path to python virtualenv to use
// define("PYTHON", ROOT . "ENV/bin/python");

// Path to prediction data dir from predict/
define("PREDS_PATH", "preds/");

// Filenames used by the predictor
define("SCENARIO_FILE", "scenario.ini");
define("FLIGHT_CSV", "flight_path.csv");
define("PROGRESS_JSON", "progress.json");
define("LOG_FILE", "py_log");

?>
