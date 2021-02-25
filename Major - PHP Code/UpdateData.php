<?php

/*
 * ############################# April  2020 #############################
 * ## Major project - Developing a smart automated medication dispenser ##
 * ##      Submitted as part of the BSc Software Engineering award      ## 
 * ##                    by Matthew Dene Lewington                      ##
 * #######################################################################
 */


 /*
 * This receives the dose times from the node, converts to time (for easy comparison), 
 * performs a bubble sort and updates the SQL server with the dose schedule
 */

class Update {
	// Attributes
	private $link;
	private $doses = [];

	// Constructor
    function __construct()
    {
        $this->Connect();
    }

	// Methods
    function Connect()
    {
        $this->link = mysqli_connect('localhost', 'root', '', 'demo') or die ('Cannot connect to database');
	}

	// Convert the GET dose params to a time
	function ConvertToTime()
	{
		for ($i = 0; $i < 8; $i++)
		{
			if ($_GET['d'.($i+1)] != "")
			{
				$temp = new DateTime($_GET['d'.($i+1)]);
				array_push($this->doses, $temp->format('H:i:s'));
			}
		}
	}
	
	// Bubble sort the data
	function SortData()
	{
		for ($i = 0; $i < count($this->doses); $i++)
		{
			for ($j = 0; $j < count($this->doses); $j++)
			{
				if (strtotime($this->doses[$i]) < strtotime($this->doses[$j]))
				{
					$temp = $this->doses[$i];
					$this->doses[$i] = $this->doses[$j];
					$this->doses[$j] = $temp;
				}
			}
		}
	}

	// Write to the schedule
    function WriteToSchedule()
    {
		$id = $_GET['id'];
		$day = $_GET['day'];	
		
		$query = "UPDATE `day` SET ";

		for ($i = 0; $i < 8; $i++)
		{
			if (isset($this->doses[$i]))
			{
				$query = $query . "`dose" . ($i+1) . "` = '" . $this->doses[$i] . "'";
			}
			else
			{
				$query = $query . "`dose" . ($i+1) . "` = NULL";
			}

			if ($i < 7)
			{
				$query = $query . ", ";
			}
		}
		
		$query = $query . " WHERE scheduleID = $id";
		$query = $query . " AND day.name = '$day'";
		        
		mysqli_query($this->link, $query) or die ('Invalid query: ' . $query);
    }
}
if($_GET['id'] != '')
{
	$update = new Update();
	$update->ConvertToTime();
	$update->SortData();
    $update->WriteToSchedule();
}
?>