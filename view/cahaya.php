<?php
include ("../db/get_data.php");
 $data = data('cahaya');
 if($data>=50&&$data<=300){
     echo "<font color='green' size='6'><strong>$data</strong></font><br>";
 }else{
     echo "<font color='red' size='6'><strong>$data</strong></font><br>";
 }
//echo data('cahaya')."%";
// mysqli_close($konek);
