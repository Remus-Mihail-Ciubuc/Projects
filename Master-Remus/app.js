const express = require('express');
const expressLayouts = require('express-ejs-layouts');
const bodyParser = require('body-parser')
const fs = require('fs');
const cookieParser = require('cookie-parser');

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
app.use( express.static('imagini'));
app.use(express.static('js'));
// la accesarea din browser adresei http://localhost:6789/ se va returna textul 'Hello World'
// proprietățile obiectului Request - req - https://expressjs.com/en/api.html#req
// proprietățile obiectului Response - res - https://expressjs.com/en/api.html#res

app.use(cookieParser()) ;

let ofertaMasini=fs.readFileSync('masini.json');
const listaMasini = JSON.parse(ofertaMasini);

app.get('/', (req, res) => {
    var msgLog="";
	if(!req.cookies.user)
	{
		req.cookies.user={};
	}else{
		msgLog="Bine ai venit: "+req.cookies.user;
	}
	res.render("index",{msgLog:msgLog,listMasini:listaMasini});
});

let rawUseri = fs.readFileSync('utilizatori.json');
const listaUtilizatori = JSON.parse(rawUseri);




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
    rezervari=[];
    nrRez=0;
	res.redirect("/");
});

var rezervari=[];
var nrRez=0;
app.post('/rezervare', (req, res) => {
    rezervari[nrRez]=req.body;
    console.log(rezervari);
    nrRez+=1;
	res.render('vizualizare_programari',{listRez:rezervari,nr:nrRez});
});

app.post('/stergeRezervare', (req, res) => {
    console.log(req.body.index);
    delete rezervari[req.body.index];
    nrRez-=1;
    if(rezervari==[])
        res.redirect("/");
	res.render('vizualizare_programari',{listRez:rezervari,nr:nrRez});
});

app.get('/vizRezervari', (req, res) => {
	res.render('vizualizare_programari',{listRez:rezervari,nr:nrRez});
});

app.listen(port, () => console.log(`Serverul rulează la adresa http://localhost:`));