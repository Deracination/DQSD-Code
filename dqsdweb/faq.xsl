<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes"/>
  <xsl:template match="/">
    <HTML>
      <HEAD>
        <TITLE>Dave's Quick Search DeskBar FAQ</TITLE>
        <LINK REL="stylesheet" TYPE="text/css" HREF="faq.css" />
        <SCRIPT>
          <xsl:comment>
          function toggleVisible()
          {
            if ( !document.all )
              return false;

            if ( event.srcElement.tagName == 'DIV' )
            {
              var answer = event.srcElement.parentElement.children['answer'];
              var toggle = event.srcElement.children['toggle'];
            }
            else if ( event.srcElement.id == 'toggle' )
            {
              var answer = event.srcElement.parentElement.parentElement.all['answer'];
              var toggle = event.srcElement;
            }
            if ( answer )
            {
              answer.style.display = ( answer.style.display == "none" ? "block" : "none" );
              toggle.innerText = ( answer.style.display == "none" ? "+" : "-" );
            }
            return false;
          }
          </xsl:comment>
        </SCRIPT>
      </HEAD>
      <BODY>
      <xsl:for-each select="/faqs/category">
        <div class="category">
          <xsl:variable name="category_num" select="position()"/>
          <xsl:number value="$category_num" format="1." />&#160;
          <xsl:value-of select="@name"/>
        </div>
        <xsl:for-each select="faq">
          <div class="faq">
            <div class="question" onclick="return toggleVisible();">
              <a href="#" class="toggle" id="toggle">+</a>
              <xsl:number value="$category_num" format="1" />
              <xsl:number value="position()" format=".1" />&#160;<xsl:value-of select="question"/>
            </div>
            <div id="answer" class="answer" style="display: none">
              <xsl:apply-templates select="answer"/>
            </div>
          </div>
        </xsl:for-each>
      </xsl:for-each>
      </BODY>
    </HTML>
  </xsl:template>

  <xsl:template match="link">
    <a href="{@href}"><xsl:value-of select="."/></a>
  </xsl:template>
</xsl:stylesheet>
