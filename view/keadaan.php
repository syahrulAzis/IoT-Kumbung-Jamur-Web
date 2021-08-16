<?php
include("../db/get_data.php");
$data = data('keadaan');
if ($data == 1) {
    echo "<font color='green' size='5'><strong>Keadaan Kumbung Baik</strong></font><br>";
} else if ($data == 2) {
    echo "<font color='red' size='5'><strong>Keadaan Kumbung Tidak Baik</strong></font><br>";
} else {
    echo "";
}

//echo data('keadaan') . "Kumbung";
// mysqli_close($konek);
