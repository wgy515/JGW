<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">

<xsl:template match="/">
  <html>
  <body>
   
   <h1>QRCT's QCN Differencing Tool(V2.1) Result</h1>
<table border="1">
	<tbody>
		<tr>
			<td>QCN1</td><td><xsl:value-of select="DiffResult/QCN1Path"></xsl:value-of></td><td><xsl:value-of select="DiffResult/QCN1DefSrcPath"/></td>
		</tr>
		<tr>
			<td>QCN2</td><td><xsl:value-of select="DiffResult/QCN2Path"></xsl:value-of></td><td><xsl:value-of select="DiffResult/QCN2DefSrcPath"/></td>

		</tr>
		<tr>
			<td>TimeStamp</td><td><xsl:value-of select="DiffResult/timestamp"></xsl:value-of></td><td></td>
		</tr>
	</tbody>
</table>

<h2>Table of Content</h2>
<table border="1">
	<tbody>
		<tr>
			<td><a href="#RFNV_Summary_View">NV/RFNV Summary View</a></td>
		</tr>
		<tr>
			<td><a href="#EFSNV_Summary_View">EFS NV Summary View</a></td>
		</tr>
		<tr>
			<td><a href="#RFNV_Detail_View">NV/RFNV Items Detail View</a></td>
		</tr>
		<tr>
			<td><a href="#EFSNV_Detail_View">EFS NV Items Detail View</a></td>
		</tr>


	</tbody>
</table>


<h2>Legend</h2>
<table border="1">
	<tbody>
		<tr>
			<td bgcolor="#007F00">The NV item is in both QCNs.  Their values are the same. </td>
		<!--</tr>
		<tr>
		-->
			<td bgcolor="#FF0000">The NV item is in both QCNs.  Their values are different. </td>
		<!--
		</tr>
		<tr>
		-->
			<td bgcolor="#00AFFF">The NV item is in only 1 of the QCNs. </td>
		</tr>


	</tbody>
</table>



 
  <h2><a name="#RFNV_Summary_View">NV/RF NV item collection summary</a></h2>
  <table border="1">
    <tr bgcolor="#FFFF00">
      <th>NV ID</th>
	  <th>NAM</th>
	  <th>SIM</th>
      <th>Name</th>
      <th>In QCN 1</th>
      <th>In QCN 2</th>
      <th>same value</th>
    </tr>
    <xsl:for-each select="DiffResult/NvItem">
    <tr>		
      <td><a href="#{@id}"><xsl:value-of select="@id"/></a></td>
	  <td><xsl:value-of select="@index"/></td>
	  <td><xsl:value-of select="@subscriptionid"/></td>
      <xsl:choose>
        <xsl:when test="(@inQCN1='true') and (@inQCN2='true') and (@sameValue='true')">
          <td bgcolor="#007F00">
          <xsl:value-of select="@name"/></td>
        </xsl:when>
        <xsl:when test="(@inQCN1='true') and (@inQCN2='true') and (@sameValue='false')">
  		<td bgcolor="#FF0000">
          <xsl:value-of select="@name"/></td>
        </xsl:when>
        <xsl:otherwise>
        	<td bgcolor="#00AFFF">
          <xsl:value-of select="@name"/></td>
        </xsl:otherwise>
        </xsl:choose>
      <td><xsl:value-of select="@inQCN1"/></td>
      <td><xsl:value-of select="@inQCN2"/></td>
	<td><a href="#{@id}"><xsl:value-of select="@sameValue"/></a></td>
    </tr>
    </xsl:for-each>
  </table>
  <br></br>
  <h2><a name="EFSNV_Summary_View">EFS NV item summary view</a></h2>
  <table border="1">
    <tr bgcolor="#FFFF00">
      <th>EFS Full Path</th>
      <th>In QCN 1</th>
      <th>In QCN 2</th>
      <th>same value</th>
	  <!--
      <th>Value In QCN 1</th>
      <th>Value In QCN 2</th>
	  -->
    </tr>
    <xsl:for-each select="DiffResult/EfsNvItem">
    <tr>		
      
      <xsl:choose>
        <xsl:when test="(@inQCN1='true') and (@inQCN2='true') and (@sameValue='true')">
          <td bgcolor="#007F00">
          <xsl:value-of select="@fullpathname"/></td>
        </xsl:when>
        <xsl:when test="(@inQCN1='true') and (@inQCN2='true') and (@sameValue='false')">
  		<td bgcolor="#FF0000">
          <xsl:value-of select="@fullpathname"/></td>
        </xsl:when>
        <xsl:otherwise>
        	<td bgcolor="#00AFFF">
          <xsl:value-of select="@fullpathname"/></td>
        </xsl:otherwise>
        </xsl:choose>
	
      <td><xsl:value-of select="@inQCN1"/></td>
      <td><xsl:value-of select="@inQCN2"/></td>
	<td><a href="#{@fullpathname}"><xsl:value-of select="@sameValue"/></a></td>
<!--    
    <td style="width:200px"><xsl:value-of select="valueInQCN1"/></td>
	<td style="width:200px"><xsl:value-of select="valueInQCN2"/></td>
-->
    </tr>
    </xsl:for-each>
  </table>

  <h2><a name="RFNV_Detail_View">NV/RFNV Items Detail View</a></h2>
  Only blue or red items are shown !
  <xsl:for-each select="DiffResult/NvItem">
	<xsl:choose>
		<xsl:when test="(@sameValue='false')">
			<table border="1">
			    
				<tr><td><a name="{@id}"><xsl:value-of select="@id"/></a></td><td><xsl:value-of select="@name"/></td></tr>
				<tr><td>QCN1</td><td><xsl:value-of select="valueInQCN1"/></td></tr>
				<tr><td>QCN2</td><td><xsl:value-of select="valueInQCN2"/></td></tr>
			</table>
			<br></br>
		</xsl:when>
	</xsl:choose>
	
  </xsl:for-each>
  
  <h2><a name="EFSNV_Detail_View">EFS NV Items Detail View</a></h2>
  Only blue or red items are shown !
  <xsl:for-each select="DiffResult/EfsNvItem">
	<xsl:choose>
		<xsl:when test="(@sameValue='false')">
			<table border="1">
				<tr><td><a name="{@fullpathname}"><xsl:value-of select="@fullpathname"/></a></td></tr>
				<tr><td>QCN1</td><td><xsl:value-of select="valueInQCN1"/></td></tr>
				<tr><td>QCN2</td><td><xsl:value-of select="valueInQCN2"/></td></tr>
			</table>
			<br></br>
		</xsl:when>
	</xsl:choose>
	
  </xsl:for-each>
  </body>
  </html>
</xsl:template>

</xsl:stylesheet>
