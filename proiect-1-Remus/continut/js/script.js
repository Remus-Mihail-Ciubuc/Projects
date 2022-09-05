
var DateActualizare=setInterval(sectiunea1,1000);

function sectiunea1(){
    
      var d = new Date();
    document.getElementById("date").innerHTML = d;

    
    document.getElementById("url").innerHTML= "URL:  " + window.location.href;
    
   
    // var x = document.getElementById("location");
    // function getLocation() {
    //   if (navigator.geolocation) {
    //     navigator.geolocation.getCurrentPosition(showPosition);
    //   } else {
    //     x.innerHTML = "Geolocation is not supported by this browser.";
    //   }
    // }
    
    // function showPosition(position) {
    //   x.innerHTML = "Locatie: latitudine: " + position.coords.latitude +
    //   " și longitudine: " + position.coords.longitude;
    // }
    // getLocation();

    
   document.getElementById("nume").innerHTML= " Nume browser: " + navigator.appName + " și versiunea este: " + navigator.appVersion;

  
    var SO="Unknown";
   if(navigator.appVersion.indexOf("Win")!=-1) SO="Windows";
   if(navigator.appVersion.indexOf("Mac")!=-1) SO="Mac";
   if(navigator.appVersion.indexOf("Linux")!=-1) SO="Linux";

   document.getElementById("SO").innerHTML = "Sistemul de operare este: "+ SO;

}

function Coordonate(){ 
  function showPosition(position) {
    x.innerHTML = "Locatie: latitudine: " + position.coords.latitude +
    " și longitudine: " + position.coords.longitude;
  }
  getLocation();

  var x = document.getElementById("location");
    function getLocation() {
      if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(showPosition);
      } else {
        x.innerHTML = "Geolocation is not supported by this browser.";
      }
    }
}

function sectiunea2()
{
  var i;
  var n=[],viz=[];
  var input=[];
  var j;
  var k=0;
  

  for(i=0;i<8;i++)
  {
    viz[i]=0;
    n[i]=Math.floor((Math.random()*20));
    if(n[i]<10)
      n[i]='0'+n[i];
      else if(n[i]<16)
        n[i]='0'+[(n[i].toString(16)).toUpperCase()];
        else n[i]=[(n[i].toString(16)).toUpperCase()];
    document.getElementById("n"+i).innerHTML=n[i]    
    input[i]=document.getElementById("input"+i);
    console.log(input[i].value);
  }
  console.log("aici");

//console.clear();

for(i=0;i<8;i++)
  for(j=0;j<8;j++)
      if(input[i].value==n[j] && viz[j]==0)
      {
        k++;
        viz[j]=1;
        break;
      }

document.getElementById("contor").innerHTML=k;


}


function Draw(){
  var c = document.getElementById("myCanvas");
  var ctx = c.getContext("2d");
  ctx.beginPath();
  ctx.rect(20, 20, 150, 100);
  ctx.stroke();
}













    
    

  
