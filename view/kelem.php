<?php
include ("../db/get_data.php");
 $data = data('kelembaban');
 if($data>=80&&$data<=90.99){
     echo "<font color='green' size='6'><strong>$data</strong></font><br>";
 }else{
     echo "<font color='red' size='6'><strong>$data</strong></font><br>";
 } 
//echo data('kelembaban')."%";
// mysqli_close($konek);
?>