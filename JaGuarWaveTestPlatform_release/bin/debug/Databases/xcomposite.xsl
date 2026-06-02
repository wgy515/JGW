<!-- 
/*!
 *********************************************************************
 *
 *  Copyright (c) 2002-2015 Qualcomm Technologies, Inc. 
 *
 *  Purpose : Perform XInclude expansion of source files
 *
 *********************************************************************
<b>Version History:</b>
\verbatim
\endverbatim
 *********************************************************************
 */
-->

<!--
 Limitation:
   There is no XPointer support in MSXML, so we don't support xpointer as part of the document reference.
   That said, we may support this in the future, so documents that use this stylesheet should provide an
   xpointer path into the first child node of the document root (this stylesheet will always assume this path).
   
   This is done because all documents included should be valid files by themselves (must have one and only one root node)

   Example:
   **a.xml**
   <NV xmlns:xi="http://www.w3.org/2001/XInclude" xmlns:xlink="http://www.w3.org/1999/xlink">
      <xi:include href="b.xml#xpointer(/NV/*)"/>
   </NV>

   **b.xml**
   <NV>
      <Item id="1123" val="1"/>
      <Item id="1456" val="2"/>
      <Item id="1789" val="3"/>
   </NV>

   The #xpointer(/NV/*) portion is ignored, but the hard-coded behavior will result in the XPath provided.
-->

<xsl:stylesheet version = '1.0' 
	xmlns:xsl='http://www.w3.org/1999/XSL/Transform' 
	xmlns:xi='http://www.w3.org/2001/XInclude'	>
	
<xsl:output method="xml" indent="yes" encoding="utf-8" version="1" />

<!-- match to root and apply templates to all children -->

<xsl:template match="xi:include">
	<!-- declare a varialbe to hold the nodes of the referenced document -->
	<xsl:variable name="docNodes" select="document(@href)"/>
	<!-- include the doc, but skip the root node -->
	
	<xsl:apply-templates select="$docNodes/*/*"/> 
</xsl:template>

<xsl:param name="indent-increment" select="'     ' " />

<xsl:template match="*" >
<xsl:param name = "indent" select = "'&#x0A;' " />
<!-- copy all non-xinclude nodes -->
<xsl:value-of select="$indent" />

<xsl:copy>
<xsl:copy-of  select="@*" />
<xsl:apply-templates >
<xsl:with-param name="indent" select="concat( $indent, $indent-increment )" />
</xsl:apply-templates>
<xsl:if test="*">
      <xsl:value-of select="$indent" />
</xsl:if> 
</xsl:copy>
</xsl:template>

<xsl:template match="/ |comment() | processing-instruction()" >
<xsl:copy>
<xsl:apply-templates />
</xsl:copy>
</xsl:template>



</xsl:stylesheet>
