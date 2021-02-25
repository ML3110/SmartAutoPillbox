<?php

/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This retrieves the log from the SQL server and echoes it back to the 
 * node device
 */

class Log
{
    // Attributes
    public $link;

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

    function GetLog()
    {
        $query = "SELECT * FROM `log` WHERE `userid` = ";
        $query = $query . $_GET["id"];

        $result = mysqli_query($this->link, $query) or die ('Invalid query: ' . $query);

        while ($row = $result->fetch_array())
        {
            $rows[] = $row;
        }

        // Make timestamp of current date
        $dtnow = date("Y-m-d");
        foreach ($rows as $row)
        {
            // Create date objects with fetched param and current date
            $dt1 = date_create($row["date"]);
            $dt2 = date_create($dtnow);

            // Calculate the difference between the two
            $interval = date_diff($dt1, $dt2);

            // If the interval is less than 7 days (Can be modified, or further methods
            // developed for additional functionality)
            if ($interval->y == 0 && $interval->m == 0 && $interval->d < 7)
            {
                echo $row["date"];
                echo ",";
                echo date('H:i', strtotime($row["dosetime"]));
            }

            // Otherwise echo back blank
            else
            {
                echo "-,-";
            }
            
            echo "\n";
        }
    }
}

?>
<?php
$log = new Log();
$log->GetLog();
?>