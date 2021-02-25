<?php

/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This receives the parameters from the node device and pushes them to the
 * SQL log
 */

class Log {
    // Attributes
    private $link;

    // Constructors
    function __construct()
    {
        $this->Connect();
    }

    // Properties

    // Methods
    function Connect()
    {
        $this->link = mysqli_connect('localhost', 'root', '', 'demo') or die ('Cannot connect to database');
    }

    // Write to log file
    function WriteToLog()
    {
        $query = "INSERT INTO `log` (`userid`, `date`, `day`, `dosetime`, `dispensetime`, `userpress`) VALUES (";

        $query = $query . $_GET["id"];
        $query = $query . ", '";
        $query = $query . date("Y-m-d");
        $query = $query . "', '";
        $query = $query . date("l");
        $query = $query . "', '";
        $query = $query . $_GET["doset"];
        $query = $query . "', '";
        $query = $query . $_GET["dispenset"];
        $query = $query . "', '";
        $query = $query . $_GET["userpress"];
        $query = $query . "')"; 

        mysqli_query($this->link, $query) or die ('Invalid query: ' . $query);
    }

}

?>

<?php
$log = new Log();
$log->WriteToLog();
?>