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

function getTemp()
{
    var temperature = 20;
    var xhttp = new XMLHttpRequest();
    temperature = document.getElementById("temp").value;
    xhttp.onreadystatechange = function() {
        if(this.status == 200) {
            document.getElementById("temp").value = this.responseText;
            console.log(this.responseText);
        }
    };
    xhttp.open("GET", "getTemp?temp=" + temperature, true);
    xhttp.send();
    alert("La température cible a été changé pour " + temperature)
}

//obtenir les type de bois depuis la bd pour les renvoyer sur la vue html
function getBois(){

}