function getFromESP_getNom () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("nom").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "getNomEsp", true);
    xhttp.send();
}

setInterval(
    function getTempSysteme(){
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if(this.status == 200) {
                document.getElementById("temp").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "lireTemp", true);
        xhttp.send();
    }
, 5000);