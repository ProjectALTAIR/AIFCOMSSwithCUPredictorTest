<?php
require_once("includes/functions.inc.php");
require_once("includes/config.inc.php");

$stats = new StatsD();

$action = $_GET['action'];

$software_available = array("gfs", "gfs_hd");

switch($action) {

case "submitForm":
    $pred_model = array();
    $json_return = array();
    $json_return['valid'] = "false";
    $thepost = implode(" ", $_POST);

//    error_log("The damn hour variable: ");
//    error_log($thepost);

        // First, make a model from the form data
        if ( !$pred_model = createModel($_POST)) {
            $json_return['error'] = "Server couldn't make a model from the form 
                data";
            echo json_encode($json_return);
            break;
        }

        // If that worked, make sure the model is valid
//        $verify_dump = verifyModel($pred_model, $software_available);
//        if ( !$verify_dump['valid'] ) {
//            $json_return['error'] = $verify_dump['msg'];
//            echo json_encode($json_return);
//            break;
//        }

        // If we have a valid model, try and make a UUID
        if ( !$pred_model['uuid'] = makesha1hash($pred_model) ) {
            $json_return['error'] = "Couldn't make the SHA1 hash";
            echo json_encode($json_return);
            break;
        }

        // If all of the above worked, let's run the prediction
        runPred($pred_model);
        $json_return['valid'] = "true";
        $json_return['uuid'] = $pred_model['uuid'];
        $json_return['timestamp'] = $pred_model['timestamp'];


    echo json_encode($json_return);
    break;

default:
    echo "Couldn't interpret 'action' variable";
    break;

}

?>
