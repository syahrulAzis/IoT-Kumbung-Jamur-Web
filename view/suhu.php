<?php
include ("../db/get_data.php");
 $data = data('suhu');
 if($data>=30&&$data<=35.99){
     echo "<font color='green' size='6'><strong>$data</strong></font><br>";
 }else{
     echo "<font color='red' size='6'><strong>$data</strong></font><br>";
 }  
//echo data('suhu')."°C";
// mysqli_close($konek);
?>