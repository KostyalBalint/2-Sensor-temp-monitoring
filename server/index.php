<?php
	include_once 'includes/database.inc.php'
 ?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Temp sensor data</title>

		<script src="./chart_js/dist/Chart.bundle.js"></script>
		<script src="./chart_js/samples/utils.js"></script>
		<link href="https://fonts.googleapis.com/css?family=Muli:200,300,400,600,700,800,900" rel="stylesheet">
		<link rel="stylesheet" type="text/css" href="style.css">

	</head>
	<body>
			<div class="container">
				<div class="text-center">
			 	<h1 class="title text-orange text-center">Recorded temperature datas</h1>
					<form action="index.php" method="post" class="text-gray">
						<p style="display: inline-block; padding: 10px;">Start time</p><input type="date" value="<?php if (isset($_REQUEST['start_t'])) {echo $_REQUEST['start_t'];} else {echo date("Y-m-d");}?>" name="start_t" id="start_t" class="time_input"></input>
						<p style="display: inline-block; padding: 10px;">End time</p><input type="date" value="<?php if (isset($_REQUEST['end_t'])) {echo $_REQUEST['end_t'];} else {echo date("Y-m-d");}?>" name="end_t" id="end_t" class="time_input"></input>
						<center><button type="submit" class="btn" style="display: block">Set interval</button></center>
					</form>
						<!--h1 class="title text-orange text-center" style="margin-top: 5%; margin-bottom: 5%; display:none;" id="noInfo">For this date we don't have any informations!</h1-->

						<div class="canvas">
							<canvas id="canvas"></canvas>
						</div>
				</div>
						<div class="voltage">
							<p style="margin: auto;" class="text-gray">Supply voltage: <span id="SpVoltage" ></span> V</p>
						</div>
						<div class="footer">
							<p style="margin: auto;" class="text-gray">Made by: Bálint Kostyál</p>
						</div>
			</div>
		<?php
			// Get the values back from the form //
			$start_t = $_REQUEST['start_t'];
			$end_t = $_REQUEST['end_t'];
			//echo date('Y-m-d');

			if (($start_t <= $end_t) && $start_t != null && $end_t != null) {
				//Good input
				$sql_command = "SELECT * FROM temp_data WHERE DATE(date) >= '$start_t' AND DATE(date) <= '$end_t' ORDER BY date;";
			}
			else {
				//Bad input
				$sql_command = "SELECT * FROM temp_data WHERE DATE(date) = CURRENT_DATE() ORDER BY date;";
			}

			$result = mysqli_query($conn, $sql_command);
			$result_check = mysqli_num_rows($result);

			if ($result_check > 0) {

				while ($row = mysqli_fetch_assoc($result)) {
					//echo $row['date'] . $row['temp'] . "<br>";
					$dates[$i] = $row['date'];
					$temps_1[$i] = $row['temp_1'];
					$temps_2[$i] = $row['temp_2'];
					$voltage[$i] = $row['voltage'];
					$i++;
				}

				$out_dates = array_values($dates);
				$out_temps_1 = array_values($temps_1);
				$out_temps_2 = array_values($temps_2);

			}
			echo "<script>\n";
			echo 'var string_dates =' . json_encode($out_dates, JSON_PRETTY_PRINT) . ';';
			echo 'var string_temps_1 =' . json_encode($out_temps_1, JSON_PRETTY_PRINT) . ';';
			echo 'var string_temps_2 =' . json_encode($out_temps_2, JSON_PRETTY_PRINT) . ';';
			//echo 'document.getElementById('SpVoltage').innerHTML = ' . json_encode($voltage[$i - 1], JSON_PRETTY_PRINT) . ';';
			echo 'var voltage = ' . json_encode($voltage[$i - 1], JSON_PRETTY_PRINT) . ';';
			echo "\n</script>";

			/*echo "<script>\n";
			echo 'var start_date =' . json_encode($start_t, JSON_PRETTY_PRINT) . ';';
			echo 'var end_date =' . json_encode($end_t, JSON_PRETTY_PRINT) . ';';
			echo 'console.log(start_date );';
			echo 'console.log(end_date );';
			echo "\n</script>";*/

		?>
	</body>
	<script type="text/javascript">

		/*for (var i =0; i < dates_hour.length; i++) {
			dates_hour[i] = dates_hour[i].substring(11, dates_hour[0].length-1);
		}*/
		if(voltage != null){
			document.getElementById('SpVoltage').innerHTML = voltage;
		}
		else{
			document.getElementById('SpVoltage').innerHTML = "-";
		}

		let dates_hour = string_dates;

		var temp_chart = {
				type: 'line',
				data: {
						labels: dates_hour,
						datasets: [{
								label: "Outside air temp.",
								backgroundColor: window.chartColors.yellow,
								borderColor: window.chartColors.yellow,
								fill: false,
								data: []
						},
						{
							label: "Collector surface temp.",
							backgroundColor: window.chartColors.blue,
							borderColor: window.chartColors.blue,
							fill: false,
							data: []
						}]
				},
				options: {
						responsive: true,
						title:{
								display:false,
								text:'Temp sensor data'
						},
						tooltips: {
								mode: 'nearest',
								intersect: false
						},
						hover: {
								mode: 'nearest',
								intersect: false
						},
						scales: {
								xAxes: [{
										display: true,
										scaleLabel: {
												display: true,
												labelString: 'Time'
										}
								}],
								yAxes: [{
										display: true,
										scaleLabel: {
												display: true,
												labelString: 'Temperature'
										}
								}]
						}
				}
		};


		window.onload = function() {
			var ctx = document.getElementById("canvas").getContext("2d");
			window.myLine = new Chart(ctx, temp_chart);
		};

		for (var i = 0; i < string_temps_1.length; i++) {
				//temps[i] = parseFloat(string_temps[i] + "<br>");
			temp_chart.data.datasets[0].data.push(parseFloat(string_temps_1[i] + "<br>"));
			temp_chart.data.datasets[1].data.push(parseFloat(string_temps_2[i] + "<br>"));
		}
		//window.myLine.update();

		function AddData(chart, label, data) {
	    chart.data.labels.push(label);
	    chart.data.datasets.forEach((dataset) => {
	        dataset.data.push(data);
	    });
	    chart.update();
		}

	</script>
</html>
