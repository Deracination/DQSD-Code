/*
 * http://www.familysearch.org/Eng/Library/fhlcatalog/func.js
 * 11 Oct 2002 - prepended fhlc_ to filename and functions to avoid collisions
 */

function fhlc_ReplaceLigatures (InString)    
  {  
  	if(InString == "")
       return "";
    
  	var OutString = "";  
    var Count = 0;
    var TempChar = ' ';
  	for (Count=0; Count < InString.length; Count++)    
  	{  
  		TempChar=InString.substring (Count, Count+1);  
		if (TempChar=="Š" || TempChar=="š")  
   			OutString=OutString+"s";  
		else if (TempChar=="Ÿ" || TempChar=="ÿ" || TempChar=="Ý" || TempChar=="ý")  
   			OutString=OutString+"y";  
  		else if (TempChar=="À" || TempChar=="Á" || TempChar=="Â" || TempChar=="Ã" || TempChar=="Ä" || TempChar=="Å"
              || TempChar=="à" || TempChar=="á" || TempChar=="â" || TempChar=="ã" || TempChar=="ä" || TempChar=="å")  
  			OutString=OutString+"a"; 
		else if (TempChar=="Ç" || TempChar=="ç")  
   			OutString=OutString+"c";  
  		else if (TempChar=="È" || TempChar=="É" || TempChar=="Ê" || TempChar=="Ë"
              || TempChar=="è" || TempChar=="é" || TempChar=="ê" || TempChar=="ë")  
  			OutString=OutString+"e"; 
  		else if (TempChar=="Ì" || TempChar=="Í" || TempChar=="Î" || TempChar=="Ï"
              || TempChar=="ì" || TempChar=="í" || TempChar=="î" || TempChar=="ï")  
  			OutString=OutString+"i"; 
  		else if (TempChar=="Ð" || TempChar=="ð")  
  			OutString=OutString+"d"; 
  		else if (TempChar=="Ñ" || TempChar=="ñ")  
  			OutString=OutString+"n"; 
  		else if (TempChar=="Ò" || TempChar=="Ó" || TempChar=="Ô" || TempChar=="Õ" || TempChar=="Ö" || TempChar=="Ø"
              || TempChar=="ò" || TempChar=="ó" || TempChar=="ô" || TempChar=="õ" || TempChar=="ö" || TempChar=="ø")  
  			OutString=OutString+"o"; 
  		else if (TempChar=="Ù" || TempChar=="Ú" || TempChar=="Û" || TempChar=="Ü"
              || TempChar=="ù" || TempChar=="ú" || TempChar=="û" || TempChar=="ü")  
  			OutString=OutString+"u"; 
  		else if (TempChar=="Œ" || TempChar=="œ")  
  			OutString=OutString+"oe"; 
  		else if (TempChar=="Æ" || TempChar=="æ")  
  			OutString=OutString+"ae"; 
  		else if (TempChar=="Þ" || TempChar=="þ")  
  			OutString=OutString+"th"; 
  		else if (TempChar=="ß")  
  			OutString=OutString+"ss"; 
  		else  
  			OutString=OutString+TempChar;  
  	}  
  	return (OutString);  
  }  

  function fhlc_ReplaceSubstring(Old,New,InString,minLength)
  {
  	if(InString == "")
       return "";
    
	var CurrString = "" + InString;

	var index = CurrString.indexOf(Old,0);

    while(index != -1)
	{
        var iPrevSpace = -1;
        var i = 0;
        
        for(i = 0; i < index; i++)
            if(CurrString.charAt(i) ==' ' || CurrString.charAt(i) =='_')
                iPrevSpace = i;

        var iCharsBeforeInWord = index - iPrevSpace - 1;

	    var iNextSpace = -1;
        for(i = CurrString.length; i > index; i--)
            if(CurrString.charAt(i) ==' ' || CurrString.charAt(i) =='_')
                iNextSpace = i;

        if(iNextSpace == -1)
            iNextSpace = CurrString.length;

        var iCharsAfterInWord = iNextSpace - index - Old.length;

        var iLetters = iCharsBeforeInWord + Old.length + iCharsAfterInWord;

        if(iLetters >= minLength)
		    CurrString = CurrString.substring(0,index) + New + CurrString.substring(index+Old.length,CurrString.length); 

        if(index + 1 <= CurrString.length)
            index = CurrString.indexOf(Old,index+1);
        else
            index = -1;

        if(CurrString == "")
            index = -1;
	} 
	      
  	return (CurrString);  
  }

  function fhlc_ReplaceBeginning(Old,New,InString,minLength)
  {
  	if(InString == "")
       return "";
    
	var strTemp = "" + InString;
	var OutString = "";

	var iIndex = strTemp.indexOf(Old,0);

    while(iIndex != -1)
	{
		// is the bit we want to replace right at the beginning of the string?
		if(iIndex == 0)
		{
			// ensure the first word of the string is long enough
			if(strTemp.length > minLength 
			         && (strTemp.indexOf(' ',0) == -1 || strTemp.indexOf(' ',0) > minLength)
					 && (strTemp.indexOf('_',0) == -1 || strTemp.indexOf('_',0) > minLength))
		        OutString = New + strTemp.substring(Old.length,strTemp.length);
		    else
		        OutString = "" + strTemp;
		} 
		// is there a space or'_' before the bit we want to replace?
		else if(strTemp.charAt(iIndex-1) == " " || strTemp.charAt(iIndex-1) == "_")
		{
		    if(strTemp.length > (iIndex + minLength) 
			   && (strTemp.indexOf(' ',iIndex) == -1 || strTemp.indexOf(' ',iIndex) > (iIndex + minLength))
			   && (strTemp.indexOf('_',iIndex) == -1 || strTemp.indexOf('_',iIndex) > (iIndex + minLength)))
		        OutString = strTemp.substring(0,iIndex) + New + strTemp.substring(iIndex+Old.length,strTemp.length); 
		    else
		        OutString = "" + strTemp;
		}
		else
		    OutString = "" + strTemp;

	    strTemp = "" + OutString;
	    OutString = "";

	    if(strTemp != "")
            iIndex = strTemp.indexOf(Old,iIndex+1);
        else
            iIndex = -1;        
	} 
	    
    OutString = "" + strTemp;
  
  	return (OutString);  
  }

  function fhlc_ReverseString(InString)
  {
  	if(InString == "")
       return "";
    
  	var OutString = "";  
    var Count = 0;
    var TempChar = ' ';

  	for (Count = InString.length - 1; Count >= 0; Count--)    
  	{  
  		TempChar=InString.charAt(Count);  
  	    OutString=OutString+TempChar;  
  	}  
  	
	return (OutString);  
  }

  function fhlc_ReplaceEnding(Old,New,InString,minLength)
  {
  	if(InString == "")
       return "";
    
	// reverse the strings
	var revOld = fhlc_ReverseString(Old);
	var revNew = fhlc_ReverseString(New);
	var revInString = fhlc_ReverseString(InString);
	
	// effectively we now want to replace the beginning
	var revOutString = fhlc_ReplaceBeginning(revOld,revNew,revInString,minLength);
	
	// remember to reverse back the string we are after
	var OutString = fhlc_ReverseString(revOutString);
  
  	return (OutString);  
  }

  function fhlc_LoseStopword(Stopword,InString) 
  {
  	if(InString == "")
       return "";
    
    var locOutString = "";	
	var strTemp = InString;
	var lStop = Stopword.length;

	// is the stopword in the string?
	var iIndex = strTemp.indexOf(Stopword,0);

    while(iIndex != -1)
	{
		// if the stopword is the start of the string
		if(iIndex == 0)
		{
            if(strTemp.charAt(lStop) == ' ' || strTemp.charAt(lStop) == '_' || strTemp.length==lStop)
		        locOutString = strTemp.substring(lStop+1,strTemp.length);
		    else
		        locOutString = strTemp;
		} 
		// if the stopword is the end of the string
		else if(iIndex == (strTemp.length-lStop))
		{
            if(strTemp.charAt(iIndex-1) == ' ' || strTemp.charAt(iIndex-1) == '_')
		        locOutString = strTemp.substring(0,iIndex-1);
		    else
		        locOutString = strTemp;
		} 
		// if the stopword is in the middle of the string
		else if((strTemp.charAt(iIndex-1) == " " || strTemp.charAt(iIndex-1) == "_")
		      &&(strTemp.charAt(iIndex+lStop) == " " || strTemp.charAt(iIndex+lStop) == "_"))
        {
			locOutString = strTemp.substring(0,iIndex) + strTemp.substring(iIndex+lStop,strTemp.length);
        }
		// if the stopword is not in the string
		else
		    locOutString = strTemp;

	    strTemp = locOutString;
	    locOutString = "";

	    if(strTemp != "")
            iIndex = strTemp.indexOf(Stopword,iIndex+1);
        else
            iIndex = -1;        
	} 
	    
    locOutString = "" + strTemp;

  	return (locOutString);  
  }
     
  function fhlc_Normalise(InString)    
  {  
  	if(InString == "")
       return "";
    
  	// we'll return this when we're done
	OutString="";  

    // convert to lower case
	OutString="" + InString.toLowerCase();

 	// get rid of stopwords
	OutString = fhlc_LoseStopword("a",OutString);
	OutString = fhlc_LoseStopword("an",OutString);
	OutString = fhlc_LoseStopword("and",OutString);
	OutString = fhlc_LoseStopword("at",OutString);
	OutString = fhlc_LoseStopword("da",OutString);
	OutString = fhlc_LoseStopword("das",OutString);
	OutString = fhlc_LoseStopword("de",OutString);
	OutString = fhlc_LoseStopword("del",OutString);
	OutString = fhlc_LoseStopword("dels",OutString);
	OutString = fhlc_LoseStopword("dem",OutString);
	OutString = fhlc_LoseStopword("den",OutString);
	OutString = fhlc_LoseStopword("der",OutString);
	OutString = fhlc_LoseStopword("des",OutString);
	OutString = fhlc_LoseStopword("det",OutString);
	OutString = fhlc_LoseStopword("di",OutString);
	OutString = fhlc_LoseStopword("die",OutString);
	OutString = fhlc_LoseStopword("du",OutString);
	OutString = fhlc_LoseStopword("e",OutString);
	OutString = fhlc_LoseStopword("een",OutString);
	OutString = fhlc_LoseStopword("ein",OutString);
	OutString = fhlc_LoseStopword("eine",OutString);
	OutString = fhlc_LoseStopword("eines",OutString);
	OutString = fhlc_LoseStopword("el",OutString);
	OutString = fhlc_LoseStopword("els",OutString);
	OutString = fhlc_LoseStopword("em",OutString);
	OutString = fhlc_LoseStopword("en",OutString);
	OutString = fhlc_LoseStopword("et",OutString);
	OutString = fhlc_LoseStopword("ett",OutString);
	OutString = fhlc_LoseStopword("for",OutString);
	OutString = fhlc_LoseStopword("from",OutString);
	OutString = fhlc_LoseStopword("gli",OutString);
	OutString = fhlc_LoseStopword("gl",OutString);
	OutString = fhlc_LoseStopword("het",OutString);
	OutString = fhlc_LoseStopword("il",OutString);
	OutString = fhlc_LoseStopword("im",OutString);
	OutString = fhlc_LoseStopword("in",OutString);
	OutString = fhlc_LoseStopword("is",OutString);
	OutString = fhlc_LoseStopword("la",OutString);
	OutString = fhlc_LoseStopword("las",OutString);
	OutString = fhlc_LoseStopword("le",OutString);
	OutString = fhlc_LoseStopword("les",OutString);
	OutString = fhlc_LoseStopword("los",OutString);
	OutString = fhlc_LoseStopword("not",OutString);
	OutString = fhlc_LoseStopword("o",OutString);
	OutString = fhlc_LoseStopword("of",OutString);
	OutString = fhlc_LoseStopword("on",OutString);
	OutString = fhlc_LoseStopword("or",OutString);
	OutString = fhlc_LoseStopword("os",OutString);
	OutString = fhlc_LoseStopword("te",OutString);
	OutString = fhlc_LoseStopword("the",OutString);
	OutString = fhlc_LoseStopword("to",OutString);
	OutString = fhlc_LoseStopword("um",OutString);
	OutString = fhlc_LoseStopword("uma",OutString);
	OutString = fhlc_LoseStopword("un",OutString);
	OutString = fhlc_LoseStopword("una",OutString);
	OutString = fhlc_LoseStopword("und",OutString);
	OutString = fhlc_LoseStopword("une",OutString);
	OutString = fhlc_LoseStopword("y",OutString);
	OutString = fhlc_LoseStopword("ye",OutString);
	OutString = fhlc_LoseStopword("yr",OutString);

	// convert disallowed characters
	strTemp = "" + OutString;
	OutString = "";
	
	for (Count=0; Count < strTemp.length; Count++)    
  	{  
		TempChar=strTemp.charAt(Count);  
  		
		if (TempChar=="@" || TempChar=="#" || TempChar=="$" || TempChar=="^" || TempChar=="&" || TempChar=="("  
		    || TempChar==")" || TempChar=="-" || TempChar=="/" || TempChar=="<" || TempChar==">" || TempChar=="="
            || TempChar==":" || TempChar==";")  
  			OutString=OutString+"_";  
		else if (TempChar=="?" || TempChar=="," || TempChar=="'" || TempChar=='"' || TempChar=="+" || TempChar==".")  
   			OutString=OutString;  
		else if (TempChar=="Š" || TempChar=="š")  
   			OutString=OutString+"s";  
		else if (TempChar=="Ÿ" || TempChar=="ÿ" || TempChar=="Ý" || TempChar=="ý")  
   			OutString=OutString+"y";  
  		else if (TempChar=="À" || TempChar=="Á" || TempChar=="Â" || TempChar=="Ã" || TempChar=="Ä" || TempChar=="Å"
              || TempChar=="à" || TempChar=="á" || TempChar=="â" || TempChar=="ã" || TempChar=="ä" || TempChar=="å")  
  			OutString=OutString+"a"; 
		else if (TempChar=="Ç" || TempChar=="ç")  
   			OutString=OutString+"c";  
  		else if (TempChar=="È" || TempChar=="É" || TempChar=="Ê" || TempChar=="Ë"
              || TempChar=="è" || TempChar=="é" || TempChar=="ê" || TempChar=="ë")  
  			OutString=OutString+"e"; 
  		else if (TempChar=="Ì" || TempChar=="Í" || TempChar=="Î" || TempChar=="Ï"
              || TempChar=="ì" || TempChar=="í" || TempChar=="î" || TempChar=="ï")  
  			OutString=OutString+"i"; 
  		else if (TempChar=="Ð" || TempChar=="ð")  
  			OutString=OutString+"d"; 
  		else if (TempChar=="Ñ" || TempChar=="ñ")  
  			OutString=OutString+"n"; 
  		else if (TempChar=="Ò" || TempChar=="Ó" || TempChar=="Ô" || TempChar=="Õ" || TempChar=="Ö" || TempChar=="Ø"
              || TempChar=="ò" || TempChar=="ó" || TempChar=="ô" || TempChar=="õ" || TempChar=="ö" || TempChar=="ø")  
  			OutString=OutString+"o"; 
  		else if (TempChar=="Ù" || TempChar=="Ú" || TempChar=="Û" || TempChar=="Ü"
              || TempChar=="ù" || TempChar=="ú" || TempChar=="û" || TempChar=="ü")  
  			OutString=OutString+"u"; 
  		else if (TempChar=="Œ" || TempChar=="œ")  
  			OutString=OutString+"oe"; 
  		else if (TempChar=="Æ" || TempChar=="æ")  
  			OutString=OutString+"ae"; 
  		else if (TempChar=="Þ" || TempChar=="þ")  
  			OutString=OutString+"th"; 
  		else if (TempChar=="ß")  
  			OutString=OutString+"ss"; 
  		else  
  			OutString=OutString+TempChar;  
  	}  

	// get rid of multiple spaces
	strTemp = "" + OutString;
	OutString = "";
	
	for (Count=0; Count < strTemp.length; Count++)    
  	{  
		TempChar=strTemp.charAt(Count);  
  		
		if (TempChar==" " || TempChar=="_")
		{
		    if (strTemp.charAt(Count+1)==" " || strTemp.charAt(Count+1)=="_")
  			    OutString=OutString;
			else
  			    OutString=OutString+"_";  
		}  
  		else  
  			OutString=OutString+TempChar;  
  	}  

	// get rid of leading spaces
	while(OutString.charAt(0)==" " || OutString.charAt(0)=="_")
	    OutString = OutString.substring(1,OutString.length);

	// get rid of trailing spaces
	len = OutString.length;
	while(OutString.charAt(len-1)==" " || OutString.charAt(len-1)=="_")
	{
	    OutString = OutString.substring(0,len-1);
		len--;
	}


    // replace beginnings
    OutString = fhlc_ReplaceBeginning("mc","mac",OutString,3);
    OutString = fhlc_ReplaceBeginning("m'","mac",OutString,3);

    // replace endings
    OutString = fhlc_ReplaceEnding("sses","ss",OutString,7);
    OutString = fhlc_ReplaceEnding("ies","i",OutString,6);
    OutString = fhlc_ReplaceEnding("s","",OutString,5);
    OutString = fhlc_ReplaceEnding("gue","g",OutString,4);
    OutString = fhlc_ReplaceEnding("gu","g",OutString,4);
    OutString = fhlc_ReplaceEnding("amme","am",OutString,5);

    // replace substrings
    OutString = fhlc_ReplaceSubstring("iz","is",OutString,5);

	// return the normalised string
  	return (OutString);  
  }  

  // extended to handle single character wildcard
  function fhlc_repPCWithStar (InString)    
  {  
  	if(InString == "")
       return "";
    
  	var OutString = "";  
    var Count = 0;
    var TempChar = ' ';
  	for (Count=0; Count < InString.length; Count++)    
  	{  
  		TempChar=InString.substring (Count, Count+1);  
  		if (TempChar=="%")  
  			OutString=OutString+"*";  
  		else if (TempChar=="_")  
  			OutString=OutString+"!";  
  		else  
  			OutString=OutString+TempChar;  
  	}  
  	return (OutString);  
  }  

  function fhlc_repSpWithU (InString)    
  {  
  	if(InString == "")
       return "";
    
  	var OutString="";  
  	var Count = 0;
    var TempChar = ' ';
    for (Count=0; Count < InString.length; Count++)    
  	{  
  		TempChar=InString.substring (Count, Count+1);  
  		if (TempChar!=" ")  
  			OutString=OutString+TempChar;  
  		else  
  			OutString=OutString+"_";  
  	}  
  	return (OutString);  
  }  

  function fhlc_repUWithSp (InString)    
  {  
  	if(InString == "")
       return "";
    
  	var OutString="";  
  	var Count = 0;
    var TempChar = ' ';
    for (Count=0; Count < InString.length; Count++)    
  	{  
  		TempChar=InString.substring (Count, Count+1);  
  		if (TempChar!="_")  
  			OutString=OutString+TempChar;  
  		else  
  			OutString=OutString+" ";  
  	}  
  	return (OutString);  
  }  

function fhlc_Clean(strIn)
{ 
  if(strIn == "") 
    return ""; 

  strOut = ""; 
  for (iCount=0; iCount < strIn.length; iCount++) 
  {  
    chTemp=strIn.charAt(iCount);    	
    if (chTemp==" ")  
      strOut=strOut;  
    else if (chTemp=="0" && strOut=="")  
      strOut=strOut;  
    else  
      strOut=strOut+chTemp;  
  }  
  return (strOut);
}
