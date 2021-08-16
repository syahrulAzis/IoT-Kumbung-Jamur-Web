<?php
include("../db/koneksi.php");
$temp = $_GET['suhu'];
$humi = $_GET['kelembaban'];
$ldr  = $_GET['cahaya'];
$kdn  = $_GET['keadaan'];
// $ppm = $_GET['gas'];

//$simpan = mysqli_query($konek, "INSERT INTO `work` (`suhu`, `kelembaban`,`cahaya`) VALUES ($temp, $humi,$ldr)");
$simpan = mysqli_query($konek, "INSERT INTO `work` (`suhu`, `kelembaban`,`cahaya`,`keadaan`) VALUES ($temp, $humi,$ldr,$kdn)");

//Notofikasi Bila Data Tersimpan
if ($simpan) {
    echo "Data Berhasil disimpan";
} else {
    echo "Data Gagal disimpan";
}
