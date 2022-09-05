const express = require('express');
const expressLayouts = require('express-ejs-layouts');
const bodyParser = require('body-parser')
const fs = require('fs');
const cookieParser = require('cookie-parser');
//const session=require('express-session');

const app = express();

const port = 6789;

// directorul 'views' va conține fișierele .ejs (html + js executat la server)
app.set('view engine', 'ejs');
// suport pentru layout-uri - implicit fișierul care reprezintă template-ul site-ului este views/layout.ejs
app.use(expressLayouts);
// directorul 'public' va conține toate resursele accesibile direct de către client (e.g., fișiere css, javascript, imagini)
app.use(express.static('public'))
// corpul mesajului poate fi interpretat ca json; datele de la formular se găsesc în format json în req.body
app.use(bodyParser.json());
// utilizarea unui algoritm de deep parsing care suportă obiecte în obiecte
app.use(bodyParser.urlencoded({ extended: true }));
app.use('/css', express.static('css'));

// la accesarea din browser adresei http://localhost:6789/ se va returna textul 'Hello World'
// proprietățile obiectului Request - req - https://expressjs.com/en/api.html#req
// proprietățile obiectului Response - res - https://expressjs.com/en/api.html#res

app.use(cookieParser()) ;


app.get('/', (req, res) => {
    var msgLog="";
	if(!req.cookies.user)
	{
		req.cookies.user={};
	}else{
		msgLog="Bine ai venit: "+req.cookies.user;
	}
	res.render("index",{msgLog:msgLog,listProd:lista});
});

let rawIntrebari = fs.readFileSync('intrebari.json');
const listaIntrebari = JSON.parse(rawIntrebari)["intrebari"];
let rawUseri = fs.readFileSync('utilizatori.json');
const listaUtilizatori = JSON.parse(rawUseri);

// la accesarea din browser adresei http://localhost:6789/chestionar se va apela funcția specificată
app.get('/chestionar', (req, res) => {
	res.render('chestionar', {intrebari: listaIntrebari});
});

app.post('/rezultat-chestionar', (req, res) => {
    var solutions = req.body;
    var corecte = 0;
    for(var index=0; index < listaIntrebari.length; index++){
        var current = solutions["intrebare-" + index];
        if(current == listaIntrebari[index].corect){
            corecte ++;
        }
    } 
    res.redirect('/rezultat-chestionar?corecte=' + corecte);
});

app.get('/rezultat-chestionar', (req, res) => {
    var total = listaIntrebari.length;
    var corecte = req.query.corecte;
    res.render('rezultat-chestionar', {total: total, rezultat: corecte});
});

app.get('/autentificare', (req, res) => {


    var eroare = "";
    if(req.cookies["mesajEroare"] != null){
        eroare = req.cookies["mesajEroare"];
    }
    res.clearCookie("mesajEroare");
    res.render('autentificare', {eroare: eroare});
});

app.post('/verificare-autentificare', (req, res) => {
    var body = req.body;
    let ok = 0;

    listaUtilizatori.forEach(element => {
        if(body.user == element.utilizator && body.password == element.parola){
            //userObj = element;
            ok = 1; 
            return;
        }
    });

    if(ok != 0){
        res.clearCookie("mesajEroare");

        res.cookie("user", body.user);
        res.redirect("/");
    }
    else{
        res.cookie("mesajEroare", "Nume sau parola gresite!");
        res.redirect("/autentificare");
    }

});

app.get('/de-logare', (req, res) => {
	res.clearCookie("user");
	res.redirect("/");
});



app.get('/creare-bd',(req,res)=>{
	var mysql = require('mysql2');

	var con = mysql.createConnection({
	 	host: "localhost",
	 	user: "root",		
	 	password: "remus",
		database:"cumparaturi"
	 });

	 con.connect(function(err) {
	 if (err) throw err;
	 	console.log("Connected!");
	 	 con.query("CREATE DATABASE IF NOT EXISTS cumparaturi", function (err, result) {
	 	 	if (err) throw err;
	 	 	console.log("Database created");
	 	 });

	 	con.query("CREATE TABLE if not exists produse(name VARCHAR(100),pret INT(100))", function (err, result) {
	 		if (err) throw err;
	 		console.log("Table created");
	 	});
	 });

	res.redirect("http://localhost:6789/")

});
var lista={};
app.get('/inserare-bd',(req,res)=>{
	var mysql = require('mysql2');
	 var con = mysql.createConnection({
	 	host: "localhost",
	 	user: "root",		
	 	password: "remus",
		database: "cumparaturi" 
	 });
	  con.connect(function(err) {
	 	if (err) throw err;
	 	console.log("Connected!");
	 	var sql = "INSERT INTO produse (name, pret) VALUES ('Monopoly','120'),('Uno','45'),('Ligretto','90');";
		
		 	con.query(sql, function (err) {
		   	 if (err) throw err;
		   	 console.log("1 record inserted");
		 });

			con.query("SELECT * FROM produse", function (err, result, fields) {
			  if (err) throw err;
			  lista = result; 
			  console.log(lista);
			  res.render("index",{msgLog:"Nimic",listProd:lista});
			});
	  });
});

app.listen(port, () => console.log(`Serverul rulează la adresa http://localhost:`));