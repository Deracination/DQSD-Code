/*
  This file contains all the internationalized strings used in DQSD.
  Feel free to add translations for additional lanugages here. (Your
  local two-letter language code is the one returned when you type
  "navigator.userLanugage=" in the deskbar.)

  Please mail or post any additional translations to the support
  group at dqsdd@groups.yahoo.com so other people can enjoy them too.
  
  Thanks!
*/

var MONTH_NAMES =
{
  en: Array('January','February','March','April','May','June','July','August','September','October','November','December','Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'),
  es: Array('Enero','Febrero','Marzo','Abril','Mayo','Junio','Julio','Agosto','Septiembre','Octubre','Noviembre','Diciembre','Ene','Feb','Mar','Abr','May','Jun','Jul','Ago','Sep','Oct','Nov','Dic'),
  fr: Array('Janvier','Février','Mars','Avril','Mai','Juin','Juillet','Août','Septembre','Octobre','Novembre','Décembre','Janv', 'Févr', 'Mars', 'Avr', 'Mai', 'Juin', 'Juil', 'Août', 'Sept', 'Oct', 'Nov', 'Déc'),
  de: Array('Januar', 'Februar', 'März', 'April', 'Mai', 'Juni', 'Juli', 'August', 'September', 'Oktober', 'November', 'Dezember', 'Jan', 'Feb', 'Mär', 'Apr', 'Mai', 'Jun', 'Jul', 'Aug', 'Sep', 'Okt', 'Nov', 'Dez'),
  no: Array('Januar','Februar','Mars','April','Mai','Juni','Juli','August','September','Oktober','November','Desember','Jan','Feb','Mar','Apr','Mai','Jun','Jul','Aug','Sep','Okt','Nov','Des'),
  nl: Array('Januari','Februari','Maart','April','Mei','Juni','Juli','Augustus','September','Oktober','November','December','Jan','Feb','Mrt','Apr','Mei','Jun','Jul','Aug','Sep','Okt','Nov','Dec'),
  dk: Array('Januar','Februar','Marts','April','Maj','Juni','Juli','August','September','Oktober','November','December','Jan','Feb','Mar','Apr','Maj','Jun','Jul','Aug','Sep','Okt','Nov','Dec'),
  pt: Array('Janeiro','Fevereiro','Março','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro','Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'),
  it: Array('Gennaio', 'Febbraio', 'Marzo', 'Aprile', 'Maggio', 'Giugno', 'Luglio', 'Agosto', 'Settembre', 'Ottobre', 'Novembre', 'Dicembre', 'Gen', 'Feb', 'Mar', 'Apr', 'Mag', 'Giu', 'Lug', 'Ago', 'Set', 'Ott', 'Nov', 'Dic')
};

var DAY_NAMES =
{
  en: Array('Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'),
  es: Array('Domingo','Lunes','Martes','Miércoles','Jueves','Viernes','Sábado','Dom','Lun','Mar','Mie','Jue','Vie','Sab'),
  fr: Array('Dimanche','Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi','Dim','Lun','Mar','Mer','Jeu','Ven','Sam'),
  de: Array('Sonntag', 'Montag', 'Dienstag', 'Mittwoch', 'Donnerstag', 'Freitag', 'Samstag', 'Son', 'Mon', 'Die', 'Mit', 'Don', 'Fre', 'Sam'),
  no: Array('Søndag','Mandag','Tirsdag','Onsdag','Torsdag','Fredag','Lørdag','Søn','Man','Tir','Ons','Tor','Fre','Lør'),
  nl: Array('Zondag','Maandag','Dinsdag','Woensdag','Donderdag','Vrijdag','Zaterdag','Zon','Maa','Din','Woe','Don','Vrij','Zat'),
  dk: Array('Søndag', 'Mandag', 'Tirsdag', 'Onsdag', 'Torsdag', 'Fredag', 'Lørdag', 'Søn', 'Man', 'Tir', 'Ons', 'Tor', 'Fre', 'Lør'),
  pt: Array('Domingo','Segunda','Terça','Quarta','Quinta','Sexta','Sábado','Dom','Seg','Ter','Qua','Qui','Sex','Sab'),
  it: Array('Domenica', 'Lunedì', 'Martedì', 'Mercoledì', 'Giovedì', 'Venerdì', 'Sabato', 'Dom', 'Lun', 'Mar', 'Mer', 'Gio', 'Ven', 'Sab')
};




var QUICK_SEARCH =
{
  en: "Quick Search",
  es: "Búsqueda Rápida",
  fr: "Recherche Rapide",
  de: "Schnelle Suche",
  no: "Hurtigsøk",
  nl: "Snelzoek",
  dk: "Hurtig Søgning",
  pt: "Pesquisa Rápida"
};

var HELP_TEXT =
{
  en: "Type ? for help",
  es: "Escriba ? para ayuda",
  fr: "Saisir ? pour l'aide",
  de: "? für Hilfe",
  no: "Skriv ? for hjelp",
  nl: "Typ ? voor hulp",
  dk: "Skriv ? for hjælp",
  pt: "Digite ? para ajuda"
};

var SEARCH_TITLE =
{
  en: "Search",
  es: "Buscar",
  fr: "Chercher",
  de: "Suchen",
  nl: "Zoek",
  no: "Søk",
  dk: "Søg"
};

var NEXT_MONTH =
{
  en: "Next month",
  es: "Mes siguiente",
  de: "Nächster Monat",
  nl: "Volgende maand",
  no: "Neste måned",
  dk: "Næste måned"
};

var PREV_MONTH =
{
  en: "Previous month",
  es: "Mes anterior",
  de: "Voriger Monat",
  nl: "Vorige maand",
  no: "Forrige måned",
  dk: "Forrige måned"
};

var NEXT_YEAR =
{
  en: "Next year",
  es: "Año siguiente",
  de: "Nächstes Jahr",
  nl: "Volgend jaar",
  no: "Neste år",
  dk: "Næste år"
};

var PREV_YEAR =
{
  en: "Previous year",
  es: "Año anterior",
  de: "Voriges Jahr",
  nl: "Vorig jaar",
  no: "Forrige år",
  dk: "Forrige år"
};

var GO_TO_TODAY =
{
  en: "Go to today",
  es: "Ir a hoy",
  de: "Wechselt auf aktuelles Datum",
  nl: "Ga naar vandaag",
  no: "Gå til i dag",
  dk: "Gå til i dag"
};


function getMonthName(m)
{
  var arr = MONTH_NAMES[language];
  if (!arr) arr = MONTH_NAMES["en"];
  return arr[m - 1];
}

function getDayName(d)
{
  var arr = DAY_NAMES[language];
  if (!arr) arr = DAY_NAMES["en"];
  return arr[d - 1];
}

function local(stringmap)
{
  var qs = stringmap[language];
  if (!qs) return stringmap["en"];
  return qs;
}



