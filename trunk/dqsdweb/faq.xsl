<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:html="http://www.w3.org/1999/xhtml">
   <xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes" />

   <xsl:template match="/">
      <html>
         <head>
            <title>
               <xsl:value-of select="/faq_document/doc_title" />
            </title>

            <link rel="stylesheet" type="text/css" href="faq.css" />

<script language="Javascript1.2" src="utilities.js">
</script>
         </head>

         <body>
            <xsl:element name="a">
            	<xsl:attribute name="name">#top</xsl:attribute>
         	</xsl:element>

			<xsl:apply-templates select="/faq_document/doc_title" />

            <div class="lastupdate">
               <nobr>Last updated: 
<script>
              
<xsl:comment>                document.write( getDisplayDate( document.lastModified ) );
              </xsl:comment>








            
</script>
               </nobr>
            </div>

            <div class="homepage">[ 
            <a href="doc.htm">Dave's Quick Search Deskbar</a>

            ]</div>

            <xsl:apply-templates select="/faq_document/faqs" />

            <div class="homepage">[ 
            <a href="doc.htm">Dave's Quick Search Deskbar</a>

            ]</div>
         </body>
      </html>
   </xsl:template>

   <xsl:template match="link">
      <a href="{@href}">
         <xsl:value-of select="." />
      </a>
   </xsl:template>

   <xsl:template match="doc_title">
      <h2>
         <xsl:value-of select="." />
      </h2>
   </xsl:template>

   <xsl:template match="key_bindings">
      <table class="keybindings">
         <tbody>
            <tr>
               <th class="keyname">Key</th>

               <th>Description</th>
            </tr>

            <xsl:apply-templates select="key" />
         </tbody>
      </table>
   </xsl:template>

   <xsl:template match="key">
      <tr>
         <td class="keyname">
            <xsl:value-of select="@name" />
         </td>

         <td>
            <xsl:value-of select="." />
         </td>
      </tr>
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
      <br />

      <center>
 		 <table class="toc">
            <tr>
               <xsl:for-each select="category">
                  <td>
                     <xsl:element name="a">
                        <xsl:attribute name="href">#<xsl:value-of select="@name" />
                        </xsl:attribute>
                        <xsl:value-of select="@name" />
                     </xsl:element>
                  </td>
               </xsl:for-each>
            </tr>
         </table>
      </center>

      <br />

      <xsl:for-each select="category">
         <xsl:variable name="category_num" select="position()" />

         <div class="category">
         <xsl:number value="$category_num" format="1." />

          
         <span class="categorytitle">
            <xsl:element name="a">
               <xsl:attribute name="name">
                  <xsl:value-of select="@name" />
               </xsl:attribute>

               <xsl:value-of select="@name" />
            </xsl:element>
         </span>

         <span class="toplink">[ 
         <xsl:element name="a">
         <xsl:attribute name="href">#top</xsl:attribute>

         Top</xsl:element>

         ]</span>
         </div>

         <xsl:for-each select="faq">
            <div class="faq">
               <div class="question">
               <xsl:number value="$category_num" format="1" />

               <xsl:number value="position()" format=".1" />

                
               <xsl:value-of select="question" />
               </div>

               <div id="answer" class="answer">
                  <xsl:apply-templates select="answer" />
               </div>
            </div>
         </xsl:for-each>
      </xsl:for-each>
   </xsl:template>
</xsl:stylesheet>

