/*
  This file contains all the internationalized strings used in DQSD.
  Feel free to add translations for additional lanugages here. (Your
  local two-letter language code is the one returned when you type
  "navigator.userLanugage=" in the deskbar.)

  Please mail or post any additional translations to the support
  group at dqsd-devel@lists.sourceforge.net so other people can
  enjoy them too.
  
  Thanks!
*/

var MONTH_NAMES =
{
  en: Array('January','February','March','April','May','June','July','August','September','October','November','December','Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'),
  es: Array('Enero','Febrero','Marzo','Abril','Mayo','Junio','Julio','Agosto','Septiembre','Octubre','Noviembre','Diciembre','Ene','Feb','Mar','Abr','May','Jun','Jul','Ago','Sep','Oct','Nov','Dic'),
  fr: Array('Janvier','F�vrier','Mars','Avril','Mai','Juin','Juillet','Ao�t','Septembre','Octobre','Novembre','D�cembre','Janv', 'F�vr', 'Mars', 'Avr', 'Mai', 'Juin', 'Juil', 'Ao�t', 'Sept', 'Oct', 'Nov', 'D�c'),
  de: Array('Januar', 'Februar', 'M�rz', 'April', 'Mai', 'Juni', 'Juli', 'August', 'September', 'Oktober', 'November', 'Dezember', 'Jan', 'Feb', 'M�r', 'Apr', 'Mai', 'Jun', 'Jul', 'Aug', 'Sep', 'Okt', 'Nov', 'Dez'),
  no: Array('Januar','Februar','Mars','April','Mai','Juni','Juli','August','September','Oktober','November','Desember','Jan','Feb','Mar','Apr','Mai','Jun','Jul','Aug','Sep','Okt','Nov','Des'),
  nl: Array('Januari','Februari','Maart','April','Mei','Juni','Juli','Augustus','September','Oktober','November','December','Jan','Feb','Mrt','Apr','Mei','Jun','Jul','Aug','Sep','Okt','Nov','Dec'),
  dk: Array('Januar','Februar','Marts','April','Maj','Juni','Juli','August','September','Oktober','November','December','Jan','Feb','Mar','Apr','Maj','Jun','Jul','Aug','Sep','Okt','Nov','Dec'),
  pt: Array('Janeiro','Fevereiro','Mar�o','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro','Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'),
  it: Array('Gennaio', 'Febbraio', 'Marzo', 'Aprile', 'Maggio', 'Giugno', 'Luglio', 'Agosto', 'Settembre', 'Ottobre', 'Novembre', 'Dicembre', 'Gen', 'Feb', 'Mar', 'Apr', 'Mag', 'Giu', 'Lug', 'Ago', 'Set', 'Ott', 'Nov', 'Dic'),
  is: Array('Jan�ar','Febr�ar','Mars','Apr�l','Ma�','J�n�','J�l�','�g�st','September','Okt�ber','N�vember','Desember','jan.','feb.','mar.','apr.','ma�','j�n.','j�l.','�g.','sep.','okt.','n�v.','des.'),
  fi: Array('Tammikuu','Helmikuu','Maaliskuu','Huhtikuu','Toukokuu','Kes�kuu','Hein�kuu','Elokuu','Syyskuu','Lokakuu','Marraskuu','Joulu','Tammi','Helmi','Maalis','Huhti','Touko','Kes�','Hein�','Elo','Syys','Loka','Marras','Joulu'),
  et: Array('Jaanuar', 'Veebruar', 'M�rts', 'Aprill', 'Mai', 'Juuni', 'Juuli', 'August', 'September', 'Oktoober', 'November', 'Detsember', 'Jaan', 'Veebr', 'M�rts', 'Apr', 'Mai', 'Juuni', 'Juuli', 'Aug', 'Sept', 'Okt', 'Nov', 'Dets'),
  sv: Array('Januari','Februari','Mars','April','Maj','Juni','Juli','Augusti','September','Oktober','November','December','Jan','Feb','Mar','Apr','Maj','Jun','Jul','Aug','Sep','Okt','Nov','Dec'),
  cs: Array('Leden','�nor','B�ezen','Duben','Kv�ten','�erven','�ervenec','Srpen','Z���','��jen','Listopad','Prosinec','ledna','�nora','b�ezna','dubna','kv�tna','�ervna','�ervence','srpna','z���','��jna','listopadu','prosince')
};

var DAY_NAMES =
{
  en: Array('Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'),
  es: Array('Domingo','Lunes','Martes','Mi�rcoles','Jueves','Viernes','S�bado','Dom','Lun','Mar','Mie','Jue','Vie','Sab'),
  fr: Array('Dimanche','Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi','Dim','Lun','Mar','Mer','Jeu','Ven','Sam'),
  de: Array('Sonntag', 'Montag', 'Dienstag', 'Mittwoch', 'Donnerstag', 'Freitag', 'Samstag', 'Son', 'Mon', 'Die', 'Mit', 'Don', 'Fre', 'Sam'),
  no: Array('S�ndag','Mandag','Tirsdag','Onsdag','Torsdag','Fredag','L�rdag','S�n','Man','Tir','Ons','Tor','Fre','L�r'),
  nl: Array('Zondag','Maandag','Dinsdag','Woensdag','Donderdag','Vrijdag','Zaterdag','Zon','Maa','Din','Woe','Don','Vrij','Zat'),
  dk: Array('S�ndag', 'Mandag', 'Tirsdag', 'Onsdag', 'Torsdag', 'Fredag', 'L�rdag', 'S�n', 'Man', 'Tir', 'Ons', 'Tor', 'Fre', 'L�r'),
  pt: Array('Domingo','Segunda','Ter�a','Quarta','Quinta','Sexta','S�bado','Dom','Seg','Ter','Qua','Qui','Sex','Sab'),
  it: Array('Domenica', 'Luned�', 'Marted�', 'Mercoled�', 'Gioved�', 'Venerd�', 'Sabato', 'Dom', 'Lun', 'Mar', 'Mer', 'Gio', 'Ven', 'Sab'),
  is: Array('sunnudagur','m�nudagur','�ri�judagur','mi�vikudagur','fimmtudagur','f�studagur','laugardagur','sun','m�n','�ri','mi�','fim','f�s','lau'),
  fi: Array('Sunnuntai', 'Maanantai', 'Tiistai', 'Keskiviikko', 'Torstai', 'Perjantai', 'Lauantai', 'Su', 'Ma', 'Ti', 'Ke', 'To', 'Pe', 'La'),
  et: Array('P�hap�ev', 'Esmasp�ev', 'Teisip�ev', 'Kolmap�ev', 'Neljap�ev', 'Reede', 'Laup�ev', 'P', 'E', 'T', 'K', 'N', 'R', 'L'),
  sv: Array('S�ndag','M�ndag','Tisdag','Onsdag','Torsdag','Fredag','L�rdag','S�n','M�n','Tis','Ons','Tor','Fre','L�r'),
  cs: Array('Ned�le', 'Pond�l�', '�ter�', 'St�eda', '�tvrtek', 'P�tek', 'Sobota', 'Ne', 'Po', '�t', 'St', '�t', 'P�', 'So')
  };


var QUICK_SEARCH =
{
  en: "Quick Search",
  es: "B�squeda R�pida",
  fr: "Recherche Rapide",
  de: "Schnelle Suche",
  no: "Hurtigs�k",
  nl: "Snelzoek",
  dk: "Hurtig S�gning",
  pt: "Pesquisa R�pida",
  is: "Hra�leit",
  fi: "Pikahaku",
  et: "Kiirotsing",
  sv: "Snabbs�kning"
};

var HELP_TEXT =
{
  en: "Type ? for help",
  es: "Escriba ? para ayuda",
  fr: "Saisir ? pour l'aide",
  de: "? f�r Hilfe",
  no: "Skriv ? for hjelp",
  nl: "Typ ? voor hulp",
  dk: "Skriv ? for hj�lp",
  pt: "Digite ? para ajuda",
  is: "? gefur hj�lp",
  fi: "Kirjoita ? saadaksesi apua",
  et: "Abi saamiseks sisesta ?",
  sv: "Skriv ? f�r hj�lp"
};

var ABOUT_TEXT = 
{
  en: "Help / About...",
  fi: "Apua / Tietoja...",
  et: "Abi / Info...",
  sv: "Hj�lp / Om..."
};

var SEARCH_TITLE =
{
  en: "Search",
  es: "Buscar",
  fr: "Chercher",
  de: "Suchen",
  nl: "Zoek",
  no: "S�k",
  dk: "S�g",
  is: "Leit",
  fi: "Haku",
  et: "Otsing",
  sv: "S�k"
};

var NEXT_MONTH =
{
  en: "Next month",
  es: "Mes siguiente",
  de: "N�chster Monat",
  nl: "Volgende maand",
  no: "Neste m�ned",
  dk: "N�ste m�ned",
  is: "N�sti m�nu�ur",
  fi: "Seuraava kuukausi",
  et: "J�rgmine kuu",
  sv: "N�sta m�nad"
};

var PREV_MONTH =
{
  en: "Previous month",
  es: "Mes anterior",
  de: "Voriger Monat",
  nl: "Vorige maand",
  no: "Forrige m�ned",
  dk: "Forrige m�ned",
  is: "Fyrri m�nu�ur",
  fi: "Edellinen kuukausi",
  et: "Eelmine kuu",
  sv: "F�rra m�naden"
};

var NEXT_YEAR =
{
  en: "Next year",
  es: "A�o siguiente",
  de: "N�chstes Jahr",
  nl: "Volgend jaar",
  no: "Neste �r",
  dk: "N�ste �r",
  is: "N�sta �r",
  fi: "Seuraava vuosi",
  et: "J�rgmine aasta",
  sv: "N�sta �r"
};

var PREV_YEAR =
{
  en: "Previous year",
  es: "A�o anterior",
  de: "Voriges Jahr",
  nl: "Vorig jaar",
  no: "Forrige �r",
  dk: "Forrige �r",
  is: "Fyrra �r",
  fi: "Edellinen vuosi",
  et: "Eelmine aasta",
  sv: "F�rra �ret"
};

var GO_TO_TODAY =
{
  en: "Go to today",
  es: "Ir a hoy",
  de: "Wechselt auf aktuelles Datum",
  nl: "Ga naar vandaag",
  no: "G� til i dag",
  dk: "G� til i dag",
  is: "Fara � daginn � dag",
  fi: "Siirry t�h�n p�iv��n",
  et: "T�nasele p�evale",
  sv: "G� till idag"
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
