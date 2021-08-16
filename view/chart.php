<?php 
include ("../db/koneksi.php");
$sql_id = mysqli_query($konek, "SELECT MAX(id) FROM work ");
$data_id = mysqli_fetch_array($sql_id);
$id_akhir = $data_id['MAX(id)'];
$id_awal = $id_akhir - 4;

$tanggal = mysqli_query($konek,"SELECT waktu FROM work WHERE id>='$id_awal' and id<='$id_akhir' ORDER BY id ASC");
$suhu = mysqli_query($konek,"SELECT suhu FROM work WHERE id>='$id_awal' and id<='$id_akhir' ORDER BY id ASC");
$kelembaban = mysqli_query($konek,"SELECT kelembaban FROM work WHERE id>='$id_awal' and id<='$id_akhir' ORDER BY id ASC");
$cahaya = mysqli_query($konek,"SELECT cahaya FROM work WHERE id>='$id_awal' and id<='$id_akhir' ORDER BY id ASC");
?>
<div class="panel panel-primary">
    <div class="panel-heading">
        <h4>Grafik</h4>
    </div>
    <div class="panel-body">
    <canvas id="myChart"></canvas>
    <script type="text/javascript">
    var canvas = document.getElementById('myChart');
    var data = {
        labels : [
            <?php 
                while($data_tanggal=mysqli_fetch_array($tanggal))
                {
                    echo'"'.$data_tanggal['waktu'].'",';
                }
                ?>
        ],
        datasets:[
            {
            label : "Suhu",
            fill : true,
            backgroundColor : "rgba(255,0,0, 0.2)",
            borderColor : "rgba(255,0,0, 1)",
            lineTension : 0.5,
            pointRadius : 3,
            data: [
                <?php 
                    while($data_suhu=mysqli_fetch_array($suhu))
                    {
                        echo $data_suhu['suhu'].',';
                    }
                    ?>
            ]
        },
        
        {
            label : "Kelembaban",
            fill : true,
            backgroundColor : "rgba(0,255,0, 0.2)",
            borderColor : "rgba(0,255,0, 1)",
            lineTension : 0.5,
            pointRadius : 3,
            data: [
                <?php 
                    while($data_kelembaban=mysqli_fetch_array($kelembaban ))
                    {
                        echo $data_kelembaban['kelembaban'].',';
                    }
                    ?>
            ]
        },

        {
            label : "Cahaya",
            fill : true,
            backgroundColor : "rgba(0,0,255, 0.2)",
            borderColor : "rgba(0,0,255, 1)",
            lineTension : 0.5,
            pointRadius : 3,
            data: [
                <?php 
                    while($data_cahaya=mysqli_fetch_array($cahaya))
                    {
                        echo $data_cahaya['cahaya'].',';
                    }
                    ?>
            ]
        }

        ]
    };

    //Opsi Grafik
    var option = {
        showLines : true,
        animation : {duration : 0}
    };

    //Cetak grafik Kedalam Canvas 
    var myLineChart = Chart.Line(canvas,{
        data : data,
        options : option
    });
    </script>
    </div>
</div>