// code to do translations

function translate(t)
{
  // detect strings that ask for a translation
  var trans = "(en_no|en_zh|en_fr|en_de|en_it|en_ja|en_ko|en_pt|en_es|zh_en|fr_en|fr_de|de_en|de_fr|it_en|ja_en|ko_en|pt_en|ru_en|es_en|" +
               "en-no|en-zh|en-fr|en-de|en-it|en-ja|en-ko|en-pt|en-es|zh-en|fr-en|fr-de|de-en|de-fr|it-en|ja-en|ko-en|pt-en|ru-en|es-en)";
  var prot = new RegExp("^" + trans + "(.+)","gi");
  var prot2 = new RegExp("(.+)" + trans + "$","gi");
  if (prot.exec(t))
  {
     var temp1 = t.replace(prot,"$1").replace("-", "_");
     var temp2 = t.replace(prot,"$2");
     choosetranslate(temp1,temp2);
     return true;
  }
  else if (prot2.exec(t))
  {
     var temp2 = t.replace(prot2,"$1");
     var temp1 = t.replace(prot2,"$2").replace("-", "_");
     choosetranslate(temp1,temp2);
     return true;
  }
  else
     return false;
}

var FREE_TRANSLATIONS = 
{
  es_en: "Spanish/English",
  fr_en: "French/English",
  de_en: "German/English",
  it_en: "Italian/English",
  pt_en: "Portuguese/English",
  en_es: "English/Spanish",
  en_fr: "English/French",
  en_it: "English/Italian",
  en_no: "English/Norwegian",
  en_pt: "English/Portuguese"
};

function choosetranslate(l,q)
{
  if (FREE_TRANSLATIONS[l] != null)
    freetranslate(FREE_TRANSLATIONS[l], q);
  else
    babeltranslate(l, q);
}

function babeltranslate(l,q)
{
  //q is the query, l is the language choices
  var prot = new RegExp("^(http://|https://)([\\-a-z0-9]+\\.)*[\\-a-z0-9]+" +
                       "|^[a-z]:($|\\\\)" +
                       "|^\\\\\\\\[a-z]+($|\\\\($|[a-z]+($|\\\\)))", "i");
  var dns = new RegExp("^www\.([\\-a-z0-9]+\\.)+[\\-a-z0-9]+$" +
                   "|^([\\-a-z0-9]+\\.)+(com|net|org|edu|gov|mil|[a-z]{2})$", "i");

  q = q.replace(/^\s*/, '').replace(/\s*$/, '');

  babl.lp.value = l;

  if (dns.exec(q))
  {
    babl.tt.value = "url";
    babl.url.value = "http://" + q;
  }
  else if (prot.exec(q))
  {
    babl.tt.value = "url";
    babl.url.value = q;
  }
  else
  {
    babl.tt.value = "urltext";
    babl.urltext.value = q;
  }
  submitForm(babl);
}

function freetranslate(l,q)
{
  //q is the query, l is the language choices
  var prot = new RegExp("^(http://|https://)([\\-a-z0-9]+\\.)*[\\-a-z0-9]+" +
                       "|^[a-z]:($|\\\\)" +
                       "|^\\\\\\\\[a-z]+($|\\\\($|[a-z]+($|\\\\)))", "i");
  var dns = new RegExp("^www\.([\\-a-z0-9]+\\.)+[\\-a-z0-9]+$" +
                   "|^([\\-a-z0-9]+\\.)+(com|net|org|edu|gov|mil|[a-z]{2})$", "i");

  q = q.replace(/^\s*/, '').replace(/\s*$/, '');
  if (dns.exec(q))
  {
    fturl.language.value = l;
    fturl.url.value = "http://" + q;
    submitForm(fturl);
  }
  else if (prot.exec(q))
  {
    fturl.language.value = l;
    fturl.url.value = q;
    submitForm(fturl);
  }
  else
  {
    fttext.Language.value = l;
    fttext.SrcText.value = q;
    submitForm(fttext);
  }
}

// form for Babelfish translation

document.write(
  "<form name=babl"
+ "      target=_blank"
+ "      action=http://babelfish.altavista.com/babelfish/tr"
+ "      method=post>"
+ "  <input type=hidden name=doit value=done>"
+ "  <input type=hidden name=lp>"
+ "  <input type=hidden name=tt>"
+ "  <input type=hidden name=urltext>"
+ "  <input type=hidden name=url>"
+ "</form>");

// form for FreeTranlsation.com translation by url

document.write(
  "<form name=fturl"
+ "      target=_blank"
+ "      action=http://www.freetranslation.com/web.asp"
+ "      method=get>"
+ "  <input type=hidden name=language>"
+ "  <input type=hidden name=url>"
+ "  <input type=hidden name=Sequence VALUE=core>"
+ "</form>");


// form for FreeTranlsation.com translation of text
document.write(
  "<form name=fttext"
+ "      target=_blank"
+ "      action=http://ets.freetranslation.com/"
+ "      method=get>"
+ "  <input type=hidden name=Sequence VALUE=core>"
+ "  <input type=hidden name=Mode value=html>"
+ "  <input type=hidden name=template value=TextResults2.htm>"
+ "  <input type=hidden name=Language>"
+ "  <input type=hidden name=SrcText>"
+ "</form>");


