<?php
// $servername = "127.0.0.1";
// $database = "cv";
// $username = "root";
// $password = "";
// $konek = mysqli_connect ($servername, $database, $username, $password); 
// $sql = "SELECT * FROM `work`";
// if (!$konek) {
//     die("Connection failed: " . mysqli_connect_error());
// }

$konek = mysqli_connect("localhost", "root", "", "kjamur");
$sql = "SELECT * FROM `work`";
if (!$konek) {
    die("Connection failed: " . mysqli_connect_error());
}
function query($query)
{
    global $konek;
    $result = mysqli_query($konek, $query);
    $rows = [];
    while ($row = mysqli_fetch_assoc($result)) {
        $rows[] = $row;
    }
    return $rows;
}
function registrasi($data)
{
    global $konek;

    //$nik = $data["nik"];
    $username = strtolower(stripslashes($data["username"]));
    $password = mysqli_real_escape_string($konek, $data["password"]);
    $password2 = mysqli_real_escape_string($konek, $data["password2"]);


    $result = mysqli_query($konek, "SELECT username FROM users
            WHERE username = '$username'");
    if (mysqli_fetch_assoc($result)) {
        echo "<script>
        alert('username sudah terdaftar!');
        </script>";
        return false;
    }


    if ($password !== $password2) {
        echo "<script>
        alert('Konfirmasi password tidak sesuai !');
        </script>";
        return false;
    }

    $password = password_hash($password, PASSWORD_DEFAULT);

    mysqli_query($konek, "INSERT INTO users VALUES('', '$username', '$password')");

    return mysqli_affected_rows($konek);
}
