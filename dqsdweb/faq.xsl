<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:template match="/">
		<HTML>
			<HEAD>
				<TITLE>Dave's Quick Search DeskBar FAQ</TITLE>
				<LINK REL="stylesheet" TYPE="text/css" HREF="faq.css" />
				<script>
					<xsl:comment>
					function toggleVisible()
					{
						var answer = event.srcElement.parentElement.parentElement.all['answer'];
						var toggle = event.srcElement.parentElement.all['toggle'];
						if ( answer )
						{
							answer.style.display = ( answer.style.display == "none" ? "block" : "none" );
							toggle.innerText = ( answer.style.display == "none" ? "+" : "-" );
						}
					}
					</xsl:comment>
				</script>
			</HEAD>
			<BODY>
				<xsl:apply-templates select="/faqs/category"/>
       </BODY>
		</HTML>
	</xsl:template>
	
	<xsl:template match="category">
		<div class="category">
			<xsl:value-of select="@name"/>
		</div>
		<xsl:apply-templates select="faq"/>
	</xsl:template>
	
	<xsl:template match="faq">
		<div class="faq">
			<div class="question">
				<a href="#" class="toggle" id="toggle" onclick="toggleVisible();">+</a><xsl:value-of select="question"/>
			</div>
			<div id="answer" class="answer" style="display: none">
				<xsl:value-of select="answer"/>
			</div>
		</div>
	</xsl:template>

</xsl:stylesheet>
