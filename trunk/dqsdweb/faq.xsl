<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" 
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:html="http://www.w3.org/1999/xhtml">
  <xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes"/>

  <xsl:template match="/">
    <html>
      <head>
        <title><xsl:value-of select="/faq_document/doc_title"/></title>
        <link rel="stylesheet" type="text/css" href="faq.css" />
        <script>
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
        </script>
        <script language="Javascript1.2" src="utilities.js"></script>
      </head>
      <body>
        <xsl:apply-templates select="/faq_document/doc_title"/>

        <div class="lastupdate">
          <nobr>Last updated:
            <script>
              <xsl:comment>
                document.write( getDisplayDate( document.lastModified ) );
              </xsl:comment>
            </script>
          </nobr>
        </div>

        <div class="homepage">
        [ <a href="doc.htm">Dave's Quick Search Deskbar</a> ]
        </div>
  
        <xsl:apply-templates select="/faq_document/faqs"/>

        <div class="homepage">
        [ <a href="doc.htm">Dave's Quick Search Deskbar</a> ]
        </div>
  
      </body>
    </html>
  </xsl:template>
   
  <xsl:template match="link">
    <a href="{@href}"><xsl:value-of select="."/></a>
  </xsl:template>

  <xsl:template match="doc_title">
    <h2><xsl:value-of select="."/></h2>
  </xsl:template>
  
  <xsl:template match="key_bindings">
  	  <table class="keybindings">
			<tbody>
				<tr>
					<th class="keyname">Key</th><th>Description</th>
				</tr>
				<xsl:apply-templates select="key" />
			</tbody>
		</table>
  </xsl:template>
  
  <xsl:template match="key">
    <tr><td class="keyname"><xsl:value-of select="@name"/></td><td><xsl:value-of select="."/></td></tr>
  </xsl:template>
  
  <xsl:template match="html:*">
    <xsl:element name="{local-name()}">
      <xsl:for-each select="@*">
        <xsl:attribute name="{local-name()}">
          <xsl:value-of select="." />
        </xsl:attribute>
      </xsl:for-each>
      <xsl:apply-templates />
    </xsl:element>
  </xsl:template>

  <xsl:template match="/faq_document/faqs">
      <xsl:for-each select="category">
        <div class="category">
          <xsl:variable name="category_num" select="position()"/>
          <xsl:number value="$category_num" format="1." />&#160;
          <span class="categorytitle"><xsl:value-of select="@name"/></span><!-- &#160;<span class="toplink">[ <a href="#top">top</a> ]</span> -->
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
  </xsl:template>

</xsl:stylesheet>
