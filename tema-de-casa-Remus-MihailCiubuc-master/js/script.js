function CalcularePret(event)
{
    var DataEnd=document.getElementById("zile").value;
    var Pret=document.getElementById("pret").value;
    document.getElementById("dataMea5").innerHTML=Pret*DataEnd;
}