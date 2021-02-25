<?php

/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */

/*
 * This gets the schedule from the SQL server and echoes it back to the node
 * device
 */

class GetData {
    public $link;

    function __construct()
    {
        $this->Connect();
    }

    // Connect to db
    function Connect()
    {
        $this->link = mysqli_connect('localhost', 'root', '', 'demo') or die ('Cannot connect to database');
    }

    // Read schedule
    function ReadScheduleWeek()
    {
        $id = $_GET['id'];

        $query = "SELECT day.name as day, day.dose1, day.dose2, day.dose3, day.dose4, day.dose5, day.dose6, day.dose7, day.dose8
        FROM `day`
        INNER JOIN `schedule` ON day.scheduleID = schedule.scheduleID
        INNER JOIN `user` on user.scheduleID = schedule.scheduleID
        WHERE user.userID = $id";

        $result = mysqli_query($this->link, $query) or die ('Invalid query:' . $query);
        while ($row = $result->fetch_array())
        {
            $rows[] = $row;
        }

        // Echo the day along with the times of each dose as long as it isn't blank
        foreach($rows as $row)
        {
            echo $row["day"];
            echo ",";
            if ($row["dose1"] != "")
            {
                echo date('H:i', strtotime($row["dose1"]));
            }
            echo ",";
            if ($row["dose2"] != "")
            {
                echo date('H:i', strtotime($row["dose2"]));
            }
            echo ",";
            if ($row["dose3"] != "")
            {
                echo date('H:i', strtotime($row["dose3"]));
            }
            echo ",";
            if ($row["dose4"] != "")
            {
                echo date('H:i', strtotime($row["dose4"]));
            }
            echo ",";
            if ($row["dose5"] != "")
            {
                echo date('H:i', strtotime($row["dose5"]));
            }
            echo ",";
            if ($row["dose6"] != "")
            {
                echo date('H:i', strtotime($row["dose6"]));
            }
            echo ",";
            if ($row["dose7"] != "")
            {
                echo date('H:i', strtotime($row["dose7"]));
            }
            echo ",";
            if ($row["dose8"] != "")
            {
                echo date('H:i', strtotime($row["dose8"]));
            }
            echo "\n";
        }
    }
}

if($_GET['id'] != '')
{
    $getData = new GetData($_GET['id']);
    $getData->ReadScheduleWeek();
}

?>